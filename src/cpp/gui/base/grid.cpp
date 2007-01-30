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


#include "grid.h"
#include "event.h"
#include <GL/glu.h>
#include <assert.h>
#include <iostream>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>
#include "icons.h"

/* ----------------------------------------------------------------------- */

ViewGridMenu::ViewGridMenu(ViewGridGL * grid, QWidget * parent, const char * name) :
  QPopupMenu(parent,name)
{
  if(grid){
    QPixmap _gridXY(ViewerIcon::getPixmap(ViewerIcon::icon_gridXY));
    QPixmap _gridXZ(ViewerIcon::getPixmap(ViewerIcon::icon_gridXZ));
    QPixmap _gridYZ(ViewerIcon::getPixmap(ViewerIcon::icon_gridYZ));
    QPixmap _axis(ViewerIcon::getPixmap(ViewerIcon::icon_axis));
    idXYGrid = insertItem(_gridXY,tr("XY Plane"),grid,SLOT(changeXYGridDisplayMode()));
    idXZGrid = insertItem(_gridXZ,tr("XZ Plane"),grid,SLOT(changeXZGridDisplayMode()));
    idYZGrid = insertItem(_gridYZ,tr("YZ Plane"),grid,SLOT(changeYZGridDisplayMode()));
    insertSeparator();
    idAxis = insertItem(_axis,tr("Axis"),grid,SLOT(changeAxisDisplayMode()));
    setCheckable( TRUE );
    setItemChecked(idXYGrid,grid->getXYGrid());
    setItemChecked(idYZGrid,grid->getYZGrid());
    setItemChecked(idXZGrid,grid->getXZGrid());
    setItemChecked(idAxis,grid->getAxis());
    QObject::connect(grid,SIGNAL(AxisDisplayChanged(bool)),this,SLOT(setAxis(bool)));
    QObject::connect(grid,SIGNAL(XYGridDisplayChanged(bool)),this,SLOT(setXYGrid(bool)));
    QObject::connect(grid,SIGNAL(XZGridDisplayChanged(bool)),this,SLOT(setXZGrid(bool)));
    QObject::connect(grid,SIGNAL(YZGridDisplayChanged(bool)),this,SLOT(setYZGrid(bool)));
  }
}

ViewGridMenu::~ViewGridMenu()
{
}

void
ViewGridMenu::setXYGrid(bool b)
{
  setItemChecked(idXYGrid,b);
}

void
ViewGridMenu::setXZGrid(bool b)
{
  setItemChecked(idXZGrid,b);
}

void
ViewGridMenu::setYZGrid(bool b)
{
  setItemChecked(idYZGrid,b);
}

void
ViewGridMenu::setAxis(bool b)
{
  setItemChecked(idAxis,b);
}

/* ----------------------------------------------------------------------- */

ViewGridGL::ViewGridGL(ViewCameraGL *camera,
		       QGLWidget * parent, 
		       const char * name):
  ViewRelativeObjectGL(camera,parent,name),
  __gridUnit(1),
  __gridSize(15),
  __gridList(0),
  __axisList(0),
  __Axis(true),
  __XYGrid(true),
  __XZGrid(false),
  __YZGrid(false)
{
}


ViewGridGL::~ViewGridGL()
{
  if (__gridList) glDeleteLists(__gridList,1);
  if (__axisList) glDeleteLists(__axisList,1); 
}


int
ViewGridGL::getState() const{
	return (__Axis) + (__XYGrid? 1 << 1: 0) + (__XZGrid? 1 << 2: 0) + (__YZGrid? 1 << 3: 0);
}

void
ViewGridGL::setState(int s) {
	__Axis = s & 1;
	__XYGrid = s & (1 << 1);
	__XZGrid = s & (1 << 2);
	__YZGrid = s & (1 << 3);
}


void 
ViewGridGL::changeAxisDisplayMode()
{
  __Axis = ! __Axis;
  emit AxisDisplayChanged(__Axis);
  emit valueChanged();
}

void 
ViewGridGL::showAxis(bool b)
{
  __Axis = b;
  emit AxisDisplayChanged(__Axis);
  emit valueChanged();
}

void 
ViewGridGL::changeXYGridDisplayMode()
{
  __XYGrid = ! __XYGrid;
  emit XYGridDisplayChanged(__XYGrid);
  emit valueChanged();
}

void 
ViewGridGL::showXYGrid(bool b)
{
   __XYGrid = b;
  emit XYGridDisplayChanged(__XYGrid);
  emit valueChanged();
}

void 
ViewGridGL::changeXZGridDisplayMode()
{
  __XZGrid = ! __XZGrid;
  emit XZGridDisplayChanged(__XZGrid);
  emit valueChanged();
}

void 
ViewGridGL::showXZGrid(bool b)
{
   __XZGrid = b;
  emit XZGridDisplayChanged(__XZGrid);
  emit valueChanged(); 
}

void 
ViewGridGL::changeYZGridDisplayMode()
{
  __YZGrid = ! __YZGrid;
  emit YZGridDisplayChanged(__YZGrid);
  emit valueChanged();
}

void 
ViewGridGL::showYZGrid(bool b)
{
   __YZGrid = b;
  emit YZGridDisplayChanged(__YZGrid);
  emit valueChanged(); 
}

void 
ViewGridGL::setGridUnit(int unit)
{
  if(__gridUnit != unit){
	__gridUnit = unit;
	emit GridUnitChanged(__gridUnit);
	emit valueChanged();
  }
}

void 
ViewGridGL::setGridUnit(const QString& unit)
{
  __gridUnit = unit.toInt();
}

void 
ViewGridGL::setGridSize(int size)
{
  if(__gridSize != size){
	__gridSize = size;
    if (__gridList) {
	  glDeleteLists(__gridList,1); __gridList = 0; }
	emit GridSizeChanged(__gridSize);
	emit valueChanged();
  }
}

void 
ViewGridGL::setGridSize(const QString& size)
{
  __gridSize = size.toInt();
  if (__gridList) {
	glDeleteLists(__gridList,1); __gridList = 0; }
}

void 
ViewGridGL::initializeGL()
{
  int k;
  int min,max;
  min = -__gridSize;
  max = __gridSize;


  if (__gridList) {
    glDeleteLists(__gridList,1);
  }
  __gridList = glGenLists(1);
  glNewList(__gridList,GL_COMPILE);
  glBegin(GL_LINES);
  for(k=min;k<= - 1;k=k+1)
  {
      glColor3f(0.25,0.25,0.25);
      glVertex3f(min,k,0);
      glVertex3f(max,k,0);
  }
  for(k=min;k<=-1;k=k+1)
  {
      glVertex3f(k,min,0);
      glVertex3f(k,max,0);
  }
  for(k=1;k<=max;k=k+1)
  {
      glColor3f(0.25,0.25,0.25);
      glVertex3f(min,k,0);
      glVertex3f(max,k,0);
  }
  for(k=1;k<=max;k=k+1)
  {
      glVertex3f(k,min,0);
      glVertex3f(k,max,0);
  }
  glEnd();
  glEndList();

  GEOM_GL_ERROR;

  if (__axisList) {
    glDeleteLists(__axisList,1);
  }
  __axisList = glGenLists(1);
  glNewList(__axisList,GL_COMPILE);

  glBegin(GL_LINES);
  glColor3f(0,0.3f,0);
  glVertex3f(0,min,0);
  glVertex3f(0,0,0);
  glEnd();
  glBegin(GL_LINES);
  glColor3f(0,0.8f,0);
  glVertex3f(0,0,0);
  glVertex3f(0,max,0);
  //Y+
  glVertex3f (0, 10.5, 0.25);  
  glVertex3f (0, 10.75, 0);
  glVertex3f (0, 10.75, 0);
  glVertex3f (0, 11, 0.25);
  glVertex3f (0, 10.75, 0);
  glVertex3f (0, 10.75, -0.25);
  glVertex3f (0, 11.25, 0);
  glVertex3f (0, 11.75, 0);
  glVertex3f (0, 11.5, -0.25);
  glVertex3f (0, 11.5, 0.25);
  glEnd();

  GEOM_GL_ERROR;
  
  //axi des x
  glBegin(GL_LINES);
  glColor3f(0.3f,0,0);
  glVertex3f(min,0,0);   
  glVertex3f(0,0,0);
  glEnd();
  glBegin(GL_LINES);
  glColor3f(0.8f,0,0);
  glVertex3f(0,0,0);
  glVertex3f(max,0,0);
  //X+
  glVertex3f (10.5, 0, 0.25);
  glVertex3f (11, 0, -0.25);
  glVertex3f (10.5, 0, -0.25);
  glVertex3f (11, 0, 0.25);
  glVertex3f (11.25, 0, 0);
  glVertex3f (11.75, 0, 0);
  glVertex3f (11.5, 0, -0.25);
  glVertex3f (11.5, 0, 0.25);
  glEnd();

  GEOM_GL_ERROR;
  
  //z axis z
  glBegin(GL_LINES);
  glColor3f(0,0,0.3f);      
  glVertex3f(0,0,min);
  glVertex3f(0,0,0);
  glEnd();
  glBegin(GL_LINES);
  glColor3f(0,0,0.8f);
  glVertex3f(0,0,0);
  glVertex3f(0,0,max);

  // Z+
  glVertex3f (0, -0.25, 10.5);
  glVertex3f (0, 0.25, 11);
  glVertex3f (0, -0.25, 11);
  glVertex3f (0, 0.25, 11);
  glVertex3f (0, -0.25,  10.5);
  glVertex3f (0, 0.25, 10.5);
  glVertex3f (0, 0.5, 10.75);
  glVertex3f (0, 1, 10.75);
  glVertex3f (0, 0.75, 10.5);
  glVertex3f (0, 0.75, 11);
  glEnd();
  glEndList();

  GEOM_GL_ERROR;
 
}

void 
ViewGridGL::paintGL()
{

  glPushAttrib(GL_CURRENT_BIT);
  glMatrixMode(GL_MODELVIEW);

  glBlendFunc(GL_ONE,GL_ZERO);

  if (!__gridList)initializeGL();

  glPushMatrix();
  if(__gridUnit > 1)glScalef(float(__gridUnit),float(__gridUnit),float(__gridUnit));
  if(__XYGrid)
    //XY plane
    {
      glCallList(__gridList);
    }

  if(__YZGrid)
    //YZ plane
    {
      glPushMatrix();
      glRotated(90,0,1,0);
      glCallList(__gridList);
      glPopMatrix();
    }

  if(__XZGrid)
    //XZ plane
    {
	  glPushMatrix();
	  glRotated(90,1,0,0);
	  glCallList(__gridList);
	  glPopMatrix();
    }
  
  if(__Axis)
    //axis
    {
      glCallList(__axisList);
    }
  else {
	  glColor3f(0.25,0.25,0.25);
	  if(__XYGrid || __XZGrid){
		glBegin(GL_LINES);
		glVertex3f(-__gridSize,0,0);
		glVertex3f(__gridSize,0,0);
		glEnd();
	  }
	  if(__XYGrid || __YZGrid){
		glBegin(GL_LINES);
		glVertex3f(0,-__gridSize,0);
		glVertex3f(0,__gridSize,0);
		glEnd();
	  }
	  if(__XZGrid || __YZGrid){
		glBegin(GL_LINES);
		glVertex3f(0,0,-__gridSize);
		glVertex3f(0,0,__gridSize);
		glEnd();
	  }

  }

  glPopMatrix();
  glPopAttrib();
  GEOM_GL_ERROR;
}

void 
ViewGridGL::changeStepEvent(const int newStep, const int oldStep)
{
  __gridUnit = int(float(newStep*__gridUnit)/float(oldStep));
  if(__gridUnit == 0)__gridUnit = 1;
  emit GridUnitChanged(__gridUnit);
}

QPopupMenu *
ViewGridGL::createToolsMenu(QWidget * parent)
{
  ViewGridMenu * __GridMenu = new ViewGridMenu(this,parent);  
  return __GridMenu;
}

void ViewGridGL::fillToolBar(QToolBar * toolBar)
{
    QPixmap _gridXY(ViewerIcon::getPixmap(ViewerIcon::icon_gridXY));
    QPixmap _gridXZ(ViewerIcon::getPixmap(ViewerIcon::icon_gridXZ));
    QPixmap _gridYZ(ViewerIcon::getPixmap(ViewerIcon::icon_gridYZ));
    QPixmap _axis(ViewerIcon::getPixmap(ViewerIcon::icon_axis));
    QToolButton * b = new QToolButton(_axis,tr("Axis"),tr("Axis"),
                         this, SLOT(changeAxisDisplayMode()), toolBar);
	QWhatsThis::add(b,tr("<b>Axis Visibility</b><br><br>"
	"Change the visibility of the Axis.<br><br>"
	"You can also use Menu <br><b>Tools > Grid > Axis</b><br>"));
	b->setToggleButton(true) ;
	b->setOn(__Axis);
    QObject::connect(this,SIGNAL(AxisDisplayChanged(bool)),
					 b,SLOT(setOn(bool)));


    b = new QToolButton(_gridXY,tr("XY Plane"),tr("XY Plane"),
                         this, SLOT(changeXYGridDisplayMode()), toolBar);
	QWhatsThis::add(b,tr("<b>XY Plane Visibility</b><br><br>"
	"Change the visibility of the XY Grid.<br><br>"
	"You can also use Menu <br><b>Tools > Grid > XY Plane</b><br>"));
	b->setToggleButton(true) ;
	b->setOn(__XYGrid);
    QObject::connect(this,SIGNAL(XYGridDisplayChanged(bool)),
					 b,SLOT(setOn(bool)));

    b = new QToolButton(_gridXZ,tr("XZ Plane"),tr("XZ Plane"),
                         this, SLOT(changeXZGridDisplayMode()), toolBar);
	QWhatsThis::add(b,tr("<b>XZ Plane Visibility</b><br><br>"
	"Change the visibility of the XZ Grid.<br><br>"
	"You can also use Menu <br><b>Tools > Grid > XZ Plane</b><br>"));
	b->setToggleButton(true) ;
	b->setOn(__XZGrid);
    QObject::connect(this,SIGNAL(XZGridDisplayChanged(bool)),
					 b,SLOT(setOn(bool)));

    b = new QToolButton(_gridYZ,tr("YZ Plane"),tr("YZ Plane"),
                         this, SLOT(changeYZGridDisplayMode()), toolBar);
	QWhatsThis::add(b,tr("<b>YZ Plane Visibility</b><br><br>"
	"Change the visibility of the YZ Grid.<br><br>"
	"You can also use Menu <br><b>Tools > Grid > YZ Plane</b><br>"));
	b->setToggleButton(true) ;
	b->setOn(__YZGrid);
    QObject::connect(this,SIGNAL(YZGridDisplayChanged(bool)),
					 b,SLOT(setOn(bool)));

	toolBar->addSeparator();
}


void 
ViewGridGL::geomCoordSys()
{
  bool a = __XYGrid;
  bool b = __XZGrid;
  bool c = __YZGrid;
  if(__XZGrid != c){
    __XZGrid = c;
    emit XZGridDisplayChanged(__XZGrid);
  }
  if(__XYGrid != b){
    __XYGrid = b;
    emit XYGridDisplayChanged(__XYGrid);
  }
  if(__YZGrid != a){
    __YZGrid = a;
    emit YZGridDisplayChanged(__YZGrid);
  }
}

void 
ViewGridGL::glCoordSys()
{
  bool a = __XYGrid;
  bool b = __XZGrid;
  bool c = __YZGrid;
  if(__XZGrid != a){
    __XZGrid = a;
    emit XZGridDisplayChanged(__XZGrid);
  }
  if(__XYGrid != c){
    __XYGrid = c;
    emit XYGridDisplayChanged(__XYGrid);
  }
  if(__YZGrid != b){
    __YZGrid = b;
    emit YZGridDisplayChanged(__YZGrid);
  }
}

void ViewGridGL::gridEvent(ViewGridEvent * e){
  bool c = false;
  if((e->def & 1) && (__XYGrid != e->xy)){
	__XYGrid = e->xy;
	emit XYGridDisplayChanged(__XYGrid);
	c = true;
  }
  if((e->def & 2) && (__YZGrid != e->yz)){
	__YZGrid = e->yz;
	emit YZGridDisplayChanged(__YZGrid);
	c = true;
  }
  if((e->def & 4) && (__XZGrid != e->xz)){
	__XZGrid = e->xz;
	emit XZGridDisplayChanged(__XZGrid);
	c = true;
  }
  if((e->def & 8) && (__Axis != e->axis)){
	__Axis = e->axis;
	emit AxisDisplayChanged(__Axis);
	c = true;
  }
  if((e->def & 16) && (__gridSize != e->size) && e->size > 1){
	__gridSize = e->size;
	if (__gridList) { glDeleteLists(__gridList,1); __gridList = 0; }
	emit GridSizeChanged(__gridSize);
	c = true;
  }
  if((e->def & 32) && (__gridUnit != e->unit) && e->unit > 1){
	__gridUnit = e->unit;
	emit GridUnitChanged(__gridUnit);
	c = true;
  }
  if(c)emit valueChanged();
}

/* ----------------------------------------------------------------------- */
