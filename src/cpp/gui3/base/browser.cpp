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

#include "browser.h"
#include "scenegl.h"
#include "interface/qbrowser.h"

#include <qlistview.h>
#include <qpushbutton.h>
#include <qcheckbox.h>

/* ----------------------------------------------------------------------- */



ViewBrowser::ViewBrowser(QWidget * parent,
             const char * name,
             bool modal)
  : ViewDialog(parent,name, modal),
    __scene(NULL),
    __empty(true)
{
  setCaption(tr("Browser"));
#if QT_VERSION >= 220
//  setSizeGripEnabled(TRUE);
#endif
  __browser = new QBrowser(this,"Object List");
  resize(__browser->size());
  QObject::connect(__browser->OkButton,SIGNAL(clicked()),this,SLOT(hide()));
  QObject::connect(__browser->CancelButton,SIGNAL(clicked()),this,SLOT(hide()));
  QObject::connect(this,SIGNAL(visibilityChanged(bool)),this,SLOT(refresh(bool)));
  QObject::connect(__browser->__FullMode,SIGNAL(clicked()),this,SLOT(clear()));
}


ViewBrowser::~ViewBrowser(){
}

void
ViewBrowser::setRenderer(ViewRendererGL * r)
{
  if(__scene!=NULL){
    QObject::disconnect(__browser->__list,SIGNAL(doubleClicked(QListViewItem *)),
                        __scene,SLOT(selectionEvent(QListViewItem *)));
    QObject::disconnect(__scene,SIGNAL(selectionChanged(const QString&)),
                        this,SLOT(setSelection(const QString&)));
  }
  __scene = r;
  if(__scene){
    QObject::connect(__scene,SIGNAL(sceneChanged()),this,SLOT(clear()));
    QObject::connect(__browser->__list,SIGNAL(doubleClicked(QListViewItem *)),
            __scene,SLOT(selectionEvent(QListViewItem *)));
    QObject::connect(__scene,SIGNAL(selectionChanged(const QString&)),
                        this,SLOT(setSelection(const QString&)));
  }
}

void
ViewBrowser::setSelection(const QString& name){
  QListViewItem * i = __browser->__list->firstChild();
  if(i){
    i = i->firstChild();
    if(i){
      bool select = false;
      while(i!=NULL&&!select){
        if(i->text(0) == name)select = true;
        else i = i->nextSibling();
      }
      if(select){
        qDebug("Found Selection.");
        __browser->__list->clearSelection();
        __browser->__list->setSelected(i,true);
        if(isVisible())repaint();
      }
      else qDebug("Selection not Found.");
    }
    else qDebug("First child of root node not found.");
  }
}

void
ViewBrowser::clear()
{
  __browser->__list->clear();
  for(int i = __browser->__list->columns()-1; i >= 0 ; i--){
    __browser->__list->removeColumn(i);
  }
  __empty = true;
  if(isVisible())refresh();
}

void
ViewBrowser::refresh(bool b)
{
  if(__empty && b){
    if(__scene)__scene->browse(__browser->__list,__browser->__FullMode->isChecked() );
    __empty = false;
  }
}

void
ViewBrowser::resizeEvent( QResizeEvent * event)
{
  if(isVisible()){
    int x = event->size().width() - event->oldSize().width();
    int y = event->size().height() - event->oldSize().height();
    __browser->resize( event->size().width(),event->size().height()  );
    __browser->__list->resize(   __browser->__list->size() + QSize(x,y) );
    __browser->OkButton->move(__browser->OkButton->pos()+QPoint(x,y));
    __browser->CancelButton->move(__browser->CancelButton->pos()+QPoint(x,y));
  }
}

void
ViewBrowser::keyPressEvent ( QKeyEvent * e)
{
  if( e->key() == Key_F2 ||
      e->key() == Key_Escape ||
      e->key() == Key_Home) hide();
}

