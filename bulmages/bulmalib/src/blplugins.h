/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef BLPLUGINS_H
#define BLPLUGINS_H

#include <QList>
#include <QLibrary>

#include "blfunctions.h"


/** Controla el sistema de carga y disparo segun metodo de los plugins
 */
class BlPlugins
{
private:
    QList<QLibrary *> m_lista;

public:
    BlPlugins();
    ~BlPlugins();
    void cargaLibs ( const QString &libs );
    int lanza ( const char *func, void *clase );
    int lanza ( const char *func, void *clase, void **ret );
};

extern BlPlugins *g_plugins;
extern void *g_plugParams;

#endif
