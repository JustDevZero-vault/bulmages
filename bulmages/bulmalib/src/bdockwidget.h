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

#ifndef __BDOCKWIDGET__
#define __BDOCKWIDGET__

#include <QDockWidget>

#include "qworkspace2.h"


/// Provides the dock window to list all windows in BulmaFact
/// \param m_listBox this variable is the listBox that contains all
/// titles of the diferent windows.
class BDockWidget : public QDockWidget
{
    Q_OBJECT

private:
    QWorkspace2 *m_pWorkspace;

public:
    void setWorkspace ( QWorkspace2 *w );
    BDockWidget ( const QString & title, QWidget * parent = 0, Qt::WindowFlags flags = 0 );
    ~BDockWidget();

public slots:
    void cambiaVisible ( bool );

protected:
    void closeEvent ( QCloseEvent *event );

signals:
    void cambiaEstadoVisible ( bool );
};

#endif
