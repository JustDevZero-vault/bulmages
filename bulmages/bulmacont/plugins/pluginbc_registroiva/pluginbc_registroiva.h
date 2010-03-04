/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include "bcbulmacont.h"
#include "blpostgresqlclient.h"
#include "bccompany.h"
#include "bcasientoform.h"
#include "bcasientosubform.h"
#include "bcasientoview.h"
#include "pdefs.h"


extern "C" MY_EXPORT int entryPoint ( BcBulmaCont * );
extern "C" MY_EXPORT int BcAsientoForm_guardaAsiento1_post ( BcAsientoForm * );
extern "C" MY_EXPORT int empresa_cobPag ( BcCompany * );
extern "C" MY_EXPORT int empresa_registroiva ( BcCompany * );
extern "C" MY_EXPORT int BcAsientoSubForm_boton_iva ( BcAsientoSubForm * );
extern "C" MY_EXPORT int BcAsientoView_BcAsientoView ( BcAsientoView * );

