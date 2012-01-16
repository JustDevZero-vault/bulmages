/***************************************************************************
 *   Copyright (C) 2008 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2012 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
 *                                                                         *
 *   http://www.iglues.org                                                 *
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

#include "blbankedit.h"


int weightControlDigit[] = {6, 3, 7, 9, 10, 5, 8, 4, 2, 1};


///  Inicializa el objeto y hace todas las conexiones necesarias.
/**
\param parent
**/
BlBankEdit::BlBankEdit ( QWidget *parent ) : BlWidget ( parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    QObject::connect ( mui_bankAccount, SIGNAL ( returnPressed() ), this, SLOT ( returnKeyPressed() ) );
    QObject::connect ( mui_bankAccount, SIGNAL ( editingFinished() ), this, SLOT ( bankAccountLostFocus() ) );
    QObject::connect ( mui_bankAccount, SIGNAL ( editingFinished() ), this, SIGNAL ( editingFinished() ) );
    
    /// Establecemos el 'FocusProxy' para que el foco funcione bien.
    setFocusProxy(mui_bank);
    
}


/// No requiere de acciones especiales en el destructor.
/**
**/
BlBankEdit::~BlBankEdit()
{
    BL_FUNC_DEBUG
}


/// Comprueba que los d&iacute;gitos de control son correctos y si no lo son muestra un mensaje de aviso.
/**
**/
void BlBankEdit::checkControlDigit()
{
    BL_FUNC_DEBUG
    QString string1 = mui_bank->text() + mui_bankOffice->text();
    QString string2 = mui_bankAccount->text();
    int controlDigit1 = 0;
    int controlDigit2 = 0;

    /// Si no hay cuenta bancaria puesta entonces no hacemos comprobaciones.
    if ( string1.size() + string2.size() == 0 )
        return;

    int partialValue1 = 0;

    for ( int i = 0; i < 8; i++ ) {
        partialValue1 += string1[8 - i - 1].digitValue() * weightControlDigit[i];
    } // end for

    controlDigit1 = 11 - ( partialValue1 % 11 );
    if ( controlDigit1 == 11 ) controlDigit1 = 0;
    if ( controlDigit1 == 10 ) controlDigit1 = 1;

    int partialValue2 = 0;
    
    for ( int i = 0; i < 10; i++ ) {
        partialValue2 += string2[10 - i - 1].digitValue() * weightControlDigit[i];
    } // end for
    
    controlDigit2 = 11 - ( partialValue2 % 11 );
    if ( controlDigit2 == 11 ) controlDigit2 = 0;
    if ( controlDigit2 == 10 ) controlDigit2 = 1;

    QString controlText = QString::number ( controlDigit1 ) + QString::number ( controlDigit2 );

    /// Si los d&iacute;gitos de control no se corresponden muestra un mensaje de aviso.
    if ( controlText != mui_controlDigit->text() ) {
	blMsgWarning ( _( "Cuenta bancaria incorrecta" ) );
        throw - 1;
    } // end if
    
}


///
/**
\param text
**/
void BlBankEdit::setText ( QString text )
{
    BL_FUNC_DEBUG
    bankAccountLostFocus();
    mui_bank->setText ( text.left ( 4 ) );
    mui_bankOffice->setText ( text.left ( 8 ).right ( 4 ) );
    mui_controlDigit->setText ( text.left ( 10 ).right ( 2 ) );
    mui_bankAccount->setText ( text.left ( 20 ).right ( 10 ) );
    
}


///
/**
\param value
**/
void BlBankEdit::setFieldValue ( QString value )
{
    BL_FUNC_DEBUG
    setText ( value );
}




///
/**
\return
**/
QString BlBankEdit::text()
{
    BL_FUNC_DEBUG
    
    bankAccountLostFocus();
    QString text = mui_bank->text() + mui_bankOffice->text() + mui_controlDigit->text() + mui_bankAccount->text();
    checkControlDigit();
    return text;
}


///
/**
\return
**/
QString BlBankEdit::fieldValue()
{
    BL_FUNC_DEBUG
    return text();
}


///
/**
**/
void BlBankEdit::returnKeyPressed()
{
    BL_FUNC_DEBUG
    bankAccountLostFocus();
    emit returnPressed();
    
}


///
/**
**/
void BlBankEdit::selectAll()
{
    BL_FUNC_DEBUG
    mui_bankAccount->selectAll();
    
}


///
/**
**/
void BlBankEdit::setFocus()
{
    BL_FUNC_DEBUG
    mui_bankAccount->setFocus ( Qt::OtherFocusReason );
}


///
/**
\param text
**/
void BlBankEdit::bankAccountChanged ( const QString &text )
{
    BL_FUNC_DEBUG
    emit ( valueChanged ( mui_bank->text() + mui_bankOffice->text() + mui_controlDigit->text() + mui_bankAccount->text() ) );
}



///
/**
**/
void BlBankEdit::bankAccountLostFocus()
{
    BL_FUNC_DEBUG
    QString text = mui_bank->text() + mui_bankOffice->text() + mui_controlDigit->text() + mui_bankAccount->text();
    if ( text != "" )
        bankAccountChanged ( text );
    
}

