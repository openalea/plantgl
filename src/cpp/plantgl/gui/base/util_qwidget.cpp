/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al. 
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use, 
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info". 
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability. 
 *       
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or 
 *   data to be ensured and,  more generally, to use and operate it in the 
 *   same conditions as regards security. 
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
 *
 *  ----------------------------------------------------------------------------
 */


#include "util_qwidget.h"

#include <QtGlobal>

#include <QtGui/qpen.h>
#include <QtGui/qevent.h>
#include <QtGui/qbrush.h>
#include <QtGui/qpainter.h>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
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

void PGL(drawArrow)(QPixmap * pm,int s){
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

void PGL(fillButton)(QPushButton* button, const QColor& color,const QSize& defaultsize)
{
  if(color.isValid()){
    button->setStyleSheet("background-color: rgb("+QString::number(color.red())+", "+QString::number(color.green())+", "+QString::number(color.blue())+")");
    

    button->update();

  }
}

