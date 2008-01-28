/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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


 

#include "mesh.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include "polyline.h"
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/colorarray.h>
#include <plantgl/tool/util_string.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


const bool Mesh::DEFAULT_CCW(true);
const bool Mesh::DEFAULT_SOLID(false);
const bool Mesh::DEFAULT_NORMALPERVERTEX(true);
const bool Mesh::DEFAULT_COLORPERVERTEX(true);

const PolylinePtr Mesh::DEFAULT_SKELETON(0);


/* ----------------------------------------------------------------------- */


Mesh::Builder::Builder( )
  : ExplicitModel::Builder()
  , CCW(0)
  , NormalPerVertex(0)
  , ColorPerVertex(0)
  , Solid(0)
  , NormalList(0)
  , TexCoordList(0)
  , Skeleton(0)
{
}


Mesh::Builder::~Builder( ) {
  // nothing to do
}


void 
Mesh::Builder::destroy( ) {
  MeshDestroy( );
}

void 
Mesh::Builder::MeshDestroy( ) {
  EMDestroy();
  if (CCW) delete CCW;
  if (NormalPerVertex) delete NormalPerVertex;
  if (ColorPerVertex) delete ColorPerVertex;
  if (Solid) delete Solid;
  if (NormalList) delete NormalList;
  if (TexCoordList) delete TexCoordList;
  if (Skeleton) delete Skeleton;
}

bool 
Mesh::Builder::isValid( ) const{
  return MeshValid( );
}

bool 
Mesh::Builder::MeshValid( ) const{
  if(!EMValid())return false;
  
  if(NormalList){
    uint32_t _normalListSize = (*NormalList)->getSize();
	if(NormalPerVertex && *NormalPerVertex &&_normalListSize != (*PointList)->getSize()){
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Mesh","Normals","Number of normals must be compatible to PointList.");
	return false;
	}

    for (uint32_t _i = 0; _i < _normalListSize; _i++){
      if (!(*NormalList)->getAt(_i).isValid()) {
	genMessage
	  (ERRORMSG(INVALID_FIELD_ITH_VALUE_ssss),"Mesh","NormalList",number(_i+1).c_str(),"Must be a valid normal.");
	return false;
      }
      else if (!(*NormalList)->getAt(_i).isNormalized() ){
	genMessage
	  (WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"Mesh","NormalList",number(_i+1).c_str(),"Must be a normalized normal.");
	return false;
      }
    }
  }
  if(TexCoordList){
    uint32_t _texCoordListSize = (*TexCoordList)->getSize();
    for (uint32_t _i = 0; _i < _texCoordListSize; _i++){
      if (!(*TexCoordList)->getAt(_i).isValid()) {
	genMessage
	  (ERRORMSG(INVALID_FIELD_ITH_VALUE_ssss),"Mesh","TexCoordList",number(_i+1).c_str(),"Must be a valid texture coordinates.");
	return false;
      }

    }
  }

  // Skeleton field
  if (Skeleton && (! (Skeleton))){
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Mesh","Skeleton","Must be a valid skeleton.");
    return false;
  };

  return true;
}

/* ----------------------------------------------------------------------- */


Mesh::Mesh() :
  ExplicitModel(),
  __ccw(DEFAULT_CCW),
  __solid(DEFAULT_SOLID),
  __colorPerVertex(DEFAULT_COLORPERVERTEX),
  __normalPerVertex(DEFAULT_NORMALPERVERTEX),
  __normalList(0),
  __texCoordList(0),
  __skeleton(DEFAULT_SKELETON)
{
}

Mesh::Mesh( const Point3ArrayPtr& points, 
		   bool normalPerVertex,
		   bool ccw,
		   bool solid,
		   const PolylinePtr& skeleton ) :
  ExplicitModel(points),
  __ccw(ccw),
  __solid(solid),
  __colorPerVertex(DEFAULT_COLORPERVERTEX),
  __normalPerVertex(normalPerVertex),
  __normalList(0),
  __texCoordList(0),
  __skeleton(skeleton)
{
}

Mesh::Mesh( const Point3ArrayPtr& points,
		    const Point3ArrayPtr& normals ,
			const Color4ArrayPtr& colors ,
			const Point2ArrayPtr& texCoord ,
			bool normalPerVertex,
			bool colorPerVertex,
			bool ccw,
			bool solid ,
			const PolylinePtr& skeleton ) :
    ExplicitModel(points, colors),
    __ccw(ccw),
    __solid(solid),
    __normalPerVertex(normalPerVertex),
	__colorPerVertex(colorPerVertex),
    __normalList(normals),
    __texCoordList(texCoord),
    __skeleton(skeleton)
{
}

Mesh::~Mesh( ) {
}

const bool 
Mesh::getCCW( ) const {
  return __ccw;
}

bool& 
Mesh::getCCW( ) {
  return __ccw;
}

Point3ArrayPtr& 
Mesh::getNormalList( ) {
  return __normalList;
}

const Point3ArrayPtr& 
Mesh::getNormalList( ) const {
  return __normalList;
}

Point2ArrayPtr& 
Mesh::getTexCoordList( ) {
  return __texCoordList;
}

const Point2ArrayPtr& 
Mesh::getTexCoordList( ) const {
  return __texCoordList;
}


const PolylinePtr& 
Mesh::getSkeleton( ) const {
  return __skeleton;
}

PolylinePtr& 
Mesh::getSkeleton( ) {
  return __skeleton;
}

const bool 
Mesh::getSolid( ) const {
  return __solid;
}

bool& 
Mesh::getSolid( ) {
  return __solid;
}

bool 
Mesh::isACurve( ) const {
  return false;
}

bool 
Mesh::isASurface( ) const {
  return true;
}

bool 
Mesh::isAVolume( ) const {
  return __solid;
}

bool
Mesh::isCCWToDefault( ) const {
  return __ccw == DEFAULT_CCW;
}

bool 
Mesh::isSkeletonToDefault( ) const {
  return __skeleton == DEFAULT_SKELETON;
}

bool
Mesh::isSolidToDefault( ) const {
  return __solid == DEFAULT_SOLID;
}

bool
Mesh::isTexCoordListToDefault() const {
  return (!__texCoordList);
}

/// Returns whether \b ColorPerVertex is set to its default value.
bool Mesh::isColorPerVertexToDefault() const{
	return __colorPerVertex == DEFAULT_COLORPERVERTEX;
}

/// Returns whether \b NormalPerVertex is set to its default value.
bool Mesh::isNormalPerVertexToDefault() const{
	return __normalPerVertex == DEFAULT_NORMALPERVERTEX;
}

bool
Mesh::isNormalListToDefault() const {
  if(!__normalList)return true;
  else if(getNormalPerVertex()){
	Point3ArrayPtr nmls = computeNormalPerVertex();
	return (*nmls == *__normalList);
  }
  else {
	Point3ArrayPtr nmls = computeNormalPerFace();
	return (*nmls == *__normalList);
  }
}

void
Mesh::computeNormalList(bool pervertex){
	__normalPerVertex = pervertex;
  if(pervertex)
	__normalList = computeNormalPerVertex();
  else
	__normalList = computeNormalPerFace();
}

bool
Mesh::hasTexCoordList( ) const {
  return __texCoordList.isValid();
}

/// Returns the center of the \b i-th face.
Vector3 
Mesh::getFaceCenter( uint32_t i ) const 
{ 
    uint32_t nbpoints = getFaceSize(i);
    Vector3 center;
    for(uint32_t j = 0; j < nbpoints; ++j)
        center += getFacePointAt(i,j);
    return center/nbpoints;
}

const Vector3& Mesh::getPointAt( uint32_t i ) const {
  GEOM_ASSERT(__pointList.isValid());
  GEOM_ASSERT(i < __pointList->getSize());
  return __pointList->getAt(i);
}


/* ----------------------------------------------------------------------- */


