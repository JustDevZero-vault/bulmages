/***************************************************************************
                          aboutview.cpp  -  description
                             -------------------
    begin                : Fri Dec 27 2002
    copyright            : (C) 2002 by Tomeu Borr�s Riera
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

#include "aboutview.h"

/** \brief Constructor de la clase
  *
  * En realidad no hace nada pq el dialogo s�lo requiere visualizaci�n y no realiza acciones.
  */
aboutview::aboutview(QWidget *parent, const char *name ) : aboutdlg(parent,name,false,0) {
}

/** \brief Destructor de la clase
  *
  * En realidad no hace nada pq el dialogo s�lo requiere visualizaci�n y no realiza acciones.
  */

aboutview::~aboutview(){
}
