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

#ifndef BCEXTRACTOVIEW_H
#define BCEXTRACTOVIEW_H

#include "ui_bcextractobase.h"
#include "blpostgresqlclient.h"
#include "bccompany.h"
#include "bcform.h"
#include "pdefs_pluginbc_mayor.h"


/// Esta clase mantiene la generaci&oacute;n de extractos contables.
/** A partir de esta pantalla se pueden llevar a cabo todos los temas de punteo y
    casaci&oacute;n.
    Y acceso a filtrado. Tambi&eacute;n desde esta pantalla se puede pedir m&aacute;s
    informaci&oacute;n de una cuenta determinada. */
class PLUGINBC_MAYOR_EXPORT BcExtractoView : public BcForm, public Ui_BcExtractoBase
{
    Q_OBJECT

public:
    /// El extracto se realiza sobre un cursor que siempre est&aacute; en memoria.
    BlDbRecordSet *m_cursorcta;
    bool m_tratarpunteos;

public:
    BcExtractoView ( BcCompany *, QWidget *parent = 0, int flags = 0 );
    ~BcExtractoView();

    void inicializa1 ( QString, QString, QString, QString, int );
    void vaciar();
    void presentar();
    void ajustes();
    void boton_filtrar();
    QString imprimeExtractoCuenta ( QString );
    int save();
    void imprimir();

public slots:
//    virtual void on_mui_imprimir_clicked();
    virtual void on_mui_actualizar_clicked();
    virtual void accept();
    virtual void on_mui_casacion_clicked();
    virtual void on_mui_guardarpunteo_clicked();
    virtual void on_mui_cargarpunteos_clicked();
    virtual void on_mui_borrapunteo_clicked();
    virtual void boton_siguiente();
    virtual void boton_anterior();
    virtual void boton_fin();
    virtual void boton_inicio();
    virtual void boton_guardar();
    virtual void on_mui_configurar_clicked();
    virtual void on_mui_list_cellDoubleClicked ( int, int columna );
    virtual void openAsiento();

};


#endif

