#include <qapplication.h>
#include "configuracion.h"
#include "logpass.h"
#include "bselector.h"
#include "postgresiface2.h"
#include <qtextcodec.h>

/// Instancia de la aplicaci�n. Usada en algunos casos para acceder a determinadas funcionalidades como la traducci�n.
QApplication *theApp;
/// Instancia de la traducci�n que se carga en tiempo de ejecuci�n y que se usa de forma global.
QTranslator * traductor;
/** \author Tomeu Borr�s Riera
  * \fn main
  * \brief Funci�n principal del programa BulmaG�s
  *
  * Crea el objeto base y lo lanza, tambi�n comprueba si se ha lanzado la
  * aplicaci�n con parametros o sin ellos
  */
int main( int argc, char ** argv ) {
    confpr = new configuracion();
    QApplication a( argc, argv );
    theApp = &a;
    traductor = new QTranslator ( 0 );
    // set the location where your .qm files are in load() below as the last parameter instead of "."
    // for development, use "/" to use the english original as
    // .qm files are stored in the base project directory.
    if (confpr->valor(CONF_TRADUCCION) == "locales") {
        traductor->load( QString("bulmages_") + QTextCodec::locale(), confpr->valor(CONF_DIR_TRADUCCION).ascii() );
    } else {
        QString archivo = "bulmages_"+confpr->valor(CONF_TRADUCCION);
        traductor->load(archivo.ascii(),confpr->valor(CONF_DIR_TRADUCCION).ascii());
    }// end if    delete login1;

    a.installTranslator( traductor );
    BSelector *bw = new BSelector();
    bw->setCaption( "BulmaG�s Selector" );
    a.setMainWidget(bw);

    /// Lo primero que hacemos es comprobar el sistema de autentificaci�n de Postgres para pedir un loggin y un password en caso de que sea necesario para entrar en el sistema.
    logpass *login1 = new logpass(0,"");
    if (!login1->authOK()) {
        login1->exec();
    }// end if

    /// Una vez que hemos pasado por el sistema de loggin y password y ya somos autenticos podemos mostrar el selector de acciones.
    bw->show();
    return a.exec();
}// end main
