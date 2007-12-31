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

#ifndef BUSQUEDAFECHA2_H
#define BUSQUEDAFECHA2_H

#include <QLineEdit>
#include <QKeyEvent>


class BusquedaFecha2 : public QLineEdit
{
    Q_OBJECT

public:
    BusquedaFecha2 ( QWidget *parent = 0 );
    ~BusquedaFecha2();

    void setText ( QString val );
    virtual void setValorCampo ( QString val );
    QString text();
    virtual QString valorCampo();
    void selectAll();
    virtual bool eventFilter ( QObject *obj, QEvent *event );

public slots:
    void on_mui_editingFinished();
};

#endif

