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

#include "geomscenegl.h"


/// GEOM
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/geometry/boundingbox.h>

/// Action
#include <plantgl/algo/base/surfcomputer.h>
#include <plantgl/algo/base/volcomputer.h>
#include <plantgl/algo/base/statisticcomputer.h>
#include <plantgl/algo/base/polygoncomputer.h>
#include "qgeomlistview.h"

/// Viewer
#include "util_qstring.h"
#include "../base/util_qwidget.h"

#include <plantgl/tool/util_string.h>

/// Qt
#include <qpopupmenu.h>
#include <qframe.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qtabwidget.h>
#include <qslider.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <qdragobject.h> 
#include <qfileinfo.h> 

#ifdef QT_THREAD_SUPPORT
#ifndef _DEBUG
#define GEOM_THREAD
#endif
#endif

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace std;
using namespace STDEXT;

/* ----------------------------------------------------------------------- */

real_t
ViewGeomSceneGL3::getSceneSurface()
{
  if(!__scene)return 0;
  SurfComputer _sfc(__discretizer);
  real_t surface = 0;
  if(_sfc.process(__scene))
    surface = _sfc.getSurface();
  return surface;
}

real_t ViewGeomSceneGL3::getSceneVolume()
{
  if(!__scene)return 0;
  VolComputer _vfc(__discretizer);
  real_t volume = 0;
  if(_vfc.process(__scene))
    volume = _vfc.getVolume();
  return volume;
}

real_t
ViewGeomSceneGL3::getSelectionSurface()
{
  if(!__scene)return 0;
  SurfComputer _sfc(__discretizer);
  real_t surface = 0;
  for(hash_map<uint32_t,Shape3DPtr>::iterator _it = __selectedShapes.begin();
  _it !=__selectedShapes.end(); _it++)
	  if(_it->second->apply(_sfc))
		surface += _sfc.getSurface();
  return surface;
}

real_t ViewGeomSceneGL3::getSelectionVolume()
{
  if(!__scene)return 0;
  VolComputer _vfc(__discretizer);
  real_t volume = 0;
  for(hash_map<uint32_t,Shape3DPtr>::iterator _it = __selectedShapes.begin();
  _it !=__selectedShapes.end(); _it++)
	  if(_it->second->apply(_vfc))
		volume += _vfc.getVolume();
  return volume;
}

/* ----------------------------------------------------------------------- */


bool
ViewGeomSceneGL3::addEditEntries(QPopupMenu * menu)
{
  menu->insertItem( tr("Remove Selection"),
                    this,SLOT(removeSelection()),Key_Delete);
  menu->insertItem( tr("Keep Selection Only"),
                    this,SLOT(keepSelectionOnly()));
  menu->insertSeparator();
  QPopupMenu * sub = new QPopupMenu(menu);
  menu->insertItem(tr("Replace Selection by"),sub);
  sub->insertItem( tr("Wire"),
                    this,SLOT(wireSelection()));
  sub->insertItem( tr("Discretization"),
                    this,SLOT(discretizeSelection()));
  sub->insertItem( tr("Triangulation"),
                    this,SLOT(triangulateSelection()));
  menu->insertSeparator();
  ViewModalRendererGL3::addEditEntries(menu);
  return true;
}

bool
ViewGeomSceneGL3::addProperties(QTabWidget * tab)
{
  QWidget * tab2 = new QWidget( tab, "Scene Prop" );
  if(__scene.isValid() && !__scene->isEmpty()){
    real_t surface = getSceneSurface();
    real_t volume = getSceneVolume();
    QFrame * Line = new QFrame( tab2 );
    Line->setGeometry( QRect( 30, 100, 351, 20 ) );
#if QT_VERSION < 300
    Line->setFrameStyle( QFrame::HLine | QFrame::Sunken );
#else
    Line->setFrameShape( QFrame::HLine );
    Line->setFrameShadow( QFrame::Sunken );
#endif

    QLabel * TextLabel = new QLabel( tab2 );
    TextLabel->setGeometry( QRect( 150, 90, 120, 31 ) );
    TextLabel->setText( " "+tr( "General Properties" ) );

    TextLabel = new QLabel( tab2 );
    TextLabel->setGeometry( QRect( 18, 35, 130,31 ) );
    TextLabel->setText( tr( "Number of Element" )+" :" );

    StatisticComputer comp;
    __scene->apply(comp);

    QLineEdit * TextLabel2 = new QLineEdit( tab2 );
    TextLabel2->setReadOnly(true);
    TextLabel2->setAlignment(Qt::AlignHCenter);
    TextLabel2->setGeometry( QRect( 178, 35, 190, 31 ) );
    TextLabel2->setText(  QString::number(comp.getSize())+"  ( "+QString::number(__scene->getSize())+" "+tr("shape(s)")+" )" );

    TextLabel = new QLabel( tab2 );
    TextLabel->setGeometry( QRect( 20, 120, 130, 31 ) );
    TextLabel->setText( tr( "Surface" )+" :" );

    TextLabel = new QLabel( tab2 );
    TextLabel->setGeometry( QRect( 20, 145, 130, 31 ) );
    TextLabel->setText( tr( "Volume")+" :" );

    TextLabel = new QLabel( tab2 );
    TextLabel->setGeometry( QRect( 20, 170, 130, 31 ) );
    TextLabel->setText( tr( "Number of Polygon")+" :" );

    TextLabel = new QLabel( tab2 );
    TextLabel->setGeometry( QRect( 20, 195, 130, 31 ) );
    TextLabel->setText( tr( "Memory Size")+" :" );

    TextLabel2 = new QLineEdit( tab2 );
    TextLabel2->setReadOnly(true);
    TextLabel2->setAlignment(Qt::AlignHCenter);
    TextLabel2->setGeometry( QRect( 170, 120, 200, 25 ) );
    TextLabel2->setText( tr( QString::number(surface) ) );

    TextLabel2 = new QLineEdit( tab2 );
    TextLabel2->setReadOnly(true);
    TextLabel2->setAlignment(Qt::AlignHCenter);
    TextLabel2->setGeometry( QRect( 170, 145, 200, 25 ) );
    TextLabel2->setText( tr( QString::number(volume) ) );

    TextLabel2 = new QLineEdit( tab2 );
    TextLabel2->setReadOnly(true);
    TextLabel2->setAlignment(Qt::AlignHCenter);
    TextLabel2->setGeometry( QRect( 170, 170, 200, 25 ) );
    TextLabel2->setText( tr( QString::number(polygonNumber(__scene) ) ));

    uint32_t s = comp.getMemorySize();
    QString labl;
    if( s/1024 > 0 )
        labl = QString::number(s/1024)+" Kb "+QString::number(s % 1024)+" bytes.";
    else
        labl = QString::number(s)+" bytes.";


    TextLabel2 = new QLineEdit( tab2 );
    TextLabel2->setReadOnly(true);
    TextLabel2->setAlignment(Qt::AlignHCenter);
    TextLabel2->setGeometry( QRect( 170, 195, 200, 25 ) );
    TextLabel2->setText( tr( labl ) );

    Line = new QFrame( tab2  );
    Line->setGeometry( QRect( 20, 250, 351, 20 ) );
#if QT_VERSION < 300
    Line->setFrameStyle( QFrame::HLine | QFrame::Sunken );
#else
    Line->setFrameShape( QFrame::HLine );
    Line->setFrameShadow( QFrame::Sunken );
#endif

    TextLabel = new QLabel( tab2 );
    TextLabel->setGeometry( QRect( 150, 240, 80, 31 ) );
    TextLabel->setText( " "+tr( "Bounding Box" ) );

    TextLabel = new QLabel( tab2 );
    TextLabel->setGeometry( QRect( 20, 280, 130, 31 ) );
    TextLabel->setText( tr( "Upper Rigth Corner")+" :" );

    TextLabel = new QLabel( tab2 );
    TextLabel->setGeometry( QRect( 20, 310, 130, 31 ) );
    TextLabel->setText( tr( "Lower Left Corner")+" :" );

    TextLabel = new QLabel( tab2 );
    TextLabel->setGeometry( QRect( 20, 340, 130, 31 ) );
    TextLabel->setText( tr( "Size")+" :" );

    TextLabel = new QLabel( tab2 );
    TextLabel->setGeometry( QRect( 20, 370, 130, 31 ) );
    TextLabel->setText( tr( "Center")+" :" );

    TextLabel2 = new QLineEdit( tab2 );
    TextLabel2->setReadOnly(true);
    TextLabel2->setAlignment(Qt::AlignHCenter);
    TextLabel2->setGeometry( QRect( 168, 275, 200, 25 ) );
    TextLabel2->setText( tr( toQString( __bbox->getUpperRightCorner()) ) );

    TextLabel2 = new QLineEdit( tab2 );
    TextLabel2->setReadOnly(true);
    TextLabel2->setAlignment(Qt::AlignHCenter);
    TextLabel2->setGeometry( QRect( 168, 305, 200, 25 ) );
    TextLabel2->setText( tr( toQString( __bbox->getLowerLeftCorner()) ) );

    TextLabel2 = new QLineEdit( tab2 );
    TextLabel2->setReadOnly(true);
    TextLabel2->setAlignment(Qt::AlignHCenter);
    TextLabel2->setGeometry( QRect( 168, 335, 200, 25 ) );
    TextLabel2->setText( tr(toQString( __bbox->getSize()*2)  ) );

    TextLabel2 = new QLineEdit( tab2 );
    TextLabel2->setReadOnly(true);
    TextLabel2->setAlignment(Qt::AlignHCenter);
    TextLabel2->setGeometry( QRect( 168, 365, 200, 25 ) );
    TextLabel2->setText( tr( toQString( __bbox->getCenter()) ) );


  }
  else {
    QLabel * TextLabel = new QLabel( tab2 );
    TextLabel->setGeometry( QRect( 150, 90, 120, 31 ) );
    TextLabel->setText( " "+tr( "Empty Scene" ) );
  }

  tab->insertTab( tab2, tr( "Geom &Scene" ) );

  if(!__selectedShapes.empty()){
	  
	  StatisticComputer comp;
	  ScenePtr selection = getSelection();
	  selection->apply(comp);

	  tab2 = new QWidget( tab, "Selection Prop" );
	  real_t surface = getSelectionSurface();
	  real_t volume = getSelectionVolume();
	  QFrame * Line = new QFrame( tab2 );
	  Line->setGeometry( QRect( 30, 100, 351, 20 ) );
#if QT_VERSION < 300
	  Line->setFrameStyle( QFrame::HLine | QFrame::Sunken );
#else
	  Line->setFrameShape( QFrame::HLine );
	  Line->setFrameShadow( QFrame::Sunken );
#endif
	  
	  QLabel * TextLabel = new QLabel( tab2 );
	  TextLabel->setGeometry( QRect( 18, 20, 130,30 ) );
	  TextLabel->setText( tr( "Selection")+" :" );
	  
	  QLineEdit * TextLabel2 = new QLineEdit( tab2 );
	  TextLabel2->setReadOnly(true);
	  TextLabel2->setAlignment(Qt::AlignHCenter);
	  TextLabel2->setGeometry( QRect( 178, 20, 190, 30 ) );
	  hash_map<uint32_t,Shape3DPtr>::const_iterator _it = __selectedShapes.begin();
	  QString listid = QString::number(_it->second->getId()==0?_it->first:_it->second->getId());
	  for(_it++;_it != __selectedShapes.end();_it++)
			listid += ','+QString::number(_it->second->getId()==0?_it->first:_it->second->getId());
		  
	  TextLabel2->setText( listid );

	  TextLabel = new QLabel( tab2 );
	  TextLabel->setGeometry( QRect( 18, 55, 130,30 ) );
	  TextLabel->setText( tr( "Number of Element")+" :" );
	  	  
	  TextLabel2 = new QLineEdit( tab2 );
	  TextLabel2->setReadOnly(true);
	  TextLabel2->setAlignment(Qt::AlignHCenter);
	  TextLabel2->setGeometry( QRect( 178, 55, 190, 30 ) );
	  TextLabel2->setText( QString::number(comp.getSize())+"  ( "+QString::number(selection->getSize())+" "+tr("shape(s)")+" )" );
	  
	  TextLabel = new QLabel( tab2 );
	  TextLabel->setGeometry( QRect( 150, 90, 120, 31 ) );
	  TextLabel->setText( " "+tr( "General Properties" ) );

	  TextLabel = new QLabel( tab2 );
	  TextLabel->setGeometry( QRect( 20, 120, 130, 31 ) );
	  TextLabel->setText( tr( "Surface")+" :" );
	  
	  TextLabel = new QLabel( tab2 );
	  TextLabel->setGeometry( QRect( 20, 145, 130, 31 ) );
	  TextLabel->setText( tr( "Volume")+" :" );
	  
	  TextLabel = new QLabel( tab2 );
	  TextLabel->setGeometry( QRect( 20, 170, 130, 31 ) );
	  TextLabel->setText( tr( "Number of Polygon")+" :" );
	  
	  TextLabel = new QLabel( tab2 );
	  TextLabel->setGeometry( QRect( 20, 195, 130, 31 ) );
	  TextLabel->setText( tr( "Memory Size")+" :" );
	  
	  TextLabel2 = new QLineEdit( tab2 );
	  TextLabel2->setReadOnly(true);
	  TextLabel2->setAlignment(Qt::AlignHCenter);
	  TextLabel2->setGeometry( QRect( 170, 120, 200, 25 ) );
	  TextLabel2->setText( tr( QString::number(surface) ) );
	  
	  TextLabel2 = new QLineEdit( tab2 );
	  TextLabel2->setReadOnly(true);
	  TextLabel2->setAlignment(Qt::AlignHCenter);
	  TextLabel2->setGeometry( QRect( 170, 145, 200, 25 ) );
	  TextLabel2->setText( tr( QString::number(volume) ) );
	  
	  TextLabel2 = new QLineEdit( tab2 );
	  TextLabel2->setReadOnly(true);
	  TextLabel2->setAlignment(Qt::AlignHCenter);
	  TextLabel2->setGeometry( QRect( 170, 170, 200, 25 ) );
	  TextLabel2->setText( tr( QString::number(polygonNumber(selection) ) ));
	  
	  uint32_t s = comp.getMemorySize();
	  QString labl;
	  if( s/1024 > 0 )
		  labl = QString::number(s/1024)+" Kb "+QString::number(s % 1024)+" bytes.";
	  else
		  labl = QString::number(s)+" bytes.";
	  
	  
	  TextLabel2 = new QLineEdit( tab2 );
	  TextLabel2->setReadOnly(true);
	  TextLabel2->setAlignment(Qt::AlignHCenter);
	  TextLabel2->setGeometry( QRect( 170, 195, 200, 25 ) );
	  TextLabel2->setText( tr( labl ) );
	  
	  Line = new QFrame( tab2  );
	  Line->setGeometry( QRect( 20, 250, 351, 20 ) );
#if QT_VERSION < 300
	  Line->setFrameStyle( QFrame::HLine | QFrame::Sunken );
#else
	  Line->setFrameShape( QFrame::HLine );
	  Line->setFrameShadow( QFrame::Sunken );
#endif
	  
	  TextLabel = new QLabel( tab2 );
	  TextLabel->setGeometry( QRect( 150, 240, 80, 31 ) );
	  TextLabel->setText( " " + tr( "Bounding Box" ) );
	  
	  TextLabel = new QLabel( tab2 );
	  TextLabel->setGeometry( QRect( 20, 280, 130, 31 ) );
	  TextLabel->setText( tr( "Upper Rigth Corner")+" :" );
	  
	  TextLabel = new QLabel( tab2 );
	  TextLabel->setGeometry( QRect( 20, 310, 130, 31 ) );
	  TextLabel->setText( tr( "Lower Left Corner")+" :" );
	  
	  TextLabel = new QLabel( tab2 );
	  TextLabel->setGeometry( QRect( 20, 340, 130, 31 ) );
	  TextLabel->setText( tr( "Size")+" :" );
	  
	  TextLabel = new QLabel( tab2 );
	  TextLabel->setGeometry( QRect( 20, 370, 130, 31 ) );
	  TextLabel->setText( tr( "Center")+" :" );
	  
	  BoundingBoxPtr bbox = getSelectionBoundingBox();
	  
	  TextLabel2 = new QLineEdit( tab2 );
	  TextLabel2->setReadOnly(true);
	  TextLabel2->setAlignment(Qt::AlignHCenter);
	  TextLabel2->setGeometry( QRect( 168, 275, 200, 25 ) );
	  if(bbox)TextLabel2->setText( tr( toQString( bbox->getUpperRightCorner()) ) );
	  
	  TextLabel2 = new QLineEdit( tab2 );
	  TextLabel2->setReadOnly(true);
	  TextLabel2->setAlignment(Qt::AlignHCenter);
	  TextLabel2->setGeometry( QRect( 168, 305, 200, 25 ) );
	  if(bbox)TextLabel2->setText( tr( toQString( bbox->getLowerLeftCorner()) ) );
	  
	  TextLabel2 = new QLineEdit( tab2 );
	  TextLabel2->setReadOnly(true);
	  TextLabel2->setAlignment(Qt::AlignHCenter);
	  TextLabel2->setGeometry( QRect( 168, 335, 200, 25 ) );
	  if(bbox)TextLabel2->setText( tr(toQString( bbox->getSize()*2)  ) );
	  
	  TextLabel2 = new QLineEdit( tab2 );
	  TextLabel2->setReadOnly(true);
	  TextLabel2->setAlignment(Qt::AlignHCenter);
	  TextLabel2->setGeometry( QRect( 168, 365, 200, 25 ) );
	  if(bbox)TextLabel2->setText( tr( toQString( bbox->getCenter()) ) );

	  tab->insertTab( tab2, tr( "Selection" ) );
	  
	}
	return true;
}

bool
ViewGeomSceneGL3::browse(QListView * l,bool b)
{
  if(!__scene)return false;
  GeomListViewBuilder3 builder(l);
  builder.setFullMode(b);
  __scene->apply(builder);
  return true;
}

QPopupMenu *
ViewGeomSceneGL3::createToolsMenu(QWidget * parent)
{
  QPopupMenu * menu = ViewModalRendererGL3::createToolsMenu(parent);
  menu->insertSeparator();
  QPopupMenu * __displayMenu = new QPopupMenu(menu);
  __displayMenu->setCheckable(true);
  int id = __displayMenu->insertItem(tr("Enable"),    this,SLOT(changeDisplayListUse()));
  ViewPopupButton3 * b = new ViewPopupButton3(__displayMenu,id);
  QObject::connect(this,SIGNAL(displayList(bool)),b,SLOT(check(bool)));
  b->check(getDisplayListUse());
  __displayMenu->insertSeparator();
  __displayMenu->insertItem(tr("Recompute"),      this,SLOT(clearDisplayList()));
  menu->insertItem(tr("&Display List"),__displayMenu);
  return menu;

}

void 
ViewGeomSceneGL3::clipboard(){
	QClipboard * clipboard = QApplication::clipboard();
	if(clipboard ){
		QMimeSource* data = clipboard->data();
		if(data!=NULL ){
			if(QUriDrag::canDecode(data)){
				QStrList res;
				QUriDrag::decode(data,res);
				if(!res.isEmpty()){
					QFileInfo f(res.at(0));
					QString ext = f.extension();
					ext.upper();
					  if(f.exists()&& (ext == "GEOM" ||ext == "BGEOM")){
						open(res.at(0));
					  }
				}
			}
		}
	}
}

/* ----------------------------------------------------------------------- */

void
ViewMultiGeomSceneGL3::fillToolBar(QToolBar * toolBar)
{
  ViewGeomSceneGL3::fillToolBar(toolBar);
}

  /// Add other toolbar.
bool
ViewMultiGeomSceneGL3::addOtherToolBar(QMainWindow * menu)
{
  __transitionBar = new ViewToolBar3(menu,"Transition Bar");
  QLabel * Label = new QLabel(__transitionBar, "TransitionLabel" );
  Label->setText( " "+tr( "Transition" ) +" ");
   __transSlider =  new QSlider ( 0 , __transitionRenderer.getTotalStep() ,
                                 0, 0, QSlider::Horizontal,
                                 __transitionBar,"Transition Slider" );
  __transSlider->setFixedSize(100,25);
  QObject::connect (__transSlider,SIGNAL(valueChanged(int)),
                    this,SLOT(setRenderingStep(int)) );
  QObject::connect (this,SIGNAL(renderingStepChanged(int)),
                    __transSlider,SLOT(setValue(int)) );
  __transitionBar->hide();
  return true;
}


QPopupMenu *
ViewMultiGeomSceneGL3::createToolsMenu(QWidget * parent)
{
  QPopupMenu * __menu = ViewGeomSceneGL3::createToolsMenu(parent);
  __menu->insertSeparator();
  int id = __menu->insertItem(tr("Transition Slider"),this,SLOT(changeSliderVisibility()));
  ViewPopupButton3 * bt = new ViewPopupButton3(__menu,id,"Transition Slider");
  QObject::connect(this,SIGNAL(sliderVisibilityChanged(bool)),bt,SLOT(check(bool)));
  return __menu;
}

void
ViewMultiGeomSceneGL3::changeSliderVisibility()
{
  if( __transitionBar){
    if( __transitionBar->isVisible()){
      __transitionBar->hide();
      emit sliderVisibilityChanged(false);
    }
    else {
      __transitionBar->show();
      emit sliderVisibilityChanged(true);
    }
  }
}
