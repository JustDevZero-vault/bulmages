/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <QString>

#include "importainteligente.h"
#include "empresa.h"


/// Constructor de la clase que inicializa las variables y coge los datos de base de
/// datos a usar y empresa.
/** \param emp Empresa que construye el objeto y que sirve para inicialización de la
    base de datos.
    Contiene la implementación de la clase importainteligente que sirve para importar
    ficheros de XML a la base de datos de BulmaCont. */
importainteligente::importainteligente(empresa *emp) : QXmlDefaultHandler() {
    fprintf(stderr, "Inicializador de importainteligente\n");
    empresaactual = emp;
    conexionbase = emp->bdempresa();
    tag = "";
    data = "";
}


/// Se ha encontrado en el parser del XML un tag de inicio.
/** La acutaci&oacute;n normal es hacer una inserci&oacute;n del elemento e introducir
    en el Mapa tvalores el identificador encontrado para que al encontrar el tag de fin
    se haga un update del registro con los valores hayados en el XML.
    Se asigna a la variable global tag el nombre del tag para indicar que estamos
    dentro de dicho tag.
    \todo deber&iacute;a usarse una pila en lugar de una simple variable.
    \param qName Nombre del tag encontrado.
    \returns Devuelve TRUE porque no queremos que se detenga la ejecuci&oacute;n del
    parseo aun habiendo encontrado errores. */
bool importainteligente::startElement(const QString&, const QString&, const QString& qName, const QXmlAttributes&) {
    tag = qName;
    QString SQLQuery;
    if (tag == "ainteligente") {
        SQLQuery.sprintf("INSERT INTO ainteligente (descripcion) VALUES ('Elemento importado')\n");
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        SQLQuery = "SELECT max(idainteligente) AS idainteligente FROM ainteligente";
        cursor2 *cur = conexionbase->cargacursor(SQLQuery, "unquerymas");
        conexionbase->commit();
        if (!cur->eof()) {
            tvalores["idainteligente"] = cur->valor("idainteligente");
        } // end if
        delete cur;
    } // end if
    if (tag == "binteligente") {
        SQLQuery.sprintf("INSERT INTO binteligente (idainteligente) VALUES (%s)\n", conexionbase->sanearCadena(tvalores["idainteligente"]).toAscii().constData());
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        SQLQuery = "SELECT max(idbinteligente) AS idbinteligente FROM binteligente";
        cursor2 *cur = conexionbase->cargacursor(SQLQuery, "unquerymas");
        conexionbase->commit();
        if (!cur->eof()) {
            tvalores["idbinteligente"] = cur->valor("idbinteligente");
        } // end if
        delete cur;
    } // end if
    return TRUE;
}


/// Se ha encontrado en el parseo del XML un tag de finalizaci&oacute;n.
/** La funci&oacute;n identificar&aacute; mediante el par&aacute;metro \ref qName
    que tag ha finalizado y har&aacute; los UPDATES necesarios en la
    base de datos con los valores almacenados en el mapa \ref tvalores.
    La variable global tag es puesta a "" para que no se hagan consideraciones
    en los demás elementos de que se est&aacute; en el tag que acaba de cerrarse.
    \todo Se deber&iacute;a vaciar el Mapa \ref tvalores para que no haya propagaci&oacute;n
    de valores en caso de ausencia de elementos en el XML.
    \todo Deber&iacute;a usarse una pila en lugar de la variable global tag.
    \todo Deberian agruparse los UPDATES de la base de datos para que ocupen menos
    tiempo de proceso.
    \param qName Nombre del tag del que se ha encontrado el final.
  */
bool importainteligente::endElement(const QString&, const QString&, const QString& qName) {
    QString SQLQuery;
    if (qName == "ainteligente") {
        SQLQuery.sprintf("UPDATE ainteligente SET descripcion = '%s' WHERE idainteligente = %s\n", tvalores["descripcion"].toAscii().constData(), tvalores["idainteligente"].toAscii().constData());
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
        SQLQuery.sprintf("UPDATE ainteligente SET comentariosasiento = '%s' WHERE idainteligente = %s\n", tvalores["comentariosasiento"].toAscii().constData(), tvalores["idainteligente"].toAscii().constData());
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
    } // end if
    if (qName == "binteligente") {
        SQLQuery.sprintf("UPDATE binteligente SET fecha = '%s' WHERE idbinteligente = %s\n", tvalores["fecha"].toAscii().constData(), tvalores["idbinteligente"].toAscii().constData());
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
        SQLQuery.sprintf("UPDATE binteligente SET conceptocontable = '%s' WHERE idbinteligente=%s\n", tvalores["conceptocontable"].toAscii().constData(), tvalores["idbinteligente"].toAscii().constData());
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
        SQLQuery.sprintf("UPDATE binteligente SET codcuenta = '%s' WHERE idbinteligente = %s\n", tvalores["codcuenta"].toAscii().constData(), tvalores["idbinteligente"].toAscii().constData());
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
        SQLQuery.sprintf("UPDATE binteligente SET descripcion = '%s' WHERE idbinteligente = %s\n", tvalores["descripcionb"].toAscii().constData(), tvalores["idbinteligente"].toAscii().constData());
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
        SQLQuery.sprintf("UPDATE binteligente SET debe = '%s' WHERE idbinteligente = %s\n", tvalores["debe"].toAscii().constData(), tvalores["idbinteligente"].toAscii().constData());
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
        SQLQuery.sprintf("UPDATE binteligente SET haber = '%s' WHERE idbinteligente = %s\n", tvalores["haber"].toAscii().constData(), tvalores["idbinteligente"].toAscii().constData());
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
        SQLQuery.sprintf("UPDATE binteligente SET contrapartida = '%s' WHERE idbinteligente = %s\n", tvalores["contrapartida"].toAscii().constData(), tvalores["idbinteligente"].toAscii().constData());
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
        SQLQuery.sprintf("UPDATE binteligente SET comentario = '%s' WHERE idbinteligente = %s\n", tvalores["comentario"].toAscii().constData(), tvalores["idbinteligente"].toAscii().constData());
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
        SQLQuery.sprintf("UPDATE binteligente SET canal = '%s' WHERE idbinteligente = %s\n", tvalores["canal"].toAscii().constData(), tvalores["idbinteligente"].toAscii().constData());
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
        SQLQuery.sprintf("UPDATE binteligente SET idc_coste = '%s' WHERE idbinteligente = %s\n", tvalores["idc_coste"].toAscii().constData(), tvalores["idbinteligente"].toAscii().constData());
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
        SQLQuery.sprintf("UPDATE binteligente SET marcaconciliacion = '%s' WHERE idbinteligente = %s\n", tvalores["marcaconciliacion"].toAscii().constData(), tvalores["idbinteligente"].toAscii().constData());
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
    } // end if
    tag = "";
    data = "";
    return TRUE;
}


/// Se ha encontrado informaci&oacute;n de contenido en el XML.
/** La funci&oacute;n almacena en el mapa de clase \ref tvalores la informaci&oacute;n
    contenida asignandolo al ltimo tag que se ha abierto. */
bool importainteligente::characters(const QString& ch) {
    if (tag != "") {
        data = ch;
        tvalores[tag] = data;
    } // end if
    return TRUE;
}

