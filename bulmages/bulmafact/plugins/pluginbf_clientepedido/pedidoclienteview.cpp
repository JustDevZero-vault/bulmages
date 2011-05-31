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

#include <fstream>

#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QLayout>
#include <QCloseEvent>


#include "pedidoclienteview.h"
#include "bfcompany.h"
#include "listlinpedidoclienteview.h"
#include "clientslist.h"
#include "bfinformereferencia.h"
#include "blfunctions.h"
#include "blplugins.h"


///
/**
\param comp
\param parent
\return
**/
PedidoClienteView::PedidoClienteView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    blDebug ( "PedidoClienteView::PedidoClienteView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        setupUi ( this );

        setTitleName ( _ ( "Pedido Cliente" ) );
        setDbTableName ( "pedidocliente" );
        setDbFieldId ( "idpedidocliente" );
        addDbField ( "idpedidocliente", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Identificador" ) );
        addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Cliente" ) );
        addDbField ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Almacen" ) );
        addDbField ( "numpedidocliente", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Numero pedido cliente" ) );
        addDbField ( "fechapedidocliente", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Identificador presupuesto" ) );
        addDbField ( "descpedidocliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Fecha" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Forma pago" ) );
        addDbField ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Trabajador" ) );
        addDbField ( "contactpedidocliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Contacto" ) );
        addDbField ( "telpedidocliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Telefono" ) );
        addDbField ( "comentpedidocliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios" ) );
        addDbField ( "procesadopedidocliente", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Procesado" ) );
        addDbField ( "refpedidocliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia" ) );


        /// Disparamos los plugins.
        int res = g_plugins->lanza ( "PedidoClienteView_PedidoClienteView", this );
        if ( res != 0 )
            return;
        subform3->setMainCompany ( comp );
        mui_idcliente->setMainCompany ( comp );
        mui_idforma_pago->setMainCompany ( comp );
        m_descuentos->setMainCompany ( comp );
        mui_idalmacen->setMainCompany ( comp );
        mui_idtrabajador->setMainCompany ( comp );
        mui_refpedidocliente->setMainCompany ( comp );
        setListaLineas ( subform3 );
        setListaDescuentos ( m_descuentos );
        mui_idforma_pago->setId ( "" );
        mui_idalmacen->setId ( "" );
        mui_idtrabajador->setId ( "0" );
        /// Establecemos los parametros de busqueda del Cliente
        mui_idcliente->setLabel ( _ ( "Cliente:" ) );
        mui_idcliente->setTableName ( "cliente" );
        mui_idcliente->m_valores["cifcliente"] = "";
        mui_idcliente->m_valores["nomcliente"] = "";
        insertWindow ( windowTitle(), this, FALSE );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el pedido cliente" ), this );
    } // end try
    blDebug ( "END PedidoClienteView::PedidoClienteView", 0 );
}


///
/**
**/
PedidoClienteView::~PedidoClienteView()
{
    blDebug ( "PedidoClienteView::~PedidoClienteView", 0 );
    blDebug ( "END PedidoClienteView::~PedidoClienteView", 0 );
}


///
/**
**/
void PedidoClienteView::inicializar()
{
    blDebug ( "PedidoClienteView::inicializar", 0 );
    subform3->inicializar();
    m_descuentos->inicializar();
    pintar();
    dialogChanges_readValues();
    blDebug ( "END PedidoClienteView::inicializar", 0 );
}


///
/**
\param iva
\param base
\param total
\param desc
\param irpf
\param reqeq
**/
void PedidoClienteView::pintatotales ( BlFixed iva, BlFixed base, BlFixed total, BlFixed desc, BlFixed irpf, BlFixed reqeq )
{
    blDebug ( "PedidoClienteView::pintatotales", 0 );
    m_totalBases->setText ( QString ( base.toQString() ) );
    m_totalTaxes->setText ( QString ( iva.toQString() ) );
    m_totalDiscounts->setText ( QString ( desc.toQString() ) );
    m_totalIRPF->setText ( QString ( irpf.toQString() ) );
    m_totalReqEq->setText ( QString ( reqeq.toQString() ) );
    m_totalpedidocliente->setText ( total.toQString() );
    blDebug ( "END PedidoClienteView::pintatotales", 0 );
}


///
/**
**/
void PedidoClienteView::on_mui_verpresupuesto_clicked()
{
    blDebug ( "PedidoClienteView::on_mui_verpresupuesto_clicked", 0 );

    blDebug ( "END PedidoClienteView::on_mui_verpresupuesto_clicked", 0 );
}


/** Genera un Albaran a Cliente a partir de los datos
    que tiene el pedido.
*/
/**
\return
**/
void PedidoClienteView::generarAlbaran()
{
    blDebug ( "PedidoClienteView::generarAlbaran", 0 );
    /*

        AlbaranClienteView *bud = NULL;
        BlDbRecordSet *cur = NULL;

        try {
            /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
            /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
            /// el siguiente cur->eof() el programa fallar&aacute;.
            /// Comprobamos que existe el albaran con esos datos, y en caso afirmativo lo mostramos.

            QString SQLQuery = "";

            if ( dbValue ( "refpedidocliente" ).isEmpty() || dbValue ( "idcliente" ).isEmpty() ) {
                /// El presupuesto no se ha guardado y no se dispone en la base de datos
                /// de estos datos. Se utilizan en su lugar los del formulario.
                /// Verifica que exista, por lo menos, un cliente seleccionado.
                if ( mui_idcliente->idcliente().isEmpty() ) {
                    blMsgInfo ( _( "Tiene que seleccionar un cliente" ), this );
                    return;
                } else {
                    SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + mui_refpedidocliente->text() + "' AND idcliente = " + mui_idcliente->idcliente();
                } // end if
            } else {
                SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + dbValue ( "refpedidocliente" ) + "' AND idcliente = " + dbValue ( "idcliente" );
            } // end if

            cur = mainCompany() ->loadQuery ( SQLQuery );

            if ( !cur->eof() ) {
                /// Informamos que ya hay un albaran y que la abriremos.
                /// Si no salimos de la funci&oacute;n.
                if ( QMessageBox::question ( this,
                                             _( "Albaran ya existe" ),
                                             _( "Existe un albaran a este cliente con la misma referencia que este pedido. Desea abrirlo para verificar?" ),
                                             _( "&Si" ), _( "&No" ), QString::null, 0, 1 ) ) {
                    return;
                } // end if
                bud = new AlbaranClienteView ( mainCompany(), NULL );
                mainCompany() ->m_pWorkspace->addSubWindow ( bud );
                bud->load ( cur->value( "idalbaran" ) );
                bud->show();
                return;
            } // end if
            delete cur;

            /// Creamos el albaran.
            bud = mainCompany() ->newAlbaranClienteView();
            mainCompany() ->m_pWorkspace->addSubWindow ( bud );
            bud->load ( "0" );

            /// Traspasamos los datos al albaran.
            recogeValores();
            bud->setDbValue ( "comentalbaran", dbValue ( "comentpedidocliente" ) );
            bud->setDbValue ( "descalbaran", dbValue ( "descpedidocliente" ) );
            bud->setDbValue ( "idforma_pago", dbValue ( "idforma_pago" ) );
            bud->setDbValue ( "refalbaran", dbValue ( "refpedidocliente" ) );
            bud->setDbValue ( "idcliente", dbValue ( "idcliente" ) );
            bud->setDbValue ( "idalmacen", dbValue ( "idalmacen" ) );
            bud->setDbValue ( "contactalbaran", dbValue ( "contactpedidocliente" ) );
            bud->setDbValue ( "telalbaran", dbValue ( "telpedidocliente" ) );
            bud->setDbValue ( "idtrabajador", dbValue ( "idtrabajador" ) );

            /// Traspasamos las lineas al albaran.
            BlDbSubFormRecord *linea, *linea1;
            for ( int i = 0; i < m_listalineas->rowCount(); ++i ) {
                linea = m_listalineas->lineaat ( i );
                if ( linea->dbValue ( "idarticulo" ) != "" ) {
                    linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                    bud->getlistalineas() ->newRecord();
                    bud->getlistalineas() ->setProcesarCambios ( FALSE );
                    linea1->setDbValue ( "desclalbaran", linea->dbValue ( "desclpedidocliente" ) );
                    linea1->setDbValue ( "cantlalbaran", linea->dbValue ( "cantlpedidocliente" ) );
                    linea1->setDbValue ( "pvplalbaran", linea->dbValue ( "pvplpedidocliente" ) );
                    linea1->setDbValue ( "ivalalbaran", linea->dbValue ( "ivalpedidocliente" ) );
                    linea1->setDbValue ( "descuentolalbaran", linea->dbValue ( "descuentolpedidocliente" ) );
                    linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
                    linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
                    linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );
                    bud->getlistalineas() ->setProcesarCambios ( TRUE );
                } // end if
            } // end for

            /// Traspasamos los descuentos.
            for ( int i = 0; i < m_listadescuentos->rowCount(); ++i ) {
                linea1 = m_listadescuentos->lineaat ( i );
                if ( linea1->dbValue ( "proporciondpedidocliente" ) != "" ) {
                    linea = bud->getlistadescuentos() ->lineaat ( bud->getlistadescuentos() ->rowCount() - 1 );
                    bud->getlistadescuentos() ->setProcesarCambios ( FALSE );
                    linea->setDbValue ( "conceptdalbaran", linea1->dbValue ( "conceptdpedidocliente" ) );
                    linea->setDbValue ( "proporciondalbaran", linea1->dbValue ( "proporciondpedidocliente" ) );
                    bud->getlistadescuentos() ->setProcesarCambios ( TRUE );
                    bud->getlistadescuentos() ->newRecord();
                } // end if
            } // end for
            /// Pintamos el pedido y lo presentamos.
            bud->pintar();
            bud->calculaypintatotales();
            bud->show();

        } catch ( ... ) {
            blMsgInfo ( _( "Error inesperado" ), this );
            if ( cur ) delete cur;
            if ( bud ) delete bud;
        } // end try
    */
    blDebug ( "END PedidoClienteView::generarAlbaran", 0 );
}


///
/**
\param id
**/
void PedidoClienteView::on_mui_idcliente_valueChanged ( QString id )
{
    blDebug ( "PedidoClienteView::on_m_cliente_valueChanged", 0 );
    subform3->setIdCliente ( id );
    mui_idforma_pago->setIdCliente ( id );
    blDebug ( "END PedidoClienteView::on_m_cliente_valueChanged", 0 );
}


///
/**
\return
**/
int PedidoClienteView::beforeDelete()
{
    blDebug ( "PedidoClienteView::borrar", 0 );
    m_listalineas->remove();
    m_listadescuentos->remove();
    blDebug ( "PedidoClienteView::borrar", 0 );
    return 0;
}


/// Esta funcion carga un PedidoCliente.
/**
\param idbudget
\return
**/
int PedidoClienteView::cargarPost ( QString idbudget )
{
    blDebug ( "PedidoClienteView::cargarPost", 0 );

    m_listalineas->load ( idbudget );
    m_listadescuentos->load ( idbudget );

    /// Disparamos los plugins.
    g_plugins->lanza ( "PedidoClienteView_cargarPost_Post", this );

    calculaypintatotales();

    blDebug ( "END PedidoClienteView::cargar", 0 );
    return 0;
}


/// Guardamos el pedido cliente.
/**
\return
**/
int PedidoClienteView::afterSave()
{
    blDebug ( "PedidoClienteView::guardar", 0 );
    m_listalineas->setColumnValue ( "idpedidocliente", dbValue ( "idpedidocliente" ) );
    m_listalineas->save();
    m_listadescuentos->setColumnValue ( "idpedidocliente", dbValue ( "idpedidocliente" ) );
    m_listadescuentos->save();
    return 0;
}


///
/**
**/
void PedidoClienteView::s_pintaTotales()
{
    blDebug ( "PedidoClienteView::s_pintaTotales", 0 );
    calculaypintatotales();
    blDebug ( "END PedidoClienteView::s_pintaTotales", 0 );
}


///
/**
**/
void PedidoClienteView::on_mui_pasaraalbaran_clicked()
{
    blDebug ( "PedidoClienteView::on_mui_pasaraalbaran_clicked", 0 );
    generarAlbaran();
    blDebug ( "END PedidoClienteView::on_mui_pasaraalbaran_clicked", 0 );
}


///
/**
**/
void PedidoClienteView::on_m_descuentos_editFinish ( int, int )
{
    blDebug ( "PedidoClienteView::on_m_descuentos_editFinish", 0 );
    calculaypintatotales();
    blDebug ( "END PedidoClienteView::on_m_descuentos_editFinish", 0 );
}


///
/**
**/
void PedidoClienteView::on_subform3_editFinish ( int, int )
{
    blDebug ( "PedidoClienteView::on_subform3_editFinish", 0 );
    calculaypintatotales();
    blDebug ( "END sPedidoClienteView::on_subform3_editFinish", 0 );
}


///
/**
\param id
**/
void PedidoClienteView::on_mui_idalmacen_valueChanged ( QString id )
{
    blDebug ( "PedidoClienteView::on_mui_idalmacen_valueChanged", 0 );
    m_listalineas->setIdAlmacen ( id );
    blDebug ( "END PedidoClienteView::on_mui_idalmacen_valueChanged", 0 );
}

