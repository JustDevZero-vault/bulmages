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

#include <QCheckBox>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "albaranclientelist.h"
#include "albaranclienteview.h"
#include "blplugins.h"
#include "bfcompany.h"
#include "blfunctions.h"


///
/**
**/
void TicketClienteListSubform::cargar()
{
    blDebug ( "TicketClienteListSubform::cargar", 0 );
    QString SQLQuery = "SELECT * FROM albaran";
    BlSubForm::cargar ( SQLQuery );
    blDebug ( "END TicketClienteListSubform::cargar", 0 );
}


///
/**
\param query
**/
void TicketClienteListSubform::cargar ( QString query )
{
    blDebug ( "TicketClienteListSubform::cargar", 0 );
    BlSubForm::cargar ( query );
    blDebug ( "END TicketClienteListSubform::cargar", 0 );
}


///
/**
\param val
**/
void TicketClienteList::setidcliente ( QString val )
{
    blDebug ( "TicketClienteList::setidcliente", 0 );
    m_cliente->setId ( val );
    blDebug ( "END TicketClienteList::setidcliente", 0 );
}


///
/**
\return
**/
QString TicketClienteList::idCliDelivNote()
{
    blDebug ( "TicketClienteList::idCliDelivNote", 0 );
    blDebug ( "END TicketClienteList::idCliDelivNote", 0 );
    return mdb_idalbaran;
}


///
/**
**/
void TicketClienteList::crear()
{
    blDebug ( "TicketClienteList::crear", 0 );
    TicketClienteView *acv = new TicketClienteView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany()->m_pWorkspace->addSubWindow ( acv );
    acv->inicializar();
    acv->pintar();
    
    /// Si se crea el nuevo albaran desde la lista de albaranes de un cliente,
    /// entonces se crea el albaran para este cliente.
    if (m_cliente->id() != "") {
	acv->mui_idcliente->setId( m_cliente->id() );
    } // end if

    acv->show();
    acv->mui_descalbaran->setFocus ( Qt::OtherFocusReason );
    blDebug ( "END TicketClienteList::crear", 0 );
}


/** Constructor de la clase sin inicializacion de company. Usando este
    constructor no se olvide de usar setcompany para que la clase no de
    excepciones.
    El constructor crea la pantalla, la pone en editmodo por defecto y la mete
    en el listado de ventanas.
    Usando esta clase tampoco se inicializan bien los widgets que contiene.
*/
/**
\param parent
\param flag
\param editmodo
\return
**/
TicketClienteList::TicketClienteList ( QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    blDebug ( "TicketClienteList::TicketClienteList", 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "TicketClienteList_TicketClienteList", this );
    if ( res != 0 )
        return;
    mdb_idalbaran = "";
    setSubForm ( mui_list );
    hideBusqueda();
    iniciaForm();
    /// Llamamos a los scripts
    blScript(this);
    blDebug ( "END TicketClienteList::TicketClienteList", 0 );
}


/** Constructor de la clase.
    Inicializa toda la ventana y los widgets que esta contiene.
    Hace una presentacion inicial de los albaranes de cliente.
    Pone la pantalla en modo de edicion por defecto  a no ser que se especifique lo contrario.
    Mete la ventana en el workspace.
*/
/**
\param comp
\param parent
\param flag
\param editmodo
\return
**/
TicketClienteList::TicketClienteList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    blDebug ( "TicketClienteList::TicketClienteList", 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "TicketClienteList_TicketClienteList", this );
    if ( res != 0 )
        return;
    m_cliente->setMainCompany ( comp );
    m_articulo->setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    setSubForm ( mui_list );
    iniciaForm();

    /// Establecemos los parametros de busqueda del Cliente
    m_cliente->setLabel ( _ ( "Cliente:" ) );
    m_cliente->setTableName ( "cliente" );
    m_cliente->m_valores["cifcliente"] = "";
    m_cliente->m_valores["nomcliente"] = "";

    /// Iniciamos el buscador de trabajadores.
    m_idtrabajador->setMainCompany ( mainCompany() );
    m_idtrabajador->setQuery ( "SELECT * FROM trabajador ORDER BY apellidostrabajador, nomtrabajador" );
    m_idtrabajador->setTableName ( "trabajador" );
    m_idtrabajador->setFieldId ( "idtrabajador" );
    m_idtrabajador->m_valores["apellidostrabajador"] = "";
    m_idtrabajador->m_valores["nomtrabajador"] = "";
    m_idtrabajador->setAllowNull ( TRUE );
    m_idtrabajador->setId("");
    
    presentar();
    mdb_idalbaran = "";
    if ( modoEdicion() )
        mainCompany() ->meteWindow ( windowTitle(), this );
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "albaran" );
    /// Llamamos a los scripts
    blScript(this);
    blDebug ( "END TicketClienteList::TicketClienteList", 0 );
}


///
/**
\return
**/
void TicketClienteList::iniciaForm()
{
    blDebug ( "TicketClienteList::iniciaForm" );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "TicketClienteList_iniciaForm", this );
    if ( res != 0 )
        return;
    mui_procesada->insertItem ( 0, _ ( "Todos los tickets" ) );
    mui_procesada->insertItem ( 1, _ ( "Tickets procesados" ) );
    mui_procesada->insertItem ( 2, _ ( "Tickets no procesados" ) );
    mui_procesada->setCurrentIndex ( 2 );
    blDebug ( "END TicketClienteList::iniciaForm" );
}


///
/**
\param comp
**/
void TicketClienteList::setMainCompany ( BfCompany *comp )
{
    blDebug ( "TicketClienteList::setMainCompany", 0 );
    BlMainCompanyPointer::setMainCompany ( comp );
    m_cliente->setMainCompany ( comp );
    m_articulo->setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    
    
    /// Iniciamos el buscador de trabajadores.
    m_idtrabajador->setMainCompany ( mainCompany() );
    m_idtrabajador->setQuery ( "SELECT * FROM trabajador ORDER BY apellidostrabajador, nomtrabajador" );
    m_idtrabajador->setTableName ( "trabajador" );
    m_idtrabajador->setFieldId ( "idtrabajador" );
    m_idtrabajador->m_valores["apellidostrabajador"] = "";
    m_idtrabajador->m_valores["nomtrabajador"] = "";
    m_idtrabajador->setAllowNull ( TRUE );   
    m_idtrabajador->setId("");
    
    blDebug ( "END TicketClienteList::setMainCompany", 0 );
}


/** Destructor de la clase */
/**
**/
TicketClienteList::~TicketClienteList()
{
    blDebug ( "TicketClienteList::~TicketClienteList", 0 );
    blDebug ( "END TicketClienteList::~TicketClienteList", 0 );
}


/** Carga el listado de la base de datos y lo presenta.
    Tambien carga el total y lo presenta.
*/
/**
**/
void TicketClienteList::presentar()
{
    blDebug ( "TicketClienteList::presentar" );

    mui_list->cargar ( "SELECT *, totalalbaran AS total, bimpalbaran AS base, impalbaran AS impuestos FROM albaran LEFT JOIN  cliente ON albaran.idcliente = cliente.idcliente LEFT JOIN almacen ON albaran.idalmacen = almacen.idalmacen LEFT JOIN forma_pago ON albaran.idforma_pago = forma_pago.idforma_pago WHERE 1 = 1 " + generarFiltro() );

    /// Hacemos el calculo del total.
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT SUM(totalalbaran) AS total FROM albaran LEFT JOIN cliente ON albaran.idcliente=cliente.idcliente LEFT JOIN almacen ON almacen.idalmacen = albaran.idalmacen where 1 = 1 " + generarFiltro() );
    /// Esta consulta podria resultar NULA y debe tratarse el caso
    if ( cur ) {
        m_total->setText ( cur->valor ( "total" ) );
        delete cur;
    } // end if

    blDebug ( "END TicketClienteList::presentar" );
}


/**  Este metodo se activa cuando bien pulsando sobre el boton de editar
     o bien haciendo doble click en el modo de edicion se desea invocar la accion
     Editar el elemento si estamos en modo editmode o cerrar la ventana y emitir
     un signal selected() si estamos en el modo selector.

     Primero determina el idalbaran seleccionado, luego crea la instancia de
     la ventana de edicion TicketClienteView y lo mete en el workspace.
     Por ultimo hace que dicha ventana carge de la base de datos el idalbaran
     seleccionado.
*/
/**
\param row
\return
**/
void TicketClienteList::editar ( int row )
{
    blDebug ( "TicketClienteList::editar", 0 );
    mdb_idalbaran = mui_list->dbValue ( QString ( "idalbaran" ), row );
    if ( modoEdicion() ) {
        TicketClienteView * prov = new TicketClienteView ( ( BfCompany * ) mainCompany(), 0 );
        if ( prov->cargar ( mdb_idalbaran ) ) {
            delete prov;
            return;
        } // end if
        mainCompany() ->m_pWorkspace->addSubWindow ( prov );
        prov->show();
    } else {
        emit ( selected ( mdb_idalbaran ) );
    } // end if
    blDebug ( "END TicketClienteList::editar", 0 );
}



/** Responde a la pulsacion del boton borrar en el formulario.
    Carga el elemento seleccionado e invoca a su metodo de borrar.
*/
/// \todo: Comprobar que se libera bien la memoria.
/// \todo: Intentar que no se tenga que recargar todo el listado y que simplemente se borre la fila seleccionada.
/**
\return
**/
void TicketClienteList::borrar()
{
    blDebug ( "TicketClienteList::borrar", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Debe seleccionar una linea" ), this );
        return;
    } // end if
    try {
        mdb_idalbaran = mui_list->dbValue ( QString ( "idalbaran" ) );
        if ( modoEdicion() ) {
            TicketClienteView * acv = new TicketClienteView ( ( BfCompany * ) mainCompany(), 0 );
            if ( acv->cargar ( mdb_idalbaran ) )
                throw - 1;
            acv->on_mui_borrar_clicked();
            acv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al borrar el albaran a cliente" ), this );
    } // end try
    blDebug ( "END TicketClienteList::borrar", 0 );
}


/** La impresion de listados es estandarizada por la clase BlDbRecord
*/
void TicketClienteList::imprimir()
{
    blDebug ( "TicketClienteList::imprimir", 0 );
    mui_list->imprimirPDF ( _ ( "Ticketes a clientes" ) );
    blDebug ( "END TicketClienteList::imprimir", 0 );
}


/** Este metodo ayuda a construir el query de filtrado construyendo la clausula
    WHERE a partir de las opciones de filtrado.
*/
/**
\return
**/
QString TicketClienteList::generarFiltro()
{
    /// Tratamiento de los filtros.
    blDebug ( "TicketClienteList::generarFiltro", 0 );
    QString filtro = "";

    if ( m_filtro->text() != "" ) {
        filtro = " AND ( lower(descalbaran) LIKE lower('%" + m_filtro->text() + "%') ";
        filtro += " OR refalbaran LIKE '" + m_filtro->text() + "%' ";
        filtro += " OR lower(nomcliente) LIKE lower('%" + m_filtro->text() + "%')) ";
    } else {
        filtro = "";
    } // end if

    if ( m_cliente->id() != "" )
        filtro += " AND albaran.idcliente='" + m_cliente->id() + "'";

    if ( m_articulo->idarticulo() != "" )
        filtro += " AND idalbaran IN (SELECT DISTINCT idalbaran FROM lalbaran WHERE idarticulo='" + m_articulo->idarticulo() + "')";

    /// Tratamos los elementos procesados y no procesados.
    if ( mui_procesada->currentIndex() == 1 ) {
        /// Muestra solo las procesadas.
        filtro += " AND procesadoalbaran";
    } else if ( mui_procesada->currentIndex() == 2 ) {
        /// Muestra solo las NO procesadas.
        filtro += " AND NOT procesadoalbaran ";
    } // end if

    if ( m_fechain->text() != "" )
        filtro += " AND fechaalbaran >= '" + m_fechain->text() + "' ";

    if ( m_fechafin->text() != "" )
        filtro += " AND fechaalbaran <= '" + m_fechafin->text() + "' ";

    if (m_idtrabajador->id() != "") {
	filtro += " AND idtrabajador = " + m_idtrabajador->id();
    } // end if
    
    blDebug ( "END TicketClienteList::generarFiltro", 0 );
    return ( filtro );
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Constructor del subformulario de albaranes a cliente.
    Configura el subformulario para que use la tabla albaran.
*/
/**
\param parent
\return
**/
TicketClienteListSubform::TicketClienteListSubform ( QWidget *parent ) : BfSubForm ( parent )
{
    blDebug ( "TicketClienteListSubform::TicketClienteListSubform", 0 );

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "TicketClienteListSubform_TicketClienteListSubform", this );
    if ( res != 0 )
        return;
    setDbTableName ( "albaran" );
    setDbFieldId ( "idalbaran" );
    addSubFormHeader ( "refalbaran", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Referencia de albaran" ) );
    addSubFormHeader ( "codigoalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Codigo de almacen" ) );
    addSubFormHeader ( "numalbaran", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Numero de albaran" ) );
    addSubFormHeader ( "descalbaran", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Descripcion" ) );
    addSubFormHeader ( "fechaalbaran", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha de albaran" ) );
    addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre de cliente" ) );
    addSubFormHeader ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "ID forma de pago" ) );
    addSubFormHeader ( "descforma_pago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Descripcion de forma de pago" ) );
    addSubFormHeader ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "ID trabajador" ) );
    addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "ID cliente" ) );
    addSubFormHeader ( "idalbaran", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "ID albaran" ) );
    addSubFormHeader ( "comentalbaran", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Comentarios del albaran" ) );
    addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "ID almacen" ) );
    addSubFormHeader ( "base", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Base imponible" ) );
    addSubFormHeader ( "impuestos", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Impuestos" ) );
    addSubFormHeader ( "total", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Total" ) );
    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    blDebug ( "END TicketClienteListSubform::TicketClienteListSubform", 0 );
}


///
/**
**/
TicketClienteListSubform::~TicketClienteListSubform()
{
    blDebug ( "TicketClienteListSubform::~TicketClienteListSubform", 0 );
    blDebug ( "END TicketClienteListSubform::~TicketClienteListSubform", 0 );
}