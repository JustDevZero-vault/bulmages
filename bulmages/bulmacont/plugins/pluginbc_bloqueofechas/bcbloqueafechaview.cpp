/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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

#include <QWidget>
#include <QInputDialog>
#include <QDate>

#include "bcbloqueafechaview.h"
#include "bccompany.h"


///
/**
\param parent
\return
**/
BcTreeWidgetItem::BcTreeWidgetItem ( QTreeWidget *parent )
        : QTreeWidgetItem ( parent )
{
    blDebug ( "BcTreeWidgetItem::BcTreeWidgetItem", 0 );
    blDebug ( "END BcTreeWidgetItem::BcTreeWidgetItem", 0 );
    return;
}


///
/**
\param parent
\return
**/
BcTreeWidgetItem::BcTreeWidgetItem ( QTreeWidgetItem *parent )
        : QTreeWidgetItem ( parent )
{
    blDebug ( "BcTreeWidgetItem::BcTreeWidgetItem", 0 );
    blDebug ( "END BcTreeWidgetItem::BcTreeWidgetItem", 0 );
    return;
}


///
/**
\return
**/
BcTreeWidgetItem::~BcTreeWidgetItem()
{
    return;
}


///
/**
\param emp
\param parent
**/
BcBloqueaFechaView::BcBloqueaFechaView ( BcCompany *emp, QWidget *parent )
        : BcForm ( emp, parent )
{
    blDebug ( "BcBloqueaFechaView::BcBloqueaFechaView", 0 );

    setTitleName ( _ ( "Bloquear Fechas" ) );
    /// Establecemos cual es la tabla en la que basarse para obtener permisos
    setDbTableName ( "asiento" );

    qsbloqueado = _ ( "Bloqueado" );
    qsabierto = _ ( "Abierto" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    QString query;
    inicializa();
    mainCompany() ->meteWindow ( windowTitle(), this );
    blDebug ( "ENd BcBloqueaFechaView::BcBloqueaFechaView", 0 );
}


///
/**
**/
BcBloqueaFechaView::~BcBloqueaFechaView()
{
    blDebug ( "BcBloqueaFechaView::~BcBloqueaFechaView", 0 );
    mainCompany() ->sacaWindow ( this );
    blDebug ( "ENd BcBloqueaFechaView::~BcBloqueaFechaView", 0 );
}


///
/**
**/
void BcBloqueaFechaView::inicializa()
{
    blDebug ( "BcBloqueaFechaView::inicializa", 0 );
    BcTreeWidgetItem *itemlevel0;
    BcTreeWidgetItem *itemlevel1 = 0;

    QString consultabd;
    mui_treeWidget->setColumnCount ( 2 );
    QStringList cabecera;
    cabecera << _ ( "Ejercicio" ) << _ ( "Estado" );
    mui_treeWidget->setColumnWidth ( 0, 200 );
    mui_treeWidget->setHeaderLabels ( cabecera );

    mui_treeWidget->clear();
    mui_treeWidget->setSortingEnabled ( FALSE );

    /// Consultamos a la base de datos.
    consultabd.sprintf ( "SELECT * FROM ejercicios WHERE periodo = 0 ORDER BY ejercicio DESC" );
    BlDbRecordSet *curPeri, *curEjer = mainCompany() ->loadQuery ( consultabd );

    while ( !curEjer->eof() ) {

        itemlevel0 = new BcTreeWidgetItem ( mui_treeWidget );
        itemlevel0->setText ( 0, curEjer->valor ( "ejercicio" ) ); /// Columna 0.

        if ( curEjer->valor ( "bloqueado" ) == "t" ) {
            itemlevel0->setText ( 1, qsbloqueado );
        } else {
            itemlevel0->setText ( 1, qsabierto );
        } // end if

        itemlevel0->ej = curEjer->valor ( "ejercicio" );
        itemlevel0->per = curEjer->valor ( "periodo" );


        consultabd.sprintf ( "SELECT * FROM ejercicios WHERE ejercicio = '%s' ORDER BY periodo DESC", curEjer->valor ( "ejercicio" ).toAscii().constData() );
        curPeri = mainCompany() ->loadQuery ( consultabd );
        while ( !curPeri->eof() ) {
            switch ( curPeri->valor ( "periodo" ).toInt() ) {
            case 12:
                itemlevel1 = new BcTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _ ( "Diciembre" ) ); /// Columna 0.
                break;
            case 11:
                itemlevel1 = new BcTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _ ( "Noviembre" ) ); /// Columna 0.
                break;
            case 10:
                itemlevel1 = new BcTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _ ( "Octubre" ) ); /// Columna 0.
                break;
            case 9:
                itemlevel1 = new BcTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _ ( "Septiembre" ) ); /// Columna 0.
                break;
            case 8:
                itemlevel1 = new BcTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _ ( "Agosto" ) ); /// Columna 0.
                break;
            case 7:
                itemlevel1 = new BcTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _ ( "Julio" ) ); /// Columna 0.
                break;
            case 6:
                itemlevel1 = new BcTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _ ( "Junio" ) ); /// Columna 0.
                break;
            case 5:
                itemlevel1 = new BcTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _ ( "Mayo" ) ); /// Columna 0.
                break;
            case 4:
                itemlevel1 = new BcTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _ ( "Abril" ) ); /// Columna 0.
                break;
            case 3:
                itemlevel1 = new BcTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _ ( "Marzo" ) ); /// Columna 0.
                break;
            case 2:
                itemlevel1 = new BcTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _ ( "Febrero" ) ); /// Columna 0.
                break;
            case 1:
                itemlevel1 = new BcTreeWidgetItem ( itemlevel0 );
                itemlevel1->setText ( 0, _ ( "Enero" ) ); /// Columna 0.
                break;
            } // end switch
            itemlevel1->ej = curEjer->valor ( "ejercicio" );
            itemlevel1->per = curPeri->valor ( "periodo" );
            curPeri->valor ( "bloqueado" ) == "t" ? itemlevel1->setText ( 1, qsbloqueado ) : itemlevel1->setText ( 1, qsabierto );
            curPeri->nextRecord();
        } // end while
        curEjer->nextRecord();
    } // end while
    blDebug ( "ENd BcBloqueaFechaView::inicializa", 0 );
}


///
/**
\param item
\param columna
**/
void BcBloqueaFechaView::on_mui_treeWidget_itemDoubleClicked ( QTreeWidgetItem *item, int columna )
{
    blDebug ( "BcBloqueaFechaView::on_mui_treeWidget_doubleClicked", 0 );
    int error;
    BcTreeWidgetItem *it = ( BcTreeWidgetItem * ) item;
    if ( columna == 1 ) {
        if ( item->text ( 1 ) == qsbloqueado ) {
            item->setText ( 1, qsabierto );
            QString consultabd = "UPDATE ejercicios SET bloqueado = FALSE WHERE ejercicio = '" + it->ej + "' AND periodo = '" + it->per + "'";
            error = mainCompany() ->runQuery ( consultabd );
        } else {
            item->setText ( 1, qsbloqueado );
            QString consultabd = "UPDATE ejercicios SET bloqueado = TRUE WHERE ejercicio = '" + it->ej + "' AND periodo = '" + it->per + "'";
            error = mainCompany() ->runQuery ( consultabd );
        } // end if
    } // end if
    blDebug ( "END BcBloqueaFechaView::on_mui_treeWidget_doubleClicked", 0 );
}


///
/**
**/
void BcBloqueaFechaView::on_mui_crear_clicked()
{
    blDebug ( "BcBloqueaFechaView::on_mui_crear_clicked", 0 );
    /// Tomamos el anyo pasado como referencia.
    int ejer = QDate::currentDate().year() - 1;

    /// Miramos si ya hay ejercicios introducidos y si es asi cogemos el siguiente como referencia.
    QString consultabd = "SELECT max(ejercicio) AS ej FROM ejercicios";
    BlDbRecordSet *cur = mainCompany()->loadQuery ( consultabd );
    if ( cur ) {
        if ( !cur->eof() ) {
            if ( cur->valor ( "ej" ).toInt() != 0 )
                ejer = cur->valor ( "ej" ).toInt();
        } // end if
        delete cur;
    } // end if
    ejer++;

    /// Presentamos el dialogo preguntado que ejercicio crear.
    bool ok = FALSE;
    ejer = QInputDialog::getInteger ( this, _ ( "Introduzca Ejercicio a Crear" ),
                                      _ ( "Ponga el anyo:" ), ejer, 0, 10000, 1, &ok );

    /// Comprobamos que el ejercicio introducido sea valido.
    if ( !ok ) return;
    if ( ejer < 1000 ) {
        blMsgInfo ( "Ejercicio invalido" );
        return;
    } // end if
    QString query = "SELECT * FROM ejercicios WHERE ejercicio = " + QString::number ( ejer );
    cur = mainCompany()->loadQuery ( query );
    if ( cur ) {
        if ( cur->numregistros() > 0 ) {
            delete cur;
            blMsgInfo ( _ ( "Ejercicios ya contemplado" ) );
            return;
        } // end if
    } // end if


    /// Creamos el ejercicio.
    for ( int x = 0; x <= 12; x++ ) {
        try {
            QString consultabd = "INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES('" + QString::number ( ejer ) + "', '" + QString::number ( x ) + "', 'f')";
            mainCompany()->begin();
            mainCompany() ->runQuery ( consultabd );
            mainCompany()->commit();
        } catch ( ... ) {
            blMsgInfo ( "Error con la base de datos" );
            mainCompany()->rollback();
        } // end try
    } // end for

    /// Presentamos
    inicializa();

    blDebug ( "BcBloqueaFechaView::on_mui_crear_clicked", 0 );
}

