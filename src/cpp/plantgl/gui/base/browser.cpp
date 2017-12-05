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

#include <QtGlobal>
#include <QtCore/QEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0) 
    #include <QtWidgets/QScrollArea>
    #include <QtWidgets/QPushButton>
    #include <QtWidgets/QCheckBox>
    #include <QtWidgets/QTreeWidgetItem>
    #include <QtWidgets/QTreeWidget>
#else
    #include <QtGui/QScrollArea>
    #include <QtGui/QPushButton>
    #include <QtGui/QCheckBox>
    #include <QtGui/QTreeWidgetItem>
    #include <QtGui/QTreeWidget>
#endif

#include "browser.h"
#include "scenegl.h"
#include "interface/qbrowser.h"

/* ----------------------------------------------------------------------- */


ViewBrowser::ViewBrowser(QWidget * parent, const QString& title)
  : QDockWidget(title,parent),
    __scene(NULL),
    __empty(true)
{
  setWindowTitle(title);
  setObjectName(title);
  QWidget * mwidget = new QWidget(this);
  setWidget(mwidget);
  __browser = new Ui::QBrowser();
  __browser->setupUi(mwidget);
  QObject::connect(__browser->__FullMode,SIGNAL(clicked()),this,SLOT(clear()));
  __browser->__list->setColumnCount(3);
  __browser->__list->setSortingEnabled(false);
}


ViewBrowser::~ViewBrowser(){
}

void
ViewBrowser::setRenderer(ViewRendererGL * r)
{
  if(__scene!=NULL){
    QObject::disconnect(__browser->__list,SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
                        __scene,SLOT(selectionEvent(QTreeWidgetItem *)));
    QObject::disconnect(__scene,SIGNAL(selectionChanged(const QString&)),
                        this,SLOT(setSelection(const QString&)));
  }

  __scene = r;
  if(__scene){
    QObject::connect(__scene,SIGNAL(sceneChanged()),this,SLOT(clear()));
    QObject::connect(__browser->__list,SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
            __scene,SLOT(selectionEvent(QTreeWidgetItem *)));
    QObject::connect(__scene,SIGNAL(selectionChanged(const QString&)),
                        this,SLOT(setSelection(const QString&)));
  }
}

void
ViewBrowser::setSelection(const QString& name){
  QTreeWidgetItem * root = __browser->__list->topLevelItem(0);
  if(root){
    int chnb = 0;
    QTreeWidgetItem * item = root->child(chnb);
    if(item){
      bool select = false;
      while(chnb < root->childCount() && item!=NULL &&!select){
            if(item->text(0) == name)select = true;
            else ++chnb; item = root->child(chnb);
      }
      if(select){
            qDebug("Found Selection.");
            __browser->__list->clearSelection();
            item->setSelected(true);
            if(isVisible()) __browser->__list->scrollToItem(item);
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
  /*for(int i = __browser->__list->columnCount()-1; i >= 0 ; i--){
    __browser->__list->removeColumn(i);
  }*/
  __empty = true;
  if(isVisible())refresh();
}

void
ViewBrowser::refresh(bool b)
{
  if(__empty && b){
    if(__scene) {
          __scene->browse(__browser->__list,__browser->__FullMode->isChecked() );
    }
    __empty = false;
  }
}

void
ViewBrowser::showEvent( QShowEvent * myevent)
{
	refresh(true);
}

void
ViewBrowser::keyPressEvent ( QKeyEvent * e)
{
	if( e->key() == Qt::Key_F2 ||
      e->key() == Qt::Key_Escape ||
      e->key() == Qt::Key_Home) hide();
}

