/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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
#include "bconfiguracion.h"
#include <unistd.h>
#include <qmessagebox.h>
#include <errno.h>
#include <qtable.h>
#include <qtabwidget.h>
#include <qfile.h>

#include "bnuevaempresa.h"
#include "nuevafact.h"

#ifdef WIN32
#include <process.h>
#endif

#ifndef WIN32
#include <map>
#include <set>
#endif



BConfiguracion::BConfiguracion(BSelector * ref, QWidget * parent, const char * name, WFlags f=0) : UIconfiguracion(parent,name,f) {
    PunteroAlSelector=ref;
    if (PunteroAlSelector->tipoEmpresa() == "BulmaCont") {
        cargarFichaBulmages();
        m_tab->setTabEnabled(m_tab->page(1),FALSE);
        m_tab->setTabEnabled(m_tab->page(2),FALSE);
        m_tab->setTabEnabled(m_tab->page(3),FALSE);
    }// end if
    if (PunteroAlSelector->tipoEmpresa() == "BulmaFact") {
        cargarFichaBulmages();
        m_tab->setTabEnabled(m_tab->page(0),FALSE);
        m_tab->setTabEnabled(m_tab->page(2),FALSE);
        m_tab->setTabEnabled(m_tab->page(3),FALSE);
    }// end if
    cargarFichaUsuarios();
    tablaconfiguracion();
}// end BConfiguracion

BConfiguracion::~BConfiguracion() {}

void BConfiguracion::cerrar() {
    // Guardamos la configuraci�n.
    for (int i =0;i<1000;i++) {
        if (confpr->nombre(i) != "") {
            confpr->setValor(i,m_tconfiguracion->text(i,1).ascii());
        }// end if
    }// end for

    confpr->saveconfig();
    //  traductor->load(archivo.c_str(),confpr->valor(CONF_DIR_TRADUCCION).c_str());
    //Cargo la nueva fuente
    //Cierro la ventana de Configuraci�n
    close();
}


/// Esta funci�n rellena la tabla de configuraci�n del programa.
/// Para luego poder guardar los parametros que haga falta.
void BConfiguracion::tablaconfiguracion() {
    m_tconfiguracion->setLeftMargin(0);
    m_tconfiguracion->hideColumn(2);
    m_tconfiguracion->setNumRows(1000);
    QString cad;
    for (int i =0;i<1000;i++) {
        cad= cad.setNum(i);
        m_tconfiguracion->setText(i,0,confpr->nombre(i));
        m_tconfiguracion->setText(i,1,confpr->valor(i));
        m_tconfiguracion->setText(i,2,cad);
        if (confpr->nombre(i) == "")
            m_tconfiguracion->hideRow(i);
    }// end for
    m_tconfiguracion->adjustColumn(0);
    m_tconfiguracion->adjustColumn(1);
}// end tablaconfiguracion




/**********************************************************************************************/
/* BLOQUE UTILIDADES                                                                          */
/**********************************************************************************************/
void BConfiguracion::cargarFichaBulmages() {
    lineEditA_1->setText(PunteroAlSelector->nombreempresa->text());
}// end cargarFichaBulmages


void BConfiguracion::FontChanged(const QString & ) {}// end FontChanged

void BConfiguracion::FontSizeChanged(int ) {}// end FontSizeChanged

void BConfiguracion::BotonA_10aceptar() {}// BotonA_10aceptar



#include "importContaplus.h"
/// Responde a la pusaci�n de importar datos de Contaplus a BulmaG�s
void BConfiguracion::BotonContaplus() {
    postgresiface2 *DBconn = new postgresiface2();
    DBconn->inicializa(PunteroAlSelector->empresaDB(), confpr->valor(CONF_LOGIN_USER).ascii(), confpr->valor(CONF_PASSWORD_USER).ascii());
    importContaplus *import= new importContaplus(DBconn,0,0,0);
    import->exec();
    delete import;
    delete DBconn;
}// end BotonContaplus


void BConfiguracion::BotonA_11rechazar() {
    //poner el comboBoxFuente y el comboBoxIdioma a sus valores anteriores.
    if (PunteroAlSelector->tipoEmpresa() == "BulmaCont") {
        cargarFichaBulmages();
    }// end if
}// end BotonA_11rechazar


// ---------------------------------------------------------------------------
// Esta funci�n se ejecuta cuando se lanza el cambio de nombre de la empresa.|
// ---------------------------------------------------------------------------
void BConfiguracion::BotonA_6nombreEmpresa() {
    if (lineEditA_1->isReadOnly() ) { //Activa el line edit para que pueda ser editado.
        lineEditA_1->setReadOnly(false);
        lineEditA_1->setPaletteBackgroundColor(QColor(255,255,255));
    } else { //Y guarda el nuevo nombre de empresa.
        lineEditA_1->setReadOnly(true);
        lineEditA_1->setPaletteBackgroundColor(QColor(255,255,0));
        PunteroAlSelector->nombreempresa->setText(lineEditA_1->text());
    }// end if
}// end BotonA_6nombreEmpresa


/*********************************************************************************************************/
/* Aqui abrimos el cuador de dialogo que nos permite crear una empresa nueva basada en la                */
/* BASE DE DATOS bgplangcont                                                                             */
/*********************************************************************************************************/
void BConfiguracion::nuevaEmpresa() {
    BNuevaEmpresa *n= new BNuevaEmpresa(this,"Creador",true);
    n->exec();
    delete n;
}//Fin nuevaEmpresa

/*********************************************************************************************************/
/* Aqui abrimos el cuador de dialogo que nos permite crear una empresa nueva basada en la                */
/* BASE DE DATOS bgplangcont                                                                             */
/*********************************************************************************************************/
void BConfiguracion::nuevaFacturacion() {
    nuevafact *n= new nuevafact(this,"Creador",true);
    n->exec();
    delete n;
}//Fin nuevaEmpresa


/*********************************************************************************************************/
/* Aqui creamos una nueva empresa que es una copia exacta de otra empresa que ya existe.                 */
/*********************************************************************************************************/
void BConfiguracion::BotonA_61clonarEmpresa() {
    QString dbEmpresa;
    if (dbEmpresa!=NULL) {
        QMessageBox::information( this, "Debug", "Clonando la Base de Datos: " +dbEmpresa +"\n\nProceso no disponible...\n", QMessageBox::Ok);
    }// end if
}

/*********************************************************************************************************/
/* Aqui borramos una empresa entera. No nos permite borrar la base de datos bgplangcont ni la base       */
/* de datos de la empresa que tengamos abierta en este momento.                                          */
/*********************************************************************************************************/
void BConfiguracion::borrarEmpresa() {
    QString dbEmpresa;
    QString nombreEmpresa;
    QString idEmpresa;
    QString ejercicio;
    // Siempre se borra la empresa actual.
    dbEmpresa = PunteroAlSelector->empresaDB();
    // (new BVisorEmpresas(& dbEmpresa, this,"Eliminador",true))->exec();
    if (dbEmpresa!="") {
        if (dbEmpresa=="bgplangcont") {
            QMessageBox::information( this, tr("Atenci�n"), tr("Esta Base de Datos no puede ser eliminada.\n\r Es la plantilla para generar nuevas empresas."), QMessageBox::Ok);
        } else if (dbEmpresa != PunteroAlSelector->empresaDB()) {
            QMessageBox::warning( this, tr("Atenci�n"), tr("No Est� permitido eliminar la base \n\r de datos actualmente abierta."), QMessageBox::Ok,0);
        } else {
	    int mensaje = QMessageBox::warning( this, tr("Atenci�n"), tr("Borrar una empresa puede suponer perdida de datos\n �Desea continuar?\n"), QMessageBox::Yes,QMessageBox::No,0);
	    if (mensaje == QMessageBox::Yes) { 
		//Despues de evaluar algunos detalles, procedemos a eliminar la base de datos.
/*		postgresiface2 *DBconn = new postgresiface2();
		DBconn->inicializa( "template1", confpr->valor(CONF_LOGIN_USER), confpr->valor(CONF_PASSWORD_USER) );
		DBconn->ejecuta("DROP DATABASE " + dbEmpresa);
		delete DBconn;
*/
		QString sentencia = "dropdb "+dbEmpresa;
		system(sentencia.ascii());
		PunteroAlSelector->seleccionaempresa_clicked();
	    }// end if
        }// end if
    }// end if
    close();
}//Fin borrarEmpresa




/**
* Aqui creamos un nuevo ejercicio para la empresa que tengamos abierta en el momento de llamar          
* a este proceso.                                                                                       
*/
void BConfiguracion::nuevoEjercicio() {
    QString query;
    int x;
    QString ejer;
    postgresiface2 *DBconn = new postgresiface2();
    DBconn->inicializa(PunteroAlSelector->empresaDB());
    ejer = m_ejercicio->text();
    if (ejer == "") {
        DBconn->begin();
        query.sprintf("SELECT MAX(ejercicio) AS ejercicio FROM ejercicios WHERE periodo=0");
        cursor2 *curA = DBconn->cargacursor(query,"curA");
        if (!curA->eof())
            ejer = curA->valor("ejercicio");
        ejer.setNum(ejer.toInt() + 1);
        if (ejer.toInt()<2000)
            ejer="2003";
    }// end if
    for (x=0; x<=12; x++) {
        query.sprintf("INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES('%s', '%d', 'f')",ejer.ascii(),x);
        DBconn->ejecuta(query);
    }// endfor
    DBconn->commit();
    QMessageBox::information( this, tr("Nuevo Ejercicio"), "El ejercicio ("+ ejer + ") ha sido creado con exito" , "&Aceptar");
}//Fin nuevoEjercicio




/*********************************************************************************************************/
/* Creamos una copia de seguridad de una base de datos                                                   */
/*********************************************************************************************************/
void BConfiguracion::salvarEmpresa() {
    QString dbEmpresa;
    QString PGserver;
    //  PGserver = "-h ";

    PGserver = confpr->valor(CONF_SERVIDOR).ascii();
    dbEmpresa = PunteroAlSelector->empresaDB();
    fprintf(stderr,"VAmos a guardar la empresa %s\n", dbEmpresa.ascii());

    //  (new BVisorEmpresas(& dbEmpresa, this,"Backup",true))->exec();
    if (dbEmpresa!="") {
        QString fn = QFileDialog::getSaveFileName(0, "Empresas (*.pgdump)", 0,"Guardar Empresa","Elige el nombre de empresa con el que guardar");
        if (!fn.isEmpty()) {
            if (fn.right(7)!= ".pgdump")
                fn = fn +".pgdump";
            fprintf(stderr,"Vamos a guardar la empresa en el fichero %s\n",fn.ascii());

            QString cadena;
            cadena.sprintf("%sguardaemp %s %s %s", confpr->valor(CONF_EJECUTABLES).ascii(), PGserver.ascii(), dbEmpresa.ascii(), fn.ascii() );
            fprintf(stderr,"%s\n", cadena.ascii());
            system(cadena.ascii());
        }// end if
    }// end if
}// end salvarEmpresa


/*********************************************************************************************************/
/* Restauramos una copia de seguridad de una base de datos                                               */
/*********************************************************************************************************/
// Para cargar la empresa debe estar sin ningun usuario dentro de ella.
void BConfiguracion::BotonA_4restaurarEmpresa() {
    fprintf(stderr,"Restaurar empresa \n");
    QString dbEmpresa;
    QString PGserver;
    PGserver = confpr->valor(CONF_SERVIDOR).ascii();
    QString usuario;

    dbEmpresa = PunteroAlSelector->empresaDB();
    if (dbEmpresa!="") {
        fprintf(stderr,"Restaurar empresa cargamos fichero\n");
        //     QString fn = QFileDialog::getOpenFileName(0, theApp->translate("empresa","Empresas (*.pgdump)",""), 0,theApp->translate("empresa","Cargar Empresa",""),theApp->translate("emrpesa","Elige el fichero a cargar.",""));
        QString fn = QFileDialog::getOpenFileName(0, "Empresas (*.pgdump)", 0,"Cargar Empresa","Elige el fichero a cargar.");

        if (!fn.isEmpty()) {
            QString comando= "cargaemp "+PGserver+" "+dbEmpresa+" "+fn;
            fprintf(stderr,"%s\n", comando.ascii());
            system (comando.ascii());
        }// end if
    }// end if
}

/**********************************************************************************************/
/* BLOQUE USUARIOS                                                                            */
/**********************************************************************************************/

void BConfiguracion::cargarFichaUsuarios() {}


void BConfiguracion::listView1_currentChanged(QListViewItem *) {
#ifndef WIN32
#endif
}// end listView1_currentChanged


//Salvamos los usuarios en la base de datos
void BConfiguracion::BotonB_1Aplicar() {}

//Desacemos los cambios que hemos hecho (UNDO).
void BConfiguracion::BotonB_2Desacer() {
    cargarFichaUsuarios();
}

void BConfiguracion::listView2_clickBotonDerecho(QListViewItem* ,const QPoint&  ,int ) {}


//Por conveniencia (Bug QT??)
void BConfiguracion::listiView2_clickMouse(int,QListViewItem * ,const QPoint& ,int ) {}
