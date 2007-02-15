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

#include "qobjectbrowser.h"
#include "interface/qbrowser.h"
#include "icons.h"

#include <qlistview.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qpixmap.h>
#include <qwidgetlist.h>
#include <qwidgetintdict.h>
#include <qobjectlist.h>
#include <qobjectdict.h>

#include <qapplication.h>
#include <qmetaobject.h>
#include <qstrlist.h>
#include <qbitmap.h>
#include <qcursor.h>


/* ----------------------------------------------------------------------- */

class QObjectListBuilder {
  
public :
  QObjectListBuilder (QListView * qListView): __current(NULL),
	__pixgeom(ViewerIcon::icon_geometry),
	__pixappe(ViewerIcon::icon_appearance),
	__pixshape(ViewerIcon::icon_shape),
	__pixtransf(ViewerIcon::icon_transformed),
	__pixatt(ViewerIcon::icon_attribut),
	__pixattptr(ViewerIcon::icon_attributptr){
	qListView->addColumn( "Name" );
	qListView->addColumn( "Value" );
	qListView->addColumn( "Type" );
	qListView->setTreeStepSize( 20 );
	qListView->setSorting(-1,TRUE);
	__current = new QListViewItem(qListView,"Root","Qt Workspace");
	__current->setPixmap(0,__pixgeom);
	__current->setOpen(TRUE);
  }
  
  void build(){
	//           QListViewItem * cur = __current;
	QListViewItem * qt_list = new QListViewItem(__current,"Basic Qt Object");
	qt_list->setPixmap(0,__pixattptr);
	QListViewItem * other_list = new QListViewItem(__current,"Custom Qt Object");
	other_list->setPixmap(0,__pixattptr);
	const QObjectList * roots = QObject::objectTrees();
	if(roots != NULL){
	  uint size = roots->count();
	  __current->setText(2,"List<QObject>["+QString::number(size)+"]");
	  QObjectList r(*roots);
	  for(uint i = 0; i < size; i++ ){
		QObject * obj = r.at(i);
		if(obj != NULL){
		  if(obj->className()[0] == 'Q')
			__current = qt_list;
		  else __current = other_list;
		  process(obj);
		}
	  }
	}
  }
  
  void process(const QObject * obj){
	if(obj != NULL){
	  QListViewItem * buf = __current;
	  __current = new QListViewItem(__current,obj->className(),QString(obj->name()));
	  __current->setPixmap(0,__pixgeom);
	  QMetaObject* _m = obj->metaObject();
	  QListViewItem * att = NULL;
	  if(_m != NULL){
		QString _superclass(_m->superClassName());
		if(!_superclass.isEmpty()){
		  att = new QListViewItem(__current,"Inherit",_superclass);
		  att->setPixmap(0,__pixattptr);
		  QMetaObject* _meta = _m->superClass();
		  QListViewItem * att4 = NULL;
		  while((_meta = _meta->superClass())!= NULL){
			att4 = new QListViewItem(att,att4,QString(_meta->className()));
			att4->setPixmap(0,__pixatt);
		  }
		}
		att = new QListViewItem(__current,att,"Priority",(obj->highPriority()?"High":"Normal"));
		att->setPixmap(0,__pixatt);
		att = new QListViewItem(__current,att,"Widget",(obj->isWidgetType()?"True":"False"));
		att->setPixmap(0,__pixatt);
		QStrList _slots = _m->slotNames(true);
		if(!_slots.isEmpty()){
		  att = new QListViewItem(__current,att,"Slots");
		  att->setPixmap(0,__pixtransf);
		  uint sl_size = _slots.count();
		  QListViewItem * att2 = NULL;
		  for(uint j = 0; j < sl_size; j++ ){
			att2 = new QListViewItem(att,att2,_slots.at(j));
			att2->setPixmap(0,__pixatt);
		  }
		}
		QStrList _signals = _m->signalNames(true);
		if(!_signals.isEmpty()){
		  att = new QListViewItem(__current,att,"Signals");
		  att->setPixmap(0,__pixtransf);
		  uint si_size = _signals.count();
		  QListViewItem * att2 = NULL;
		  for(uint j = 0; j < si_size; j++ ){
			att2 = new QListViewItem(att,att2,_signals.at(j));
			att2->setPixmap(0,__pixatt);
		  }
		}
		int numCInfo = _m->numClassInfo(true);
		if(numCInfo !=0){
		  att = new QListViewItem(__current,att,"ClassInfo","List<Info>["+QString::number(numCInfo)+']');
		  att->setPixmap(0,__pixtransf);
		  QListViewItem * att2 = NULL;
		  for(int j = 0; j < numCInfo; j++ ){
			const QClassInfo * _inf = _m->classInfo(j);
			if(_inf != NULL){
			  att2 = new QListViewItem(att,att2,QString(_inf->name),QString(_inf->value));
			  att2->setPixmap(0,__pixatt);
			}
		  }
		}
		QStrList _props = _m->propertyNames(true);
		if(!_props.isEmpty()){
		  att = new QListViewItem(__current,att,"Properties");
		  att->setPixmap(0,__pixtransf);
		  uint p_size = _props.count();
		  QListViewItem * att2 = NULL;
		  for(uint j = 0; j < p_size; j++ ){
			att2 = new QListViewItem(att,att2,_props.at(j));
			att2->setPixmap(0,__pixatt);
			
			QVariant val;
			QString propname(_props.at(j));
#if QT_VERSION >= 300
			const QMetaProperty*  prop = _m->property (j,true);
#else
			const QMetaProperty*  prop = _m->property (propname,true);
#endif
			QString proptype;
			if(prop){
			  proptype = prop->type();
			  att2->setText(2,proptype);
			  /*
			  QListViewItem * att3 = new QListViewItem(att2,"Writable",(prop->writable()?"True":"False"));
			  att3->setPixmap(0,__pixatt);
			  att3 = new QListViewItem(att2,att3,"Designable",(prop->designable()?"True":"False"));
			  att3->setPixmap(0,__pixatt);
			  */
			}
			
			val = obj->property(propname);
			
			if(!val.isValid())att2->setText(1,"Invalid");
			else if(prop->isEnumType()){
			  att2->setText(1,prop->valueToKey(val.toInt()));
			}
			else if(prop->isSetType()){
			  QStrList st = prop->valueToKeys(val.toInt());
			  QString t = st.at(0);
			  for(uint i= 1; i < st.count(); i++)t+='/'+st.at(i);
			  att2->setText(1,t);
			}
			else if(val.type() == QVariant::String)att2->setText(1,'"'+val.toString()+'"');
			else if(val.type() == QVariant::CString)att2->setText(1,'"'+val.toCString()+'"');
			else if(val.type() == QVariant::Bool){
			  if(val.toBool())att2->setText(1,"True");
			  else att2->setText(1,"False");
			}
			else if(val.type() == QVariant::Int)att2->setText(1,QString::number(val.toInt()));
			else if(val.type() == QVariant::UInt)att2->setText(1,QString::number(val.toUInt()));
			else if(val.type() == QVariant::Double)att2->setText(1,QString::number(val.toDouble()));
			else if(val.type() == QVariant::Rect){
			  const QRect r = val.toRect();
			  att2->setText(1,'[' + QString::number(r.left())  + ',' + QString::number(r.top())+
				',' + QString::number(r.right()) + ',' + QString::number(r.bottom())+']');
			}
			else if(val.type() == QVariant::Region){
			  const QRegion reg = val.toRegion();
			  QRect r = reg.boundingRect();
			  att2->setText(1,'[' + QString::number(r.left())  + ',' + QString::number(r.top())+
				',' + QString::number(r.right()) + ',' + QString::number(r.bottom())+"],");
			}
			else if(val.type() == QVariant::Size){
			  const QSize s = val.toSize();
			  att2->setText(1,'[' + QString::number(s.width())  + ',' + QString::number(s.height())+']');
			}
			else if(val.type() == QVariant::Point){
			  const QPoint p = val.toPoint();
			  att2->setText(1,'[' + QString::number(p.x())  + ',' + QString::number(p.y())+']');
			}
			else if(val.type() == QVariant::Color){
			  const QColor c = val.toColor();
			  att2->setText(1,'[' + QString::number(c.red())   + ',' +
				QString::number(c.green()) + ',' +
				QString::number(c.blue())  + ']');
			}
			else if(val.type() == QVariant::ColorGroup){
			  const QColorGroup cg = val.toColorGroup();
			  QColor c  = cg.base();
			  att2->setText(1,'[' + QString::number(c.red())   + ',' +
				QString::number(c.green()) + ',' +
				QString::number(c.blue())  + "], ...");
			}
			else if(val.type() == QVariant::Font){
			  const QFont f = val.toFont();
			  QString text = '\'' + f.family()   + "', " + QString::number(f.pointSize())
				+ ", " + QString::number(f.weight());
			  if(f.italic())text+=", italic";
			  att2->setText(1,text);
			}
			else if(val.type() == QVariant::SizePolicy){
			  QSizePolicy sp = val.toSizePolicy();
			  QString text;
			  if(sp.horData() == QSizePolicy::Fixed)text+="Fixed";
			  else if(sp.horData() == QSizePolicy::Minimum )text+="Minimum";
			  else if(sp.horData() == QSizePolicy::Maximum )text+="Maximum";
			  else if(sp.horData() == QSizePolicy::Preferred )text+="Preferred";
			  else if(sp.horData() == QSizePolicy::MinimumExpanding )text+="MinimumExpanding";
			  else if(sp.horData() == QSizePolicy::Expanding )text+="Expanding";
			  text +='/';
			  if(sp.verData() == QSizePolicy::Fixed)text+="Fixed";
			  else if(sp.verData() == QSizePolicy::Minimum )text+="Minimum";
			  else if(sp.verData() == QSizePolicy::Maximum )text+="Maximum";
			  else if(sp.verData() == QSizePolicy::Preferred )text+="Preferred";
			  else if(sp.verData() == QSizePolicy::MinimumExpanding )text+="MinimumExpanding";
			  else if(sp.verData() == QSizePolicy::Expanding )text+="Expanding";
			  
			  att2->setText(1,text);
			}
			else if(val.type() == QVariant::Pixmap){
			  QPixmap pix = val.toPixmap();
			  if(!pix.isNull())att2->setPixmap(1,pix);
			}
			else if(val.type() == QVariant::Cursor){
			  const QCursor cur = val.toCursor();
			  const QBitmap * pix = cur.bitmap();
			  if(pix && !pix->isNull())att2->setPixmap(1,*pix);
			  else att2->setText(1,QString::number(cur.shape()));
			}
		  }
		}
       }

	   const QObjectList * roots = obj->children();
	   if(roots != NULL){
		 __current = new QListViewItem(__current,att,"children","ptr="+QString::number((unsigned long)roots),
		   "List<QObject>["+QString::number(roots->count())+"]");
		 __current->setPixmap(0,__pixappe);
		 QObjectList r(*roots);
		 uint size = r.count();
		 for(uint i = 0; i < size; i++ ){
		   QObject * _obj = r.at(i);
		   process(_obj);
		 }
		 
	   }
	   __current = buf;
       }
    }
		
protected :
  
  QListViewItem * __current;
  QPixmap __pixgeom;
  QPixmap __pixappe;
  QPixmap __pixshape;
  QPixmap __pixtransf;
  QPixmap __pixatt;
  QPixmap __pixattptr;
};

/* ----------------------------------------------------------------------- */



ViewQObjectBrowser::ViewQObjectBrowser(QWidget * parent,
									   const char * name,
									   bool modal)
									   : ViewDialog(parent,name, modal)
{
  setCaption("QObject Browser");
#if QT_VERSION >= 220
  setSizeGripEnabled(TRUE);
#endif
  __browser = new QBrowser(this,"QObject List");
  resize(__browser->size());
  QObject::connect(__browser->OkButton,SIGNAL(clicked()),this,SLOT(hide()));
  QObject::connect(__browser->CancelButton,SIGNAL(clicked()),this,SLOT(hide()));
  __browser->__FullMode->hide();
  QObjectListBuilder b(__browser->__list);
  b.build();
  
}


ViewQObjectBrowser::~ViewQObjectBrowser(){
}

void
ViewQObjectBrowser::resizeEvent( QResizeEvent * event)
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
ViewQObjectBrowser::keyPressEvent ( QKeyEvent * e)
{
  if( e->key() == Key_Escape ||
	e->key() == Key_Home) hide();
}

