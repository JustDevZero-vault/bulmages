/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>

#include <fstream>

#include "zview.h"
#include "company.h"
#include "configuracion.h"
#include "plugins.h"

#include "funcaux.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
/**
\param comp
\param parent
**/
ZView::ZView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "ZView::ZView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        /// Lanzamos los plugins.
        if ( g_plugins->lanza ( "ZView_ZView", this ) ) return;

        setTitleName ( tr ( "Cuadre de Caja" ) );
        setDBTableName ( "z" );
        setDBCampoId ( "idz" );
        addDBCampo ( "idz", DBCampo::DBint, DBCampo::DBPrimaryKey, tr ( "ID" ) );
        addDBCampo ( "fechaz", DBCampo::DBdate, DBCampo::DBNotNull, tr ( "Fecha" ) );
        addDBCampo ( "horaz", DBCampo::DBvarchar, DBCampo::DBNotNull  , tr ( "Hora" ) );
        addDBCampo ( "totalz", DBCampo::DBnumeric, DBCampo::DBNotNull  , tr ( "Total" ) );
        addDBCampo ( "numtickets", DBCampo::DBint, DBCampo::DBNotNull  , tr ( "Num. Tickets" ) );
        addDBCampo ( "idalmacen", DBCampo::DBint, DBCampo::DBNothing, tr ( "Id. almacen" ) );
        addDBCampo ( "nomalmacen", DBCampo::DBvarchar, DBCampo::DBNothing, tr ( "Almacen" ) );

        mui_idalmacen->setEmpresaBase ( comp );


        mui_list->setEmpresaBase ( comp );
        mui_list->setDBTableName ( "albaran" );
        mui_list->setDBCampoId ( "idalbaran" );
        mui_list->addSHeader ( "idalbaran",  DBCampo::DBint, DBCampo::DBNoSave,     SHeader::DBNoWrite,                         tr ( "Id. Albaran" ) );
        mui_list->addSHeader ( "numalbaran",  DBCampo::DBvarchar, DBCampo::DBNoSave,     SHeader::DBNoWrite,                         tr ( "Numero" ) );
        mui_list->addSHeader ( "descalbaran",  DBCampo::DBvarchar, DBCampo::DBNoSave,     SHeader::DBNoWrite,                         tr ( "Descripcion" ) );
        mui_list->addSHeader ( "refalbaran",  DBCampo::DBvarchar, DBCampo::DBNoSave,     SHeader::DBNoWrite,                         tr ( "Referencia" ) );
        mui_list->addSHeader ( "fechaalbaran",  DBCampo::DBdate, DBCampo::DBNoSave,     SHeader::DBNoWrite,                         tr ( "Fecha" ) );
        mui_list->addSHeader ( "procesadoalbaran",  DBCampo::DBboolean, DBCampo::DBNoSave,     SHeader::DBNoWrite,                         tr ( "Procesado" ) );
        mui_list->addSHeader ( "anuladoalbaran",  DBCampo::DBboolean, DBCampo::DBNoSave,     SHeader::DBNoWrite,                         tr ( "Anulado" ) );
        mui_list->addSHeader ( "horaalbaran",  DBCampo::DBvarchar, DBCampo::DBNoSave,     SHeader::DBNoWrite,                         tr ( "Hora" ) );
        mui_list->addSHeader ( "totalalbaran",  DBCampo::DBnumeric, DBCampo::DBNoSave,     SHeader::DBNoWrite,                         tr ( "Total" ) );
        mui_list->addSHeader ( "bimpalbaran",  DBCampo::DBnumeric, DBCampo::DBNoSave,     SHeader::DBNoWrite,                         tr ( "Base Imponible" ) );
        mui_list->addSHeader ( "impalbaran",  DBCampo::DBnumeric, DBCampo::DBNoSave,     SHeader::DBNoWrite,                         tr ( "Impuestos" ) );


        mui_list->setinsercion ( FALSE );
        mui_list->setDelete ( FALSE );
        mui_list->setSortingEnabled ( TRUE );

        dialogChanges_cargaInicial();
        meteWindow ( windowTitle(), this, FALSE );
    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al crear el almacen" ) );
    } // end try
    _depura ( "END ZView::ZView", 0 );
}


/// No precisa acciones adicionales en el destructor.

/**
**/
ZView::~ZView()
{
    _depura ( "ZView::~ZView", 0 );
    _depura ( "END ZView::~ZView", 0 );
}


///
/**
\param idz
\return
**/
int ZView::cargarPost ( QString idz )
{
    mui_list->cargar ( "SELECT * FROM albaran WHERE idz=" + idz );
    return 0;
}


