/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#ifndef AUTOFORM_H
#define AUTOFORM_H


#include <QWidget>
#include <QCloseEvent>
#include <QtScript>
#include <QScriptEngine>
#include <QUiLoader>

#include "pdefs_pluginbl_autoform.h"
#include "blform.h"
#include "blformlist.h"

#include "ui_autoformbase.h"
#include "ui_autoformlistbase.h"

class PLUGINBL_AUTOFORM_EXPORT BlAutoForm : public BlForm, public Ui_AutoFormBase
{
    Q_OBJECT
public:
    BlAutoForm ( BlMainCompany *emp = NULL, QWidget *parent = 0, Qt::WFlags f = 0, edmode modo = BL_EDIT_MODE, const QString &interfacefile = "");
    ~BlAutoForm();
    virtual int load ( QString id, bool paint = TRUE );
    virtual int afterSave();
    void launch();
};





class PLUGINBL_AUTOFORM_EXPORT BlAutoFormList : public BlFormList, public Ui_AutoFormListBase
{
  Q_OBJECT
public:
  BlAutoFormList ( BlMainCompany *comp=NULL, QWidget *parent=0, Qt::WFlags flag=0, edmode editmodo = BL_EDIT_MODE, const QString &interfacefile = "" );
  ~BlAutoFormList();
  void launch();
  BlAutoForm * createAutoForm();
  virtual void editar ( int );
  void remove();
  void crear();
};

#endif

