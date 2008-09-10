/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#ifndef BUSQUEDATARIFA_H
#define BUSQUEDATARIFA_H

#include "company.h"
#include "postgresiface2.h"
#include "qcombobox2.h"


class BusquedaTarifa : public QComboBox2
{
    Q_OBJECT

private:
    cursor2 *m_cursorcombo;
    QString m_codigotarifa;

public:
    BusquedaTarifa ( QWidget *parent = 0 );
    ~BusquedaTarifa();
    virtual void setIdTarifa ( QString idtarifa );
    virtual QString valorCampo();
    virtual void setValorCampo( QString idtarifa );
    QString idtarifa();

public slots:
    void m_activated ( int index );

signals:
    void valueChanged ( QString );
};

#endif

