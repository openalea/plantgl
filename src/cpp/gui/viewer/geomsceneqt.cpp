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
#include <scenegraph/scene/shape.h>
#include <scenegraph/geometry/boundingbox.h>

/// Action
#include <algo/base/surfcomputer.h>
#include <algo/base/volcomputer.h>
#include <algo/base/statisticcomputer.h>
#include <algo/base/polygoncomputer.h>
#include "qgeomlistview.h"

/// Viewer
#include "util_qstring.h"
#include "../base/util_qwidget.h"

#include <tool/util_string.h>

/// Qt
#include <qmenu.h>
#include <qframe.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qtabwidget.h>
#include <qslider.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <qfileinfo.h> 
#include <qmainwindow.h> 
#include <qmimedata.h> 
#include <qurl.h> 

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
ViewGeomSceneGL::getSceneSurface()
{
  if(!__scene)return 0;
  SurfComputer _sfc(__discretizer);
  real_t surface = 0;
  if(_sfc.process(__scene))
    surface = _sfc.getSurface();
  return surface;
}

real_t ViewGeomSceneGL::getSceneVolume()
{
  if(!__scene)return 0;
  VolComputer _vfc(__discretizer);
  real_t volume = 0;
  if(_vfc.process(__scene))
    volume = _vfc.getVolume();
  return volume;
}

real_t
ViewGeomSceneGL::getSelectionSurface()
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

real_t ViewGeomSceneGL::getSelectionVolume()
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
ViewGeomSceneGL::addEditEntries(QMenu * menu)
{
  menu->addAction( tr("Remove Selection"),
	  this,SLOT(removeSelection()),Qt::Key_Delete);
  menu->addAction( tr("Keep Selection Only"),
                    this,SLOT(keepSelectionOnly()));
  menu->addSeparator();
  QMenu * sub = new QMenu(menu);
  sub->setTitle(tr("Replace Selection by"));
  menu->addMenu(sub);
  sub->addAction( tr("Wire"),
                    this,SLOT(wireSelection()));
  sub->addAction( tr("Discretization"),
                    this,SLOT(discretizeSelection()));
  sub->addAction( tr("Triangulation"),
                    this,SLOT(triangulateSelection()));
  menu->addSeparator();
  ViewModalRendererGL::addEditEntries(menu);
  return true;
}

bool
ViewGeomSceneGL::addProperties(QTabWidget * tab)
{
  QWidget * tab2 = new QWidget( tab );
  if(__scene.isValid() && !__scene->isEmpty()){
    real_t surface = getSceneSurface();
    real_t volume = getSceneVolume();
    QFrame * Line = new QFrame( tab2 );
    Line->setGeometry( QRect( 30, 100, 351, 20 ) );
    Line->setFrameShape( QFrame::HLine );
    Line->setFrameShadow( QFrame::Sunken );

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
    TextLabel2->setText( QString::number(surface) );

    TextLabel2 = new QLineEdit( tab2 );
    TextLabel2->setReadOnly(true);
    TextLabel2->setAlignment(Qt::AlignHCenter);
    TextLabel2->setGeometry( QRect( 170, 145, 200, 25 ) );
    TextLabel2->setText( QString::number(volume) );

    TextLabel2 = new QLineEdit( tab2 );
    TextLabel2->setReadOnly(true);
    TextLabel2->setAlignment(Qt::AlignHCenter);
    TextLabel2->setGeometry( QRect( 170, 170, 200, 25 ) );
    TextLabel2->setText( QString::number(polygonNumber(__scene) ));

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
    TextLabel2->setText( labl );

    Line = new QFrame( tab2  );
    Line->setGeometry( QRect( 20, 250, 351, 20 ) );
    Line->setFrameShape( QFrame::HLine );
    Line->setFrameShadow( QFrame::Sunken );

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
    TextLabel2->setText( toQString( __bbox->getUpperRightCorner()) );

    TextLabel2 = new QLineEdit( tab2 );
    TextLabel2->setReadOnly(true);
    TextLabel2->setAlignment(Qt::AlignHCenter);
    TextLabel2->setGeometry( QRect( 168, 305, 200, 25 ) );
    TextLabel2->setText( toQString( __bbox->getLowerLeftCorner()) );

    TextLabel2 = new QLineEdit( tab2 );
    TextLabel2->setReadOnly(true);
    TextLabel2->setAlignment(Qt::AlignHCenter);
    TextLabel2->setGeometry( QRect( 168, 335, 200, 25 ) );
    TextLabel2->setText( toQString( __bbox->getSize()*2)  );

    TextLabel2 = new QLineEdit( tab2 );
    TextLabel2->setReadOnly(true);
    TextLabel2->setAlignment(Qt::AlignHCenter);
    TextLabel2->setGeometry( QRect( 168, 365, 200, 25 ) );
    TextLabel2->setText( toQString( __bbox->getCenter()) );


  }
  else {
    QLabel * TextLabel = new QLabel( tab2 );
    TextLabel->setGeometry( QRect( 150, 90, 120, 31 ) );
    TextLabel->setText( " "+tr( "Empty Scene" ) );
  }

  tab->addTab( tab2, tr( "Geom &Scene" ) );

  if(!__selectedShapes.empty()){
	  
	  StatisticComputer comp;
	  ScenePtr selection = getSelection();
	  selection->apply(comp);

	  tab2 = new QWidget( tab );
	  real_t surface = getSelectionSurface();
	  real_t volume = getSelectionVolume();
	  QFrame * Line = new QFrame( tab2 );
	  Line->setGeometry( QRect( 30, 100, 351, 20 ) );
	  Line->setFrameShape( QFrame::HLine );
	  Line->setFrameShadow( QFrame::Sunken );
  
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
	  TextLabel2->setText( QString::number(surface) );
	  
	  TextLabel2 = new QLineEdit( tab2 );
	  TextLabel2->setReadOnly(true);
	  TextLabel2->setAlignment(Qt::AlignHCenter);
	  TextLabel2->setGeometry( QRect( 170, 145, 200, 25 ) );
	  TextLabel2->setText( QString::number(volume) );
	  
	  TextLabel2 = new QLineEdit( tab2 );
	  TextLabel2->setReadOnly(true);
	  TextLabel2->setAlignment(Qt::AlignHCenter);
	  TextLabel2->setGeometry( QRect( 170, 170, 200, 25 ) );
	  TextLabel2->setText( QString::number(polygonNumber(selection) ) );
	  
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
	  TextLabel2->setText( labl );
	  
	  Line = new QFrame( tab2  );
	  Line->setGeometry( QRect( 20, 250, 351, 20 ) );
	  Line->setFrameShape( QFrame::HLine );
	  Line->setFrameShadow( QFrame::Sunken );
	  
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
	  if(bbox)TextLabel2->setText( toQString( bbox->getUpperRightCorner()) );
	  
	  TextLabel2 = new QLineEdit( tab2 );
	  TextLabel2->setReadOnly(true);
	  TextLabel2->setAlignment(Qt::AlignHCenter);
	  TextLabel2->setGeometry( QRect( 168, 305, 200, 25 ) );
	  if(bbox)TextLabel2->setText( toQString( bbox->getLowerLeftCorner()) );
	  
	  TextLabel2 = new QLineEdit( tab2 );
	  TextLabel2->setReadOnly(true);
	  TextLabel2->setAlignment(Qt::AlignHCenter);
	  TextLabel2->setGeometry( QRect( 168, 335, 200, 25 ) );
	  if(bbox)TextLabel2->setText( toQString( bbox->getSize()*2)  );
	  
	  TextLabel2 = new QLineEdit( tab2 );
	  TextLabel2->setReadOnly(true);
	  TextLabel2->setAlignment(Qt::AlignHCenter);
	  TextLabel2->setGeometry( QRect( 168, 365, 200, 25 ) );
	  if(bbox)TextLabel2->setText( toQString( bbox->getCenter()) );

	  tab->addTab( tab2, tr( "Selection" ) );
	  
	}
	return true;
}

bool
ViewGeomSceneGL::browse(Q3ListView * l,bool b)
{
  if(!__scene)return false;
  GeomListViewBuilder builder(l);
  builder.setFullMode(b);
  __scene->apply(builder);
  return true;
}

QMenu *
ViewGeomSceneGL::createToolsMenu(QWidget * parent)
{
  QMenu * menu = ViewModalRendererGL::createToolsMenu(parent);
  menu->addSeparator();
  QMenu * __displayMenu = new QMenu(menu);
  QAction * act = __displayMenu->addAction(tr("Enable"),    this,SLOT(changeDisplayListUse()));
  act->setChecked(getDisplayListUse());
  QObject::connect(this,SIGNAL(displayList(bool)),act,SLOT(setChecked(bool)));
  __displayMenu->addSeparator();
  __displayMenu->addAction(tr("Recompute"),      this,SLOT(clearDisplayList()));
  __displayMenu->setTitle(tr("&Display List"));
  menu->addMenu(__displayMenu);
  return menu;
}

void 
ViewGeomSceneGL::clipboard(){
	QClipboard * clipboard = QApplication::clipboard();
	if(clipboard ){
		const QMimeData* data = clipboard->mimeData();
		if(data!=NULL ){
			if(data->hasUrls()){
				QList<QUrl> urls = data->urls();
				if(!urls.empty())
				{
					QFileInfo f(urls[0].toLocalFile());
					QString ext = f.suffix();
					ext.toUpper();
				    if(f.exists()&& (ext == "GEOM" ||ext == "BGEOM")){
						open(urls[0].toLocalFile());
					}
				}
			}
		}
	}
}

/* ----------------------------------------------------------------------- */

void
ViewMultiGeomSceneGL::fillToolBar(QToolBar * toolBar)
{
  ViewGeomSceneGL::fillToolBar(toolBar);
}

  /// Add other toolbar.
bool
ViewMultiGeomSceneGL::addOtherToolBar(QMainWindow * menu)
{
  __transitionBar = new ViewToolBar(tr("Transition"),menu,"TransitionBar");
  QLabel * Label = new QLabel(__transitionBar);
  Label->setText( " "+tr( "Transition" ) +" ");
  __transSlider =  new QSlider ( Qt::Horizontal, __transitionBar );
  __transSlider->setRange(0 , __transitionRenderer.getTotalStep());
  __transSlider->setValue(0);
  __transSlider->setFixedSize(100,25);
  QObject::connect (__transSlider,SIGNAL(valueChanged(int)), this,SLOT(setRenderingStep(int)) );
  QObject::connect (this,SIGNAL(renderingStepChanged(int)), __transSlider,SLOT(setValue(int)) );
  menu->addToolBar(__transitionBar);
  __transitionBar->hide();
  return true;
}


QMenu *
ViewMultiGeomSceneGL::createToolsMenu(QWidget * parent)
{
  QMenu * __menu = ViewGeomSceneGL::createToolsMenu(parent);
  __menu->addSeparator();
  QAction * act = __menu->addAction(tr("Transition Slider"),this,SLOT(changeSliderVisibility()));
  QObject::connect(this,SIGNAL(sliderVisibilityChanged(bool)),act,SLOT(setChecked(bool)));
  return __menu;
}

void
ViewMultiGeomSceneGL::changeSliderVisibility()
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
