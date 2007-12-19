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

#include "event.h"

ViewEvent::ViewEvent(int type):
	QEvent(QEvent::Type(type)),
	sent_event(false)
{
#ifdef PGL_DEBUG
    printf("Create event %i of type %i\n",size_t(this),type);
#endif
}

ViewEvent::~ViewEvent(){
#ifdef PGL_DEBUG
    printf("Destroy event %i of type %i\n",size_t(this),type());
#endif
}

/*  ------------------------------------------------------------------------ */

ViewSceneChangeEvent::ViewSceneChangeEvent(int type) :
  ViewEvent(eSceneChange),
  scene_type(type)
{
}

ViewSceneChangeEvent::~ViewSceneChangeEvent()
{
  // Nothing to do.
}

ViewSceneChangeEvent *
ViewSceneChangeEvent::copy()
{
  return new ViewSceneChangeEvent(scene_type);
}

const int&
ViewSceneChangeEvent::getSceneType() const
{
  return scene_type;
}

void 
ViewSceneChangeEvent::setSceneType(const int& i)
{
  scene_type =i;
}

/*  ------------------------------------------------------------------------ */

ViewFileChangeEvent::ViewFileChangeEvent(const QString& file) :
  ViewEvent(eFileChange),
  filename(file)
{
}

ViewFileChangeEvent::~ViewFileChangeEvent()
{
  // Nothing to do.
}


/* ----------------------------------------------------------------------- */

ViewImageSaveEvent::ViewImageSaveEvent(const QString& file,const QString& form,bool _withAlpha) :
  ViewEvent(eImageSave),
  filename(file),
  format(form),
  withAlpha(_withAlpha)
{
}

ViewImageSaveEvent::~ViewImageSaveEvent()
{
  // Nothing to do.
}


/* ----------------------------------------------------------------------- */

ViewRefreshEvent::ViewRefreshEvent() :
  ViewEvent(eRefresh)
{
}

ViewRefreshEvent::~ViewRefreshEvent()
{
  // Nothing to do.
}


/* ----------------------------------------------------------------------- */



ViewSelectRecoverEvent::ViewSelectRecoverEvent() :
  ViewEvent(eGetSelection),
  __exchange(0){
  __exchange =new std::vector<uint32_t>();
}

ViewSelectRecoverEvent::ViewSelectRecoverEvent(std::vector<uint32_t> * e) :
  ViewEvent(eGetSelection),
  __exchange(e){
}

ViewSelectRecoverEvent::~ViewSelectRecoverEvent(){
  // Nothing to do.
}

void
ViewSelectRecoverEvent::setSelection(const std::vector<uint32_t>& a){
  *__exchange = a;
}



/* ----------------------------------------------------------------------- */

ViewSelectionSet::ViewSelectionSet(const std::vector<uint32_t>& d) :
  ViewEvent(eSetSelection),
  __data(d){
}

ViewSelectionSet::~ViewSelectionSet(){
  // Nothing to do.
}

const std::vector<uint32_t>& 
ViewSelectionSet::getSelection() const{
  return __data;
}

/* ----------------------------------------------------------------------- */

ViewEndEvent::ViewEndEvent() :
  ViewEvent(eEnd){
}

ViewEndEvent::~ViewEndEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewShowEvent::ViewShowEvent() :
  ViewEvent(eShow){
}

ViewShowEvent::~ViewShowEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewFullScreenEvent::ViewFullScreenEvent(bool b) :
  ViewEvent(eFullScreen),
  value(b){
}

ViewFullScreenEvent::~ViewFullScreenEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewGLFrameOnlyEvent::ViewGLFrameOnlyEvent(bool b) :
  ViewEvent(eGLFrameOnly),
  value(b){
}

ViewGLFrameOnlyEvent::~ViewGLFrameOnlyEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewQuestionEvent::ViewQuestionEvent() :
  ViewEvent(eQuestion),
  result(NULL){
}

ViewQuestionEvent::ViewQuestionEvent(const QString& _caption,
				 const QString& _text,
				 const QString& _but0txt,
				 const QString& _but1txt,
				 const QString& _but2txt,
				 int * _result) :
  ViewEvent(eQuestion),
  result(_result),
  caption(_caption),
  text(_text),
  but0txt(_but0txt),
  but1txt(_but1txt),
  but2txt(_but2txt){
}

ViewQuestionEvent::~ViewQuestionEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewItemSelectionEvent::ViewItemSelectionEvent(const QString& _caption,
				 const QString& _text,
				 const QStringList& _values,
				 bool _editable,
				 QString * res,
				 bool * _ok) :
  ViewEvent(eItemSelection),
	result((res?res:new QString())),
	ok((_ok?_ok:new bool(false))),
	caption(_caption),
	text(_text),
	values(_values),
	editable(_editable){
}

ViewItemSelectionEvent::~ViewItemSelectionEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewDoubleSelectionEvent::ViewDoubleSelectionEvent(const QString& _caption,
				 const QString& _text,
				 double _value,
                 double _minvalue,
                 double _maxvalue,
				 double * _res,
				 bool * _ok) :
  ViewEvent(eDoubleSelection),
	result((_res?_res:new double())),
	ok((_ok?_ok:new bool(false))),
	caption(_caption),
	text(_text),
	value(_value),
	minvalue(_minvalue),
    maxvalue(_maxvalue){
}

ViewDoubleSelectionEvent::~ViewDoubleSelectionEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewAnimationEvent::ViewAnimationEvent(bool m) :
  ViewEvent(eAnimation),
  mode(m){
}

ViewAnimationEvent::~ViewAnimationEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewBgColorEvent::ViewBgColorEvent(const QColor& c) :
  ViewEvent(eBgColor),
  color(c){
}

ViewBgColorEvent::~ViewBgColorEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewGridEvent::ViewGridEvent(bool _xy, 
							 bool _yz, 
							 bool _xz, 
							 bool _axis, 
							 int _size, 
							 int _unit, 
							 int _def) :
  ViewEvent(eGrid),
  xy(_xy),yz(_yz),xz(_xz),axis(_axis),size(_size),unit(_unit),def(_def){
}

ViewGridEvent::~ViewGridEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewCameraSetEvent::ViewCameraSetEvent(const Vector3& _pos, 
								 const Vector3& _target,
								 float _azimuth, 
								 float _elevation, 
								 int _def):
  ViewEvent(eCameraSet),
  position(_pos),target(_target),azimuth(_azimuth),elevation(_elevation),def(_def){
}

ViewCameraSetEvent::~ViewCameraSetEvent(){
  // Nothing to do.
}
/* ----------------------------------------------------------------------- */

ViewCameraGetEvent::ViewCameraGetEvent(Vector3* _pos, Vector3* _heading,Vector3* _up):
  ViewEvent(eCameraGet),
  position(_pos),heading(_heading),up(_up){
}

ViewCameraGetEvent::~ViewCameraGetEvent(){
  // Nothing to do.
}


/* ----------------------------------------------------------------------- */

/// Constructor.
ViewCPActivateEvent::ViewCPActivateEvent(int _cpid, bool _activation) : 
   ViewEvent(eClippingPlaneActivate),cpid(_cpid),activation(_activation) {}

  /// Destructor.
   ViewCPActivateEvent::~ViewCPActivateEvent(){}

/* ----------------------------------------------------------------------- */

  /// Constructor.
ViewCPSetEvent::ViewCPSetEvent(int _cpid, double _a, double _b, double _c, double _d):
	   ViewEvent(eClippingPlaneSet), cpid(_cpid),a(_a),b(_b),c(_c),d(_d) {}

  /// Destructor.
	   ViewCPSetEvent::~ViewCPSetEvent(){}


/* ----------------------------------------------------------------------- */

ViewPosEvent::ViewPosEvent(int _x, int _y, int _w, int _h, int _def):
  ViewEvent(ePos),
  x(_x),y(_y),w(_w),h(_h),def(_def){
}

ViewPosEvent::~ViewPosEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewFileSelEvent::ViewFileSelEvent(const QString& _caption,
				   const QString& _startPath,
				   const QString& _filter,
				   bool _existing, 
				   bool _dir,
				   QString * res) :
  ViewEvent(eFileSelection),
	  result((res?res:new QString())),
	caption(_caption),
	startPath(_startPath),
	filter(_filter),
	dir(_dir),
	existing(_existing){
}

ViewFileSelEvent::~ViewFileSelEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewRayBuffEvent::ViewRayBuffEvent(const TOOLS(Vector3)& _pos,
				   const TOOLS(Vector3)& _dir,
		           const TOOLS(Vector3)& _dx, 
		           const TOOLS(Vector3)& _dy,
		           int _sx, int _sy,
				   ViewRayBuffer ** res) :
  ViewEvent(eRayBuff),
	result((res?res:NULL)),
	pos(_pos),
	dir(_dir),
	dx(_dx),
	dy(_dy),
	sx(_sx),
	sy(_sy){
	if(!result)result = new  ViewRayBuffer *;
  }

ViewRayBuffEvent::~ViewRayBuffEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewZBuffEvent::ViewZBuffEvent(ViewZBuffer ** res) :
  ViewEvent(eZBuff),
	result((res?res:NULL)){
	if(!result)result = new  ViewZBuffer *;
  }

ViewZBuffEvent::~ViewZBuffEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewProjSizeEvent::ViewProjSizeEvent(double * _size, int * _nbpix, double * _pixwidth) :
  ViewEvent(eProjSize),
    nbpixel(_nbpix),
	pixelwidth(_pixwidth),
	size((_size?_size:new double(0.0)))
{
}

ViewProjSizeEvent::~ViewProjSizeEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */


ViewCameraProjEvent::ViewCameraProjEvent(bool _mode) :
  ViewEvent(eCameraProj),
    mode(_mode)
{
}

ViewCameraProjEvent::~ViewCameraProjEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */
