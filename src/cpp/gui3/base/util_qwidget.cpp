/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): C. Nouguier & F. Boudon
 *                       N. Dones & B. Adam
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

#include "util_qwidget.h"
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qpainter.h>
#include <qprogressbar.h>
#include <qpen.h>
#include <stdio.h>


/* ----------------------------------------------------------------------- */

ViewToolBar3::ViewToolBar3 ( const QString & label, 
			   QMainWindow * mw, 
#if QT_VERSION >= 300
			   ToolBarDock tbd,
#else
			   QMainWindow::ToolBarDock tbd ,
#endif
			   bool newLine , 
			   const char * name ):
  QToolBar(label,mw,tbd,newLine,name)
{
}

ViewToolBar3::ViewToolBar3 ( const QString & label, 
			   QMainWindow *mw, 
			   QWidget * w, 
			   bool newLine , 
			   const char * name , 
			   WFlags f ) :
  QToolBar(label,mw,w,newLine,name,f)
{
}

ViewToolBar3::ViewToolBar3 ( QMainWindow * parent , const char * name ) :
  QToolBar(parent,name)
{
}

ViewToolBar3::~ViewToolBar3()
{
}

void 
ViewToolBar3::showEvent ( QShowEvent * e)
{
  emit __visibilityChanged(true);
  QToolBar::showEvent(e);
}

void 
ViewToolBar3::hideEvent ( QHideEvent * e)
{
  emit __visibilityChanged(false);
  QToolBar::hideEvent(e);
}

void 
ViewToolBar3::changeVisibility()
{
  if(isVisible())hide();
  else show();
}

/* ----------------------------------------------------------------------- */

ViewExclusiveButtonSet3::ViewExclusiveButtonSet3(unsigned int size,QToolBar * toolbar,const char * name):
  QObject(toolbar,name),
  __i(0),
  __size(size)
{
  __buttonSet = new QToolButton *[size];
}

ViewExclusiveButtonSet3::~ViewExclusiveButtonSet3()
{
  delete [] __buttonSet;
}

void
ViewExclusiveButtonSet3::add(QToolButton * b)
{
  if(__i < __size){
    __buttonSet[__i] = b;
    __i++;
    b->setToggleButton(true);
  }
}

void 
ViewExclusiveButtonSet3::setSelection(const int i)
{
  unsigned int k = i-1;
  for(unsigned int j = 0; j < __i; j++){
    __buttonSet[j]->setOn(j == k);
  }
}

/* ----------------------------------------------------------------------- */

ViewDialog3::ViewDialog3( QWidget * parent, 
	    const char * name, 
	    bool modal, WFlags f ):
  QDialog(parent,name,modal,f)
{
}

ViewDialog3::~ViewDialog3()
{
}

void 
ViewDialog3::show()
{
  if(!isVisible()){
    QDialog::show();
  }
  else {
    if(!isActiveWindow()){
      QDialog::setActiveWindow();
    }
    else{
      QDialog::hide();
    }
  }
}

void 
ViewDialog3::keyPressEvent (QKeyEvent * e)
{
  if(e->key() == Key_Escape) hide();
}

void 
ViewDialog3::hideEvent (QHideEvent * event)
{
  emit visibilityChanged(false);
  QDialog::hideEvent(event);
}

void 
ViewDialog3::showEvent (QShowEvent * event)
{
  emit visibilityChanged(true);
  QDialog::showEvent(event);
}

/* ----------------------------------------------------------------------- */

ViewMainDialog3::ViewMainDialog3( QWidget * parent,			  
				  const char * name, 
				  bool modal, 
				  WFlags f ):
  ViewDialog3(parent,name,modal,f),
	__mainwidget(0){
}
  
  /// Destructor.
ViewMainDialog3::~ViewMainDialog3(){
}

void 
ViewMainDialog3::setMainWidget(QWidget * mainwidget){
  __mainwidget = mainwidget;
}
  
void ViewMainDialog3::resizeEvent(QResizeEvent * e){
  printf("resize event\n");
  if(__mainwidget)__mainwidget->resize(e->size());
}

/* ----------------------------------------------------------------------- */

  ViewPopupButton3::ViewPopupButton3(QPopupMenu * parent, int id, char * name ):
  QObject(parent,name),
  __id(id),
  __menu(parent)
{
  
}


ViewPopupButton3::~ViewPopupButton3()
{
}

void 
ViewPopupButton3::check(bool b)
{
  __menu->setItemChecked(__id,b);
}

/* ----------------------------------------------------------------------- */

ViewStatusBar3::ViewStatusBar3(QWidget * parent, const char * name ):
QStatusBar(parent,name),__progress(new QProgressBar(10,this)){ 
	__progress->setMaximumWidth( 100 );
	__progress->setCenterIndicator(true);
	__progress->setPercentageVisible ( false );
	addWidget(__progress,0,true);
}

ViewStatusBar3::~ViewStatusBar3(){}

QProgressBar * ViewStatusBar3::progressBar(){ return __progress; }

void 
ViewStatusBar3::setProgress(int progress,int total) {
	__progress->setProgress(progress,total);
	if(progress == total)__progress->reset();
}

void 
ViewStatusBar3::setProgress(int progress){
	__progress->setProgress(progress);
}

void 
ViewStatusBar3::setTotalSteps(int total){
	__progress->setTotalSteps(total);
}


/* ----------------------------------------------------------------------- */

void drawArrow3(const QPixmap * pm,int s){
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
  else qWarning(QString(__FILE__)+':'+QString::number(__LINE__)
				+": Cannot draw on pixmap.");
}

/* ----------------------------------------------------------------------- */
