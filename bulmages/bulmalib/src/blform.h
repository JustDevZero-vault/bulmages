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

#ifndef BLFORM_H
#define BLFORM_H

#include <QWidget>
#include <QCloseEvent>

#include "dialogchanges.h"
#include "funcaux.h"
#include "empresabase.h"
#include "blwidget.h"
#include "dbrecord.h"


class BlForm : public BlWidget, public DBRecord, public dialogChanges
{
    Q_OBJECT

public:
    enum edmode {
        EditMode = 0, SelectMode = 1
    };

private:
    /// Indica si es modo consulta o modo edicion. (altera el comportamiento del
    /// doble click sobre la lista)
    edmode m_modo;
    QString m_title;
    QMap<QString,QString> m_variables;

protected:
    void setModoConsulta();
    void setModoEdicion();
    bool modoEdicion();
    bool modoConsulta();

public:
    BlForm ( QWidget *parent = 0, Qt::WFlags f = 0, edmode modo = EditMode );
    BlForm ( EmpresaBase *emp = NULL, QWidget *parent = 0, Qt::WFlags f = 0, edmode modo = EditMode );
    void substrVars(QString &buff, int tipoEscape = 0);
    virtual ~BlForm();
    virtual int guardar();
    virtual int guardarPost();
    virtual int borrarPre();
    virtual int borrar();
    virtual void closeEvent ( QCloseEvent * );
    virtual int sacaWindow();
    virtual void meteWindow ( QString , QObject *, bool compdup = TRUE );
    void setDBTableName ( QString nom );
    void setTitleName ( QString nom );
    virtual void pintar();
    virtual void pintarPost();
    virtual int cargar ( QString id );
    virtual int cargarPost ( QString id );
    virtual void recogeValores();
    virtual void creaMenu ( QMenu * );
    virtual void procesaMenu ( QAction * );
    virtual int generaRML ( const QString & );
    virtual int generaRML ( void );
    virtual int trataTags ( QString &buff, int tipoEscape = 0 );
    virtual QString trataQuery ( const QString &query, const QString &datos, int tipoEscape = 0 );
    QString trataCursor ( cursor2 *cur, const QString &datos, int tipoEscape = 0 );
    virtual QString trataIfQuery ( const QString &query, const QString &datos );
    virtual QString trataIf ( const QString &query, const QString &datos, const QString &datos1 );
    virtual QString trataExists ( const QString &query, const QString &datos );
    virtual void cargaSpecs();
    virtual void generaCampo ( const QString &objname, const QString &textname, const QString &type );
    virtual void activaDocumentos();
    virtual void desactivaDocumentos();

public slots:
    virtual void on_mui_aceptar_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_eliminar_clicked();
    virtual void on_mui_cancelar_clicked();
    virtual void on_mui_guardar_clicked();
    virtual void on_customContextMenuRequested ( const QPoint &pos );
    virtual void on_mui_imprimir_clicked();

signals:
    virtual void cerrar();
    void pintaMenu ( QMenu * );
    void trataMenu ( QAction * );
};

#endif
