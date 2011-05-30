/***************************************************************************
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

#include <QCloseEvent>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QToolButton>
#include <QCheckBox>

#include "bldialogchanges.h"
#include "blfixed.h"
#include "bldatesearch.h"
#include "bfbuscarformapago.h"
#include "bfbuscaralmacen.h"
#include "bfbuscartrabajador.h"
#include "albaranclienteview.h"
#include "clientslist.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "blfunctions.h"
#include "bfinformereferencia.h"
#include "bfclientealbarandescuentosubform.h"
#include "bfclientealbaransubform.h"
#include "pedidoclienteview.h"
#include "pedidosclientelist.h"
#include "blpostgresqlclient.h"
#include "presupuestolist.h"
#include "presupuestoview.h"
#include "blplugins.h"


/** Constructor de la clase corresponde a la parte visual de la ficha de cliente.
    Inicializa la ventana y todos sus componentes.
    Mete la ventana en el WorkSpace.
*/
/**
\param comp
\param parent
\return
**/
AlbaranClienteView::AlbaranClienteView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    blDebug ( "AlbaranClienteView::AlbaranClienteView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        setTitleName ( _ ( "Albaran" ) );
        setDbTableName ( "albaran" );
        setDbFieldId ( "idalbaran" );
        addDbField ( "idalbaran", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Id albaran" ) );
        addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Cliente" ) );
        addDbField ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Almacen" ) );
        addDbField ( "numalbaran", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Numero de albaran" ) );
        addDbField ( "fechaalbaran", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha de creacion" ) );
        addDbField ( "contactalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Persona de contacto" ) );
        addDbField ( "telalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Telefono de contacto" ) );
        addDbField ( "comentalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentario" ) );
        addDbField ( "comentprivalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentario privado" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Forma de pago" ) );
        addDbField ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Trabajador" ) );
        addDbField ( "procesadoalbaran", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Procesado" ) );
        addDbField ( "descalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Descripcion" ) );
        addDbField ( "refalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia" ) );

        /// Disparamos los plugins.
        int res = g_plugins->lanza ( "AlbaranClienteView_AlbaranClienteView", this );
        if ( res != 0 )
            return;

        subform2->setMainCompany ( comp );
        m_descuentos->setMainCompany ( comp );
        mui_idalmacen->setMainCompany ( comp );
        mui_idforma_pago->setMainCompany ( comp );
        mui_idcliente->setMainCompany ( comp );
        mui_idtrabajador->setMainCompany ( comp );
        mui_refalbaran->setMainCompany ( comp );

        setListaLineas ( subform2 );
        setListaDescuentos ( m_descuentos );


        /// Establecemos los parametros de busqueda del Cliente
        mui_idcliente->setLabel ( _ ( "Cliente:" ) );
        mui_idcliente->setTableName ( "cliente" );
        mui_idcliente->m_valores["cifcliente"] = "";
        mui_idcliente->m_valores["nomcliente"] = "";

        /// Inicializamos para que no se queden sin ser pintada.
        mui_idforma_pago->setFieldValue ( "0" );
        mui_idalmacen->setFieldValue ( "0" );
        mui_idtrabajador->setFieldValue ( "0" );

        insertWindow ( windowTitle(), this, FALSE );
        /// Disparamos los plugins por flanco descendente.
        g_plugins->lanza ( "AlbaranClienteView_AlbaranClienteView_Post", this );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el albaran a cliente" ), this );
    } // end try
    blDebug ( "END AlbaranClienteView::AlbaranClienteView", 0 );
}


/** Destructor de la clase.
    Indica al listado que debe actualizarse.
*/
/// \TODO: Este metodo deberia mejorarse para que indicase al listado que
/// solo debe eliminar una fila del mismo.
/**
**/
AlbaranClienteView::~AlbaranClienteView()
{
    blDebug ( "AlbaranClienteView::~AlbaranClienteView", 0 );
    blDebug ( "END AlbaranClienteView::~AlbaranClienteView", 0 );
}


/// Inicializa todos los elementos del formulario
/**
**/
void AlbaranClienteView::inicializar()
{
    blDebug ( "AlbaranClienteView::inicializar", 0 );
    subform2->inicializar();
    m_descuentos->inicializar();
    dialogChanges_readValues();
    blDebug ( "END AlbaranClienteView::inicializar", 0 );
}



/// Pinta los totales en las casillas correspondientes
/**
\param iva
\param base
\param total
\param desc
\param irpf
\param reqeq
**/
void AlbaranClienteView::pintatotales ( BlFixed iva, BlFixed base, BlFixed total, BlFixed desc, BlFixed irpf, BlFixed reqeq )
{
    blDebug ( "AlbaranClienteView::pintatotales", 0 );
    m_totalBases->setText ( base.toQString() );
    m_totalTaxes->setText ( iva.toQString() );
    m_totalalbaran->setText ( total.toQString() );
    m_totalDiscounts->setText ( desc.toQString() );
    m_totalIRPF->setText ( QString ( irpf.toQString() ) );
    m_totalReqEq->setText ( QString ( reqeq.toQString() ) );
    blDebug ( "END AlbaranClienteView::pintatotales", 0 );
}





/** SLOT de ver el pedidocliente.
    Busca los pedidos a cliente que tienen la misma referencia que el albaran
    y los abre.
*/
/**
**/
void AlbaranClienteView::on_mui_verpedidocliente_clicked()
{
    blDebug ( "AlbaranClienteView::on_mui_verpedidocliente_clicked", 0 );

    PedidoClienteView *bud = NULL;
    BlDbRecordSet *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe el pedido con esos datos, y en caso afirmativo lo mostramos.

        QString SQLQuery = "";

        if ( dbValue ( "refalbaran" ).isEmpty() || dbValue ( "idcliente" ).isEmpty() ) {
            /// El presupuesto no se ha guardado y no se dispone en la base de datos
            /// de estos datos. Se utilizan en su lugar los del formulario.
            /// Verifica que exista, por lo menos, un cliente seleccionado.
            if ( mui_idcliente->id().isEmpty() ) {
                blMsgInfo ( _ ( "Tiene que seleccionar un cliente" ), this );
                return;
            } else {
                SQLQuery = "SELECT * FROM pedidocliente WHERE refpedidocliente = '" + mui_refalbaran->text() + "' AND idcliente = " + mui_idcliente->id();
            } // end if
        } else {
            SQLQuery = "SELECT * FROM pedidocliente WHERE refpedidocliente = '" + dbValue ( "refalbaran" ) + "' AND idcliente = " + dbValue ( "idcliente" );
        } // end if

        cur = mainCompany() ->loadQuery ( SQLQuery );

        if ( !cur->eof() ) {
            while ( !cur->eof() ) {

                /// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
                int resur = g_plugins->lanza ( "SNewPedidoClienteView", ( BfCompany * ) mainCompany() );
                if ( !resur ) {
                    blMsgInfo ( "no se pudo crear instancia de factura" );
                    return;
                } // end if
                bud = ( PedidoClienteView * ) g_plugParams;
                mainCompany() ->m_pWorkspace->addSubWindow ( bud );
                bud->cargar ( cur->value( "idpedidocliente" ) );
                bud->show();
                cur->nextRecord();
            } // end while
        } else {
            blMsgInfo ( _ ( "No hay pedidos con la misma referencia." ), this );
            blDebug ( "no hay pedidos con esta referencia", 2 );
        } // end if

        delete cur;

    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try

    blDebug ( "END AlbaranClienteView::on_mui_verpedidocliente_clicked", 0 );
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
void AlbaranClienteView::generarFactura()
{
    blDebug ( "AlbaranClienteView::generarFactura", 0 );
    /*
        /// Disparamos los plugins.
        int res = g_plugins->lanza ( "AlbaranClienteView_generarFactura", this );
        if ( res != 0 )
            return;

        FacturaView *bud = NULL;
        BlDbRecordSet *cur = NULL;

        try {
            /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
            /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
            /// el siguiente cur->eof() el programa fallar&aacute;.
            /// Comprobamos que existe una factura con esos datos, y en caso afirmativo lo mostramos.

            QString SQLQuery = "";

            if ( dbValue ( "refalbaran" ).isEmpty() || dbValue ( "idcliente" ).isEmpty() ) {
                /// El albaran no se ha guardado y no se dispone en la base de datos
                /// de estos datos. Se utilizan en su lugar los del formulario.
                /// Verifica que exista, por lo menos, un cliente seleccionado.
                if ( mui_idcliente->idcliente().isEmpty() ) {
                    blMsgInfo ( _( "Tiene que seleccionar un cliente" ), this );
                    return;
                } else {
                    SQLQuery = "SELECT * FROM factura WHERE reffactura = '" + mui_refalbaran->text() + "' AND idcliente = " + mui_idcliente->idcliente();
                } // end if
            } else {
                SQLQuery = "SELECT * FROM factura WHERE reffactura = '" + dbValue ( "refalbaran" ) + "' AND idcliente = " + dbValue ( "idcliente" );
            } // end if

            cur = mainCompany() ->loadQuery ( SQLQuery );

            if ( !cur->eof() ) {
                /// Informamos que ya hay una factura y que la abriremos.
                /// Si no salimos de la funci&oacute;n.
                if ( QMessageBox::question ( this,
                                             _( "Factura existente" ),
                                             _( "Existe una factura a este cliente con la misma referencia que este albaran. Desea abrirla para verificar?" ),
                                             _( "&Si" ), _( "&No" ), QString::null, 0, 1 ) ) {
                    return;
                }
                bud = mainCompany() ->newFacturaView();
                mainCompany() ->m_pWorkspace->addSubWindow ( bud );
                bud->cargar ( cur->value( "idfactura" ) );
                bud->show();
                return;
            } // end if
            delete cur;

            /// Creamos la factura.
            bud = mainCompany() ->newFacturaView();
            mainCompany() ->m_pWorkspace->addSubWindow ( bud );

            /// Cargamos un elemento que no existe para inicializar bien la clase.
            bud->cargar ( "0" );

            /// Traspasamos los datos a la factura.
            recogeValores();
            bud->setDbValue ( "comentfactura", dbValue ( "comentalbaran" ) );
            bud->setDbValue ( "idforma_pago", dbValue ( "idforma_pago" ) );
            bud->setDbValue ( "reffactura", dbValue ( "refalbaran" ) );
            bud->setDbValue ( "idcliente", dbValue ( "idcliente" ) );
            bud->setDbValue ( "idalmacen", dbValue ( "idalmacen" ) );

            QString l;
            BlDbSubFormRecord *linea, *linea1;
            for ( int i = 0; i < m_listalineas->rowCount(); ++i ) {
                linea = m_listalineas->lineaat ( i );
                if ( linea->dbValue ( "idarticulo" ) != "" ) {
                    linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                    /// Haciendo el nuevo registro antes nos evitamos problemas de foco.
                    bud->getlistalineas() ->newRecord();
                    bud->getlistalineas() ->setProcesarCambios ( FALSE );
                    linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
                    linea1->setDbValue ( "desclfactura", linea->dbValue ( "desclalbaran" ) );
                    linea1->setDbValue ( "cantlfactura", linea->dbValue ( "cantlalbaran" ) );
                    linea1->setDbValue ( "pvplfactura", linea->dbValue ( "pvplalbaran" ) );
                    linea1->setDbValue ( "ivalfactura", linea->dbValue ( "ivalalbaran" ) );
                    linea1->setDbValue ( "descuentolfactura", linea->dbValue ( "descuentolalbaran" ) );
                    linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
                    linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );
                    bud->getlistalineas() ->setProcesarCambios ( TRUE );
                    linea1->refresh();
                } // end if
            } // end for

            /// Traspasamos los descuentos.
            for ( int i = 0; i < m_listadescuentos->rowCount(); ++i ) {
                linea1 = m_listadescuentos->lineaat ( i );
                if ( linea1->dbValue ( "proporciondalbaran" ) != "" ) {
                    linea = bud->getlistadescuentos() ->lineaat ( bud->getlistadescuentos() ->rowCount() - 1 );
                    bud->getlistadescuentos() ->setProcesarCambios ( FALSE );
                    linea->setDbValue ( "conceptdfactura", linea1->dbValue ( "conceptdalbaran" ) );
                    linea->setDbValue ( "proporciondfactura", linea1->dbValue ( "proporciondalbaran" ) );
                    bud->getlistadescuentos() ->setProcesarCambios ( TRUE );
                    bud->getlistadescuentos() ->newRecord();
                } // end if
            } // end for

            bud->pintar();
            bud->calculaypintatotales();
            bud->show();

            mui_procesadoalbaran->setChecked ( TRUE );

        } catch ( ... ) {
            blMsgInfo ( _( "Error inesperado" ), this );
            if ( cur ) delete cur;
            if ( bud ) delete bud;
        } // end try
    */
    blDebug ( "END AlbaranClienteView::generarFactura", 0 );
}


/// Se encarga de agregar un albaran a una factura ya existente.
/// Para ello presenta un selector de factura y permite escoger a que factura
/// Agregar el albaran.
/**
**/
void AlbaranClienteView::agregarFactura()
{
    /// Pedimos la factura a la que agregar.
    blDebug ( "AlbaranClienteView::agregarFactura", 0 );
    /*
        QDialog *diag = new QDialog ( 0 );
        diag->setWindowTitle ( _( "Seleccione la factura a la que agregar el albaran." ) );
        diag->setModal ( true );

        /// \TODO: Debe pasar por company la creacion del listado
        FacturasList *fac = new FacturasList ( mainCompany(), diag, 0, BL_SELECT_MODE );
        connect ( fac, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

        /// Hacemos que las opciones de filtrado del listado ya est&eacute;n bien.
        fac->m_cliente->setidcliente ( dbValue ( "idcliente" ) );
        fac->on_mui_actualizar_clicked();

        /// Lanzamos el di&aacute;logo.
        diag->exec();
        QString idfactura = fac->idfactura();
        delete diag;

        /// Si no hay idfactura es que hemos abortado y por tanto cancelamos la operaci&oacute;n.
        if ( idfactura == "" )
            return;

        /// Creamos la factura.
        FacturaView *bud = mainCompany() ->newFacturaView();
        bud->cargar ( idfactura );

        /// Agregamos en los comentarios que se ha a&ntilde;adido este albar&aacute;n.
        bud->setDbValue ( "comentfactura", bud->dbValue ( "comentfactura" ) + _( "Num. albaran" ) + dbValue ( "numalbaran" ) + "\n" );

        mainCompany() ->m_pWorkspace->addSubWindow ( bud );
        /// \TODO EN TEORIA SE DEBERIA COMPROBAR QUE LA FACTURA ES DEL MISMO CLIENTE,
        /// PERO POR AHORA PASAMOS DE HACERLO.
        QString l;
        BlDbSubFormRecord *linea, *linea1;
        for ( int i = 0; i < m_listalineas->rowCount(); ++i ) {
            linea = m_listalineas->lineaat ( i );
            if ( linea->dbValue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->setProcesarCambios ( FALSE );
                linea1->setDbValue ( "desclfactura", linea->dbValue ( "desclalbaran" ) );
                linea1->setDbValue ( "cantlfactura", linea->dbValue ( "cantlalbaran" ) );
                linea1->setDbValue ( "pvplfactura", linea->dbValue ( "pvplalbaran" ) );
                linea1->setDbValue ( "descuentolfactura", linea->dbValue ( "descuentolalbaran" ) );
                linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
                linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
                linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );
                linea1->setDbValue ( "ivalfactura", linea->dbValue ( "ivalalbaran" ) );
                bud->getlistalineas() ->setProcesarCambios ( TRUE );
                bud->getlistalineas() ->newRecord();
            } // end if
        } // end for
        bud->calculaypintatotales();
        bud->show();
        mui_procesadoalbaran->setChecked ( TRUE );
    */
    blDebug ( "END AlbaranClienteView::agregarFactura", 0 );
}



///
/**
\param id
**/
void AlbaranClienteView::on_mui_idcliente_valueChanged ( QString id )
{
    blDebug ( "AlbaranClienteView::on_m_cliente_valueChanged", 0 );
    subform2->setIdCliente ( id );
    mui_idforma_pago->setIdCliente ( id );
    blDebug ( "END AlbaranClienteView::on_m_cliente_valueChanged", 0 );
}






/// Este slot se activa cuando hay cambios en los subformularios.
/**
**/
void AlbaranClienteView::s_pintaTotales()
{
    blDebug ( "AlbaranClienteView::s_pintaTotales", 0 );
    calculaypintatotales();
    blDebug ( "END AlbaranClienteView::s_pintaTotales", 0 );
}


///
/**
**/
void AlbaranClienteView::on_mui_facturar_clicked()
{
    blDebug ( "AlbaranClienteView::on_mui_facturar_clicked", 0 );
    generarFactura();
    blDebug ( "END AlbaranClienteView::on_mui_facturar_clicked", 0 );
}


///
/**
**/
void AlbaranClienteView::on_mui_agregarafactura_clicked()
{
    blDebug ( "AlbaranClienteView::on_mui_agregarafactura_clicked", 0 );
    agregarFactura();
    blDebug ( "END AlbaranClienteView::on_mui_agregarafactura_clicked", 0 );
}


///
/**
**/
void AlbaranClienteView::on_m_descuentos_editFinish ( int, int )
{
    blDebug ( "AlbaranClienteView::on_m_descuentos_editFinish", 0 );
    calculaypintatotales();
    blDebug ( "END AlbaranClienteView::on_m_descuentos_editFinish", 0 );
}


///
/**
**/
void AlbaranClienteView::on_subform2_editFinish ( int, int )
{
    blDebug ( "AlbaranClienteView::on_subform2_editFinish", 0 );
    calculaypintatotales();
    blDebug ( "END AlbaranClienteView::on_subform2_editFinish", 0 );
}



/** Metodo de borrar un albaran. Se encarga de mandar a la base de datos
    la instruccion necesaria para el borrado de un albaran y controlar
    los posibles errores que se produzcan.
    Tambi&eacute;n borra todas las lineas y las lineas de descuento que se refieren
    a el albaran que se pretende borrar.
*/
/// \todo: Este metodo deberia poderse delegar en BlDbRecord, o por lo menos la parte del borrado del registro.
/// \todo: Hace falta meter el metodo dentro del sistema de excepciones try catch.
/**
\return
**/
int AlbaranClienteView::borrarPre()
{
    blDebug ( "AlbaranClienteView::borrar", 0 );
    m_listalineas->borrar();
    m_listadescuentos->borrar();
    blDebug ( "END AlbaranClienteView::borrar", 0 );
    return 0;
}



/** Este m&eacute;todo carga un AlbaranCliente. Tambi&eacute;n carga las lineas
    de albar&aacute;n y los descuentos de albar&aacute;n.
    Tras la carga tambi&eacute;n invoca un repintado del albaran para que se vea
    correctamente la pantalla.
*/
/**
\param idalbaran
\return
**/
int AlbaranClienteView::cargarPost ( QString idalbaran )
{
    blDebug ( "AlbaranClienteView::cargar", 0 );

    m_listalineas->cargar ( idalbaran );
    m_listadescuentos->cargar ( idalbaran );

    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    g_plugins->lanza ( "AlbaranCliente_cargarPost_Post", this );

    calculaypintatotales();
    blDebug ( "END AlbaranClienteView::cargar", 0 );
    return 0;
}


/** Este m&eacute;todo se encarga de hacer el guardado del albar&aacute; en la
    base de datos. Una vez guardado se guardan las lineas de albar&aacute;n y los
    descuentos de albar&aacute;n. Tras hacer el guardado se hace una carga del albaran
    para recuperar los datos que haya podido escribir la base de datos automaticamente.
    Dichos datos son la referencia y el n&uacute;mero de albar&aacute;n.

    Si todo funciona bien este m&eacute;todo devuelve 0. Si se produce algun error
    se genera una excepcion -1.
*/
/**
\return
**/
int AlbaranClienteView::guardarPost()
{
    blDebug ( "AlbaranClienteView::guardarPost", 0 );

    m_listalineas->setColumnValue ( "idalbaran", dbValue ( "idalbaran" ) );
    m_listalineas->guardar();
    m_listadescuentos->setColumnValue ( "idalbaran", dbValue ( "idalbaran" ) );
    m_listadescuentos->guardar();

    blDebug ( "END AlbaranClienteView::guardarPost", 0 );
    return 0;
}


///
/**
\param id
**/
void AlbaranClienteView::on_mui_idalmacen_valueChanged ( QString id )
{
    blDebug ( "AlbaranClienteView::on_mui_idalmacen_valueChanged", 0 );
    m_listalineas->setIdAlmacen ( id );
    blDebug ( "END AlbaranClienteView::on_mui_idalmacen_valueChanged", 0 );
}

