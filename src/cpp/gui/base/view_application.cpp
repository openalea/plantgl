//#define GEOM_DEBUG
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


#include <qapplication.h>
#include <qcolor.h>
#include "Tools/dirnames.h"
#include "view_application.h"
#include "view_simpleappli.h"
#include "view_viewer.h"
#include "view_geomevent.h"
#include "view_object.h"
#include "util_geomgl.h"
#include "util_serializedobj.h"
#include "appe_material.h"
#include "scne_shape.h"


GEOM_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace std;


#ifdef QT_THREAD_SUPPORT

#include <qthread.h>
#include "view_threadedappli.h"

#endif

static bool MODE = 
#ifndef GEOM_DEBUG
true
#else
false 
#endif
;


static ViewerAppli * VIEWER_APPLI = NULL;

class EndViewer {
public :
  EndViewer(){ }
  ~EndViewer(){ if(VIEWER_APPLI) delete VIEWER_APPLI;  }
};

/// To destroy and stop all at end of process.
static EndViewer ENDV;
static bool DestroyedAppli = false;

void initViewerAppli(){
	if(VIEWER_APPLI == NULL){
#ifdef QT_THREAD_SUPPORT
		if(qApp == NULL && MODE){
			if(!DestroyedAppli)
				VIEWER_APPLI = new ViewerThreadedAppli();
			else
				std::cerr << "Cannot create a second valid threaded Viewer Application!" << std::endl;
		}
		else
#endif
			VIEWER_APPLI = new ViewerSimpleAppli();
	}
}



ViewerApplication::ViewerApplication(){}

ViewerApplication::~ViewerApplication(){}

bool
ViewerApplication::isThreadUsed(){
#ifdef QT_THREAD_SUPPORT
    return MODE;
#else
    return false;
#endif
}

bool 
ViewerApplication::useThread(bool use){
#ifdef QT_THREAD_SUPPORT
  if(!use && VIEWER_APPLI )return false;
  MODE = use;
  return true;
#else
  return (!use);
#endif
}

void 
ViewerApplication::setBashMode(bool m){
	ViewObjectGL::BASHMODE = m;
}

void
ViewerApplication::display(const GeometryPtr& g){
  ScenePtr scene(new Scene());
  scene->add(GeomShape(g,Material::DEFAULT_MATERIAL));
  ViewerApplication::display(scene);
}

void
ViewerApplication::add(const GeometryPtr& g){
  ScenePtr scene(new Scene());
  scene->add(GeomShape(g,Material::DEFAULT_MATERIAL));
  ViewerApplication::add(scene);
}

void
ViewerApplication::display(const ScenePtr& s){
	initViewerAppli();
	if(VIEWER_APPLI)VIEWER_APPLI->display(s);
}

void
ViewerApplication::add(const ScenePtr& s){
	initViewerAppli();
	if(VIEWER_APPLI)VIEWER_APPLI->add(s);
}

const vector<uint32_t>
ViewerApplication::getSelection() {
  if(VIEWER_APPLI)return VIEWER_APPLI->getSelection();
  else return vector<uint32_t>();
}

int
ViewerApplication::question(const std::string& caption,
					   const std::string& text,
					   const std::string& but0txt,
					   const std::string& but1txt,
					   const std::string& but2txt){
   initViewerAppli();
   if(VIEWER_APPLI)
	return VIEWER_APPLI->question(caption,text,but0txt,but1txt,but2txt);
   else return -1;
}

std::string ViewerApplication::itemSelection(const std::string& caption,
					   const std::string& text,
					   const std::vector<std::string> & values,
					   bool& ok,
					   bool editable){
  initViewerAppli();
  if(VIEWER_APPLI)
     return VIEWER_APPLI->itemSelection(caption,text,values,ok,editable);
  else return "";
}

std::string 
ViewerApplication::getOpenFile(const std::string& caption,
								 const std::string& filter,
								 const std::string& startPath){
  initViewerAppli();
  if(VIEWER_APPLI)
	return VIEWER_APPLI->getFile(caption,startPath,filter,true,false);
  else return "";
}

std::string 
ViewerApplication::getSaveFile(const std::string& caption,
								 const std::string& filter,
								 const std::string& startPath){
  initViewerAppli();
  if(VIEWER_APPLI)
	return VIEWER_APPLI->getFile(caption,startPath,filter,false,false);
  else return "";
}

std::string ViewerApplication::getDirectory(const std::string& caption,
						 const std::string& startPath){
  initViewerAppli();
  if(VIEWER_APPLI)
	return VIEWER_APPLI->getFile(caption,startPath,"",true,true);
  else return "";
}

ViewRayBuffer * 
ViewerApplication::castRays(const TOOLS(Vector3)& pos, 
	                               const TOOLS(Vector3)& dir,
								   const TOOLS(Vector3)& dx, 
								   const TOOLS(Vector3)& dy,
								   int sx, int sy){
  
  initViewerAppli();
  if(VIEWER_APPLI)
	return VIEWER_APPLI->castRays(pos,dir,dx,dy,sx,sy);
  return NULL;
}

ViewZBuffer * 
ViewerApplication::grabZBuffer(){
  
  initViewerAppli();
  if(VIEWER_APPLI)
	return VIEWER_APPLI->grabZBuffer();
  return NULL;
}



bool ViewerApplication::wait( unsigned long time ){
  initViewerAppli();
  if(VIEWER_APPLI)
	return VIEWER_APPLI->Wait(time);
  return false;
}

bool ViewerApplication::running(){
  if(VIEWER_APPLI) return VIEWER_APPLI->isRunning();
  else return false;
}


bool
ViewerApplication::stop() {
    if(VIEWER_APPLI) return VIEWER_APPLI->stopSession();
    else return false;
}

void
ViewerApplication::start() {
  if(!VIEWER_APPLI)initViewerAppli();
  else VIEWER_APPLI->startSession();
}

void
ViewerApplication::exit() {
    if(VIEWER_APPLI) { VIEWER_APPLI->exit();  delete VIEWER_APPLI; VIEWER_APPLI = NULL; DestroyedAppli = true; }
}

void _sendAnEvent(QCustomEvent *e){
  initViewerAppli();
  if(VIEWER_APPLI)VIEWER_APPLI->sendAnEvent(e);
}

void
ViewerApplication::setSelection(const vector<uint32_t>& sel) {
  _sendAnEvent(new ViewSelectionSet(sel));
}

void 
ViewerApplication::saveImage( const std::string& filename, 
							  const std::string& format,
							  bool  withAlpha ){
	_sendAnEvent(new ViewImageSaveEvent(filename.c_str(),format.c_str(),withAlpha));
}

void 
ViewerApplication::update(){
  _sendAnEvent(new ViewRefreshEvent());
}

void
ViewerApplication::fullScreen(bool b){
  _sendAnEvent(new ViewFullScreenEvent(b));
}

void
ViewerApplication::glFrameOnly(bool b){
  _sendAnEvent(new ViewGLFrameOnlyEvent(b));
}

void ViewerApplication::animation(bool anim){
  _sendAnEvent(new ViewAnimationEvent(anim));
}

void ViewerApplication::setBgColor(int red, int green, int blue){
  _sendAnEvent(new ViewBgColorEvent(QColor(red,green,blue)));
}

void 
ViewerApplication::setCamera(const Vector3& pos){
  _sendAnEvent(new ViewCameraEvent(pos,Vector3::ORIGIN,0,0,0));
}
void 
ViewerApplication::setCamera(const Vector3& pos, real_t azimuth, real_t elevation){
  _sendAnEvent(new ViewCameraEvent(pos,Vector3::ORIGIN,azimuth,elevation,1));
}
void 
ViewerApplication::lookAt(const Vector3& pos, const Vector3& target){
  _sendAnEvent(new ViewCameraEvent(pos,target,0,0,2));
}
void 
ViewerApplication::lookAt(const Vector3& target){
  _sendAnEvent(new ViewCameraEvent(Vector3::ORIGIN,target,0,0,3));
}

void 
ViewerApplication::setXYGrid(bool b){
  _sendAnEvent(new ViewGridEvent(b,false,false,false,0,0,1));
}
void 
ViewerApplication::setXZGrid(bool b){
  _sendAnEvent(new ViewGridEvent(false,b,false,false,0,0,2));
}

void 
ViewerApplication::setYZGrid(bool b){
  _sendAnEvent(new ViewGridEvent(false,false,b,false,0,0,4));
}

void 
ViewerApplication::setAxis(bool b){
  _sendAnEvent(new ViewGridEvent(false,false,false,b,0,0,8));
}

void 
ViewerApplication::setGridSize(int val){
  _sendAnEvent(new ViewGridEvent(false,false,false,false,val,0,16));
}
void 
ViewerApplication::setGridUnit(int val){
  _sendAnEvent(new ViewGridEvent(false,false,false,false,0,val,32));
}
void 
ViewerApplication::setGrid(bool a, bool b, bool c, bool d) {
  _sendAnEvent(new ViewGridEvent(a,b,c,b,0,0,15));
}

void 
ViewerApplication::glFrameSize(int w,int h){
  _sendAnEvent(new ViewPosEvent(0,0,w,h,0));
}
void 
ViewerApplication::resize(int w,int h){
  _sendAnEvent(new ViewPosEvent(0,0,w,h,1));
}
void 
ViewerApplication::move(int x,int y){
  _sendAnEvent(new ViewPosEvent(x,y,0,0,2));
}

void 
ViewerApplication::setGeometry(int x,int y,int w, int h){
  _sendAnEvent(new ViewPosEvent(x,y,w,h,3));
}

double 
ViewerApplication::getProjectionSize(int* nbpixel, double* pixelwidth){
  double res = 0;
  _sendAnEvent(new ViewProjSizeEvent(&res,nbpixel,pixelwidth) );
  return res;
}

std::vector<std::pair<uint32_t,double> > 
ViewerApplication::getProjectionSizes(const GEOM(ScenePtr)& sc){
  std::vector<std::pair<uint32_t,double> > res;
  _sendAnEvent(new GeomProjListEvent(sc,&res));
  return res;
}

void ViewerApplication::setPerspectiveCamera(){
  _sendAnEvent(new ViewCameraProjEvent(true));
}

void ViewerApplication::setOrthographicCamera(){
  _sendAnEvent(new ViewCameraProjEvent(false));
}
