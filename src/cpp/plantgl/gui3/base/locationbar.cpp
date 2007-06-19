/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): F. Boudon
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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

#include <qcombobox.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qtoolbutton.h>
#include <qfile.h>
#include <qlistbox.h>
#include <qmainwindow.h>
#include <qcursor.h>
#include <qwhatsthis.h>
#include <qvariant.h>

/*----------------------------------------------------------------------------*/

/* 
 *  Constructs a ViewLocationBar3 which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
ViewLocationBar3::ViewLocationBar3(const QString &label, 
				 QMainWindow *mw, 
#if QT_VERSION >= 300
				 ToolBarDock tbd,
#else
				 QMainWindow::ToolBarDock tbd,
#endif
				 bool newLine, 
				 const char *name)
  : ViewToolBar3( label,mw,tbd,newLine, name  )
{
    if ( !name )
	setName( "LocationBar" );
	setHorizontalStretchable (true);

    QToolButton * bt = NULL;
    QPixmap erase(ViewerIcon3::getPixmap(ViewerIcon3::icon_locerase));
    bt =  new QToolButton(this,"Erase Location");
    bt->setPixmap(erase);
	bt->setFixedSize(QSize(25,25));
	QWhatsThis::add(bt,"<b>"+tr("Erase Location")+"</b><br><br>"
	"Erase the current filename to specify a new one.");
    QLabel * LocationLabel = new QLabel(this, "LocationLabel" );
    LocationLabel->setText( tr( " Location " ) );
	LocationLabel->setFixedSize(QSize(50,25));
    
    __Location = new QComboBox( this, "LocationEdit"  );
	__Location->setProperty("minimumHeight",25);
//	__Location->setProperty("minimumWidth",350);
	__Location->setEditable( TRUE );
	__Location->setAutoCompletion ( TRUE );
	__Location->setCursor( QCursor( 4 ) );

	QWhatsThis::add(__Location,"<b>"+tr("The Filename")+"</b><br><br>"
	"The name of the file of the current scene.");
    if(bt)QObject::connect(bt,SIGNAL(clicked()),__Location,SLOT(clearEdit()));
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ViewLocationBar3::~ViewLocationBar3()
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