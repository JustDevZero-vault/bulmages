/***************************************************************************
                          qlistviewitem1.h  -  description
                             -------------------
    begin                : lun jun 23 2003
    copyright            : (C) 2003 by Tomeu Borr�s Riera
    email                : tborras@conetxia.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QLISTVIEWITEM1_H
#define QLISTVIEWITEM1_H

#include <qlistview.h>

/**
  *@author Tomeu Borr�s Riera
  */

class QListViewItem1 : public QListViewItem  {
public:
   int tipolista;
public: 
	QListViewItem1(QListView *parent): QListViewItem(parent) {};
   QListViewItem1(QListViewItem *parent) : QListViewItem(parent) {};
	~QListViewItem1() {};
   void  paintCell ( QPainter * , const QColorGroup & , int, int ,int  );
   void setTipo(int a) { tipolista = a;};
   void width(const QFontMetrics &, const QListView *, int);
   
};

#endif
