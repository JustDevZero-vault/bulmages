/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#include "actividadview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"

/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
ActividadView::ActividadView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    blDebug ( "ActividadView::ActividadView", 0 );
    
    setAttribute ( Qt::WA_DeleteOnClose );
    
    try {
        setupUi ( this );
        blCenterOnScreen ( this );

        setTitleName ( _ ( "Actividad" ) );
        setDbTableName ( "actividad" );
        setDbFieldId ( "idactividad" );
        addDbField ( "idactividad", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID actividad" ) );
        addDbField ( "idtipoactividad", BlDbField::DbInt, BlDbField::DbNothing, _ ( "ID tipo actividad" ) );
        addDbField ( "nombreactividad", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre del actividad" ) );
        addDbField ( "codigoactividad", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Codigo" ) );
        addDbField ( "idprofesor", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id profesor" ) );
        addDbField ( "precioactividad", BlDbField::DbNumeric, BlDbField::DbNothing, _ ( "Precio" ) );
        addDbField ( "finicialactividad", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha Inicial" ) );
        addDbField ( "ffinalactividad", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha Final" ) );
        addDbField ( "vigenteactividad", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Vigente" ) );
        addDbField ( "comentariosactividad", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios" ) );
        addDbField ( "lunesactividad", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Lunes" ) );
        addDbField ( "martesactividad", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Martes" ) );
        addDbField ( "miercolesactividad", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Miercoles" ) );
        addDbField ( "juevesactividad", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Jueves" ) );
        addDbField ( "viernesactividad", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Viernes" ) );
        addDbField ( "sabadoactividad", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Sabado" ) );
        addDbField ( "domingoactividad", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Domingo" ) );
        addDbField ( "periodoactividad", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Periodo" ) );
        addDbField ( "hinicialactividad", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Hora Inicio" ) );
        addDbField ( "duracionactividad", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Duracion" ) );
        addDbField ( "lugaractividad", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Lugar" ) );
        addDbField ( "diractividad", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Direccion" ) );
	
        /// Disparamos los plugins.
        int res = g_plugins->lanza ( "ActividadView_ActividadView", this );
        if ( res != 0 ) {
            return;
        } // end if

        meteWindow ( windowTitle(), this, FALSE );

        /// Inicializamos el subformulario de alumnos
        mui_alumnosList->setMainCompany ( comp );
        mui_faltasAsistenciaList->setMainCompany ( comp );
	mui_tutoresList->setMainCompany( comp );

        /// Establecemos los parametros de busqueda de Profesor
        mui_idprofesor->setMainCompany ( comp );
        mui_idprofesor->setLabel ( _ ( "Profesor:" ) );
        mui_idprofesor->setTableName ( "profesor" );
        mui_idprofesor->setFieldId("idprofesor");
        mui_idprofesor->m_valores["nombreprofesor"] = "";

        /// Establecemos los parametros de busqueda de tipo de actividad.
        mui_idtipoactividad->setMainCompany ( mainCompany() );
        mui_idtipoactividad->setQuery ( "SELECT * FROM tipoactividad ORDER BY ascii(nombretipoactividad), nombretipoactividad" );
        mui_idtipoactividad->setTableName ( "tipoactividad" );
        mui_idtipoactividad->setFieldId ( "idtipoactividad" );
        mui_idtipoactividad->m_valores["nombretipoactividad"] = "";
        mui_idtipoactividad->setAllowNull ( FALSE );

        pintar();
        dialogChanges_cargaInicial();

        mui_idtipoactividad->setId ( "" );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el actividad" ), this );
    } // end try
    
    blDebug ( "END ActividadView::ActividadView", 0 );
}

/** No precisa acciones adicionales en el destructor.
*/
ActividadView::~ActividadView()
{
    blDebug ( "ActividadView::~ActividadView", 0 );
    blDebug ( "END ActividadView::~ActividadView", 0 );
}

QString ActividadView::nombrePlantilla ( void )
{
    blDebug ( "ActividadView::nombrePlantilla", 0 );
    blDebug ( "END ActividadView::nombrePlantilla", 0 );
    
    return QString ( "actividad" );
}

void ActividadView::imprimir()
{
    blDebug ( "ActividadView::imprimir", 0 );
    
    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";

    if ( dbValue ( "idprofesor" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        blMsgInfo ( _ ( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        return;
    }
    
    /// Disparamos los plugins
    int res = g_plugins->lanza ( "CoboView_on_mui_imprimir_clicked", this );
    if ( res != 0 ) {
        return;
    } // end if
    
    BfForm::imprimir();

    blDebug ( "END ActividadView::imprimir", 0 );
}

int ActividadView::guardarPost()
{
    blDebug ( "ActividadView::guardarPost", 0 );
    
    mui_alumnosList->setColumnValue ( "idactividad", dbValue ( "idactividad" ) );
    mui_alumnosList->guardar();
    
    mui_faltasAsistenciaList->setColumnValue ( "idactividad", dbValue ( "idactividad" ) );
    mui_faltasAsistenciaList->guardar();
    
    mui_tutoresList->setColumnValue ( "idactividad", dbValue ( "idactividad" ) );
    mui_tutoresList->guardar();
    
    blDebug ( "END ActividadView::guardarPost", 0 );
    return 0;
}

int ActividadView::borrarPre()
{
    blDebug ( "ActividadView::borrarPre", 0 );
    
    QString query = "DELETE FROM alumnoactividad WHERE idactividad=" + dbValue ( "idactividad" );
    mainCompany()->runQuery ( query );
    
    query = "DELETE FROM clienteactividad WHERE idactividad=" + dbValue ( "idactividad" );
    mainCompany()->runQuery ( query );
    
    blDebug ( "END ActividadView::borrarPre", 0 );
    
    return 0;
}

int ActividadView::cargarPost ( QString id )
{
    blDebug ( "ActividadView::cargarPost", 0 );

    mui_alumnosList->cargar ( id );
    mui_faltasAsistenciaList->cargar ( id );
    mui_tutoresList->cargar ( id );

    blDebug ( "END ActividadView::cargarPost", 0 );
    
    return 0;
}

/// =============================================================================
///                    SUBFORMULARIOS
/// =============================================================================

///
/**
\param parent
**/
ListAlumnosActividadView::ListAlumnosActividadView ( QWidget *parent ) : BfSubForm ( parent )
{
    blDebug ( "ListAlumnosActividadView::ListAlumnosActividadView", 0 );
    
    setDbTableName ( "alumnoactividad" );
    setDbFieldId ( "idalumnoactividad" );
    addSubFormHeader ( "idalumnoactividad", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _ ( "Identificador" ) );
    addSubFormHeader ( "idalumno", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired , BlSubFormHeader::DbHideView, _ ( "Id alumno" ) );
    addSubFormHeader ( "nombrealumno1", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre alumno" ) );
    addSubFormHeader ( "idactividad", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView, _ ( "Id Actividad" ) );

    setInsert ( TRUE );
    setSortingEnabled ( TRUE );
    
    blDebug ( "END ListAlumnosActividadView::ListAlumnosActividadView", 0 );
}

///
/**
\param idactividad
**/
void ListAlumnosActividadView::cargar ( QString idactividad )
{
    blDebug ( "ListAlumnosActividadView::cargar", 0 );
    
    BlSubForm::cargar ( "SELECT *, (apellido1alumno || ' ' || apellido2alumno || ', ' || nombrealumno) AS nombrealumno1 FROM alumnoactividad LEFT JOIN alumno ON alumnoactividad.idalumno = alumno.idalumno WHERE alumnoactividad.idactividad=" + idactividad  );
    
    blDebug ( "END ListAlumnosActividadView::cargar", 0 );
}


///
/**
\param parent
**/
ListTutoresActividadView::ListTutoresActividadView ( QWidget *parent ) : BfSubForm ( parent )
{
    blDebug ( "ListTutoresActividadView::ListTutoresActividadView", 0 );
    
    setDbTableName ( "clienteactividad" );
    setDbFieldId ( "idclienteactividad" );
    addSubFormHeader ( "idclienteactividad", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _ ( "Identificador" ) );
    addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired , BlSubFormHeader::DbHideView, _ ( "Id tutor" ) );
    addSubFormHeader ( "nombretutor1", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre tutor" ) );
    addSubFormHeader ( "cifcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "CIF" ) );
//    addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre" ) );
    addSubFormHeader ( "idactividad", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView, _ ( "Id Actividad" ) );

    setInsert ( TRUE );
    setSortingEnabled ( TRUE );
    
    blDebug ( "END ListTutoresActividadView::ListTutoresActividadView", 0 );
}

///
/**
\param idactividad
**/
void ListTutoresActividadView::cargar ( QString idactividad )
{
    blDebug ( "ListTutoresActividadView::cargar", 0 );
    
    BlSubForm::cargar ( "SELECT *, ( COALESCE(apellido1cliente,'') || ' ' || COALESCE(apellido2cliente,'') || ', ' || COALESCE(nomcliente,'')) AS nombretutor1 FROM clienteactividad LEFT JOIN cliente ON clienteactividad.idcliente = cliente.idcliente WHERE clienteactividad.idactividad=" + idactividad  );
    
    blDebug ( "END ListTutoresActividadView::cargar", 0 );
}






///
/**
\param parent
**/
ListFaltasAsistenciaActividadView::ListFaltasAsistenciaActividadView ( QWidget *parent ) : BfSubForm ( parent )
{
    blDebug ( "ListAlumnosActividadView::ListAlumnosActividadView", 0 );
    
    setDbTableName ( "faltaasistenciaalumnoactividad" );
    setDbFieldId ( "idfaltaasistenciaalumnoactividad" );
    addSubFormHeader ( "idfaltaasistenciaalumnoactividad", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _ ( "Identificador" ) );
    addSubFormHeader ( "idalumno", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired , BlSubFormHeader::DbHideView, _ ( "Id alumno" ) );
    addSubFormHeader ( "idactividad", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView, _ ( "Id Actividad" ) );
    addSubFormHeader ( "nombrealumno1", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre alumno" ) );
    addSubFormHeader ( "fechafaltaasistenciaalumnoactividad", BlDbField::DbDate, BlDbField::DbNotNull | BlDbField::DbRequired, BlSubFormHeader::DbNone, _ ( "Fecha de la falta" ) );

    setInsert ( TRUE );
    setSortingEnabled ( TRUE );
    
    blDebug ( "END ListAlumnosActividadView::ListAlumnosActividadView", 0 );
}

///
/**
\param idactividad
**/
void ListFaltasAsistenciaActividadView::cargar ( QString idactividad )
{
    blDebug ( "ListAlumnosActividadView::cargar", 0 );
    
    BlSubForm::cargar ( "SELECT *, (apellido1alumno || ' ' || apellido2alumno || ', ' || nombrealumno) AS nombrealumno1 FROM faltaasistenciaalumnoactividad LEFT JOIN alumno ON faltaasistenciaalumnoactividad.idalumno = alumno.idalumno WHERE faltaasistenciaalumnoactividad.idactividad=" + idactividad  );
    
    blDebug ( "END ListAlumnosActividadView::cargar", 0 );
}
