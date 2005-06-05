//
// C++ Implementation: factura
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "facturaview.h"
#include "company.h"
#include "listlinfacturaview.h"
#include "factura.h"
#include "clientslist.h"

#include <qmessagebox.h>

#include <qtable.h>
#include <qwidget.h>
#include <qobjectlist.h>
#include <qcombobox.h>
#include <qpopupmenu.h>
#include <qtoolbutton.h>

#include <qlayout.h>

#include <fstream>
using namespace std;

#include "funcaux.h"


FacturaView::FacturaView(company *comp, QWidget *parent, const char *name)
: FacturaBase(parent, name, Qt::WDestructiveClose) , Factura (comp) {
    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    subform2->setcompany(comp);
    setListLinFactura(subform2);
    m_cursorcombo=NULL;
    inicialize();
    comp->meteWindow(caption(),this);
    fprintf(stderr,"Fin de la inicializaci�n de Factura\n");
}


FacturaView::~FacturaView() {
    companyact->refreshBudgets();
    companyact->sacaWindow(this);
}

void FacturaView::inicialize() {
    m_nomalmacen->setText("");

    m_totalBases->setReadOnly(TRUE);
    m_totalBases->setAlignment(Qt::AlignRight);
    m_totalTaxes->setReadOnly(TRUE);
    m_totalTaxes->setAlignment(Qt::AlignRight);
    m_totalDiscounts->setReadOnly(TRUE);
    m_totalDiscounts->setAlignment(Qt::AlignRight);
    m_totalfactura->setReadOnly(TRUE);
    m_totalfactura->setAlignment(Qt::AlignRight);

    if (m_idfactura=="0") {
        cursor2 * cur0= companyact->cargacursor("SELECT * FROM configuracion where nombre='AlmacenDefecto'","queryconfig");
        if (!cur0->eof()) {
            if (cur0->valor("valor")!="") {
                m_codigoalmacen->setText(cur0->valor("valor"));
            }// end if
        }// end if
        delete cur0;
    }// end if
}// end inicialize

void FacturaView::pintaFormaPago(QString idformapago) {
    fprintf(stderr,"pintaFormaPago(%s)\n",idformapago.ascii());
    // Tratamos la forma de pago.
    m_comboformapago->clear();
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = companyact->cargacursor("SELECT * FROM forma_pago");
    int i = 0;
    int i1 = 0;
    while (!m_cursorcombo->eof()) {
        i ++;
        if (m_cursorcombo->valor("idforma_pago") == idformapago) {
            i1 = i;
        }
        m_comboformapago->insertItem(m_cursorcombo->valor("descforma_pago"));
        m_cursorcombo->siguienteregistro();
    }
    if (i1 != 0 ) {
        m_comboformapago->setCurrentItem(i1-1);
    }
} //end cargarcombodformapago



void   FacturaView::pintatotales(float base, float iva) {
    m_totalBases->setText(QString::number(base));
    m_totalTaxes->setText(QString::number(iva));
    m_totalfactura->setText(QString::number(iva+base));
}// end pintatotales


// Bsqueda de Clientes.
void FacturaView::s_searchClient() {
    fprintf(stderr,"Busqueda de un client\n");
    ClientsList *clients = new ClientsList(companyact, NULL, theApp->translate("Seleccione cliente","company"));
    clients->selectMode();
    // Esto es convertir un QWidget en un sistema modal de dialogo.
    this->setEnabled(false);
    clients->show();
    while(!clients->isHidden())
        theApp->processEvents();
    this->setEnabled(true);

    if (clients->cifclient() !="" && clients->cifclient() !=NULL) {
        m_cifclient->setText(clients->cifclient());
    }
    delete clients;
}// end searchClient

