/***************************************************************************
                          balanceprintview.h  -  description
                             -------------------
    begin                : mi�jun 25 2003
    copyright            : (C) 2003 by Tomeu Borr�s Riera
    email                : tborras@conetxia.com
    modificat per        : (C) 2003 Antoni Mirabete i Ter� - amirabet@biada.org
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef BALANCEPRINTVIEW_H
#define BALANCEPRINTVIEW_H

#include <errno.h>
#include "balanceprintdlg.h"
#include "postgresiface2.h"


class empresa;

/** 
  * @author Tomeu Borr�s Riera 
  * \class BalancePrintView balanceprintview.h
  * \brief Formulario introducci�n de datos para la impresi� del balance de sumas y saldos.
  * \todo Hay que cambiar el array de ccostes por un cursor
  *
  * Clase que permite introducir los datos necesarios para la generaci�n del balance (Fechas y rangos) y que se encarga de generar el balance en los formatos de impresi� indicados.
  */

class BalancePrintView : public BalancePrintDlg  {
   Q_OBJECT
private:
    /// La base de datos con la que la clase esta trabajando   
   postgresiface2 *conexionbase;   
   /// La empresa con la que la clase esta trabajando   
   empresa *empresaactual;
   
public: 
   BalancePrintView(empresa *emp);
   ~BalancePrintView();
   void inicializa1(QString, QString, QString, QString, bool);
   
private:   
   void accept();
   void presentar(char *tipus);
   
private slots:
   virtual void boton_codigoinicial();
   virtual void boton_codigofinal();
   virtual void boton_canales();
   virtual void boton_ccostes();
};

#endif
