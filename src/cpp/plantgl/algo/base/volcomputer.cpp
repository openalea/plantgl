/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
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




#include "volcomputer.h"
#include "discretizer.h"

#include <plantgl/pgl_scene.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/container/geometryarray2.h>

#include <plantgl/math/util_math.h>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

#define GEOM_NOT_CACHE

#ifdef GEOM_NOT_CACHE

#define GEOM_VOLCOMPUTER_CHECK_CACHE(geom);
#define GEOM_VOLCOMPUTER_UPDATE_CACHE(geom);

#else

#define GEOM_VOLCOMPUTER_CHECK_CACHE(geom) \
  Cache<real_t>::Iterator _it = __cache.find(geom->getId()); \
  if (! (_it == __cache.end())) { \
    __result = _it->second; \
    return true; \
  };


#define GEOM_VOLCOMPUTER_UPDATE_CACHE(geom) \
  __cache.insert(geom->getId(),__result);


#endif

#define GEOM_DISCRETIZE(geom) \
  GEOM_VOLCOMPUTER_CHECK_CACHE(geom); \
  geom->apply(__discretizer); \
  if((__discretizer.getDiscretization())->apply(*this)){ \
   GEOM_VOLCOMPUTER_UPDATE_CACHE(geom); \
   return true; \
  } \
  else return false; \


/* ----------------------------------------------------------------------- */


VolComputer::VolComputer( Discretizer& discretizer ) :
  SurfComputer(discretizer){
}

VolComputer::~VolComputer( ) {
}

real_t VolComputer::getVolume( ) {
    return __result;
  }


/* ----------------------------------------------------------------------- */


bool VolComputer::process(Shape * Shape){
  __result = 0;
  GEOM_ASSERT(Shape);
  return Shape->getGeometry()->apply(*this);
}

bool VolComputer::process(Inline * geomInline){
    __result = 0;
  GEOM_ASSERT(geomInline);
    return process(geomInline->getScene());
}

/* ----------------------------------------------------------------------- */


bool VolComputer::process( AmapSymbol * amapSymbol ) {
  __result = 0;
  GEOM_ASSERT(amapSymbol);

  GEOM_VOLCOMPUTER_CHECK_CACHE(amapSymbol);

  if(amapSymbol->getSolid()){
    uint_t _iSize = amapSymbol->getIndexListSize();
    Vector3 center = amapSymbol->getPointList()->getCenter();
    for (uint_t _i = 0; _i < _iSize; _i++) {
      const Index& _index = amapSymbol->getIndexList()->getAt(_i);
      uint_t _jSize = _index.getSize();
      const Vector3& _origfacet = amapSymbol->getFacePointAt(_i,0);
      Vector3 _center = center - _origfacet;
      for (uint_t _j = 1; _j < _jSize - 1; _j++) {
	Vector3 _vi = amapSymbol->getFacePointAt(_i,_j) - _origfacet;
	Vector3 _vj = amapSymbol->getFacePointAt(_i,_j + 1) - _origfacet;
	__result += fabs(dot(_center,cross(_vi,_vj)))/6;
       };
    };
  }

  GEOM_VOLCOMPUTER_UPDATE_CACHE(amapSymbol);

  return true;
}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( AsymmetricHull * asymmetricHull ) {
  __result = 0;
  GEOM_ASSERT(asymmetricHull);

  GEOM_DISCRETIZE(asymmetricHull);

}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( AxisRotated * axisRotated ) {
  __result = 0;
  GEOM_ASSERT(axisRotated);

  // A rotation does not affect a volume
  axisRotated->getGeometry()->apply(*this);

  return true;
}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( Box * box ) {
  __result = 0;
  GEOM_ASSERT(box);

  GEOM_VOLCOMPUTER_CHECK_CACHE(box);
  Vector3 dim = box->getSize();
  __result = 8 * dim.x() * dim.y() * dim.z();
  GEOM_VOLCOMPUTER_UPDATE_CACHE(box);

  return true;
}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( Cone * cone ) {
  __result = 0;
  GEOM_ASSERT(cone);


  GEOM_VOLCOMPUTER_CHECK_CACHE(cone);

  real_t _radius = cone->getRadius();
  real_t _height = cone->getHeight();
  if(cone->getSolid()) {
    /// V = 1/3 * PI * r�  * h
    __result = (GEOM_PI * sq(_radius) * _height) / 3;
  }
  else {
    __result = 0;
  }

  GEOM_VOLCOMPUTER_UPDATE_CACHE(cone);
  return true;
}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( Cylinder * cylinder ) {
  __result = 0;
  GEOM_ASSERT(cylinder);

  real_t _radius = cylinder->getRadius();
  real_t _height = cylinder->getHeight();
  if(cylinder->getSolid()) {
      //  V = PI * r�  * h
      __result = GEOM_PI * sq(_radius) * _height;
  }
  else {
      __result = 0;
  }

  return true;
}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( EulerRotated * eulerRotated ) {
  __result = 0;
  GEOM_ASSERT(eulerRotated);

  // A rotation does not affect a volume
  eulerRotated->getGeometry()->apply(*this);

  return true;
}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( ExtrudedHull * extrudedHull ) {
  __result = 0;
  GEOM_ASSERT(extrudedHull);
  GEOM_DISCRETIZE(extrudedHull);
  return true;
}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( FaceSet * faceSet ) {
  __result = 0;
  GEOM_ASSERT(faceSet);

  GEOM_VOLCOMPUTER_CHECK_CACHE(faceSet);

  if(faceSet->getSolid()){
    uint_t _iSize = faceSet->getIndexListSize();
    Vector3 center = faceSet->getPointList()->getCenter();
    for (uint_t _i = 0; _i < _iSize; _i++) {
      const Index& _index = faceSet->getIndexList()->getAt(_i);
      uint_t _jSize = _index.getSize();
      const Vector3& _origfacet = faceSet->getFacePointAt(_i,0);
      Vector3 _center = center - _origfacet;
      for (uint_t _j = 1; _j < _jSize - 1; _j++) {
	Vector3 _vi = faceSet->getFacePointAt(_i,_j) - _origfacet;
	Vector3 _vj = faceSet->getFacePointAt(_i,_j + 1) - _origfacet;
	__result += fabs(dot(_center,cross(_vi,_vj)))/6;
       };
    };
  }


  GEOM_VOLCOMPUTER_UPDATE_CACHE(faceSet);

  return true;
}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( Frustum * frustum ) {
  __result = 0;
  GEOM_ASSERT(frustum);
  GEOM_VOLCOMPUTER_CHECK_CACHE(frustum);
  real_t _radius = frustum->getRadius();
  real_t _height = frustum->getHeight();
  real_t _taper = frustum->getTaper();

  real_t _top_radius = _radius * _taper;

  if(frustum->getSolid()) {
    // V = 1/3 * Pi * h [ r1 � + r1*r2 + r2 � ].
    __result = (GEOM_PI * _height* ( sq(_radius) + _radius*_top_radius + sq(_top_radius)))/3;
  }
  else {
    __result = 0;
  }


  GEOM_VOLCOMPUTER_UPDATE_CACHE(frustum);

  return true;
}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( Extrusion * extrusion) {
  __result = 0;
  GEOM_ASSERT(extrusion);


  GEOM_DISCRETIZE(extrusion);
}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( Group * group ) {
  __result = 0;
  GEOM_ASSERT(group);

  GEOM_VOLCOMPUTER_CHECK_CACHE(group);

  // Cumulates the volume for each elements within the group
  real_t _volume = 0;
  const GeometryArrayPtr& _geometryList = group->getGeometryList();
  for (GeometryArray::const_iterator _i = _geometryList->getBegin();
       _i != _geometryList->getEnd();
       _i++) {
    (*_i)->apply(*this);
    _volume += __result;
  };
  __result = _volume;

  GEOM_VOLCOMPUTER_UPDATE_CACHE(group);

  return true;
}


/* ----------------------------------------------------------------------- */
// volume(T*Object)= det(T-Translation(T)) * Volume(Object)
// For an IFS, volume= [ sum_i( det(T_i) ) ** depth ] * volume(Object)

bool VolComputer::process( IFS * ifs) {
  __result = 0;
  GEOM_ASSERT(ifs);
GEOM_TRACE("process IFS");

//todo OK
  const Transform4ArrayPtr& tList= ifs->getTransfoList();
  Transform4Array::const_iterator t_It= tList->getBegin();
  real_t volume = 0.;
  while( t_It != tList->getEnd() )
    {
    volume+= (*t_It)->getVolume();
    t_It++;
    }

  volume= IFS::power(volume,ifs->getDepth());

  ifs->getGeometry()->apply(*this);
  __result*= volume;

  return true;
}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( Oriented * oriented ) {
  __result = 0;
  GEOM_ASSERT(oriented);

  // A rotation does not affect a volume
  oriented->getGeometry()->apply(*this);

  return true;
}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( Paraboloid * paraboloid ) {
  __result = 0;
  GEOM_ASSERT(paraboloid);
  if(paraboloid->getSolid()){
    /// V =  Pi * h * r� * s / ( s + 2 )

    const real_t& radius = paraboloid->getRadius();
    const real_t& height = paraboloid->getHeight();
    const real_t& shape = paraboloid->getShape();
    __result =  GEOM_PI  * height * sq(radius) * shape / (shape + 2);
  }
  return true;

//  GEOM_DISCRETIZE(paraboloid);

}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( Revolution * revolution ) {
  __result = 0;
  GEOM_ASSERT(revolution);

  GEOM_DISCRETIZE(revolution);

}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( Swung * swung )
{
  __result = 0;
  GEOM_ASSERT(swung);

  GEOM_DISCRETIZE(swung);
}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( QuadSet * quadSet ) {
  __result = 0;
  GEOM_ASSERT(quadSet);

  GEOM_VOLCOMPUTER_CHECK_CACHE(quadSet);

  if(quadSet->getSolid()){
    uint_t _size = quadSet->getIndexListSize();
    Vector3 center = quadSet->getPointList()->getCenter();    
    for (uint_t _i = 0; _i < _size; _i++) {
      const Vector3& _origfacet = quadSet->getFacePointAt(_i,0);
      Vector3 _center = center - _origfacet;
      Vector3 _v1 = quadSet->getFacePointAt(_i,1) - _origfacet;
      Vector3 _v2 = quadSet->getFacePointAt(_i,2) - _origfacet;
      Vector3 _v3 = quadSet->getFacePointAt(_i,3) - _origfacet;
      __result += fabs(dot(_center,cross(_v1,_v2)))/6;
      __result += fabs(dot(_center,cross(_v2,_v3)))/6;
    };
  }

  GEOM_VOLCOMPUTER_UPDATE_CACHE(quadSet);

  return true;
}

/* ----------------------------------------------------------------------- */


bool VolComputer::process( Scaled * scaled ) {
  __result = 0;
  GEOM_ASSERT(scaled);

  scaled->getGeometry()->apply(*this);
  __result *= scaled->getScale().x() * scaled->getScale().y() * scaled->getScale().z();
  return true;
}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( Sphere * sphere ) {
  __result = 0;
  GEOM_ASSERT(sphere);

  // 4/3 PI r * r * r
  real_t _radius = sphere->getRadius();
  __result = GEOM_TWO_PI * 2 * cb(_radius) / 3 ;

  return true;
}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( Tapered * tapered ) {
  __result = 0;
  GEOM_ASSERT(tapered);

  GEOM_DISCRETIZE(tapered);

}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( Translated * translated ) {
  __result = 0;
  GEOM_ASSERT(translated);

  // A translation does not affect a volume
  return translated->getGeometry()->apply(*this);
}


/* ----------------------------------------------------------------------- */


bool VolComputer::process( TriangleSet * triangleSet ) {
  __result = 0;
  GEOM_ASSERT(triangleSet);

  GEOM_VOLCOMPUTER_CHECK_CACHE(triangleSet);

  if(triangleSet->getSolid()){
    /// For each facet (v0,v1,v2) we compute the volume of the tetrahedron (v0,v1,v2,center of triangleSets)
    /// For this v = 1/3 | xC yC zC 1 |
    ///                  | x0 y0 z0 1 |
    ///                  | x1 y1 z1 1 |
    ///                  | x2 y2 z2 1 |
    /// Normaly coef is 1/6 (??)

    uint_t _size = triangleSet->getIndexListSize();
    Vector3 center = triangleSet->getPointList()->getCenter();
    for (uint_t _i = 0; _i < _size; _i++) {
      const Vector3& _origfacet = triangleSet->getFacePointAt(_i,0);
      Vector3 _center = center - _origfacet;
      Vector3 _v1 = triangleSet->getFacePointAt(_i,1) - _origfacet;
      Vector3 _v2 = triangleSet->getFacePointAt(_i,2) - _origfacet;
      __result += fabs(dot(_center,cross(_v1,_v2)))/6;
    };
  }
  GEOM_VOLCOMPUTER_UPDATE_CACHE(triangleSet);

  return true;
}


/* ----------------------------------------------------------------------- */

bool VolComputer::process(const ScenePtr scene){
  if(!scene || scene->isEmpty()){
    __result = 0;
    return false;
  }
  real_t volume = 0;
  for(Scene::const_iterator _it=scene->getBegin();
      _it!=scene->getEnd();
      _it++)
    if((*_it)->apply(*this))
      volume+=__result;
  __result = volume;
  return true;
}

bool VolComputer::process(const Scene& scene){
  if(scene.isEmpty()){
    __result = 0;
    return false;
  }
  real_t volume = 0;
  for(Scene::const_iterator _it=scene.getBegin();
      _it!=scene.getEnd();
      _it++)
    if((*_it)->apply(*this))
      volume+=__result;
  __result = volume;
  return true;
}

/* ----------------------------------------------------------------------- */

real_t PGL(sceneVolume)(const ScenePtr scene){
  Discretizer dis;
  VolComputer _sfc(dis);
  if(_sfc.process(scene))
    return _sfc.getVolume();
  else return 0;
}

real_t PGL(sceneVolume)(const Scene& scene){
  Discretizer dis;
  VolComputer _sfc(dis);
  if(_sfc.process(scene))
    return _sfc.getVolume();
  else return 0;
}

/* ----------------------------------------------------------------------- */

bool VolComputer::process( Material * material ) {
  GEOM_ASSERT(material);
  // nothing to do
  __result = 0;
  return false;
}

bool VolComputer::process( ImageTexture * texture ) {
  GEOM_ASSERT(texture);
  // nothing to do
  __result = 0;
  return false;
}

bool VolComputer::process( MonoSpectral * monoSpectral ) {
  GEOM_ASSERT(monoSpectral);
  // nothing to do
  __result = 0;
  return false;
}

bool VolComputer::process( MultiSpectral * multiSpectral ) {
  GEOM_ASSERT(multiSpectral);
  // nothing to do
  __result = 0;
  return false;
}

bool VolComputer::process( BezierPatch * bezierPatch ){
  // nothing to do
  __result = 0;
  return true;
};

bool VolComputer::process( ElevationGrid * elevationGrid ){
  // nothing to do
  __result = 0;
  return true;
};

bool VolComputer::process( NurbsPatch * nurbsPatch ){
  // nothing to do
  __result = 0;
  return true;
};

bool VolComputer::process( Disc * disc ){
  // nothing to do
  __result = 0;
  return true;
};


/* ------------------------------------------------------------------------- */
