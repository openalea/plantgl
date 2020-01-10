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




#include <plantgl/math/util_math.h>

#include "surfcomputer.h"
#include "discretizer.h"

#include <plantgl/pgl_scene.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/container/geometryarray2.h>

PGL_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */

#define GEOM_NOT_CACHE

#ifdef GEOM_NOT_CACHE

#define GEOM_SURFCOMPUTER_CHECK_CACHE(geom);
#define GEOM_SURFCOMPUTER_UPDATE_CACHE(geom);

#else

#define GEOM_SURFCOMPUTER_CHECK_CACHE(geom) \
  Cache<real_t>::Iterator _it = __cache.find(geom->getObjectId()); \
  if (! (_it == __cache.end())) { \
    __result = _it->second; \
    cerr << geom->getName() <<"> Use of cache " << __result << " for " << geom->getObjectId() << endl; \
    return true; \
  };


#define GEOM_SURFCOMPUTER_UPDATE_CACHE(geom) \
  __cache.insert(geom->getObjectId(),__result); \
    cerr << geom->getName()  << "> Fill cache " << __result << " for " << geom->getObjectId() << endl; \

#endif

#define GEOM_DISCRETIZE(geom) \
  GEOM_SURFCOMPUTER_CHECK_CACHE(geom); \
  if(geom->apply(__discretizer)){ \
   ExplicitModelPtr expl =ExplicitModelPtr(__discretizer.getDiscretization()); \
   if(expl->apply(*this)){ \
    GEOM_SURFCOMPUTER_UPDATE_CACHE(geom); \
    return true; \
   } \
   else { \
    cerr << "Error with surface computer !" << endl; \
    return false; \
   } \
  } \
  else { \
   cerr << "Error with discretization !" << endl; \
   return false; \
  } \

/* ----------------------------------------------------------------------- */


SurfComputer::SurfComputer( Discretizer& discretizer ) :
  Action(),
  __cache(),
  __result(0),
  __discretizer(discretizer) {
}

SurfComputer::~SurfComputer( ) {
}

void SurfComputer::clear( ) {
  __result = 0;
  __cache.clear();
}

real_t SurfComputer::getSurface( ) {
  return __result;
}

Discretizer&
SurfComputer::getDiscretizer( ) {
  return __discretizer;
}

/* ----------------------------------------------------------------------- */


bool SurfComputer::process(Shape * Shape){
    GEOM_ASSERT(Shape);
    GEOM_SURFCOMPUTER_CHECK_CACHE(Shape);
    bool b = Shape->getGeometry()->apply(*this);
    GEOM_SURFCOMPUTER_UPDATE_CACHE(Shape);
    return b;
}

bool SurfComputer::process(Inline * geomInline){
    GEOM_ASSERT(geomInline);
    return process(geomInline->getScene());
}

/* ----------------------------------------------------------------------- */


bool SurfComputer::process( AmapSymbol * amapSymbol ) {
  GEOM_ASSERT(amapSymbol);

  GEOM_SURFCOMPUTER_CHECK_CACHE(amapSymbol);

  __result  = 0;
  uint_t _iSize = amapSymbol->getIndexListSize();
  for (uint_t _i = 0; _i < _iSize; _i++) {
    const Index& _index = amapSymbol->getIndexList()->getAt(_i);
    uint_t _jSize = _index.size();
    for (uint_t _j = 1; _j < _jSize - 1; _j++) {
      /* Vector3 _vi = amapSymbol->getPointAt(_i,_j) -
                    amapSymbol->getPointAt(_i,0);
      Vector3 _vj = amapSymbol->getPointAt(_i,_j + 1) -
                    amapSymbol->getPointAt(_i,0);
      __result += 0.5 * norm(cross(_vi,_vj)); */
      __result += surface(amapSymbol->getFacePointAt(_i,0),
                          amapSymbol->getFacePointAt(_i,_j),
                          amapSymbol->getFacePointAt(_i,_j + 1));
    };
  };

  GEOM_SURFCOMPUTER_UPDATE_CACHE(amapSymbol);

  return true;
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( AsymmetricHull * asymmetricHull ) {
  GEOM_ASSERT(asymmetricHull);

  GEOM_DISCRETIZE(asymmetricHull);

}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( AxisRotated * axisRotated ) {
  GEOM_ASSERT(axisRotated);

  // A rotation does not affect a surface
  axisRotated->getGeometry()->apply(*this);

  return true;
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( BezierPatch * bezierPatch ) {
  GEOM_ASSERT(bezierPatch);

  GEOM_DISCRETIZE(bezierPatch);

}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( Box * box ) {
  GEOM_ASSERT(box);

  GEOM_SURFCOMPUTER_CHECK_CACHE(box);
  Vector3 dim = box->getSize();
  __result = 8 * ( dim.x() * dim.y() +  dim.x() * dim.z() + dim.y() * dim.z());
  GEOM_SURFCOMPUTER_UPDATE_CACHE(box);

  return true;
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( Cone * cone ) {
  GEOM_ASSERT(cone);

  GEOM_SURFCOMPUTER_CHECK_CACHE(cone);

  real_t _radius = cone->getRadius();
  real_t _height = cone->getHeight();
  if(!cone->getSolid()) {
      // Side Area : Pi * r * racine2(r2 + h2).
      __result = GEOM_TWO_PI * _radius * sqrt(sq(_radius) + sq(_height))/2;
  }
  else {
      // Side and Base Area : Pi * r * ( r *  racine2(r2 + h2)).
      __result =  GEOM_TWO_PI  * _radius * ( _radius + sqrt(sq(_radius) + sq(_height)) )/2;
  }

  GEOM_SURFCOMPUTER_UPDATE_CACHE(cone);
  return true;
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( Cylinder * cylinder ) {
  GEOM_ASSERT(cylinder);

  real_t _radius = cylinder->getRadius();
  real_t _height = cylinder->getHeight();
  if(!cylinder->getSolid()) {
      //  Side Area : 2 Pi r h
      __result = GEOM_TWO_PI * _radius * _height;
  }
  else {
      // Side and Base Area :  2 Pi r h + 2 Pi r2
      __result = GEOM_TWO_PI  * _radius * ( _radius + _height );
  }

  return true;
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( ElevationGrid * elevationGrid ) {
  GEOM_ASSERT(elevationGrid);

  GEOM_DISCRETIZE(elevationGrid);

}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( EulerRotated * eulerRotated ) {
  GEOM_ASSERT(eulerRotated);

  // A rotation does not affect a surface
  eulerRotated->getGeometry()->apply(*this);

  return true;
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( ExtrudedHull * extrudedHull ) {
  GEOM_ASSERT(extrudedHull);

  GEOM_DISCRETIZE(extrudedHull);

}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( FaceSet * faceSet ) {
  GEOM_ASSERT(faceSet);

  GEOM_SURFCOMPUTER_CHECK_CACHE(faceSet);

  __result  = 0;
  uint_t _iSize = faceSet->getIndexListSize();
  for (uint_t _i = 0; _i < _iSize; _i++) {
    const Index& _index = faceSet->getIndexList()->getAt(_i);
    uint_t _jSize = _index.size();
    for (uint_t _j = 1; _j < _jSize - 1; _j++) {
      __result += surface(faceSet->getFacePointAt(_i,0),faceSet->getFacePointAt(_i,_j),faceSet->getFacePointAt(_i,_j+1));
    };
  };

  GEOM_SURFCOMPUTER_UPDATE_CACHE(faceSet);

  return true;
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( Frustum * frustum ) {
  GEOM_ASSERT(frustum);
  __result = 0;
  GEOM_SURFCOMPUTER_CHECK_CACHE(frustum);
  real_t _radius = frustum->getRadius();
  real_t _height = frustum->getHeight();
  real_t _taper = frustum->getTaper();
  real_t _rtaper = fabs(1 - _taper);

  if(  _rtaper > GEOM_EPSILON){

      real_t _top_radius = _radius * _taper;
      real_t h = _height / _rtaper;
      real_t _hdif = (_height * _taper) / _rtaper;

      if(!frustum->getSolid()) {
          // q : top radius. h : height of the cone containing the frustrum. a : height of the frustrum.
          // Side Area : Pi * [ r * racine2(r2 + h2) - q * racine2(q2 + (h -a)2) ].
          __result = GEOM_TWO_PI * ( _radius * sqrt(sq(_radius) + sq(h)) - _top_radius * sqrt( sq(_top_radius) + sq(_hdif)))/2;
      }
      else {
          // Side and Base Area : Pi * [ r + q + r * racine2(r2 + h2) - q * racine2(q2 + (h -a)2) ].
          __result = GEOM_TWO_PI  * ( _radius + _top_radius + _radius * sqrt(sq(_radius) + sq(h)) - _top_radius * sqrt( sq(_top_radius) + sq(_hdif)))/2;
      }
  }
  else {
      if(!frustum->getSolid()) {
          //  Side Area : 2 Pi r h
          __result = GEOM_TWO_PI * _radius * _height;
      }
      else {
          // Side and Base Area :  2 Pi r h + 2 Pi r2
          __result = GEOM_TWO_PI  * _radius * ( _radius + _height );
      }
  }


  GEOM_SURFCOMPUTER_UPDATE_CACHE(frustum);

  return true;
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( Extrusion * extrusion) {
  GEOM_ASSERT(extrusion);

  GEOM_DISCRETIZE(extrusion);
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( Group * group ) {
  GEOM_ASSERT(group);

  GEOM_SURFCOMPUTER_CHECK_CACHE(group);

  // Cumulates the surface for each elements within the group
  __result = 0;
  real_t _surface = 0;
  const GeometryArrayPtr& _geometryList = group->getGeometryList();
  for (GeometryArray::const_iterator _i = _geometryList->begin();
       _i != _geometryList->end();
       _i++) {
    (*_i)->apply(*this);
    _surface += __result;
  };
  __result = _surface;

  GEOM_SURFCOMPUTER_UPDATE_CACHE(group);

  return true;
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( IFS * ifs ) {
  GEOM_ASSERT(ifs);
GEOM_TRACE("process IFS");

  GEOM_DISCRETIZE(ifs);
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( NurbsPatch * nurbsPatch ) {
  GEOM_ASSERT(nurbsPatch);

  GEOM_DISCRETIZE(nurbsPatch);

}

/* ----------------------------------------------------------------------- */


bool SurfComputer::process( Oriented * oriented ) {
  GEOM_ASSERT(oriented);

  // A rotation does not affect a surface
  oriented->getGeometry()->apply(*this);

  return true;
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( Paraboloid * paraboloid ) {
  GEOM_ASSERT(paraboloid);

  GEOM_DISCRETIZE(paraboloid);

}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( Revolution * revolution ) {
  GEOM_ASSERT(revolution);

  GEOM_DISCRETIZE(revolution);

}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( Swung * swung )
{
  GEOM_ASSERT(swung);

  GEOM_DISCRETIZE(swung);
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( QuadSet * quadSet ) {
  GEOM_ASSERT(quadSet);

  GEOM_SURFCOMPUTER_CHECK_CACHE(quadSet);

  __result  = 0;
  uint_t _size = quadSet->getIndexListSize();
  for (uint_t _i = 0; _i < _size; _i++) {
    Vector3 _v1 = quadSet->getFacePointAt(_i,1) - quadSet->getFacePointAt(_i,0);
    Vector3 _v2 = quadSet->getFacePointAt(_i,2) - quadSet->getFacePointAt(_i,0);
    Vector3 _v3 = quadSet->getFacePointAt(_i,3) - quadSet->getFacePointAt(_i,0);
    __result += 0.5 * norm(cross(_v1,_v2));
    __result += 0.5 * norm(cross(_v2,_v3));
  };

  GEOM_SURFCOMPUTER_UPDATE_CACHE(quadSet);

  return true;
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( Scaled * scaled ) {
  GEOM_ASSERT(scaled);

  GEOM_DISCRETIZE(scaled);
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( ScreenProjected * scp) {
  GEOM_ASSERT(scp);
  return (scp->getGeometry()?scp->getGeometry()->apply(*this):false);
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( Sphere * sphere ) {
  GEOM_ASSERT(sphere);

  // 4 PI r2
  const real_t& _radius = sphere->getRadius();
  __result = GEOM_TWO_PI * 2 * sq(_radius);

  return true;
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( Tapered * tapered ) {
  GEOM_ASSERT(tapered);

  GEOM_DISCRETIZE(tapered);

}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( Translated * translated ) {
  GEOM_ASSERT(translated);

  // A translation does not affect a surface
  return translated->getGeometry()->apply(*this);
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( TriangleSet * triangleSet ) {
  GEOM_ASSERT(triangleSet);

  GEOM_SURFCOMPUTER_CHECK_CACHE(triangleSet);

  __result  = 0;
  uint_t _size = triangleSet->getIndexListSize();
  for (uint_t _i = 0; _i < _size; _i++) {
    __result += surface(triangleSet->getFacePointAt(_i,0),
                        triangleSet->getFacePointAt(_i,1),
                        triangleSet->getFacePointAt(_i,2));
  };

  GEOM_SURFCOMPUTER_UPDATE_CACHE(triangleSet);

  return true;
}


/* ----------------------------------------------------------------------- */


bool SurfComputer::process( Disc * disc ) {
  GEOM_ASSERT(disc);

  // 2 PI r
  __result = GEOM_TWO_PI * disc->getRadius();

  return true;
}


/* ----------------------------------------------------------------------- */

bool SurfComputer::process(const ScenePtr scene){
  if(!scene || scene->empty()){
    return false;
  }
  Cache<real_t>::Iterator _it = __cache.find((size_t)scene.get());
  if (! (_it == __cache.end())) {
    __result = _it->second;
    return true;
  }
  __result = 0;
  real_t surface = 0;
  for(Scene::const_iterator _it2=scene->begin();
      _it2!=scene->end();
      _it2++)
    if((*_it2)->apply(*this))
      surface+=__result;
  __result = surface;
  __cache.insert((size_t)scene.get(),__result);
  return true;
}

bool SurfComputer::process(const Scene& scene){
  if(scene.empty()){
    return false;
  }
  Cache<real_t>::Iterator _it = __cache.find((size_t)&scene);
  if (! (_it == __cache.end())) {
    __result = _it->second;
    return true;
  }
  __result = 0;
  real_t surface = 0;
  for(Scene::const_iterator _it2=scene.begin();
      _it2!=scene.end();
      _it2++)
    if((*_it2)->apply(*this))
      surface+=__result;
  __result = surface;
  __cache.insert((size_t)&scene,__result);
  return true;
}

/* ----------------------------------------------------------------------- */

real_t PGL(sceneSurface)(const ScenePtr scene){
  Discretizer dis;
  SurfComputer _sfc(dis);
  if(_sfc.process(scene))
    return _sfc.getSurface();
  else return 0;
}

real_t PGL(sceneSurface)(const Scene& scene){
  Discretizer dis;
  SurfComputer _sfc(dis);
  if(_sfc.process(scene))
    return _sfc.getSurface();
  else return 0;
}

/* ----------------------------------------------------------------------- */

bool SurfComputer::process( Material * material ) {
  GEOM_ASSERT(material);
  // nothing to do
  __result = 0;
  return false;
}

bool SurfComputer::process( ImageTexture * texture ) {
  GEOM_ASSERT(texture);
  // nothing to do
  __result = 0;
  return false;
}

bool SurfComputer::process( Texture2D * texture ) {
  GEOM_ASSERT(texture);
  // nothing to do
  __result = 0;
  return false;
}

bool SurfComputer::process( Texture2DTransformation * texture ) {
  GEOM_ASSERT(texture);
  // nothing to do
  __result = 0;
  return false;
}

bool SurfComputer::process( MonoSpectral * monoSpectral ) {
  GEOM_ASSERT(monoSpectral);
  // nothing to do
  __result = 0;
  return false;
}

bool SurfComputer::process( MultiSpectral * multiSpectral ) {
  GEOM_ASSERT(multiSpectral);
  // nothing to do
  __result = 0;
  return false;
}

bool SurfComputer::process( BezierCurve * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);
  // nothing to do
  __result = 0;
  return true;
};

bool SurfComputer::process( NurbsCurve * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);
  // nothing to do
  __result = 0;
  return true;
};

bool SurfComputer::process( PointSet * pointSet ) {
  GEOM_ASSERT(pointSet);
  // nothing to do
  __result = 0;
  return true;
}

bool SurfComputer::process( Polyline * polyline ) {
  GEOM_ASSERT(polyline);
  // nothing to do
  __result = 0;
  return true;
};

bool SurfComputer::process( BezierCurve2D * bezierCurve ){
  GEOM_ASSERT(bezierCurve);
  // nothing to do
  __result = 0;
  return true;
};

bool SurfComputer::process( NurbsCurve2D * nurbsCurve ){
  GEOM_ASSERT(nurbsCurve);
  // nothing to do
  __result = 0;
  return true;
};

bool SurfComputer::process( PointSet2D * pointSet ){
  GEOM_ASSERT(pointSet);
  // nothing to do
  __result = 0;
  return true;
}

bool SurfComputer::process( Polyline2D * polyline ){
  GEOM_ASSERT(polyline);
  // nothing to do
  __result = 0;
  return true;
};

bool SurfComputer::process( Text * text ){
  GEOM_ASSERT(text);
  // nothing to do
  __result = 0;
  return true;
}

bool SurfComputer::process( Font * font ){
  GEOM_ASSERT(font);
  // nothing to do
  __result = 0;
  return true;
};
