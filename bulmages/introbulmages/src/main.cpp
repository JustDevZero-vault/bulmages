#include <qapplication.h>
#include "configuracion.h"
#include "logpass.h"
#include "bselector.h"
#include "postgresiface2.h"
#include <qtextcodec.h>

QApplication *theApp;
QTranslator * traductor;

int main( int argc, char ** argv ) {
    confpr = new configuracion();
    QApplication a( argc, argv );
    theApp = &a;

  traductor = new QTranslator ( 0 );
  // set the location where your .qm files are in load() below as the last parameter instead of "."
  // for development, use "/" to use the english original as
  // .qm files are stored in the base project directory.
if (confpr->valor(CONF_TRADUCCION) == "locales") {
  traductor->load( QString("bulmages_") + QTextCodec::locale(), confpr->valor(CONF_DIR_TRADUCCION).c_str() );
} else {
   string archivo = "bulmages_"+confpr->valor(CONF_TRADUCCION);
   traductor->load(archivo.c_str(),confpr->valor(CONF_DIR_TRADUCCION).c_str());
}// end if    delete login1;

  a.installTranslator( traductor );
    BSelector *bw = new BSelector();
    bw->setCaption( "BulmaG�s Selector" );
    a.setMainWidget(bw);
    
     
    logpass *login1 = new logpass(0,"");
    if (!login1->authOK) {
	login1->exec();
    }// end if
    
    bw->show();
    return a.exec();   //QLabel *lblAuthError;
//    delete login1;
    
}// end main
