/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef COBROVIEW_H
#define COBROVIEW_H

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "ui_cobrobase.h"
#include "postgresiface2.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include "dialogchanges.h"
#include "fichabf.h"


class Company;


/** Ventana de ficha de cobro.
    Se encarga de la presentacion de la ficha de cobro y del tratamiento de eventos producidos
    en dicha ventana.
    Deriva de Ficha para metodos comunes a todas las ventanas.
    Deriva de Cobro para el manejo de la Base de datos. */
class CobroView : public FichaBf, public Ui_CobroBase
{
    Q_OBJECT

public:
    CobroView ( Company *, QWidget * );
    ~CobroView();
    virtual void imprimir();
    virtual int trataTags ( QString &buff );
    virtual QString nombrePlantilla(void) ;
    virtual int guardarPost();
};

#endif
