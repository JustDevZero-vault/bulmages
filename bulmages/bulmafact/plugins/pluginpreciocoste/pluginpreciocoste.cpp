/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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
#include <QToolButton>

#include "pluginpreciocoste.h"


///
/**
\return
**/
int entryPoint ( Bulmafact * )
{
    _depura ( "Estoy dentro del plugin de precio de Coste", 0 );
    return 0;
}


/// Al crear la ventana de trabajadores tambien creamos un combo box para el tipo de trabajador.
/**
\param trab
\return
**/
int ArticuloView_ArticuloView_Post ( ArticuloView *trab )
{
    _depura ( "ArticuloView_ArticuloView_Post", 0 );

    trab->addDBCampo ( "preciocostearticulo", DBCampo::DBnumeric, DBCampo::DBNothing, QApplication::translate ( "Articulo", "Precio Coste" ) );
    trab->addDBCampo ( "margenarticulo", DBCampo::DBnumeric, DBCampo::DBNothing, QApplication::translate ( "Articulo", "Margen" ) );
    trab->addDBCampo ( "actualizarmargenarticulo", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate ( "Articulo", "Actualizar PVP" ) );


    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout19" ) );

    QLabel *textLabel2_9_26 = new QLabel ( trab->m_frameplugin );
    textLabel2_9_26->setObjectName ( QString::fromUtf8 ( "textLabel2_9_9" ) );
    hboxLayout160->addWidget ( textLabel2_9_26 );
    textLabel2_9_26->setText ( "Precio Coste" );

    QLineEdit *passtrab = new QLineEdit ( trab->m_frameplugin );
    passtrab->setObjectName ( QString::fromUtf8 ( "mui_preciocostearticulo" ) );
    hboxLayout160->addWidget ( passtrab );

    QLabel *textLabel2_9_27 = new QLabel ( trab->m_frameplugin );
    textLabel2_9_27->setObjectName ( QString::fromUtf8 ( "textLabel2_9_97" ) );
    hboxLayout160->addWidget ( textLabel2_9_27 );
    textLabel2_9_27->setText ( "Margen" );

    QLineEdit *passtrab1 = new QLineEdit ( trab->m_frameplugin );
    passtrab1->setObjectName ( QString::fromUtf8 ( "mui_margenarticulo" ) );
    hboxLayout160->addWidget ( passtrab1 );


    QLabel *textLabel2_9_28 = new QLabel ( trab->m_frameplugin );
    textLabel2_9_28->setObjectName ( QString::fromUtf8 ( "textLabel2_9_98" ) );
    hboxLayout160->addWidget ( textLabel2_9_28 );
    textLabel2_9_28->setText ( "Actualizar" );

    QCheckBox *passtrab2 = new QCheckBox ( trab->m_frameplugin );
    passtrab2->setObjectName ( QString::fromUtf8 ( "mui_actualizarmargenarticulo" ) );
    hboxLayout160->addWidget ( passtrab2 );

    /// Comprobamos que exista el layout.
    QVBoxLayout *m_hboxLayout1 = trab->m_frameplugin->findChild<QVBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QVBoxLayout ( trab->m_frameplugin );
        m_hboxLayout1->setSpacing ( 0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addLayout ( hboxLayout160 );
    return 0;
}


int SubForm2Bf_on_mui_list_editFinished ( SubForm2Bf *subform )
{
    _depura ( "SubForm2Bf_on_mui_list_editFinished", 0 );
    int err;
    cursor2 *cur = NULL;
    cursor2 *cur1 = NULL;
    cursor2 *cur2 = NULL;

    if (  subform->tableName() != "lpedidoproveedor"
            && subform->tableName() != "lalbaranp"
            && subform->tableName() != "lfacturap" ) {
        return 0;
    } // end if

    SDBRecord *rec = subform->lineaat ( subform->m_prevRow );
    if ( rec == NULL ) {
        return -1;
    } // end if

    SDBCampo *camp = ( SDBCampo * ) subform->item ( subform->m_prevRow, subform->m_prevCol );
    camp->refresh();

    if ( camp->nomcampo() == "pvplalbaranp"  || camp->nomcampo() == "pvplfacturap" ) {
        SDBRecord * rec = subform->lineaat ( subform->currentRow() );

        if ( rec->DBvalue ( "idarticulo" ) == "" ) return 0;

        QString query = "SELECT preciocostearticulo FROM articulo WHERE idarticulo = " + rec->DBvalue ( "idarticulo" );
        cur = subform->empresaBase() ->cargacursor ( query );
        if ( ! cur->eof() ) {
            if ( "'" + cur->valor ( "preciocostearticulo" ) + "'" == camp->valorcampoprep ( err ) ) {
                delete cur;
                return 0;
            } // end if
        } // end if
        delete cur;

        int ret = QMessageBox::question ( subform, "Pregunta",
                                          "Actualizo precio de coste del articulo..?",
                                          QMessageBox::Yes,
                                          QMessageBox::No );

        if ( ret == QMessageBox::Yes ) {
            QString query1 = "UPDATE articulo SET preciocostearticulo=" + camp->valorcampoprep ( err ) + " where idarticulo=" + rec->DBvalue ( "idarticulo" );
            subform->empresaBase() ->begin();
            subform->empresaBase() ->ejecuta ( query1 );
            subform->empresaBase() ->commit();
        } // end if
        return 0;
    } // end if

    if ( camp->nomcampo() == "codigocompletoarticulo" ) {
        cur = subform->empresaBase() ->cargacursor ( "SELECT * FROM articulo WHERE codigocompletoarticulo = '" + camp->text() + "'" );
        if ( !cur->eof() ) {
            rec->setDBvalue ( "idarticulo", cur->valor ( "idarticulo" ) );
            rec->setDBvalue ( "codigocompletoarticulo", cur->valor ( "codigocompletoarticulo" ) );
            rec->setDBvalue ( "nomarticulo", cur->valor ( "nomarticulo" ) );
            if (  subform->tableName() == "lpedidoproveedor"
                    || subform->tableName() == "lalbaranp"
                    || subform->tableName() == "lfacturap" ) {
                rec->setDBvalue ( "desc" + subform->tableName(), cur->valor ( "nomarticulo" ) );
                rec->setDBvalue ( "cant" + subform->tableName(), "1.00" );
                rec->setDBvalue ( "descuento" + subform->tableName(), "0.00" );
                rec->setDBvalue ( "pvp" + subform->tableName(), cur->valor ( "preciocostearticulo" ) );
            } // end if
        } else {
            mensajeAviso ( subform->tr ( "Articulo inexistente" ) );
            delete cur;
            return -1;
        } // end if

        cur1 = subform->empresaBase() ->cargacursor ( "SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor ( "idtipo_iva" ) + " ORDER BY fechatasa_iva LIMIT 1" );
        if ( !cur->eof() ) {
            if ( subform->tableName() == "lpedidoproveedor"
                    || subform->tableName() == "lalbaranp"
                    || subform->tableName() == "lfacturap" ) {
                rec->setDBvalue ( "iva" + subform->tableName(), cur1->valor ( "porcentasa_iva" ) );


                if ( subform->idproveedor() != "" ) {
                    cur2 = subform->empresaBase() ->cargacursor ( "SELECT recargoeqproveedor FROM proveedor WHERE idproveedor = " + subform->idproveedor() );
                    if ( !cur2->eof() ) {
                        if ( cur2->valor ( "recargoeqproveedor" ) == "t" ) {
                            rec->setDBvalue ( "reqeq" + subform->tableName(), cur1->valor ( "porcentretasa_iva" ) );
                        } // end if
                    } // end if
                    delete cur2;
                } else {
                    rec->setDBvalue ( "reqeq" + subform->tableName(), "0" );
                } // end if

            } // end if
        } // end if
        if ( cur1 != NULL )
            delete cur1;
        if ( cur != NULL )
            delete cur;
    } // end if

    return 0;
}


int ArticuloListSubForm_ArticuloListSubForm_Post ( ArticuloListSubForm *artl )
{
    artl->addSHeader ( "preciocostearticulo", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, artl->tr ( "P.C." ) );
    artl->addSHeader ( "margenarticulo", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, artl->tr ( "Margen" ) );
    artl->addSHeader ( "actualizarmargenarticulo", DBCampo::DBboolean, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, artl->tr ( "Actualizar Margen" ) );
}

