/***************************************************************************
                          actualizacionesview.cpp  -  description
                             -------------------
    begin                : mar jul 22 2003
    copyright            : (C) 2003 by Tomeu Borr�s Riera
    email                : tborras@conetxia.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "actualizacionesview.h"
#include <qtextbrowser.h>

actualizacionesview::actualizacionesview(QWidget *parent, const char *name ) : actualizacionesdlg(parent,name) {
   textBrowser1->setSource("/usr/share/bulmages/ayuda/index.htm");  
}

actualizacionesview::~actualizacionesview(){
}
