
#include <QLabel>
#include "mticket.h"
#include "dbrecord.h"
#include "bulmatpv.h"
#include "subform2bt.h"
#include <QTextBrowser>

extern QTextBrowser *g_browser;

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, Fixed> base;


MTicket::MTicket ( EmpresaTPV *emp, QWidget *parent ) : BLWidget ( emp, parent )
{
    _depura ( "MTicket::MTicket", 0 );
    setupUi ( this );
    emp->pWorkspace() ->addWindow ( this );
    setWindowTitle ( "Ticket" );
    _depura ( "END MTicket::MTicket", 0 );
}


MTicket::~MTicket()
{
    _depura ( "MTicket::~MTicket", 0 );
    _depura ( "END MTicket::~MTicket", 0 );
}


void MTicket::pintar()
{
    _depura ( "MTicket::pintar", 0 );
    Ticket *tick =     ( ( EmpresaTPV * ) empresaBase() ) ->ticketActual();
    //QString html = "<font size=\"1\">";
    QString html = "<p style=\"font-family:monospace; font-size: 12pt;\">";
    QString html1 = "<font size=\"1\">";

    html1 += "Ticket: " + tick->DBvalue ( "nomticket" ) + "<BR>";
    html1 += "Trabajador: " + tick->DBvalue ( "idtrabajador" ) + "<BR>";
    QString query = "SELECT * FROM cliente WHERE idcliente = " + tick->DBvalue ( "idcliente" );
    cursor2 *cur1 = empresaBase() ->cargacursor ( query );
    html1 += "Cliente: " + tick->DBvalue ( "idcliente" ) + " " + cur1->valor ( "nomcliente" ) + "<BR>";
    delete cur1;

    html += "<TABLE border=\"0\">";
    DBRecord *item;
    for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
        item = tick->listaLineas() ->at ( i );
        QString bgcolor = "#FFFFFF";
        if ( item == tick->lineaActTicket() ) bgcolor = "#CCCCFF";
        html += "<TR>";
        html += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\" width=\"50\">" + item->DBvalue ( "cantlalbaran" ) + "</TD>";
        html += "<TD bgcolor=\"" + bgcolor + "\">" + item->DBvalue ( "nomarticulo" ) + "</TD>";
        Fixed totalLinea ( "0.00" );
        totalLinea = Fixed ( item->DBvalue ( "cantlalbaran" ) ) * Fixed ( item->DBvalue ( "pvplalbaran" ) );
        html += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\" width=\"50\">" + totalLinea.toQString(); + "</TD>";
        html += "</TR>";
    }// end for
    html += "</TABLE>";

// ======================================
    html += "<BR><HR><BR>";
    base basesimp;
    base basesimpreqeq;
    DBRecord *linea;
    /// Impresion de los contenidos.
    QString l;
    Fixed irpf ( "0" );

    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
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
    /*
        SDBRecord *linea1;
        if (m_listadescuentos->rowCount()) {
            for (int i = 0; i < m_listadescuentos->rowCount(); ++i) {
                linea1 = m_listadescuentos->lineaat(i);
                Fixed propor(linea1->DBvalue("proporcion" + m_listadescuentos->tableName()).toAscii().constData());
                porcentt = porcentt + propor;
            } // end for
        } // end if
    */

    /// Calculamos el total de base imponible.
    Fixed totbaseimp ( "0.00" );
    Fixed parbaseimp ( "0.00" );
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        if ( porcentt > Fixed ( "0.00" ) ) {
            parbaseimp = it.value() - it.value() * porcentt / 100;
        } else {
            parbaseimp = it.value();
        } // end if
        html1 += "Base Imp " + it.key() + "% " + parbaseimp.toQString() + "<BR>";
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
        html1 += "IVA " + it.key() + "% " + pariva.toQString() + "<BR>";
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
        html1 += "R.Eq " + it.key() + "% " + parreqeq.toQString() + "<BR>";
        totreqeq = totreqeq + parreqeq;
    } // end for



    Fixed totirpf = totbaseimp * irpf / 100;

    html1 += "<B>Base Imp. " + totbaseimp.toQString() + "<BR>";
    html1 += "<B>IVA. " + totiva.toQString() + "<BR>";
    html1 += "<B>IRPF. " + totirpf.toQString() + "<BR>";

    Fixed total = totiva + totbaseimp + totreqeq - totirpf;
    html1 += "<B>Total: " + total.toQString() + "<BR>";



    html += "</p>";
    html1 += "</FONT>";

// ======================================
    /// Pintamos el HTML en el textBrowser
    mui_browser->setText ( html );
    g_browser->setText ( html1 );
    _depura ( "END MTicket::pintar", 0 );
}


void MTicket::on_mui_subir_clicked()
{
/// Simulamos la pulsacion de la techa arriba
    ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_Up );

}


void MTicket::on_mui_bajar_clicked()
{
/// Simulamos la pulsacion de la techa arriba
    ( ( EmpresaTPV * ) empresaBase() ) ->pulsaTecla ( Qt::Key_Down );
}


void MTicket::on_mui_borrar_clicked()
{

    Ticket * tick = ( ( EmpresaTPV * ) empresaBase() ) ->ticketActual();
    tick->ponerCantidad ( "0" );

    pintar();
}

/// ============================= MYPLUGIN =====================



///
/**
**/
myplugin::myplugin()
{
    _depura ( "myplugin::myplugin", 0 );
    _depura ( "END myplugin::myplugin", 0 );
}


///
/**
**/
myplugin::~myplugin()
{
    _depura ( "myplugin::~myplugin", 0 );
    _depura ( "END myplugin::~myplugin", 0 );
}


///
/**
**/
void myplugin::elslot ( QTableWidgetItem * )
{
    _depura ( "myplugin::elslot", 0 );
    QString idarticulo =  m_lan->DBvalue ( "idarticulo" );
    m_tpv->empresaTPV() ->ticketActual() ->insertarArticulo ( idarticulo, Fixed ( "1" ) );
    _depura ( "END myplugin::elslot", 0 );
}


///
/**
\param bges
**/
void myplugin::inicializa ( BulmaTPV *tpv )
{
    _depura ( "myplugin::inicializa", 0 );
    m_tpv = tpv;

    m_lan = new SubForm2Bt ( NULL );
    m_lan->setEmpresaBase ( tpv->empresaTPV() );
    m_lan->setDBTableName ( "articulo" );
    m_lan->setDBCampoId ( "idarticulo" );
    m_lan->addSHeader ( "idarticulo", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "ID articulo" ) );
    m_lan->addSHeader ( "codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "Codigo completo del articulo" ) );
    m_lan->addSHeader ( "nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "Nombre del articulo" ) );
    m_lan->addSHeader ( "abrevarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "Descripcion abreviada del articulo" ) );
    m_lan->addSHeader ( "obserarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "Observaciones sobre el articulo" ) );
    m_lan->addSHeader ( "desctipo_articulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "Descripcion del tipo de articulo" ) );
    m_lan->addSHeader ( "desctipo_iva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "Descripcion tipo de I.V.A." ) );
    m_lan->addSHeader ( "pvparticulo", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "P.V.P. articulo" ) );
    m_lan->addSHeader ( "stockarticulo", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, QApplication::translate ( "ticketbasico", "Disponible en stock" ) );
    m_lan->setinsercion ( FALSE );
    m_lan->setDelete ( FALSE );
    m_lan->setSortingEnabled ( TRUE );
    tpv->workspace() ->addWindow ( m_lan );
    m_lan->showFullScreen();
    m_lan->cargar ( "SELECT * FROM articulo" );

    connect ( m_lan, SIGNAL ( itemDoubleClicked ( QTableWidgetItem * ) ), this, SLOT ( elslot ( QTableWidgetItem * ) ) );
    _depura ( "END myplugin::inicializa", 0 );
}