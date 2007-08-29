/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

#ifndef PRESUPUESTO_H
#define PRESUPUESTO_H

#include <QString>

#include "fixed.h"
#include "listlinpresupuestoview.h"
#include "listdescpresupuestoview.h"
#include "company.h"
#include "fichabf.h"


/// Administra la informaci&oacute;n de un Presupuesto.
/** */
class Presupuesto : public FichaBf {

public:
    Presupuesto(Company *, QWidget *);

    virtual ~Presupuesto();
    virtual int cargar(QString);
    virtual void pintaPresupuesto();
    virtual int guardar();
    virtual int borrar();
    virtual QString detalleArticulos();

    void vaciaPresupuesto();
};

#endif

