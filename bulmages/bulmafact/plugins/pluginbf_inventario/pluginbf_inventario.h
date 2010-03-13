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

#ifndef PLUGINBF_INVENTARIO_H
#define PLUGINBF_INVENTARIO_H

#include "blpostgresqlclient.h"
#include "bfbulmafact.h"
#include "blwidget.h"
#include "articuloview.h"
#include "pdefs_pluginbf_inventario.h"


extern "C" PLUGINBF_INVENTARIO_EXPORT int entryPoint ( BfBulmaFact * );
extern "C" PLUGINBF_INVENTARIO_EXPORT int ArticuloView_ArticuloView ( ArticuloView * );
extern "C" PLUGINBF_INVENTARIO_EXPORT int BlForm_cargar ( BlForm * );
extern "C" PLUGINBF_INVENTARIO_EXPORT int BlForm_guardar_Post ( BlForm * );
extern "C" PLUGINBF_INVENTARIO_EXPORT int BfSubForm_on_mui_list_editFinished ( BfSubForm * );


class myplugininv : public QObject, BlMainCompanyPointer
{
    Q_OBJECT

public:
    BfBulmaFact *m_bges;

public:
    myplugininv();
    ~myplugininv();
    void inicializa ( BfBulmaFact * );

public slots:
    void elslot();
};


#endif

