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

#include <stdio.h>

#include "pluginbf_actividad.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "actividadview.h"
#include "actividadeslist.h"
#include "tipoactividadview.h"
#include "blsearchwidget.h"

ActividadesList *g_actividadesList = NULL;
BfBulmaFact *g_bges = NULL;


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de entrada del plugin de actividades\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_actividad", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_bges = bges;

    if ( bges->company()->hasTablePrivilege ( "cobro", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( _("&Activitats"), "menuActivitats", "menuMaestro" );

        pPluginMenu->addSeparator();
        
        BlAction *accionA = new BlAction ( _ ( "&Actividades" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad.png" ) ) );
        accionA->setStatusTip ( _ ( "Actividades" ) );
        accionA->setWhatsThis ( _ ( "Actividades" ) );
        accionA->setObjectName("mui_actionActividades");
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );
        //connect ( accionA, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        BlAction *accionB = new BlAction ( _ ( "&Nueva actividad" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad_add.png" ) ) );
        accionB->setStatusTip ( _ ( "Nueva actividad" ) );
        accionB->setWhatsThis ( _ ( "Nueva actividad" ) );
        accionB->setObjectName("mui_actionActividadNueva");
        pPluginMenu->addAction ( accionB );
        bges->Fichas->addAction ( accionB );
        //connect ( accionB, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

        pPluginMenu->addSeparator();

        BlAction *accionC = new BlAction ( _ ( "&Tipos de Actividad" ), 0 );
        accionC->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad.png" ) ) );
        accionC->setStatusTip ( _ ( "Tipos de Actividad" ) );
        accionC->setWhatsThis ( _ ( "Tipos de Actividad" ) );
        accionC->setObjectName("mui_actionActividadTipos");
        pPluginMenu->addAction ( accionC );
        bges->Fichas->addAction ( accionC );
        //connect ( accionC, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );
        
    } // end if

    return 0;
}

int BlAction_triggered(BlAction *accion) {
    blDebug ( "pluginbf_actividad::BlAction_triggered", 0 );

    if (accion->objectName() == "mui_actionActividades") {
        if ( g_actividadesList ) {
            g_actividadesList->hide();
            g_actividadesList->show();
        } // end if
    }
    if (accion->objectName() == "mui_actionActividadNueva") {
        ActividadView * bud = new ActividadView ( ( BfCompany * ) g_bges->company(), NULL );
        g_bges->company() ->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    }
    if (accion->objectName() == "mui_actionActividadTipos") {
        TipoActividadView * bud = new TipoActividadView ( ( BfCompany * ) g_bges->company(), NULL );
        g_bges->company() ->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    }
    blDebug ( "END pluginbf_actividad::BlAction_triggeredr", 0 );

    return 0;
}
int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "actividad", "SELECT" ) ) {
        g_actividadesList = new ActividadesList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_actividadesList );
        g_actividadesList->hide();
    }// end if
    return 0;
}




int BlSubFormDelegate_createEditor ( BlSubFormDelegate *bl )
{
    blDebug ( "pluginbf_actividad::BlSubFormDelegate_createEditor", 0 );
    int ret = 0;
    if ( g_fieldName == "nombreactividad" ) {
        BlDbCompleterComboBox * editor = new BlDbCompleterComboBox ( g_editor );
        editor->setObjectName ( "EditNombreActividad" );
        editor->setMainCompany ( ( BfCompany * ) bl->m_subform->mainCompany() );
        editor->m_valores["nombreactividad"] = "";
        editor->m_tabla = "actividad";
        g_plugParams =  editor;
        ret = -1;
    } // end if
    blDebug ( "END pluginbf_actividad::BlSubFormDelegate_createEditor", 0 );
    return ret;
}

int BlSubFormDelegate_setModelData ( BlSubFormDelegate *bl )
{
    blDebug ( "pluginbf_actividad::BlSubFormDelegate_setModelData", 0 );
    int ret = 0;
    if ( g_editor->objectName() == "EditNombreActividad" ) {
        BlDbCompleterComboBox * comboBox = ( BlDbCompleterComboBox * ) g_editor;
        QString value = comboBox->currentText();
        value = value.left ( value.indexOf ( ".-" ) );
        g_model->setData ( g_index, value );
        ret = -1;
    } // end if
    blDebug ( "END pluginbf_actividad::BlSubFormDelegate_setModelData", 0 );
    return ret;
}


int BlSubFormDelegate_setEditorData ( BlSubFormDelegate *bl )
{
    blDebug ( "pluginbf_actividad::BlSubFormDelegate_setEditorData", 0 );
    int ret = 0;
    if ( g_editor->objectName() == "EditNombreActividad" ) {
        QString value = g_index.model() ->data ( g_index, Qt::DisplayRole ).toString();
        BlDbCompleterComboBox *comboBox = ( BlDbCompleterComboBox * ) g_editor;
        comboBox->addItem ( value );
        ret = -1;
    } // end if
    blDebug ( "END pluginbf_actividad::BlSubFormDelegate_setEditorData", 0 );
    return ret;
}

int BlSubForm_editFinished ( BlSubForm *sub )
{
    blDebug ( "pluginbf_actividad::BlSubForm_editFinished", 0 );
    if ( sub->m_campoactual->fieldName() == "nombreactividad" ) {
        BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT idactividad FROM actividad WHERE nombreactividad = '" + sub->m_campoactual->text() + "'" );
        if ( !cur->eof() ) {
            sub->m_registrolinea->setDbValue ( "idactividad", cur->value( "idactividad" ) );
        } // end if
        delete cur;
    } // end if

    blDebug ( "END pluginbf_actividad::BlSubForm_editFinished", 0 );
    return 0;
}




/// --------------------------------------------------------------
/// --------- Implemento la edicion de actividads -----------------
/// Con esta funcionalidad creamos menus contextuales en todos los subformularios donde
/// Aparezca el identificador de actividad como elemento y permite de forma sencilla
/// La creacion, la edicion, y la seleccion.
///


/**
\param parent
**/
SubForm_Actividad::SubForm_Actividad ( BlSubForm *parent ) : QObject ( parent )
{
    blDebug ( "SubForm_Actividad::SubForm_Actividad", 0 );
    blDebug ( "END SubForm_Actividad::SubForm_Actividad", 0 );
}

///
/**
**/
SubForm_Actividad::~SubForm_Actividad()
{
    blDebug ( "SubForm_Actividad::~SubForm_Actividad", 0 );
    blDebug ( "END SubForm_Actividad::~SubForm_Actividad", 0 );
}


///
/**
\param menu
**/
void SubForm_Actividad::s_pintaMenu ( QMenu *menu )
{
    blDebug ( "SubForm_Actividad::s_pintaMenu", 0 );
    BfSubForm *sub = ( BfSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "nombreactividad" );
    if ( header ) {
        menu->addSeparator();
        menu->addAction ( _ ( "Nuevo actividad" ) );
        QString idactividad = sub->dbValue ( "idactividad" );
        if ( idactividad != "" ) menu->addAction ( _ ( "Editar actividad" ) );
        if ( ! ( header->options() & BlSubFormHeader::DbNoWrite ) )  {
            menu->addAction ( _ ( "Seleccionar actividad" ) );
        } // end if
    } // end if
    blDebug ( "END SubForm_Actividad::s_pintaMenu", 0 );
}


///
/**
\param action
**/
void SubForm_Actividad::s_trataMenu ( QAction *action )
{
    blDebug ( "SubForm_Actividad::s_trataMenu", 0 );
    BfSubForm *sub = ( BfSubForm * ) parent();
    if ( action->text() == _ ( "Editar actividad" ) ) {
        QString idactividad = sub->dbValue ( "idactividad" );
        if ( idactividad != "" )
            editarActividad ( idactividad );
    } else if ( action->text() == _ ( "Seleccionar actividad" ) ) {
        seleccionarActividad ( sub );
    } else if ( action->text() == _ ( "Nuevo actividad" )  ) {
        nuevoActividad();
    } // end if

    blDebug ( "END SubForm_Actividad::s_trataMenu", 0 );
}


///
/**
**/
void SubForm_Actividad::editarActividad ( QString idactividad )
{
    blDebug ( "SubForm_Actividad::editarActividad", 0 );
    BlSubForm * subf = ( BlSubForm * ) parent();
    ActividadView * art = new ActividadView ( ( BfCompany * ) subf->mainCompany(), 0 );
    subf->mainCompany() ->m_pWorkspace->addSubWindow ( art );
    /// Si la carga no va bien entonces terminamos.
    if ( art->cargar ( idactividad ) ) {
        delete art;
        blDebug ( "END ActividadesList::editar", 0, "Carga Erronea" );
        return;
    } // end if
    art->hide();
    art->show();
    blDebug ( "END SubForm_Actividad::editarActividad", 0 );
}



///
/**
**/
void SubForm_Actividad::nuevoActividad( )
{
    blDebug ( "SubForm_Actividad::editarActividad", 0 );
    BlSubForm * subf = ( BlSubForm * ) parent();
    ActividadView * art = new ActividadView ( ( BfCompany * ) subf->mainCompany(), 0 );
    subf->mainCompany() ->m_pWorkspace->addSubWindow ( art );
    art->hide();
    art->show();
    blDebug ( "END SubForm_Actividad::editarActividad", 0 );
}


///
/**
**/
void SubForm_Actividad::seleccionarActividad ( BfSubForm *sub )
{
    blDebug ( "SubForm_Actividad::editarActividad", 0 );

    ActividadesList *artlist = new ActividadesList ( ( BfCompany * ) sub->mainCompany(), NULL, 0, BL_SELECT_MODE );
    /// Esto es convertir un QWidget en un sistema modal de dialogo.
    sub->setEnabled ( false );
    artlist->show();
    while ( !artlist->isHidden() )
        g_theApp->processEvents();
    sub->setEnabled ( true );
    QString idActividad = artlist->idactividad();
    delete artlist;

    /// Si no tenemos un idactividad salimos ya que significa que no se ha seleccionado ninguno.
    if ( idActividad == "" ) {
        blDebug ( "END BfSubForm::pressedAsterisk", 0 );
        return;
    } // end if

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM actividad WHERE idactividad = " + idActividad );
    if ( !cur->eof() ) {
        sub->lineaact()->setDbValue ( "idactividad", idActividad );
        sub->lineaact()->setDbValue ( "codigoactividad", cur->value( "codigoactividad" ) );
        sub->lineaact()->setDbValue ( "nombreactividad", cur->value( "nombreactividad" ) );
    } // end if
    delete cur;

    blDebug ( "END SubForm_Actividad::editarActividad", 0 );
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    blDebug ( "BlSubForm_BlSubForm_Post", 0 );
    SubForm_Actividad *subformods = new SubForm_Actividad ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    blDebug ( "END BlSubForm_BlSubForm_Post", 0 );
    return 0;
}



int Busqueda_on_mui_buscar_clicked ( BlSearchWidget *busq )
{
    blDebug ( "Busqueda_on_mui_buscar_clicked", 0 );

    if ( busq->tableName() == "actividad" ) {

        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        blCenterOnScreen ( diag );

        ActividadesList *actividades = new ActividadesList ( ( BfCompany * ) busq->mainCompany(), diag, 0, BL_SELECT_MODE );
        busq->connect ( actividades, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

        /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
        /// para que sea redimensionable y aparezca el titulo de la ventana.
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget ( actividades );
        layout->setMargin ( 0 );
        layout->setSpacing ( 0 );
        diag->setLayout ( layout );
        diag->setWindowTitle ( actividades->windowTitle() );

        diag->exec();
        
        if ( actividades->idactividad() != "" ) {
            busq->setId ( actividades->idactividad() );
        } // end if
        
        delete diag;

        return 1;
        
    } // end if
    
    blDebug ( "END Busqueda_on_mui_buscar_clicked", 0 );
    
    return 0;

}





