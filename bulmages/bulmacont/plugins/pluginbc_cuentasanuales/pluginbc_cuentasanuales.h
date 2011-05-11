/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
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

#ifndef PLUGINBC_CUENTASANUALES_H
#define PLUGINBC_CUENTASANUALES_H

#include "bcbulmacont.h"
#include "blpostgresqlclient.h"
#include "bccompany.h"
#include "bccuentasanualesview.h"
#include "pdefs_pluginbc_cuentasanuales.h"


extern "C" PLUGINBC_CUENTASANUALES_EXPORT BcCuentasAnualesView *g_cuentasAnuales;
extern "C" PLUGINBC_CUENTASANUALES_EXPORT int entryPoint ( BcBulmaCont * );


class PluginBc_CuentasAnuales : public QObject, BlMainCompanyPointer
{
    Q_OBJECT

public:
    BcBulmaCont *m_bulmacont;

public:
    PluginBc_CuentasAnuales();
    ~PluginBc_CuentasAnuales();
    void inicializa ( BcBulmaCont * );

public slots:
    void elslot();
};


#endif

