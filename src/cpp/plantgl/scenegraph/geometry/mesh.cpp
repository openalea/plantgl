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





#include "mesh.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include "polyline.h"
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/colorarray.h>
#include <plantgl/tool/util_string.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


const bool Mesh::DEFAULT_CCW(true);
const bool Mesh::DEFAULT_SOLID(false);
const bool Mesh::DEFAULT_NORMALPERVERTEX(true);
const bool Mesh::DEFAULT_COLORPERVERTEX(true);

const Vector3 Mesh::DEFAULT_NORMAL_VALUE(0,0,1);
const PolylinePtr Mesh::DEFAULT_SKELETON;


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
    uint_t _normalListSize = (*NormalList)->size();
    if(NormalPerVertex && *NormalPerVertex &&_normalListSize != (*PointList)->size()){
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Mesh","Normals","Number of normals must be compatible to PointList.");
    return false;
    }

    for (uint_t _i = 0; _i < _normalListSize; _i++){
      if (!(*NormalList)->getAt(_i).isValid()) {
    pglErrorEx
      (PGLERRORMSG(INVALID_FIELD_ITH_VALUE_ssss),"Mesh","NormalList",number(_i+1).c_str(),"Must be a valid normal.");
    return false;
      }
      else if (!(*NormalList)->getAt(_i).isNormalized() ){
    pglErrorEx
      (PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"Mesh","NormalList",number(_i+1).c_str(),"Must be a normalized normal.");
    return false;
      }
    }
  }
  if(TexCoordList){
    uint_t _texCoordListSize = (*TexCoordList)->size();
    for (uint_t _i = 0; _i < _texCoordListSize; _i++){
      if (!(*TexCoordList)->getAt(_i).isValid()) {
    pglErrorEx
      (PGLERRORMSG(INVALID_FIELD_ITH_VALUE_ssss),"Mesh","TexCoordList",number(_i+1).c_str(),"Must be a valid texture coordinates.");
    return false;
      }

    }
  }

  // Skeleton field
  if (Skeleton && (! (Skeleton))){
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Mesh","Skeleton","Must be a valid skeleton.");
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
  __normalList(),
  __texCoordList(),
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
  __normalList(),
  __texCoordList(),
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
  return (__texCoordList);
}

/// Returns the center of the \b i-th face.
Vector3
Mesh::getFaceCenter( uint_t i ) const
{
    uint_t nbpoints = getFaceSize(i);
    Vector3 center;
    for(uint_t j = 0; j < nbpoints; ++j)
        center += getFacePointAt(i,j);
    return center/nbpoints;
}


Point3ArrayPtr
Mesh::computeNormalPerVertex() const {
    std::vector<bool> hasNormal(__pointList->size(),false);
    Point3ArrayPtr normalList(new Point3Array(__pointList->size()));
    uint_t i = 0;
    uint_t j = 0;
    for(j=0; j < getIndexListSize(); j++){
        Vector3 _norm = cross(getFacePointAt(j,__ccw ? 1 : 2) - getFacePointAt(j,0),
                              getFacePointAt(j,__ccw ? 2 : 1) - getFacePointAt(j,0));
        for(i = 0; i < getFaceSize(j); i++){
            uint_t _index = getFacePointIndexAt(j,i);
            normalList->getAt(_index)+=_norm;
            hasNormal[_index] = true;
        }
    }
    for(i=0; i < __pointList->size(); i++) {
        if(!hasNormal[i] )
            normalList->setAt(i,Vector3( 1,0,0 ) );
    }
    for(Point3Array::iterator _it=normalList->begin();_it!=normalList->end();_it++)
    {
        _it->normalize();
        if (fabs(norm(*_it) - 1.0) > GEOM_EPSILON) *_it = DEFAULT_NORMAL_VALUE;
    }

    return normalList;
}

Point3ArrayPtr
Mesh::computeNormalPerFace() const {
    Point3ArrayPtr normalList(new Point3Array(getIndexListSize()));
    for(uint_t j=0; j < getIndexListSize(); j++){
        normalList->setAt(j,cross(getFacePointAt(j,__ccw ? 1 : 2) - getFacePointAt(j,0),
                  getFacePointAt(j,__ccw ? 2 : 1) - getFacePointAt(j,0)));
    }
    bool hasinvalid = false;
    for(Point3Array::iterator _it=normalList->begin();_it!=normalList->end();_it++)
    {
        _it->normalize();
        if (fabs(norm(*_it) - 1.0) > GEOM_EPSILON) *_it = DEFAULT_NORMAL_VALUE;
    }

    return normalList;
}


/* ----------------------------------------------------------------------- */


