/***************************************************************************
                          extractoprintview.cpp  -  description
                             -------------------
    begin                : jue jun 26 2003
    copyright            : (C) 2003 by Tomeu Borr�s Riera
    email                : tborras@conetxia.com
    modificat per        : (C) 2003 Antoni Mirabete i Ter�s - amirabet@biada.org
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "extractoprintview.h"
#include "listcuentasview1.h"
#include <unistd.h>

ExtractoPrintView::ExtractoPrintView(QWidget *parent, const char *name ) : ExtractoPrintDlg(parent,name) {
   fichero=NULL;
}

ExtractoPrintView::~ExtractoPrintView(){
}

int ExtractoPrintView::inicializa(postgresiface2 *conn) {
    conexionbase = conn;
    return(0);
}// end inicializa

void ExtractoPrintView::inicializa1(QString fechainicial, QString fechafinal, QString codi, QString codf) {
	fechainicial1->setText(fechainicial);
	fechafinal1->setText(fechafinal);
	codigoinicial->setText(codi);
	codigofinal->setText(codf);
}// end inicializa1

/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void ExtractoPrintView::accept() {

// Versi� per si nom�s permetem escollir una opci

   if (radiotexto->isChecked()) presentar("txt");
   else presentar("html");

}


// Versi� per si decidim que es poden escollir v�ries opcions

/*

   if (radiotexto->isChecked()) presentar("txt");
   if (!(radiotexto->isChecked())) presentar("html");
}

*/


void ExtractoPrintView::presentar(char *tipus) {

   int txt, html;
   int error;
   int pid;
   float debe, haber,saldo;
   float debeinicial=0, haberinicial=0, saldoinicial=0;
   float debefinal, haberfinal, saldofinal;
   int idcuenta;
   int idasiento;
//   char *textasiento;
   int contrapartida;
   int activo;
   string cad;
   cursor2 *cursoraux, *cursoraux1, *cursoraux2;

   QString finicial = fechainicial1->text();
   QString ffinal = fechafinal1->text();
   QString cinicial = codigoinicial->text();
   QString cfinal = codigofinal->text();

// tipus de presentaci�

   txt=!strcmp(tipus,"txt");
   html=!strcmp(tipus,"html");

   char *argstxt[]={"major.txt","major.txt",NULL};      //presentaci� txt normal
   char *argshtml[]={"major.html","major.html",NULL};   //presentaci� html normal

   ofstream fitxersortidatxt(argstxt[0]);     // creem els fitxers de sordida
   ofstream fitxersortidahtml(argshtml[0]);

   if (!fitxersortidatxt) txt=0;    // verifiquem que s'hagin creat correctament els fitxers
   if (!fitxersortidahtml) html=0;  // es pot millorar el tractament d'errors

   if (txt | html){                 // nom�s continuem si hem pogut crear algun fitxer

 

      if (txt) {
         //presentaci� txt

         fitxersortidatxt.setf(ios::fixed);
         fitxersortidatxt.precision(2);

         fitxersortidatxt << "                                    MAJOR \n" ;
         fitxersortidatxt << "Data Inicial: " << finicial.latin1() << "   Data Final: " << ffinal.latin1() << endl;
         fitxersortidatxt << "_________________________________________________________________________________________________________\n";
      }

      if (html) {
         //presentaci� html

         fitxersortidahtml.setf(ios::fixed);
         fitxersortidahtml.precision(2);

         fitxersortidahtml << "<html>\n";
         fitxersortidahtml << "<head>\n";
         fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
         fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
         fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
         fitxersortidahtml << "  <title> Major </title>\n";
         fitxersortidahtml << "</head>\n";
         fitxersortidahtml << "<body>\n";
         fitxersortidahtml << "<table><tr><td colspan=\"6\" class=titolmajor> Major <hr></td></tr>\n\n";
         fitxersortidahtml << "<tr><td colspan=\"6\" class=periodemajor> Data Inicial: " << finicial.latin1() << " -  Data Final: " << ffinal.latin1() << "<hr></td></tr>\n\n";
      }

      conexionbase->begin();
      cursoraux = conexionbase->cargacuentascodigo(-1,(char *) cinicial.latin1(), (char *)cfinal.latin1());

      while(!cursoraux->eof()) {
         idcuenta = atoi(cursoraux->valor(0).latin1());
         cursoraux1 = conexionbase->cargaapuntesctafecha(idcuenta,(char *) finicial.latin1(), (char *)ffinal.latin1());

         if (!cursoraux1->eof()) {
            activo = strcmp((char *) cursoraux->valor(13).latin1() , "f");

            if (txt) {
               fitxersortidatxt << "\n\n" << setw(12) << cursoraux->valor(1).latin1() << setw(50) << cursoraux->valor(2).latin1() << endl;
               if (activo) {
                  fitxersortidatxt << " Compte d'Actiu";
               } else {
                  fitxersortidatxt << " Compte de Passiu";
               }

            }

            if (html) {
               fitxersortidahtml << "<tr><td colspan=\"6\" class=comptemajor>" << cursoraux->valor(1).latin1() << " "<< cursoraux->valor(2).latin1() << "</td></tr>\n";
               if (activo) {
                  fitxersortidahtml << "<tr><td colspan=\"6\" class=tipuscomptemajor> Compte d'Actiu </td></tr>\n";
               } else {
                  fitxersortidahtml << "<tr><td colspan=\"6\" class=tipuscomptemajor> Compte de Passiu </td></tr>\n";
               }

            }

            cursoraux2 = conexionbase->cargasaldoscuentafecha(idcuenta, (char *)finicial.latin1());
            if (!cursoraux2->eof()) {
               debeinicial = atof(cursoraux2->valor(0).latin1());
               haberinicial = atof(cursoraux2->valor(1).latin1());
               if (activo) {
                  saldoinicial = debeinicial - haberinicial;
               } else {
                  saldoinicial = haberinicial-debeinicial;
               }

               if (txt) {
                  //presentaci� txt
                  fitxersortidatxt << "\nAsiento  Fecha   Contrapartida   Descripcion                          Debe         Haber         Saldo\n";
                  fitxersortidatxt << "                                                 SUMAS ANTERIORES...   " << setw(10) << debeinicial << setw(10) << haberinicial << setw(10) << saldoinicial << endl;
                  fitxersortidatxt << "_________________________________________________________________________________________________________\n";
               }

               if (html) {
                  //presentaci� html
                  fitxersortidahtml << "<tr><td class=titolcolumnamajor> Asiento </td><td class=titolcolumnamajor> Fecha </td><td class=titolcolumnamajor> Contrapartida </td><td class=titolcolumnamajor> Descripcion </td><td class=titolcolumnamajor> Debe </td><td class=titolcolumnamajor> Haber </td><td class=titolcolumnamajor> Saldo </td></tr>\n";
                  fitxersortidahtml << "<tr><td></td><td></td><td></td><td class=sumamajor> Sumes anteriors...</td><td class=dosdecimals> " << debeinicial << " </td><td class=dosdecimals> " << haberinicial << " </td><td class=dosdecimals> " << saldoinicial << "</td><td>\n";
               }

               saldo = saldoinicial;
               debefinal = debeinicial;
               haberfinal = haberinicial;

               for(;!cursoraux1->eof();cursoraux1->siguienteregistro()) {
                  idasiento=atoi(cursoraux1->valor(2).latin1());
                  contrapartida = atoi(cursoraux1->valor(10).latin1());
                  debe=atof(cursoraux1->valor(8).latin1());
                  haber=atof(cursoraux1->valor(9).latin1());

                  if (activo) {
                     saldo += debe - haber;
                  } else {
                     saldo += haber -debe;
                  }

               debefinal += debe;
               haberfinal += haber;
               cad = cursoraux1->valor(4).latin1();
               //presentaci� txt
               if (txt) fitxersortidatxt <<  setw(5) << idasiento << setw(14) << cad.substr(1,10).c_str() << setw(10) << contrapartida << "  " << setw(40)  << setiosflags(ios::left) << cursoraux1->valor(5).latin1() << setw(10) << resetiosflags(ios::left) << debe << setw(10) << haber << setw(10) << saldo << endl;
               //presentaci� html
               if (html) fitxersortidahtml << " <tr><td class=assentamentmajor> " << idasiento << " </td><td> " << cad.substr(1,10).c_str() << " </td><td class=contrapartidamajor> " << contrapartida << " </td><td> " << cursoraux1->valor(5).latin1() << " </td><td class=dosdecimals> " << debe << " </td><td class=dosdecimals> " << haber << " </td><td class=dosdecimals> " << saldo << " </td></tr>\n ";
            }

            if (activo) {
               saldofinal = debefinal - haberfinal;
            } else {
               saldofinal = haberfinal -debefinal;
            }
                if (txt) {
                   //presentaci� txt
                   fitxersortidatxt << "                                               __________________________________________________________\n";
                   fitxersortidatxt << "                                                  TOTAL SUBCUENTA...   " << setw(10) << debefinal << setw(10) << haberfinal << setw(10) << saldofinal << endl;
                }
                if (html) {
                   //presentaci� html
                   fitxersortidahtml << "<tr><td></td><td></td><td></td><td class=sumamajor> Total subcompte... </td><td class=dosdecimals>" << debefinal << " </td><td class=dosdecimals> " << haberfinal << " </td><td class=dosdecimals> " << saldofinal << "</td></tr>\n\n";
                }
            cursoraux2->cerrar();
            delete cursoraux2;
         }
      }
         cursoraux1->cerrar();
         delete cursoraux1;
         cursoraux->siguienteregistro();
      }
      if (html) fitxersortidahtml << "\n</table></body></html>\n";     //presentaci� html
      conexionbase->commit();
      delete cursoraux;
   }
    if (txt) {
       //presentaci� txt
       fitxersortidatxt.close();
       if ((pid=fork()) < 0) {
          perror ("Fork failed");
          exit(errno);
       }
       if (!pid) {
          error = execvp(confpr->valor(CONF_EDITOR).c_str(),argstxt);
       }
    }
    if (html) {
       //presentaci� html
       fitxersortidahtml.close();
       if ((pid=fork()) < 0) {
          perror ("Fork failed");
          exit(errno);
       }
       if (!pid) {
          error = execvp(confpr->valor(CONF_NAVEGADOR).c_str(),argshtml);
       }
    }
}



void ExtractoPrintView::boton_codinicial() {
   listcuentasview1 *listcuentas = new listcuentasview1();
   listcuentas->modo=1;
   listcuentas->inicializa(conexionbase);
   listcuentas->exec();
   codigoinicial->setText(listcuentas->codcuenta);
   delete listcuentas;
}// end boton_codinicial

void ExtractoPrintView::boton_codfinal() {
   listcuentasview1 *listcuentas = new listcuentasview1();
   listcuentas->modo=1;
   listcuentas->inicializa(conexionbase);
   listcuentas->exec();
   codigofinal->setText(listcuentas->codcuenta);
   delete listcuentas;
}// end boton_codfinal





