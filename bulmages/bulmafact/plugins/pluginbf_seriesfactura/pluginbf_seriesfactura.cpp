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

#include "pluginbf_seriesfactura.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "listseriefacturaview.h"




///
/**
**/
MyPlugSeriesFactura::MyPlugSeriesFactura()
{
    _depura ( "MyPlugSeriesFactura::MyPlugSeriesFactura", 0 );
    _depura ( "END MyPlugSeriesFactura::MyPlugSeriesFactura", 0 );
}


///
/**
**/
MyPlugSeriesFactura::~MyPlugSeriesFactura()
{
    _depura ( "MyPlugSeriesFactura::~MyPlugSeriesFactura", 0 );
    _depura ( "END MyPlugSeriesFactura::~MyPlugSeriesFactura", 0 );
}


///
/**
**/
void MyPlugSeriesFactura::elslot1()
{
    _depura ( "MyPlugSeriesFactura::elslot1", 0 );
        ListSerieFacturaView * bud = new ListSerieFacturaView((BfCompany *)mainCompany(), NULL);
        mainCompany() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    _depura ( "END MyPlugSeriesFactura::elslot1", 0 );
}



///
/**
\param bges
**/
void MyPlugSeriesFactura::inicializa ( Bulmafact *bges )
{
    _depura ( "MyPlugSeriesFactura::inicializa", 0 );

    if ( bges->getcompany()->hasTablePrivilege ( "serie_factura", "SELECT" ) ) {

    /// Miramos si existe un menu Ventas
	QMenu *pPluginMenu = bges->menuMaestro;
	pPluginMenu->addSeparator();

	/// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
	m_bges = bges;
	setMainCompany ( bges->getcompany() );
	QAction *planCuentas = new QAction ( _( "&Series de Factura" ), 0 );
	//planCuentas->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/employee-list.svg" ) ));
	planCuentas->setStatusTip ( _( "Series de Factura" ) );
	planCuentas->setWhatsThis ( _( "Series de Factura" ) );
	pPluginMenu->addAction ( planCuentas );
	bges->Listados->addAction (planCuentas);
	connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

    }// end if
    _depura ( "END MyPlugSeriesFactura::inicializa", 0 );
}





///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de Entrada del plugin de Trabajadors\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginbf_seriesfactura", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    MyPlugSeriesFactura *plug = new MyPlugSeriesFactura();
    plug->inicializa ( bges );
    return 0;
}



/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewTrabajadorView (BfCompany *v)
{
	ListSerieFacturaView *h = new ListSerieFacturaView(v, 0);
	g_plugParams = h;
	return 1;
}