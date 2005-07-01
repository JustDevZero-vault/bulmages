//
// C++ Implementation: ListLinPedidoClienteView
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

/*
CREATE TABLE lpedidocliente (
   numlpedidocliente serial PRIMARY KEY,
   desclpedidocliente character varying(150),
   cantlpedidocliente float,
   pvplpedidocliente float,
   prevlpedidocliente date,
   ivalpedidocliente numeric(5,2),
   descuentolpedidocliente float,   
   idpedidocliente integer NOT NULL REFERENCES pedidocliente(idpedidocliente),
   idarticulo integer REFERENCES articulo(idarticulo)
);
*/

#define COL_NUMLPEDIDOCLIENTE 0
#define COL_IDARTICULO 1
#define COL_CODARTICULO 2
#define COL_NOMARTICULO 3
#define COL_DESCLPEDIDOCLIENTE 4
#define COL_CANTLPEDIDOCLIENTE 5
#define COL_PVPLPEDIDOCLIENTE 6
#define COL_DESCUENTOLPEDIDOCLIENTE 7
#define COL_IDPEDIDOCLIENTE 8
#define COL_REMOVE 9
#define COL_TASATIPO_IVA 10
#define COL_TIPO_IVA 11
#define COL_PREVLPEDIDOCLIENTE 12
#define COL_IVALPEDIDOCLIENTE 13

#include "articleslist.h"
#include "listlinpedidoclienteview.h"
#include "linpedidocliente.h"
#include <qtable.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>


ListLinPedidoClienteView::ListLinPedidoClienteView(QWidget * parent, const char * name) : QTable(parent, name), ListLinPedidoCliente() {
    /// Inicializamos la tabla de lineas de Factura
    setNumCols(14);
    setNumRows(100);
    horizontalHeader()->setLabel( COL_NUMLPEDIDOCLIENTE, tr( "N L�nea" ) );
    horizontalHeader()->setLabel( COL_DESCLPEDIDOCLIENTE, tr( "Descripci�n" ) );
    horizontalHeader()->setLabel( COL_CANTLPEDIDOCLIENTE, tr( "Cantidad" ) );
    horizontalHeader()->setLabel( COL_PVPLPEDIDOCLIENTE, tr( "Precio" ) );
    horizontalHeader()->setLabel( COL_DESCUENTOLPEDIDOCLIENTE, tr( "Descuento" ) );
    horizontalHeader()->setLabel( COL_IDPEDIDOCLIENTE, tr( "N Pedido" ) );
    horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Art�culo" ) );
    horizontalHeader()->setLabel( COL_CODARTICULO, tr( "C�digo Art�lo" ) );
    horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "Descripci�n Art�culo" ) );
    horizontalHeader()->setLabel( COL_TASATIPO_IVA, tr( "% IVA" ) );
    horizontalHeader()->setLabel( COL_TIPO_IVA, tr( "Tipo IVA" ) );
    horizontalHeader()->setLabel( COL_PREVLPEDIDOCLIENTE, tr( "COL_PREVLPEDIDOCLIENTE" ) );
    horizontalHeader()->setLabel( COL_IVALPEDIDOCLIENTE, tr( "COL_IVALPEDIDOCLIENTE" ) );

    setColumnWidth(COL_NUMLPEDIDOCLIENTE,100);
    setColumnWidth(COL_DESCLPEDIDOCLIENTE,300);
    setColumnWidth(COL_CANTLPEDIDOCLIENTE,100);
    setColumnWidth(COL_PVPLPEDIDOCLIENTE,100);
    setColumnWidth(COL_DESCUENTOLPEDIDOCLIENTE,74);
    setColumnWidth(COL_IDPEDIDOCLIENTE,100);
    setColumnWidth(COL_IDARTICULO,100);
    setColumnWidth(COL_CODARTICULO,100);
    setColumnWidth(COL_NOMARTICULO,300);
    setColumnWidth(COL_TASATIPO_IVA,50);
    setColumnWidth(COL_TIPO_IVA,50);

    hideColumn(COL_NUMLPEDIDOCLIENTE);
    hideColumn(COL_IDPEDIDOCLIENTE);
    hideColumn(COL_IDARTICULO);
    hideColumn(COL_REMOVE);
    hideColumn(COL_TASATIPO_IVA);
    hideColumn(COL_TIPO_IVA);

    setSelectionMode( QTable::SingleRow );

    setColumnReadOnly(COL_NOMARTICULO,true);
    // Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
    setPaletteBackgroundColor(confpr->valor(CONF_BG_LINPEDIDOSCLIENTE));

    connect(this, SIGNAL(valueChanged(int, int)), this, SLOT(valueBudgetLineChanged(int , int )));

    connect(this, SIGNAL(contextMenuRequested(int, int, const QPoint &)), this, SLOT(contextMenu(int, int, const QPoint &)));

    installEventFilter(this);

}


ListLinPedidoClienteView::~ListLinPedidoClienteView() {}


void ListLinPedidoClienteView::pintaListLinPedidoCliente() {
    fprintf(stderr,"INICIO de pintaListLinPedidoCliente\n");
    setNumRows(0);
    setNumRows(100);
    /// \todo Habr� que vaciar la tabla para que el pintado fuera exacto.
    LinPedidoCliente *linea;
    uint i = 0;
    for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
        setText(i, COL_NUMLPEDIDOCLIENTE, linea->numlpedidocliente());
        setText(i, COL_IDARTICULO, linea->idarticulo());
        setText(i, COL_CODARTICULO, linea->codigocompletoarticulo());
        setText(i, COL_NOMARTICULO, linea->nomarticulo());
        setText(i, COL_DESCLPEDIDOCLIENTE, linea->desclpedidocliente());
        setText(i, COL_CANTLPEDIDOCLIENTE, linea->cantlpedidocliente());
        setText(i, COL_DESCUENTOLPEDIDOCLIENTE, linea->descuentolpedidocliente());
        setText(i, COL_IDPEDIDOCLIENTE, linea->idpedidocliente());
        setText(i, COL_REMOVE, "XX");
        setText(i, COL_TASATIPO_IVA, "XX");
        setText(i, COL_TIPO_IVA, "XX");
        setText(i, COL_PVPLPEDIDOCLIENTE, linea->pvplpedidocliente());
	setText(i, COL_PREVLPEDIDOCLIENTE, linea->prevlpedidocliente());
	setText(i, COL_IVALPEDIDOCLIENTE, linea->ivalpedidocliente());
        i++;
    }// end for
    fprintf(stderr,"FIN de pintaListLinPedidoCliente\n");
}



void ListLinPedidoClienteView::contextMenu ( int row, int, const QPoint & pos ) {
    QPopupMenu *popup;
    int opcion;
    popup = new QPopupMenu;
    popup->insertItem(tr(tr("Borrar Linea")),1);
    opcion = popup->exec(pos);
    delete popup;
    switch(opcion) {
    case 1:
        borraLinPedidoCliente(row);
    }// end switch
}// end contextMenuRequested


void ListLinPedidoClienteView::borraLinPedidoClienteact() {
    borraLinPedidoCliente(currentRow());
}// end borraLinPedidoClienteact


void ListLinPedidoClienteView::pintalinListLinPedidoCliente(int pos) {
fprintf(stderr,"pintalinListLinPedidoCliente(%d)\n",pos);
    LinPedidoCliente *linea;
    linea = m_lista.at(pos);
        setText(pos, COL_NUMLPEDIDOCLIENTE, linea->numlpedidocliente());
        setText(pos, COL_IDARTICULO, linea->idarticulo());
        setText(pos, COL_CODARTICULO, linea->codigocompletoarticulo());
        setText(pos, COL_NOMARTICULO, linea->nomarticulo());
        setText(pos, COL_DESCLPEDIDOCLIENTE, linea->desclpedidocliente());
        setText(pos, COL_CANTLPEDIDOCLIENTE, linea->cantlpedidocliente());
        setText(pos, COL_DESCUENTOLPEDIDOCLIENTE, linea->descuentolpedidocliente());
        setText(pos, COL_IDPEDIDOCLIENTE, linea->idpedidocliente());
        setText(pos, COL_REMOVE, "XX");
        setText(pos, COL_TASATIPO_IVA, "XX");
        setText(pos, COL_TIPO_IVA, "XX");
        setText(pos, COL_PVPLPEDIDOCLIENTE, linea->pvplpedidocliente());
	setText(pos, COL_PREVLPEDIDOCLIENTE, linea->prevlpedidocliente());
	setText(pos, COL_IVALPEDIDOCLIENTE, linea->ivalpedidocliente());
}


bool ListLinPedidoClienteView::eventFilter( QObject *obj, QEvent *ev ) {
	fprintf(stderr,"eventFilter()\n");
    QString idArticle;
//    LinPedidoCliente *linea=lineaact();
    LinPedidoCliente *linea;//=m_lista.at(currentRow());
    
        if ( ev->type() == QEvent::KeyRelease ) {
//        if ( ev->type() == QEvent::KeyPress ) {
            QKeyEvent *k = (QKeyEvent *)ev;
            int col=currentColumn();
            int row=currentRow();
            switch (k->key()) {
            case Qt::Key_Plus:
                break;
            case Qt::Key_Asterisk:
	    	linea = lineaact();
                idArticle = searchArticle();
                linea->setidarticulo(idArticle);
                pintalinListLinPedidoCliente(currentRow());
		return TRUE;
                break;
            case Qt::Key_Return:
            case Qt::Key_Enter:
                // Esto se hace porque en la ltima linea del qtable tiene un comportamiento raro. Se reportar�como bug a trolltech.
                switch (col) {
                case COL_CODARTICULO:
                    setCurrentCell(row, COL_DESCLPEDIDOCLIENTE);
                    break;
                case COL_DESCLPEDIDOCLIENTE:
                    setCurrentCell(row, COL_CANTLPEDIDOCLIENTE);
                    break;
                case COL_CANTLPEDIDOCLIENTE:
                    setCurrentCell(row, COL_PVPLPEDIDOCLIENTE);
                    break;
                case COL_PVPLPEDIDOCLIENTE:
                    setCurrentCell(row+1, COL_CODARTICULO);
                    break;
                }// end switch
                return TRUE;
                break;
            }// end switch
    }// end if
    return QTable::eventFilter( obj, ev );
} //end eventFilter



void ListLinPedidoClienteView::valueBudgetLineChanged(int row, int col) {
    fprintf(stderr,"valueBudgetLineChanged \n");
    LinPedidoCliente *linea;
    linea = lineaat(row);
    if (linea != NULL) {
        switch (col) {
        case COL_DESCUENTOLPEDIDOCLIENTE: {
                float discountLine = text(row, COL_DESCUENTOLPEDIDOCLIENTE).replace(",",".").toFloat();
                linea->setdescuentolpedidocliente(QString::number(discountLine));
                break;
            }
        case COL_CODARTICULO:
            manageArticle(row);
            break;
        case COL_DESCLPEDIDOCLIENTE:
            linea->setdesclpedidocliente(text(row,COL_DESCLPEDIDOCLIENTE));
            break;
        case COL_CANTLPEDIDOCLIENTE: {
                float cantLine = text(row, COL_CANTLPEDIDOCLIENTE).replace(",",".").toFloat();
                linea->setcantlpedidocliente(QString::number(cantLine));
                break;
            }
        case COL_PVPLPEDIDOCLIENTE:  {
                float pvpLine = text(row, COL_PVPLPEDIDOCLIENTE).replace(",",".").toFloat();
                linea->setpvplpedidocliente(QString::number(pvpLine));
                break;
            }
        case COL_TASATIPO_IVA: {
                float ivaLine = text(row, COL_TASATIPO_IVA).replace(",",".").toFloat();
                linea->setivalpedidocliente(QString::number(ivaLine));
                break;
            }// end case
        }// end switch
        pintalinListLinPedidoCliente(row);
    }// end if
} //end valueBudgetLineChanged


/// Devuelve la linea que se esta tratando actualmente
LinPedidoCliente *ListLinPedidoClienteView::lineaact() {
    fprintf(stderr,"ListLinPedidoClienteView::lineaact()\n");
    return lineaat(currentRow());
}// end lineaact


/// Devuelve la linea especificada, y si no existe se van creando lineas hasta que exista.
LinPedidoCliente *ListLinPedidoClienteView::lineaat(int row) {
    fprintf(stderr,"ListLinPedidoCliente::lineaat(%d)\n", row);
    LinPedidoCliente *linea;
    if (row >=0) {
        while (m_lista.at(row) == 0 ) {
	    fprintf(stderr,"Creamos la linea\n");
            linea=new LinPedidoCliente(companyact);
            linea->setidpedidocliente(mdb_idpedidocliente);
            m_lista.append(linea);  
        }// end while	
	      return(m_lista.at(row));
    } else {
    	fprintf(stderr,"Linea inexistente\n");
        return NULL;
    }// end if

}// end lineaat


void ListLinPedidoClienteView::manageArticle(int row) {
	fprintf(stderr,"manageArticle(%d)\n",row);
    LinPedidoCliente *linea= lineaat(row);
    QString articleCode = text(row, COL_CODARTICULO);
    linea->setcodigocompletoarticulo(text(row,COL_CODARTICULO));
    pintalinListLinPedidoCliente(row);
} //end manageArticle


QString ListLinPedidoClienteView::searchArticle() {
    fprintf(stderr,"Busqueda de un art�culo\n");
    articleslist *artlist = new articleslist(companyact, NULL, theApp->translate("Seleccione Art�culo","company"));
    // , WType_Dialog| WShowModal
    artlist->modoseleccion();
    // Esto es convertir un QWidget en un sistema modal de dialogo.
    this->setEnabled(false);
    artlist->show();
    while(!artlist->isHidden())
        theApp->processEvents();
    this->setEnabled(true);
    QString idArticle = artlist->idArticle();
    delete artlist;
    return idArticle;
}// end searchArticle
