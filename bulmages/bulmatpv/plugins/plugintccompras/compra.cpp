#include "compra.h"
#include "fixed.h"
#include <QWidget>
#include <vector>

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, Fixed> base;

Compra::Compra ( EmpresaTPV *emp, QWidget *parent ) : BLWidget ( emp, parent )
{
    setupUi ( ( QDialog* ) this );
/*
    this->mui_tablesFrame->setDisabled ( TRUE );

    m_ticket = NULL;
*/
    /*
        m_value = 0;
        base basesimp;
        base basesimpreqeq;
        DBRecord *linea;

        Ticket *tick = emp->ticketActual();

        /// Impresion de los contenidos.
        QString l;
        Fixed irpf ( "0" );

        cursor2 *cur = emp->cargacursor ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
        if ( cur ) {
            if ( !cur->eof() ) {
                irpf = Fixed ( cur->valor ( "valor" ) );
            } // end if
            delete cur;
        } // end if


        Fixed descuentolinea ( "0.00" );
        for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
            linea = tick->listaLineas() ->at ( i );
            Fixed cant ( linea->DBvalue ( "cantlalbaran" ) );
            Fixed pvpund ( linea->DBvalue ( "pvplalbaran" ) );
            Fixed desc1 ( linea->DBvalue ( "descuentolalbaran" ) );
            Fixed cantpvp = cant * pvpund;
            Fixed base = cantpvp - cantpvp * desc1 / 100;
            descuentolinea = descuentolinea + ( cantpvp * desc1 / 100 );
            basesimp[linea->DBvalue ( "ivalalbaran" ) ] = basesimp[linea->DBvalue ( "ivalalbaran" ) ] + base;
            basesimpreqeq[linea->DBvalue ( "reqeqlalbaran" ) ] = basesimpreqeq[linea->DBvalue ( "reqeqlalbaran" ) ] + base;
        } // end for

        Fixed basei ( "0.00" );
        base::Iterator it;
        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
            basei = basei + it.value();
        } // end for

        /// Calculamos el total de los descuentos.
        /// De momento aqui no se usan descuentos generales en venta.
        Fixed porcentt ( "0.00" );

        /// Calculamos el total de base imponible.
        Fixed totbaseimp ( "0.00" );
        Fixed parbaseimp ( "0.00" );
        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
            if ( porcentt > Fixed ( "0.00" ) ) {
                parbaseimp = it.value() - it.value() * porcentt / 100;
            } else {
                parbaseimp = it.value();
            } // end if
            totbaseimp = totbaseimp + parbaseimp;
        } // end for

        /// Calculamos el total de IVA.
        Fixed totiva ( "0.00" );
        Fixed pariva ( "0.00" );
        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
            Fixed piva ( it.key().toAscii().constData() );
            if ( porcentt > Fixed ( "0.00" ) ) {
                pariva = ( it.value() - it.value() * porcentt / 100 ) * piva / 100;
            } else {
                pariva = it.value() * piva / 100;
            } // end if
            totiva = totiva + pariva;
        } // end for

        /// Calculamos el total de recargo de equivalencia.
        Fixed totreqeq ( "0.00" );
        Fixed parreqeq ( "0.00" );
        for ( it = basesimpreqeq.begin(); it != basesimpreqeq.end(); ++it ) {
            Fixed preqeq ( it.key().toAscii().constData() );
            if ( porcentt > Fixed ( "0.00" ) ) {
                parreqeq = ( it.value() - it.value() * porcentt / 100 ) * preqeq / 100;
            } else {
                parreqeq = it.value() * preqeq / 100;
            } // end if
            totreqeq = totreqeq + parreqeq;
        } // end for

        Fixed totirpf = totbaseimp * irpf / 100;

        Fixed total = totiva + totbaseimp + totreqeq - totirpf;

        mui_total->setText(total.toQString());
    */
}

void Compra::on_mui_aceptar_released() {
	close();
	
}

void Compra::on_mui_codigoarticulo_returnPressed() {

	QString texto = "<TABLE width=\"100%\">";
	QString tventas = "0";
	QString tcompras = "0";
	QString bgcolor = "#FFFFFF";
	texto += "<TR bgcolor=\"#BBBBBB\">";
	texto += "<TD width=\"100\">Color</TD>";
	texto += "<TD width=\"100\">Talla</TD>";
	texto += "<TD width=\"100\">Comprados</TD>";
	texto += "<TD width=\"100\">Vendidos</TD>";
	texto += "<TD width=\"100\">Stock</TD>";
	texto += "</TR>";
	QString query = "SELECT * FROM articulo LEFT JOIN tc_articulo_alias AS t3 ON articulo.idarticulo = t3.idarticulo  LEFT JOIN tc_talla AS t1 ON t3.idtc_talla = t1.idtc_talla LEFT JOIN tc_color AS t2 ON t3.idtc_color = t2.idtc_color WHERE t3.aliastc_articulo_tallacolor = '" + mui_codigoarticulo->text() + "' OR articulo.codigocompletoarticulo = '"+mui_codigoarticulo->text()+"' ORDER BY nomtc_color, nomtc_talla";
	cursor2 *cur = empresaBase()->cargacursor(query);
	while (! cur->eof()) {
		QString query1= "SELECT SUM(cantlalbaranp) AS suma FROM lalbaranp WHERE idarticulo="+cur->valor("idarticulo")+"  AND idtc_talla="+cur->valor("idtc_talla")+" AND idtc_color="+ cur->valor("idtc_color");
		QString query2= "SELECT SUM(cantlalbaran) AS suma FROM lalbaran WHERE idarticulo="+cur->valor("idarticulo")+"  AND idtc_talla="+cur->valor("idtc_talla")+" AND idtc_color="+ cur->valor("idtc_color");
		cursor2 *cur1 = empresaBase()->cargacursor(query1);
		cursor2 *cur2 = empresaBase()->cargacursor(query2);
		if (!cur2->eof()) {
			tventas = cur2->valor("suma");
		} // end if
		if (!cur1->eof()) {
			tcompras = cur1->valor("suma");
		} // end if
			
		if (!cur1->eof()) {
			texto += "<TR bgcolor=\""+bgcolor+"\">";
			texto += "<TD>" + cur->valor ("nomtc_color") + "</TD>";
			texto += "<TD>" + cur->valor ("nomtc_talla") + "</TD>";
			texto += "<TD align=\"right\">" + tcompras + "</TD>";
			texto += "<TD align=\"right\">" + tventas + "</TD>";
			texto += "<TD align=\"right\">" + QString::number(tcompras.toFloat() - tventas.toFloat()) + "</TD>";
			texto += "</TR>";
		} // end if
		delete cur1;
		delete cur2;
		cur->siguienteregistro();
		if (bgcolor == "#FFFFFF") {
			bgcolor = "#CCCCFF";
		} else {
			bgcolor =  "#FFFFFF";
		} // end if
	} // end while
	delete cur;
	mui_subform->setText(texto);
}

Compra::~Compra()
{}


