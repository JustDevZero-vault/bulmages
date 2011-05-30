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

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>

#include "pluginbf_debug.h"
#include "blconfiguration.h"
#include "bfbulmafact.h"


///
/**
\param bcont
**/
int entryPoint ( BfBulmaFact *bcont )
{
    blDebug ( "Entrada del plugin depuracion", 10 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_debug", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    /// A&ntilde;ade en el men&uacute; del programa la opci&oacuteMn para
    /// acceder al modo de depuraci&oacuteMm.
    BlAction *accionA = new BlAction ( _("&Modo depuracion"), 0 );
    accionA->setCheckable ( TRUE );

    if ( g_confpr->value( CONF_DEBUG ) == "TRUE" ) {
        accionA->setChecked ( TRUE );
    } else {
        accionA->setChecked ( FALSE );
    }

    accionA->setStatusTip ( _ ( "Activa/desactiva el modo de depuracion" ) );
    accionA->setWhatsThis ( _ ( "Depuracion.\n\nActiva/desactiva el modo de depuracion" ) );
    accionA->setObjectName("mui_actionDebug");
    bcont->menuVentana->addSeparator();
    bcont->menuVentana->addAction ( accionA );
    blDebug ( "END: Plugin depuracion", 10 );
    return 0;
}


int BlAction_triggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionDebug") {

        if (accion->isChecked ()) {
            g_confpr->setValue( CONF_DEBUG, "TRUE");
        } else {
            g_confpr->setValue( CONF_DEBUG, "FALSE");
        } // end if

    } // end if
    return 0;
}
