/***************************************************************************
                          ivaview.cpp  -  description
                             -------------------
    begin                : Tue Jan 28 2003
    copyright            : (C) 2003 by Tomeu Borr�s Riera & Josep Burci�n
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

/**
-- La tabla prevcobro es prevision de cobros / pagos para facturas.
-- Esta tabla proporciona las formas de pago de cada factura que se pasa a un cliente o que recibimos de un proveedor.

-- tipoprevcobro indica si es un cobro (true) o un pago (false).
-- fpagoprevcobro es un identificador a la otra tabla de Formas de Pago.
-- idcuenta La cuenta sobre la que se har� el cobro / pago.
-- idasiento Asiento de cobro (Si est� hecho).
-- cantidadprevistaprevcobro Cantidad Prevista del cobro.
-- cantidadprevcobro         Cantidad Cobrada.
-- fprevistaprevcobro        Fecha en que se prevee el cobro / pago.
-- fcobroprevcobro           Fecha en que se ha realizado el cobro / pago.
-- idregistroiva             Registro de IVA con el que se corresponde, o factura con la que se corresponde.
-- tipoprevcobro             Indica si es un cobro o un pago.


CREATE TABLE prevcobro (
    idprevcobro serial PRIMARY KEY,
    fprevistaprevcobro date,
    fcobroprevcobro date,
    fpagoprevcobro integer,
    idcuenta integer REFERENCES cuenta(idcuenta),
    idasiento integer REFERENCES asiento(idasiento),
    cantidadprevistaprevcobro double,
    cantidadprevcobro         double,
    idregistroiva NOT NULL REFERENCES registroiva(idregistroiva),
    tipoprevcobro boolean
);

*/
 
 
 
 
#include "ivaview.h"
#include "listcuentasview1.h"
#include "empresa.h"
#include "calendario.h"

#define IVA_SOPORTADO    1
#define IVA_REPERCUTIDO  2

#define COL_PREV_IDPREVCOBRO 0
#define COL_PREV_FPREVISTAPREVCOBRO 1
#define COL_PREV_FCOBROPREVCOBRO     2
#define COL_PREV_IDCUENTA  3
#define COL_PREV_CODCUENTA 4
#define COL_PREV_NOMCUENTA 5
#define COL_PREV_CANTIDADPREVISTAPREVCOBRO 6
#define COL_PREV_CANTIDADPREVCOBRO 7
#define COL_PREV_FPAGOPREVCOBRO 8
#define COL_PREV_IDREGISTROIVA 9




ivaview::ivaview(empresa *emp,QWidget *parent, const char *name ) : ivadlg(parent,name) {
   empresaactual = emp;
   conexionbase = emp->bdempresa();
   idborrador=0;
   idregistroiva =0;
  
   m_listPrevision->setNumCols(10);
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_IDPREVCOBRO, tr( "IDPREVCOBRO") );
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_FPREVISTAPREVCOBRO, tr( "COL_PREV_FPREVISTAPREVCOBRO") );
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_FCOBROPREVCOBRO, tr( "FCOBROPREVCOBRO") );
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_IDCUENTA, tr( "IDCUENTA") );
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_CODCUENTA, tr( "CODCUENTA") );
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_NOMCUENTA, tr( "NOMCUENTA") );
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_CANTIDADPREVISTAPREVCOBRO, tr( "COL_PREV_CANTIDADPREVISTAPREVCOBRO") );
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_CANTIDADPREVCOBRO, tr( "CANTIDADPREVCOBRO") );
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_FPAGOPREVCOBRO, tr( "FPAGOPREVCOBRO") );
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_IDREGISTROIVA, tr( "IDREGISTROIVA") );
   m_listPrevision->setColumnWidth(COL_PREV_FPREVISTAPREVCOBRO,90);
   m_listPrevision->setColumnWidth(COL_PREV_FCOBROPREVCOBRO,90);
   m_listPrevision->setColumnWidth(COL_PREV_CODCUENTA,75);
   m_listPrevision->setColumnWidth(COL_PREV_CANTIDADPREVISTAPREVCOBRO,75);
   m_listPrevision->setColumnWidth(COL_PREV_CANTIDADPREVCOBRO,75);
   m_listPrevision->setColumnWidth(COL_PREV_FPAGOPREVCOBRO,75);
   
   // Ocultamos las columnas que son de un tipo espec�fico.
   m_listPrevision->hideColumn(COL_PREV_IDCUENTA);
   m_listPrevision->hideColumn(COL_PREV_IDPREVCOBRO);
   m_listPrevision->hideColumn(COL_PREV_IDREGISTROIVA);
   
   m_listPrevision->setNumRows(50);
}// end ivaview


ivaview::~ivaview(){
}// end ~ivaview


int ivaview::inicializa(int sop_rep=0) {
  setSoportadoRepercutido(sop_rep);
  return(0);
}// end inicializa


// Esquema:
// Si se trata de una modificacion modificamos.
// Si se trata de una insercion insertamos.
void ivaview::accept() {
  QString query;
  float baseimponible1=atof(baseimponible->text().ascii());
  float iva1 = atof(iva->text().ascii());
  QString factura1= factura->text();
  
  QString cif1 = cif->text();
  conexionbase->begin();
  cursor2 *cursorcuenta =conexionbase->cargacuenta(0,contrapartida->text());
  conexionbase->commit();
  if (!cursorcuenta->eof()) {
    int idcuenta= atoi(cursorcuenta->valor("idcuenta").ascii());
    if (idregistroiva !=0) {
          // Se trata de una inserci�n, ya que no hay registros de iva introducidos.
         query.sprintf("UPDATE registroiva set idborrador=%d, baseimp=%2.2f, iva=%2.0f, contrapartida=%d, factura='%s', numorden='%s', cif='%s' WHERE idregistroiva=%d", idborrador, baseimponible1, iva1, idcuenta, factura1.ascii(), numorden->text().ascii(), cif1.ascii(), idregistroiva);
    } else {
          // Se trata de una modificacion, ya que hay registros de iva introducidos.
         query.sprintf("INSERT INTO registroiva (idborrador, baseimp, iva, contrapartida, factura, numorden, cif) VALUES (%d,%2.2f, %2.0f, %d, '%s', '%s', '%s')", idborrador, baseimponible1, iva1, idcuenta, factura1.ascii(), numorden->text().ascii(), cif1.ascii());
    }// end if
    fprintf(stderr,"%s\n",query.ascii());
    conexionbase->begin();
    conexionbase->ejecuta(query);
    conexionbase->commit();
  }// end if
  delete cursorcuenta;
  
  // Guardamos todas las previsiones de pago y asi nos curamos en salud
  guardaprevpago();
  
  done(1);
}// end accept


/***************************************************************************
 * Se ha pulsado sobre el boton de borrar en ivaview por lo que borraremos *
 * el registro actual.                                                     *
 ***************************************************************************/
void ivaview::boton_borrar() {
  QString query;
  if (idregistroiva != 0) {
    query.sprintf("DELETE FROM registroiva WHERE idregistroiva=%d",idregistroiva);
    conexionbase->begin();
    conexionbase->ejecuta(query);
    conexionbase->commit();
  }// end if
  done(1);
}// end boton_borrar


void ivaview::boton_buscacuenta() {
   listcuentasview1 *listcuentas = new listcuentasview1();
   listcuentas->modo=1;
   listcuentas->inicializa(conexionbase);
   listcuentas->exec();
   contrapartida->setText(listcuentas->codcuenta);
   delete listcuentas;
}// end boton_buscacuenta


void ivaview::cambiadacontrapartida() {
   fprintf(stderr,"cambiada la contrapartida\n");
}// end cambiadacontrapartida


// Esquema:
// 1.- Comprobamos que no haya ya un registro con la factura
// 2.- Si hay factura la ponemos, si no la hay sacamos los datos que podemos y los presentamos
void ivaview::inicializa1(int idapunte1) {
  QString query, cadena;
  double debe, haber, total_factura=0;
  idborrador = idapunte1;
  //Busca entradas de IVA en la tabla registroiva
  query.sprintf ( "SELECT * FROM registroiva, cuenta WHERE idborrador=%d AND registroiva.contrapartida=cuenta.idcuenta",idborrador);
  conexionbase->begin();
  cursor2 *cursoriva = conexionbase->cargacursor(query,"cursoriva");
  conexionbase->commit();
  if (!cursoriva->eof()) {
    // Se trata de un registro que ya ha sido introducido
    idregistroiva=atoi(cursoriva->valor("idregistroiva").ascii());
    contrapartida->setText(cursoriva->valor("codigo"));
    empfactura->setText(cursoriva->valor("nombreent_cuenta"));
    baseimponible->setText(cursoriva->valor("baseimp"));
    iva->setText(cursoriva->valor("iva"));
    factura->setText(cursoriva->valor("factura"));
    numorden->setText(cursoriva->valor("numorden"));
    cif->setText(cursoriva->valor("cif"));
    query.sprintf ("SELECT * from borrador, asiento, cuenta WHERE borrador.idborrador=%d AND borrador.idasiento=asiento.idasiento AND borrador.idcuenta=cuenta.idcuenta",idborrador);
    conexionbase->begin();
    cursor2 * cursorapunte = conexionbase->cargacursor(query,"cursorapunte");
    conexionbase->commit();
    if (!cursorapunte->eof()) {
      numasiento->setText(cursorapunte->valor("ordenasiento")); 
      cuentaiva->setText(cursorapunte->valor("codigo"));
    }// end if
    if (cuentaiva->text().left(3) == "600") {
      cuentaiva->setText("472");
      iva->setText("0");
      importeiva->setText("0.00");
    }// end if
    if (cuentaiva->text().left(3) == "700") {
      cuentaiva->setText("477");
      iva->setText("0");
      importeiva->setText("0.00");
    }// end if
    delete cursorapunte;
  } else {
      // Aun no se ha metido ningun registro de este estilo, debe inicializarse.
      // Hay una problematica al meterse asientos con multiples entradas de IVA, la cosa es durilla.
      query.sprintf ("SELECT * from borrador, asiento, cuenta WHERE borrador.idborrador=%d AND borrador.idasiento=asiento.idasiento AND borrador.idcuenta=cuenta.idcuenta",idborrador); //Josep Burci�n
      fprintf(stderr,"%s\n",query.ascii());
      conexionbase->begin();
      cursor2 * cursorapunte = conexionbase->cargacursor(query,"cursorapunte");
      conexionbase->commit();
      if (!cursorapunte->eof()) {
        fprintf(stderr,"Asiento %s\n",cursorapunte->valor("idasiento").ascii());
        numasiento->setText(cursorapunte->valor("ordenasiento"));
        cuentaiva->setText(cursorapunte->valor("codigo"));
        fprintf(stderr,"Cuenta usuario %s\n", cursorapunte->valor("conceptocontable").ascii());
        debe = atof(cursorapunte->valor("debe").ascii());
        haber = atof(cursorapunte->valor("haber").ascii());
        
        // La funcion bcontrapartidaborr (definida en la base de datos) busca contrapartidas de forma m�s compleja a como
        // se propuso en un principio. Ahora en un mismo apunte con multiples entradas se encuentra la contrapartida mediante proximidades en el mismo cuadre.
	// Atenci�n a este SELECT que tiene tela,
	// Si se hace un SELECT * FROM ..... WHERE borrador.idborrador = bcontrapartidaborr(%d) el rendimiento cae en picado.
        query.sprintf("SELECT * FROM borrador,cuenta WHERE borrador.idcuenta = cuenta.idcuenta AND borrador.idborrador IN  (SELECT bcontrapartidaborr(%d))",idborrador);
        fprintf(stderr,"%s\n",query.ascii());
        conexionbase->begin();
        cursor2 *cursorcontrapartida = conexionbase->cargacursor(query,"contrapartida");
        conexionbase->commit();
        // en el caso en que exista una contrapartida, se podra calcular la contrapartida
        // Y la base imponible.
        if (!cursorcontrapartida->eof()) {
          empfactura->setText(cursorcontrapartida->valor("nombreent_cuenta"));
          contrapartida->setText(cursorcontrapartida->valor("codigo"));
          cif->setText(cursorcontrapartida->valor("cifent_cuenta"));
          total_factura= atof(cursorcontrapartida->valor("debe").ascii());
          if (total_factura == 0) {
            total_factura = atof(cursorcontrapartida->valor("haber").ascii());
          }// end if
          fprintf(stderr,"total factura %2.2f\n",total_factura);
          double iva1 = atof(cursorapunte->valor("debe").ascii());
          if (iva1 == 0) {
            iva1 = atof(cursorapunte->valor("haber").ascii());
          }// end if
          double baseimponible1 = total_factura - iva1;
          double porcentiva = total_factura/baseimponible1 -1;
          cadena.sprintf("%2.2f",baseimponible1);
          baseimponible->setText(cadena);
          cadena.sprintf("%2.0f",porcentiva*100);
          iva->setText(cadena);
          if (cuentaiva->text().left(3) == "600") {
              cuentaiva->setText("472");
              iva->setText("0");
              importeiva->setText("0.00");
              cadena.sprintf("%2.2f",total_factura);
              baseimponible->setText(cadena);
          }
          if (cuentaiva->text().left(3) == "700") {
              cuentaiva->setText("477");
              iva->setText("0");
              importeiva->setText("0.00");
              cadena.sprintf("%2.2f",total_factura);
              baseimponible->setText(cadena);
          }
          
//* JOSEP BURCION
          //Proponemos un n�mero de factura si se trata de una venta y
          //proponemos un n�mero de orden si se trata de una compra
          //Realmente esto se tendria que implementar con contadores en
          //la base de datos.
          cursor2 * recordset;
          //Primero comprovamos si esta factura ya tiene un apunte de iva distinto y cojemos el mismo numero de factura
          query.sprintf("SELECT factura, numorden FROM registroiva WHERE idborrador IN (SELECT idborrador FROM borrador WHERE idasiento=(SELECT idasiento FROM borrador WHERE idborrador='%i'))",idborrador);
          conexionbase->begin();
          recordset = conexionbase->cargacursor(query,"recordset");
          conexionbase->commit();
          if (!recordset->eof()) {
              if (! numorden->isEnabled()) {  //Se trata de una Venta
                  factura->setText(recordset->valor("factura"));
              } else { //Se trata de una compra
                  factura->setText(recordset->valor("factura"));
                  numorden->setText(recordset->valor("numorden"));
              }
              
          }  else {   //La factura no existe, entonces proponemos el siguiente n�mero de factura
              if (! numorden->isEnabled()) {  //Se trata de una Venta
                  //query.sprintf( "SELECT factura FROM registroiva WHERE idborrador=(SELECT MAX(idborrador) FROM registroiva WHERE numorden='-1')");
                  query.sprintf("SELECT MAX(to_number(factura,'99999')) AS factura FROM registroiva WHERE numorden=''");
                  conexionbase->begin();
                  recordset = conexionbase->cargacursor(query,"recordset");
                  conexionbase->commit();
                  if (!recordset->eof()) {
                      int numfact = 1 + atoi(recordset->valor("factura").ascii());
                      cadena.sprintf("%i",numfact);
                      factura->setText(cadena);
                  }
              }
              else { //Se trata de una compra
                  //query.sprintf( "SELECT numorden FROM registroiva WHERE idborrador=(SELECT MAX(idborrador) FROM registroiva WHERE numorden<>'-1')");
                  query.sprintf("SELECT MAX(to_number(numorden,'99999')) AS numorden FROM registroiva WHERE numorden<>''");
                  conexionbase->begin();
                  recordset = conexionbase->cargacursor(query,"recordset");
                  conexionbase->commit();
                  if (!recordset->eof()) {
                      int numord = 1 + atoi(recordset->valor("numorden").ascii());
                      cadena.sprintf("%i",numord);
                      numorden->setText(cadena);
                 }// end if
              }// end if
          }// end if
          delete recordset;//Fin proposicion numeros factura y orden.
//* END JOSEP BURCION 
        }// end if
        delete cursorcontrapartida;
      }// end if
      delete cursorapunte;
  }// end if
  delete cursoriva;
  
  
  // Cargamos las formas de pago correspondientes y las ponemos como toca.
  QString SQLQuery;
  SQLQuery.sprintf("SELECT * FROM prevcobro LEFT JOIN cuenta ON prevcobro.idcuenta = cuenta.idcuenta WHERE idregistroiva=%d", idregistroiva);
  conexionbase->begin();
  cursor2 *curprevcobros = conexionbase->cargacursor(SQLQuery, "queryprevcobros");
  conexionbase->commit();
  int i =0;
  while ( ! curprevcobros->eof()) {  
      m_listPrevision->setText(i, COL_PREV_IDPREVCOBRO,curprevcobros->valor("idprevcobro"));
      m_listPrevision->setText(i, COL_PREV_FPREVISTAPREVCOBRO,curprevcobros->valor("fprevistaprevcobro"));
      m_listPrevision->setText(i, COL_PREV_FCOBROPREVCOBRO,curprevcobros->valor("fcobroprevcobro"));
      m_listPrevision->setText(i, COL_PREV_IDCUENTA,curprevcobros->valor("idcuenta"));
      m_listPrevision->setText(i, COL_PREV_CODCUENTA,curprevcobros->valor("codigo"));
      m_listPrevision->setText(i, COL_PREV_NOMCUENTA,curprevcobros->valor("descripcion"));
      m_listPrevision->setText(i, COL_PREV_CANTIDADPREVISTAPREVCOBRO,curprevcobros->valor("cantidadprevistaprevcobro"));
      m_listPrevision->setText(i, COL_PREV_CANTIDADPREVCOBRO,curprevcobros->valor("cantidadprevcobro"));
      m_listPrevision->setText(i, COL_PREV_FPAGOPREVCOBRO,curprevcobros->valor("fpagoprevcobro"));
      m_listPrevision->setText(i, COL_PREV_IDREGISTROIVA,curprevcobros->valor("idregistroiva"));
      i++;
      curprevcobros->siguienteregistro(); 
  }// end while
  delete curprevcobros;
  
}// end inicializa1


//El usuario cambia el % de IVA
//Entonces recalculamos el importe de IVA
void ivaview::iva_changed() {
    importeiva->setText(QString::number(((baseimponible->text().toFloat()) *(iva->text().toFloat()))/100,'f',2));
}

//El usuario cambia el importe del IVA
//Entonces recalculamos la base
//Controlamos que IVA no tenga valor ZERO para evitar "DIVISION POR ZERO!!"
void ivaview::importeiva_changed() {
    if (iva->text().toFloat() != 0) {
        baseimponible->setText(QString::number((importeiva->text().toFloat() * 100) / iva->text().toFloat(),'f',2));
    }
}

//El usuario cambia la base del IVA
//Entonces recalculamos el importe
void ivaview::baseiva_lostfocus() {
    iva_changed(); //Hacemos lo mismo que en iva_changed();
}

//Cambia ligeramente el aspecto del dialogo en funcion de si visualizamos un IVA soportado o repercutido.
// 1 = Soportado (Compra)
// 2 = Repercutido (Venta)
void ivaview::setSoportadoRepercutido(int sop_rep=0) {
  if (sop_rep==1) setCaption("Iva soportado (COMPRAS)"); 
  else if (sop_rep==2) setCaption("Iva repercutido (VENTAS)");
  else setCaption("Iva soportado/repercutido");
  numorden->setEnabled(sop_rep==1);
}

void ivaview::guardaprevpago() {
   int i;
   i=0;
   while (m_listPrevision->text(i,COL_PREV_FPREVISTAPREVCOBRO) != "") {
      guardaprevpago(i++);
   }// end while
}// end guardaprevpago

/**
  * \brief Esta funci�n guarda una linea de la tabla de previsiones
  *
  * La linea se guarda en la base de datos prevcobro, la tabla afectada es m_listPrevision
  * El parametro que se pasa es la columna de la tabla que queremos guardar en la base de datos.
  */
void ivaview::guardaprevpago (int numrow) {
   QString idprevpago = m_listPrevision->text(numrow, COL_PREV_IDPREVCOBRO);
   if (idprevpago != "") {       // Ya un elemento, debemos hacer un UPDATE
      QString SQLQuery = "UPDATE prevcobro SET ";
      SQLQuery += " idcuenta="+m_listPrevision->text(numrow, COL_PREV_IDCUENTA)+" ";
      SQLQuery += " , fprevistaprevcobro = '"+m_listPrevision->text(numrow, COL_PREV_FPREVISTAPREVCOBRO)+"' ";
      SQLQuery += " , fcobroprevcobro = '"+m_listPrevision->text(numrow, COL_PREV_FCOBROPREVCOBRO)+"' ";
      SQLQuery += " , cantidadprevistaprevcobro = "+m_listPrevision->text(numrow, COL_PREV_CANTIDADPREVISTAPREVCOBRO)+" ";
      SQLQuery += " , cantidadprevcobro = "+m_listPrevision->text(numrow, COL_PREV_CANTIDADPREVCOBRO)+" ";
      SQLQuery += " WHERE idprevcobro="+idprevpago;
      fprintf (stderr,"%s\n", SQLQuery.ascii());
      conexionbase->begin();
      conexionbase->ejecuta(SQLQuery);
      conexionbase->commit();
   } else {                      // Hay que hacer un INSERT
   }// end if
}// end guardaprevpago

/**
  * \brief SLOT que captura el cambio de foco en la rejilla de Previsiones de cobros y pagos.
  * Detecta si se ha cambiado de fila y si es el caso se guarda el registro.
  * EN DESUSO
  */
void ivaview::tcambiaseleccion() {
   static int rowactual=0;
   fprintf(stderr,"tcambiaseleccion\n");
   if (rowactual != m_listPrevision->currentRow())
       guardaprevpago(rowactual);
   rowactual = m_listPrevision->currentRow();
}// end ivaview

/**
  * \brief SLOT que captura el cambio de foco en la rejilla de Previsiones de cobros y pagos.
  * Detecta si se ha cambiado de fila y si es el caso se guarda el registro.
  */
void ivaview::cambiadogrid(int row, int col) {
   QString fecha;
   switch (col) {
      case COL_PREV_FPREVISTAPREVCOBRO:
         fecha = normalizafecha(m_listPrevision->text(row,col)).toString("dd/MM/yyyy");
         m_listPrevision->setText(row, col, fecha);
      break;
      case COL_PREV_FCOBROPREVCOBRO:
         fecha = normalizafecha(m_listPrevision->text(row,col)).toString("dd/MM/yyyy");
         m_listPrevision->setText(row, col, fecha);
      break;
      case COL_PREV_CODCUENTA:
         cambiadasubcuenta(row);
      break;
   }// end switch
}// end apuntecambiadogrid

/**
  * \brief SLOT que captura la pulsaci�n de determinadas telcas especiales para la aplicacion
  * Se emite cuando se han pulsado determinadas teclas en la edici�n de la tabla de cobros/pagos.
  * Responde a la pulsaci�n del '+' y del '*'
  */
void ivaview::pulsadomas(int row, int col, int caracter) { 
   QString query, fecha;
   QPopupMenu *menucoste = new QPopupMenu( this );
   calendario *cal;
   int dia, mes, ano;
   QList<QDate> a;
   QString cadena;   
   int opcion;
   switch (caracter) {
      case '+':
         switch(col) {
            case COL_PREV_FPREVISTAPREVCOBRO:
               cal = new calendario(0,0);
               cal->exec();
               a = cal->dn->selectedDates();
               dia = a.first()->day();
               mes = a.first()->month();
               ano = a.first()->year();
               cadena.sprintf( "%2.2d/%2.2d/%d",dia, mes, ano);
               fprintf(stderr,"Se ha pulsado:%s\n", cadena.ascii());
               m_listPrevision->setText(row, COL_PREV_FPREVISTAPREVCOBRO, cadena);
               delete cal;
            break;
            case COL_PREV_FCOBROPREVCOBRO:
               cal = new calendario(0,0);
               cal->exec();
               a = cal->dn->selectedDates();
               dia = a.first()->day();
               mes = a.first()->month();
               ano = a.first()->year();
               cadena.sprintf( "%2.2d/%2.2d/%d",dia, mes, ano);
               fprintf(stderr,"Se ha pulsado:%s\n", cadena.ascii());
               m_listPrevision->setText(row, COL_PREV_FCOBROPREVCOBRO, cadena);
               delete cal;
            break;
            
            case COL_PREV_FPAGOPREVCOBRO:
               /** Montamos el menu que se va a presentar */
               menucoste->insertItem(tr("Ninguno"), 1000);
               opcion = menucoste->exec();
               delete menucoste;
                if (opcion == 1000) {
                }// end if
            break;
         }// end switch
      case '*':
         fprintf(stderr,"Se ha pulsado el *\n");
         //duplicar(col);
      break;
      case 4100:
         fprintf(stderr,"Se ha pulsado el enter\n");
         switch (col) {
            case COL_PREV_FPREVISTAPREVCOBRO:
               fecha = normalizafecha(m_listPrevision->text(row,col)).toString("dd/MM/yyyy");
               m_listPrevision->setText(row, col, fecha);
               m_listPrevision->setCurrentCell(row, COL_PREV_FCOBROPREVCOBRO);
            break;
            case COL_PREV_FCOBROPREVCOBRO:
               fecha = normalizafecha(m_listPrevision->text(row,col)).toString("dd/MM/yyyy");
               m_listPrevision->setText(row, col, fecha);
               m_listPrevision->setCurrentCell(row, COL_PREV_CODCUENTA);
            break;
            case COL_PREV_CODCUENTA:
               cambiadasubcuenta(row);
               m_listPrevision->setCurrentCell(row, COL_PREV_CANTIDADPREVISTAPREVCOBRO);
            break;          
            default:
               m_listPrevision->setCurrentCell(row, col+1);
            break;
         }// end switch
      break;
   }// end switch
}// end pulsadomas

/**
  * \brief Esta funci�n se llama cada vez que se sospecha que se ha cambiado la cuenta.
  *
  * Comprueba cual es la cadena que se ha metido como cuenta y le hace todos los tratamientos posibles
  * hasta determinar si es una cuenta del plan contable o no.
  */
void ivaview::cambiadasubcuenta(int row) {
    int numdigitos = empresaactual->numdigitosempresa();
    QString subcuenta = m_listPrevision->text(row,COL_PREV_CODCUENTA);
    if (subcuenta == "") {
        // Hacemos aparecer la ventana de cuentas
        listcuentasview1 *listcuentas = new listcuentasview1();
        listcuentas->modo=1;
        listcuentas->inicializa(conexionbase);
        listcuentas->exec();
        m_listPrevision->setText(row,COL_PREV_CODCUENTA,listcuentas->codcuenta);
        m_listPrevision->setText(row,COL_PREV_IDCUENTA,listcuentas->idcuenta);
        delete listcuentas;
    }// end if
    // Hacemos la recodificacin
    QString cad = m_listPrevision->text(row,COL_PREV_CODCUENTA);
    if (cad != "") {
        cad = extiendecodigo(cad,numdigitos);
        conexionbase->begin();
        cursor2 *cursorcta = conexionbase->cargacuenta(0, cad );
        conexionbase->commit();
        int num = cursorcta->numregistros();
        if (num >0) {
            m_listPrevision->setText(row,COL_PREV_CODCUENTA,cursorcta->valor("codigo"));
            m_listPrevision->setText(row,COL_PREV_NOMCUENTA,cursorcta->valor("descripcion"));
            m_listPrevision->setText(row,COL_PREV_IDCUENTA,cursorcta->valor("idcuenta"));
        } else {
            QMessageBox::warning( 0, tr("No existe cuenta"), tr("No existe una cuenta con el codigo proporcionado, desea crear una?."), QMessageBox::Yes, QMessageBox::No);
        }// end if
        delete cursorcta;
    }// end if
}// end cambiadasubcuenta
