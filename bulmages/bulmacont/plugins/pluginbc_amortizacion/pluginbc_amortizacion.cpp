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

#include <cstdio>

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>
#include <QTextCodec>
#include <QLocale>

#include "pluginbc_amortizacion.h"
#include "bcamortizacionlistview.h"

BcAmortizacionListView *g_amortizacionlist;





///
/**
**/
MyPluginAmortizacion::MyPluginAmortizacion()
{
    _depura ( "MyPluginAmortizacion::MyPluginAmortizacion", 0 );
    _depura ( "END MyPluginAmortizacion::MyPluginAmortizacion", 0 );
}


///
/**
**/
MyPluginAmortizacion::~MyPluginAmortizacion()
{
    _depura ( "MyPluginAmortizacion::~MyPluginAmortizacion", 0 );
    _depura ( "END MyPluginAmortizacion::~MyPluginAmortizacion", 0 );
}



///
/**
**/
void MyPluginAmortizacion::elslot()
{
    _depura ( "MyPluginAmortizacion::elslot", 0 );
    if (g_amortizacionlist == NULL) {
      g_amortizacionlist = new BcAmortizacionListView ( ( BcCompany * ) mainCompany(), 0 );
      mainCompany() ->pWorkspace() ->addWindow ( g_amortizacionlist );
    } // end if
    g_amortizacionlist->hide();
    g_amortizacionlist->show();
    _depura ( "END MyPluginAmortizacion::elslot", 0 );
}



///
/**
\param bges
**/
void MyPluginAmortizacion::inicializa ( BcBulmaCont *bges )
{
    _depura ( "MyPluginAmortizacion::inicializa", 0 );

    /// Creamos el men&uacute;.
    setMainCompany ( (BlMainCompany *)bges->empresaactual() );
    m_bulmacont = bges;
    QMenu *pPluginMenu;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( _ ( "menuVer" ) );

    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        pPluginMenu = new QMenu ( _ ( "&Ver" ), bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuVer" ) );
    } // end if

    QAction *accion = new QAction ( _ ( "&Amortizaciones" ), 0 );
    accion->setStatusTip ( _ ( "Permite ver las amortizaciones" ) );
    accion->setWhatsThis ( _ ( "Podra disponer de la informacion de las amortizaciones" ) );
    accion->setIcon(QIcon(QString::fromUtf8(":/Genericos32x32/images/png/i_bulmacont_amortizaciones.png")));
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accion );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar() ->insertMenu ( bges->menuMaestro->menuAction(), pPluginMenu );
    bges->toolBar->addAction ( accion );

    _depura ( "END MyPluginAmortizacion::inicializa", 0 );
}




///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    _depura ( "entryPoint::entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbc_amortizacion", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_amortizacionlist = NULL;

    MyPluginAmortizacion *plug = new MyPluginAmortizacion();
    plug->inicializa ( bcont );

    _depura ( "END entryPoint::entryPoint", 0 );
    return 0;
}
