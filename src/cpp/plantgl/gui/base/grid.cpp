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



#include "grid.h"
#include "event.h"
#include <plantgl/algo/opengl/util_gl.h>
#include <assert.h>
#include <iostream>
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qwhatsthis.h>
    #include <QtWidgets/qtoolbutton.h>
    #include <QtWidgets/qtoolbar.h>
#else
    #include <QtGui/qwhatsthis.h>
    #include <QtGui/qtoolbutton.h>
    #include <QtGui/qtoolbar.h>
#endif
#include "icons.h"
#include "configuration.h"

/* ----------------------------------------------------------------------- */

ViewGridGL::ViewGridGL(ViewCameraGL *camera,
               QOpenGLBaseWidget * parent,
               const char * name, PGLOpenGLFunctionsPtr ogl):
  ViewRelativeObjectGL(camera,parent,name, ogl),
  __gridUnit(1),
  __gridSize(15),
  __gridList(0),
  __axisList(0),
  __Axis(true),
  __XYGrid(true),
  __XZGrid(false),
  __YZGrid(false)
{
  ViewerSettings settings;
  settings.beginGroup("Grids");
  __XYGrid = settings.value("XYGrid",__XYGrid).toBool();
  __XZGrid = settings.value("XZGrid",__XZGrid).toBool();
  __YZGrid = settings.value("YZGrid",__YZGrid).toBool();
  __Axis = settings.value("Axis",__Axis).toBool();
  __gridSize = settings.value("Size",__gridSize).toInt();
  settings.endGroup();

}

ViewGridGL::~ViewGridGL()
{
  if (__gridList) __ogl->glDeleteLists(__gridList,1);
  if (__axisList) __ogl->glDeleteLists(__axisList,1);
}

void ViewGridGL::endEvent()
{
  ViewerSettings settings;
  settings.beginGroup("Grids");
  settings.setValue("XYGrid",__XYGrid);
  settings.setValue("XZGrid",__XZGrid);
  settings.setValue("YZGrid",__YZGrid);
  settings.setValue("Axis",__Axis);
  settings.setValue("Size",__gridSize);
  settings.endGroup();
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
ViewGridGL::setGridUnit(double unit)
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
  __gridUnit = unit.toDouble();
}

void
ViewGridGL::setGridSize(int size)
{
  if(__gridSize != size){
    __gridSize = size;
    if (__gridList) {
      __ogl->glDeleteLists(__gridList,1); __gridList = 0; }
    emit GridSizeChanged(__gridSize);
    emit valueChanged();
  }
}

void
ViewGridGL::setGridSize(const QString& size)
{
  __gridSize = size.toInt();
  if (__gridList) {
    __ogl->glDeleteLists(__gridList,1); __gridList = 0; }
}

void
ViewGridGL::initializeGL()
{
  int k;
  int min,max;
  min = -__gridSize;
  max = __gridSize;


  if (__gridList) {
    __ogl->glDeleteLists(__gridList,1);
  }
  __gridList = __ogl->glGenLists(1);
  __ogl->glNewList(__gridList,GL_COMPILE);
  __ogl->glBegin(GL_LINES);
  for(k=min;k<= - 1;k=k+1)
  {
      __ogl->glColor3f(0.25,0.25,0.25);
      __ogl->glVertex3f(min,k,0);
      __ogl->glVertex3f(max,k,0);
  }
  for(k=min;k<=-1;k=k+1)
  {
      __ogl->glVertex3f(k,min,0);
      __ogl->glVertex3f(k,max,0);
  }
  for(k=1;k<=max;k=k+1)
  {
      __ogl->glColor3f(0.25,0.25,0.25);
      __ogl->glVertex3f(min,k,0);
      __ogl->glVertex3f(max,k,0);
  }
  for(k=1;k<=max;k=k+1)
  {
      __ogl->glVertex3f(k,min,0);
      __ogl->glVertex3f(k,max,0);
  }
  __ogl->glEnd();
  __ogl->glEndList();

  GEOM_GL_ERROR;

  if (__axisList) {
    __ogl->glDeleteLists(__axisList,1);
  }
  __axisList = __ogl->glGenLists(1);
  __ogl->glNewList(__axisList,GL_COMPILE);

  __ogl->glBegin(GL_LINES);
  __ogl->glColor3f(0,0.3f,0);
  __ogl->glVertex3f(0,min,0);
  __ogl->glVertex3f(0,0,0);
  __ogl->glEnd();
  __ogl->glBegin(GL_LINES);
  __ogl->glColor3f(0,0.8f,0);
  __ogl->glVertex3f(0,0,0);
  __ogl->glVertex3f(0,max,0);
  //Y+
  __ogl->glVertex3f (0, 10.5, 0.25);
  __ogl->glVertex3f (0, 10.75, 0);
  __ogl->glVertex3f (0, 10.75, 0);
  __ogl->glVertex3f (0, 11, 0.25);
  __ogl->glVertex3f (0, 10.75, 0);
  __ogl->glVertex3f (0, 10.75, -0.25);
  __ogl->glVertex3f (0, 11.25, 0);
  __ogl->glVertex3f (0, 11.75, 0);
  __ogl->glVertex3f (0, 11.5, -0.25);
  __ogl->glVertex3f (0, 11.5, 0.25);
  __ogl->glEnd();

  GEOM_GL_ERROR;

  //axi des x
  __ogl->glBegin(GL_LINES);
  __ogl->glColor3f(0.3f,0,0);
  __ogl->glVertex3f(min,0,0);
  __ogl->glVertex3f(0,0,0);
  __ogl->glEnd();
  __ogl->glBegin(GL_LINES);
  __ogl->glColor3f(0.8f,0,0);
  __ogl->glVertex3f(0,0,0);
  __ogl->glVertex3f(max,0,0);
  //X+
  __ogl->glVertex3f (10.5, 0, 0.25);
  __ogl->glVertex3f (11, 0, -0.25);
  __ogl->glVertex3f (10.5, 0, -0.25);
  __ogl->glVertex3f (11, 0, 0.25);
  __ogl->glVertex3f (11.25, 0, 0);
  __ogl->glVertex3f (11.75, 0, 0);
  __ogl->glVertex3f (11.5, 0, -0.25);
  __ogl->glVertex3f (11.5, 0, 0.25);
  __ogl->glEnd();

  GEOM_GL_ERROR;

  //z axis z
  __ogl->glBegin(GL_LINES);
  __ogl->glColor3f(0,0,0.3f);
  __ogl->glVertex3f(0,0,min);
  __ogl->glVertex3f(0,0,0);
  __ogl->glEnd();
  __ogl->glBegin(GL_LINES);
  __ogl->glColor3f(0,0,0.8f);
  __ogl->glVertex3f(0,0,0);
  __ogl->glVertex3f(0,0,max);

  // Z+
  __ogl->glVertex3f (0, -0.25, 10.5);
  __ogl->glVertex3f (0, 0.25, 11);
  __ogl->glVertex3f (0, -0.25, 11);
  __ogl->glVertex3f (0, 0.25, 11);
  __ogl->glVertex3f (0, -0.25,  10.5);
  __ogl->glVertex3f (0, 0.25, 10.5);
  __ogl->glVertex3f (0, 0.5, 10.75);
  __ogl->glVertex3f (0, 1, 10.75);
  __ogl->glVertex3f (0, 0.75, 10.5);
  __ogl->glVertex3f (0, 0.75, 11);
  __ogl->glEnd();
  __ogl->glEndList();

  GEOM_GL_ERROR;

}

void
ViewGridGL::paintGL()
{

  __ogl->glPushAttrib(GL_CURRENT_BIT);
  __ogl->glMatrixMode(GL_MODELVIEW);

  __ogl->glBlendFunc(GL_ONE,GL_ZERO);

  if (!__gridList)initializeGL();

  __ogl->glPushMatrix();
  if(__gridUnit > 1)__ogl->glScalef(float(__gridUnit),float(__gridUnit),float(__gridUnit));
  if(__XYGrid)
    //XY plane
    {
      __ogl->glCallList(__gridList);
    }

  if(__YZGrid)
    //YZ plane
    {
      __ogl->glPushMatrix();
      __ogl->glRotated(90,0,1,0);
      __ogl->glCallList(__gridList);
      __ogl->glPopMatrix();
    }

  if(__XZGrid)
    //XZ plane
    {
      __ogl->glPushMatrix();
      __ogl->glRotated(90,1,0,0);
      __ogl->glCallList(__gridList);
      __ogl->glPopMatrix();
    }

  if(__Axis)
    //axis
    {
      __ogl->glCallList(__axisList);
    }
  else {
      __ogl->glColor3f(0.25,0.25,0.25);
      if(__XYGrid || __XZGrid){
        __ogl->glBegin(GL_LINES);
        __ogl->glVertex3f(-__gridSize,0,0);
        __ogl->glVertex3f(__gridSize,0,0);
        __ogl->glEnd();
      }
      if(__XYGrid || __YZGrid){
        __ogl->glBegin(GL_LINES);
        __ogl->glVertex3f(0,-__gridSize,0);
        __ogl->glVertex3f(0,__gridSize,0);
        __ogl->glEnd();
      }
      if(__XZGrid || __YZGrid){
        __ogl->glBegin(GL_LINES);
        __ogl->glVertex3f(0,0,-__gridSize);
        __ogl->glVertex3f(0,0,__gridSize);
        __ogl->glEnd();
      }

  }

  __ogl->glPopMatrix();
  __ogl->glPopAttrib();
  GEOM_GL_ERROR;
}

void
ViewGridGL::changeStepEvent(double newStep, double oldStep)
{
  __gridUnit = int(float(newStep*__gridUnit)/float(oldStep));
  if(__gridUnit == 0)__gridUnit = 1;
  emit GridUnitChanged(__gridUnit);
}

QMenu *
ViewGridGL::createToolsMenu(QWidget * parent)
{
    QMenu * menu = new QMenu(parent);

    QPixmap _gridXY(ViewerIcon::getPixmap(ViewerIcon::gridXY));
    QPixmap _gridXZ(ViewerIcon::getPixmap(ViewerIcon::gridXZ));
    QPixmap _gridYZ(ViewerIcon::getPixmap(ViewerIcon::gridYZ));
    QPixmap _axis(ViewerIcon::getPixmap(ViewerIcon::axis));

    QAction * idXYGrid = menu->addAction(_gridXY,tr("XY Plane"),this,SLOT(changeXYGridDisplayMode()));
    QAction * idXZGrid = menu->addAction(_gridXZ,tr("XZ Plane"),this,SLOT(changeXZGridDisplayMode()));
    QAction * idYZGrid = menu->addAction(_gridYZ,tr("YZ Plane"),this,SLOT(changeYZGridDisplayMode()));
    menu->addSeparator();
    QAction * idAxis = menu->addAction(_axis,tr("Axis"),this,SLOT(changeAxisDisplayMode()));

    idXYGrid->setCheckable( true );
    idXZGrid->setCheckable( true );
    idYZGrid->setCheckable( true );
    idAxis->setCheckable( true );

    idXYGrid->setChecked( this->getXYGrid() );
    idXZGrid->setChecked( this->getYZGrid() );
    idYZGrid->setChecked( this->getXZGrid() );
    idAxis->setChecked( this->getAxis() );

    QObject::connect(this,SIGNAL(AxisDisplayChanged(bool)),idAxis,SLOT(setChecked(bool)));
    QObject::connect(this,SIGNAL(XYGridDisplayChanged(bool)),idXYGrid,SLOT(setChecked(bool)));
    QObject::connect(this,SIGNAL(XZGridDisplayChanged(bool)),idXZGrid,SLOT(setChecked(bool)));
    QObject::connect(this,SIGNAL(YZGridDisplayChanged(bool)),idYZGrid,SLOT(setChecked(bool)));

  return menu;
}

void ViewGridGL::fillToolBar(QToolBar * toolBar)
{
    QPixmap _gridXY(ViewerIcon::getPixmap(ViewerIcon::gridXY));
    QPixmap _gridXZ(ViewerIcon::getPixmap(ViewerIcon::gridXZ));
    QPixmap _gridYZ(ViewerIcon::getPixmap(ViewerIcon::gridYZ));
    QPixmap _axis(ViewerIcon::getPixmap(ViewerIcon::axis));
    QAction * b = toolBar->addAction(_axis,tr("Axis"),this, SLOT(changeAxisDisplayMode()));
    b->setWhatsThis(tr("<b>Axis Visibility</b><br><br>"
    "Change the visibility of the Axis.<br><br>"
    "You can also use Menu <br><b>Tools > Grid > Axis</b><br>"));
    b->setCheckable(true) ;
    b->setChecked(__Axis);
    QObject::connect(this,SIGNAL(AxisDisplayChanged(bool)), b,SLOT(setChecked(bool)));


    b = toolBar->addAction(_gridXY,tr("XY Plane"),this, SLOT(changeXYGridDisplayMode()));
    b->setWhatsThis(tr("<b>XY Plane Visibility</b><br><br>"
    "Change the visibility of the XY Grid.<br><br>"
    "You can also use Menu <br><b>Tools > Grid > XY Plane</b><br>"));
    b->setCheckable(true) ;
    b->setChecked(__XYGrid);
    QObject::connect(this,SIGNAL(XYGridDisplayChanged(bool)),b,SLOT(setChecked(bool)));

    b = toolBar->addAction(_gridXZ,tr("XZ Plane"),this, SLOT(changeXZGridDisplayMode()));
    b->setWhatsThis(tr("<b>XZ Plane Visibility</b><br><br>"
    "Change the visibility of the XZ Grid.<br><br>"
    "You can also use Menu <br><b>Tools > Grid > XZ Plane</b><br>"));
    b->setCheckable(true) ;
    b->setChecked(__XZGrid);
    QObject::connect(this,SIGNAL(XZGridDisplayChanged(bool)),b,SLOT(setChecked(bool)));

    b = toolBar->addAction(_gridYZ,tr("YZ Plane"),this, SLOT(changeYZGridDisplayMode()));
    b->setWhatsThis(tr("<b>YZ Plane Visibility</b><br><br>"
    "Change the visibility of the YZ Grid.<br><br>"
    "You can also use Menu <br><b>Tools > Grid > YZ Plane</b><br>"));
    b->setCheckable(true) ;
    b->setChecked(__YZGrid);
    QObject::connect(this,SIGNAL(YZGridDisplayChanged(bool)),b,SLOT(setChecked(bool)));

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

void ViewGridGL::gridEvent(ViewEvent * _e){
  ViewGridEvent * e = (ViewGridEvent *)(_e);
  bool c = false;
  if((e->arg7 & 1) && (__XYGrid != e->arg1)){
    __XYGrid = e->arg1;
    emit XYGridDisplayChanged(__XYGrid);
    c = true;
  }
  if((e->arg7 & 2) && (__YZGrid != e->arg2)){
    __YZGrid = e->arg2;
    emit YZGridDisplayChanged(__YZGrid);
    c = true;
  }
  if((e->arg7 & 4) && (__XZGrid != e->arg3)){
    __XZGrid = e->arg3;
    emit XZGridDisplayChanged(__XZGrid);
    c = true;
  }
  if((e->arg7 & 8) && (__Axis != e->arg4)){
    __Axis = e->arg4;
    emit AxisDisplayChanged(__Axis);
    c = true;
  }
  if((e->arg7 & 16) && (__gridSize != e->arg5) && e->arg5 > 1){
    __gridSize = e->arg5;
    if (__gridList) { __ogl->glDeleteLists(__gridList,1); __gridList = 0; }
    emit GridSizeChanged(__gridSize);
    c = true;
  }
  if((e->arg7 & 32) && (__gridUnit != e->arg6) && e->arg6 > 1){
    __gridUnit = e->arg6;
    emit GridUnitChanged(__gridUnit);
    c = true;
  }
  if(c)emit valueChanged();
}

/* ----------------------------------------------------------------------- */
