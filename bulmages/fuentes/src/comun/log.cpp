/***************************************************************************
 *Copyright (C) 2003 by Fco. Javier MC, Jean Ren� M�rou, V�ctor G. Marim�n *
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
#include "log.h"
#include "configuracion.h"
#include "empresa.h"

#include <qdatetime.h>


bitacora *ctllog;

bitacora::bitacora() {
    // abre archivo Log.
    archivolog = new ofstream(confpr->valor(CONF_ARCHIVO_LOG).c_str(),ios::out |
                              ios::app);
			      
    empresaactual = NULL;
}


bitacora::~bitacora() {
    // Cierra archivo Log.
    archivolog->close();
}

void bitacora::add (int tipoLog, QString qsTxt ) {
     QDateTime hora = QDateTime::currentDateTime();
     QString horastr = hora.toString(Qt::LocalDate);
     QString tipoLogtxt;
     
    /*switch(tipoLog) { //  Lo he comentado porque el tipoLogtxt podr�a ser variado si se considera que el numero puede ser tipo filtro
    	case LOG_SEG:
		tipoLogtxt = "SEG";
	break;
	case LOG_SIS:
		tipoLogtxt = "SIS";
	break;
    }// end tipoLog
    // Escribir en archivo log.
    *archivolog << tipoLog << ", " << tipoLogtxt.ascii() << ", " << horastr.ascii() << ", " empresaactual->nomuserempresa().ascii() << ", " << qsTxt.ascii() << endl;
    */
   if (empresaactual != NULL) {
    *archivolog << tipoLog <<  ", " << horastr.ascii() << ", " << empresaactual->nomuserempresa().ascii() << ", " << qsTxt.ascii() << endl;
   } else {
    *archivolog << tipoLog <<  ", " << horastr.ascii() << ", "  <<  ", " << qsTxt.ascii() << endl;
   }// end if
}
