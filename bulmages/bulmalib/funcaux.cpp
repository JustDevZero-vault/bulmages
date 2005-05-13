/***************************************************************************
                          funcaux.cpp  -  description
                             -------------------
    begin                : Sun Jan 26 2003
    copyright            : (C) 2003 by Josep Burcion and Tomeu Borr�s  
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
/** \file funcaux.cpp
  * Fichero de implementaci�n de funciones auxiliares que no es preciso encapsular<BR>
  * <P>En este fichero se implementan todas aquellas funciones que por su brevedad o aislamiento son
  * utilizadas de forma regular en el programa. Definidas en \ref funcaux.h </P>
  * <P>Dichas funciones normalmente son de uso general, por lo que es normal ver este archivo incluido
  * en la pr�ctica totalidad de los dem�s ficheros</P>
  */

#include "funcaux.h"
#include <qstring.h>

/** Proteje cadenas de texto pasandoles una sustituci�n de codigos especiales de XML
  * \param string cadena que se va a proteger.
  * \return cadena en formato protegido.
  */
QString XMLProtect( const QString& string ) {
    QString s = string;
    s.replace( "&", "&amp;" );
    s.replace( ">", "&gt;" );
    s.replace( "<", "&lt;" );
    s.replace( "\"", "&quot;" );
    s.replace( "\'", "&apos;" );
    return s;
}

/** Extiende un string a un numero de cuenta sustituyendo los '.' por ceros.
  * \param cad Cadena inicial
  * \param num1 Numero de d�gitos totales de la cuenta.
  * \return Devuelve un string con el c�digo de cuenta extendido al n�mero de d�gitos indicado.
  */
string extiendecodigo (string cad, unsigned int num1) {
   string cod=cad;
   unsigned int num=num1;
   if (cod.length() < num) {
     string str7 (num-cod.length()+1,'0');
     int pos = cod.find(".",0);
     if (pos > 0) {
       cod.replace(pos,1,str7);
     }// end if
   }// end if
   return(cod);
}

/** Extiende un string a un numero de cuenta sustituyendo los '.' por ceros.
  * \param cad Cadena inicial
  * \param num1 Numero de d�gitos totales de la cuenta.
  * \return Devuelve un QString con la cuenta extendida al n�mero de d�gitos indicado.
  */
QString extiendecodigo (QString cad, unsigned int num1) {
   QString cod=cad;
   unsigned int num=num1;
   if (cod.length() < num) {
     string str7 (num-cod.length()+1,'0');
     int pos = cod.find(".",0);
     if (pos > 0) {
       cod.replace(pos,1,str7.c_str());
     }// end if
   }// end if
   return(cod);
}


/** Redondeo de numeros en punto flotante.
  * \param n Numero a redondear
  * \param d Numero de decimales
  * \return Numero redondeado
  */
float fround(float n, unsigned d) {
  return floor(n*pow(10., d) + .5) / pow (10., d);
}// end fround


/** Esta funci�n convierte un numero con decimales a un entero. Usando la regla
  * que si el el primer decimal es mayor o igual  a 5 se devuelve el entero superior.
  * \param valor Numero a convertir
  */
int roundI(double valor) {
    int retorno;
    double mayor=floor(valor);
    if ((mayor-valor) >= 0.5)  retorno=(int)mayor-1;
    else retorno= (int)mayor;
    return retorno;
}

/** Procesa el string pasado como par�metro y devuelve una estructura del tipo QDate
 * Esta funcion extiende la fecha pasada como parametro 
 * QString y devuelve la fecha en formato QDate.
 * \param fechaintro string con la fecha a ser normalizada.
 */
QDate normalizafecha(QString fechaintro) {
  QDate fecharesult;
  int d, M, y;
  switch(fechaintro.length()) {
    case 4: // fecha tipo ddMM, sin // y sin a�o.
      d = fechaintro.mid(0,2).toInt();
      M = fechaintro.mid(2,2).toInt();
      y = QDate::currentDate().year();
      break;
    case 5:// fecha tipo dd/MM
      d = fechaintro.mid(0,2).toInt();
      M = fechaintro.mid(3,2).toInt();
      y = QDate::currentDate().year();
    break;
    case 6: // fecha tipo ddMMyy
      d = fechaintro.mid(0,2).toInt();
      M = fechaintro.mid(2,2).toInt();
      y = 2000 + fechaintro.mid(4,2).toInt();
     break;
     case 8:
        if(fechaintro.contains("/",TRUE) || fechaintro.contains("-",TRUE)) {
            // fecha tipo  dd/MM/yy o dd-MM-yy
            d = fechaintro.mid(0,2).toInt();
            M = fechaintro.mid(3,2).toInt();
            y = 2000 + fechaintro.mid(6,2).toInt();
        } else {
            // o bien tipo ddMMyyyy
            d = fechaintro.mid(0,2).toInt();
            M = fechaintro.mid(2,2).toInt();
            y = fechaintro.mid(4,4).toInt();
        }// end if
     break;
     case 10: // fecha tipo dd/MM/yyyy
        d = fechaintro.mid(0,2).toInt();
        M = fechaintro.mid(3,2).toInt();
        y = fechaintro.mid(6,4).toInt();
    break;    
    default:
        d = QDate::currentDate().day();
        M = QDate::currentDate().month();
        y = QDate::currentDate().year();
  }// end switch
  if (!fecharesult.setYMD(y,M,d)) fecharesult=QDate::currentDate();
  return(fecharesult);
}// end normalizafecha


/** Esta funci�n ajusta el c�digo pasado al n�mero de digitos especificado.
    Para ello b�sca los ceros intermedios y los amplia hasta que el numero de caracteres sea el deseado.
    Lo hace a partir del quinto d�gito por defecto. Aunque este parametro deber�a ser configurable.
  */
QString ajustacodigo (QString cad, unsigned int num1) {
   QString cod=cad;
   unsigned int longcad = cad.length();
   if (longcad > 4) { 
	if (longcad < num1) {
	string str7 (num1 -longcad,'0');
	cod = cad.left(4);
	cod += str7;
	cod += cad.right(longcad-4);
	}// end if
	if (longcad > num1) {
	cod = cad.left(4);
	cod += cad.right(num1-4);
	}// end if
   }// end if
   return(cod);
}// end ajustacodigo


