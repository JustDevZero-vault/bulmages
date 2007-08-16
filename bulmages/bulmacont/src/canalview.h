/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociación Iglues -- Contabilidad Linux         *
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

#ifndef CANALVIEW_H
#define CANALVIEW_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QCloseEvent>
#include <QTextStream>

#include "ui_canalbase.h"
#include "postgresiface2.h"
#include "dialogchanges.h"
#include "fichabc.h"


class Empresa;


class canalview : public FichaBc, public Ui_canaldlg {
    Q_OBJECT

private:
    QLineEdit *m_nomcanal;
    QTextEdit *m_desccanal;

public:
    postgresiface2 *conexionbase;
    int canales[100];
    /// Indica cual es el canal que se esta visualizando.
    int idcanal;
    /// Si su valor es 0 entonces es que no se esta visualizando ningun centro de coste.
    Empresa *empresaactual;

public:
    canalview(Empresa *, QWidget *parent = 0);
    ~canalview();
    void mostrarplantilla();
    void pintar();
    void on_mui_borrar_clicked();

public slots:
    virtual void on_mui_idcanal_valueChanged(QString);
    virtual void on_mui_guardar_clicked();
    virtual void on_mui_crear_clicked();
    virtual void closeEvent(QCloseEvent *);
};

#endif

