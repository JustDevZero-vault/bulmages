/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borr�s                                    *
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
 ***************************************************************************/
#include "importContaplus.h"
#include "pgimportfiles.h"

#include <qfile.h>
#include <qfiledialog.h>
#include <qlineedit.h>
#include <qprogressbar.h>
#include <qtextbrowser.h>
#include <qcheckbox.h>

#include <qtoolbutton.h>

#include "calendario.h"

QProgressBar *progress;
QTextBrowser *mensajes;
QString mensajein="";
void realizado(int a, int b) {
	fprintf(stderr,"mensaje publicado");
	progress->setProgress(a,b);
}// end realizado

void publicamensaje(QString mensaje) {
	mensajein+=mensaje;
	mensajes->setText(mensajein);
	mensajes->scrollBy(0,400);
}// end publicamensaje


importContaplus::importContaplus(postgresiface2 * con, QWidget * parent, const char * name, WFlags f=0) :  importContaplusBase(parent,name,f){
	progress = m_progressbar;
	mensajes = m_mensajes;
	conexionbase = con;
}//end importContaplus

/// Se ha pulsado sobre el bot�n de b�squeda de una subcuenta.
void importContaplus::botonBuscarXML() {
	m_XML->setText( QFileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER),"Contaplus (*.xml)", this, "select file", "Elija el Archivo"));
}// end botonBuscarSubCta

/// Se ha pulsado sobre el bot�n de b�squeda de una subcuenta.
void importContaplus::botonBuscarSubCta() {
	m_subCta->setText( QFileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER),"Contaplus (*.txt)", this, "select file", "Elija el Archivo"));
}// end botonBuscarSubCta

/** \brief SLOT que responde a la pulsaci�n de selecci�n de archivo.
  */
void importContaplus::botonBuscarDiario() {
	m_diario->setText( QFileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER),"Contaplus (*.txt)", this, "select file", "Elija el Archivo"));
}// end botonBuscarDiario	

void importContaplus::botonImportar() {
	QString finicial = m_fInicial->text();
	QString ffinal = m_fFinal->text();
	void (*func) (int,int);
	func = realizado;
	void (*func1) (QString);
	func1 = publicamensaje;
	pgimportfiles *importacion = new pgimportfiles(conexionbase,func, func1);
	if (m_subCta->text() != "") {
		QFile filecont (m_subCta->text());
		QFile fileasie (m_diario->text());
		filecont.open(IO_ReadOnly);
		fileasie.open(IO_ReadOnly);
		importacion->setFInicial(finicial);
		importacion->setFFinal(ffinal);	
		if (m_test->isChecked() ) {
			importacion->setModoTest();
		}// end if
		importacion->contaplus2Bulmages(filecont, fileasie);
		filecont.close();
		fileasie.close();
		delete importacion;
	} else {
		QFile filexml (m_XML->text());
		filexml.open(IO_ReadOnly);
		importacion->XML2Bulmages(filexml);
		filexml.close();
	}// end if
	mensajein="";
}// end botonImportar 


/** \brief SLOT que responde a la pulsaci�n del bot�n de exportar
  * 
  * Se ha pulsado sobre el bot�n de exportar. Lee los campos del formulario
  * mira si la opci�n de exportaci�n es XML o contaplus y llama a las funciones apropiadas
  * de la clase \ref pgimportfiles 
  * Esta funci�n utiliza los punteros a funci�n para inicializar \ref pgimportfiles con las funciones que se van a
  * encargar de presentaci�n del estado de la importaci�n.
  * \todo Los punteros a funci�n deber�an ser reemplazados por funciones virtuales y haciendo derivar esta clase de pgimportfiles.
  */
void importContaplus::botonExportar() {
	/// Leemos las fechas entre las que tiene que ser el listado.
	QString finicial = m_fInicial->text();
	QString ffinal = m_fFinal->text();
	
	/// funci�n de completitud. \ref pgimportfiles llamar� a esta funci�n para indicar el progreso de la exportaci�n.
	void (*func) (int,int);
	func = realizado;
	/// Funci�n de mensajeria. \ref pgimportfiles llamar� a esta funci�n para presentar mensajes.
	void (*func1) (QString);
	func1 = publicamensaje;
	pgimportfiles *importacion = new pgimportfiles(conexionbase,func, func1);
	importacion->setFInicial(finicial);
	importacion->setFFinal(ffinal);	
	if (m_test->isChecked() ) {
		importacion->setModoTest();
	}// end if	
	if (m_subCta->text() != "") {
		QFile filecont (m_subCta->text());
		QFile fileasie (m_diario->text());
		filecont.open(IO_WriteOnly);
		fileasie.open(IO_WriteOnly);
		importacion->bulmages2Contaplus(filecont, fileasie);
		filecont.close();
		fileasie.close();
	} // end if
	if (m_XML->text() != "") {
		QFile filexml (m_XML->text());
		filexml.open(IO_WriteOnly);
		importacion->bulmages2XML(filexml);
		filexml.close();
	}// end if
	delete importacion;
	mensajein="";
}// end botonExportar


/** \brief SLOT que responde a la b�squeda de la fecha inicial
  */
void importContaplus::botonBuscarFInicial() {
        QList<QDate> a;
        calendario *cal = new calendario(0,0);
        cal->exec();
        a = cal->dn->selectedDates();
        m_fInicial->setText(a.first()->toString("dd/MM/yyyy"));
        delete cal;
}// end botonBuscarFInicial

/** \brief SLOT quer esponde a la busqueda de la fecha final
  */
void importContaplus::botonBuscarFFinal() {
        QList<QDate> a;
        calendario *cal = new calendario(0,0);
        cal->exec();
        a = cal->dn->selectedDates();
        m_fFinal->setText(a.first()->toString("dd/MM/yyyy"));
        delete cal;
}// end botonBuscarFFinal

