/***************************************************************************
 *   Copyright (C) 2009 by Xavier Rusiñol Escriu                           *
 *   russi7@gmail.com                                                      *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
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

#include <fstream>

#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>

#include "socioview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"
#include "blpostgresqlclient.h"

/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
SocioView::SocioView ( BfCompany *comp, QWidget *parent ) : BfForm ( comp, parent )
{
    _depura ( "SocioView::SocioView", 0 );
    
    setAttribute ( Qt::WA_DeleteOnClose );
    
    try {
    
        setupUi ( this );
        centrarEnPantalla ( this );

        setTitleName ( _ ( "Socio" ) );
        setDbTableName ( "cliente" );
        setDbFieldId ( "idcliente" );
        addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID tutor" ) );
        addDbField ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre del tutor" ) );
        addDbField ( "numsociocliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre del tutor" ) );
        addDbField ( "sociocliente", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Es Socio" ) );
        addDbField ( "cifcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "codcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "dircliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "poblcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "bancocliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "cpcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "telcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "teltrabcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "movilcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "faxcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "mailcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Forma de Pago" ) );
        addDbField ( "idprovincia", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Provincia" ) );

	addDbField ( "faltacliente", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha de alta" ) );
	addDbField ( "fbajacliente", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha de baja" ) );
	addDbField ( "fnacimientocliente", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha de nacimiento" ) );
	addDbField ( "profesioncliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Profesion" ) );
	addDbField ( "idtiposocio", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Tipo de socio" ) );
	addDbField ( "cuotafamiliarcliente", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Cuota familiar" ) );
	addDbField ( "nombretitularbancocliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre titular" ) );
	addDbField ( "apellidotitularbancocliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Apellidos titular" ) );
	addDbField ( "entidadbancocliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Entidad bancaria" ) );


        mui_alumnosList->setMainCompany ( mainCompany() );
        mui_idforma_pago->setMainCompany( mainCompany() );
        mui_idprovincia->setMainCompany ( mainCompany() );
        mui_idprovincia->setQuery ( "SELECT * FROM provincia LEFT JOIN pais ON provincia.idpais = pais.idpais ORDER BY descpais, provincia" );
        mui_idprovincia->setTableName ( "provincia" );
        mui_idprovincia->setFieldId ( "idprovincia" );
        mui_idprovincia->m_valores["provincia"] = "";
        mui_idprovincia->m_valores["descpais"] = "";
        mui_idprovincia->setAllowNull ( TRUE );
        mui_idprovincia->setId ( "" );

	mui_idtiposocio->setMainCompany ( mainCompany() );
        mui_idtiposocio->setQuery ( "SELECT * FROM tiposocio ORDER BY idtiposocio" );
        mui_idtiposocio->setTableName ( "tiposocio" );
        mui_idtiposocio->setFieldId ( "idtiposocio" );
        mui_idtiposocio->m_valores["nombretiposocio"] = "";
        mui_idtiposocio->m_valores["cuotatiposocio"] = "";
        mui_idtiposocio->setAllowNull ( TRUE );
        mui_idtiposocio->setId ( "" );



	mui_listrecibos->setMainCompany( mainCompany() );
	mui_listrecibos->setDbTableName ( "recibo" );
	mui_listrecibos->setDbFieldId ( "idrecibo" );
	mui_listrecibos->addSubFormHeader ( "idrecibo", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Recibo" ) );
	mui_listrecibos->addSubFormHeader ( "cantrecibo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Cantidad" ) );
	mui_listrecibos->addSubFormHeader ( "pagadorecibo", BlDbField::DbBoolean, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Pagado" ) );
	mui_listrecibos->addSubFormHeader ( "fecharecibo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha" ) );
	mui_listrecibos->addSubFormHeader ( "idcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "ID Tutor" ) );
	mui_listrecibos->addSubFormHeader ( "descforma_pago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Forma de Pago" ) );
	mui_listrecibos->setInsert ( FALSE );
	mui_listrecibos->setSortingEnabled ( TRUE );

// =========================================


        meteWindow ( windowTitle(), this, FALSE );
        pintar();
        dialogChanges_cargaInicial();
        
    } catch ( ... ) {
    
        mensajeInfo ( _ ( "Error al crear el tutor" ), this );
        
    } // end try
    
    _depura ( "END SocioView::SocioView", 0 );
}

/** No precisa acciones adicionales en el destructor.
*/
SocioView::~SocioView()
{
    _depura ( "SocioView::~SocioView", 0 );
    _depura ( "END SocioView::~SocioView", 0 );
}

void SocioView::on_mui_sociocliente_toggled ( bool state )
{
    _depura ( "SocioView::on_mui_sociocliente_toggled", 0 );
    
    if ( mui_numsociocliente->text() == "" && state ) {
        QString query = "SELECT COALESCE(max(numsociocliente) + 1 , 0) AS numsoc FROM cliente";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
        mui_numsociocliente->setText ( cur->valor ( "numsoc" ) );
    } // end if
    
    _depura ( "END SocioView::on_mui_sociocliente_toggled", 0 );
}

QString SocioView::nombrePlantilla ( void )
{
    _depura ( "SocioView::nombrePlantilla", 0 );
    _depura ( "END SocioView::nombrePlantilla", 0 );
    
    return QString ( "tutor" );
}

void SocioView::imprimir()
{
    _depura ( "SocioView::imprimir", 0 );
    
    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";

    if ( dbValue ( "idcliente" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        mensajeInfo ( _ ( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        return;
    }
    
    /// Disparamos los plugins
    int res = g_plugins->lanza ( "SocioView_imprimir", this );
    if ( res != 0 ) {
        return;
    } // end if
    
    BfForm::imprimir();

    _depura ( "END SocioView::imprimir", 0 );
}

int SocioView::guardarPost()
{
    _depura ( "SocioView::guardarPost", 0 );

    mui_alumnosList->setColumnValue ( "idcliente", dbValue ( "idcliente" ) );
    mui_alumnosList->guardar();
    
    _depura ( "END SocioView::guardarPost", 0 );
    
    return 0;
}

int SocioView::borrarPre()
{
    _depura ( "SocioView::borrarPre", 0 );

    /*
        QString query = "DELETE FROM socio WHERE idcliente=" + dbValue("idcliente");
        mainCompany()->runQuery(query);
    */
    
    QString query = "DELETE FROM alumnocliente WHERE idcliente=" + dbValue ( "idcliente" );
    mainCompany()->runQuery ( query );
    
    _depura ( "END SocioView::borrarPre", 0 );
    
    return 0;
}

int SocioView::cargarPost ( QString id )
{
    _depura ( " SocioView::cargarPost", 0 );

    /*
        QString query1 = "SELECT * FROM socio WHERE idcliente = " + id;
        BlDbRecordSet *cur = mainCompany()->loadQuery(query1);
        if (!cur->eof()) {
          mui_numsocio->setText(cur->valor("numsocio"));
        } // end if
        delete cur;
    */
    
    mui_alumnosList->cargar ( id );

    mui_listrecibos->cargar("SELECT * FROM recibo LEFT JOIN forma_pago ON recibo.idforma_pago = forma_pago.idforma_pago LEFT JOIN banco ON recibo.idbanco = banco.idbanco WHERE recibo.idcliente = " + id);

    _depura ( "END SocioView::cargarPost", 0 );
    
    return 0;
}

/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================

///
/**
\param parent
**/
ListAlumnosSocioView::ListAlumnosSocioView ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListAlumnosSocioView::ListAlumnosSocioView", 0 );
    
    setDbTableName ( "alumnocliente" );
    setDbFieldId ( "idalumnocliente" );
    addSubFormHeader ( "idalumnocliente", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _ ( "Identificador" ) );
    addSubFormHeader ( "idalumno", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired , BlSubFormHeader::DbHideView, _ ( "Id alumno" ) );
    addSubFormHeader ( "nombrealumno1", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre alumno" ) );
    addSubFormHeader ( "porcentalumnocliente", BlDbField::DbNumeric, BlDbField::DbNotNull | BlDbField::DbRequired , BlSubFormHeader::DbHideView, _ ( "Porcentaje Fact." ) );
    addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView, _ ( "Id tutor" ) );

    setInsert ( TRUE );
    setSortingEnabled ( TRUE );
    
    _depura ( "END ListAlumnosSocioView::ListAlumnosSocioView", 0 );
}

///
/**
\param idcliente
**/
void ListAlumnosSocioView::cargar ( QString idcliente )
{
    _depura ( "ListAlumnosSocioView::cargar", 0 );
    
    BlSubForm::cargar ( "SELECT *, (apellido1alumno || ' ' || apellido2alumno || ', ' || nombrealumno) AS nombrealumno1 FROM alumnocliente LEFT JOIN alumno ON alumnocliente.idalumno = alumno.idalumno  WHERE alumnocliente.idcliente=" + idcliente  );
    
    _depura ( "END ListAlumnosSocioView::cargar", 0 );
}