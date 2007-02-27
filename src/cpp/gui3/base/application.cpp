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
#include <tool/dirnames.h>
#include "application.h"
#include "simpleappli.h"
#include "viewer.h"
#include "event.h"
#include "object.h"
#include "util_serializedobj.h"


TOOLS_USING_NAMESPACE
using namespace std;


#ifdef QT_THREAD_SUPPORT

#include <qthread.h>
#include "threadedappli.h"

#endif

static bool MODE = 
#ifndef GEOM_DEBUG
true
#else
false 
#endif
;


static ViewerAppli3 * VIEWER_APPLI = NULL;

class EndViewer3 {
public :
  EndViewer3(){ }
  ~EndViewer3(){ if(VIEWER_APPLI) { VIEWER_APPLI->exit(); delete VIEWER_APPLI; VIEWER_APPLI = NULL;} }
};

/// To destroy and stop all at end of process.
static EndViewer3 ENDV;
static bool DestroyedAppli = false;

void initViewerAppli(){
	if(VIEWER_APPLI == NULL){
#ifdef QT_THREAD_SUPPORT
		if(qApp == NULL && MODE){
			if(!DestroyedAppli)
				VIEWER_APPLI = new ViewerThreadedAppli3();
			else
				std::cerr << "Cannot create a second valid threaded Viewer Application!" << std::endl;
		}
		else
#endif
			VIEWER_APPLI = new ViewerSimpleAppli3();
	}
}



ViewerApplication3::ViewerApplication3(){}

ViewerApplication3::~ViewerApplication3(){}

bool
ViewerApplication3::isThreadUsed(){
#ifdef QT_THREAD_SUPPORT
    return MODE;
#else
    return false;
#endif
}

bool 
ViewerApplication3::useThread(bool use){
#ifdef QT_THREAD_SUPPORT
  if(!use && VIEWER_APPLI )return false;
  MODE = use;
  return true;
#else
  return (!use);
#endif
}

void 
ViewerApplication3::setBashMode(bool m){
	ViewObjectGL3::BASHMODE = m;
}


const vector<uint32_t>
ViewerApplication3::getSelection() {
  if(VIEWER_APPLI)return VIEWER_APPLI->getSelection();
  else return vector<uint32_t>();
}

int
ViewerApplication3::question(const std::string& caption,
					   const std::string& text,
					   const std::string& but0txt,
					   const std::string& but1txt,
					   const std::string& but2txt){
   initViewerAppli();
   if(VIEWER_APPLI)
	return VIEWER_APPLI->question(caption,text,but0txt,but1txt,but2txt);
   else return -1;
}

std::string ViewerApplication3::itemSelection(const std::string& caption,
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
ViewerApplication3::getOpenFile(const std::string& caption,
								 const std::string& filter,
								 const std::string& startPath){
  initViewerAppli();
  if(VIEWER_APPLI)
	return VIEWER_APPLI->getFile(caption,startPath,filter,true,false);
  else return "";
}

std::string 
ViewerApplication3::getSaveFile(const std::string& caption,
								 const std::string& filter,
								 const std::string& startPath){
  initViewerAppli();
  if(VIEWER_APPLI)
	return VIEWER_APPLI->getFile(caption,startPath,filter,false,false);
  else return "";
}

std::string ViewerApplication3::getDirectory(const std::string& caption,
						 const std::string& startPath){
  initViewerAppli();
  if(VIEWER_APPLI)
	return VIEWER_APPLI->getFile(caption,startPath,"",true,true);
  else return "";
}

ViewRayBuffer3 * 
ViewerApplication3::castRays(const TOOLS(Vector3)& pos, 
	                               const TOOLS(Vector3)& dir,
								   const TOOLS(Vector3)& dx, 
								   const TOOLS(Vector3)& dy,
								   int sx, int sy){
  
  initViewerAppli();
  if(VIEWER_APPLI)
	return VIEWER_APPLI->castRays(pos,dir,dx,dy,sx,sy);
  return NULL;
}

ViewZBuffer3 * 
ViewerApplication3::grabZBuffer(){
  
  initViewerAppli();
  if(VIEWER_APPLI)
	return VIEWER_APPLI->grabZBuffer();
  return NULL;
}



bool ViewerApplication3::wait( unsigned long time ){
  initViewerAppli();
  if(VIEWER_APPLI)
	return VIEWER_APPLI->Wait(time);
  return false;
}

bool ViewerApplication3::running(){
  if(VIEWER_APPLI) return VIEWER_APPLI->isRunning();
  else return false;
}


bool
ViewerApplication3::stop() {
    if(VIEWER_APPLI) return VIEWER_APPLI->stopSession();
    else return false;
}

void
ViewerApplication3::start() {
  if(!VIEWER_APPLI)initViewerAppli();
  else VIEWER_APPLI->startSession();
}

void
ViewerApplication3::exit() {
    if(VIEWER_APPLI) { 
		VIEWER_APPLI->exit();  
		delete VIEWER_APPLI; 
		VIEWER_APPLI = NULL; 
		DestroyedAppli = true; 
	}
}

void ViewerApplication3::_sendAnEvent(QCustomEvent *e){
  initViewerAppli();
  if(VIEWER_APPLI)VIEWER_APPLI->sendAnEvent(e);
}

void ViewerApplication3::_setViewerBuilder(ViewerBuilder3 * builder){
	ViewerAppli3::setBuilder(builder);
}

void
ViewerApplication3::setSelection(const vector<uint32_t>& sel) {
  _sendAnEvent(new ViewSelectionSet3(sel));
}

void 
ViewerApplication3::saveImage( const std::string& filename, 
							  const std::string& format,
							  bool  withAlpha ){
	_sendAnEvent(new ViewImageSaveEvent3(filename.c_str(),format.c_str(),withAlpha));
}

void 
ViewerApplication3::update(){
  _sendAnEvent(new ViewRefreshEvent3());
}

void
ViewerApplication3::fullScreen(bool b){
  _sendAnEvent(new ViewFullScreenEvent3(b));
}

void
ViewerApplication3::glFrameOnly(bool b){
  _sendAnEvent(new ViewGLFrameOnlyEvent3(b));
}

void ViewerApplication3::animation(bool anim){
  _sendAnEvent(new ViewAnimationEvent3(anim));
}

void ViewerApplication3::setBgColor(int red, int green, int blue){
  _sendAnEvent(new ViewBgColorEvent3(QColor(red,green,blue)));
}

void 
ViewerApplication3::setCamera(const Vector3& pos){
  _sendAnEvent(new ViewCameraEvent3(pos,Vector3::ORIGIN,0,0,0));
}
void 
ViewerApplication3::setCamera(const Vector3& pos, real_t azimuth, real_t elevation){
  _sendAnEvent(new ViewCameraEvent3(pos,Vector3::ORIGIN,azimuth,elevation,1));
}
void 
ViewerApplication3::lookAt(const Vector3& pos, const Vector3& target){
  _sendAnEvent(new ViewCameraEvent3(pos,target,0,0,2));
}
void 
ViewerApplication3::lookAt(const Vector3& target){
  _sendAnEvent(new ViewCameraEvent3(Vector3::ORIGIN,target,0,0,3));
}

void 
ViewerApplication3::setXYGrid(bool b){
  _sendAnEvent(new ViewGridEvent3(b,false,false,false,0,0,1));
}
void 
ViewerApplication3::setXZGrid(bool b){
  _sendAnEvent(new ViewGridEvent3(false,b,false,false,0,0,2));
}

void 
ViewerApplication3::setYZGrid(bool b){
  _sendAnEvent(new ViewGridEvent3(false,false,b,false,0,0,4));
}

void 
ViewerApplication3::setAxis(bool b){
  _sendAnEvent(new ViewGridEvent3(false,false,false,b,0,0,8));
}

void 
ViewerApplication3::setGridSize(int val){
  _sendAnEvent(new ViewGridEvent3(false,false,false,false,val,0,16));
}
void 
ViewerApplication3::setGridUnit(int val){
  _sendAnEvent(new ViewGridEvent3(false,false,false,false,0,val,32));
}
void 
ViewerApplication3::setGrid(bool a, bool b, bool c, bool d) {
  _sendAnEvent(new ViewGridEvent3(a,b,c,b,0,0,15));
}

void 
ViewerApplication3::glFrameSize(int w,int h){
  _sendAnEvent(new ViewPosEvent3(0,0,w,h,0));
}
void 
ViewerApplication3::resize(int w,int h){
  _sendAnEvent(new ViewPosEvent3(0,0,w,h,1));
}
void 
ViewerApplication3::move(int x,int y){
  _sendAnEvent(new ViewPosEvent3(x,y,0,0,2));
}

void 
ViewerApplication3::setGeometry(int x,int y,int w, int h){
  _sendAnEvent(new ViewPosEvent3(x,y,w,h,3));
}

double 
ViewerApplication3::getProjectionSize(int* nbpixel, double* pixelwidth){
  double res = 0;
  _sendAnEvent(new ViewProjSizeEvent3(&res,nbpixel,pixelwidth) );
  return res;
}


void ViewerApplication3::setPerspectiveCamera(){
  _sendAnEvent(new ViewCameraProjEvent3(true));
}

void ViewerApplication3::setOrthographicCamera(){
  _sendAnEvent(new ViewCameraProjEvent3(false));
}
