/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include <QMap>
#include <QLineEdit>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

#include "partidasview.h"
#include "bfcompany.h"

#define COL_NOMFAMILIA            0
#define COL_CODCOMPLETOFAMILIA    1
#define COL_DESCFAMILIA           2
#define COL_IDFAMILIA             3
#define COL_CODFAMILIA            4
#define COL_PRODUCTOFISICOFAMILIA 5


///
/**
\param comp
\param parent
\param modoConsulta
**/
PartidasView::PartidasView ( BfCompany *comp, QWidget *parent, bool modoConsulta )
        : BfForm ( comp, parent )
{
    _depura ( "PartidasView::PartidasView", 0 );
    setupUi ( this );
    m_listPartidas->setColumnCount ( 3 );
    QStringList headers;
    headers << _ ( "Nombre" ) << _ ( "Codigo" ) << _ ( "Descripcion" ) << _ ( "Id familia" ) << _ ( "Codigo completo" );
    m_listPartidas->setHeaderLabels ( headers );

    m_listPartidas->setColumnWidth ( 0, 200 );
    m_listPartidas->setColumnWidth ( 1, 100 );

    m_listPartidas->setColumnHidden ( COL_IDFAMILIA, TRUE );
    m_listPartidas->setColumnHidden ( COL_CODFAMILIA, TRUE );
    m_listPartidas->setColumnHidden ( COL_PRODUCTOFISICOFAMILIA, TRUE );

    m_semaforoPintar = FALSE;

    m_idfamilia = "";

    mui_nomPartida->setEnabled ( FALSE );
    mui_descPartida->setEnabled ( FALSE );
    mui_codCompletoPartida->setEnabled ( FALSE );
    mui_codPartida->setEnabled ( FALSE );


    if ( modoConsulta ) {
        setModoConsulta();
        groupBox1->hide();
        mui_detalles->hide();
        mui_crear->hide();
        mui_guardar->hide();
        mui_borrar->hide();
        mui_cancelar->hide();
        mui_aceptar->hide();
        mui_imprimir->hide();
    } else {
        setModoEdicion();
        setAttribute ( Qt::WA_DeleteOnClose );
        mainCompany()->meteWindow ( windowTitle(), this, FALSE );
    } // end if

    pintar();
    _depura ( "END PartidasView::PartidasView", 0 );
}


///
/**
**/
PartidasView::~PartidasView()
{
    _depura ( "PartidasView::~PartidasView", 0 );
    _depura ( "END PartidasView::~PartidasView", 0 );
}


///
/**
**/
void PartidasView::pintar()
{
    _depura ( "PartidasView::pintar", 0 );
    /// Activamos el semaforo de pintado para que no haya slots concurrentes.
    m_semaforoPintar = TRUE;
    QTreeWidgetItem *it;
    QMap <int, QTreeWidgetItem*> Lista1;
    int padre;
    int idfamilia = 0;
    BlDbRecordSet *cursoraux1, *cursoraux2;

    /// Vaciamos el arbol.
    while ( m_listPartidas->topLevelItemCount() > 0 ) {
        it = m_listPartidas->takeTopLevelItem ( 0 );
        delete it;
    } // end while

    cursoraux1 = mainCompany()->loadQuery ( "SELECT * FROM familia WHERE padrefamilia IS NULL ORDER BY idfamilia" );
    while ( !cursoraux1->eof() ) {
        padre = cursoraux1->valor ( "padrefamilia" ).toInt();
        idfamilia = cursoraux1->valor ( "idfamilia" ).toInt();
        it = new QTreeWidgetItem ( m_listPartidas );
        Lista1[idfamilia] = it;
        it->setText ( COL_NOMFAMILIA, cursoraux1->valor ( "nombrefamilia" ) );
        it->setText ( COL_CODFAMILIA, cursoraux1->valor ( "codigofamilia" ) );
        it->setText ( COL_DESCFAMILIA, cursoraux1->valor ( "descfamilia" ) );
        it->setText ( COL_IDFAMILIA, cursoraux1->valor ( "idfamilia" ) );
        it->setText ( COL_CODCOMPLETOFAMILIA, cursoraux1->valor ( "codigocompletofamilia" ) );
        it->setText ( COL_PRODUCTOFISICOFAMILIA, cursoraux1->valor ( "productofisicofamilia" ) );
        m_listPartidas->expandItem ( it );
        cursoraux1->nextRecord();
    } // end while
    delete cursoraux1;
    cursoraux2 = mainCompany()->loadQuery ( "SELECT * FROM familia WHERE padrefamilia IS NOT NULL ORDER BY idfamilia" );
    while ( !cursoraux2->eof() ) {
        padre = cursoraux2->valor ( "padrefamilia" ).toInt();
        idfamilia = cursoraux2->valor ( "idfamilia" ).toInt();
        it = new QTreeWidgetItem ( Lista1[padre] );
        Lista1[idfamilia] = it;
        it->setText ( COL_NOMFAMILIA, cursoraux2->valor ( "nombrefamilia" ) );
        it->setText ( COL_CODFAMILIA, cursoraux2->valor ( "codigofamilia" ) );
        it->setText ( COL_DESCFAMILIA, cursoraux2->valor ( "descfamilia" ) );
        it->setText ( COL_IDFAMILIA, cursoraux2->valor ( "idfamilia" ) );
        it->setText ( COL_CODCOMPLETOFAMILIA, cursoraux2->valor ( "codigocompletofamilia" ) );
        it->setText ( COL_PRODUCTOFISICOFAMILIA, cursoraux2->valor ( "productofisicofamilia" ) );
        m_listPartidas->expandItem ( it );
        cursoraux2->nextRecord();
    } // end while
    delete cursoraux2;
    m_idfamilia = "";
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    m_semaforoPintar = FALSE; /// Desactivamos el semaforo de pintado.
    _depura ( "END PartidasView::pintar", 0 );
}


///
/**
\return
**/
QString PartidasView::codigoCompletoPartida()
{
    _depura ( "PartidasView::codigoCompletoPartida", 0 );
    QTreeWidgetItem *it = m_listPartidas->currentItem();
    if ( it ) {
        return it->text ( COL_CODCOMPLETOFAMILIA );
    } // end if
    _depura ( "END PartidasView::codigoCompletoPartida", 0 );
    return "";
}


///
/**
\return
**/
QString PartidasView::idPartida()
{
    _depura ( "PartidasView::idPartida", 0 );
    QTreeWidgetItem *it = m_listPartidas->currentItem();
    if ( it ) {
        return it->text ( COL_IDFAMILIA );
    } // end if
    _depura ( "END PartidasView::idPartida", 0 );
    return "";
}


///
/**
\return
**/
QString PartidasView::nombrePartida()
{
    _depura ( "PartidasView::nombrePartida", 0 );
    QTreeWidgetItem *it = m_listPartidas->currentItem();
    if ( it ) {
        return it->text ( COL_NOMFAMILIA );
    } // end if
    _depura ( "END PartidasView::nombrePartida", 0 );
    return "";
}


/// Se ha seleccionado un item en la lista.
/// Lo que hacemos es mostar el elemento.
/// Si el anterior ha sido modificado pedimos para actuar en consecuencia.
/**
\param it
**/
void PartidasView::on_m_listPartidas_itemDoubleClicked ( QTreeWidgetItem *it )
{
    _depura ( "PartidasView::on_m_listPartidas_itemDoubleClicked", 0 );
    if ( m_modoConsulta ) {
        m_idfamilia = it->text ( COL_IDFAMILIA );
        emit selected ( m_idfamilia );
    } // end if
    _depura ( "END PartidasView::on_m_listPartidas_itemDoubleClicked", 0 );
}


/// Se ha seleccionado un item en la lista.
/// Lo que hacemos es mostar el elemento.
/// Si el anterior ha sido modificado pedimos para actuar en consecuencia.
/**
\param current
\param previos
\return
**/
void PartidasView::on_m_listPartidas_currentItemChanged ( QTreeWidgetItem *current, QTreeWidgetItem *previous )
{

    if ( m_semaforoPintar ) return;
    m_semaforoPintar = TRUE;
    _depura ( "PartidasView::on_m_listPartidas_currentItemChanged", 0, m_idfamilia );
    /// Si estamos dentro del proceso de pintado salimos sin hacer nada ya que puede haber problemas.

    if ( previous ) {
        m_idfamilia = previous->text ( COL_IDFAMILIA );
    } // end if

    if ( !m_idfamilia.isEmpty() ) {
        trataModificado();
        if ( previous )
            pintar ( previous );

    } // end if
    if ( current ) {
        m_idfamilia = current->text ( COL_IDFAMILIA );
    } else {
        m_idfamilia = "";
    } // end if

    mostrarplantilla();
    _depura ( "END PartidasView::on_m_listPartidas_currentItemChanged", 0 );
    m_semaforoPintar = FALSE;
}


///
/**
**/
void PartidasView::mostrarplantilla()
{
    _depura ( "PartidasView::mostrarplantilla", 0 );
    QString query;
    if ( !m_idfamilia.isEmpty() ) {
        mui_nomPartida->setEnabled ( TRUE );
        mui_descPartida->setEnabled ( TRUE );
        mui_codCompletoPartida->setEnabled ( TRUE );
        mui_codPartida->setEnabled ( TRUE );

        query = "SELECT * from familia WHERE idfamilia = " + m_idfamilia;
        BlDbRecordSet *cursorfamilia = mainCompany()->loadQuery ( query );
        if ( !cursorfamilia->eof() ) {
            mui_nomPartida->setText ( cursorfamilia->valor ( "nombrefamilia" ) );
            mui_descPartida->setPlainText ( cursorfamilia->valor ( "descfamilia" ) );
            mui_codCompletoPartida->setText ( cursorfamilia->valor ( "codigocompletofamilia" ) );
            mui_codPartida->setText ( cursorfamilia->valor ( "codigofamilia" ) );

            if ( cursorfamilia->valor ( "productofisicofamilia" ) == "t" ) {
                mui_productofamilia->setChecked ( TRUE );
            } else {
                mui_serviciofamilia->setChecked ( TRUE );
            } // end if
        } // end if
        delete cursorfamilia;
    } else {
        mui_nomPartida->setEnabled ( FALSE );
        mui_descPartida->setEnabled ( FALSE );
        mui_codCompletoPartida->setEnabled ( FALSE );
        mui_codPartida->setEnabled ( FALSE );
        mui_nomPartida->setText ( "" );
        mui_descPartida->setPlainText ( "" );
        mui_codCompletoPartida->setText ( "" );
        mui_codPartida->setText ( "" );
    } // end if
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    _depura ( "END PartidasView::mostrarplantilla", 0 );
}


///
/**
\return
**/
bool PartidasView::trataModificado()
{
    _depura ( "PartidasView::trataModificado", 0 );
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_hayCambios() ) {
        if ( QMessageBox::warning ( this,
                                    _ ( "Guardar familia" ),
                                    _ ( "Desea guardar los cambios?" ),
                                    QMessageBox::Ok,
                                    QMessageBox::Cancel ) == QMessageBox::Ok ) {
            on_mui_guardar_released();
        } // end if
    } // end if
    _depura ( "END PartidasView::trataModificado", 0 );
    return ( FALSE );
}


/// SLOT que responde a la pulsacion del boton de guardar el tipo de IVA que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
int PartidasView::guardar()
{
    _depura ( "PartidasView::guardar", 0 );
    QString prodfam;
    try {
        if ( m_idfamilia.isEmpty() ) {
            mensajeInfo ( _ ( "Debe seleccionar una familia" ) );
            return -1;
        } // end if
        if ( mui_productofamilia->isChecked() ) {
            prodfam = " TRUE ";
        } else {
            prodfam = " FALSE ";
        } // end if
        QString query = "UPDATE familia SET nombrefamilia = '" +
                        mainCompany()->sanearCadena ( mui_nomPartida->text() ) + "', descfamilia = '" +
                        mainCompany()->sanearCadena ( mui_descPartida->toPlainText() ) + "' , codigofamilia = '" +
                        mainCompany()->sanearCadena ( mui_codPartida->text() ) + "', productofisicofamilia= " + prodfam + " WHERE idfamilia =" + m_idfamilia;
        int error = mainCompany()->runQuery ( query );
        if ( error ) {
            throw - 1;
        } // end if
        /// Guardamos la informacion de la fila que esta seleccionada para volver
        /// a ponerla despues.
        QTreeWidgetItem *posicionCursor;
        posicionCursor = m_listPartidas->currentItem();
        if ( posicionCursor ) {
            posicionCursor->setSelected ( TRUE );
            /// Pintamos los datos en el listado.
            pintar ( posicionCursor );
        } // end if
        dialogChanges_cargaInicial();
        _depura ( "END PartidasView::guardar", 0 );
        return 0;
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al guardar la familia" ) );
        return -1;
    } // end try
}


/// Pinta una fila especifica del listado de familias presentado.
/**
\param it
**/
void PartidasView::pintar ( QTreeWidgetItem *it )
{
    QString idfamilia = it->text ( COL_IDFAMILIA );
    if ( it ) {
        BlDbRecordSet * cursoraux1 = mainCompany()->loadQuery ( "SELECT * FROM familia WHERE idfamilia = " + idfamilia );
        if ( !cursoraux1->eof() ) {
            it->setText ( COL_NOMFAMILIA, cursoraux1->valor ( "nombrefamilia" ) );
            it->setText ( COL_CODFAMILIA, cursoraux1->valor ( "codigofamilia" ) );
            it->setText ( COL_DESCFAMILIA, cursoraux1->valor ( "descfamilia" ) );
            it->setText ( COL_IDFAMILIA, cursoraux1->valor ( "idfamilia" ) );
            it->setText ( COL_CODCOMPLETOFAMILIA, cursoraux1->valor ( "codigocompletofamilia" ) );
            it->setText ( COL_PRODUCTOFISICOFAMILIA, cursoraux1->valor ( "productofisicofamilia" ) );
        } // end if
        delete cursoraux1;
    } // end if
}


/// SLOT que responde a la pulsacion del boton de nuevo
/// Inserta en la tabla de Partidas
/**
**/
void PartidasView::on_mui_crear_released()
{
    _depura ( "PartidasView::on_mui_crear_released", 0 );
    try {
        mainCompany()->begin();
        /// Si se ha modificado el contenido advertimos y guardamos.
        trataModificado();
        QString padrefamilia;
        if ( m_idfamilia != "" ) {
            padrefamilia = m_idfamilia;
        } else {
            padrefamilia = "NULL";
        } // end if

        QString query = "INSERT INTO familia (nombrefamilia, descfamilia, padrefamilia, codigofamilia) VALUES ('NUEVA FAMILIA', 'Descripcion de la familia', " + padrefamilia + ", 'XXX')";

        int error = mainCompany()->runQuery ( query );
        if ( error ) {
            throw - 1;
        } // end if
        BlDbRecordSet *cur = mainCompany()->loadQuery ( "SELECT max(idfamilia) AS idfamilia FROM familia" );
        mainCompany()->commit();
        m_idfamilia = cur->valor ( "idfamilia" );
        delete cur;
        pintar();
        _depura ( "END PartidasView::on_mui_crear_released", 0 );
    } catch ( ... ) {
        mainCompany()->rollback();
        mensajeInfo ( _ ( "Error al crear la familia" ) );
    } // end try
}


/// SLOT que responde a la pulsacion del boton de nuevo desde Raiz
/// Inserta en la tabla de Partidas
/**
**/
/** No es necesario con las Qt 4.4
void PartidasView::on_mui_crearRaiz_released()
{
    _depura ( "PartidasView::on_mui_crearRaiz_released", 0 );
    try {
        mainCompany()->begin();
        /// Si se ha modificado el contenido advertimos y guardamos.
        trataModificado();

        QString query = "INSERT INTO familia (nombrefamilia, descfamilia, padrefamilia, codigofamilia) VALUES ('NUEVA FAMILIA', 'Descripcion de la familia',  NULL , 'XXX')";

        int error = mainCompany()->runQuery ( query );
        if ( error ) {
            throw - 1;
        } // end if
        BlDbRecordSet *cur = mainCompany()->loadQuery ( "SELECT max(idfamilia) AS idfamilia FROM familia" );
        mainCompany()->commit();
        m_idfamilia = cur->valor ( "idfamilia" );
        delete cur;
        pintar();
        _depura ( "END PartidasView::on_mui_crearRaiz_released", 0 );
    } catch ( ... ) {
        mainCompany()->rollback();
        mensajeInfo ( _("Error al crear la familia") );
    } // end try
}

*/


///
/**
**/
void PartidasView::on_mui_borrar_released()
{
    _depura ( "PartidasView::on_mui_borrar_released", 0 );

    int val = QMessageBox::question ( this,
                                      _ ( "Borrar" ) + " " + windowTitle(),
                                      _ ( "Desea eliminar" ) + " " + windowTitle(),
                                      QMessageBox::Yes,
                                      QMessageBox::Cancel | QMessageBox::Escape | QMessageBox::Default );

    if ( val == QMessageBox::Yes ) {
        if ( !borrar() ) {
            dialogChanges_cargaInicial();
            _depura ( windowTitle() + " " + "borrado satisfactoriamente.", 10 );
        } else {
            mensajeInfo ( windowTitle() + " " + _ ( "no se ha podido borrar" ) );
        } // end if
    } // end if
    _depura ( "END PartidasView::on_mui_borrar_released", 0 );
}


/// SLOT que responde a la pulsacion del boton de borrar la familia que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
int PartidasView::borrar()
{
    _depura ( "PartidasView::borrar", 0 );
    if ( m_idfamilia == "" ) {
        mensajeInfo ( _ ( "Debe seleccionar una familia" ) );
        return -1;
    } // end if
    try {
        QString query = "DELETE FROM FAMILIA WHERE idfamilia = " + m_idfamilia;
        int error = mainCompany()->runQuery ( query );
        if ( error ) {
            throw - 1;
        } // end if
        m_idfamilia = "";
        dialogChanges_cargaInicial();
        pintar();
        _depura ( "END PartidasView::borrar", 0 );
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al borrar la familia" ) );
        return -1;
    } // end try
    return 0;
}


///
/**
**/
void PartidasView::on_mui_imprimir_released()
{
    _depura ( "PartidasView::on_mui_imprimir_released", 0 );

    QString archivo = g_confpr->valor ( CONF_DIR_OPENREPORTS ) + "familias.rml";
    QString archivod = g_confpr->valor ( CONF_DIR_USER ) + "familias.rml";
    QString archivologo = g_confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system ( archivo.toAscii().constData() );
    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy "  + archivologo + " " + g_confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + g_confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#endif

    system ( archivologo.toAscii().constData() );
    QFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();

    QString fitxersortidatxt = "";

    /// Linea de totales del presupuesto.
    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"3cm, 15cm\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "        <td>" + _ ( "Codigo" ) + "</td>";
    fitxersortidatxt += "        <td>" + _ ( "Nombre" ) + "</td>";
    fitxersortidatxt += "</tr>";

    BlDbRecordSet *cur = mainCompany()->loadQuery ( "SELECT * FROM familia ORDER BY codigocompletofamilia" );
    while ( !cur->eof() ) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "        <td>" + cur->valor ( "codigocompletofamilia" ) + "</td>";
        fitxersortidatxt += "        <td>" + cur->valor ( "nombrefamilia" ) + "</td>";
        fitxersortidatxt += "</tr>";
        cur->nextRecord();
    } // end if
    delete cur;
    fitxersortidatxt += "</blockTable>";

    buff.replace ( "[story]", fitxersortidatxt );

    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << buff;
        file.close();
    } // end if
    invocaPDF ( "familias" );
    _depura ( "END PartidasView::on_mui_imprimir_released", 0 );
}


///
/**
**/
void PartidasView::on_mui_aceptar_released()
{
    _depura ( "PartidasView::on_mui_aceptar_released", 0 );
    QTreeWidgetItem *it = m_listPartidas->currentItem();
    if ( it ) {
        m_idfamilia = it->text ( COL_IDFAMILIA );
    } else {
        m_idfamilia = "";
    } // end if
    BfForm::on_mui_aceptar_released();
    _depura ( "END PartidasView::on_mui_aceptar_released", 0 );
}


///
/**
**/
void PartidasView::setModoConsulta()
{
    m_modoConsulta = TRUE;
}


///
/**
**/
void PartidasView::setModoEdicion()
{
    m_modoConsulta = FALSE;
}



