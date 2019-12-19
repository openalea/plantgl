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




#include "discretizer.h"
#include "wirecomputer.h"

#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/pgl_container.h>
#include <plantgl/scenegraph/scene/shape.h>

#include <plantgl/math/util_math.h>

#ifdef GEOM_DEBUG
#include <plantgl/tool/timer.h>
#endif

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


#define GEOM_WireComputer_CHECK_CACHE(geom) \
  if (!geom->unique()) { \
    Cache<GeometryPtr>::Iterator _it = __cache.find(geom->getId()); \
    if (! (_it == __cache.end())) { \
       __wire = GeometryPtr(_it->second); \
      if(__wire)return true; \
      else std::cerr << "Cache of WireComputer Error !" << std::endl; \
    }; \
  };


#define GEOM_WireComputer_UPDATE_CACHE(geom) \
  if (!geom->unique()) \
    __cache.insert(geom->getId(),__wire);


#define GEOM_WireComputer_TRANSFORM(transformed) \
  GEOM_WireComputer_CHECK_CACHE(transformed); \
  if((transformed->getGeometry())->apply(*this)){ \
    __wire = __wire->transform(transformed->getTransformation()); \
    GEOM_WireComputer_UPDATE_CACHE(transformed); \
  } \
  else { \
    __wire = GeometryPtr(); \
    return false; \
  }

#define GEOM_DISCRETIZE(geom) \
    GEOM_ASSERT(geom); \
    if(!(geom->apply(__discretizer))) { \
      __wire = GeometryPtr(); \
      return false; \
    } \
    else return __discretizer.getDiscretization()->apply(*this); \


/* ----------------------------------------------------------------------- */


WireComputer::WireComputer(Discretizer& dis) :
    __cache(),
    __wire(),
    __discretizer(dis){
}

WireComputer::~WireComputer( ) {
}

void WireComputer::clear( ) {
  __wire = GeometryPtr();
  __cache.clear();
}

const GeometryPtr& WireComputer::getWire( ) const {
  return __wire;
}

/* ----------------------------------------------------------------------- */

bool WireComputer::process(Shape * Shape){
    GEOM_ASSERT(Shape);
    return (Shape->geometry->apply(*this));
}


/* ----------------------------------------------------------------------- */
bool WireComputer::process( Material * material ) {
  GEOM_ASSERT(material);
  // nothing to do
  __wire = GeometryPtr();
  return false;
}

bool WireComputer::process( ImageTexture * texture ) {
  GEOM_ASSERT(texture);
  // nothing to do
  __wire = GeometryPtr();
  return false;
}

bool WireComputer::process( Texture2D * texture ) {
  GEOM_ASSERT(texture);
  // nothing to do
  __wire = GeometryPtr();
  return false;
}

bool WireComputer::process( Texture2DTransformation * texture ) {
  GEOM_ASSERT(texture);
  // nothing to do
  __wire = GeometryPtr();
  return false;
}

bool WireComputer::process( MonoSpectral * monoSpectral ) {
  GEOM_ASSERT(monoSpectral);
    // nothing to do
  __wire = GeometryPtr();
  return false;
}

bool WireComputer::process( MultiSpectral * multiSpectral ) {
  GEOM_ASSERT(multiSpectral);
  // nothing to do
  __wire = GeometryPtr();
  return false;
}

/* ----------------------------------------------------------------------- */

bool WireComputer::process( AmapSymbol * amapSymbol ) {
  GEOM_ASSERT(amapSymbol);
  // nothing to do as amapSymbol has a cached representation of type of Mesh.
  GeometryArrayPtr polys(new GeometryArray);
  for(IndexArray::const_iterator _it = amapSymbol->getIndexList()->begin();
  _it != amapSymbol->getIndexList()->end(); _it++){
    Point3ArrayPtr points(new Point3Array);
    for(Index::const_iterator _i = _it->begin(); _i != _it->end(); _i++){
      points->push_back(amapSymbol->getPointList()->getAt(*_i));
    }
    points->push_back(amapSymbol->getPointList()->getAt(*(_it->begin())));
    polys->push_back(GeometryPtr(new Polyline(points)));
  }
  if(polys->empty())__wire = GeometryPtr();
  else if(polys->size() == 1)__wire = polys->getAt(0);
  else __wire = GeometryPtr(new Group(polys));
  return true;
}

bool WireComputer::process( FaceSet * faceSet ) {
  GEOM_ASSERT(faceSet);
  // nothing to do as faceSet is already an ExplicitModel
  GeometryArrayPtr polys(new GeometryArray);
  for(IndexArray::const_iterator _it = faceSet->getIndexList()->begin();
  _it != faceSet->getIndexList()->end(); _it++){
    Point3ArrayPtr points(new Point3Array);
    for(Index::const_iterator _i = _it->begin(); _i != _it->end(); _i++){
      points->push_back(faceSet->getPointList()->getAt(*_i));
    }
    points->push_back(faceSet->getPointList()->getAt(*(_it->begin())));
    polys->push_back(GeometryPtr(new Polyline(points)));
  }
  if(polys->empty())__wire = GeometryPtr();
  else if(polys->size() == 1)__wire = polys->getAt(0);
  else __wire = GeometryPtr(new Group(polys));
  return true;
}

bool WireComputer::process( PointSet * pointSet ) {
  GEOM_ASSERT(pointSet);
  // nothing to do as quadSet is already an ExplicitModel
  __wire = GeometryPtr();
  return true;
}

bool WireComputer::process( Polyline * polyline ) {
  GEOM_ASSERT(polyline);
  // nothing to do as quadSet is already an ExplicitModel
  __wire = GeometryPtr(polyline);
  return true;
}

bool WireComputer::process( QuadSet * quadSet ) {
  GEOM_ASSERT(quadSet);
  // nothing to do as quadSet is already an ExplicitModel
  GeometryArrayPtr polys(new GeometryArray);
  for(Index4Array::const_iterator _it = quadSet->getIndexList()->begin();
  _it != quadSet->getIndexList()->end(); _it++){
    Point3ArrayPtr points(new Point3Array);
    for(Index4::const_iterator _i = _it->begin(); _i != _it->end(); _i++){
      points->push_back(quadSet->getPointList()->getAt(*_i));
    }
    points->push_back(quadSet->getPointList()->getAt(*(_it->begin())));
    polys->push_back(GeometryPtr(new Polyline(points)));
  }
  if(polys->empty())__wire = GeometryPtr();
  else if(polys->size() == 1)__wire = polys->getAt(0);
  else __wire = GeometryPtr(new Group(polys));
  return true;
}

bool WireComputer::process( TriangleSet * triangleSet ) {
  GEOM_ASSERT(triangleSet);
  GeometryArrayPtr polys(new GeometryArray);
  for(Index3Array::const_iterator _it = triangleSet->getIndexList()->begin();
  _it != triangleSet->getIndexList()->end(); _it++){
    Point3ArrayPtr points(new Point3Array);
    for(Index3::const_iterator _i = _it->begin(); _i != _it->end(); _i++){
      points->push_back(triangleSet->getPointList()->getAt(*_i));
    }
    points->push_back(triangleSet->getPointList()->getAt(*(_it->begin())));
    polys->push_back(GeometryPtr(new Polyline(points)));
  }
  if(polys->empty())__wire = GeometryPtr();
  else if(polys->size() == 1)__wire = polys->getAt(0);
  else __wire = GeometryPtr(new Group(polys));
  return true;
}


/* ----------------------------------------------------------------------- */

bool WireComputer::process( AsymmetricHull * asymmetricHull ) {
  GEOM_DISCRETIZE(asymmetricHull);
}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( AxisRotated * axisRotated ) {
  GEOM_DISCRETIZE(axisRotated);
}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( BezierCurve * bezierCurve ) {
  GEOM_DISCRETIZE(bezierCurve);

}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( BezierPatch * bezierPatch ) {
  GEOM_DISCRETIZE(bezierPatch);
}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( Box * box ) {
  GEOM_DISCRETIZE(box);
}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( Cone * cone ) {
  GEOM_DISCRETIZE(cone);
}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( Cylinder * cylinder ) {
  GEOM_DISCRETIZE(cylinder);
}


/* ----------------------------------------------------------------------- */



bool WireComputer::process( ElevationGrid * elevationGrid ) {
  GEOM_DISCRETIZE(elevationGrid);
}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( EulerRotated * eulerRotated ) {
  GEOM_DISCRETIZE(eulerRotated);
}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( ExtrudedHull * extrudedHull ) {
  GEOM_DISCRETIZE(extrudedHull);
}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( Frustum * frustum ) {
  GEOM_DISCRETIZE(frustum);
}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( Extrusion * extrusion ){
    GEOM_DISCRETIZE(extrusion);
}

/* ----------------------------------------------------------------------- */

bool WireComputer::process( Group * group )  {
  GEOM_DISCRETIZE(group);
}


/* ----------------------------------------------------------------------- */

bool WireComputer::process( IFS * ifs )  {
  GEOM_DISCRETIZE(ifs);
}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( NurbsCurve * nurbsCurve ) {
  GEOM_DISCRETIZE(nurbsCurve);
}

/* ----------------------------------------------------------------------- */

bool WireComputer::process( NurbsPatch * nurbsPatch ) {
  GEOM_DISCRETIZE(nurbsPatch);
}

/* ----------------------------------------------------------------------- */


bool WireComputer::process( Oriented * oriented ) {
  GEOM_DISCRETIZE(oriented);
}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( Paraboloid * paraboloid ) {
  GEOM_DISCRETIZE(paraboloid);
}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( Revolution * revolution ) {
  GEOM_DISCRETIZE(revolution);
}


/* ----------------------------------------------------------------------- */

bool WireComputer::process( Swung * swung )
{
  GEOM_DISCRETIZE(swung);
}

/* ----------------------------------------------------------------------- */


bool WireComputer::process( Scaled * scaled ) {
  GEOM_DISCRETIZE(scaled);
}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( ScreenProjected * scp) {
  GEOM_ASSERT(scp);
  return (scp->getGeometry()?scp->getGeometry()->apply(*this):false);
}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( Sphere * sphere ) {
  GEOM_DISCRETIZE(sphere);
}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( Tapered * tapered ) {
  GEOM_DISCRETIZE(tapered);
}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( Translated * translated ) {
  GEOM_DISCRETIZE(translated);
}



/* ----------------------------------------------------------------------- */


bool WireComputer::process( BezierCurve2D * bezierCurve ) {
  GEOM_DISCRETIZE(bezierCurve);
}


/* ----------------------------------------------------------------------- */

bool WireComputer::process( Disc * disc ) {
  GEOM_DISCRETIZE(disc);
}


/* ----------------------------------------------------------------------- */


bool WireComputer::process( NurbsCurve2D * nurbsCurve ) {
  GEOM_DISCRETIZE(nurbsCurve);
}

/* ----------------------------------------------------------------------- */

bool WireComputer::process( PointSet2D * pointSet ){
  GEOM_ASSERT(pointSet);

  __wire = GeometryPtr();
  return true;
}

/* ----------------------------------------------------------------------- */

bool WireComputer::process( Polyline2D * polyline ){
  GEOM_ASSERT( polyline );

  GEOM_WireComputer_CHECK_CACHE( polyline );
  Point3ArrayPtr a (new Point3Array(*(polyline->getPointList()),0));
  __wire = GeometryPtr(new Polyline(a));
  GEOM_WireComputer_UPDATE_CACHE(polyline);
  return true;
}

/* ----------------------------------------------------------------------- */

bool WireComputer::process( Text * text ){
  GEOM_ASSERT(text);
  __wire = GeometryPtr();
  return true;
}

/* ----------------------------------------------------------------------- */

bool WireComputer::process( Font * font ){
  GEOM_ASSERT(font);
  __wire = GeometryPtr();
  return true;
}

/* ----------------------------------------------------------------------- */
