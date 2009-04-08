/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#include <QMenu>

#include "asientolistsubform.h"


/// Subformulario.
/**
\param parent
**/
AsientoListSubform::AsientoListSubform ( QWidget *parent, const char * )
        : BcSubForm ( parent )
{
    _depura ( "AsientoListSubform::AsientoListSubform", 0 );
    setDbTableName ( "asiento" );
    setFileConfig ( "AsientoListSubform" );
    setDbFieldId ( "idasiento" );
    addSubFormHeader ( "ordenasiento", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Orden" ) );
    addSubFormHeader ( "fecha", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha" ) );
    addSubFormHeader ( "numap", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Apuntes" ) );
    addSubFormHeader ( "numborr", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Borrador" ) );
    addSubFormHeader ( "comentariosasiento", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Comentarios" ) );
    addSubFormHeader ( "totaldebe", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Debe" ) );
    addSubFormHeader ( "totalhaber", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Haber" ) );
    addSubFormHeader ( "idasiento", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id asiento" ) );
    addSubFormHeader ( "clase", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Clase" ) );
    setinsercion ( FALSE );
    setDelete ( FALSE );
    setOrdenPorQuery ( TRUE );
//    setOrdenEnabled ( TRUE );

    connect ( this, SIGNAL ( pintaMenu ( QMenu * ) ), this, SLOT ( s_pintaMenu ( QMenu * ) ) );
    connect ( this, SIGNAL ( trataMenu ( QAction * ) ), this, SLOT ( s_trataMenu ( QAction * ) ) );

    _depura ( "END AsientoListSubform::AsientoListSubform", 0 );
}


///
/**
**/
AsientoListSubform::~AsientoListSubform()
{
    _depura ( "AsientoListSubform::~AsientoListSubform", 0 );
    _depura ( "END AsientoListSubform::~AsientoListSubform", 0 );
}



/// Slot que trata la solicitud de pintar el menu contextual sobre el subformulario.
/**
\param menu
**/
void AsientoListSubform::s_pintaMenu ( QMenu *menu )
{
    _depura ( "ListLinAsiento1View::s_pintaMenu", 0 );
    menu->addSeparator();
    menu->addAction ( _ ( "Mostrar asiento" ) );
    menu->addSeparator();
    menu->addAction ( _ ( "Mostrar extracto (dia)" ) );
    menu->addAction ( _ ( "Mostrar extracto (mes)" ) );
    menu->addAction ( _ ( "Mostrar extracto (ano)" ) );
    _depura ( "ListLinAsiento1View::s_pintaMenu", 0 );
}

/// Slot que trata la activacion de un elemento en el menu contextual.
/**
\param action
/return
**/
void AsientoListSubform::s_trataMenu ( QAction *action )
{
    _depura ( "ListLinAsiento1View::s_trataMenu", 0 );
    if ( !action ) return;
    if ( action->text() == _ ( "Mostrar asiento" ) )
        boton_asiento();
    if ( action->text() == _ ( "Mostrar extracto (dia)" ) )
        boton_extracto1 ( 0 );
    if ( action->text() == _ ( "Mostrar extracto (mes)" ) )
        boton_extracto1 ( 1 );
    if ( action->text() == _ ( "Mostrar extracto (ano)" ) )
        boton_extracto1 ( 2 );

    _depura ( "END ListLinAsiento1View::s_trataMenu", 0 );
}

///
/**
\return
**/

/*
void AsientoListSubform::contextMenuEvent ( QContextMenuEvent * )
{
    _depura ( "AsientoListSubform::contextMenuEvent", 0 );
    QAction *del = NULL;
    int row = currentRow();
    if ( row < 0 ) {
        return;
    } // end if
    int col = currentColumn();
    if ( row < 0 ) {
        return;
    } // end if
    QMenu *popup = new QMenu ( this );
    QAction *mostapunte = popup->addAction ( "Mostrar asiento" );
    popup->addSeparator();
    QAction *mostextractodia = popup->addAction ( "Mostrar extracto (dia)" );
    QAction *mostextractomes = popup->addAction ( "Mostrar extracto (mes)" );
    QAction *mostextractoano = popup->addAction ( "Mostrar extracto (ano)" );

    if ( m_delete ) {
        del = popup->addAction ( _( "Borrar registro" ) );
    } // end if
    popup->addSeparator();
    QAction *ajustc = popup->addAction ( _( "Ajustar columa" ) );
    QAction *ajustac = popup->addAction ( _( "Ajustar altura" ) );
    QAction *ajust = popup->addAction ( _( "Ajustar columnas" ) );
    QAction *ajusta = popup->addAction ( _( "Ajustar alturas" ) );
    popup->addSeparator();
    QAction *verconfig = popup->addAction ( _( "Ver configurador de subformulario" ) );

    QAction *opcion = popup->exec ( QCursor::pos() );

    if ( opcion == mostapunte ) {
        boton_asiento();
    } // end if
    if ( opcion == mostextractodia ) {
        boton_extracto1 ( 0 );
    } // end if
    if ( opcion == mostextractomes ) {
        boton_extracto1 ( 1 );
    } // end if
    if ( opcion == mostextractoano ) {
        boton_extracto1 ( 2 );
    } // end if
    if ( opcion == ajust ) {
        resizeColumnsToContents();
    } // end if
    if ( opcion == ajusta ) {
        resizeRowsToContents();
    } // end if
    if ( opcion == ajustc ) {
        resizeColumnToContents ( col );
    } // end if
    if ( opcion == ajustac ) {
        resizeRowToContents ( row );
    } // end if
    if ( opcion == verconfig ) {
        showConfig();
    } // end if
    delete popup;
    _depura ( "END AsientoListSubform::contextMenuEvent", 0 );
}
*/
