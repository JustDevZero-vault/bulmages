//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "tipoivaview.h"

#include "empresa.h"

tipoivaview::tipoivaview(empresa *emp,QWidget *parent, const char *name) : tipoivadlg(parent, name) {
    empresaactual = emp;
    conexionbase = empresaactual->bdempresa();
    m_curtipoiva = NULL;
    pintar();
}// end tipoivaview


/** El destructor de la clase guarda los datos (por si ha habido cambios)
  * y libera la memoria que se haya ocupado
  */
tipoivaview::~tipoivaview() {
    s_saveTipoIVA();
    if (m_curtipoiva != NULL)
        delete m_curtipoiva;
}// end ~tipoivaview


/** Pinta la ventana, recarga el combo y si se pasa el parametro muestra el identificador indicado
  */
void tipoivaview::pintar(QString idtipoiva) {
    int posicion=0;
    /// Vamos a inicializar el combo de los tipos de IVA
    if (m_curtipoiva != NULL)
        delete m_curtipoiva;
    QString query = "SELECT * from tipoiva left join cuenta ON tipoiva.idcuenta = cuenta.idcuenta ORDER BY nombretipoiva";
    m_curtipoiva = conexionbase->cargacursor(query);
    m_comboTipoIVA->clear();
    int i = 0;
    while (!m_curtipoiva->eof()) {
        m_comboTipoIVA->insertItem(m_curtipoiva->valor("nombretipoiva"), i);
        if (idtipoiva == m_curtipoiva->valor("idtipoiva") )
            posicion=i;
        m_curtipoiva->siguienteregistro();
        i++;
    }// end while
    mostrarplantilla(posicion);
}// end pintar


/**
  * Esta funcion muestra el tipo de iva en la ventana.
  * \param pos si es distinto de cero se busca en el combo la posici�n indicada sino se usa la posici�n actual del combo.
  */
void tipoivaview::mostrarplantilla(int pos) {
    fprintf(stderr,"mostrarplantilla\n");
    if (m_comboTipoIVA->count() > 0) {
        int i;
        if (pos != 0)
            m_comboTipoIVA->setCurrentItem(pos);
        i = m_comboTipoIVA->currentItem();
        m_nombreTipoIVA->setText(m_curtipoiva->valor("nombretipoiva", i));
        m_codigoCtaTipoIVA->setText(m_curtipoiva->valor("codigo",i));
        m_porcentTipoIVA->setText(m_curtipoiva->valor("porcentajetipoiva",i));
    }// end if
}// end mostrarplantilla


/*****************************************************
  Esta funcion sirve para hacer el cambio sobre un
  centro de coste .
  ****************************************************/
void tipoivaview::cambiacombo(int) {
    fprintf(stderr,"cambiado el combo\n");
    mostrarplantilla();
}// end cambiacombo


/** SLOT que responde a la pulsaci�n del bot�n de guardar el tipo de iva que se est� editando.
  * Lo que hace es que se hace un update de todos los campos
  */
void tipoivaview::s_saveTipoIVA() {
    int posicion = m_comboTipoIVA->currentItem();
    QString idtipoiva = m_curtipoiva->valor("idtipoiva",posicion);
    QString query = "UPDATE tipoiva SET nombretipoiva='"+m_nombreTipoIVA->text()+"', porcentajetipoiva= "+m_porcentTipoIVA->text()+" , idcuenta = id_cuenta('"+m_codigoCtaTipoIVA->text()+"') WHERE idtipoiva="+m_curtipoiva->valor("idtipoiva", posicion);
    conexionbase->ejecuta(query);
    pintar(m_curtipoiva->valor("idtipoiva", posicion));
}// end s_saveTipoIVA


/** SLOT que responde a la pulsaci�n del bot�n de nuevo tipo de iva
  * Inserta en la tabla de ivas
  */
void tipoivaview::s_newTipoIVA() {
    QString query = "INSERT INTO tipoiva (nombretipoiva, porcentajetipoiva, idcuenta) VALUES ('NUEVO TIPO IVA',0,id_cuenta('47'))";
    conexionbase->begin();
    conexionbase->ejecuta(query);
    cursor2 *cur = conexionbase->cargacursor("SELECT max(idtipoiva) AS idtipoiva FROM tipoiva");
    conexionbase->commit();
    pintar(cur->valor("idtipoiva"));
    delete cur;
}// end s_newTipoIVA


/** SLOT que responde a la pulsaci�n del bot�n de borrar un tipo de IVA
  * Borra en la tabla de tiposiva el TIPO de iva concreto
  */
void tipoivaview::s_deleteTipoIVA() {

    switch( QMessageBox::warning( this, "Borrar Tipo de IVA",
        "Se va a borrar el Tipo de IVA,\n"
        "Esto puede ocasionar p�rdida de datos\n"
        "Tal vez deberia pensarselo mejor antes\n"
        "porque igual su trabajo se va a tomar por culo.",
        QMessageBox::Ok ,
        QMessageBox::Cancel )) {
    case QMessageBox::Ok: // Retry clicked or Enter pressed
	conexionbase->ejecuta("DELETE FROM tipoiva WHERE idtipoiva ="+m_curtipoiva->valor("idtipoiva",m_comboTipoIVA->currentItem()));
	pintar();	
        break;
    case QMessageBox::Cancel: // Abort clicked or Escape pressed
        break;
    }// end switch

}// s_deleteTipoIVA

