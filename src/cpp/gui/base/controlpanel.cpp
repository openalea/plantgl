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



#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qcolordialog.h>
#include <qslider.h>
#include <qlineedit.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qdial.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qmainwindow.h>

#include "controlpanel.h"
#include "glframe.h"
#include "camera.h"
#include "light.h"
#include "grid.h"
#include "scenegl.h"


/* ----------------------------------------------------------------------- */


ViewControlPanel::ViewControlPanel( ViewGLFrame * _glframe,  
			    const QString & label, 
			    QMainWindow * mw, 
#if QT_VERSION >= 300
			    ToolBarDock dock,
#else
			    QMainWindow::ToolBarDock dock, 
#endif
			    bool newLine , 
			    const char * name )
    : ViewToolBar( label, mw, dock, newLine, name ),
	__useSpinBox(true)
{

  	setOrientation(Qt::Vertical);

	QObject::connect(this,SIGNAL(orientationChanged( Orientation )),
			this,SLOT(orientationChangedEvent( Orientation )) );
	QPixmap logo(ViewerIcon::getPixmap(ViewerIcon::icon_logobar));
	QToolButton * bt = new QToolButton(logo,
			"","",mw,SLOT(displayAbout()),this);
	bt->setUsesBigPixmap(true);
	

	ViewRendererGL * sc = _glframe->getSceneRenderer();
	ViewSceneRendererGL * sc2 = dynamic_cast<ViewSceneRendererGL *>(sc);

	if(sc2){
	  QObject::connect(sc2,SIGNAL(lightEnableChanged(bool)),
			   this,SLOT(setLightEnable(bool)) );
	}

	// Scene Position : valeur de rotation de la GL	

	__sceneGroupBox = new QGroupBox(tr("Camera"),this,"Scene Position Group Box");
//	__sceneGroupBox->setMaximumHeight(145);
	QLabel * _lblElevation=new QLabel(tr("Elevation"),__sceneGroupBox);
	_lblElevation->setGeometry(80,20,44,30);
	QLabel * _lblAzimuth=new QLabel(tr("Azimuth"),__sceneGroupBox);
	_lblAzimuth->setGeometry(80,65,40,30);
	QLabel * _lblZoom=new QLabel(tr("Zoom"),__sceneGroupBox);
	_lblZoom->setGeometry(80,105,30,30);
	QLabel* _StepMoveLabel = new QLabel(tr("Move"),__sceneGroupBox);
	_StepMoveLabel->setGeometry(80,150,30,20);
	ViewCameraGL * camera = _glframe->getCamera();

    QPixmap unlinked(ViewerIcon::getPixmap(ViewerIcon::icon_unlinked));
    __linkedZoom = new QPushButton("",__sceneGroupBox);
	__linkedZoom->setGeometry(1,105,15,30);
	setLinked(camera->isDimLock());
	__linkedZoom->setFlat(true);

	QObject::connect (__linkedZoom,SIGNAL(clicked()), 	
			camera,SLOT(lockDim()) ); 	
	QObject::connect (camera,SIGNAL(lockDimChanged(bool)), 	
			this,SLOT(setLinked(bool)) ); 	

	__elevation=new QDial(-179,180,1,
			      (int)camera->getElevation(),
			      __sceneGroupBox,"Elevation");
	__elevation->setGeometry(20,15,40,40);
	__elevation->setLineStep(5);
	__elevation->setWrapping(true);
	__elevation->setNotchesVisible(true);
	__elevation->hide();

	__elevationSpin=new QSpinBox(-179,180,1,__sceneGroupBox,"ElevationSpin");
	__elevationSpin->setValue((int)camera->getElevation());
	__elevationSpin->setGeometry(15,25,65,30);

	__azimuth=new QDial(-179,180,1,
			    (int)camera->getAzimuth(),
			    __sceneGroupBox,"Azimuth");
	__azimuth->setGeometry(20,60,40,40);
	__azimuth->setLineStep(5);
	__azimuth->setWrapping(true);
	__azimuth->setNotchesVisible(true);
	__azimuth->hide();

	__azimuthSpin=new QSpinBox(-179,180,1,__sceneGroupBox,"AzimuthSpin");
	__azimuthSpin->setValue((int)camera->getAzimuth());
	__azimuthSpin->setGeometry(15,65,65,30);

	__zoom=new QSpinBox(INT_MIN,INT_MAX,1,__sceneGroupBox,"Zoom");
	__zoom->setGeometry(15,105,65,30);
	__zoom->setValue((int)camera->getZoom());

	QSpinBox* _StepMove=new QSpinBox(1,INT_MAX,1,__sceneGroupBox,"StepMove");
	_StepMove->setGeometry(15,145,65,30);
	_StepMove->setValue(camera->getStepMove());

	QObject::connect (camera,SIGNAL(elevationChanged(int)), 	
			__elevation,SLOT(setValue(int)) ); 	
	QObject::connect (camera,SIGNAL(azimuthChanged(int)), 	
			__azimuth,SLOT(setValue(int)) ); 	
	QObject::connect (camera,SIGNAL(elevationChanged(int)), 	
			__elevationSpin,SLOT(setValue(int)) ); 	
	QObject::connect (camera,SIGNAL(azimuthChanged(int)), 	
			__azimuthSpin,SLOT(setValue(int)) ); 	
	QObject::connect (camera,SIGNAL(zoomChanged(int)), 	
			__zoom,SLOT(setValue(int)) ); 	

	QObject::connect (__elevation,SIGNAL(valueChanged(int)),
			  camera,SLOT(setElevation(int)) );
	QObject::connect (__azimuth,SIGNAL(valueChanged(int)),
			  camera,SLOT(setAzimuth(int)) );
	QObject::connect (__elevationSpin,SIGNAL(valueChanged(int)),
			  camera,SLOT(setElevation(int)) );
	QObject::connect (__azimuthSpin,SIGNAL(valueChanged(int)),
			  camera,SLOT(setAzimuth(int)) );
	QObject::connect (__zoom,SIGNAL(valueChanged(int)),
			  camera,SLOT(setZoom(int)) );

	QObject::connect (camera,SIGNAL(stepMoveChanged(int)),
			  _StepMove,SLOT(setValue(int)) ); 	
	QObject::connect (_StepMove,SIGNAL(valueChanged(int)),
			  camera,SLOT(setStepMove(int)) );

	//Light azimuth & elevation
	ViewLightGL * light = _glframe->getLight();
	

	__lightPosGroupBox = new QGroupBox(tr("Light Position"),this,"Light Position Group Box");
	QLabel* _lblLightElevation = new QLabel(tr("Elevation"),__lightPosGroupBox);
	_lblLightElevation->setGeometry(80,20,44,30);
	QLabel* _lblLightAzimuth = new QLabel(tr("Azimuth"),__lightPosGroupBox);
	_lblLightAzimuth->setGeometry(80,65,40,30);
	QLabel* _lblLightDistance = new QLabel(tr("Distance"),__lightPosGroupBox);
	_lblLightDistance->setGeometry(80,105,44,30);


	__lightElevation=new QDial(-179,180,1,
				   (int)light->getElevation(),
				   __lightPosGroupBox,"LightElevation");
	__lightElevation->setGeometry(15,15,40,40);
	__lightElevation->setLineStep(5);
	__lightElevation->setWrapping(true);
	__lightElevation->setNotchesVisible(true);
	__lightElevation->hide();

	__lightElevationSpin=new QSpinBox(-179,180,1,__lightPosGroupBox,"ligthElevationSpin");
	__lightElevationSpin->setValue((int)light->getElevation());
	__lightElevationSpin->setGeometry(10,25,65,30);

	__lightAzimuth=new QDial(-179,180,1,
				 (int)light->getAzimuth(),
				 __lightPosGroupBox,"LightAzimuth");
	__lightAzimuth->setGeometry(15,60,40,40);
	__lightAzimuth->setLineStep(5);
	__lightAzimuth->setWrapping(true);
	__lightAzimuth->setNotchesVisible(true);
	__lightAzimuth->hide();

	__lightAzimuthSpin=new QSpinBox(-179,180,1,__lightPosGroupBox,"ligthAzimuthSpin");
	__lightAzimuthSpin->setValue((int)light->getAzimuth());
	__lightAzimuthSpin->setGeometry(10,65,65,30);

	QSpinBox * __LightDistance= new QSpinBox(INT_MIN,INT_MAX,1,__lightPosGroupBox,"LightDistance");
	__LightDistance->setGeometry(10,105,65,30);
	__LightDistance->setValue((int)light->getDistance());

	QObject::connect (light,SIGNAL(elevationChanged(int)),
			  __lightElevation,SLOT(setValue(int)) );
	QObject::connect (light,SIGNAL(azimuthChanged(int)),
			  __lightAzimuth,SLOT(setValue(int)) );
	QObject::connect (light,SIGNAL(elevationChanged(int)),
			  __lightElevationSpin,SLOT(setValue(int)) );
	QObject::connect (light,SIGNAL(azimuthChanged(int)),
			  __lightAzimuthSpin,SLOT(setValue(int)) );
	QObject::connect (light,SIGNAL(distanceChanged(int)),
			  __LightDistance,SLOT(setValue(int)) );

	QObject::connect (__lightElevation,SIGNAL(valueChanged(int)),
			  light,SLOT(setElevation(int)) );
	QObject::connect (__lightAzimuth,SIGNAL(valueChanged(int)),
			  light,SLOT(setAzimuth(int)) );
	QObject::connect (__lightElevationSpin,SIGNAL(valueChanged(int)),
			  light,SLOT(setElevation(int)) );
	QObject::connect (__lightAzimuthSpin,SIGNAL(valueChanged(int)),
			  light,SLOT(setAzimuth(int)) );
	QObject::connect (__LightDistance,SIGNAL(valueChanged(int)),
			  light,SLOT(setDistance(int)) );

	__lightColGroupBox = new QGroupBox(tr("Light Material"),this,"Light Color Group Box");

	QLabel* __lblLightAmbient = new QLabel(tr("Ambient"),__lightColGroupBox);
	__lblLightAmbient->setGeometry(80,20,40,30);
	QLabel* __lblLightDiffuse = new QLabel(tr("Diffuse"),__lightColGroupBox);
	__lblLightDiffuse->setGeometry(80,50,40,30);
	QLabel* __lblLightSpecular = new QLabel(tr("Specular"),__lightColGroupBox);
	__lblLightSpecular->setGeometry(80,80,44,30);

	__LightAmbientButton=new QPushButton(__lightColGroupBox,"LightAmbient");
	__LightAmbientButton->setGeometry(10,20,40,30);
	QPixmap col(40,30);
	__LightAmbient = light->getAmbient();
	col.fill(__LightAmbient);
	__LightAmbientButton->setPixmap(col);

	__LightDiffuseButton=new QPushButton(__lightColGroupBox,"LightDiffuse");
	__LightDiffuseButton->setGeometry(10,50,40,30);
	__LightDiffuse = light->getDiffuse();
	col.fill(__LightDiffuse);
	__LightDiffuseButton->setPixmap(col);

	__LightSpecularButton=new QPushButton(__lightColGroupBox,"LightSpecular");
	__LightSpecularButton->setGeometry(10,80,40,30);
	__LightSpecular = light->getSpecular();
	col.fill(__LightSpecular);
	__LightSpecularButton->setPixmap(col);

	QObject::connect (light,SIGNAL(ambientChanged(const QColor&)), 	
			  this,SLOT(setLightAmbient(const QColor&)) );
	QObject::connect (this,SIGNAL(lightAmbientChanged(const QColor&)),
			  light,SLOT(setAmbient(const QColor&)) );
	QObject::connect (__LightAmbientButton,SIGNAL(clicked()),
			  this,SLOT(selectLightAmbient()) );

	QObject::connect (light,SIGNAL(diffuseChanged(const QColor&)), 	
			  this,SLOT(setLightDiffuse(const QColor&)) );
	QObject::connect (this,SIGNAL(lightDiffuseChanged(const QColor&)),
			  light,SLOT(setDiffuse(const QColor&)) );
	QObject::connect (__LightDiffuseButton,SIGNAL(clicked()),
			  this,SLOT(selectLightDiffuse()) );

	QObject::connect (light,SIGNAL(specularChanged(const QColor&)), 	
			  this,SLOT(setLightSpecular(const QColor&)) );
	QObject::connect (this,SIGNAL(lightSpecularChanged(const QColor&)),
			  light,SLOT(setSpecular(const QColor&)) );
	QObject::connect (__LightSpecularButton,SIGNAL(clicked()),
			  this,SLOT(selectLightSpecular()) );



	//grid control

	__gridGroupBox = new QGroupBox(tr("Grids"),this,"Grid Group Box");


	//step for the grid
	ViewGridGL * grid = _glframe->getGrid();

	QSpinBox* __unitGrid= new QSpinBox(1,INT_MAX,1,__gridGroupBox,"SizeGrid");
	__unitGrid->setValue(grid->getGridUnit());
	__unitGrid->setGeometry(10,20,65,30);
	QObject::connect (grid,SIGNAL(GridUnitChanged(int)),
			  __unitGrid,SLOT(setValue(int)) ); 	
	QObject::connect (__unitGrid,SIGNAL(valueChanged(int)),
			  grid,SLOT(setGridUnit(int)) );

	QSpinBox* __sizeGrid= new QSpinBox(1,INT_MAX,1,__gridGroupBox,"StepUnit");
	__sizeGrid->setValue(grid->getGridSize());
	__sizeGrid->setGeometry(10,50,65,30);

	QObject::connect (grid,SIGNAL(GridSizeChanged(int)),
			  __sizeGrid,SLOT(setValue(int)) ); 	
	QObject::connect (__sizeGrid,SIGNAL(valueChanged(int)),
			  grid,SLOT(setGridSize(int)) );

	QLabel* m_lblStepGrid = new QLabel(tr("Unit"),__gridGroupBox);
	m_lblStepGrid->setGeometry(80,20,30,30);

	m_lblStepGrid = new QLabel(tr("Size"),__gridGroupBox);
	m_lblStepGrid->setGeometry(80,50,30,30);

}


ViewControlPanel::~ViewControlPanel()
{
}

void
ViewControlPanel::setLinked(bool b)
{
  if(!b){
   QPixmap linked(ViewerIcon::getPixmap(ViewerIcon::icon_linked));
   __linkedZoom->setPixmap(linked);
  }
  else {
   QPixmap unlinked(ViewerIcon::getPixmap(ViewerIcon::icon_unlinked));
   __linkedZoom->setPixmap(unlinked);
  }
}

void
ViewControlPanel::setLightEnable(bool a)
{
  if(!a){
    __lightPosGroupBox->hide();
    __lightColGroupBox->hide();
  }
  else {
    __lightColGroupBox->show();
    __lightPosGroupBox->show();
  }
}

void 
ViewControlPanel::moveEvent ( QMoveEvent * event)
{
}

void 
ViewControlPanel::orientationChangedEvent( Orientation orientation )
{
}


void 
ViewControlPanel::resizeEvent( QResizeEvent * event)
{ 
}

void 
ViewControlPanel::setLightAmbient(const QColor& color)
{
  __LightAmbient = color;
  QPixmap col(40,30);
  col.fill(color);
  __LightAmbientButton->setPixmap(col);
}

void 
ViewControlPanel::selectLightAmbient()
{

  QColor LightAmbient = QColorDialog::getColor(__LightAmbient,this);
  if(LightAmbient.isValid()){
    __LightAmbient = LightAmbient;
    QPixmap col(40,30);
    col.fill(LightAmbient);
    __LightAmbientButton->setPixmap(col);
    emit lightAmbientChanged(__LightAmbient);
  }
}

void 
ViewControlPanel::setLightDiffuse(const QColor& color)
{
  __LightDiffuse = color;
  QPixmap col(40,30);
  col.fill(color);
  __LightDiffuseButton->setPixmap(col);
}

void 
ViewControlPanel::selectLightDiffuse()
{

  QColor LightDiffuse = QColorDialog::getColor(__LightDiffuse,this);
  if(LightDiffuse.isValid()){
    __LightDiffuse = LightDiffuse;
    QPixmap col(40,30);
    col.fill(LightDiffuse);
    __LightDiffuseButton->setPixmap(col);
    emit lightDiffuseChanged(__LightDiffuse);
  }
}

void 
ViewControlPanel::setLightSpecular(const QColor& color)
{
  __LightSpecular = color;
  QPixmap col(40,30);
  col.fill(color);
  __LightSpecularButton->setPixmap(col);
}

void 
ViewControlPanel::selectLightSpecular()
{

  QColor LightSpecular = QColorDialog::getColor(__LightSpecular,this);
  if(LightSpecular.isValid()){
    __LightSpecular = LightSpecular;
    QPixmap col(40,30);
    col.fill(LightSpecular);
    __LightSpecularButton->setPixmap(col);
    emit lightSpecularChanged(__LightSpecular);
  }
}

void
ViewControlPanel::useSpinBox(bool b){
  if(__useSpinBox != b){
	__useSpinBox = b;
	if(__useSpinBox){
	  __elevation->hide();
	  __azimuth->hide();
	  __elevationSpin->show();
	  __azimuthSpin->show();
	  __lightElevation->hide();
	  __lightAzimuth->hide();
	  __lightElevationSpin->show();
	  __lightAzimuthSpin->show();
	}
	else {
	  __elevation->show();
	  __azimuth->show();
	  __elevationSpin->hide();
	  __azimuthSpin->hide();
	  __lightElevation->show();
	  __lightAzimuth->show();
	  __lightElevationSpin->hide();
	  __lightAzimuthSpin->hide();
	}
  }
}

bool
ViewControlPanel::spinBoxUsed() const{
  return __useSpinBox;
}
