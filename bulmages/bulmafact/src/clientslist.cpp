/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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

 /*
-- El cliente siempre tiene la raz�n, bueno, o por lo menos eso cree.
--Codi: Clau artificial.
--Nom: Nom comercial o fiscal.
--Nom_alternatiu: Nom comercial o fiscal.
--CIF: Codi d'Identificaci� Fiscal.
--C_Banc: Compte Bancari.
--Adr: Adre�a.
--Pobl: Poblaci�.
--CProv: Codi de provincia (dos primers d�gits del codi postal).
--sCP: Tres darrers d�gits del codi postal.
--Telf: Tel�fon.
--Fax: Fax.
--Email: eMail.
--Url: Url.
--Data_alta
--Data_Baixa
---Comentaris
CREATE TABLE cliente (
   idcliente serial PRIMARY KEY,
   nomcliente character varying(100),
   nomaltcliente character varying(300),
   cifcliente character varying(200),
   bancocliente character varying(35),
   dircliente character varying(100),
   poblcliente character varying(40),
   cpcliente character varying(10),
   telcliente character varying(20),
   faxcliente character varying(20),
   mailcliente character varying(100),
   urlcliente character varying(150),
   faltacliente date DEFAULT NOW(),
   fbajacliente date,
   comentcliente character varying(2000),
   
   idrecargo integer NOT NULL REFERENCES recargo(idrecargo)
);
*/

#include "clientslist.h"
#include "clientedit.h"
#include "company.h"
#include <qmessagebox.h>
#include <qtable.h>
#include <qlineedit.h>

#define COL_IDCLIENTE 0
#define COL_NOMCLIENTE 1
#define COL_NOMALTCLIENTE 2
#define COL_CIFCLIENTE 3
#define COL_BANCOCLIENTE 4
#define COL_DIRCLIENTE 5
#define COL_POBLCLIENTE 6
#define COL_CPCLIENTE 7
#define COL_TELCLIENTE 8
#define COL_FAXCLIENTE 9
#define COL_MAILCLIENTE 10
#define COL_URLCLIENTE 11
#define COL_FALTACLIENTE 12
#define COL_FBAJACLIENTE 13
#define COL_COMENTCLIENTE 14
#define COL_IDRECARGO 15

#define EDIT_MODE 0

ClientsList::ClientsList(company *comp, QWidget *parent, const char *name, int flag)
 : ClientsListBase(parent, name, flag) {
   companyact = comp;
   m_idclient="";
   m_cifclient="";
   m_nomclient="";
   m_mode = EDIT_MODE;
 
   m_clientList->setNumRows( 0 );
   m_clientList->setSelectionMode( QTable::SingleRow );
   m_clientList->setSorting( TRUE );
   m_clientList->setColumnMovingEnabled( TRUE );
   m_clientList->setNumCols(16);
   m_clientList->horizontalHeader()->setLabel( COL_IDCLIENTE, tr( "C�digo" ) );
   m_clientList->horizontalHeader()->setLabel( COL_NOMCLIENTE, tr( "Nombre Fiscal" ) );
   m_clientList->horizontalHeader()->setLabel( COL_NOMALTCLIENTE, tr( "Nombre Comercial" ) );
   m_clientList->horizontalHeader()->setLabel( COL_CIFCLIENTE, tr( "CIF/NIF" ) );
   m_clientList->horizontalHeader()->setLabel( COL_BANCOCLIENTE, tr( "Cuenta Bancaria" ) );
   m_clientList->horizontalHeader()->setLabel( COL_DIRCLIENTE, tr( "Domicilio" ) );
   m_clientList->horizontalHeader()->setLabel( COL_POBLCLIENTE, tr("Poblaci�n") );
   m_clientList->horizontalHeader()->setLabel( COL_CPCLIENTE, tr("C.P.") );
   m_clientList->horizontalHeader()->setLabel( COL_TELCLIENTE, tr("Tel�fono") );
   m_clientList->horizontalHeader()->setLabel( COL_FAXCLIENTE, tr("Fax") );
   m_clientList->horizontalHeader()->setLabel( COL_MAILCLIENTE, tr("Correo Electr�nico") );
   m_clientList->horizontalHeader()->setLabel( COL_URLCLIENTE, tr("P�gina Web") );
   m_clientList->horizontalHeader()->setLabel( COL_FALTACLIENTE, tr("Fecha de Alta") );
   m_clientList->horizontalHeader()->setLabel( COL_FBAJACLIENTE, tr("Fecha de Baja") );
   m_clientList->horizontalHeader()->setLabel( COL_COMENTCLIENTE, tr("Observaciones") );
   m_clientList->horizontalHeader()->setLabel( COL_IDRECARGO, tr("Cod. Recargo Eq.") );

   m_clientList->setColumnWidth(COL_IDCLIENTE,100);
   m_clientList->setColumnWidth(COL_NOMCLIENTE,300);
   m_clientList->setColumnWidth(COL_NOMALTCLIENTE,300);
   m_clientList->setColumnWidth(COL_CIFCLIENTE,75);
   m_clientList->setColumnWidth(COL_BANCOCLIENTE,100);
   m_clientList->setColumnWidth(COL_DIRCLIENTE,300);
   m_clientList->setColumnWidth(COL_POBLCLIENTE,200);
   m_clientList->setColumnWidth(COL_CPCLIENTE,60);
   m_clientList->setColumnWidth(COL_TELCLIENTE,100);
   m_clientList->setColumnWidth(COL_FAXCLIENTE,100);
   m_clientList->setColumnWidth(COL_MAILCLIENTE,400);
   m_clientList->setColumnWidth(COL_URLCLIENTE,400);
   m_clientList->setColumnWidth(COL_FALTACLIENTE,75);
   m_clientList->setColumnWidth(COL_FBAJACLIENTE,75);
   m_clientList->setColumnWidth(COL_COMENTCLIENTE,1000);
   m_clientList->setColumnWidth(COL_IDRECARGO,50);
   
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_clientList->setPaletteBackgroundColor("#DDDDFF");   
    m_clientList->setReadOnly(TRUE);        
    
   initClients();
   companyact->meteWindow(caption(),this);

}// end clientslist

ClientsList::~ClientsList() {
	companyact->sacaWindow(this);
}// end ~clientslist


/**
  * Iniciamos los clientes.
  * Hacemos la consulta a la base de datos y presentamos el listado.
  */
void ClientsList::initClients() {
       companyact->begin();
       cursor2 * cur= companyact->cargacursor("SELECT * FROM cliente  WHERE nomcliente LIKE '%"+m_findClient->text()+"%'","unquery");
       companyact->commit();
       m_clientList->setNumRows( cur->numregistros() );
       int i=0;
       while (!cur->eof()) {
         m_clientList->setText(i,COL_IDCLIENTE,cur->valor("idcliente"));
         m_clientList->setText(i,COL_NOMCLIENTE,cur->valor("nomcliente"));
         m_clientList->setText(i,COL_NOMALTCLIENTE,cur->valor("nomaltcliente"));
         m_clientList->setText(i,COL_CIFCLIENTE,cur->valor("cifcliente"));
         m_clientList->setText(i,COL_BANCOCLIENTE,cur->valor("bancocliente"));
         m_clientList->setText(i,COL_DIRCLIENTE,cur->valor("dircliente"));
         m_clientList->setText(i,COL_POBLCLIENTE,cur->valor("poblcliente"));
         m_clientList->setText(i,COL_CPCLIENTE,cur->valor("cpcliente"));
         m_clientList->setText(i,COL_TELCLIENTE,cur->valor("telcliente"));
         m_clientList->setText(i,COL_FAXCLIENTE,cur->valor("faxcliente"));
         m_clientList->setText(i,COL_MAILCLIENTE,cur->valor("mailcliente"));
         m_clientList->setText(i,COL_URLCLIENTE,cur->valor("urlcliente"));
         m_clientList->setText(i,COL_FALTACLIENTE,cur->valor("faltacliente"));
         m_clientList->setText(i,COL_FBAJACLIENTE,cur->valor("fbajacliente"));
         m_clientList->setText(i,COL_COMENTCLIENTE,cur->valor("comentcliente"));
         m_clientList->setText(i,COL_IDRECARGO,cur->valor("idrecargo"));
         i++;
         cur->siguienteregistro();
       }// end while
      delete cur;
}// end initClients


void ClientsList::m_clientList_clicked(int a, int , int , const QPoint &) {
   m_idclient = m_clientList->text(a,COL_IDCLIENTE);
}// end m_clientList_clicked

void ClientsList::newClient() {
   companyact->newClient();
}// end newClient

void ClientsList::editClient() {
   ClientEdit *cli = new ClientEdit(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Clientes", "company"));
   cli->loadClient(m_idclient);
   cli->show();
}

/**
   *  Se ha hecho un doble click sobre la tabla de Clientes.
  */
void ClientsList::m_clientList_doubleClicked(int a, int , int , const QPoint &) {
   m_idclient = m_clientList->text(a,COL_IDCLIENTE);
   m_cifclient = m_clientList->text(a,COL_CIFCLIENTE);
   m_nomclient = m_clientList->text(a, COL_NOMCLIENTE);
   if (m_mode == EDIT_MODE ) {
      ClientEdit *cli = new ClientEdit(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Clientes", "company"));
      cli->loadClient(m_idclient);
      cli->show();
   } else {
      close();
   }// end if
}// end doubleClicked


void ClientsList::s_findClients() {
	initClients();
}// end s_findClients


void ClientsList::s_printClients() {
	fprintf(stderr,"Impresi�n del listado\n");
// #ifdef REPORTS
    	/// Mediante comandos de sistema reemplazamos lo que necesitamos para obtener un fichero deseable.
	QString cadena;
	// ACORDARSE DE CAMBIAR LAS RUTAS POR LAS DEL ARCHIVO DE CONFIGURACION.
	cadena = "cp /home/tborras/bulmages/installbulmages/reports/bulma-styles.xml   /tmp/bulma-styles.xml" ;
	system (cadena.ascii());	
	cadena = "cp /home/tborras/bulmages/installbulmages/reports/clientslist.rtk   /tmp/clientslist.rtk" ;
	system (cadena.ascii());	
	cadena = "rtkview --input-sql-driver QPSQL7 --input-sql-database ";
	cadena += companyact->nameDB()+" ";
	// OJO QUE LA LINEA BUENA ES LA QUE ESTA COMENTADA
//	cadena += confpr->valor(CONF_DIR_REPORTS)+"cuentas.rtk &";
	// ESTA LINEA ES PARA HACER PRUEBAS
//	cadena += "/home/tborras/bulmages/installbulmages/reports/extracto1.rtk &";
	cadena += "/tmp/clientslist.rtk &";
	fprintf(stderr,"%s\n",cadena.ascii());
	system (cadena.ascii());    
// #endif

}// end s_printClients



