//
// C++ Implementation: ListDescuentoPresupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "listdescpresupuesto.h"
#include "company.h"
#include "descpresupuesto.h"
#include "funcaux.h"

ListDescuentoPresupuesto::ListDescuentoPresupuesto(company *comp) {
    companyact = comp;
    m_lista.setAutoDelete(TRUE);
    mdb_idpresupuesto="";
}// end ListDescuentoPresupuesto

ListDescuentoPresupuesto::ListDescuentoPresupuesto() {
    fprintf(stderr,"Constructor de ListDescuentoPresupuesto\n");
    companyact=NULL;
    m_lista.setAutoDelete(TRUE);
    mdb_idpresupuesto="";
}// end ListDescuentoPresupuesto

ListDescuentoPresupuesto::~ListDescuentoPresupuesto() {}


void ListDescuentoPresupuesto::nuevalinea(QString concept, QString propor) {
    DescuentoPresupuesto *lin = new DescuentoPresupuesto(companyact,
                                "",
                                concept,
                                propor,
                                mdb_idpresupuesto);
    m_lista.append(lin);
}// end nuevalinea

DescuentoPresupuesto *ListDescuentoPresupuesto::linpos(int pos) {
    return (m_lista.at(pos));
}// end linpos


// Carga l�eas de presupuesto
int ListDescuentoPresupuesto::cargaDescuentos(QString idbudget) {
    int error=0;
    vaciar();
    _depura("ListDescuentoPresupuesto::chargeBudgetLines\n",0);
    mdb_idpresupuesto = idbudget;
    fprintf(stderr,"Hacemos la carga del cursor\n");
    cursor2 * cur= companyact->cargacursor("SELECT * FROM dpresupuesto WHERE idpresupuesto="+idbudget,"unquery");
    if (cur->error())
        error = 1;
    int i=0;
    while (!cur->eof())   {
        /// Creamos un elemento del tipo DescuentoPresupuesto y lo agregamos a la lista.
        DescuentoPresupuesto *lin = new DescuentoPresupuesto(companyact,
                                    cur->valor("iddpresupuesto"),
                                    cur->valor("conceptdpresupuesto"),
                                    cur->valor("proporciondpresupuesto"),
                                    cur->valor("idpresupuesto")
                                                            );
        m_lista.append(lin);
        i++;
        cur->siguienteregistro();
    }// end while
    delete cur;
    /// Tratamiento de excepciones
    if (error) {
        _depura("Error en la carga de la linea de presupuesto\n",0);
        return 1;
    }// end if
    _depura("Fin de ListDescuentoPresupuesto::chargeBudgetLines\n",0);
    return 0;
}// end chargeBudgetLines


void ListDescuentoPresupuesto::guardaListDescuentoPresupuesto() {
    DescuentoPresupuesto *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        linea->guardaDescuentoPresupuesto();
        i++;
    }// end for
}// en guardaListDescuentoPresupuesto



void ListDescuentoPresupuesto::vaciar() {
    mdb_idpresupuesto = "";
    m_lista.clear();
}// end guardaListDescuentoPresupuesto


void ListDescuentoPresupuesto::borrar() {
    if (mdb_idpresupuesto != "")  {
        companyact->begin();
        int error = companyact->ejecuta("DELETE FROM dpresupuesto WHERE idpresupuesto="+mdb_idpresupuesto);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
    }// end if
}// end borrar


void ListDescuentoPresupuesto::borraDescuentoPresupuesto(int pos) {
    DescuentoPresupuesto *linea;
    linea = m_lista.at(pos);
    linea->borrar();
    m_lista.remove(pos);
    pintaListDescuentoPresupuesto();
}// end borraDescuentoPresupuesto
