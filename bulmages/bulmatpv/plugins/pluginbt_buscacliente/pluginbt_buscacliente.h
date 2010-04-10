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

#ifndef PLUGINBT_BUSCACLIENTE_H
#define PLUGINBT_BUSCACLIENTE_H

#include "btbulmatpv.h"
#include "btcompany.h"
#include "blsearchwidget.h"
#include "pdefs_pluginbt_buscacliente.h"


extern "C" PLUGINBT_BUSCACLIENTE_EXPORT int entryPoint ( BtBulmaTPV * );
extern "C" PLUGINBT_BUSCACLIENTE_EXPORT int BtCompany_createMainWindows_Post ( BtCompany * );
extern "C" PLUGINBT_BUSCACLIENTE_EXPORT int Busqueda_on_m_inputBusqueda_editingFinished_Post ( BlSearchWidget * );

#endif

