/***************************************************************************
                          regivaprintview.cpp  -  description
                             -------------------
    begin                : dom jul 13 2003
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

#include "regivaprintview.h"
#include "listivaview.h"
#include "empresa.h"

#include "modelo300.h"
#ifndef WIN32
#include <unistd.h>
#endif
#include <fstream>
using namespace std;

extern Mod300ps *modelo;

regivaprintview::regivaprintview(empresa *emp, QWidget *parent, const char *name ) : regivaprintdlg(parent,name) {
    fichero = NULL;
    empresaactual=emp;
    conexionbase=emp->bdempresa();
}


regivaprintview::~regivaprintview() {
}

/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void regivaprintview::accept() {
    if (radiotexto->isChecked())
        presentar("txt");
    if (radiohtml->isChecked())
        presentar("html");
    if (radioPS->isChecked()) {
        //Lanza dialogo de generacion de modelo 300
        //Deberia revisarse el Widget parent de la variable modelo (listivaview--->modelo, en vez de regivaprintview--->modelo)
        modelo->exec();
    }// end if
}// end accept



void regivaprintview::inicializa1(QString finicial1, QString ffinal1) {
    fechainicial1->setText(finicial1);
    fechafinal1->setText(ffinal1);
}// end if


/**
 * Se ha pulsado sobre el boton aceptar del formulario
 */
void regivaprintview::presentar(char *tipus) {
#ifndef WIN32

    int txt, html, txtapren, htmlapren;
    int error;
    int pid;
    QString data;
    QString datahora;

    QString query;

    // tipus de presentaci
    txt=!strcmp(tipus,"txt");
    html=!strcmp(tipus,"html");
    txtapren=!strcmp(tipus,"txtapren");
    htmlapren=!strcmp(tipus,"htmlapren");

    // Cogemos los valores del formulario.
    QString finicial = fechainicial1->text();
    QString ffinal = fechafinal1->text();

    if (txt | html) {

        char *argstxt[]={"iva.txt","iva.txt",NULL};      //presentaci� txt normal
        char *argshtml[]={"iva.html","iva.html",NULL};   //presentaci� html normal

        ofstream fitxersortidatxt(argstxt[0]);     // creem els fitxers de sordida
        ofstream fitxersortidahtml(argshtml[0]);

        if (!fitxersortidatxt)
            txt=0;    // verifiquem que s'hagin creat correctament els fitxers
        if (!fitxersortidahtml)
            html=0;  // es pot millorar el tractament d'errors
        if (txt | html) {                // nom�s continuem si hem pogut crear algun fitxer
            int num1;
            cursor2 *cursorapt;
            conexionbase->begin();
            query.sprintf("SELECT * FROM registroiva, cuenta, borrador, asiento  where cuenta.idcuenta=borrador.idcuenta AND borrador.idborrador=registroiva.idborrador AND asiento.idasiento=borrador.idasiento AND (cuenta.codigo LIKE '43%%' OR cuenta.codigo LIKE '600%%') AND borrador.fecha>='%s' AND borrador.fecha<='%s'",fechainicial1->text().ascii(), fechafinal1->text().ascii());

            fprintf(stderr,"%s\n",query.ascii());
            cursorapt = conexionbase->cargacursor(query,"mycursor");
            conexionbase->commit();
            // Calculamos cuantos registros van a crearse y dimensionamos la tabla.
            num1 = cursorapt->numregistros();
            int hoja=0;

            if (txt) {
                //presentaci� txt normal
                fitxersortidatxt.setf(ios::fixed);
                fitxersortidatxt.precision(2);
                fitxersortidatxt << "                                        IVA Repercutit \n" ;
                fitxersortidatxt << "Data Inicial: " << finicial.ascii() << 
		"   Data Final: " << ffinal.ascii() << endl;
                fitxersortidatxt << "Assentament  Data   Compte  Descripci�   Base Imponible    Factura \n" ;
                fitxersortidatxt << "__________________________________________________________________________________________________________\n";
            }// end if
            if (html) {
                //presentaci� html normal
                fitxersortidahtml.setf(ios::fixed);
                fitxersortidahtml.precision(2);
                fitxersortidahtml << "<html>\n";
                fitxersortidahtml << "<head>\n";
                fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
                fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
                fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
                fitxersortidahtml << "  <title> IVA Repercutit </title>\n";
                fitxersortidahtml << "</head>\n";
                fitxersortidahtml << "<body>\n";
                fitxersortidahtml << "<table><tr><td colspan=\"10\" class=titoliva> IVA Repercutit <hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td colspan=\"10\" class periodeiva> Data Inicial: " << finicial.ascii() << " -  Data Final: " << ffinal.ascii() << "<hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td class=titolcolumnaiva> Assentament </td><td class=titolcolumnaiva> Data </td><td class=titolcolumnaiva> Compte </td><td class=titolcolumnaiva> Descripci� </td><td class=titolcolumnaiva> Base Imponible </td><td class=titolcolumnaiva> % IVA </td><td class=titolcolumnaiva> Quota IVA </td><td class=titolcolumnaiva> Factura </td><td class=titolcolumnaiva> Cif </td><td class=titolcolumnaiva> Compte IVA </td></tr>\n";
            }// end if
            while (!cursorapt->eof()) {
                datahora=cursorapt->valor("fecha");
                data=datahora.mid(0,10);

		int baseimp = cursorapt->valor("baseimp").replace(".","").toInt();
		int total = baseimp * cursorapt->valor("baseimp").replace(".","").toInt() / 10000;
		QString numberstr = QString::number(total);
    		numberstr = numberstr.left(numberstr.length()-2)+"."+numberstr.right(2);
                if (txt) {
                    //presentaci� txt normal
                    fitxersortidatxt << setw(10) << cursorapt->valor("idasiento") << " " <<
		     data.ascii() << " " << 
		     cursorapt->valor("contrapartida").ascii() << " " << cursorapt->valor("descripcion").ascii() << " " << cursorapt->valor("baseimp").ascii() << " " << 
		     cursorapt->valor("iva").ascii() << " " << 
		     numberstr.ascii() <<" " << 
		     cursorapt->valor("factura").ascii() << " " << 
		     cursorapt->valor("cif").ascii() << " " <<  
		     endl;
                }// end if
                if (html) {
                    //presentaci� html normal
                    fitxersortidahtml << "<tr><td class=assentamentiva>" << cursorapt->valor("idasiento").ascii() << "</td><td class=dataiva>" << data.ascii() << "</td><td class=contrapartidaiva>" << cursorapt->valor("contrapartida").ascii() << "</td><td class=descripcioiva>" << cursorapt->valor("descripcion").ascii() << "</td><td class=baseimponibleiva>" << cursorapt->valor("baseimp").ascii() << "</td><td class=tipusiva>" << cursorapt->valor("iva").ascii() << "</td><td class=quotaiva>" << numberstr.ascii() << "</td><td class=facturaiva>" << cursorapt->valor("factura").ascii() << "</td><td class=cifiva>" << cursorapt->valor("cif").ascii() << "</td></tr> \n";
                }// end if
                // Calculamos la siguiente cuenta registro y finalizamos el bucle
                cursorapt->siguienteregistro();
            }// end while
            // Vaciamos el cursor de la base de datos.
            delete cursorapt;


            conexionbase->begin();
            query.sprintf("SELECT * FROM registroiva, cuenta, borrador, asiento  WHERE cuenta.idcuenta=borrador.idcuenta AND borrador.idborrador=registroiva.idborrador AND asiento.idasiento=borrador.idasiento AND (cuenta.codigo NOT LIKE '43%%' AND cuenta.codigo NOT LIKE '600%%') AND borrador.fecha>='%s' AND borrador.fecha<='%s'ORDER BY borrador.fecha",fechainicial1->text().ascii(), fechafinal1->text().ascii());

            fprintf(stderr,"%s\n",query.ascii());
            cursorapt = conexionbase->cargacursor(query,"mycursor");
            conexionbase->commit();
            // Calculamos cuantos registros van a crearse y dimensionamos la tabla.
            num1 = cursorapt->numregistros();
            hoja=0;
            if (txt) {
                //presentaci� txt normal

                fitxersortidatxt.setf(ios::fixed);
                fitxersortidatxt.precision(2);
                fitxersortidatxt << "                                        IVA Soportat \n" ;
                fitxersortidatxt << "Assentament  Data   Compte  Descripci�   Base Imponible  % IVA  Quota IVA   Factura  Cif Compte IVA \n" ;
                fitxersortidatxt << "___________________________________________________________________________________________________________\n";
            }
            if (html) {
                //presentaci� html normal
                fitxersortidahtml.setf(ios::fixed);
                fitxersortidahtml.precision(2);
                fitxersortidahtml << "<html>\n";
                fitxersortidahtml << "<head>\n";
                fitxersortidahtml << "  <!DOCTYPE / public \"-//w3c//dtd xhtml 1.0 transitional//en\"\n";
                fitxersortidahtml << "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
                fitxersortidahtml << "  <LINK REL=StyleSheet HREF=\"estils.css\" TYPE=\"text/css\" MEDIA=screen>\n";
                fitxersortidahtml << "  <title> IVA Soportat </title>\n";
                fitxersortidahtml << "</head>\n";
                fitxersortidahtml << "<body>\n";
                fitxersortidahtml << "<tr><td colspan=\"10\" class=titoliva> IVA Soportat <hr></td></tr>\n\n";
                fitxersortidahtml << "<tr><td class=titolcolumnaiva> Assentament </td><td class=titolcolumnaiva> Data </td><td class=titolcolumnaiva> Compte </td><td class=titolcolumnaiva> Descripci� </td><td class=titolcolumnaiva> Base Imponible </td><td class=titolcolumnaiva> % IVA </td><td class=titolcolumnaiva> Quota IVA </td><td class=titolcolumnaiva> Factura </td><td class=titolcolumnaiva> Cif </td><td class=titolcolumnaiva> Compte IVA </td></tr>\n";
            }
            while (!cursorapt->eof()) {

                datahora=cursorapt->valor("fecha");
                data=datahora.mid(0,10);
		// Hacemos los c�lculos con punto fijo, para evitar el error del punto flotante.
		int baseimp = cursorapt->valor("baseimp").replace(".","").toInt();
		int total = baseimp * cursorapt->valor("baseimp").replace(".","").toInt() / 10000;
		QString numberstr = QString::number(total);
    		numberstr = numberstr.left(numberstr.length()-2)+"."+numberstr.right(2);
                // Acumulamos los totales para al final poder escribirlos
                if (txt) {
                    //presentaci� txt normal
                    fitxersortidatxt << setw(10) << cursorapt->valor("idasiento").ascii() << " " 
		    << data.ascii() << " "
		     << cursorapt->valor("contrapartida").ascii() << " " << cursorapt->valor("descripcion").ascii() << " " << cursorapt->valor("baseimp").ascii() << " "  <<
		      cursorapt->valor("iva").ascii() << " " <<
		      numberstr.ascii() << "-" << 
		      cursorapt->valor("factura").ascii() << " " << 
		      cursorapt->valor("cif").ascii() << " "  << 
		       endl;
                }
                if (html) {
                    //presentaci� html normal
                    fitxersortidahtml << "<tr><td class=assentamentiva>" << cursorapt->valor("idasiento").ascii() << "</td><td class=dataiva>" << data.ascii() << "</td><td class=contrapartidaiva>" << cursorapt->valor("contrapartida").ascii() << "</td><td class=descripcioiva>" << cursorapt->valor("descripcion").ascii() << "</td><td class=baseimponibleiva>" << cursorapt->valor("baseimp").ascii() << "</td><td class=tipusiva>" << cursorapt->valor("iva").ascii() << "</td><td class=quotaiva>" << numberstr.ascii() << "</td><td class=facturaiva>" << cursorapt->valor("factura").ascii() << "</td><td class=cifiva>" << cursorapt->valor("cif").ascii() << "</td></tr> \n";
                }
                // Calculamos la siguiente cuenta registro y finalizamos el bucle
                cursorapt->siguienteregistro();
            }// end while
            // Vaciamos el cursor de la base de datos.
            delete cursorapt;


/// AHORA PONEMOS EL RESUMEN	    
	    
	long int tivas=0;
	long int tivar=0;
	long int tbaseimps =0;
	long int tbaseimpr =0;
	    
    QString SQLQuery = "SELECT * FROM cuenta, tipoiva LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva FROM iva  WHERE iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='"+fechainicial1->text()+"' AND ffactura <='"+fechafinal1->text()+"' ) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta AND cuenta.codigo LIKE '472%'";
    conexionbase->begin();
    cursor2 *cur = conexionbase->cargacursor(SQLQuery, "elcursor");
    conexionbase->commit();
//    m_listSoportado->setNumRows(cur->numregistros());
    int j =0;
    while (! cur->eof() ) {
       long int baseiva = cur->valor("tbaseiva").replace(".","").toInt();
       long int porcent = cur->valor("porcentajetipoiva").replace(".","").toInt();
       long int baseimp = baseiva *10000 / porcent;
       QString numberstr = QString::number(baseimp);
       numberstr = numberstr.left(numberstr.length()-2)+"."+numberstr.right(2);

       fitxersortidatxt << cur->valor("nombretipoiva").ascii();
       fitxersortidatxt << cur->valor("tbaseiva").ascii();
       fitxersortidatxt << numberstr.ascii();

       tivas+= baseiva;
       tbaseimps+=baseimp;
       cur->siguienteregistro();
       j++;
    }// end while
    delete cur;
    
    SQLQuery = "SELECT * FROM cuenta, tipoiva  LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva FROM iva iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='"+fechainicial1->text()+"' AND ffactura <='"+fechafinal1->text()+"' ) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta AND cuenta.codigo LIKE '477%'";
    conexionbase->begin();
    cur = conexionbase->cargacursor(SQLQuery, "elcursor");
    conexionbase->commit();
//    m_listRepercutido->setNumRows(cur->numregistros());
    j =0;
    while (! cur->eof() ) {
       long int baseiva = cur->valor("tbaseiva").replace(".","").toInt();
       long int porcent = cur->valor("porcentajetipoiva").replace(".","").toInt();
       long int baseimp = baseiva*10000 / porcent;
       QString numberstr = QString::number(baseimp);
       numberstr = numberstr.left(numberstr.length()-2)+"."+numberstr.right(2); 
       fitxersortidatxt << cur->valor("nombretipoiva").ascii();
       fitxersortidatxt << cur->valor("tbaseiva").ascii();
       fitxersortidatxt << numberstr.ascii();  
       tivar+= baseiva;
       tbaseimpr+=baseimp;           
       cur->siguienteregistro();
       j++;
    }// end while    
    delete cur;

  
	    
	    
/// AHORA CERRAMOS LOS FICHEROS
            if (txt) {
                fitxersortidatxt.close();
                //presentaci� txt normal
                if ((pid=fork()) < 0) {
                    perror ("Fork failed");
                    exit(errno);
                }// end if
                if (!pid) {
                    error = execvp(confpr->valor(CONF_EDITOR).ascii(),argstxt);
                }// end if
            }// end if
            if (html) {
                fitxersortidahtml << "\n</table>\n</body>\n</html>\n";
                fitxersortidahtml.close();
                //presentaci� html normal
                if ((pid=fork()) < 0) {
                    perror ("Fork failed");
                    exit(errno);
                }// end if
                if (!pid) {
                    error = execvp(confpr->valor(CONF_NAVEGADOR).ascii(),argshtml);
                }// end if
            }// end if
        }
    }
#endif
}



