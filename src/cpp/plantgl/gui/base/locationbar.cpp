/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
 *
 *  ----------------------------------------------------------------------------
 * 
 *                      GNU General Public Licence
 *           
 *       This program is free software; you can redistribute it and/or
 *       modify it under the terms of the GNU General Public License as
 *       published by the Free Software Foundation; either version 2 of
 *       the License, or (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public
 *       License along with this program; see the file COPYING. If not,
 *       write to the Free Software Foundation, Inc., 59
 *       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  ----------------------------------------------------------------------------
 */				
#include "locationbar.h"
#include "icons.h"

#include <QtGui/qcombobox.h>
#include <QtGui/qtoolbutton.h>
#include <QtGui/qlabel.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qcursor.h>
#include <QtGui/qcursor.h>
#include <QtGui/QHBoxLayout>

/*----------------------------------------------------------------------------*/

/* 
 *  Constructs a ViewLocationBar which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
ViewLocationBar::ViewLocationBar(const QString &label, 
				                 QWidget *mw,const char *name)
  : ViewToolBar( label, mw ,name )
{


    QToolButton * bt = NULL;
    QPixmap erase(ViewerIcon::getPixmap(ViewerIcon::locerase));
    bt =  new QToolButton(this);
    bt->setIcon(QIcon(erase));
	bt->setFixedSize(QSize(25,25));
	bt->setWhatsThis("<b>"+tr("Erase Location")+"</b><br><br>"
	"Erase the current filename to specify a new one.");
    addWidget(bt);

    // QLabel * LocationLabel = new QLabel(this );
    // LocationLabel->setText( tr( " Location " ) );
	// LocationLabel->setFixedSize(QSize(50,25));
    // addWidget(LocationLabel);

    __Location = new QComboBox( this );
	__Location->setProperty("minimumHeight",25);
	//__Location->setProperty("minimumWidth",50);
	__Location->setEditable( TRUE );
	__Location->setAutoCompletion ( TRUE );
	__Location->setCursor( QCursor( Qt::IBeamCursor ) );
	__Location->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

	addWidget(__Location);

	__Location->setWhatsThis("<b>"+tr("The Filename")+"</b><br><br>"
	"The name of the file of the current scene.");
    if(bt)QObject::connect(bt,SIGNAL(clicked()),__Location,SLOT(clearEditText()));
 }

/*  
 *  Destroys the object and frees any allocated resources
 */
ViewLocationBar::~ViewLocationBar()
{
    // no need to delete child widgets, Qt does it all for us
}

/*----------------------------------------------------------------------------*/

/*
ViewComboBox::ViewComboBox( QWidget * parent, const char * name ):
  QComboBox(parent,name)
{
}

ViewComboBox::~ViewComboBox()
{
}

void ViewComboBox::focusInEvent ( QFocusEvent *e )
{
//  setEditable( TRUE );
}

void ViewComboBox::focusOutEvent ( QFocusEvent *e )
{
//  setEditable( FALSE );
}

void ViewComboBox::mousePressEvent( QMouseEvent*e)
{
//  setEditable( TRUE );
  QComboBox::mousePressEvent(e);
}


void ViewComboBox::clearfocus()
{
  clearFocus();
//  setEditable( FALSE );
}
 
void ViewComboBox::clearedit()
{
  clearEdit();
//  setEditable( TRUE );
}
 
*/
