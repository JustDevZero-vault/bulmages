/***************************************************************************
 *   Copyright (C) 2009 by Aron Galdon                               *
 *   auryn@wanadoo.es                                               *
 *   http://www.iglues.org                                                 *
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

#include "blargparser.h"

///
/**
\param argc
\param argv
**/
BlArgParser::BlArgParser( int argc, char **argv )
   : m_dbName( "" )
   , m_host( "" )
   , m_port( "" )
   , m_userName( "" )
   , m_showVersion( false )
   , m_askPassword( false )
   , m_showHelp( false )
{
   _depura ( "BlArgParser::BlArgParser", 0 );
   QString argument = "";
   char type_arg = NULL;

   for(int i = 1; i < argc; i++) {
      argument = argv[i];

      /// Usar como un valor si la iteraci&oacute;n anterior ha activado un type_arg
      switch(type_arg) {
         case 'd':
            m_dbName = argument;
         break;
         case 'h':
            m_host = argument;
         break;
         case 'p':
            m_port = argument;
         break;
         case 'U':
            m_userName = argument;
         break;
      }

      /// Cuando se asigna una letra a la variable type_arg,
      /// se espera tomar un valor en la pr&oacute;xima iteraci&oacute;n.
      type_arg = NULL;

      if (argument == "--dbname" || argument == "-d") {
         type_arg = 'd';
      }
      else if (argument == "--host" || argument == "-h") {
         type_arg = 'h';
      }
      else if (argument == "--port" || argument == "-p") {
         type_arg = 'p';
      }
      else if (argument == "--username" || argument == "-U") {
         type_arg = 'U';
      }
      else if (argument == "--version" || argument == "-V") {
         m_showVersion = true;
      }
      else if (argument == "--password" || argument == "-W") {
         m_askPassword = true;
      }
      else if (argument == "--help" || argument == "-?") {
         m_showHelp = true;
      } // end if
   } // end for

   _depura ( "END BlArgParser::BlArgParser", 0 );
}
