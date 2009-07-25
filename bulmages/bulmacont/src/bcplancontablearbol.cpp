/***************************************************************************
 *   Copyright (C) 2005 by Ricardo Diaz                                    *
 *   richard@galdi.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "bcplancontablearbol.h"

///
/**
**/
BcPlanContableArbol::BcPlanContableArbol()
{
    _depura ( "BcPlanContableArbol::BcPlanContableArbol", 0 );

    _depura ( "END BcPlanContableArbol::BcPlanContableArbol", 0 );
}


///
/**
**/
BcPlanContableArbol::~BcPlanContableArbol()
{
    _depura ( "BcPlanContableArbol::~BcPlanContableArbol", 0 );

    _depura ( "END BcPlanContableArbol::~BcPlanContableArbol", 0 );
}


///
/**
\param ramas
**/
void BcPlanContableArbol::nuevaRama ( BlDbRecordSet *ramas )
{
    _depura ( "BcPlanContableArbol::nuevaRama", 0 );

    /// Rellenamos los valores de inicializacion para una hoja.
    hoja = new tipohoja;
    hoja->idcuenta = atoi ( ramas->valor ( "idcuenta" ).toAscii().constData() );
    hoja->codigo = QString ( ramas->valor ( "codigo" ) );
    hoja->descripcion = QString ( ramas->valor ( "descripcion" ) );
    hoja->saldoant = hoja->debe = hoja->haber = hoja->saldo = hoja->debeej = hoja->haberej = hoja->saldoej = BlFixed ( "0.00" );
    hoja->numapuntes = 0;
    hoja->ramas = NULL;
    raiz << hoja;

    _depura ( "END BcPlanContableArbol::nuevaRama", 0 );
}


///
/**
\param ramas
**/
void BcPlanContableArbol::inicializa ( BlDbRecordSet *ramas )
{
    _depura ( "BcPlanContableArbol::inicializa", 0 );

    QString padre;
    tiporama* guia = NULL;
    ramas->firstRecord();
    /// Hay que construir el arbol con todas sus hojitas (cuentas).
    for (int i = 0; i < raiz.size(); ++i ) {
        padre = raiz.at ( i )->codigo;
        /// Creamos las primeras hojas en la ra&iacute;z y sintetizamos el resto
        /// recursivamente (si es que existen aun m&aacute;s hojas hijas).
        while ( !ramas->eof() && ramas->valor ( "codigo" ).startsWith ( padre ) ) {
            if ( ramas->valor ( "nivel" ).toInt() > 2 ) {
                /// Creamos una hoja.
                hoja = new tipohoja;
                hoja->idcuenta = ramas->valor ( "idcuenta" ).toInt();
                hoja->codigo = ramas->valor ( "codigo" );
                hoja->descripcion = ramas->valor ( "descripcion" );
                hoja->saldoant = hoja->debe = hoja->haber = hoja->saldo = hoja->debeej = hoja->haberej = hoja->saldoej = BlFixed ( "0.00" );
                hoja->numapuntes = 0;
                hoja->ramas = NULL;
                rama = new tiporama;
                rama->hoja = hoja;
                rama->sgte = NULL;
                /// Creamos la uni&oacute;n con la ra&iacute;z.
                if ( !raiz.at ( i )->ramas ) {
                    raiz.at ( i )->ramas = guia = rama;
                } else { /// Avanzamos el puntero a la siguiente supuesta rama.
                    guia->sgte = rama;
                    guia = guia->sgte;
                } // end if
                /// Buscamos si adem&aacute;s de esta hoja hay m&aacute;s ramas que
                /// crezcan, en tal caso habr&aacute; que sintetizarlas.
                SintetizarRamas ( &ramas, & ( hoja->ramas ) );
            } else {
                ramas->nextRecord();
            } // end if
        } // end while
    } // end for

    _depura ( "END BcPlanContableArbol::inicializa", 0 );
}


///
/**
\param cuentas
\param ramas
**/
void BcPlanContableArbol::SintetizarRamas ( BlDbRecordSet **cuentas, tiporama **ramas )
{
    _depura ( "BcPlanContableArbol::SintetizarRamas", 0 );

    tiporama *guia, *rama;
    tipohoja *hoja;
    int nivel;
    BlDbRecordSet *ptrcuentas = *cuentas;
    nivel = atoi ( ptrcuentas->valor ( "nivel" ).toAscii().constData() );
    ptrcuentas->nextRecord();
    guia = NULL;
    while ( !ptrcuentas->eof() && ( atoi ( ptrcuentas->valor ( "nivel" ).toAscii().constData() ) > nivel ) ) {
        /// Reservamos un huequecito de memoria para almacenar los datos de la rama.
        rama = new tiporama;
        if ( !guia ) {
            *ramas = guia = rama;
        } else {
            guia->sgte = rama;
            guia = guia->sgte;
        } // end if
        hoja = new tipohoja; /// Idem para una hojita.
        hoja->idcuenta = atoi ( ptrcuentas->valor ( "idcuenta" ).toAscii().constData() );
        hoja->codigo = ptrcuentas->valor ( "codigo" );
        hoja->descripcion = ptrcuentas->valor ( "descripcion" );
        hoja->saldoant = hoja->debe = hoja->haber = hoja->saldo = hoja->debeej = hoja->haberej = hoja->saldoej = BlFixed ( "0.00" );
        hoja->numapuntes = 0;
        hoja->ramas = NULL;
        guia->hoja = hoja;
        guia->sgte = NULL;
        /// Pero a&uacute;n puede haber m&aacute;s ramas que crezcan, en tal caso habr&aacute;
        /// que sintetizarlas
        SintetizarRamas ( &ptrcuentas, & ( hoja->ramas ) );
    } // end while

    _depura ( "END BcPlanContableArbol::SintetizarRamas", 0 );
}


///
/**
\param cuenta
**/
void BcPlanContableArbol::actualizaHojas ( BlDbRecordSet *cuenta )
{
    _depura ( "BcPlanContableArbol::actualizaHojas", 0 );

    int i = 0;
    bool actualizado = false;
    tipohoja *hojaraiz;
    QString cuentapadre = cuenta->valor ( "codigo" ).left ( 2 );

    /// Buscamos la rama del arbol que es la padre la cuenta pasada.
    for ( ; i < raiz.size(); ++i ) {
        hojaraiz = raiz.at ( i );
        if ( cuentapadre == hojaraiz->codigo )
            break;
    } // end for

    /// Si la rama principal se ha encontrado y tiene hijos, se actualiza
    if ( i < raiz.size() && hojaraiz->ramas ) {
        ActualizarHoja ( & ( hojaraiz->ramas ), cuenta, &actualizado );
        if ( actualizado ) {
            hojaraiz->saldoant = hojaraiz->saldoant + BlFixed ( cuenta->valor ( "saldoant" ) );
            hojaraiz->debe = hojaraiz->debe + BlFixed ( cuenta->valor ( "debe" ) );
            hojaraiz->haber = hojaraiz->haber + BlFixed ( cuenta->valor ( "haber" ) );
            hojaraiz->saldo = hojaraiz->saldo + BlFixed ( cuenta->valor ( "saldo" ) );
            hojaraiz->debeej = hojaraiz->debeej + BlFixed ( cuenta->valor ( "debeej" ) );
            hojaraiz->haberej = hojaraiz->haberej + BlFixed ( cuenta->valor ( "haberej" ) );
            hojaraiz->saldoej = hojaraiz->saldoej + BlFixed ( cuenta->valor ( "saldoej" ) );
            hojaraiz->numapuntes += cuenta->valor ( "numapuntes" ).toInt();
        } // end if
    } // end if

    _depura ( "END BcPlanContableArbol::actualizaHojas", 0 );
}


///
/**
\param ramaraiz
\param cuenta
\param actualizado
**/
void BcPlanContableArbol::ActualizarHoja ( tiporama** ramaraiz, BlDbRecordSet* cuenta, bool* actualizado )
{
    _depura ( "BcPlanContableArbol::ActualizarHoja", 0 );

    tiporama* rama = *ramaraiz;
    int idcuenta = cuenta->valor ( "idcuenta" ).toInt();

    /// Buscamos por cada una de las ramas.
    while ( rama && ! ( *actualizado ) ) {
        if ( rama->hoja->idcuenta == idcuenta ) {
            /// Ponemos los valores obtenidos de la BD.
            rama->hoja->saldoant = BlFixed ( cuenta->valor ( "saldoant" ) );
            rama->hoja->debe = BlFixed ( cuenta->valor ( "debe" ) );
            rama->hoja->haber = BlFixed ( cuenta->valor ( "haber" ) );
            rama->hoja->saldo = BlFixed ( cuenta->valor ( "saldo" ) );
            rama->hoja->debeej = BlFixed ( cuenta->valor ( "debeej" ) );
            rama->hoja->haberej = BlFixed ( cuenta->valor ( "haberej" ) );
            rama->hoja->saldoej = BlFixed ( cuenta->valor ( "saldoej" ) );
            rama->hoja->numapuntes = cuenta->valor ( "numapuntes" ).toInt();
            _depura ( "BcPlanContableArbol::ActualizarHoja", 2, cuenta->valor ( "codigo" ) + " " + QString::number ( rama->hoja->numapuntes ) );
            *actualizado = true;
        } else {
            if ( rama->hoja->ramas ) {
                ActualizarHoja ( & ( rama->hoja->ramas ), cuenta, & ( *actualizado ) );
                /// A la vuelta, actualizamos los valores si alguna hoja fue actualizada.
                if ( *actualizado ) {
                    rama->hoja->saldoant = rama->hoja->saldoant + BlFixed ( cuenta->valor ( "saldoant" ) );
                    rama->hoja->debe = rama->hoja->debe + BlFixed ( cuenta->valor ( "debe" ) );
                    rama->hoja->haber = rama->hoja->haber + BlFixed ( cuenta->valor ( "haber" ) );
                    rama->hoja->saldo = rama->hoja->saldo + BlFixed ( cuenta->valor ( "saldo" ) );
                    rama->hoja->debeej = rama->hoja->debeej + BlFixed ( cuenta->valor ( "debeej" ) );
                    rama->hoja->haberej = rama->hoja->haberej + BlFixed ( cuenta->valor ( "haberej" ) );
                    rama->hoja->saldoej = rama->hoja->saldoej + BlFixed ( cuenta->valor ( "saldoej" ) );
                    rama->hoja->numapuntes += cuenta->valor ( "numapuntes" ).toInt();
                } // end if
            } // end if
        } // end if
        rama = rama->sgte;
    } // end while

    _depura ( "END BcPlanContableArbol::ActualizarHoja", 0 );
}


///
/**
**/
void BcPlanContableArbol::inicia()
{
    _depura ( "BcPlanContableArbol::inicia", 0 );

    /// Cuando se inicia un recorrido por el BcPlanContableArbol, se resetea el que apunta a la hoja activa.
    hojaactiva = NULL;

    _depura ( "END BcPlanContableArbol::inicia", 0 );
}


/// Busca el Siguiente elemento que no sea descartable.
/**
\param nivel
\param superiores
\return
**/
// bool BcPlanContableArbol::deshoja ( unsigned int nivel, bool superiores )
// {
//     _depura ( "BcPlanContableArbol::deshoja", 0 );
//     unsigned int i;
//     bool deshojada = false;
//     if ( nivel > 2 ) {
//         /// Primero averiguamos en qu&eacute; ra&iacute;z deshojamos la &uacute;ltima
//         /// vez para continuar por ah&iacute;
//         QString raizcandidata = hojaactiva.left ( 2 );
//         if ( hojaactiva == QString ( "10" ) )
//             i = 0;
//         else
//             for ( i = 0; ( i < raiz.size() && raiz.at(i)->codigo != raizcandidata ); ++i );
//         /// Miramos si hay hoja candidata que deshojar.
//  while ( !deshojada && i < raiz.size() ) {
//   /// Si hay que sacar los niveles superiores tenemos que deshojar tambi&eacute;n
//   /// en el nivel 2.
//   if ( superiores && raiz.at(i)->codigo > hojaactiva && raiz.at(i)->numapuntes > 0 ) {
//    hoja = raiz[i];
//    /// S&oacute;lo deshojamos si ha habido movimientos en la cuenta durante
//    /// el ejercicio.
//    if ( hoja->numapuntes > 0 ) {
//    hojaactiva = hoja->codigo;
//    deshojada = true;
//    } // end if
//   } else {
//    if ( raiz.at(i)->ramas )
//    /// Buscamos por las hojas hijas.
//    Deshojar ( raiz.at(i)->ramas, nivel, superiores, &deshojada );
//   } // end if
//   i++;
//  } // end while
//     } else {
//         if ( hojaactiva == QString ( "10" ) ) {
//             if ( raiz.at(0) ) {
//                 hojaactiva = raiz.at(0)->codigo;
//                 hoja = raiz.at(0);
//                 deshojada = true;
//             } // end if
//         } else {
//             for ( i = 0; i < raiz.size(); ++i ) {
//                 if ( raiz[i]->codigo > hojaactiva ) {
//                     hoja = raiz.at(i);
//                     /// S&oacute;lo deshojamos si ha habido movimientos en la cuenta
//                     /// durante el ejercicio.
//                     if ( hoja->numapuntes > 0 ) {
//                         hojaactiva = hoja->codigo;
//                         deshojada = true;
//                         break;
//                     }
//                 } // end if
//             } // end for
//         } // end if
//     } // end if
//     return deshojada;
//
//     _depura ( "END BcPlanContableArbol::deshoja", 0 );
// }


/// Busca el Siguiente elemento que no sea descartable.
/**
\param nivel
\param superiores
\return
**/
bool BcPlanContableArbol::deshoja ( unsigned int nivel, bool superiores )
{
    _depura ( "BcPlanContableArbol::deshoja", 0 );
    int i;
    bool deshojada = false;
    if ( nivel > 2 ) {
        /// Primero averiguamos en que ra�z deshojamos la �ltima
        /// vez para continuar por ah�
        if ( hojaactiva == NULL )
            i = 0;
        else
            for ( i = 0; ( i < raiz.size() && raiz.at ( i )->codigo != hojaactiva->codigo.left ( 2 ) ); ++i );
        /// Miramos si hay hoja candidata que deshojar.
        while ( !deshojada && i < raiz.size() ) {
            /// Si hay que sacar los niveles superiores tenemos que deshojar tambi�n
            /// en el nivel 2.
            if ( superiores && ( !hojaactiva || ( raiz.at ( i )->codigo > hojaactiva->codigo && raiz.at ( i )->numapuntes > 0 ) ) ) {
                hoja = raiz.at ( i );
                /// S&oacute;lo deshojamos si ha habido movimientos en la cuenta durante
                /// el ejercicio.
                if ( hoja->numapuntes > 0 )
                    deshojada = true;
                hojaactiva = hoja;
            } else {
                if ( raiz.at ( i )->ramas )
                    /// Buscamos por las hojas hijas.
                    Deshojar ( raiz.at ( i )->ramas, nivel, superiores, &deshojada );
            } // end if
            i++;
        } // end while
    } else {
        if ( hojaactiva == NULL ) {
            hoja = raiz.first();
            hojaactiva = hoja;
            deshojada = true;
        } else {
            i = raiz.indexOf ( hojaactiva ) + 1;
            while ( i < raiz.size() && !deshojada ) {
                hoja = raiz.at ( i );
                /// S�lo deshojamos si ha habido movimientos en la cuenta
                /// durante el ejercicio.
                if ( hoja->numapuntes > 0 )
                    deshojada = true;
                hojaactiva = hoja;
                ++i;
            } // end while
        } // end if
    } // end if

    _depura ( "END BcPlanContableArbol::deshoja", 0 );
    return deshojada;
}


///
/**
\param rama
\param nivel
\param superiores
\param deshojada
**/
// void BcPlanContableArbol::Deshojar ( tiporama *rama, unsigned int nivel, bool superiores, bool *deshojada )
// {
//     _depura ( "BcPlanContableArbol::Deshojar", 0 );
//
//     unsigned int nivelhoja = rama->hoja->codigo.length();
//     if ( hojaactiva >= rama->hoja->codigo ) {
//         if ( rama->hoja->ramas && nivelhoja < nivel )
//             Deshojar ( rama->hoja->ramas, nivel, superiores, & ( *deshojada ) );
//         if ( rama->sgte && ! ( *deshojada ) )
//             Deshojar ( rama->sgte, nivel, superiores, & ( *deshojada ) );
//     } else {
//         /// No deshojaremos por aqu&iacute;, busquemos por otra rama.
//         if ( rama->hoja->numapuntes == 0 ) {
//             if ( rama->sgte )
//                 Deshojar ( rama->sgte, nivel, superiores, & ( *deshojada ) );
//         } else { /// Deshojamos
//             if ( superiores ) {
//                 if ( nivelhoja <= nivel ) {
//                     hoja = rama->hoja;
//                     hojaactiva = hoja->codigo;
//                     *deshojada = true;
//                 } else {
//                     if ( rama->sgte )
//                         /// Puede ser que hojas del mismo nivel en el arbol no sean del
//                         /// mismo nivel de cuentas.
//                         /// Ej: 4750001, 4751 y 4752 podrian ser hojas hermanas porque no
//                         /// exista un 4750
//                         /// En este caso, recorreremos tambien sus hojas hermanas por si
//                         /// acaso.
//                         Deshojar ( rama->sgte, nivel, superiores, & ( *deshojada ) );
//                 } // end if
//             } else {
//                 if ( nivelhoja == nivel ) {
//                     hoja = rama->hoja;
//                     hojaactiva = hoja->codigo;
//                     *deshojada = true;
//                 } else {
//                     if ( rama->hoja->ramas && nivelhoja < nivel )
//                         Deshojar ( rama->hoja->ramas, nivel, superiores, & ( *deshojada ) );
//                     if ( rama->sgte && ! ( *deshojada ) )
//                         Deshojar ( rama->sgte, nivel, superiores, & ( *deshojada ) );
//                 } // end if
//             } // end if
//         } // end if
//     } // end if
//
//     _depura ( "END BcPlanContableArbol::Deshojar", 0 );
// }


void BcPlanContableArbol::Deshojar ( tiporama *rama, unsigned int nivel, bool superiores, bool *deshojada )
{
    _depura ( "BcPlanContableArbol::Deshojar", 0 );

    unsigned int nivelhoja = rama->hoja->codigo.length();
    if ( hojaactiva && hojaactiva->codigo >= rama->hoja->codigo ) {
        if ( rama->hoja->ramas && nivelhoja < nivel )
            Deshojar ( rama->hoja->ramas, nivel, superiores, & ( *deshojada ) );
        if ( rama->sgte && ! ( *deshojada ) )
            Deshojar ( rama->sgte, nivel, superiores, & ( *deshojada ) );
    } else {
        /// No deshojemos por aqu�, busquemos por otra rama.
        if ( rama->hoja->numapuntes == 0 ) {
            if ( rama->sgte )
                Deshojar ( rama->sgte, nivel, superiores, & ( *deshojada ) );
        } else { /// Deshojamos
            if ( superiores ) {
                if ( nivelhoja <= nivel ) {
                    hoja = rama->hoja;
                    hojaactiva = hoja;
                    *deshojada = true;
                } else {
                    if ( rama->sgte )
                        /// Puede ser que hojas del mismo nivel en el �rbol no sean del
                        /// mismo nivel de cuentas.
                        /// Ej: 4750001, 4751 y 4752 podrian ser hojas hermanas porque no
                        /// exista un 4750 (aunque sem�nticamente debiera existir)
                        /// En este caso, recorreremos tambien sus hojas hermanas por si
                        /// acaso.
                        Deshojar ( rama->sgte, nivel, superiores, & ( *deshojada ) );
                } // end if
            } else {
                if ( nivelhoja == nivel ) {
                    hoja = rama->hoja;
                    hojaactiva = hoja;
                    *deshojada = true;
                } else {
                    if ( rama->hoja->ramas && nivelhoja < nivel )
                        Deshojar ( rama->hoja->ramas, nivel, superiores, & ( *deshojada ) );
                    if ( rama->sgte && ! ( *deshojada ) )
                        Deshojar ( rama->sgte, nivel, superiores, & ( *deshojada ) );
                } // end if
            } // end if
        } // end if
    } // end if

    _depura ( "END BcPlanContableArbol::Deshojar", 0 );
}


///
/**
\param valor
\return
**/
QString BcPlanContableArbol::hojaActual ( QString valor )
{
    _depura ( "BcPlanContableArbol::hojaActual", 0 );

    QString resultado;
    if ( valor == "saldoant" )
        resultado = hoja->saldoant.toQString ( '.' );
    else if ( valor == "debe" )
        resultado = hoja->debe.toQString ( '.' );
    else if ( valor == "haber" )
        resultado = hoja->haber.toQString ( '.' );
    else if ( valor == "saldo" )
        resultado = hoja->saldo.toQString ( '.' );
    else if ( valor == "debeej" )
        resultado = hoja->debeej.toQString ( '.' );
    else if ( valor == "haberej" )
        resultado = hoja->haberej.toQString ( '.' );
    else if ( valor == "saldoej" )
        resultado = hoja->saldoej.toQString ( '.' );
    else if ( valor == "codigo" )
        resultado = hoja->codigo;
    else if ( valor == "descripcion" )
        resultado = hoja->descripcion;
    else if ( valor == "idcuenta" )
        resultado.setNum ( hoja->idcuenta );
    else
        _depura ( "Campo " + valor + " no encontrado en la hoja del arbol", 2 );

    _depura ( "END BcPlanContableArbol::hojaActual", 0 );
    return resultado;
}


///
/**
\param codigo
\param nivel
\return
**/
bool BcPlanContableArbol::irHoja ( QString codigo, unsigned int nivel )
{
    _depura ( "BcPlanContableArbol::irHoja", 0 );

    inicia();
    while ( deshoja ( nivel, true ) && hojaActual ( "codigo" ) != codigo );
    if ( hojaActual ( "codigo" ) == codigo ) {
        return true;
    } else {
        return false;
    } // end if

    _depura ( "END BcPlanContableArbol::irHoja", 0 );
}


///
/**
\param nivel
\return
**/
QString BcPlanContableArbol::codigoCuentaMayor ( unsigned int nivel )
{
    _depura ( "BcPlanContableArbol::codigoCuentaMayor", 0 );

    QString codigo;

    inicia();
    while ( deshoja ( nivel, true ) );
    codigo = hojaActual ( "codigo" );

    _depura ( "END BcPlanContableArbol::codigoCuentaMayor", 0 );
    return codigo;
}


///
/**
\param cuenta
\param nivel
\return
**/
QString BcPlanContableArbol::hijoMayor ( QString cuenta, unsigned int nivel )
{
    _depura ( "BcPlanContableArbol::hijoMayor", 0 );

    QString codigohoja, codigohijo = cuenta;

    inicia();
    while ( deshoja ( nivel, true ) ) {
        codigohoja = hojaActual ( "codigo" );
        if ( codigohoja.startsWith ( cuenta ) && codigohoja.length() >= codigohijo.length() )
            codigohijo = codigohoja;
    }

    _depura ( "BcPlanContableArbol::hijoMayor", 0 );
    return codigohijo;
}
