/***************************************************************************
                          filtrardiarioview.cpp  -  description
                             -------------------
    begin                : mar jul 29 2003
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

#include "filtrardiarioview.h"

filtrardiarioview::filtrardiarioview(QWidget *parent, const char *name ) : filtrardiariodlg(parent,name) {
}
filtrardiarioview::~filtrardiarioview(){
}

void filtrardiarioview::cargacostes() {
   // Hacemos la carga de los centros de coste. Rellenamos el combobox correspondiente.
   combocoste->clear();
  QString query="SELECT * FROM c_coste ORDER BY nombre";
   conexionbase->begin();
   cursor2 *cursorcoste = conexionbase->cargacursor(query,"costes");
   conexionbase->commit();
   combocoste->insertItem("--",0);
   ccostes[0]=0;
   int i=1;
   while (!cursorcoste->eof()) {
      combocoste->insertItem(cursorcoste->valor(2),-1);
      ccostes[i++] = atoi(cursorcoste->valor(0).latin1());
      cursorcoste->siguienteregistro();
   }// end while
   delete cursorcoste;
}// end cargacostes


void filtrardiarioview::inicializa(postgresiface2 *conn){
   cursor2 *cursoraux1;
   conexionbase = conn;
   // Vamos a cargar el n�mero de digitos de cuenta para poder hacer una introduccion de numeros de cuenta mas practica.
   conexionbase->begin();
   QString query = "SELECT * FROM configuracion WHERE nombre= 'CodCuenta'";
   cursoraux1 = conexionbase->cargacursor(query,"codcuenta");
   numdigitos=cursoraux1->valor(2).length();
   conexionbase->commit();
   delete cursoraux1;
   // Hacemos la carga de los centros de coste. Rellenamos el combobox correspondiente.
   cargacostes();
}// end inicializa

void filtrardiarioview::setccoste(int idc_coste) {
   // Establecemos el centro de coste correspondiente.
   int i=0;
   while (ccostes[i]!=idc_coste && i<100) i++;
   if (i<100) {
     combocoste->setCurrentItem(i);
   }// end if
}// end if

