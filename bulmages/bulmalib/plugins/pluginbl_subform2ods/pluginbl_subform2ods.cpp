/***************************************************************************
 *   Copyright (C) 2007 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
 *                                                                         *
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <stdio.h>

#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QLocale>
#include <QProcess>

#include "pluginbl_subform2ods.h"


///
/**
\return
**/
int entryPoint ( QApplication * )
{
    blDebug ( "entryPoint" , 0, "Punto de Entrada del plugin de Subformods" );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbl_subform2ods", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    blDebug ( "END entryPoint", 0, "Punto de Entrada del plugin de Subformods" );
    return 0;
}


///
/**
\param parent
**/
PluginBl_SubForm2ODS::PluginBl_SubForm2ODS ( BlSubForm *parent ) : QObject ( parent )
{
    blDebug ( "PluginBl_SubForm2ODS::PluginBl_SubForm2ODS", 0 );
    blDebug ( "END PluginBl_SubForm2ODS::PluginBl_SubForm2ODS", 0 );
}


///
/**
\param parent
**/
PluginBl_SubForm2ODS::PluginBl_SubForm2ODS ( BlTreeWidget *parent ) : QObject ( parent )
{
    blDebug ( "PluginBl_SubForm2ODS::PluginBl_SubForm2ODS", 0 );
    blDebug ( "END PluginBl_SubForm2ODS::PluginBl_SubForm2ODS", 0 );
}

///
/**
**/
PluginBl_SubForm2ODS::~PluginBl_SubForm2ODS()
{
    blDebug ( "PluginBl_SubForm2ODS::~PluginBl_SubForm2ODS", 0 );
    blDebug ( "END PluginBl_SubForm2ODS::~PluginBl_SubForm2ODS", 0 );
}


///
/**
\param menu
**/
void PluginBl_SubForm2ODS::s_pintaMenu ( QMenu *menu )
{
    blDebug ( "PluginBl_SubForm2ODS::s_pintaMenu", 0 );
    menu->addSeparator();
    menu->addAction ( QIcon( ":/Images/exportods.png" ), _ ( "Exportar a hoja de calculo (ODS)" ) );
    blDebug ( "END PluginBl_SubForm2ODS::s_pintaMenu", 0 );
}

///
/**
\param menu
**/
void PluginBl_SubForm2ODS::s_pintaMenu1 ( QMenu *menu )
{
    blDebug ( "PluginBl_SubForm2ODS::s_pintaMenu1", 0 );
    menu->addSeparator();
    menu->addAction ( QIcon( ":/Images/exportods.png" ), _ ( "Exportar a hoja de calculo (ODS)" ) );
    blDebug ( "END PluginBl_SubForm2ODS::s_pintaMenu1", 0 );
}


///
/**
\param action
**/
void PluginBl_SubForm2ODS::s_trataMenu ( QAction *action )
{
    blDebug ( "PluginBl_SubForm2ODS::s_trataMenu", 0 );
    if ( action->text() == _ ( "Exportar a hoja de calculo (ODS)" ) ) {
        sacaods();
    } // end if
    blDebug ( "END PluginBl_SubForm2ODS::s_trataMenu", 0 );
}


///
/**
\param action
**/
void PluginBl_SubForm2ODS::s_trataMenu1 ( QAction *action )
{
    blDebug ( "PluginBl_SubForm2ODS::s_trataMenu1", 0 );
    if ( action->text() == _ ( "Exportar a hoja de calculo (ODS)" ) ) {
        sacaods1();
    } // end if
    blDebug ( "END PluginBl_SubForm2ODS::s_trataMenu1", 0 );
}


///
/**
**/
void PluginBl_SubForm2ODS::sacaods()
{
    blDebug ( "PluginBl_SubForm2ODS::sacaods", 0 );

    QString archivod = g_confpr->value( CONF_DIR_USER ) + "listadoods.py";
    BlSubForm * subf = ( BlSubForm * ) parent();


    QString fitxersortidatxt = "";

    fitxersortidatxt += "#!/usr/bin/python\n";
    fitxersortidatxt += "# -*- coding: utf8 -*-\n";
    fitxersortidatxt += "\n";
    fitxersortidatxt += "import ooolib\n";
    fitxersortidatxt += "\n";

    fitxersortidatxt += "# Crea el documento\n";
    fitxersortidatxt += "doc = ooolib.Calc(\"Listado\")\n";

    int y = 1;
    int x = 1;

    /// Sacamos las cabeceras con los nombres de los campos descriptivos (largos).
    for ( int h = 0; h < subf->mui_listcolumnas->rowCount(); ++h ) {
        if ( subf->mui_listcolumnas->item ( h, 0 ) ->checkState() == Qt::Checked ) {

            fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";

            QString textocabecera = ( subf->mui_listcolumnas->item ( h, 2 ) ->text() );
            textocabecera.replace ( QString ( "\n" ), QString ( "\\n\\\n" ) );

            /// Devuelve el ancho de la columna para ponerlo igual en el archivo de salida.
            fitxersortidatxt += "doc.set_column_property(" + QString::number ( x ) + ", 'width', '" + QString::number ( ( double ) subf->mui_list->columnWidth ( h ) / 90 ) + "in')\n\n";

            fitxersortidatxt += "doc.set_cell_property('bold', True)\n";
            fitxersortidatxt += "doc.set_cell_value(" + QString::number ( x++ ) + "," + QString::number ( y ) + ", 'string', u'" + textocabecera + "')\n";
            fitxersortidatxt += "doc.set_cell_property('bold', False)\n";
        } // end if
    } // end for

    y += 1;
    x = 1;
    /// Sacamos las cabeceras con los nombres de los campos de la base de datos.
    for ( int h = 0; h < subf->mui_listcolumnas->rowCount(); ++h ) {
        if ( subf->mui_listcolumnas->item ( h, 0 ) ->checkState() == Qt::Checked ) {

            fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";

            QString textocabecera = ( subf->mui_listcolumnas->item ( h, 1 ) ->text() );
            //QString textocabecera = (subf->mui_listcolumnas->dbFieldNameByColumnId());
            textocabecera.replace ( QString ( "\n" ), QString ( "\\n\\\n" ) );

            fitxersortidatxt += "doc.set_cell_property('bold', True)\n";
            fitxersortidatxt += "doc.set_cell_value(" + QString::number ( x++ ) + "," + QString::number ( y ) + ", 'string', u'" + textocabecera + "')\n";
            fitxersortidatxt += "doc.set_cell_property('bold', False)\n";
        } // end if
    } // end for

    y += 1;

//    bool resultconvdouble, resultconvinteger;
//    double resultadodouble, resultadointeger;

    /// Sacamos el contenido
    for ( int i = 0; i < subf->mui_list->rowCount(); ++i ) {

        int x = 1;

        for ( int j = 0; j < subf->mui_listcolumnas->rowCount(); ++j ) {
            if ( subf->mui_listcolumnas->item ( j, 0 ) ->checkState() == Qt::Checked ) {
                fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";

                QString textocontenido = blStringToPython( subf->mui_list->item ( i, j ) ->text() );


                //TODO: Mirar de mejorar el mecanismo de deteccion de tipo de dato.
                if ( subf->cabecera() -> at ( j )-> dbFieldType() == BlDbField::DbNumeric) {
                        /// Es 'double'.
                        fitxersortidatxt += "doc.set_cell_value(" + QString::number ( x++ ) + "," + QString::number ( y ) + ", 'float' , '" + textocontenido.replace(".","").replace(",",".") + "')\n\n";
                } else if ( subf->cabecera() -> at ( j )-> dbFieldType() == BlDbField::DbInt) {
                        /// Es 'double'.
                        fitxersortidatxt += "doc.set_cell_value(" + QString::number ( x++ ) + "," + QString::number ( y ) + ", 'float' , '" + textocontenido.replace(".","").replace(",",".") + "')\n\n";
                } else {
                    /// Es tratado como un 'string'.
                    textocontenido.replace ( QString ( "\n" ), QString ( "\\n\\\n" ) );
                    fitxersortidatxt += "doc.set_cell_value(" + QString::number ( x++ ) + "," + QString::number ( y ) + ", 'string', u'" + textocontenido + "')\n\n";
                } // end if

            } // end if
        } // end for blXMLEncode(subf->mui_list->item(i, j)->text())

        y++;

    } // end for

    fitxersortidatxt += "doc.save(\"listadoods.ods\")\n";

    QString cadena1;
    QString cadena2;

#ifdef Q_OS_WIN32
    cadena1 = g_confpr->value( CONF_DIR_USER );
    cadena1.replace("/", "\\");
    cadena1 = "\"del \"" + cadena1 + "listadoods.ods\"\"";

    cadena2 = archivod;
    cadena2.replace("/", "\\");
    cadena2 = "\"del \"" + cadena2 + "\"\"";
#else
    cadena1 = "rm " + g_confpr->value( CONF_DIR_USER ) + "listadoods.ods";
    cadena2 = "rm " + archivod;
#endif

    system ( cadena1.toAscii().data() );
    system ( cadena2.toAscii().data() );

    QFile file ( archivod );
    if ( file.open ( QIODevice::WriteOnly ) )  {
        QTextStream stream ( &file );
        stream.setCodec ( "UTF-8" );
        stream << fitxersortidatxt;
        file.close();
    } // end if

    QString cadena;

#ifdef Q_OS_WIN32

    cadena = g_confpr->value( CONF_DIR_USER );
    cadena.replace("/", "\\");

    cadena = "\"cd \"" + cadena + "\" & ";
    cadena = cadena + "\"" + g_confpr->value( CONF_PYTHON ) + "\" \"" + archivod + "\"\"";

#else
    cadena = " cd " + g_confpr->value( CONF_DIR_USER ) + "; python " + archivod;
#endif

    system ( cadena.toAscii().data() );

    /// Si no existe el archivo avisamos
    if (!QFile::exists(g_confpr->value( CONF_DIR_USER ) + "listadoods.ods") ) {
	  blMsgInfo ("Ha ocurrido un error. Verifique que la libreria python-ooolib esta correctamente instalada");
          blDebug ( "END PluginBl_SubForm2ODS::sacaods", 0 );
	  return;
    } // end if
    

#ifdef Q_OS_WIN32
    if (g_confpr->value( CONF_ODS ).isEmpty()) {
	    /// Abre con el programa por defecto del sistema.
	    cadena = "\"start \"\" \"" + g_confpr->value( CONF_DIR_USER ) + "listadoods.ods" + "\"\"";
    } else {
	    /// Abre con la configuracion forzada.
	    cadena = "\"start \"\" \"" + g_confpr->value( CONF_ODS ) + "\" \"" + g_confpr->value( CONF_DIR_USER ) + "listadoods.ods" + "\"\"";
    } // end if
#else
    cadena = g_confpr->value(CONF_ODS)  + " " + g_confpr->value( CONF_DIR_USER ) + "listadoods.ods &";
#endif

    int result = system ( cadena.toAscii().data() );
    if (result == -1) {
	blMsgError(_("Error al ejecutar el editor de ODS."));
    } // end if

    blDebug ( "END PluginBl_SubForm2ODS::sacaods", 0 );
}






///
/**
**/
void PluginBl_SubForm2ODS::sacaods1()
{
    blDebug ( "PluginBl_SubForm2ODS::sacaods", 0 );

    QString archivod = g_confpr->value( CONF_DIR_USER ) + "listadoods.py";
    BlTreeWidget * subf = ( BlTreeWidget * ) parent();


    QString fitxersortidatxt = "";

    fitxersortidatxt += "#!/usr/bin/python\n";
    fitxersortidatxt += "# -*- coding: utf8 -*-\n";
    fitxersortidatxt += "\n";
    fitxersortidatxt += "import ooolib\n";
    fitxersortidatxt += "\n";

    fitxersortidatxt += "# Crea el documento\n";
    fitxersortidatxt += "doc = ooolib.Calc(\"Listado\")\n";

    int y = 1;
    int x = 1;
/*
    /// Sacamos las cabeceras con los nombres de los campos descriptivos (largos).
    for ( int h = 0; h < subf->mui_listcolumnas->rowCount(); ++h ) {
        if ( subf->mui_listcolumnas->item ( h, 0 ) ->checkState() == Qt::Checked ) {

            fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";

            QString textocabecera = ( subf->mui_listcolumnas->item ( h, 2 ) ->text() );
            textocabecera.replace ( QString ( "\n" ), QString ( "\\n\\\n" ) );

            /// Devuelve el ancho de la columna para ponerlo igual en el archivo de salida.
            fitxersortidatxt += "doc.set_column_property(" + QString::number ( x ) + ", 'width', '" + QString::number ( ( double ) subf->mui_list->columnWidth ( h ) / 90 ) + "in')\n\n";

            fitxersortidatxt += "doc.set_cell_property('bold', True)\n";
            fitxersortidatxt += "doc.set_cell_value(" + QString::number ( x++ ) + "," + QString::number ( y ) + ", 'string', u'" + textocabecera + "')\n";
            fitxersortidatxt += "doc.set_cell_property('bold', False)\n";
        } // end if
    } // end for
*/
    y += 1;
    x = 1;

/*
    /// Sacamos las cabeceras con los nombres de los campos de la base de datos.
    for ( int h = 0; h < subf->mui_listcolumnas->rowCount(); ++h ) {
        if ( subf->mui_listcolumnas->item ( h, 0 ) ->checkState() == Qt::Checked ) {

            fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";

            QString textocabecera = ( subf->mui_listcolumnas->item ( h, 1 ) ->text() );
            //QString textocabecera = (subf->mui_listcolumnas->dbFieldNameByColumnId());
            textocabecera.replace ( QString ( "\n" ), QString ( "\\n\\\n" ) );

            fitxersortidatxt += "doc.set_cell_property('bold', True)\n";
            fitxersortidatxt += "doc.set_cell_value(" + QString::number ( x++ ) + "," + QString::number ( y ) + ", 'string', u'" + textocabecera + "')\n";
            fitxersortidatxt += "doc.set_cell_property('bold', False)\n";
        } // end if
    } // end for
*/

    y += 1;

//    bool resultconvdouble, resultconvinteger;
//    double resultadodouble, resultadointeger;


 QTreeWidgetItemIterator it(subf);
     while (*it) {



        int x = 1;

        for ( int j = 0; j < subf->columnCount(); j++ ) {
//            if ( subf->mui_listcolumnas->item ( j, 0 ) ->checkState() == Qt::Checked ) {
                fitxersortidatxt += "# Fila " + QString::number ( y ) + "\n";

                QString textocontenido = blStringToPython( (*it)->text ( j ) );

		bool ok;
		textocontenido.replace(".","").replace(",",".").toFloat(&ok);
                //TODO: Mirar de mejorar el mecanismo de deteccion de tipo de dato.
                if ( ok) {
                        /// Es 'double'.
                        fitxersortidatxt += "doc.set_cell_value(" + QString::number ( x++ ) + "," + QString::number ( y ) + ", 'float' , '" + textocontenido.replace(".","").replace(",",".") + "')\n\n";
                } else {
                    /// Es tratado como un 'string'.
                    textocontenido.replace ( QString ( "\n" ), QString ( "\\n\\\n" ) );
                    fitxersortidatxt += "doc.set_cell_value(" + QString::number ( x++ ) + "," + QString::number ( y ) + ", 'string', u'" + textocontenido + "')\n\n";
                } // end if

//            } // end if
        } // end for blXMLEncode(subf->mui_list->item(i, j)->text())

        y++;

         ++it;
     }

    fitxersortidatxt += "doc.save(\"listadoods.ods\")\n";

    QString cadena = "rm " + g_confpr->value( CONF_DIR_USER ) + "listadoods.ods";
    system ( cadena.toAscii() );
    cadena = "rm " + archivod;
    system ( cadena.toAscii() );

    QFile file ( archivod );
    if ( file.open ( QIODevice::WriteOnly ) )  {
        QTextStream stream ( &file );
        stream.setCodec ( "UTF-8" );
        stream << fitxersortidatxt;
        file.close();
    } // end if

    cadena = " cd " + g_confpr->value( CONF_DIR_USER ) + "; python " + archivod;
    system ( cadena.toAscii() );
    cadena = g_confpr->value(CONF_ODS) + " " + g_confpr->value( CONF_DIR_USER ) + "listadoods.ods &";
    system ( cadena.toAscii() );

    blDebug ( "END PluginBl_SubForm2ODS::sacaods", 0 );
}








///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    blDebug ( "BlSubForm_BlSubForm_Post", 0 );
    PluginBl_SubForm2ODS *subformods = new PluginBl_SubForm2ODS ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    blDebug ( "END BlSubForm_BlSubForm_Post", 0 );
    return 0;
}


///
/**
\param sub
\return
**/
int BlSubForm_preparaMenu ( BlSubForm *sub ) {
    blDebug ( "BlSubForm_preparaMenu", 0 );

    PluginBl_SubForm2ODS *subformods = new PluginBl_SubForm2ODS ( sub );
    
    QHBoxLayout *m_hboxLayout1 = sub->mui_menusubform->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( sub->mui_menusubform );
        m_hboxLayout1->setSpacing (0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    

      QToolButton *sel = new QToolButton ( sub->mui_menusubform );
      sel->setStatusTip ( "Exportar a hoja de calculo (ODS)" );
      sel->setToolTip ( "Exportar a hoja de calculo (ODS)" );
      sel->setMinimumSize ( QSize ( 18, 18 ) );
      sel->setIcon ( QIcon ( ":/Images/exportods.png" ) );
      sel->setIconSize ( QSize ( 18, 18 ) );    
      m_hboxLayout1->addWidget ( sel );
      sel->connect (sel, SIGNAL(released()), subformods, SLOT(sacaods ( )));

    blDebug ( "END BlSubForm_preparaMenu", 0 );
    return 0;
}


// ===================
// ESTO ES PROVISIONAL PARA QUE PUEDA HABER UNA SENCILLA EXPORTACION A HOJA DE CALCULO
// =====================



///
/**
\param sub
\return
**/
int BlTreeWidget_BlTreeWidget_Post ( BlTreeWidget *sub )
{
    blDebug ( "BlTreeWidget_BlTreeWidget_Post", 0 );
    PluginBl_SubForm2ODS *subformods = new PluginBl_SubForm2ODS ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu1 ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu1 ( QAction * ) ) );
    blDebug ( "END BlTreeWidget_BlTreeWidget_Post", 0 );
    return 0;
}

