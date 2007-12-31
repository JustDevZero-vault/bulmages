/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#ifndef TICKETQTOOLBUTTON_H
#define TICKETQTOOLBUTTON_H

#include <QWidget>
#include <QToolButton>
#include <QString>
#include "company.h"
#include "funcaux.h"
#include <QtXml/QDomDocument>
#include <QMap>
#include <QString>


#include "facturaview.h"
#include "presupuestoview.h"
#include "pedidoclienteview.h"
#include "albaranclienteview.h"


class TicketQToolButton : public QToolButton
{
    Q_OBJECT

private:
    Company *m_companyact;
    PresupuestoView *m_presupuestoView;
    PedidoClienteView *m_pedidoClienteView;
    AlbaranClienteView *m_albaranClienteView;
    FacturaView    *m_facturaView;

public:
    TicketQToolButton ( PresupuestoView *, PedidoClienteView *, AlbaranClienteView *,  FacturaView * , QWidget *parent = NULL );
    ~TicketQToolButton();
    void setBoton();
public slots:
    virtual void click();

};

#endif
