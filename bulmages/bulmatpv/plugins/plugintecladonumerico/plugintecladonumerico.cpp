/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include <QDockWidget>

#include "plugintecladonumerico.h"
#include "funcaux.h"
#include "empresatpv.h"
#include "tecladonumerico.h"



QDockWidget *g_doc1;

TecladoNumerico *g_tecl;
///
/**
\return
**/
int entryPoint ( BulmaTPV *tpv )
{
    _depura ( "entryPoint", 0 );
    /// Vamos a probar con un docwindow.
    g_doc1 = new QDockWidget ( "Teclado", tpv );
    g_doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );

    g_doc1->setGeometry ( 100, 100, 100, 500 );
    g_doc1->resize ( 330, 400 );
    tpv->addDockWidget ( Qt::LeftDockWidgetArea, g_doc1 );
    g_doc1->show();

    _depura ( "END entryPoint", 0 );
    return 0;
}

int EmpresaTPV_createMainWindows_Post ( EmpresaTPV *etpv )
{
    g_tecl = new TecladoNumerico ( etpv, g_doc1 );
// etpv->pWorkspace()->addWindow(g_tecl);
    g_doc1->setWidget ( g_tecl );
    return 0;
}

int Input_keyPressEvent_Post ( Input *in )
{
    g_tecl->mui_display->setText ( in->valorInput() );
}
