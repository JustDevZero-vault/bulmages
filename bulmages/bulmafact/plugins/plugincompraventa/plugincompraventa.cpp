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

#include "plugincompraventa.h"
#include "compraventaview.h"
#include "company.h"
#include "funcaux.h"


///
/**
**/
myplugincv::myplugincv()
{
    _depura ( "myplugincv::myplugincv", 0 );
    _depura ( "END myplugincv::myplugincv", 0 );
}


///
/**
**/
myplugincv::~myplugincv()
{
    _depura ( "myplugincv::~myplugincv", 0 );
    _depura ( "END myplugincv::~myplugincv", 0 );
}


///
/**
**/
void myplugincv::elslot()
{
    _depura ( "myplugincv::elslot", 0 );

    CompraVentaView *tar = new CompraVentaView ( ( Company * ) empresaBase(), NULL );
    empresaBase() ->m_pWorkspace->addWindow ( tar );
    tar->show();

    _depura ( "END myplugincv::elslot", 0 );
}


///
/**
\param bges
**/
void myplugincv::inicializa ( Bulmafact *bges )
{
    _depura ( "myplugincv::inicializa", 0 );
    /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
    m_bges = bges;
    setEmpresaBase ( bges->getcompany() );
    QAction *planCuentas = new QAction ( tr ( "&CompraVenta" ), 0 );
    planCuentas->setStatusTip ( tr ( "CompraVenta" ) );
    planCuentas->setWhatsThis ( tr ( "CompraVenta" ) );
    bges->menuVentas->addSeparator();
    bges->menuVentas->addAction ( planCuentas );
    connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    _depura ( "END myplugincv::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de Entrada del plugin de CompraVenta\n", 0 );
    myplugincv *plug = new myplugincv();
    plug->inicializa ( bges );
    return 0;
}
