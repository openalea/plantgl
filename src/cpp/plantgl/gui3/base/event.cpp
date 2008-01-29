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

ViewSceneChangeEvent3::ViewSceneChangeEvent3(int type) :
  ViewEvent3(12345),
  scene_type(type)
{
}

ViewSceneChangeEvent3::~ViewSceneChangeEvent3()
{
  // Nothing to do.
}

ViewSceneChangeEvent3 *
ViewSceneChangeEvent3::copy()
{
  return new ViewSceneChangeEvent3(scene_type);
}

const int&
ViewSceneChangeEvent3::getSceneType() const
{
  return scene_type;
}

void 
ViewSceneChangeEvent3::setSceneType(const int& i)
{
  scene_type =i;
}

/*  ------------------------------------------------------------------------ */

ViewFileChangeEvent3::ViewFileChangeEvent3(const QString& file) :
  ViewEvent3(12348),
  filename(file)
{
}

ViewFileChangeEvent3::~ViewFileChangeEvent3()
{
  // Nothing to do.
}


/* ----------------------------------------------------------------------- */

ViewImageSaveEvent3::ViewImageSaveEvent3(const QString& file,const QString& form,bool _withAlpha) :
  ViewEvent3(12350),
  filename(file),
  format(form),
  withAlpha(_withAlpha)
{
}

ViewImageSaveEvent3::~ViewImageSaveEvent3()
{
  // Nothing to do.
}


/* ----------------------------------------------------------------------- */

ViewRefreshEvent3::ViewRefreshEvent3() :
  ViewEvent3(12349)
{
}

ViewRefreshEvent3::~ViewRefreshEvent3()
{
  // Nothing to do.
}


/* ----------------------------------------------------------------------- */



ViewSelectRecoverEvent3::ViewSelectRecoverEvent3() :
  ViewEvent3(12347),
  __exchange(0){
  __exchange =new std::vector<uint_t>();
}

ViewSelectRecoverEvent3::ViewSelectRecoverEvent3(std::vector<uint_t> * e) :
  ViewEvent3(12347),
  __exchange(e){
}

ViewSelectRecoverEvent3::~ViewSelectRecoverEvent3(){
  // Nothing to do.
}

void
ViewSelectRecoverEvent3::setSelection(const std::vector<uint_t>& a){
  *__exchange = a;
}



/* ----------------------------------------------------------------------- */

ViewSelectionSet3::ViewSelectionSet3(const std::vector<uint_t>& d) :
  ViewEvent3(12355),
  __data(d){
}

ViewSelectionSet3::~ViewSelectionSet3(){
  // Nothing to do.
}

const std::vector<uint_t>& 
ViewSelectionSet3::getSelection() const{
  return __data;
}

/* ----------------------------------------------------------------------- */

ViewEndEvent3::ViewEndEvent3() :
  ViewEvent3(12346){
}

ViewEndEvent3::~ViewEndEvent3(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewFullScreenEvent3::ViewFullScreenEvent3(bool b) :
  ViewEvent3(12352),
  value(b){
}

ViewFullScreenEvent3::~ViewFullScreenEvent3(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewGLFrameOnlyEvent3::ViewGLFrameOnlyEvent3(bool b) :
  ViewEvent3(12353),
  value(b){
}

ViewGLFrameOnlyEvent3::~ViewGLFrameOnlyEvent3(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewQuestionEvent3::ViewQuestionEvent3() :
  ViewEvent3(12351),
  result(new int(-1)){
}

ViewQuestionEvent3::ViewQuestionEvent3(const QString& _caption,
				 const QString& _text,
				 const QString& _but0txt,
				 const QString& _but1txt,
				 const QString& _but2txt,
				 int * _result) :
  ViewEvent3(12351),
  result((_result?_result:new int(-1))),
  caption(_caption),
  text(_text),
  but0txt(_but0txt),
  but1txt(_but1txt),
  but2txt(_but2txt){
}

ViewQuestionEvent3::~ViewQuestionEvent3(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewItemSelectionEvent3::ViewItemSelectionEvent3(const QString& _caption,
				 const QString& _text,
				 const QStringList& _values,
				 bool _editable,
				 QString * res,
				 bool * _ok) :
  ViewEvent3(12354),
	result((res?res:new QString())),
	ok((_ok?_ok:new bool(false))),
	caption(_caption),
	text(_text),
	values(_values),
	editable(_editable){
}

ViewItemSelectionEvent3::~ViewItemSelectionEvent3(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewAnimationEvent3::ViewAnimationEvent3(bool m) :
  ViewEvent3(12356),
  mode(m){
}

ViewAnimationEvent3::~ViewAnimationEvent3(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewBgColorEvent3::ViewBgColorEvent3(const QColor& c) :
  ViewEvent3(12357),
  color(c){
}

ViewBgColorEvent3::~ViewBgColorEvent3(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewGridEvent3::ViewGridEvent3(bool _xy, 
							 bool _yz, 
							 bool _xz, 
							 bool _axis, 
							 int _size, 
							 int _unit, 
							 int _def) :
  ViewEvent3(12358),
  xy(_xy),yz(_yz),xz(_xz),axis(_axis),size(_size),unit(_unit),def(_def){
}

ViewGridEvent3::~ViewGridEvent3(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewCameraEvent3::ViewCameraEvent3(const Vector3& _pos, 
								 const Vector3& _target,
								 float _azimuth, 
								 float _elevation, 
								 int _def):
  ViewEvent3(12359),
  pos(_pos),target(_target),azimuth(_azimuth),elevation(_elevation),def(_def){
}

ViewCameraEvent3::~ViewCameraEvent3(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewPosEvent3::ViewPosEvent3(int _x, int _y, int _w, int _h, int _def):
  ViewEvent3(12360),
  x(_x),y(_y),w(_w),h(_h),def(_def){
}

ViewPosEvent3::~ViewPosEvent3(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewFileSelEvent3::ViewFileSelEvent3(const QString& _caption,
				   const QString& _startPath,
				   const QString& _filter,
				   bool _existing, 
				   bool _dir,
				   QString * res) :
  ViewEvent3(12361),
	  result((res?res:new QString())),
	caption(_caption),
	startPath(_startPath),
	filter(_filter),
	dir(_dir),
	existing(_existing){
}

ViewFileSelEvent3::~ViewFileSelEvent3(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewRayBuffEvent3::ViewRayBuffEvent3(const TOOLS(Vector3)& _pos,
				   const TOOLS(Vector3)& _dir,
		           const TOOLS(Vector3)& _dx, 
		           const TOOLS(Vector3)& _dy,
		           int _sx, int _sy,
				   ViewRayBuffer3 ** res) :
  ViewEvent3(12362),
	result((res?res:NULL)),
	pos(_pos),
	dir(_dir),
	dx(_dx),
	dy(_dy),
	sx(_sx),
	sy(_sy){
	if(!result)result = new  ViewRayBuffer3 *;
  }

ViewRayBuffEvent3::~ViewRayBuffEvent3(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewZBuffEvent3::ViewZBuffEvent3(ViewZBuffer3 ** res) :
  ViewEvent3(12366),
	result((res?res:NULL)){
	if(!result)result = new  ViewZBuffer3 *;
  }

ViewZBuffEvent3::~ViewZBuffEvent3(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

ViewProjSizeEvent3::ViewProjSizeEvent3(double * _size, int * _nbpix, double * _pixwidth) :
  ViewEvent3(12363),
    nbpixel(_nbpix),
	pixelwidth(_pixwidth),
	size((_size?_size:new double(0.0)))
{
}

ViewProjSizeEvent3::~ViewProjSizeEvent3(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */


ViewCameraProjEvent3::ViewCameraProjEvent3(bool _mode) :
  ViewEvent3(12364),
    mode(_mode)
{
}

ViewCameraProjEvent3::~ViewCameraProjEvent3(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */

/// 12365 used in view_geomevent
/// 12366 used in ViewZBuffEvent3
