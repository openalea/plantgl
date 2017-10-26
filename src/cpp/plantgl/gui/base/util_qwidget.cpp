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

#include "util_qwidget.h"

#include <QtGlobal>

#include <QtGui/qpen.h>
#include <QtGui/qevent.h>
#include <QtGui/qbrush.h>
#include <QtGui/qpainter.h>

#if QT_VERSION >= 0x050000 
    #include <QtWidgets/qtoolbutton.h>
    #include <QtWidgets/qmenu.h>
    #include <QtWidgets/qprogressbar.h>
    #include <QtWidgets/qpushbutton.h>
#else
    #include <QtGui/qtoolbutton.h>
    #include <QtGui/qmenu.h>
    #include <QtGui/qprogressbar.h>
    #include <QtGui/qpushbutton.h>
#endif

#include <stdio.h>


/* ----------------------------------------------------------------------- */

ViewToolBar::ViewToolBar ( const QString & label, QWidget * mw, const char* name ):
  QToolBar(label,mw)
{
	if(name)setObjectName(name);
}


ViewToolBar::~ViewToolBar()
{
}

void 
ViewToolBar::showEvent ( QShowEvent * e)
{
  emit __visibilityChanged(true);
  QToolBar::showEvent(e);
}

void 
ViewToolBar::hideEvent ( QHideEvent * e)
{
  emit __visibilityChanged(false);
  QToolBar::hideEvent(e);
}

void 
ViewToolBar::changeVisibility()
{
  if(isVisible())hide();
  else show();
}

/* ----------------------------------------------------------------------- */

ViewDialog::ViewDialog( QWidget * parent, const char * name, bool modal, Qt::WindowFlags f ):
  QDialog(parent,f)
{
	if(name)setObjectName(name);
	setModal(modal);
}

ViewDialog::~ViewDialog()
{
}

void 
ViewDialog::show()
{
  if(!isVisible()){
    QDialog::show();
  }
  else {
    if(!isActiveWindow()){
		activateWindow();
    }
    else{
      QDialog::hide();
    }
  }
}

void 
ViewDialog::keyPressEvent (QKeyEvent * e)
{
  if(e->key() == Qt::Key_Escape) hide();
}

void 
ViewDialog::hideEvent (QHideEvent * event)
{
  emit visibilityChanged(false);
  QDialog::hideEvent(event);
}

void 
ViewDialog::showEvent (QShowEvent * event)
{
  emit visibilityChanged(true);
  QDialog::showEvent(event);
}

/* ----------------------------------------------------------------------- */

ViewMainDialog::ViewMainDialog( QWidget * parent, const char * name, bool modal, Qt::WindowFlags f ):
  ViewDialog(parent,name,modal,f),
	__mainwidget(0){
}
  
  /// Destructor.
ViewMainDialog::~ViewMainDialog(){
}

void 
ViewMainDialog::setMainWidget(QWidget * mainwidget){
  __mainwidget = mainwidget;
}
  
void ViewMainDialog::resizeEvent(QResizeEvent * e){
  if(__mainwidget)__mainwidget->resize(e->size());
}

/* ----------------------------------------------------------------------- */

ViewStatusBar::ViewStatusBar(QWidget * parent, const char * name ):
QStatusBar(parent),__progress(new QProgressBar(this)){
	setObjectName(name);
	__progress->setMaximumWidth( 100 );
	// __progress->setCenterIndicator(true);
	__progress->setTextVisible ( false );
	addPermanentWidget( __progress,0 );

}

ViewStatusBar::~ViewStatusBar(){}

QProgressBar * ViewStatusBar::progressBar(){ return __progress; }

void 
ViewStatusBar::setProgress(int progress,int total) {
	__progress->setMaximum(total);
	__progress->setValue(progress);
	if(progress == total)__progress->reset();
}

void 
ViewStatusBar::setProgress(int progress){
	__progress->setValue(progress);
}

void 
ViewStatusBar::setTotalSteps(int total){
	__progress->setMaximum(total);
}


/* ----------------------------------------------------------------------- */

void PGL::drawArrow(QPixmap * pm,int s){
  QPainter p;
  if(p.begin(pm)){
	int x = p.window().width() - 4 -4*s;
	int y = p.window().height() - 4*s;
	p.setPen( QPen(Qt::black,1 ) );
	{
	  for(int j = 0; j < 4*s; j++)
		for(int i = 0; i < 3+4*s-2*j; i++){
		  p.drawPoint(QPoint(x+j+i,y+j));
		}
	}
	p.end();
  }
  else qWarning("%s:%d: Cannot draw on pixmap.", __FILE__,__LINE__);
}

/* ----------------------------------------------------------------------- */

void PGL::fillButton(QPushButton* button, const QColor& color,const QSize& defaultsize)
{
  if(color.isValid()){
    QPalette palette = button->palette();
    QBrush brush(color);
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Button, brush);
    button->setPalette(palette);

    QPixmap pixmap(defaultsize);
    pixmap.fill(color);
    button->setIcon(pixmap);
  }
}

