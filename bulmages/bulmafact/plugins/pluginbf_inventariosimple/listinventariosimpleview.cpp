/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C. (Porting to QT4)              *
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

#include <fstream>

#include <QMessageBox>
#include <QWidget>
#include <QComboBox>
#include <QToolButton>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>

#include "listinventariosimpleview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"

///
/**
\param comp
\param parent
**/
ListInventarioSimpleView::ListInventarioSimpleView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    _depura ( "ListInventarioSimpleView::ListInventarioSimpleView", 1 );
    
    setTitleName ( _ ( "InventarioSimple" ) );
    setDbTableName ( "inventariosimple" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_listado->setMainCompany ( comp );
    mui_listado->cargar();
    meteWindow ( windowTitle(), this, FALSE );
    
    _depura ( "END ListInventarioSimpleView::ListInventarioSimpleView", 1 );
}

///
/**
**/
void ListInventarioSimpleView::on_mui_aceptar_released()
{
    _depura ( "ListInventarioSimpleView::on_mui_aceptar_released", 0 );
    
    try {
        mui_listado->guardar();
        close();
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al guardar las cuotas" ) );
    } // end try
    
    _depura ( "END ListInventarioSimpleView::on_mui_aceptar_released", 0 );
}

///
/**
**/
ListInventarioSimpleView::~ListInventarioSimpleView()
{
    _depura ( "ListInventarioSimpleView::~ListInventarioSimpleView", 0 );
    _depura ( "END ListInventarioSimpleView::~ListInventarioSimpleView", 0 );
}

/// ===================================== SUBFORMULARIO ===============================================
///
/**
\param parent
**/
ListInventarioSimpleSubForm::ListInventarioSimpleSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListInventarioSimpleSubForm::ListInventarioSimpleSubForm", 0 );
    
    setDbTableName ( "cuotaporalumno" );
    setDbFieldId ( "numalumnoscuotaporalumno" );
    addSubFormHeader ( "numalumnoscuotaporalumno", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNone, _ ( "Número de alumnos" ) );
    addSubFormHeader ( "cuotacuotaporalumno", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Cuota" ) );
    setInsert ( TRUE );
    
    _depura ( "END ListInventarioSimpleSubForm::ListInventarioSimpleSubForm", 0 );
}

///
/**
**/
void ListInventarioSimpleSubForm::cargar()
{
    _depura ( "ListInventarioSimpleSubForm::cargar", 0 );
    
    BlSubForm::cargar ( "SELECT * FROM cuotaporalumno" );
    
    _depura ( "END ListInventarioSimpleSubForm::cargar", 0 );
}
