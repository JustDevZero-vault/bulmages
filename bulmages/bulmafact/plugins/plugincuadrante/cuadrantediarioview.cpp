/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>
#include <QDrag>
#include <QMenu>

#include <fstream>

#include "cuadrantediarioview.h"
#include "cuadrante1view.h"
#include "company.h"
#include "configuracion.h"
#include "plugins.h"

#include "funcaux.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
/**
\param comp
\param parent
**/
CuadranteDiarioView::CuadranteDiarioView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "CuadranteDiarioView::CuadranteDiarioView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        mui_listvalidaciones->setEmpresaBase ( empresaBase() );


        /// Lanzamos los plugins.
        if ( g_plugins->lanza ( "CuadranteDiarioView_CuadranteDiarioView", this ) ) return;

        setTitleName ( tr ( "Almacen" ) );
        setDBTableName ( "almacen" );

        mui_listtrabajadores->setDragEnabled ( TRUE );
        mui_cuadrante->setAcceptDrops ( TRUE );
// mui_cuadrante->verticalHeader()->hide();

        inicializaTrabajadores();
  
    mui_listvalidaciones->setDBTableName ( "valasterisk" );
    mui_listvalidaciones->setDBCampoId ( "idvalasterisk" );
    mui_listvalidaciones->addSHeader ( "fechavalasterisk", DBCampo::DBdate, DBCampo::DBNotNull , SHeader::DBNone, QApplication::translate ( "TrabajadorView", "Fecha" ) );
    mui_listvalidaciones->addSHeader ( "horavalasterisk", DBCampo::DBvarchar, DBCampo::DBNotNull , SHeader::DBNone, QApplication::translate ( "TrabajadorView", "Hora" ) );
    mui_listvalidaciones->addSHeader ( "nomalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , QApplication::translate ( "TrabajadorView", "Almacén" ) );
    mui_listvalidaciones->addSHeader ( "nomtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , QApplication::translate ( "TrabajadorView", "Almacén" ) );
    mui_listvalidaciones->addSHeader ( "apellidostrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , QApplication::translate ( "TrabajadorView", "Almacén" ) );

/*
    mui_listvalidaciones->addSHeader ( "aperturaalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite , QApplication::translate ( "TrabajadorView", "Apertura Mañanas" ) );
    mui_listvalidaciones->addSHeader ( "cierrealmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite , QApplication::translate ( "TrabajadorView", "Cierre Mañanas" ) );
    mui_listvalidaciones->addSHeader ( "apertura1almacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite , QApplication::translate ( "TrabajadorView", "Apertura Tardes" ) );
    mui_listvalidaciones->addSHeader ( "cierre1almacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite , QApplication::translate ( "TrabajadorView", "Cierre Tardes" ) );
    mui_listvalidaciones->addSHeader ( "archvalasterisk", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNoWrite , QApplication::translate ( "TrabajadorView", "Audicion" ) );
    mui_listvalidaciones->addSHeader ( "idalmacen", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoWrite , QApplication::translate ( "TrabajadorView", "Id Almacen" ) );
    mui_listvalidaciones->addSHeader ( "idvalasterisk", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, QApplication::translate ( "TrabajadorView", "ID Validacion" ) );
    mui_listvalidaciones->addSHeader ( "idtrabajador", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView | SHeader::DBNoWrite, QApplication::translate ( "TrabajadorView", "ID Trabajador" ) );
*/
    mui_listvalidaciones->setinsercion ( FALSE );
    mui_listvalidaciones->setDelete ( FALSE );
    mui_listvalidaciones->setSortingEnabled ( FALSE );
    dialogChanges_setQObjectExcluido ( mui_listvalidaciones->mui_list );


        inicializaCuadrante ( QDate::currentDate() );


        meteWindow ( windowTitle(), this, FALSE );
    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al iniciar el cuadrante" ) );
    } // end try
    _depura ( "END CuadranteDiarioView::CuadranteDiarioView", 0 );
}


/** No precisa acciones adicionales en el destructor.
*/
///
/**
**/
CuadranteDiarioView::~CuadranteDiarioView()
{
    _depura ( "CuadranteDiarioView::~CuadranteDiarioView", 0 );
    guardaconfig();
    _depura ( "END CuadranteDiarioView::~CuadranteDiarioView", 0 );
}


/** Inicializa la selección de trabajadores
*/
/**
**/
void CuadranteDiarioView::inicializaTrabajadores()
{
    _depura ( "CuadranteDiarioView::inicializaTrabajadores", 0 );
    mui_listtrabajadores->clear();
    mui_listtrabajadores->setColumnCount ( 2 );
    mui_listtrabajadores->hideColumn ( 1 );
    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM tipotrabajo" );
    if ( cur ) {
        while ( !cur->eof() ) {
            QTreeWidgetItem * it = new QTreeWidgetItem ( mui_listtrabajadores );
            it->setText ( 0, cur->valor ( "nomtipotrabajo" ) );
            it->setTextColor ( 0, QColor ( "#FF0000" ) );

            /// Cargamos los trabajadores correspondientes a este tipo de trabajo.
            cursor2 *curtrab = empresaBase() ->cargacursor ( "SELECT * FROM trabajador WHERE idtipotrabajo = " + cur->valor ( "idtipotrabajo" ) + "ORDER BY nomtrabajador, apellidostrabajador" );
            if ( curtrab ) {
                while ( !curtrab->eof() ) {
                    QTreeWidgetItem * itt = new QTreeWidgetItem ( it );
                    itt->setText ( 0, curtrab->valor ( "nomtrabajador" ) + " " + curtrab->valor ( "apellidostrabajador" ) );
                    itt->setText ( 1, curtrab->valor ( "idtrabajador" ) );
                    itt->setTextColor ( 0, QColor ( "#000000" ) );
                    curtrab->siguienteregistro();
                } // end while
                delete curtrab;
            } // end if

            cur->siguienteregistro();
        } // end if
        delete cur;
    } // end if
    _depura ( "END CuadranteDiarioView::inicializaTrabajadores", 0 );
}


/** Inicializa la selección de trabajadores
*/
/**
\param dateorig
\return
**/
void CuadranteDiarioView::inicializaCuadrante ( const QDate &dateorig )
{
    _depura ( "CuadranteDiarioView::inicializaCuadrante", 0 );
    QString diassem[7];
    diassem[0] = tr("Lunes");
    diassem[1] = tr("Martes");
    diassem[2] = tr("Miercoles");
    diassem[3] = tr("Jueves");
    diassem[4] = tr("Viernes");
    diassem[5] = tr("Sabado");
    diassem[6] = tr("Domingo");


    try {
        /// Si el cuadrante ha sido manipulado guardamos las configuracion del mismo.
        if ( mui_cuadrante->rowCount() != 0 )
            guardaconfig();

        mui_cuadrante->clear();

        cursor2 *cur = empresaBase() ->cargacursor ( "SELECT idalmacen, tipoalmacen, nomalmacen FROM almacen ORDER BY tipoalmacen DESC, nomalmacen" );
        if ( !cur ) throw - 1;


        mui_cuadrante->setRowCount ( cur->numregistros() );
        mui_cuadrante->setColumnCount ( 1 );

        QStringList headers;
	headers << diassem[dateorig.dayOfWeek()-1];
        mui_cuadrante->setHorizontalHeaderLabels ( headers );


        int row = 0;
        while ( !cur->eof() ) {
            /// Establecemos la altura de los campos.
            mui_cuadrante->setRowHeight ( row, 300 );

 
            QDate date = dateorig;

            for ( int column = 0; column < 1; column ++ ) {

                mui_cuadrante->setColumnWidth ( column, 250 );

                CuadranteQTextDocument *newItem = new CuadranteQTextDocument ( empresaBase(), mui_cuadrante );

                newItem->setAlmFecha ( cur->valor ( "idalmacen" ), date );

                mui_cuadrante->setCellWidget ( row, column, newItem );

                date = date.addDays ( 1 );
            } // end for
            cur->siguienteregistro();
            row++;
        } // end while

        /// Hacemos la carga de las validaciones del dia.
	QString query1 = "SELECT * FROM valasterisk LEFT JOIN trabajador ON trabajador.idtrabajador = valasterisk.idtrabajador ";
	query1 += " LEFT JOIN almacen ON valasterisk.idalmacen = almacen.idalmacen WHERE fechavalasterisk = '" + mui_calendario->selectedDate().toString ( "dd/MM/yyyy" ) + "'";
        mui_listvalidaciones->cargar ( query1 );


        cargaconfig();
        _depura ( "CuadranteDiarioView::inicializaCuadrante", 0 );
    } catch ( ... ) {
        _depura ( "Error en la carga del calendario", 2 );
        return;
    }
}


///
/**
\param date
**/
void CuadranteDiarioView::on_mui_calendario_clicked ( const QDate &date )
{
    _depura ( "CuadranteDiarioView::on_mui_calendario_clicked", 0, date.toString ( "dd/MM/yyyy" ) );
    inicializaCuadrante ( date );
}


///
/**
\param item
**/
void CuadranteDiarioView::on_mui_listtrabajadores_itemDoubleClicked ( QTreeWidgetItem *item, int )
{
    _depura ( "elementos dobleclickados", 0 );
    QString item1 = item->text ( 1 );

    QList<QTableWidgetSelectionRange> selectionranges = mui_cuadrante->selectedRanges();
    for ( int i = 0; i < selectionranges.size(); ++i ) {
        for ( int x = selectionranges.at ( i ).topRow(); x <= selectionranges.at ( i ).bottomRow(); x++ ) {
            for ( int y = selectionranges.at ( i ).leftColumn(); y <= selectionranges.at ( i ).rightColumn(); y++ ) {
                CuadranteQTextDocument *newItem = ( CuadranteQTextDocument * ) mui_cuadrante->cellWidget ( x, y );
                newItem->addTrabajador ( item1 );
            } // end for
        } // end for
    }
}


///
/**
**/
void CuadranteDiarioView::on_mui_editar_clicked()
{
    _depura ( "editar cuadrante", 0 );
    Cuadrante1View *cuad = new Cuadrante1View ( empresaBase(), 0 );
    empresaBase() ->pWorkspace() ->addWindow ( cuad );
    cuad->show();
    CuadranteQTextDocument *newItem = ( CuadranteQTextDocument * ) mui_cuadrante->cellWidget ( mui_cuadrante->currentRow(), mui_cuadrante->currentColumn() );
    newItem->connect ( cuad, SIGNAL ( save() ), newItem, SLOT ( refresh() ) );
    cuad->cargar ( newItem->idcuadrante() );
}


///
/**
\param pos
**/
void CuadranteDiarioView::on_mui_calendario_customContextMenuRequested ( const QPoint & pos )
{
    _depura ( "CuadranteDiarioView::on_mui_calendario_customContextMenuRequested", 0 );
    QMenu *popup = new QMenu ( mui_calendario );

    popup->addSeparator();
    QAction *norm = popup->addAction ( tr ( "Normal" ) );
    QAction *fiesta = popup->addAction ( tr ( "Fiesta General" ) );

    QAction *opcion = popup->exec ( mapToGlobal ( pos ) );
    if ( opcion == norm ) {
        empresaBase() ->begin();
        empresaBase() ->ejecuta ( "UPDATE CUADRANTE SET fiestacuadrante = FALSE WHERE fechacuadrante = '" + mui_calendario->selectedDate().toString ( "dd/MM/yyyy" ) + "'" );
        empresaBase() ->commit();
    } // end if

    if ( opcion == fiesta ) {
        empresaBase() ->begin();
        empresaBase() ->ejecuta ( "UPDATE CUADRANTE SET fiestacuadrante = TRUE WHERE fechacuadrante = '" + mui_calendario->selectedDate().toString ( "dd/MM/yyyy" ) + "'" );
        empresaBase() ->commit();
    } // end if

    inicializaCuadrante ( mui_calendario->selectedDate() );
    _depura ( "END CuadranteDiarioView::on_mui_calendario_customContextMenuRequested", 0 );
}


///
/**
**/
void CuadranteDiarioView::on_mui_actualizar_clicked()
{
    _depura ( "CuadranteDiarioView::on_mui_actualizar_clicked", 0 );
    inicializaTrabajadores();
    inicializaCuadrante ( mui_calendario->selectedDate() );
    _depura ( "CuadranteDiarioView::on_mui_actualizar_clicked", 0 );
}


///
/**
**/
void CuadranteDiarioView::on_mui_limpiar_clicked()
{
    _depura ( "CuadranteDiarioView::on_mui_limpiar_clicked", 0 );
    QDate date = mui_calendario->selectedDate().addDays ( -mui_calendario->selectedDate().dayOfWeek() + 1 );
    QDate datefin = date.addDays ( 6 );
    QString query = "DELETE FROM horario WHERE idcuadrante IN (SELECT idcuadrante FROM cuadrante WHERE fechacuadrante >= '" + date.toString ( "dd/MM/yyyy" ) + "' AND fechacuadrante <='" + datefin.toString ( "dd/MM/yyyy" ) + "')";
    empresaBase() ->ejecuta ( query );
    query = "DELETE FROM cuadrante WHERE fechacuadrante >= '" + date.toString ( "dd/MM/yyyy" ) + "' AND fechacuadrante <='" + datefin.toString ( "dd/MM/yyyy" ) + "'";
    empresaBase() ->ejecuta ( query );
    on_mui_actualizar_clicked();
    _depura ( "CuadranteDiarioView::on_mui_limpiar_clicked", 0 );
}



///
/**
**/
void CuadranteDiarioView::on_mui_duplicar_clicked()
{
    _depura ( "CuadranteDiarioView::on_mui_limpiar_clicked", 0 );
    for ( QDate date = mui_calendario->selectedDate().addDays ( -mui_calendario->selectedDate().dayOfWeek() + 1 )
                       ; date <= mui_calendario->selectedDate().addDays ( -mui_calendario->selectedDate().dayOfWeek() + 7 )
            ; date = date.addDays ( 1 ) ) {
        QDate fechaant = date.addDays ( -7 );
        QString query = "SELECT * FROM cuadrante WHERE fechacuadrante = '" + fechaant.toString ( "dd/MM/yyyy" ) + "'";
        _depura ( query, 2 );
        cursor2 *cur = empresaBase() ->cargacursor ( query );
        while ( !cur->eof() ) {
            query = "UPDATE cuadrante SET ";
            query += " comentcuadrante = '" + cur->valor ( "comentcuadrante" ) + "'";
            query += ", aperturacuadrante = " + ( ( cur->valor ( "aperturacuadrante" ) == "" ) ? "NULL" : "'" + cur->valor ( "aperturacuadrante" ) + "'" );
            query += ", cierrecuadrante = " + ( ( cur->valor ( "cierrecuadrante" ) == "" ) ? "NULL" : "'" + cur->valor ( "cierrecuadrante" ) + "'" );
            query += ", apertura1cuadrante = " + ( ( cur->valor ( "apertura1cuadrante" ) == "" ) ? "NULL" : "'" + cur->valor ( "apertura1cuadrante" ) + "'" );
            query += ", cierre1cuadrante = " + ( ( cur->valor ( "cierre1cuadrante" ) == "" ) ? "NULL" : "'" + cur->valor ( "cierre1cuadrante" ) + "'" );
            query += ", fiestacuadrante = '" + cur->valor ( "fiestacuadrante" ) + "'";
            query += " WHERE fechacuadrante = '" + date.toString ( "dd/MM/yyyy" ) + "' AND idalmacen = " + cur->valor ( "idalmacen" );
            empresaBase() ->ejecuta ( query );

            cursor2 *cur1 = empresaBase() ->cargacursor ( "SELECT * FROM cuadrante WHERE fechacuadrante = '" + date.toString ( "dd/MM/yyyy" ) + "' AND idalmacen = " + cur->valor ( "idalmacen" ) );
            QString idcuadrante = cur1->valor ( "idcuadrante" );
            delete cur1;

            cursor2 *cur2 = empresaBase() ->cargacursor ( "SELECT * FROM horario WHERE idcuadrante=" + cur->valor ( "idcuadrante" ) );
            while ( !cur2->eof() ) {
                query = "INSERT INTO horario (idtrabajador, idcuadrante, horainhorario, horafinhorario) VALUES (";
                query += cur2->valor ( "idtrabajador" );
                query += "," + idcuadrante;
                query += ",'" + cur2->valor ( "horainhorario" ) + "'";
                query += ",'" + cur2->valor ( "horafinhorario" ) + "'";
                query += ")";
                empresaBase() ->ejecuta ( query );
                cur2->siguienteregistro();
            } // end while
            delete cur2;

            cur->siguienteregistro();
        } // end while
        delete cur;


    } // end for
    on_mui_actualizar_clicked();
    _depura ( "CuadranteDiarioView::on_mui_limpiar_clicked", 0 );
}


///
/**
**/
void CuadranteDiarioView::on_mui_imprimir_clicked()
{
    _depura ( "CuadranteDiarioView::on_mui_imprimir_clicked", 0 );

    QString archivo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "cuadrante.rml";
    QString archivod = confpr->valor ( CONF_DIR_USER ) + "cuadrante.rml";
    QString archivologo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system ( archivo.toAscii().constData() );

    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#endif

    QFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt = "";


    ///Ponemos la tabla
    fitxersortidatxt += "<para>Cuadrante</para>\n";
    fitxersortidatxt += "<blockTable style=\"tablacontenido\" colWidths=\"6cm, 6cm, 6cm, 6cm, 6cm, 6cm, 4cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "    <td>Lunes</td>\n";
    fitxersortidatxt += "    <td>Martes</td>\n";
    fitxersortidatxt += "    <td>Miercoles</td>\n";
    fitxersortidatxt += "    <td>Jueves</td>\n";
    fitxersortidatxt += "    <td>Viernes</td>\n";
    fitxersortidatxt += "    <td>Sabado</td>\n";
    fitxersortidatxt += "    <td>Domingo</td>\n";
    fitxersortidatxt += "</tr>\n";


// ================================================================

    /// Buscamos el Lunes de la Semana
//    QDate date = mui_calendario->selectedDate().addDays(-mui_calendario->selectedDate().dayOfWeek() + 1);

    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT idalmacen FROM almacen" );
    if ( !cur ) throw - 1;

    mui_cuadrante->setRowCount ( cur->numregistros() );
    mui_cuadrante->setColumnCount ( 7 );

    int row = 0;
    while ( !cur->eof() ) {
        fitxersortidatxt += "<tr>\n";
        for ( int column = 0; column < 7; column ++ ) {
            CuadranteQTextDocument *newItem = ( CuadranteQTextDocument * ) mui_cuadrante->cellWidget ( row, column );
            fitxersortidatxt += newItem->impresion();
        } // end for
        cur->siguienteregistro();
        row++;
        fitxersortidatxt += "</tr>\n";
    } // end while

    fitxersortidatxt += "</blockTable>\n";

    buff.replace ( "[story]", fitxersortidatxt );

    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << buff;
        file.close();
    } // end if
    invocaPDF ( "cuadrante" );
    _depura ( "END CuadranteDiarioView::on_mui_imprimir_clicked", 0 );

}


/// Guardamos el archivo de configuracion.
/**
**/
void CuadranteDiarioView::guardaconfig()
{
    _depura ( "CuadranteDiarioView::guardaconfig", 0 );
    QString aux = "";
    QFile file ( confpr->valor ( CONF_DIR_USER ) + "cuadrantedcfn.cfn" );
    /// Guardado del orden y de configuraciones varias.
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );

        /// Guardado del ancho y alto de las columnas
        for ( int i = 0; i < mui_cuadrante->columnCount(); i++ ) {
            stream << mui_cuadrante->columnWidth ( i ) << "\n";
        } // end for

        /// Guardado del ancho y alto de las columnas
        for ( int i = 0; i < mui_cuadrante->rowCount(); i++ ) {
            stream << mui_cuadrante->rowHeight ( i ) << "\n";
        } // end for

        file.close();
    } // end if
    _depura ( "END CuadranteDiarioView::guardaconfig", 0 );
}


///
/**
**/
void CuadranteDiarioView::cargaconfig()
{
    _depura ( "CuadranteDiarioView::cargaconfig", 0 );
    QFile file ( confpr->valor ( CONF_DIR_USER ) + "cuadrantedcfn.cfn" );
    QString line;
    int error = 1;
    if ( file.open ( QIODevice::ReadOnly ) ) {
        error = 0;
        QTextStream stream ( &file );
        /// Establecemos la columna de ordenaci&oacute;n
        QString linea = "";


        /// Establecemos el ancho de las columnas.
        for ( int i = 0; i < mui_cuadrante->columnCount(); i++ ) {
            linea = stream.readLine();
            if ( linea.toInt() > 0 ) {
                mui_cuadrante->setColumnWidth ( i, linea.toInt() );
            } else {
                mui_cuadrante->setColumnWidth ( i, 400 );
                error = 1;
            } // end if
        } // end for

        /// Establecemos el ancho de las columnas.
        for ( int i = 0; i < mui_cuadrante->rowCount(); i++ ) {
            linea = stream.readLine();
            if ( linea.toInt() > 0 ) {
                mui_cuadrante->setRowHeight ( i, linea.toInt() );
            } else {
                mui_cuadrante->setRowHeight ( i, 250 );
                error = 1;
            } // end if
        } // end for
    } // end if
    /// Si se ha producido alg&uacute;n error en la carga hacemos un maquetado autom&aacute;tico.
    if ( error ) {
        mui_cuadrante->resizeColumnsToContents();
        mui_cuadrante->resizeRowsToContents();
    }

    file.close();
    _depura ( "END CuadranteDiarioView::cargaconfig", 0 );
}
