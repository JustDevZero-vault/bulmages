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

#include <fstream>

#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QFile>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>
#include <QCloseEvent>

#include "rutacomercialview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"
#include "blapplication.h"
#include "bfbulmafact.h"

#define coma "'"


///
/**
\param comp
\param parent
**/
RutaComercialView::RutaComercialView ( BfCompany *comp, QWidget *parent )
        :  BfForm ( comp, parent )
{
    blDebug ( Q_FUNC_INFO, 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );

    setTitleName ( _ ( "Ruta comercial" ) );
    setDbTableName ( "rutacomercial" );
    setDbFieldId ( "idrutacomercial" );
    addDbField ( "idrutacomercial", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Identificador" ) );
    addDbField ( "fecharutacomercial", BlDbField::DbDate, BlDbField::DbNotNull, _ ( "Fecha" ) );
    addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Familia" ) );
    addDbField ( "comentariosrutacomercial", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios" ) );
    addDbField ( "horarutacomercial", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Hora" ) );
    addDbField ( "refrutacomercial",  BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia" ) );

    
    /// Establecemos los parametros de busqueda del Cliente
    mui_idcliente->setMainCompany ( comp );
    mui_idcliente->setLabel ( _ ( "Cliente:" ) );
    mui_idcliente->setTableName ( "cliente" );
    mui_idcliente->m_valores["cifcliente"] = "";
    mui_idcliente->m_valores["nomcliente"] = "";

    pintar();
    dialogChanges_readValues();
    blScript(this);
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param parent
**/

RutaComercialView::RutaComercialView ( QWidget *parent )
        : BfForm ( ((BfBulmaFact *) g_main)->company(), parent )
{
    blDebug ( Q_FUNC_INFO, 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );

    setTitleName ( _ ( "Ruta comercial" ) );
    setDbTableName ( "rutacomercial" );
    setDbFieldId ( "idrutacomercial" );
    addDbField ( "idrutacomercial", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Identificador" ) );
    addDbField ( "fecharutacomercial", BlDbField::DbDate, BlDbField::DbNotNull, _ ( "Fecha" ) );
    addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Familia" ) );
    addDbField ( "comentariosrutacomercial", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios" ) );
    addDbField ( "horarutacomercial", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Hora" ) );
    addDbField ( "refrutacomercial",  BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia" ) );

    
    /// Establecemos los parametros de busqueda del Cliente
    mui_idcliente->setMainCompany ( ((BfBulmaFact *) g_main)->company() );
    mui_idcliente->setLabel ( _ ( "Cliente:" ) );
    mui_idcliente->setTableName ( "cliente" );
    mui_idcliente->m_valores["cifcliente"] = "";
    mui_idcliente->m_valores["nomcliente"] = "";

    pintar();
    dialogChanges_readValues();
    blScript(this);
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
RutaComercialView::~RutaComercialView()
{
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


