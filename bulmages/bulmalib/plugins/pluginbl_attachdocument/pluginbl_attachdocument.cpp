/***************************************************************************
 *   Copyright (C) 2008 by Tomeu Borras Riera                              *
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

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>
#include <QFile>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDesktopServices>

#include "local_blI18n.h"
#include "pluginbl_attachdocument.h"
#include "blform.h"
#include "blmaincompany.h"
#include "archmenu.h"
#include "bltoolbutton.h"
#include "blaction.h"
#include "archivo.h"

int entryPoint ( QMainWindow *bcont )
{
    blDebug ( "Entrada del plugin AttachDocument", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbl_attachdocument", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    return ( 0 );
}


int BlForm_cargar ( BlForm *ficha )
{
    blDebug ( "PluginBl_AttachDocument::BlForm_cargar", 0 );
    EQToolButton *eq = ficha->findChild<EQToolButton *>("BotonArchDoc");
    eq->hazMenu();
    return 0;
    blDebug ( "END PluginBl_AttachDocument::BlForm_cargar", 0 );
}


int BlForm_DesFicha ( BlForm *ficha )
{
    return 0;
}


///
/**
\param l
\return
**/
int BlForm_BlForm ( BlForm *l )
{
    blDebug ( "PluginBl_AttacgDocument::BlForm_BlForm", 0 );

    /// El plugin necesita un parche en la base de datos para funcionar.
    /// No se puede comprobar en entryPoint porque no se tiene acceso a MainCompany
    l->mainCompany()->dbPatchVersionCheck("PluginBl_AttachDocument", "0.11.1-0001");

    /*
    new ArchMenu ( l );
    new EQToolButton( l );
*/

    blDebug ( "END PluginBl_AttachDocument::BlForm_BlForm", 0 );
    return 0;
}



///
/**
\param l
\return
**/
int BlForm_cargaSpecs ( BlForm *l )
{
    blDebug ( "BlForm_BlForm", 0 );
    new EQToolButton ( l );
    blDebug ( "END BlForm_BlForm", 0 );
    return 0;
}



