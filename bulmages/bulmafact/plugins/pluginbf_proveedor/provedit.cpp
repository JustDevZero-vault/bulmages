/***************************************************************************
 *   Copyright (C) 2004 by J. M. Torres Rigo                               *
 *   joanmi@bulma.net                                                      *
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

#include <QLineEdit>
#include <QMessageBox>
#include <QCloseEvent>

#include "company.h"
#include "funcaux.h"
#include "plugins.h"
#include "provedit.h"


///
/**
\param comp
\param parent
**/
ProveedorView::ProveedorView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "ProveedorView::ProveedorView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setTitleName ( _( "Proveedor" ) );
        setDBTableName ( "proveedor" );
        setDBCampoId ( "idproveedor" );
        addDBCampo ( "idproveedor", DBCampo::DBint, DBCampo::DBPrimaryKey, _( "ID proveedor" ) );
        addDBCampo ( "nomproveedor", DBCampo::DBvarchar, DBCampo::DBNotNull, _( "Nombre del proveedor" ) );
        addDBCampo ( "nomaltproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Nombre alternativo del proveedor" ) );
        addDBCampo ( "cifproveedor", DBCampo::DBvarchar, DBCampo::DBNotNull, _( "C.I.F. del proveedor" ) );
        addDBCampo ( "codicliproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Codigo cliente" ) );
        addDBCampo ( "cbancproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Banco proveedor" ) );
        addDBCampo ( "dirproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Direccion" ) );
        addDBCampo ( "poblproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Poblacion" ) );
        addDBCampo ( "idprovincia", DBCampo::DBint, DBCampo::DBNothing, _( "Provincia" ) );
        addDBCampo ( "cpproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Codigo postal" ) );
        addDBCampo ( "telproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Numero de telefono" ) );
        addDBCampo ( "faxproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Numero de fax" ) );
        addDBCampo ( "emailproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Direccion electronica" ) );
        addDBCampo ( "urlproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, _( "URL" ) );
        addDBCampo ( "comentproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Comentarios" ) );
        addDBCampo ( "codproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Codigo" ) );
        addDBCampo ( "regimenfiscalproveedor", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Regimen Fiscal" ) );
        addDBCampo ( "idforma_pago", DBCampo::DBint, DBCampo::DBNothing, _( "Forma_Pago" ) );
        addDBCampo ( "recargoeqproveedor", DBCampo::DBboolean, DBCampo::DBNothing, _( "Recargo de Equivalencia" ) );
        addDBCampo ( "irpfproveedor", DBCampo::DBnumeric, DBCampo::DBNothing, _( "IRPF" ) );

        setupUi ( this );

        /// Deshabilitamos los tabs que aun no se usan.
        mui_tab->setTabEnabled ( 5, FALSE );
        mui_tab->setTabEnabled ( 6, FALSE );
        mui_tab->setTabEnabled ( 7, FALSE );

        /// Cargamos algunos valores por defecto.
        mui_idforma_pago->setEmpresaBase ( empresaBase() );
        mui_idforma_pago->setidforma_pago ( "0" );

        mui_idprovincia->setEmpresaBase ( empresaBase() );
    mui_idprovincia->setQuery("SELECT * FROM provincia LEFT JOIN pais ON provincia.idpais = pais.idpais ORDER BY descpais, provincia");
    mui_idprovincia->setTableName ("provincia");
    mui_idprovincia->setCampoId ("idprovincia");
    mui_idprovincia->m_valores["provincia"] = "";
    mui_idprovincia->m_valores["descpais"] = "";
        mui_idprovincia->setId ( "" );


        meteWindow ( windowTitle(), this, FALSE );
        dialogChanges_cargaInicial();

        /// Disparamos los plugins.
        g_plugins->lanza ( "ProveedorView_ProveedorView_Post", this );
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear el proveedor" ) );
    } // end try

    _depura ( "END ProveedorView::ProveedorView", 0 );
}

///
/**
**/
ProveedorView::~ProveedorView()
{
    _depura ( "ProveedorView::~ProveedorView", 0 );
    empresaBase() ->refreshProveedores();
    _depura ( "END ProveedorView::~ProveedorView", 0 );
}



/**
*/
/**
**/
void ProveedorView::on_mui_cifproveedor_lostFocus()
{
    _depura ( "ProveedorView::on_mui_cifproveedor_lostFocus", 0 );
	QChar digito;
	if (!validarCIFNIF(mui_cifproveedor->text(), digito)) {
		mensajeInfo("Error en el CIF del proveedor. Digito:" + QString(digito));
	} // end if
    _depura ( "END ProveedorView::on_mui_cifproveedor_lostFocus", 0 );
}




/// Esta funcion carga un proveedor de la base de datos y lo presenta.
/// Si el parametro pasado no es un identificador valido entonces se pone
/// la ventana de edicion en modo de insercion.
/**
\param idprov
\return
**/
int ProveedorView::cargarPost ( QString idprov )
{
    _depura ( "ProveedorView::cargar", 0, idprov );
    /// Lanzamos los plugins de carga
    g_plugins->lanza("ProveedorView_cargarPost_Post", this);
    _depura ( "END ProveedorView::cargar", 0 );
    return 0;
}


