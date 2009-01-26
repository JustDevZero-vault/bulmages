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

#ifndef BUSQUEDAPROFESOR_H
#define BUSQUEDAPROFESOR_H

#include "busqueda.h"


/// Permite buscar y seleccionar un profesor.
/** Implementa un Widget que permite buscar y seleccionar un
    profesor de BulmaFact de forma generica. */
class BusquedaProfesor : public Busqueda
{
    Q_OBJECT


public:
    BusquedaProfesor ( QWidget *parent = 0 );
    ~BusquedaProfesor();

public slots:
    virtual void on_mui_buscar_clicked();


};

#endif

