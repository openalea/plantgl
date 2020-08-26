//#define GEOM_DEBUG
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
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/QApplication>
#else
    #include <QtGui/QApplication>
#endif
#include <QtGui/QColor>
#include <plantgl/tool/dirnames.h>
#include "application.h"
#include "simpleappli.h"
#include "viewer.h"
#include "event.h"
#include "object.h"
#include "util_serializedobj.h"


PGL_USING_NAMESPACE
using namespace std;


#ifdef QT_THREAD_SUPPORT

#include <QtCore/qthread.h>
#include "threadedappli.h"

#endif

static bool MODE = false;


static ViewerAppliInternal * VIEWER_APPLI = NULL;

class EndViewer {
public :
  EndViewer(){ }
  ~EndViewer(){ if(VIEWER_APPLI) { VIEWER_APPLI->exit(); delete VIEWER_APPLI; VIEWER_APPLI = NULL;} }
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
ViewerApplication::setBatchMode(bool m){
    ViewObjectGL::BATCHMODE = m;
}


vector<uint_t>
ViewerApplication::getSelection() {
  if(VIEWER_APPLI)return VIEWER_APPLI->getSelection();
  else return vector<uint_t>();
}


int
ViewerApplication::question(const std::string& caption,
                       const std::string& text,
                       const std::string& but0txt,
                       const std::string& but1txt,
                       const std::string& but2txt){
   initViewerAppli();
   if(VIEWER_APPLI){
    return VIEWER_APPLI->question(caption,text,but0txt,but1txt,but2txt);
   }
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

double ViewerApplication::doubleSelection(const std::string& caption,
                                   const std::string& text,
                                   double value,
                                   double minvalue,
                                   double maxvalue,
                                   bool& ok){
  initViewerAppli();
  if(VIEWER_APPLI)
     return VIEWER_APPLI->doubleSelection(caption,text,value,minvalue,maxvalue,ok);
  else return 0;
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
ViewerApplication::castRays(const Vector3& pos,
                                   const Vector3& dir,
                                   const Vector3& dx,
                                   const Vector3& dy,
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

std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)>
ViewerApplication::grabZBufferPoints(){

  initViewerAppli();
  if(VIEWER_APPLI)
    return VIEWER_APPLI->grabZBufferPoints();
  return std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)>();
}

std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)>
ViewerApplication::grabZBufferPointsWithJitter(float jitter, int raywidth){

  initViewerAppli();
  if(VIEWER_APPLI)
    return VIEWER_APPLI->grabZBufferPointsWithJitter(jitter,raywidth);
  return std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)>();
}

bool ViewerApplication::wait( unsigned long time ){
  initViewerAppli();
  if(VIEWER_APPLI)
    return VIEWER_APPLI->Wait(time);
  return false;
}

bool ViewerApplication::running(){
  if(VIEWER_APPLI) return VIEWER_APPLI->running();
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
    if(VIEWER_APPLI) {
        VIEWER_APPLI->exit();
        delete VIEWER_APPLI;
        VIEWER_APPLI = NULL;
        DestroyedAppli = true;
    }
}

void ViewerApplication::_sendAnEvent(QEvent *e){
  initViewerAppli();
  if(VIEWER_APPLI)VIEWER_APPLI->sendAnEvent(e);
}

void ViewerApplication::_setViewerBuilder(ViewerBuilder * builder){
    ViewerAppliInternal::setBuilder(builder);
}

void ViewerApplication::registerThreadStateSaverFatory(ThreadStateSaverFactory * tssf)
{
    ViewerAppliInternal::registerThreadStateSaverFatory(tssf);
}

void ViewerApplication::cleanThreadStateSaverFatory()
{
    ViewerAppliInternal::cleanThreadStateSaverFatory();
}

void ViewerApplication::showMessage(const std::string& caption, int timeout)
{  _sendAnEvent(new ViewShowMessageEvent(QString(caption.c_str()),timeout)); }

void
ViewerApplication::setSelection(const vector<uint_t>& sel) {
  _sendAnEvent(new ViewSelectionSet(sel));
}

uint_t
ViewerApplication::waitSelection(const std::string& caption)
{
   uint_t res;
   _sendAnEvent(new ViewWaitSelection(&res,QString(caption.c_str())));
   return res;
}

void ViewerApplication::setAborter(AbortFunc func)
{
   _sendAnEvent(new ViewSetAborterEvent(func));
}

bool ViewerApplication::getRedrawPolicy(){
  if(VIEWER_APPLI) return VIEWER_APPLI->getRedrawPolicy();
  else return true;
}

void ViewerApplication::setRedrawPolicy(bool b){
  initViewerAppli();
  if(VIEWER_APPLI) VIEWER_APPLI->setRedrawPolicy(b);
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

void ViewerApplication::setAnimation(eAnimationFlag anim){
  _sendAnEvent(new ViewAnimationEvent(anim));
}

void ViewerApplication::setBgColor(int red, int green, int blue){
  _sendAnEvent(new ViewBgColorEvent(QColor(red,green,blue)));
}

void
ViewerApplication::setCamera(const Vector3& pos){
  _sendAnEvent(new ViewCameraSetEvent(pos,Vector3::ORIGIN,0,0,0));
}
void
ViewerApplication::setCamera(const Vector3& pos, real_t azimuth, real_t elevation){
  _sendAnEvent(new ViewCameraSetEvent(pos,Vector3::ORIGIN,azimuth,elevation,1));
}

void
ViewerApplication::lookAt(const Vector3& pos, const Vector3& target){
  _sendAnEvent(new ViewCameraSetEvent(pos,target,0,0,2));
}
void
ViewerApplication::lookAt(const Vector3& target){
  _sendAnEvent(new ViewCameraSetEvent(Vector3::ORIGIN,target,0,0,3));
}

void
ViewerApplication::getCamera(Vector3& pos, Vector3& heading, Vector3& up){
  _sendAnEvent(new ViewCameraGetEvent(&pos,&heading,&up));
}


void ViewerApplication::setViewAngle(real_t angle){
  _sendAnEvent(new ViewSetViewAngleEvent(angle));
}

real_t ViewerApplication::getViewAngle(){
    real_t angle;
    _sendAnEvent(new ViewGetViewAngleEvent(&angle));
    return angle;

}

void ViewerApplication::setLightEnabled(bool b)
{ _sendAnEvent(new ViewLightSetEvent(Vector3(),QColor(),QColor(),QColor(),b,ViewLightSetEvent::eActivation)); }

bool ViewerApplication::isLightEnabled()
{  bool b; _sendAnEvent(new ViewLightGetEvent(NULL,NULL,NULL,NULL,&b,ViewLightSetEvent::eActivation)); return b;}

void ViewerApplication::setLightPosition(const Vector3& position)
{ _sendAnEvent(new ViewLightSetEvent(position,QColor(),QColor(),QColor(),false,ViewLightSetEvent::ePosition)); }

Vector3 ViewerApplication::getLightPosition()
{ Vector3 position; _sendAnEvent(new ViewLightGetEvent(&position,NULL,NULL,NULL,NULL,ViewLightSetEvent::ePosition)); return position; }

void ViewerApplication::setLightAmbient(int red, int green, int blue)
{ _sendAnEvent(new ViewLightSetEvent(Vector3(),QColor(red,green,blue),QColor(),QColor(),false,ViewLightSetEvent::eAmbient)); }
void ViewerApplication::getLightAmbient(int& red, int& green, int& blue)
{
    QColor color;
    _sendAnEvent(new ViewLightGetEvent(NULL,&color,NULL,NULL,NULL,ViewLightSetEvent::eAmbient));
    red = color.red();
    green = color.green();
    blue = color.blue();
}

void ViewerApplication::setLightDiffuse(int red, int green, int blue)
{ _sendAnEvent(new ViewLightSetEvent(Vector3(),QColor(),QColor(red,green,blue),QColor(),false,ViewLightSetEvent::eDiffuse)); }
void ViewerApplication::getLightDiffuse(int& red, int& green, int& blue)
{
    QColor color;
    _sendAnEvent(new ViewLightGetEvent(NULL,NULL,&color,NULL,NULL,ViewLightSetEvent::eDiffuse));
    red = color.red();
    green = color.green();
    blue = color.blue();
}

void ViewerApplication::setLightSpecular(int red, int green, int blue)
{ _sendAnEvent(new ViewLightSetEvent(Vector3(),QColor(),QColor(),QColor(red,green,blue),false,ViewLightSetEvent::eSpecular)); }
void ViewerApplication::getLightSpecular(int& red, int& green, int& blue)
{
    QColor color;
    _sendAnEvent(new ViewLightGetEvent(NULL,NULL,NULL,&color,NULL,ViewLightSetEvent::eSpecular));
    red = color.red();
    green = color.green();
    blue = color.blue();
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
ViewerApplication::activateCP(int id, bool activation)
{ _sendAnEvent(new ViewCPActivateEvent(id,activation)); }

void
ViewerApplication::setCP(int id, double a, double b, double c, double d)
{ _sendAnEvent(new ViewCPSetEvent(id,a,b,c,d)); }


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


void ViewerApplication::setPerspectiveCamera(){
  _sendAnEvent(new ViewCameraProjEvent(true));
}

void ViewerApplication::setOrthographicCamera(){
  _sendAnEvent(new ViewCameraProjEvent(false));
}

unsigned int ViewerApplication::viewerId()
{
    if(VIEWER_APPLI && VIEWER_APPLI->getViewer()) return (unsigned int)VIEWER_APPLI->getViewer()->winId();
    else return 0;
}
