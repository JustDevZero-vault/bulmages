/***************************************************************************
                          balanceview.h  -  description
                             -------------------
    begin                : s�b abr 26 2003
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

#ifndef BALANCEVIEW_H
#define BALANCEVIEW_H

#include "balancedlg.h"
#include "intapunts3view.h"
#include "diarioview1.h"
#include "extractoview1.h"
#include "qtable1.h"
#include "qlayout.h"
#include "postgresiface2.h"
#include "configuracion.h"
#include "listcuentasview1.h"

class intapunts3view;
class diarioview1;
class extractoview1;
class empresa;

/** *@author Tomeu Borr�s Riera */

class balanceview : public balancedlg  {
  Q_OBJECT
public:
  intapunts3view *introapunts;
  diarioview1 *diario;
  extractoview1 *extracto;
  postgresiface2* conexionbase;
  cursor2 *cursorcta;
  int numdigitos;
  int ccostes[200];
  QTable1 *listado;
  empresa *empresaactual;
public: 
  balanceview(empresa *, QWidget *parent=0, const char *name=0, int flags=0);
  void inicializa1(QString, QString, QString, QString, int);
  ~balanceview();
  void inicializa2(intapunts3view *, diarioview1 *, extractoview1 *);
  void cargacostes();
  void boton_extracto1(int);
  void boton_asiento();
  void boton_diario1(int);
  void accept();
  void presentar();
public slots:
  virtual void return_fechafinal();
  virtual void return_fechainicial();
  virtual void return_codigoinicial();
  virtual void return_codigofinal();
  virtual void boton_buscacuentainicial();
  virtual void boton_buscacuentafinal();
  virtual void boton_imprimir();
  virtual void boton_fechainicial();
  virtual void boton_fechafinal();
  virtual void contextmenu(int , int , const QPoint &);
  virtual void nivelactivated(int);
  virtual void codigo_textChanged(const QString &);
  virtual void fecha_textChanged(const QString &);

};
#endif
