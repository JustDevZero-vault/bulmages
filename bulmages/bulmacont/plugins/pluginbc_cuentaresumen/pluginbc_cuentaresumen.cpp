/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include <stdio.h>

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>
#include <QMenu>
#include <QMenuBar>

#include "pluginbc_cuentaresumen.h"
#include "correctorwidget.h"
#include "bccompany.h"
#include "bcbulmacont.h"
#include "bldockwidget.h"


ResumCtaWidget *g_res;
QAction *viewCorrector;


///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    _depura ( "Entrada del plugin ResumCta", 10 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "PluginBc_CuentaResumeb", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    BcCompany *emp = bcont->empresaactual();
    /// Vamos a probar con un docwindow.
    BlDockWidget *doc1 = new BlDockWidget ( _ ( "Resumen cuenta" ), bcont );
    doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );

    doc1->setGeometry ( 100, 100, 100, 500 );
    doc1->resize ( 330, 250 );
    bcont->addDockWidget ( Qt::LeftDockWidgetArea, doc1 );

    doc1->show();

    g_res = new ResumCtaWidget ( emp, doc1 );
    g_res->setMainCompany ( emp );
    g_res->dock = doc1;

    doc1->setWidget ( g_res );

    QMenu *pPluginMenu;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bcont->menuBar() ->findChild<QMenu *> ( _ ( "menuVer" ) );

    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        pPluginMenu = new QMenu ( _ ( "&Ver" ), bcont->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuVer" ) );
    } // end if


    /// A&ntilde;ade en el men&uacute; del programa la opci&oacuteMn para
    /// acceder al corrector.
    viewCorrector = new QAction ( _ ( "&Resumen Cta" ), 0 );
    viewCorrector->setCheckable ( TRUE );
    viewCorrector->setChecked ( TRUE );
    viewCorrector->setStatusTip ( _ ( "Muestra/oculta el resumen de cuenta" ) );
    viewCorrector->setWhatsThis ( _ ( "Corrector.\n\nMuestra/oculta el corrector" ) );
    QObject::connect ( viewCorrector, SIGNAL ( toggled ( bool ) ), doc1, SLOT ( cambiaVisible ( bool ) ) );
    QObject::connect ( doc1, SIGNAL ( cambiaEstadoVisible ( bool ) ), viewCorrector, SLOT ( setChecked ( bool ) ) );

    pPluginMenu ->addSeparator();
    pPluginMenu ->addAction ( viewCorrector );

    QFile file ( g_confpr->valor ( CONF_DIR_USER ) + "pluginresumcta_.cfn" );
    if ( file.exists () ) {
        doc1->show();
        viewCorrector->setChecked ( TRUE );
    } else {
        doc1->hide();
        viewCorrector->setChecked ( FALSE );
    } // end if

    _depura ( "Iniciado correctamente el plugin ResumCta", 10 );
    return 0;
}

///
/**
\return
**/
int Busqueda_on_m_inputBusqueda_textChanged ( BlSearchWidget * )
{
    g_res->setMensaje ( "<HTML><BODY>No hay cuenta.</BODY></HTML>" );
    return 0;
}


///
/**
\param bcta
\return
**/

int Busqueda_on_m_inputBusqueda_editingFinished_Post ( BlSearchWidget *bcta )
{
    _depura ( "BcBuscarCuenta_s_lostFocus_post", 0 );
    QString cod = bcta->fieldValue("codigo");
    QString nom = bcta->fieldValue("descripcion");
    QString id  = bcta->id();
    if ( id == "" ) return 0;
    BlDbRecordSet *cur = bcta->mainCompany() ->loadQuery ( "SELECT *, debe-haber AS saldo FROM cuenta WHERE idcuenta = " + id );
    if ( !cur ) return 0;
    if ( ! cur->eof() ) {
        QString mensaje = "<HTML><BODY>";
        mensaje += "<B>Codigo:</B> " + cod + "<BR>";
        mensaje += "<B>Nombre:</B> " + nom + "<BR>";
        mensaje += "<B>Debe:</B> " + cur->valor ( "debe" ) + "<BR>";
        mensaje += "<B>Haber:</B> " + cur->valor ( "haber" ) + "<BR>";
        mensaje += "<HR>";
        mensaje += "<B>Saldo:</B> " + cur->valor ( "saldo" ) + "<BR>";
        mensaje += "</BODY></HTML>";
        g_res->setMensaje ( mensaje );
    } // end if
    delete cur;
    _depura ( "END BcBuscarCuenta_s_lostFocus_post", 0 );
    return 0;
}


///
/**
\param subform
\return
**/
int BcSubForm_on_mui_list_cellChanged_post ( BcSubForm *subform )
{
    _depura ( "BcSubForm_on_mui_list_cellChanged_post", 0 );
    if ( subform->currentRow() < 0 )
        return 0;
    if ( subform->existsHeader ( "codigo" ) ) {
        QString cod = subform->dbValue ( "codigo", subform->currentRow() );
        QString nom = subform->dbValue ( "descripcioncuenta", subform->currentRow() );
        QString id = subform->dbValue ( "idcuenta", subform->currentRow() );

        if ( id == "" ) return 0;
        BlDbRecordSet *cur = subform->mainCompany() ->loadQuery ( "SELECT *, debe-haber AS saldo FROM cuenta WHERE idcuenta = " + id );
        if ( !cur ) return 0;
        if ( ! cur->eof() ) {
            QString mensaje = "<HTML><BODY>";
            mensaje += "<B>Codigo:</B> " + cod + "<BR>";
            mensaje += "<B>Nombre:</B> " + nom + "<BR>";
            mensaje += "<B>Debe:</B> " + cur->valor ( "debe" ) + "<BR>";
            mensaje += "<B>Haber:</B> " + cur->valor ( "haber" ) + "<BR>";
            mensaje += "<HR>";
            mensaje += "<B>Saldo:</B> " + cur->valor ( "saldo" ) + "<BR>";
            mensaje += "</BODY></HTML>";
            g_res->setMensaje ( mensaje );
        } // end if
    } // end if
    return 0;
}


///
/**
\param bcont
**/
int BcBulmaCont_closeEvent ( BcBulmaCont *bcont )  {
    _depura("BcBulmaCont_closeEvent");
//    BcCompany * emp = bcont->empresaactual();
    QFile file ( g_confpr->valor ( CONF_DIR_USER ) + "pluginresumcta_.cfn" );
    if ( !viewCorrector->isChecked() ) {
        file.remove();
    } else {
        file.open ( QIODevice::WriteOnly );
        file.close();
    } // end if
    _depura("END BcBulmaCont_closeEvent");
    return 0;
}
