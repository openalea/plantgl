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

/*  ------------------------------------------------------------------------ */

ViewSceneChangeEvent::ViewSceneChangeEvent(int type) :
  ViewEvent(12345),
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
  ViewEvent(12348),
  filename(file)
{
}

ViewFileChangeEvent::~ViewFileChangeEvent()
{
  // Nothing to do.
}


/* ----------------------------------------------------------------------- */

ViewImageSaveEvent::ViewImageSaveEvent(const QString& file,const QString& form,bool _withAlpha) :
  ViewEvent(12350),
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
  ViewEvent(12349)
{
}

ViewRefreshEvent::~ViewRefreshEvent()
{
  // Nothing to do.
}


/* ----------------------------------------------------------------------- */



ViewSelectRecoverEvent::ViewSelectRecoverEvent() :
  ViewEvent(12347),
  __exchange(0){
  __exchange =new std::vector<uint32_t>();
}

ViewSelectRecoverEvent::ViewSelectRecoverEvent(std::vector<uint32_t> * e) :
  ViewEvent(12347),
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
  ViewEvent(12355),
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
  ViewEvent(12346){
}

ViewEndEvent::~ViewEndEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewFullScreenEvent::ViewFullScreenEvent(bool b) :
  ViewEvent(12352),
  value(b){
}

ViewFullScreenEvent::~ViewFullScreenEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewGLFrameOnlyEvent::ViewGLFrameOnlyEvent(bool b) :
  ViewEvent(12353),
  value(b){
}

ViewGLFrameOnlyEvent::~ViewGLFrameOnlyEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewQuestionEvent::ViewQuestionEvent() :
  ViewEvent(12351),
  result(new int(-1)){
}

ViewQuestionEvent::ViewQuestionEvent(const QString& _caption,
				 const QString& _text,
				 const QString& _but0txt,
				 const QString& _but1txt,
				 const QString& _but2txt,
				 int * _result) :
  ViewEvent(12351),
  result((_result?_result:new int(-1))),
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
  ViewEvent(12354),
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

ViewAnimationEvent::ViewAnimationEvent(bool m) :
  ViewEvent(12356),
  mode(m){
}

ViewAnimationEvent::~ViewAnimationEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewBgColorEvent::ViewBgColorEvent(const QColor& c) :
  ViewEvent(12357),
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
  ViewEvent(12358),
  xy(_xy),yz(_yz),xz(_xz),axis(_axis),size(_size),unit(_unit),def(_def){
}

ViewGridEvent::~ViewGridEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewCameraEvent::ViewCameraEvent(const Vector3& _pos, 
								 const Vector3& _target,
								 float _azimuth, 
								 float _elevation, 
								 int _def):
  ViewEvent(12359),
  pos(_pos),target(_target),azimuth(_azimuth),elevation(_elevation),def(_def){
}

ViewCameraEvent::~ViewCameraEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewPosEvent::ViewPosEvent(int _x, int _y, int _w, int _h, int _def):
  ViewEvent(12360),
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
  ViewEvent(12361),
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
  ViewEvent(12362),
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
  ViewEvent(12366),
	result((res?res:NULL)){
	if(!result)result = new  ViewZBuffer *;
  }

ViewZBuffEvent::~ViewZBuffEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewProjSizeEvent::ViewProjSizeEvent(double * _size, int * _nbpix, double * _pixwidth) :
  ViewEvent(12363),
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
  ViewEvent(12364),
    mode(_mode)
{
}

ViewCameraProjEvent::~ViewCameraProjEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

/// 12365 used in view_geomevent
/// 12366 used in ViewZBuffEvent
