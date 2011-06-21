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

#include <QKeyEvent>
#include <QEvent>

#include "bltexteditdelegate.h"
#include "local_blI18n.h"

///
/**
\param parent
**/
BlTextEditDelegate::BlTextEditDelegate ( QWidget *parent ) : QTextEdit ( parent )
{
    BlDebug::blDebug ( "BlTextEditDelegate::BlTextEditDelegate", 0 );
    installEventFilter ( this );
    
}


///
/**
**/
BlTextEditDelegate::~BlTextEditDelegate()
{
    BlDebug::blDebug ( "BlTextEditDelegate::~BlTextEditDelegate", 0 );
    
}


///
/**
\param obj
\param event
\return
**/
bool BlTextEditDelegate::eventFilter ( QObject *obj, QEvent *event )
{
    BlDebug::blDebug ( "BlTextEditDelegate::eventFilter", 0, QString::number ( event->type() ) );
    /// Si es un release de tecla se hace la funcionalidad especificada.
    if ( event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease ) {
        QKeyEvent * keyEvent = static_cast<QKeyEvent *> ( event );
        int key = keyEvent->key();
        Qt::KeyboardModifiers mod = keyEvent->modifiers();
	/// Quitamos la pulsacion del Enter ya que lo que debe funcionar es el Shift + Enter
	switch ( key ) {
	  case Qt::Key_Return:
	  case Qt::Key_Enter:
		event->ignore();
		
		return TRUE;
	      break;
	  } // end switch
    } // end if
    
    return QTextEdit::eventFilter ( obj, event );
}

