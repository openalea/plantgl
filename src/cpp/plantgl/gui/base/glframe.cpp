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

#include <iomanip>
#include "../gui_config.h"

#include <qcolordialog.h>
#include <qslider.h>
#include <qmessagebox.h>
// #include <qdragobject.h>
#include <qtoolbutton.h>
#include <qtoolbar.h>
#include <qmenu.h>
#include <qcursor.h>
#include <qbitmap.h>
#include <qapplication.h>
#include <qstatusbar.h>
#include <qprogressbar.h>
#include <qclipboard.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qprinter.h>
#include <qprintdialog.h>
#include <qfile.h>
#include <qwhatsthis.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qtabwidget.h>
#include <qvariant.h>
#include <qmainwindow.h>
#include <qurl.h>

#include "glframe.h"
#include "icons.h"
#include "camera.h"
#include "light.h"
#include "grid.h"
#include "rotcenter.h"
#include "clippingplane.h"
#include "fog.h"
#include "errordialog.h"
#include "util_qwidget.h"
#include "configuration.h"
#include "interface/gloptions.h"
#include "interface/linewidthcontrol.h"

#include "event.h"
// #include "editgeomscenegl.h"
#include "scenegl.h"
#include "zbuffer.h"

#include <plantgl/algo/opengl/util_glut.h>

// PGL_USING_NAMESPACE

using namespace std;
// using namespace STDEXT;

#define MS_EDITION
#define GL_ERROR glError(__FILE__,__LINE__)

/*  ------------------------------------------------------------------------ */

ViewGLFrame * ViewGLFrame::LAST_GL_FRAME = NULL;

/*  ------------------------------------------------------------------------ */

/// Create a ViewGLFrame widget
ViewGLFrame::ViewGLFrame( QWidget* parent, const char* name, ViewRendererGL * r, const QGLWidget * shareWidget) :
  QGLWidget(  QGLFormat( QGL::AlphaChannel ), parent, shareWidget ),
  __camera(0),
  __light(0),
  __grid(0),
  __rotCenter(0),
  __clippingPlane(0),
  __fog(0),
  __scene(r),
  __BgColor(170,170,170),
  __linewidth(1),
  __mode(Rotation),
  __lastSelectionMode(Selection),
  __linedialog(0),
  __selectionRect(0)
{
	if(name)setObjectName(name);
  /// Creation
  __camera = new ViewCameraGL(this,"Camera");
  __light = new ViewLightGL(__camera,this,"Light");
  __grid = new ViewGridGL(__camera,this,"Grid");
  __rotCenter = new ViewRotCenterGL(__camera,this,"Rotating Center");
  __clippingPlane = new ViewClippingPlaneGL(this,"Clipping Planes");
  __fog = new ViewFogGL(__camera,this,"Fog");

  LAST_GL_FRAME = this;

  /// Connexion
  rendererStatus();
  __scene->connectTo(this);
  QObject::connect(__scene,SIGNAL(changeMode(ViewGLFrame::Mode)),
				   this,SLOT(setMode(ViewGLFrame::Mode)));
  if(__scene){
    ViewSceneRendererGL * sc = dynamic_cast<ViewSceneRendererGL *>(__scene);
    if(sc){
      sc->setCamera(__camera);
      sc->setLight(__light);
    }
  }
  QObject::connect(__fog,SIGNAL(colorChanged(const QColor&)),
                   this,SLOT(setBackGroundColor(const QColor&)));

  /// Qt Option
  setAcceptDrops(TRUE);
  setFocusPolicy(Qt::StrongFocus);

  ViewerSettings settings;
  settings.beginGroup("FrameGL");
  __BgColor = settings.value("BgColor",__BgColor).value<QColor>();
  setBackGroundColor(__BgColor);
  settings.endGroup();

}


/// Release allocated resources
ViewGLFrame::~ViewGLFrame() {

  if(__scene)delete __scene;
#ifdef  PGL_DEBUG
    cout << "GL Frame deleted" << endl;
#endif
}

void ViewGLFrame::endEvent()
{
  ViewerSettings settings;
  settings.beginGroup("FrameGL");
  settings.setValue("BgColor",__BgColor);
  settings.endGroup();
  if(__scene)__scene->endEvent();
  if(__grid)__grid->endEvent();
  if(__camera)__camera->endEvent();
  if(__light)__light->endEvent();
  if(__rotCenter)__rotCenter->endEvent();
  if(__clippingPlane)__clippingPlane->endEvent();
  if(__fog)__fog->endEvent();
}

/*  ------------------------------------------------------------------------ */

ViewCameraGL *
ViewGLFrame::getCamera() const
{
  return __camera;
}

ViewLightGL *
ViewGLFrame::getLight() const
{
  return __light;
}

ViewGridGL *
ViewGLFrame::getGrid() const
{
  return __grid;
}

ViewRotCenterGL *
ViewGLFrame::getRotCenter() const
{
  return __rotCenter;
}

ViewClippingPlaneGL *
ViewGLFrame::getClippingPlanes() const
{
  return __clippingPlane;
}

ViewFogGL *
ViewGLFrame::getFog() const
{
  return __fog;
}

ViewRendererGL *
ViewGLFrame::getSceneRenderer() const
{
  return __scene;
}

void
ViewGLFrame::setSceneRenderer(ViewRendererGL * s)
{
  if(__scene) delete __scene;
  __scene = s;
  __scene->connectTo(this);
  QObject::connect(__scene,SIGNAL(changeMode(ViewGLFrame::Mode)),
				   this,SLOT(setMode(ViewGLFrame::Mode)));
  if(__scene){
    ViewSceneRendererGL * sc = dynamic_cast<ViewSceneRendererGL *>(__scene);
    if(sc){
      sc->setCamera(__camera);
      sc->setLight(__light);
    }
  }
  rendererStatus();
  emit rendererChanged();
}

void
ViewGLFrame::rendererStatus() {
  if(__scene)
  emit initMessage(QString("Renderer ... ")+__scene->metaObject()->className());
  else 
  emit initMessage(QString("Renderer ... None"));
}


const QColor&
ViewGLFrame::getBackGroundColor() const
{
  return __BgColor;
}



/*  ------------------------------------------------------------------------ */


void
ViewGLFrame::connectTo(ViewStatusBar *s)
{
	if(s){
		QObject::connect(this,SIGNAL(statusMessage(const QString&,int)),
			s,SLOT(showMessage(const QString&,int)) );  
		QObject::connect(this,SIGNAL(statusMessage(const QString&)),
			s,SLOT(showMessage(const QString&)) );  
		QObject::connect(this,SIGNAL(progressMessage(int,int)),
			s,SLOT(setProgress(int,int)) );  
		__camera->connectTo(s);
		__light->connectTo(s);
		__grid->connectTo(s);
		__rotCenter->connectTo(s);
		__clippingPlane->connectTo(s);
		__fog->connectTo(s);
	}
}

void
ViewGLFrame::connectTo(ViewErrorDialog *e)
{
  if(e){
    QObject::connect(this,SIGNAL(errorMessage(const QString&)),
		     e,SLOT(setError(const QString&)) );
    QObject::connect(this,SIGNAL(warningMessage(const QString&)),
		     e,SLOT(appendWarning(const QString&)) );
    QObject::connect(this,SIGNAL(infoMessage(const QString&)),
		     e,SLOT(appendInfo(const QString&)) );
  __camera->connectTo(e);
  __light->connectTo(e);
  __grid->connectTo(e);
  __rotCenter->connectTo(e);
  __clippingPlane->connectTo(e);
  __fog->connectTo(e);
  }
}

void 
ViewGLFrame::error(const QString& s)
{
  emit errorMessage(s);
}

void 
ViewGLFrame::warning(const QString& s)
{
  emit warningMessage(s);
}

void 
ViewGLFrame::info(const QString& s)
{
  emit infoMessage(s);
}

void 
ViewGLFrame::status(const QString& s)
{
  emit statusMessage(s);
}

void 
ViewGLFrame::status(const QString& s,int t)
{
  emit statusMessage(s,t);
}

void 
ViewGLFrame::progress(int p,int t)
{
  emit progressMessage(p,t);
}

/*  ------------------------------------------------------------------------ */

void
ViewGLFrame::setBackground()
{
  QColor m = QColorDialog::getColor(__BgColor,this);
  if(m.isValid()){
    __BgColor=m;
  __fog->setColor(m);
    qglClearColor( __BgColor );
    updateGL();
  }
}

void
ViewGLFrame::setBackGroundColor(const QColor& color)
{
  __BgColor=color;
  qglClearColor(__BgColor);
  __fog->setColor(color);
  updateGL();
  status(QString(tr("Set Background Color to")+" (%1,%2,%3)").arg(color.red()).arg(color.green()).arg(color.blue()),2000);
}

bool ViewGLFrame::glError(const char * file , int line ){
  return ViewObjectGL::glError(this,__FILE__,__LINE__);
}

void
ViewGLFrame::setLineWidth(int i)
{
  __linewidth = (GLfloat)float(i);
  status(QString(tr("Set Line Width to")+" %1").arg(i),2000);
  updateGL();
}

void 
ViewGLFrame::setMultipleSelectionMode()
{
  if(__mode != MultipleSelection){
	  setCursor(QCursor( Qt::CrossCursor  ));
    __mode = MultipleSelection;
    __scene->changeModeEvent(__mode);
	__lastSelectionMode = __mode;
    emit modeChanged(__mode);
	emit selectionMode(true); 
    status(tr("Mode Multiple Selection"),2000);
  }
}

void
ViewGLFrame::setSelectionMode()
{
  if(__mode != Selection){
	  setCursor(QCursor( Qt::PointingHandCursor ));
    __mode = Selection;
    __scene->changeModeEvent(__mode);
	__lastSelectionMode = __mode;
    emit modeChanged(__mode);    
	emit selectionMode(true); 
    status(tr("Mode Selection"),2000);
  }
}

void
ViewGLFrame::setRotationMode()
{
  if(__mode !=Rotation){
	  setCursor( Qt::ArrowCursor);
    __mode = Rotation;
    __scene->changeModeEvent(__mode);
    emit modeChanged(__mode);
	emit selectionMode(false); 
  }
}

void 
ViewGLFrame::setLastSelectionMode()
{
  setMode(__lastSelectionMode);
}

void 
ViewGLFrame::setMode(Mode i)
{
  if(__mode !=i){
    __mode = i;
    if(__mode == Rotation){
      setCursor(Qt::ArrowCursor);
    }
    else if(__mode == Selection){
      setCursor(QCursor( Qt::PointingHandCursor ));
      status(tr("Mode Selection"),2000);
    }
    else if(__mode == MultipleSelection){
      setCursor(QCursor(Qt::CrossCursor  ));
      status(tr("Mode Multiple Selection"),2000);
    }
    __scene->changeModeEvent(__mode);
    emit modeChanged(__mode);
	emit selectionMode(__mode == Selection || __mode == MultipleSelection);
	__lastSelectionMode = __mode;
  }
}
/*
void 
ViewGLFrame::changeMode()
{
  if(__mode == true){
    setCursor(Qt::ArrowCursor);
    __mode = false;
    __scene->rotationModeEvent();
    status(QString("Mode Rotation"),2000);
  }
  else {
      setCursor(QCursor( 13 ));
    __mode = true;
    __scene->selectionModeEvent();
    status(QString("Mode Selection"),2000);
  }
  emit modeChanged(__mode);
}
*/
void 
ViewGLFrame::clearSelection()
{
  // __selected_shapes.clear();
  __scene->clearSelectionEvent();
  status(tr("Selection cleared"),5000);
}

/*  ------------------------------------------------------------------------ */

/*!
  Set up the OpenGL rendering state, and define display list
*/
void ViewGLFrame::initializeGL()
{
  // clears the current GL context
  qglClearColor( __BgColor );

  /*
    Light settings
    IMPORTANT - GL_NORMALIZE is enabled because GL needs to re-normalize
    normals as we use anisotropic transformation such as scaling.
  */
  glEnable(GL_NORMALIZE);
  __light->initializeGL();
  glShadeModel(GL_SMOOTH);
// glShadeModel(GL_FLAT);
// glEnable(GL_DITHER);
//  glEnable(GL_LINE_SMOOTH);
//  glEnable(GL_POINT_SMOOTH);
//  glEnable(GL_POLYGON_SMOOTH);
//  glEnable(GL_CULL_FACE);
//  glCullFace(GL_FRONT);
//  glCullFace(GL_BACK);
  /*
    Hidden-surfaces settings (ZBuffer)
  */
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER,0.01);

  glEnable(GL_LIGHTING);
//  __light->enable();
  glEnable(GL_BLEND);
  __camera->initializeGL();
  __grid->initializeGL();
  __rotCenter->initializeGL();
  __clippingPlane->initializeGL();
  __fog->initializeGL();
  __scene->initializeGL();

}



/*!
  Set up the OpenGL view port, matrix mode, etc.
  This function is called by Viewer::show() and when a window resize is done
  by the user.
*/

void ViewGLFrame::resizeGL( int w, int h )
{
  __camera->resizeGL(w,h);
}



/*!
  Paint the box. The actual openGL commands for drawing the box are
  performed here.
*/

void ViewGLFrame::paintGL()
{
  GL_ERROR;
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  __camera->paintGL();
  __light->paintGL();
  __fog->paintGL();
  glLineWidth(GLfloat(1.0));
  glPointSize(GLfloat(1.0));

  __rotCenter->paintGL();
  __clippingPlane->paintGL();
  __light->disable();
  __grid->paintGL();

  GL_ERROR;

  glLineWidth(__linewidth);
  glPointSize(__linewidth);

  if(__scene){
	__scene->paintGL();
	if(GL_ERROR)__scene->clear();
  }

  if(__mode == MultipleSelection && __selectionRect){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,width(),0,height(),-50,50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glColor3f(0.2,0.2,0.8);
	glEnable (GL_LINE_STIPPLE);
	glLineStipple(1,0x0FFF);
	glBegin(GL_LINE_LOOP);
	int x = __selectionRect->x();
	int y = height() - __selectionRect->y();
	glVertex3f(x,y,0);
	glVertex3f(x+__selectionRect->width(),y,0);
	glVertex3f(x+__selectionRect->width(),y-__selectionRect->height(),0);
	glVertex3f(x,y-__selectionRect->height(),0);
	glEnd();
	glDisable (GL_LINE_STIPPLE);
  }


  glFinish();


}


/*!
  SelectGL
*/

void ViewGLFrame::selectGL()
{
  makeCurrent();

  GLint hits;
  GLsizei bufsize = 512;
  GLuint selectBuf[512];

  glSelectBuffer(bufsize,selectBuf);
  (void)glRenderMode(GL_SELECT);

  GL_ERROR;

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // transformations induites dans le repere du labo a partir
  // des coord de la souris dans le repere de la GL

  __camera->beginSelectGL(__mouse);

  glLineWidth(1);
  glPointSize(1);

  __rotCenter->paintGL();
  __clippingPlane->paintGL();

  glLineWidth(__linewidth);
  glPointSize(__linewidth);

  GL_ERROR;

  if (__scene)__scene->selectGL();

  __camera->endSelectGL();

  GL_ERROR;

  hits = glRenderMode(GL_RENDER);
  GLuint names, *ptr;
  ptr = (GLuint *) selectBuf;
  uint32_t val, selectedval;
  float z, zmin;

  if(hits > 0){
    for(uint32_t i = 0 ; i < (uint32_t)hits ; i++){
      names = *ptr; //    cerr << "Number of names for this hits = " << names << endl;
      ptr++;
      z = (float)*ptr;        //    cerr << "z1 = " << *ptr << endl;
      if(i == 0)zmin = z;
      ptr++;        //    cerr << "z2 = " << *ptr << endl;
      ptr++;
      val = (uint32_t)*ptr;
      if(i == 0)selectedval = val;
      else if(z < zmin){
	selectedval = val;
	zmin = z;
      }
      for(unsigned int j = 0 ; j < names ; j++)ptr++;
    }
    __scene->selectionEvent(selectedval);
    emit selectedShape(selectedval);
  }
#ifdef GEOM_DEBUG
  else warning("*** WARNING : hit miss all shapes");
#endif
}

void ViewGLFrame::multipleSelectGL(const QPoint& p)
{
  makeCurrent();

  GLint hits;
  GLsizei bufsize = 40960;
  GLuint selectBuf[40960];
  
  glSelectBuffer(bufsize,selectBuf);
  (void)glRenderMode(GL_SELECT);

  GL_ERROR;

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // transformations induites dans le repere du labo a partir 
  // des coord de la souris dans le repere de la GL

  __camera->beginSelectGL(QRect(min(__mouse.x(),p.x()),
								min(__mouse.y(),p.y()),
								abs(__mouse.x()-p.x()),
								abs(__mouse.y()-p.y())));
  
  glLineWidth(1);
  glPointSize(1);

  __rotCenter->paintGL();
  __clippingPlane->paintGL();
  
  glLineWidth(__linewidth);
  glPointSize(__linewidth);
  
  GL_ERROR;

  if (__scene)__scene->selectGL();

  __camera->endSelectGL();

  GL_ERROR;

  hits = glRenderMode(GL_RENDER);
  GLuint names, *ptr;
  ptr = (GLuint *) selectBuf;
  uint32_t val;
  float z;

  vector<uint32_t> vals;
  if(hits > 0){
    for(uint32_t i = 0 ; i < (uint32_t)hits ; i++){
      names = *ptr; //    cerr << "Number of names for this hits = " << names << endl;
      ptr++;
      z = (float)*ptr;        //    cerr << "z1 = " << *ptr << endl;
      ptr++;        //    cerr << "z2 = " << *ptr << endl;
      ptr++;
      val = (uint32_t)*ptr;
	  vals.push_back(val);
      for(unsigned int j = 0 ; j < names ; j++)ptr++;
    }
	if(!vals.empty()){
	  if(vals.size()==1){
		__scene->selectionEvent(vals[0]);
		emit selectedShape(vals[0]);
	  }
	  else {
		__scene->selectionEvent(vals);
		emit selectedShapes(vals);
	  }
	}
  }
#ifdef GEOM_DEBUG
  else warning("*** WARNING : hit miss all shapes");
#endif
}

ViewRayBuffer * 
ViewGLFrame::castRays( const Vector3& position, 
						  const Vector3& direction, 
						  const Vector3& dx, 
						  const Vector3& dy,
						  int sx, int sy)  {
	ViewRayBuffer * res = new ViewRayBuffer(sx,sy);

	GLint hits;
	GLsizei bufsize = 40960;
	GLuint selectBuf[40960];

	makeCurrent();
	Vector3 oldpos = __camera->getPosition();
	Vector3 olddir = __camera->getDirection();
	bool mode = __camera->getProjectionMode();
	if(mode)__camera->setOrthographicMode();

	size_t nbray = sx*sy;
	for(size_t i = 0; i < sx; i++){
		for(size_t j = 0; j < sy; j++){
			Vector3 delta = dx*i+dy*j;
			__camera->lookIn(position+delta,direction);
  
			glSelectBuffer(bufsize,selectBuf);
			(void)glRenderMode(GL_SELECT);
			GL_ERROR;

			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			__camera->beginSelectGL(QPoint(width()/2,height()/2));
			__clippingPlane->paintGL();

			glLineWidth(__linewidth);
			glPointSize(__linewidth);
			GL_ERROR;

			if (__scene)__scene->selectGL();
			__camera->endSelectGL();

			GL_ERROR;
			hits = glRenderMode(GL_RENDER);

			if(hits > 0){
				res->setAt(i,j,selectBuf,hits,position+delta);
				GL_ERROR;
				RayHitList& l = res->getAt(i,j);
				for(RayHitList::iterator _it = l.begin();_it != l.end(); _it++)
					_it->id = __scene->translateId(_it->id);
			}
			size_t numray = i*sy+j;
			if (numray % 20 == 0){
				std::cerr << "\x0d Ray " <<numray << " of " << nbray << std::flush;
				progress(numray,nbray);
			}
		}
	}
	std::cerr << "\x0d Ray " <<nbray << " of " << nbray << std::endl;
	progress(nbray,nbray);
	__camera->lookAt(oldpos,oldpos+olddir);
	if(mode)__camera->setProjectionMode(mode);
	return res;
}

ViewZBuffer * 
ViewGLFrame::grabZBuffer( bool all_values  )
{
    makeCurrent();
	return ViewZBuffer::importglZBuffer(all_values);
}

ViewZBuffer * 
ViewGLFrame::grabDepthBuffer( bool all_values )
{
    makeCurrent();
	return ViewZBuffer::importglDepthBuffer(all_values);
}

double ViewGLFrame::getPixelWidth(){
	bool mode = __camera->getProjectionMode();
	if(mode)__camera->setOrthographicMode();
	makeCurrent();
	GLint viewport[4];
	GLdouble modelMatrix[16], projMatrix[16];
	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
	GLdouble winx = viewport[2]/2;
	GLdouble winy = viewport[3]/2;
	GLdouble objx,objx2,objy,objy2,objz,objz2 ;
	if( !gluUnProject(0.0,10.0, 0.0, modelMatrix, projMatrix, viewport,
					 &objx,&objy, &objz) == GL_TRUE  || 
		!gluUnProject(1.0,10.0, 0.0, modelMatrix, projMatrix, viewport,
					 &objx2,&objy2, &objz2) == GL_TRUE  )return -1;
	double pixelsize = sqrt(pow(objx-objx2,2)+pow(objy-objy2,2)+pow(objz-objz2,2));
	if(mode)__camera->setProjectionMode(mode);
	return pixelsize;
}

int ViewGLFrame::getProjectionPixel(){
	int gridstate = __grid->getState();
	__grid->setState(0);
	bool mode = __camera->getProjectionMode();
	if(mode)__camera->setOrthographicMode();
	else if(gridstate != 0)updateGL();

	int w = width();
	int h = height();

	int nbpix = w*h;
	float  * zvalues = new float[nbpix];
	glReadBuffer(GL_FRONT);
	glReadPixels(0,0,w,h,GL_DEPTH_COMPONENT, GL_FLOAT, zvalues);

	uint32_t projpix = 0;
	for(uint32_t i = 0; i < nbpix; ++i)
		if(0 < zvalues[i] && zvalues[i] < 1) projpix++;

	delete [] zvalues;

	__grid->setState(gridstate);
	if(mode)__camera->setProjectionMode(mode);
	else if(gridstate != 0)updateGL();
	return projpix;
}

double ViewGLFrame::getProjectionSize(int* nbpixel, double* pixelwidth){
	bool mode = __camera->getProjectionMode();
	if(mode)__camera->setOrthographicMode();
	int nbpix = getProjectionPixel();
	double w = getPixelWidth();
	double res = nbpix * w * w;
	if(mode)__camera->setProjectionMode(mode);
	if(nbpixel)*nbpixel = nbpix;
	if(pixelwidth)*pixelwidth=w;
	return res;
}

void ViewGLFrame::printProjectionSize(){
	bool mode = __camera->getProjectionMode();
	if(mode)__camera->setOrthographicMode();
	float pixelwidth = getPixelWidth();
	int projpix = getProjectionPixel();
	if(mode)__camera->setProjectionMode(mode);
	float projsurf = pixelwidth * pixelwidth * projpix;
	status("Projection =  "+QString::number(projpix)+
		   " pixels ; Pixel Width = "+QString::number(pixelwidth)+
		   " (Screen = "+QString::number(pixelwidth*width())+" x "+QString::number(pixelwidth*height())+
		   " ) ; Projection Size = "+QString::number(projsurf));
}

/*  ------------------------------------------------------------------------ */

void ViewGLFrame::mousePressEvent( QMouseEvent* event)
{

  __mouse = event->pos();

  if(__mode == Selection){
    selectGL();
  }
  else if(__mode == MultipleSelection){
	status(QString(tr("Mouse on")+"(%1,%1)").arg(__mouse.x()).arg(__mouse.y()),2000);
	__selectionRect = new QRect(__mouse.x(),__mouse.y(),0,0);
  }
  else if(__mode == Rotation){
	  if(event->modifiers() & Qt::ControlModifier )
      {
		QBitmap bm = QBitmap::fromData(QSize(32,32),ViewerIcon::LIGHT_BITS,QImage::Format_MonoLSB );
        QBitmap mask = QBitmap::fromData(QSize(32,32),ViewerIcon::LIGHT_MASK,QImage::Format_MonoLSB);
        setCursor(QCursor(bm,mask));
      }
	  else if(event->modifiers() & Qt::ShiftModifier)
      {
		setLastSelectionMode();
		if(__mode == Selection)selectGL();
      }
	else if(event->button()==Qt::LeftButton)
      {
        QBitmap bm= QBitmap::fromData(QSize(32,32),ViewerIcon::ROTATE_BITS,QImage::Format_MonoLSB);
        QBitmap mask= QBitmap::fromData(QSize(32,32),ViewerIcon::ROTATE_MASK,QImage::Format_MonoLSB);
        setCursor(QCursor(bm,mask));
      }
    else if(event->button()==Qt::MidButton)
      {
        QBitmap bm= QBitmap::fromData(QSize(32,32),ViewerIcon::ZOOM_BITS,QImage::Format_MonoLSB);
        QBitmap mask= QBitmap::fromData(QSize(32,32),ViewerIcon::ZOOM_MASK,QImage::Format_MonoLSB);
        setCursor(QCursor(bm,mask));
      }

    else if(event->button()==Qt::RightButton)
      {
		  setCursor(Qt::SizeAllCursor);
      }
  }
  updateGL();
}

void ViewGLFrame::mouseReleaseEvent( QMouseEvent* event)
{
  if(__mode == Selection){
	if(!__scene->endSelect())setRotationMode();
  }
  else if(__mode == MultipleSelection){
	delete __selectionRect;
	__selectionRect = 0;
	QPoint mouse = event->pos();
	multipleSelectGL(mouse);
	setRotationMode();
  }
  else setCursor(Qt::ArrowCursor);

  updateGL();
}

void ViewGLFrame::mouseMoveEvent( QMouseEvent* event)
{
  QPoint mouse = event->pos();
  if(__mode == MultipleSelection){
	status(QString(tr("Selection from")+" (%1,%1) "+tr("to")+" (%1,%1)")
			.arg(__mouse.x()).arg(__mouse.y()).arg(mouse.x()).arg(mouse.y()),2000);
	if(__selectionRect)
	  *__selectionRect = QRect(min(__mouse.x(),mouse.x()),
								min(__mouse.y(),mouse.y()),
								abs(__mouse.x()-mouse.x()),
								abs(__mouse.y()-mouse.y()));
	else 
	__selectionRect = new QRect(min(__mouse.x(),mouse.x()),
								min(__mouse.y(),mouse.y()),
								abs(__mouse.x()-mouse.x()),
								abs(__mouse.y()-mouse.y()));
	repaint();
  }
  else if(__mode == Selection || event->modifiers() & Qt::ShiftModifier)
    {
      if(__scene){
        if(event->buttons() & Qt::LeftButton)
          {
            __scene->rotate(__mouse-mouse);
          }
        else if(event->buttons() & Qt::RightButton)
          {
            __scene->move(__mouse-mouse);
          }
		else if(event->buttons() & Qt::MidButton)
          {
            __scene->zoom(__mouse-mouse);
          }
      }
	  __mouse = mouse;
	  updateGL();
    }
  else if(event->modifiers() & Qt::ControlModifier)
    {
      if(event->buttons() & Qt::LeftButton)
        {
          __light->rotate(__mouse-mouse);
        }
      else if(event->buttons() & Qt::RightButton)
        {
          __light->move(__mouse-mouse);
        }
	  else if(event->buttons() & Qt::MidButton)
	{
	  __light->zoom(__mouse-mouse);
	}
	__mouse = mouse;
	updateGL();
    }
  else {
	  if(event->buttons() & Qt::LeftButton)
      {
        __camera->rotate(__mouse-mouse);
      }
    else if(event->buttons() & Qt::RightButton)
      {
        __camera->move(__mouse-mouse);
    }
    else if(event->buttons() & Qt::MidButton)
      {
        __camera->zoom(__mouse-mouse);
      }
	__mouse = mouse;
	updateGL();
  }
}

void ViewGLFrame::dragEnterEvent(QDragEnterEvent* myevent){
	if (myevent->mimeData()->hasUrls()){
		myevent->accept();
	}
	else myevent->ignore();
}

void ViewGLFrame::dropEvent(QDropEvent* myevent){
	if(__scene){
		QList<QUrl> urls = myevent->mimeData()->urls();
		QList<QUrl>::const_iterator itUrls = urls.constBegin();
		if(myevent->dropAction() == Qt::CopyAction)
			__scene->openFile(itUrls->toLocalFile());
		else
			__scene->addFile(itUrls->toLocalFile());
		for(++itUrls;itUrls != urls.constEnd();++itUrls ){
			__scene->addFile(itUrls->toLocalFile());
		}
	}
}

#ifndef WHEEL_DELTA
#define WHEEL_DELTA 120
#endif

void 
ViewGLFrame::wheelEvent ( QWheelEvent * e){
  if(__mode == Selection || e->modifiers() & Qt::ShiftModifier){
	  __scene->zooming(0,e->delta()/WHEEL_DELTA);
      // __scene->rotating(0,e->delta()*20/WHEEL_DELTA);
      e->accept();
  }
  else if(e->modifiers() & Qt::ControlModifier){
      __light->zooming(0,e->delta()/WHEEL_DELTA);
      // __light->rotating(0,e->delta()*20/WHEEL_DELTA);
      e->accept();
  }
  else {
	  __camera->zooming(0,e->delta()/WHEEL_DELTA);
      // __camera->rotating(0,e->delta()*20/WHEEL_DELTA);
      e->accept();
  }
  updateGL();
}

void
ViewGLFrame::keyPressEvent ( QKeyEvent * e)
{
  if(e->modifiers() & Qt::ShiftModifier){
    if( e->key() == Qt::Key_Up){
      __scene->rotating(0,2);
      e->accept();
      updateGL();
    }
	else if( e->key() == Qt::Key_Down){
      __scene->rotating(0,-2);
      e->accept();
      updateGL();
    }
    else if( e->key() == Qt::Key_Left){
      __scene->rotating(2,0);
      e->accept();
      updateGL();
    }
    else if( e->key() == Qt::Key_Right){
      __scene->rotating(-2,0);
      e->accept();
      updateGL();
    }
    else {
      setSelectionMode();
      e->accept();
    }
  }
  else if(e->modifiers() & Qt::ControlModifier){
    if( e->key() == Qt::Key_Up){
      __light->rotating(0,4);
      updateGL();
    }
    else if( e->key() == Qt::Key_Down){
      __light->rotating(0,-4);
      updateGL();
    }
    else if( e->key() == Qt::Key_Left){
      __light->rotating(4,0);
      updateGL();
    }
    else if( e->key() == Qt::Key_Right){
      __light->rotating(-4,0);
      updateGL();
    }
    else  {
	 QBitmap bm = QBitmap::fromData(QSize(32,32),ViewerIcon::LIGHT_BITS,QImage::Format_MonoLSB);
     QBitmap mask = QBitmap::fromData(QSize(32,32),ViewerIcon::LIGHT_MASK,QImage::Format_MonoLSB);
     setCursor(QCursor(bm,mask));
	 e->accept();
	}
  }
  else {
    if( e->key() == Qt::Key_Up){
      __camera->zooming(0,1);
      updateGL();
      e->accept();
    }
    else if( e->key() == Qt::Key_Down){
      __camera->zooming(0,-1);
      updateGL();
      e->accept();
    }
    else if( e->key() == Qt::Key_Left){
      __camera->rotating(4,0);
      updateGL();
      e->accept();
    }
    else if( e->key() == Qt::Key_Right){
      __camera->rotating(-4,0);
      updateGL();
      e->accept();
    }
    else if( e->key() == Qt::Key_PageUp){
      __camera->rotating(0,-4);
      updateGL();
      e->accept();
    }
    else if( e->key() == Qt::Key_PageDown){
      __camera->rotating(0,4);
      updateGL();
      e->accept();
    }
    else if( e->key() == Qt::Key_Home)
      {
      e->ignore();
    }
    else if( e->key() == Qt::Key_End)
      {
      e->ignore();
    }
    else if( e->key() == Qt::Key_N)
      {
      __camera->nextView();
      e->accept();
    }
    else if( e->key() == Qt::Key_P)
      {
      printProjectionSize();
      e->accept();
    }
    else e->ignore();
  }
}

void
ViewGLFrame::keyReleaseEvent ( QKeyEvent * e)
{
  if(e->modifiers() & Qt::ShiftModifier){
    setRotationMode();
    e->accept();
  }
  else if(e->modifiers() & Qt::ControlModifier){
	setCursor(Qt::ArrowCursor);
	e->accept();
  }
}

// #define DEBUG_EVENTDISPATCH

bool ViewGLFrame::event(QEvent *e){
#ifdef DEBUG_EVENTDISPATCH
	if(e->type() >= ViewEvent::eFirstEvent && e->type() <= ViewEvent::eLastEvent)
		printf("** gl ** receive pgl event\n");
	// else printf("** gl ** receive event\n");
#endif
	return QGLWidget::event(e);
}

void ViewGLFrame::customEvent(QEvent *e)
{
	if(e->type() == ViewEvent::eSceneChange){
    ViewSceneChangeEvent * event = (ViewSceneChangeEvent *)e;
    __scene->sceneChangeEvent(event);
  }
}

void ViewGLFrame::focusInEvent ( QFocusEvent * e) {
//  if(e->reason()== QFocusEvent::ActiveWindow)
  __scene->checkFileModification();
}

void ViewGLFrame::gridEvent ( ViewGridEvent * e) {
  __grid->gridEvent(e);
}

void ViewGLFrame::cameraEvent ( ViewEvent * e) {
  __camera->cameraEvent(e);
}

void ViewGLFrame::clippingPlaneEvent(ViewEvent * e)
{
	__clippingPlane->clippingPlaneEvent(e);
}

/*  ------------------------------------------------------------------------ */


void ViewGLFrame::saveImage(QString _filename,const char * _format, bool withAlpha){
	grabFrameBuffer(withAlpha).save(_filename,_format);
	status(tr("Save screenshot with format")+" \""+QString(_format)+"\" "+tr("in")+" \""+_filename+'"',10000);
}

void ViewGLFrame::copyImageToClipboard(){
	if(QApplication::clipboard()){
		QApplication::clipboard()->setImage(grabFrameBuffer(false));
		status(tr("Copy screenshot to clipboard"),10000);
	}
	else QMessageBox::warning(this,tr("System Error"),tr("Cannot access global clipboard"),"Ok");
}

void ViewGLFrame::copyImageToClipboardWithAlpha(){
	if(QApplication::clipboard()){
		QApplication::clipboard()->setImage(grabFrameBuffer(true));
		status(tr("Copy screenshot to clipboard with alpha channel"),10000);
	}
	else QMessageBox::warning(this,tr("System Error"),tr("Cannot access global clipboard"),"Ok");
}

void ViewGLFrame::printImage(){
  QPrinter printer;
  QPrintDialog dialog(&printer, this);
  if (dialog.exec()){
	QPainter paint;
	if(!paint.begin(&printer))return;
	QPoint origin(0,0);
	QRect r = printer.pageRect();
	QImage img = grabFrameBuffer(false);
	QSize r2 = img.size();
	double x = min((double)r.width()/(double)r2.width(),
				   (double)r.height()/(double)r2.height());
	x *= 0.8;
	
	paint.translate(((double)r.width() - r2.width()*x)/2.0,
					((double)r.height() - r2.height()*x)/2.0);
	paint.scale(x,x);
	paint.drawImage(QPoint(0,0),img);
	paint.setPen( QPen(Qt::black,1) );
	paint.drawRect(QRect(QPoint(0,0),r2));
	paint.end();
  }
}


QMenu *
ViewGLFrame::createEditMenu(QWidget * parent)
{
  QMenu * menu = new QMenu(parent);
  __scene->addEditEntries(menu);
  QPixmap wizard( ViewerIcon::getPixmap(ViewerIcon::wizard) );
  menu->addAction(wizard,tr("Selection"),this, SLOT(setSelectionMode()));
  QPixmap wizardRectIcon( ViewerIcon::getPixmap(ViewerIcon::wizardrect) );
  menu->addAction(wizardRectIcon,tr("Rectangle Selection"),this, SLOT(setMultipleSelectionMode()));

  QPixmap notwizard( ViewerIcon::getPixmap(ViewerIcon::notwizard) );
  menu->addAction(notwizard,tr("Clear Selection"),this, SLOT(clearSelection()));
  menu->addSeparator();

  if(__linedialog){
    QPixmap lineIcon( ViewerIcon::getPixmap(ViewerIcon::line_width) );
    QAction * action= menu->addAction(lineIcon,tr("Edit Line Width"),__linedialog, SLOT(changeVisibility()));
    QObject::connect(__linedialog,SIGNAL(__visibilityChanged(bool)),action,SLOT(setChecked(bool)));
  }
  return menu;
}

QMenu *
ViewGLFrame::createToolsMenu(QWidget * parent)
{

  QMenu * menu = new QMenu(parent);
  QPixmap wheel(ViewerIcon::getPixmap(ViewerIcon::wheel));
  QMenu * __RendererMenu = __scene->createToolsMenu(menu);  
  menu->addMenu(__RendererMenu);
  __RendererMenu->setTitle(tr("Renderer"));
  __RendererMenu->setIcon(QIcon(wheel));
  menu->addSeparator();
  QMenu * __CameraMenu = __camera->createToolsMenu(menu);
  menu->addMenu(__CameraMenu);
  __CameraMenu->setTitle(tr("Camera"));
  __CameraMenu->setIcon(QIcon(wheel));
  menu->addSeparator();
  QMenu * __LightMenu = __light->createToolsMenu(menu);
  menu->addMenu(__LightMenu);
  __LightMenu->setTitle(tr("Light"));
  __LightMenu->setIcon(QIcon(wheel));
  menu->addSeparator();
  QMenu * __FogMenu = __fog->createToolsMenu(menu);
  menu->addMenu(__FogMenu);
  __FogMenu->setTitle(tr("Fog"));
  __FogMenu->setIcon(QIcon(wheel));
  menu->addSeparator();
  QMenu * __GridMenu = __grid->createToolsMenu(menu);
  menu->addMenu(__GridMenu);
  __GridMenu->setTitle(tr("Grid"));
  __GridMenu->setIcon(QIcon(wheel));
  menu->addSeparator();
  QMenu * __CPMenu = __clippingPlane->createToolsMenu(menu);
  menu->addMenu(__CPMenu);
  __CPMenu->setTitle(tr("Clipping Plane"));
  __CPMenu->setIcon(QIcon(wheel));
  menu->addSeparator();
  QMenu * __RotCMenu = __rotCenter->createToolsMenu(menu);
  menu->addMenu(__RotCMenu);
  __RotCMenu->setTitle(tr("Rotating Center"));
  __RotCMenu->setIcon(QIcon(wheel));
  menu->addSeparator();
  
  QPixmap coloricon(ViewerIcon::getPixmap(ViewerIcon::color));
  menu->addAction(coloricon,tr("Background Color"),  this,SLOT(setBackground())); 
  return menu;
}


void
ViewGLFrame::fillToolBar(QToolBar * toolBar)
{
  QPixmap wizardIcon( ViewerIcon::getPixmap(ViewerIcon::wizard) );
  QPixmap wizardRectIcon( ViewerIcon::getPixmap(ViewerIcon::wizardrect) );
  QPixmap wizardIconMenu( ViewerIcon::getPixmap(ViewerIcon::wizardmenu) );

  ViewDoubleToolButton * bt2 = new ViewDoubleToolButton( wizardIcon,  wizardRectIcon,tr("Selection"), this, SLOT(setLastSelectionMode()), toolBar);
  QString wizardtext =tr("<b>Selection</b><br><br>Allows you to select shapes from the scene<br><br>"
	"Press deeply this button to have the choice between two modes of selection<br><br>"
	"<b>Point selection</b> allows you to select one particular shapes<br>"
	"<b>Rectangle selection</b> allows you to select all the shapes displayed inside a rectangle<br><br>"
	"Selecting two times the same shape unselect it.<br>");
  bt2->setWhatsThis(wizardtext);
  QMenu * selctmenu = new QMenu(toolBar);
  selctmenu->addAction(wizardIcon,tr("Point"),this,SLOT(setSelectionMode()));
  selctmenu->addAction(wizardRectIcon,tr("Rectangle"),this,SLOT(setMultipleSelectionMode()));
  bt2->setMenu(selctmenu);

  QObject::connect(this,SIGNAL(selectionMode(bool)),bt2,SLOT(setChecked(bool)));
  QObject::connect(this,SIGNAL(modeChanged(ViewGLFrame::Mode)),bt2,SLOT(setButton(ViewGLFrame::Mode)));
  toolBar->addWidget(bt2);

  QPixmap notwizardIcon( ViewerIcon::getPixmap(ViewerIcon::notwizard) );
  QString notwizardtext =tr("Clear Selection");

  QAction * bt = toolBar->addAction( notwizardIcon, notwizardtext, this, SLOT(clearSelection()));
  bt->setWhatsThis(tr("<b>Clear Selection</b><br><br>Clear the current selection."));

  toolBar->addSeparator();
  __scene->fillToolBar(toolBar);
  __camera->fillToolBar(toolBar);
  __light->fillToolBar(toolBar);
  __grid->fillToolBar(toolBar);
  __rotCenter->fillToolBar(toolBar);
  if(__linedialog){
    QPixmap lineIcon( ViewerIcon::getPixmap(ViewerIcon::line_width) );
    QString linetext =tr("Line Width");
	bt = toolBar->addAction( lineIcon, linetext, __linedialog, SLOT(changeVisibility()));
    bt->setCheckable(true);
    bt->setChecked(__linedialog->isVisible());
    QObject::connect(__linedialog,SIGNAL(__visibilityChanged(bool)),bt,SLOT(setChecked(bool)));
	bt->setWhatsThis(tr("<b>Line Width</b><br><br>Control of the width of the lines and the points of the scene. By default, this value is egal to one."));
  }
  QPixmap coloricon(ViewerIcon::getPixmap(ViewerIcon::color));
  bt = toolBar->addAction(coloricon,tr("Background Color"), this,SLOT(setBackground()));
  bt->setWhatsThis(tr("<b>Background Color</b><br><br>Change the background color of the 3D display."));
}

void
ViewGLFrame::addOtherToolBar(QMainWindow * menu)
{
  __linedialog  = new ViewToolBar(tr("Line Width Bar"),menu,"LineWidthBar");
  Ui::LineWidthControl form;
  QWidget * widget = new QWidget(__linedialog);
  form.setupUi(widget);
  __linedialog->addWidget(widget);
  QObject::connect (form.LineWidthSlider,SIGNAL(valueChanged(int)), this,SLOT(setLineWidth(int)) ); 	
  
  menu->addToolBar(__linedialog);
  __linedialog->hide();
}


void
ViewGLFrame::addProperties(QTabWidget * tab)
{
    if(__scene)__scene->addProperties(tab);
    if(__camera)__camera->addProperties(tab);

	QWidget * mtab = new QWidget( tab  );
	Ui::ViewGLOptionsForm glform;
	glform.setupUi(mtab);
	tab->addTab( mtab, tr( "GL Options" ) );

    makeCurrent();
	GLboolean glb = glIsEnabled(GL_CULL_FACE);

    GLint * res = new GLint;
    if(glb == GL_FALSE)glform.NoCullingButton->setChecked(true);
	else {
	  glGetIntegerv(GL_CULL_FACE_MODE ,res);
	  if(*res == GL_BACK)glform.BackFaceButton->setChecked(true);
	  else if(*res == GL_FRONT)glform.FrontFaceButton->setChecked(true);
	  else qWarning(("Error with glGet(GL_CULL_FACE_MODE) : "+QString::number(*res)+'-'+QString::number(GL_BACK)).toAscii().data());
	}

	QObject::connect(glform.NoCullingButton,SIGNAL(toggled(bool)),this,SLOT(glCullNoFace(bool)));
	QObject::connect(glform.BackFaceButton,SIGNAL(toggled(bool)),this,SLOT(glCullBackFace(bool)));
	QObject::connect(glform.FrontFaceButton,SIGNAL(toggled(bool)),this,SLOT(glCullFrontFace(bool)));


	glGetIntegerv(GL_SHADE_MODEL,res);
	if(*res == GL_FLAT)glform.FlatButton->setChecked(true);
	else if(*res == GL_SMOOTH)glform.SmoothButton->setChecked(true);

	QObject::connect(glform.FlatButton,SIGNAL(toggled(bool)),this,SLOT(glFlatShadeModel(bool)));
	QObject::connect(glform.SmoothButton,SIGNAL(toggled(bool)),this,SLOT(glSmoothShadeModel(bool)));

	glb = glIsEnabled(GL_DITHER);
	if(glb == GL_TRUE)glform.DitheringButton->setChecked(true);
	QObject::connect(glform.DitheringButton,SIGNAL(toggled(bool)),this,SLOT(glDithering(bool)));

	glb = glIsEnabled(GL_DEPTH_TEST);
	if(glb == GL_TRUE)glform.DepthTestButton->setChecked(true);
	QObject::connect(glform.DepthTestButton,SIGNAL(toggled(bool)),this,SLOT(glDepthTest(bool)));

	glb = glIsEnabled(GL_NORMALIZE);
	if(glb == GL_TRUE)glform.NormalizationButton->setChecked(true);
	QObject::connect(glform.NormalizationButton,SIGNAL(toggled(bool)),this,SLOT(glNormalization(bool)));

	delete res;
}

/* ----------------------------------------------------------------------- */
#define GL_COM(a)qWarning(#a);a;

void
ViewGLFrame::glCullNoFace(bool b){
  if(b){ makeCurrent(); GL_COM(glDisable(GL_CULL_FACE)); updateGL();}
}

void
ViewGLFrame::glCullBackFace(bool b){
  if(b){ makeCurrent(); glEnable(GL_CULL_FACE); GL_COM(glCullFace(GL_BACK));updateGL();}
}

void
ViewGLFrame::glCullFrontFace(bool b){
  if(b){ makeCurrent(); glEnable(GL_CULL_FACE); GL_COM(glCullFace(GL_FRONT));updateGL();}
}

void
ViewGLFrame::glSmoothShadeModel(bool b){
  if(b){ makeCurrent(); GL_COM(glShadeModel(GL_SMOOTH));updateGL();}
}

void
ViewGLFrame::glFlatShadeModel(bool b){
  if(b){ makeCurrent(); GL_COM(glShadeModel(GL_FLAT));updateGL();}
}

void
ViewGLFrame::glDithering(bool b){
  makeCurrent();
  if(b){GL_COM(glEnable(GL_DITHER));}
  else {GL_COM(glDisable(GL_DITHER));}
  updateGL();
}

void
ViewGLFrame::glDepthTest(bool b){
  makeCurrent();
  if(b){GL_COM(glEnable(GL_DEPTH_TEST));}
  else {GL_COM(glDisable(GL_DEPTH_TEST));}
  updateGL();
}

void
ViewGLFrame::glNormalization(bool b){
  makeCurrent();
  if(b){GL_COM(glEnable(GL_NORMALIZE));}
  else {GL_COM(glDisable(GL_NORMALIZE));}
  updateGL();
}

void
ViewGLFrame::animation(bool b){
  ViewSceneRendererGL * r = dynamic_cast<ViewSceneRendererGL *>(__scene);
  if(r)r->useDisplayList(!b);
  __camera->lockDim(b);
}


ViewDoubleToolButton::ViewDoubleToolButton( const QPixmap & pm,
										 const QPixmap & pm2,
										 const QString & textLabel, 
										 QObject * receiver, 
										 const char * slot, 
										 QToolBar * parent ):
QToolButton(parent),
__pm1(pm),__pm2(pm2){
  connect(this,SIGNAL(triggered(QAction *)),receiver,slot);
  PGL::drawArrow(&__pm1);
  QBitmap mask = __pm1.mask();
  PGL::drawArrow(&mask);
  __pm1.setMask(mask);
  PGL::drawArrow(&__pm2);
  mask = __pm2.mask();
  PGL::drawArrow(&mask);
  __pm1.setMask(mask);
  setIcon(QIcon(__pm1));
  setText(textLabel);
  setPopupMode(QToolButton::DelayedPopup);
  setCheckable(true);
}

void ViewDoubleToolButton::setButton(ViewGLFrame::Mode m)
{
  if(m == ViewGLFrame::Selection){
	setIcon(QIcon(__pm1));
  }
  else if(m == ViewGLFrame::MultipleSelection){
	setIcon(QIcon(__pm2));
  }
}


