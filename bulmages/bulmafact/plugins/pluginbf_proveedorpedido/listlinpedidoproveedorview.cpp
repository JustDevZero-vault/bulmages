/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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
#include <QKeyEvent>
#include <QEvent>

#include "listlinpedidoproveedorview.h"
#include "blfunctions.h"
#include "blfixed.h"


///
/**
\param parent
**/
ListLinPedidoProveedorView::ListLinPedidoProveedorView ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListLinPedidoProveedorView::ListLinPedidoProveedorView", 0 );
    setDbTableName ( "lpedidoproveedor" );
    setDbFieldId ( "idlpedidoproveedor" );
    addSubFormHeader ( "puntlpedidoproveedor", BlDbField::DbBoolean, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Punteo" ) );
    addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView, _ ( "Id articulo" ) );
    addSubFormHeader ( "codigocompletoarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Codigo completo articulo" ) );
    addSubFormHeader ( "nomarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "Nombre articulo" ) );
    addSubFormHeader ( "idlpedidoproveedor", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Numero de linea" ) );
    addSubFormHeader ( "desclpedidoproveedor", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Descripcion" ) );
    addSubFormHeader ( "cantlpedidoproveedor", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Cantidad" ) );
    addSubFormHeader ( "pvplpedidoproveedor", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Precio" ) );
    addSubFormHeader ( "ivalpedidoproveedor", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "% I.V.A." ) );
    addSubFormHeader ( "reqeqlpedidoproveedor", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "% R.E." ) );
    addSubFormHeader ( "descuentolpedidoproveedor", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "% Descuento" ) );
    addSubFormHeader ( "idpedidoproveedor", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id pedido" ) );
    addSubFormHeader ( "ordenlpedidoproveedor", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Orden" ) );
    setInsert ( TRUE );
    setOrdenEnabled ( TRUE );
    setOrdenPorQuery ( FALSE );
    _depura ( "END ListLinPedidoProveedorView::ListLinPedidoProveedorView", 0 );
}


///
/**
\param idpedidoproveedor
**/
void ListLinPedidoProveedorView::cargar ( QString idpedidoproveedor )
{
    _depura ( "ListLinPedidoProveedorView::cargar", 0 );
    mdb_idpedidoproveedor = idpedidoproveedor;
    BlSubForm::cargar ( "SELECT * FROM lpedidoproveedor LEFT JOIN articulo ON lpedidoproveedor.idarticulo = articulo.idarticulo WHERE idpedidoproveedor=" + mdb_idpedidoproveedor + " ORDER BY ordenlpedidoproveedor" );
    _depura ( "END ListLinPedidoProveedorView::cargar", 0 );
}

