/***************************************************************************
 *   Copyright (C) 2004 by Alvaro de Miguel                                *
 *   alvaro_demiguel@gmail.com                                             *
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
CREATE TABLE articulo (
    idarticulo serial PRIMARY KEY,
    codarticulo character varying(12),
    nomarticulo character varying(50),
    descarticulo character varying(500),
    cbarrasarticulo character varying(22),
    tipoarticulo integer,
    descuentoarticulo float,
    especificacionesarticulo character varying(2000),
    iconoarticulo oid,
    fotoarticulo oid,
    posterarticulo oid,
    margenarticulo float,
    sobrecostearticulo float,
    modeloarticulo character varying(1000),
    
    idtipo_iva integer REFERENCES tipo_iva (idtipo_iva),
    idlinea_prod integer REFERENCES linea_prod(idlinea_prod)
); 
*/ 
 
#include "articleedit.h"
#include "company.h"
#include "division.h"
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qtable.h>

#define COL_SUMINISTRA_IDSUMINISTRA 0
#define COL_SUMINISTRA_IDPROVEEDOR 0
#define COL_SUMINISTRA_NOMPROVEEDOR 1
#define COL_SUMINISTRA_REFPRO 2
#define COL_SUMINISTRA_PVD 3
#define COL_SUMINISTRA_BENEFICIO 4
#define COL_SUMINISTRA_SOBRECOSTE 5
#define COL_SUMINISTRA_PRINCIPAL 6
#define COL_SUMINISTRA_REFERENT 7


articleedit::articleedit(company *comp, QWidget *parent, const char *name)
 : articleeditbase(parent, name, Qt::WDestructiveClose) {
   companyact = comp;
   idArticle = "0";
   
   
   // Arreglamos la tabla de proveedores del art�culo
   m_suministra->setNumRows( 0 );
   m_suministra->setNumCols( 0 );
   m_suministra->setSelectionMode( QTable::SingleRow );
   m_suministra->setSorting( TRUE );
   m_suministra->setSelectionMode( QTable::SingleRow );
   m_suministra->setColumnMovingEnabled( TRUE );
   m_suministra->setNumCols(8);
   m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_IDPROVEEDOR, tr( "Id. Proveedor" ) );
	m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_NOMPROVEEDOR, tr( "Nombre Proveedor" ) );
	m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_REFPRO, tr( "Referencia del Proveedor" ) );
	m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_PVD, tr( "PVD" ) );
	m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_BENEFICIO, tr( "Beneficio" ) );
	m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_SOBRECOSTE, tr( "Sobrecoste" ) );
	m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_PRINCIPAL, tr( "Es Principal" ) );
	m_suministra->horizontalHeader()->setLabel( COL_SUMINISTRA_REFERENT, tr( "Es de Referencia" ) );
   
	m_suministra->setColumnWidth(COL_SUMINISTRA_IDPROVEEDOR,100);
	m_suministra->setColumnWidth(COL_SUMINISTRA_NOMPROVEEDOR,300);
	m_suministra->setColumnWidth(COL_SUMINISTRA_REFPRO,100);
	m_suministra->setColumnWidth(COL_SUMINISTRA_PVD,100);
	m_suministra->setColumnWidth(COL_SUMINISTRA_BENEFICIO,100);
	m_suministra->setColumnWidth(COL_SUMINISTRA_SOBRECOSTE,100);
	m_suministra->setColumnWidth(COL_SUMINISTRA_PRINCIPAL,100);
	m_suministra->setColumnWidth(COL_SUMINISTRA_REFERENT,100);

}// end articleedit

articleedit::~articleedit() {
}// end ~articleedit


/************************************************************************
* Esta funci�n carga un proveedor de la base de datos y lo presenta.    *
* Si el parametro pasado no es un identificador v�lido entonces se pone *
* la ventana de edici�n en modo de inserci�n                            *
*************************************************************************/
void articleedit::chargeArticle(QString idArt) {
   idArticle = idArt;
   fprintf(stderr,"chargeArticle activado \n");
   if (idArticle != "0") {
      QString SQLQuery = "SELECT * FROM articulo WHERE idarticulo="+idArt;
      companyact->begin();
      cursor2 *cur= companyact->cargacursor(SQLQuery, "unquery");
      companyact->commit();
      if (!cur->eof()) {
         idArticle = idArt;
         m_articleCode->setText(cur->valor("codarticulo"));
         m_articleName->setText(cur->valor("nomarticulo"));
         m_articleDesc->setText(cur->valor("descarticulo"));
         m_barCode->setText(cur->valor("cbarrasarticulo"));
  //       m_comboIvaType ...
         m_articleDiscount->setText(cur->valor("descuentoarticulo"));
         //m_specifications->setText(cur->valor("especificacionesarticulo"));
         m_articleMargin->setText(cur->valor("margenarticulo"));
         m_articleOverCost->setText(cur->valor("sobrecostearticulo"));
         m_articleModel->setText(cur->valor("modeloarticulo"));
   //      m_productionLine->setText());
         
         // Suministra relation loading
         // Cargamos las relaciones art�culo - proveedor.
         QString SQLQuery1 = "SELECT * FROM suministra WHERE idarticulo="+idArt;
         companyact->begin();
         cursor2 *cur1 = companyact->cargacursor(SQLQuery1, "cargaSuministra");
         companyact->commit();
         m_suministra->setNumRows(cur1->numregistros());
         int i=0;
         while (!cur1->eof()) {
            m_suministra->setText(i,COL_SUMINISTRA_IDPROVEEDOR,cur1->valor("idproveedor"));
       //     m_suministra->setText(i,COL_SUMINISTRA_NOMPROVEEDOR,cur1->valor("nomproveedor"));
            m_suministra->setText(i,COL_SUMINISTRA_REFPRO,cur1->valor("refprosuministra"));
            m_suministra->setText(i,COL_SUMINISTRA_PVD,cur1->valor("pvdsuministra"));
            m_suministra->setText(i,COL_SUMINISTRA_BENEFICIO,cur1->valor("beneficiosuministra"));
            m_suministra->setText(i,COL_SUMINISTRA_SOBRECOSTE,cur1->valor("sobrecostesuministra"));
            m_suministra->setText(i,COL_SUMINISTRA_PRINCIPAL,cur1->valor("principalsuministra"));
            m_suministra->setText(i++,COL_SUMINISTRA_REFERENT,cur1->valor("referentsuministra"));
            cur1->siguienteregistro();
         }// end while
         delete cur1;
      } else {
         idArticle="0";
      }// end if
      delete cur;
   }// end if
}// end chargeArticle

/************************************************************************
* Esta funci�n se ejecuta cuando se ha pulsado sobre el bot�n de nuevo  *
*************************************************************************/
void articleedit::boton_nuevo() {
	idArticle = "0";
	m_articleCode->setText("");
	m_articleName->setText("");
	m_articleDesc->setText("");
	m_barCode->setText("");
  //       m_comboIvaType ...
	m_articleDiscount->setText("");
//	m_specifications->setText("");
	m_articleMargin->setText("");
	m_articleOverCost->setText("");
	m_articleModel->setText("");
	m_productionLine->setText("");
}// end boton_nuevo

/*************************************************************************
* Esta funci�n es la respuesta a la pulsaci�n del boton de guardar       *
* Comprueba si es una inserci�n o una modificaci�n y hace los pasos      *
* pertinentes                                                            *
**************************************************************************/
void articleedit::accept() {
/*   if (idprovider != "0") {
      QString SQLQuery = "UPDATE proveedor SET urlproveedor='"+m_urlproveedor->text()+"'";
      SQLQuery += " , nomproveedor='"+m_nomproveedor->text()+"'";
      SQLQuery += " , nomaltproveedor='"+m_nomaltproveedor->text()+"'";
      SQLQuery += " , cifproveedor='"+m_cifproveedor->text()+"'";
      SQLQuery += " , codicliproveedor='"+m_codicliproveedor->text()+"'";
      SQLQuery += " , cbancproveedor='"+m_cbancproveedor->text()+"'";
      SQLQuery += " , dirproveedor='"+m_dirproveedor->text()+"'";
      SQLQuery += " , poblproveedor='"+m_poblproveedor->text()+"'";
      SQLQuery += " , cpproveedor='"+m_cpproveedor->text()+"'";
      SQLQuery += " , telproveedor='"+m_telproveedor->text()+"'";
      SQLQuery += " , faxproveedor='"+m_faxproveedor->text()+"'";
      SQLQuery += " , emailproveedor='"+m_emailproveedor->text()+"'";
      SQLQuery += " WHERE idproveedor ="+idprovider;
      companyact->begin();
      companyact->ejecuta(SQLQuery);
      companyact->commit();
      close();
   } else {
      QString SQLQuery = " INSERT INTO proveedor (nomproveedor, nomaltproveedor, cifproveedor, codicliproveedor, cbancproveedor, dirproveedor, poblproveedor, cpproveedor, telproveedor, faxproveedor, urlproveedor, emailproveedor)";
      SQLQuery += " VALUES (";
      SQLQuery += "'"+m_nomproveedor->text()+"'";
      SQLQuery += ",'"+m_nomaltproveedor->text()+"'";
      SQLQuery += ",'"+m_cifproveedor->text()+"'";
      SQLQuery += ",'"+m_codicliproveedor->text()+"'";
      SQLQuery += ",'"+m_cbancproveedor->text()+"'";
      SQLQuery += ",'"+m_dirproveedor->text()+"'";
      SQLQuery += ",'"+m_poblproveedor->text()+"'";
      SQLQuery += ",'"+m_cpproveedor->text()+"'";
      SQLQuery += ",'"+m_telproveedor->text()+"'";
      SQLQuery += ",'"+m_faxproveedor->text()+"'";
      SQLQuery += ",'"+m_urlproveedor->text()+"'";
      SQLQuery += ",'"+m_emailproveedor->text()+"'";
      SQLQuery += ")";
      companyact->begin();
      companyact->ejecuta(SQLQuery);
      companyact->commit(); 
      close();
   }// end if */
}// end accept

/************************************************************************
* Esta funci�n se ejecuta cuando se ha pulsado sobre el bot�n de borrar *
*************************************************************************/
void articleedit::boton_borrar() {
   if (idArticle != "0") {
      if ( QMessageBox::Yes == QMessageBox::question(this,"Borrar Art�culo","Esta a punto de borrar un art�culo, Estos datos pueden dar problemas.",QMessageBox::Yes, QMessageBox::No)) {
         QString SQLQuery="DELETE FROM articulo WHERE idarticulo="+idArticle;
         companyact->begin();
         companyact->ejecuta(SQLQuery);
         companyact->commit();
         close();
      }// end if
   }// end if
}// end boton_borrar

