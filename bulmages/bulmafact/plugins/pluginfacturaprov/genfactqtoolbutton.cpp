/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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
#include "genfactqtoolbutton.h"
#include "funcaux.h"

/// Necesarios para importacion de efactura
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>
#include "facturapview.h"
#include "albaranproveedorview.h"
#include "pedidoproveedorview.h"
#include "fixed.h"
#include "company.h"
#include "dbrecord.h"

///
/**
\param pres
\param ped
\param alb
\param fac
\param parent
**/
GenFacProQToolButton::GenFacProQToolButton ( QWidget *fac , QWidget *parent ) : QToolButton ( parent )
{
    _depura ( "GenFacProQToolButton::GenFacProQToolButton", 0 );
    m_object = fac;
    setBoton();
    _depura ( "END GenFacProQToolButton::GenFacProQToolButton", 0 );
}


///
/**
**/
GenFacProQToolButton::~GenFacProQToolButton()
{
    _depura ( "GenFacProQToolButton::~GenFacProQToolButton", 0 );
    _depura ( "END GenFacProQToolButton::~GenFacProQToolButton", 0 );
}


///
/**
**/
void GenFacProQToolButton::setBoton()
{
    _depura ( "GenFacProQToolButton::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( "Generar Factura Proveedor" );
    setToolTip ( "Generar Factura Proveedor" );
    setMinimumSize ( QSize ( 32, 32 ) );
    setMaximumSize ( QSize (32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-invoice.svg" ) ) );
    setIconSize ( QSize ( 32, 32 ) );
    setContentsMargins (0, 0, 0, 0);
    _depura ( "END GenFacProQToolButton::setBoton", 0 );
}


///
/**
**/
void GenFacProQToolButton::click()
{
    _depura ( "ImpQToolButton::click", 0 );

    if (m_object->objectName() == "AlbaranProveedorBase" ) {
		generarFacturaProveedor();
    }// end if

    if (m_object->objectName() == "PedidoProveedorBase" ) {
	PedidoProveedorView *fpv = (PedidoProveedorView *) m_object;
		generarFacturaProveedor1();
    }// end if

    _depura ( "END ImpQToolButton::click", 0 );
}

/// Se encarga de generar una factura a partir de un albar&aacute;n.
/** Primero de todo busca una factura por referencia que tenga este albaran.
    Si dicha factura existe entonces la cargamos y terminamos.
    Si no existe dicha factura el sistema avisa y permite crear una poniendo
    Todos los datos del albaran automaticamente en ella.
*/
/**
\return
**/
void GenFacProQToolButton::generarFacturaProveedor()
{
    _depura ( "GenFacProQToolButton::generarFacturaProveedor", 0 );
	AlbaranProveedorView *fpv = (AlbaranProveedorView *) m_object;


    FacturaProveedorView *bud = NULL;
    cursor2 *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe una factura con esos datos, y en caso afirmativo lo mostramos.

        QString SQLQuery = "";

        if ( fpv->DBvalue ( "refalbaranp" ).isEmpty() || fpv->DBvalue ( "idproveedor" ).isEmpty() ) {
            /// El albaran no se ha guardado y no se dispone en la base de datos
            /// de estos datos. Se utilizan en su lugar los del formulario.
            /// Verifica que exista, por lo menos, un cliente seleccionado.
            if ( fpv->mui_idproveedor->idproveedor().isEmpty() ) {
                mensajeInfo ( _( "Tiene que seleccionar un proveedor" ), this );
                return;
            } else {
                SQLQuery = "SELECT * FROM facturap WHERE reffacturap = '" + fpv->mui_refalbaranp->text() + "' AND idproveedor = " + fpv->mui_idproveedor->idproveedor();
            } // end if
        } else {
            SQLQuery = "SELECT * FROM facturap WHERE reffacturap = '" + fpv->DBvalue ( "refalbaranp" ) + "' AND idproveedor = " + fpv->DBvalue ( "idproveedor" );
        } // end if

        cur = fpv->empresaBase() ->cargacursor ( SQLQuery );

        if ( !cur->eof() ) {
            /// Informamos que ya hay una factura y que la abriremos.
            /// Si no salimos de la funci&oacute;n.
            if ( QMessageBox::question ( this,
                                         _( "Factura de proveedor existente" ),
                                         _( "Existe una factura de este proveedor con la misma referencia que este albaran. Desea abrirla para verificar?" ),
                                         _( "&Si" ), _( "&No" ), QString::null, 0, 1 ) ) {
                return;
            } // end if
            FacturaProveedorView *bud = new FacturaProveedorView((Company *) fpv->empresaBase(), 0);
            fpv->empresaBase() ->m_pWorkspace->addWindow ( bud );
            bud->cargar ( cur->valor ( "idfacturap" ) );
            bud->show();
            return;
        } // end if
        delete cur;

        /// Creamos la factura.
        FacturaProveedorView *bud = new FacturaProveedorView((Company *) fpv->empresaBase(), 0);
        fpv->empresaBase() ->m_pWorkspace->addWindow ( bud );

        /// Cargamos un elemento que no existe para inicializar bien la clase.
        bud->inicializar();

        bud->setDBvalue ( "comentfacturap", fpv->DBvalue ( "comentalbaranp" ) );
        bud->setDBvalue ( "idforma_pago", fpv->DBvalue ( "idforma_pago" ) );
        bud->setDBvalue ( "reffacturap", fpv->DBvalue ( "refalbaranp" ) );
        bud->setDBvalue ( "idproveedor", fpv->DBvalue ( "idproveedor" ) );
        bud->pintar();
        bud->show();

	/// Traspasamos las lineas de factura
        QString l;
        SDBRecord *linea, *linea1;
        for ( int i = 0; i < fpv->m_listalineas->rowCount(); ++i ) {
            linea = fpv->m_listalineas->lineaat ( i );
            if ( linea->DBvalue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->nuevoRegistro();
                bud->getlistalineas() ->setProcesarCambios ( FALSE );
                linea1->setDBvalue ( "desclfacturap", linea->DBvalue ( "desclalbaranp" ) );
                linea1->setDBvalue ( "cantlfacturap", linea->DBvalue ( "cantlalbaranp" ) );
                linea1->setDBvalue ( "pvplfacturap", linea->DBvalue ( "pvplalbaranp" ) );
                linea1->setDBvalue ( "descuentolfacturap", linea->DBvalue ( "descuentolalbaranp" ) );
                linea1->setDBvalue ( "idarticulo", linea->DBvalue ( "idarticulo" ) );
                linea1->setDBvalue ( "codigocompletoarticulo", linea->DBvalue ( "codigocompletoarticulo" ) );
                linea1->setDBvalue ( "nomarticulo", linea->DBvalue ( "nomarticulo" ) );
                linea1->setDBvalue ( "ivalfacturap", linea->DBvalue ( "ivalalbaranp" ) );
                bud->getlistalineas() ->setProcesarCambios ( TRUE );
            } // end if
        } // end for
        bud->calculaypintatotales();

    } catch ( ... ) {
        mensajeInfo ( _( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try

    _depura ( "END GenFacProQToolButton::generarFacturaProveedor", 0 );
}



/// Se encarga de generar una factura a partir de un albar&aacute;n.
/** Primero de todo busca una factura por referencia que tenga este albaran.
    Si dicha factura existe entonces la cargamos y terminamos.
    Si no existe dicha factura el sistema avisa y permite crear una poniendo
    Todos los datos del albaran automaticamente en ella.
*/
/**
\return
**/
void GenFacProQToolButton::generarFacturaProveedor1()
{
    _depura ( "GenFacProQToolButton::generarFacturaProveedor1", 0 );
	PedidoProveedorView *fpv = (PedidoProveedorView *) m_object;


    FacturaProveedorView *bud = NULL;
    cursor2 *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe una factura con esos datos, y en caso afirmativo lo mostramos.

        QString SQLQuery = "";

        if ( fpv->DBvalue ( "refpedidoproveedor" ).isEmpty() || fpv->DBvalue ( "idproveedor" ).isEmpty() ) {
            /// El albaran no se ha guardado y no se dispone en la base de datos
            /// de estos datos. Se utilizan en su lugar los del formulario.
            /// Verifica que exista, por lo menos, un cliente seleccionado.
            if ( fpv->mui_idproveedor->idproveedor().isEmpty() ) {
                mensajeInfo ( _( "Tiene que seleccionar un proveedor" ), this );
                return;
            } else {
                SQLQuery = "SELECT * FROM facturap WHERE reffacturap = '" + fpv->mui_refpedidoproveedor->text() + "' AND idproveedor = " + fpv->mui_idproveedor->idproveedor();
            } // end if
        } else {
            SQLQuery = "SELECT * FROM facturap WHERE reffacturap = '" + fpv->DBvalue ( "refpedidoproveedor" ) + "' AND idproveedor = " + fpv->DBvalue ( "idproveedor" );
        } // end if

        cur = fpv->empresaBase() ->cargacursor ( SQLQuery );

        if ( !cur->eof() ) {
            /// Informamos que ya hay una factura y que la abriremos.
            /// Si no salimos de la funci&oacute;n.
            if ( QMessageBox::question ( this,
                                         _( "Factura de proveedor existente" ),
                                         _( "Existe una factura de este proveedor con la misma referencia que este albaran. Desea abrirla para verificar?" ),
                                         _( "&Si" ), _( "&No" ), QString::null, 0, 1 ) ) {
                return;
            } // end if
            FacturaProveedorView *bud = new FacturaProveedorView((Company *) fpv->empresaBase(), 0);
            fpv->empresaBase() ->m_pWorkspace->addWindow ( bud );
            bud->cargar ( cur->valor ( "idfacturap" ) );
            bud->show();
            return;
        } // end if
        delete cur;

        /// Creamos la factura.
        FacturaProveedorView *bud = new FacturaProveedorView((Company *) fpv->empresaBase(), 0);
        fpv->empresaBase() ->m_pWorkspace->addWindow ( bud );

        /// Cargamos un elemento que no existe para inicializar bien la clase.
        bud->inicializar();

        bud->setDBvalue ( "comentfacturap", fpv->DBvalue ( "comentpedidoproveedor" ) );
        bud->setDBvalue ( "idforma_pago", fpv->DBvalue ( "idforma_pago" ) );
        bud->setDBvalue ( "reffacturap", fpv->DBvalue ( "refpedidoproveedor" ) );
        bud->setDBvalue ( "idproveedor", fpv->DBvalue ( "idproveedor" ) );
        bud->pintar();
        bud->show();

	/// Traspasamos las lineas de factura
        QString l;
        SDBRecord *linea, *linea1;
        for ( int i = 0; i < fpv->m_listalineas->rowCount(); ++i ) {
            linea = fpv->m_listalineas->lineaat ( i );
            if ( linea->DBvalue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->nuevoRegistro();
                bud->getlistalineas() ->setProcesarCambios ( FALSE );
                linea1->setDBvalue ( "desclfacturap", linea->DBvalue ( "desclpedidoproveedor" ) );
                linea1->setDBvalue ( "cantlfacturap", linea->DBvalue ( "cantlpedidoproveedor" ) );
                linea1->setDBvalue ( "pvplfacturap", linea->DBvalue ( "pvplpedidoproveedor" ) );
                linea1->setDBvalue ( "descuentolfacturap", linea->DBvalue ( "descuentolpedidoproveedor" ) );
                linea1->setDBvalue ( "idarticulo", linea->DBvalue ( "idarticulo" ) );
                linea1->setDBvalue ( "codigocompletoarticulo", linea->DBvalue ( "codigocompletoarticulo" ) );
                linea1->setDBvalue ( "nomarticulo", linea->DBvalue ( "nomarticulo" ) );
                linea1->setDBvalue ( "ivalfacturap", linea->DBvalue ( "ivalpedidoproveedor" ) );
                bud->getlistalineas() ->setProcesarCambios ( TRUE );
            } // end if
        } // end for
        bud->calculaypintatotales();

    } catch ( ... ) {
        mensajeInfo ( _( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try

    _depura ( "END GenFacProQToolButton::generarFactura", 0 );
}

