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
#include "interface/ui_qbrowser.h"

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

