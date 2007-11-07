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




#include <plantgl/math/util_matrix.h>

#include "bboxcomputer.h"
#include "discretizer.h"
#include <plantgl/scenegraph/geometry/boundingbox.h>
#include <plantgl/pgl_scene.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>

#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/scenegraph/container/geometryarray2.h>

#include <plantgl/math/util_math.h>

TOOLS_USING_NAMESPACE
PGL_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */


#define GEOM_BBOXCOMPUTER_CHECK_CACHE(geom) \
  if (geom->isNamed()) { \
    Cache<BoundingBoxPtr>::Iterator _it = __cache.find(geom->getId()); \
    if (! (_it == __cache.end())) { \
       __bbox = _it->second; \
      return true; \
    }; \
  };


#define GEOM_BBOXCOMPUTER_UPDATE_CACHE(geom) \
  if (geom->isNamed()) \
     __cache.insert(geom->getId(),__bbox);


#define GEOM_BBOXCOMPUTER_TRANSFORM_BBOX(matrix) \
  if( ! __bbox->isShared() ) __bbox->transform(matrix); \
  else { \
	BoundingBox bbox(*__bbox); \
	bbox.transform(matrix); \
	__bbox = BoundingBoxPtr(new BoundingBox(bbox)); \
  } \


/* ----------------------------------------------------------------------- */


BBoxComputer::BBoxComputer( Discretizer& discretizer ) :
  Action(),
  __cache(),
  __bbox(),
  __discretizer(discretizer) {
}


void BBoxComputer::clear( ) {
  __bbox = BoundingBoxPtr();
  __cache.clear();
}

BBoxComputer::~BBoxComputer( ) {
}

BoundingBoxPtr
BBoxComputer::getBoundingBox( ) {
  return __bbox;
}

Discretizer&
BBoxComputer::getDiscretizer( ) {
  return __discretizer;
}

/* ----------------------------------------------------------------------- */
bool BBoxComputer::process(Shape * Shape){
    GEOM_ASSERT(Shape);
    GEOM_BBOXCOMPUTER_CHECK_CACHE(Shape);
    return Shape->geometry->apply(*this);
    GEOM_BBOXCOMPUTER_UPDATE_CACHE(Shape);
}

bool BBoxComputer::process(Inline * geomInline){
    GEOM_ASSERT(geomInline);
    GEOM_BBOXCOMPUTER_CHECK_CACHE(geomInline);
	geomInline->getScene()->apply(*this);
	if(!__bbox) return false;

	Matrix3 _matrix = Matrix3::scaling(geomInline->getScale());
    GEOM_BBOXCOMPUTER_TRANSFORM_BBOX(_matrix);

	const Vector3& _ll = __bbox->getLowerLeftCorner();
	const Vector3& _ur = __bbox->getUpperRightCorner();
	const Vector3& _t = geomInline->getTranslation();

	__bbox = BoundingBoxPtr(new BoundingBox(_ll + _t, _ur + _t));


    GEOM_BBOXCOMPUTER_UPDATE_CACHE(geomInline);
    return true;
}

/* ----------------------------------------------------------------------- */
bool BBoxComputer::process( AmapSymbol * amapSymbol ) {
  GEOM_ASSERT(amapSymbol);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(amapSymbol);

  const Point3ArrayPtr& _pointList = amapSymbol->getPointList();
  std::pair<Vector3,Vector3> _bounds = _pointList->getBounds();
  __bbox = BoundingBoxPtr(new BoundingBox(_bounds.first,_bounds.second));
  GEOM_ASSERT(__bbox);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(amapSymbol);

  return true;
}


/* ----------------------------------------------------------------------- */

#define ASYMMAX(a,b) std::max(a,(b < 0.0 ? -b : (real_t)0.0))


bool BBoxComputer::process( AsymmetricHull * asymmetricHull ) {
  GEOM_ASSERT(asymmetricHull);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(asymmetricHull);
  const Vector3& _botPoint = asymmetricHull->getBottom();
  const Vector3& _topPoint = asymmetricHull->getTop();

  real_t _posXR = ASYMMAX(asymmetricHull->getPosXRadius(),asymmetricHull->getNegXRadius());
  _posXR = max(_posXR,max(_botPoint.x(),_topPoint.x()));
  real_t _negXR = ASYMMAX(asymmetricHull->getNegXRadius(),asymmetricHull->getPosXRadius());
  _negXR = min(-_negXR,min(_botPoint.x(),_topPoint.x()));

  real_t _posYR = ASYMMAX(asymmetricHull->getPosYRadius(),asymmetricHull->getNegYRadius());
  _posYR = max(_posYR,max(_botPoint.y(),_topPoint.y()));
  real_t _negYR = ASYMMAX(asymmetricHull->getNegYRadius(),asymmetricHull->getPosYRadius());
  _negYR = min(-_negYR,min(_botPoint.y(),_topPoint.y()));

  const real_t& _posXH = asymmetricHull->getPosXHeight();
  const real_t& _posYH = asymmetricHull->getPosYHeight();
  const real_t& _negXH = asymmetricHull->getNegXHeight();
  const real_t& _negYH = asymmetricHull->getNegYHeight();

  real_t _zMax = std::max(std::max(_posXH,_negXH),std::max(_posYH,_negYH));
  _zMax = std::max(_zMax,std::max(_botPoint.z(),_topPoint.z()));

  real_t _zMin = std::min(_topPoint.z(),
                                                std::min(_botPoint.z(),
                                                                 std::min(_posXH,
                                                                                  std::min(_negXH,
                                                                                                   std::min(_posYH,_negYH)))));

  __bbox = BoundingBoxPtr(new BoundingBox(Vector3(_negXR,_negYR,_zMin),
                                          Vector3(_posXR,_posYR,_zMax)));

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(asymmetricHull);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( AxisRotated * axisRotated ) {
  GEOM_ASSERT(axisRotated);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(axisRotated);

  // We compute the bounding box of the children Geometry
  axisRotated->getGeometry()->apply(*this);
  if(!__bbox) return false;

  // We retrieve the matrix transformation attached to axisRotated
  OrthonormalBasis3DPtr _transformation =  axisRotated->getOrthonormalBasis();
  GEOM_ASSERT(_transformation);
  Matrix3 _matrix = _transformation->getMatrix3();

  // We transform the computed bounding box
  GEOM_BBOXCOMPUTER_TRANSFORM_BBOX(_matrix);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(axisRotated);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( BezierCurve * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(bezierCurve);

  // We discretize bezierCurve
  bezierCurve->apply(__discretizer);
  ExplicitModelPtr _explicitBezier = __discretizer.getDiscretization();
  if (!_explicitBezier) {
      Discretizer d; // because of tesselator;
      bezierCurve->apply(d);
      _explicitBezier = d.getDiscretization();
  }
  GEOM_ASSERT(_explicitBezier);

  // We compute the bounding box of the discretization of bezierCurve
  _explicitBezier->apply(*this);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(bezierCurve);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( BezierPatch * bezierPatch ) {
  GEOM_ASSERT(bezierPatch);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(bezierPatch);

  // We discretize bezierPatch
  bezierPatch->apply(__discretizer);
  ExplicitModelPtr _explicitBezier = __discretizer.getDiscretization();
  GEOM_ASSERT(_explicitBezier);

  // We compute the bounding box of the discretization of bezierPatch
  _explicitBezier->apply(*this);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(bezierPatch);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( Box * box ) {
  GEOM_ASSERT(box);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(box);

  const Vector3& _ur = box->getSize();
  __bbox = BoundingBoxPtr(new BoundingBox(-_ur,_ur));

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(box);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( Cone * cone ) {
  GEOM_ASSERT(cone);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(cone);

  const real_t& _radius = cone->getRadius();
  Vector3 _ll(-_radius,-_radius,0);
  Vector3 _ur(_radius,_radius,cone->getHeight());
  __bbox = BoundingBoxPtr(new BoundingBox(_ll,_ur));

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(cone);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( Cylinder * cylinder ) {
  GEOM_ASSERT(cylinder);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(cylinder);

  const real_t& _radius = cylinder->getRadius();
  Vector3 _ll(-_radius,-_radius,0);
  Vector3 _ur(_radius,_radius,cylinder->getHeight());
  __bbox = BoundingBoxPtr(new BoundingBox(_ll,_ur));
  GEOM_ASSERT(__bbox);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(cylinder);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( ElevationGrid * elevationGrid ) {
  GEOM_ASSERT(elevationGrid);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(elevationGrid);
  const pair<Array2<real_t>::const_iterator,Array2<real_t>::const_iterator> _minandmax =
      (elevationGrid->getHeightList())->getMinAndMax();
  real_t _xMax = elevationGrid->getXSize();
  real_t _yMax = elevationGrid->getYSize();
  Vector3 _ll(0,0,*(_minandmax.first));
  Vector3 _ur(_xMax,_yMax,*(_minandmax.second));
  __bbox = BoundingBoxPtr(new BoundingBox(_ll,_ur));

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(elevationGrid);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( EulerRotated * eulerRotated ) {
  GEOM_ASSERT(eulerRotated);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(eulerRotated);

  // We compute the bounding box of the children Geometry
  eulerRotated->getGeometry()->apply(*this);
  if(!__bbox)return false;

  // We retrieve the matrix transformation attached to axisRotated
  OrthonormalBasis3DPtr _transformation =  eulerRotated->getOrthonormalBasis();
  GEOM_ASSERT(_transformation);
  Matrix3 _matrix = _transformation->getMatrix3();

  // We transform the computed bounding box
  GEOM_BBOXCOMPUTER_TRANSFORM_BBOX(_matrix);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(eulerRotated);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( ExtrudedHull * extrudedHull ) {
  GEOM_ASSERT(extrudedHull);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(extrudedHull);

  // We discretize extrudedHull
  extrudedHull->apply(__discretizer);
  ExplicitModelPtr _explicitHull = __discretizer.getDiscretization();
  GEOM_ASSERT(_explicitHull);

  // We compute the bounding box of the discretization of extrudedHull
  _explicitHull->apply(*this);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(extrudedHull);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( FaceSet * faceSet ) {
  GEOM_ASSERT(faceSet);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(faceSet);

  const Point3ArrayPtr& _pointList = faceSet->getPointList();
  pair<Vector3,Vector3> _bounds = _pointList->getBounds();
  __bbox = BoundingBoxPtr(new BoundingBox(_bounds.first,_bounds.second));
  GEOM_ASSERT(__bbox);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(faceSet);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( Frustum * frustum ) {
  GEOM_ASSERT(frustum);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(frustum);

  const real_t& _radius = frustum->getRadius();
  Vector3 _ll(-_radius,-_radius,0);
  Vector3 _ur(_radius,_radius,frustum->getHeight());
  __bbox = BoundingBoxPtr(new BoundingBox(_ll,_ur));
  GEOM_ASSERT(__bbox);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(frustum);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( Extrusion * extrusion ) {
  GEOM_ASSERT(extrusion);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(extrusion);

  // We first discretize the extrusion
  if(extrusion->apply(__discretizer)){
      ExplicitModelPtr _explicitCyl = __discretizer.getDiscretization();

      // We compute the bounding box of the discretization of extrusion
      _explicitCyl->apply(*this);

      GEOM_BBOXCOMPUTER_UPDATE_CACHE(extrusion);

      return true;
  }
  else {
      __bbox = BoundingBoxPtr();
      return false;
  }
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( Group * group ) {
  GEOM_ASSERT(group);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(group);

  const GeometryArrayPtr& _group = group->getGeometryList();
  uint32_t _size = _group->getSize();
  uint32_t _first = 0;
  do {
	_group->getAt(_first)->apply(*this);
    ++_first;
  } while(__bbox.isNull() && _first <_size);
  if(!__bbox)return false;

  BoundingBoxPtr _bbox (new BoundingBox(*__bbox));
  for (uint32_t _i = _first; _i < _size; ++_i) {
    _group->getAt(_i)->apply(*this);
    if(__bbox) _bbox->extend(__bbox);
  };
  __bbox = _bbox;
  GEOM_ASSERT(__bbox);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(group);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( IFS * ifs ) {
//todo OK
  GEOM_ASSERT(ifs);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(ifs);

  ITPtr transfos;
  transfos.cast( ifs->getTransformation() );
  GEOM_ASSERT(transfos);
  const Matrix4ArrayPtr& matrixList= transfos->getAllTransfo();
  GEOM_ASSERT(matrixList);

  // We compute the bounding box of the children Geometry
  ifs->getGeometry()->apply(*this);
  if(__bbox){
	
	Vector3 _ll = __bbox->getLowerLeftCorner();
	Vector3 _ur = __bbox->getUpperRightCorner();
	Matrix4Array::const_iterator matrix= matrixList->getBegin();
	BoundingBox UnionBox( _ll, _ur);
	UnionBox.transform(*matrix);
	matrix++;
	
	while( matrix != matrixList->getEnd() )
    {
	  BoundingBox bbox( _ll, _ur);
	  bbox.transform(*matrix);
	  UnionBox.extend(bbox);
	  matrix++;
    }
	
	_ll= UnionBox.getLowerLeftCorner();
	_ur= UnionBox.getUpperRightCorner();
	
	__bbox = BoundingBoxPtr(new BoundingBox(_ll,_ur));
	
	GEOM_ASSERT(__bbox);
	
	GEOM_BBOXCOMPUTER_UPDATE_CACHE(ifs);
	
	return true;
  }
  return false;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( NurbsCurve * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(nurbsCurve);

  // We first discretize the nurbs
  nurbsCurve->apply(__discretizer);
  ExplicitModelPtr _explicitNurbs = __discretizer.getDiscretization();
  if (!_explicitNurbs) {
      Discretizer d; // because of tesselator;
      nurbsCurve->apply(d);
      _explicitNurbs = d.getDiscretization();
  }

   // We compute the bounding box of the discretization of nurbsCurve
  _explicitNurbs->apply(*this);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(nurbsCurve);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( NurbsPatch * nurbsPatch ) {
  GEOM_ASSERT(nurbsPatch);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(nurbsPatch);

  // We discretize nurbsPatch
  nurbsPatch->apply(__discretizer);
  ExplicitModelPtr _explicitNurbs = __discretizer.getDiscretization();
  GEOM_ASSERT(_explicitNurbs);

  // We compute the bounding box of the discretization of nurbsPatch
  _explicitNurbs->apply(*this);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(nurbsPatch);

  return true;
}

/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( Oriented * oriented ) {
  GEOM_ASSERT(oriented);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(oriented);

  // We compute the bounding box of the children Geometry
  oriented->getGeometry()->apply(*this);
  if(!__bbox) return false;
	
  // We retrieve the matrix transformation attached to axisRotated
  Matrix4TransformationPtr _transformation;
  _transformation.cast(oriented->getTransformation());
  GEOM_ASSERT(_transformation);
  Matrix4 _matrix = _transformation->getMatrix();

  // We transform the computed bounding box
  GEOM_BBOXCOMPUTER_TRANSFORM_BBOX(_matrix);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(oriented);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( Paraboloid * paraboloid ) {
  GEOM_ASSERT(paraboloid);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(paraboloid);

  const real_t& _radius = paraboloid->getRadius();
  Vector3 _ll(-_radius,-_radius,0);
  Vector3 _ur(_radius,_radius,paraboloid->getHeight());
  __bbox = BoundingBoxPtr(new BoundingBox(_ll,_ur));
  GEOM_ASSERT(__bbox);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(paraboloid);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( PointSet * pointSet ) {
  GEOM_ASSERT(pointSet);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(pointSet);

  const Point3ArrayPtr& _pointList = pointSet->getPointList();
  pair<Vector3,Vector3> _bounds = _pointList->getBounds();
  __bbox = BoundingBoxPtr(new BoundingBox(_bounds.first,_bounds.second));
  GEOM_ASSERT(__bbox);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(pointSet);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( Polyline * polyline ) {
  GEOM_ASSERT(polyline);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(polyline);

  const Point3ArrayPtr& _pointList = polyline->getPointList();
  pair<Vector3,Vector3> _bounds = _pointList->getBounds();
  __bbox = BoundingBoxPtr(new BoundingBox(_bounds.first,_bounds.second));
  GEOM_ASSERT(__bbox);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(polyline);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( Revolution * revolution ) {
  GEOM_ASSERT(revolution);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(revolution);

  if(!revolution->getProfile()->apply(__discretizer))return false;

  const Point3ArrayPtr& _pointList = __discretizer.getDiscretization()->getPointList();

  uint32_t _size = _pointList->getSize();
  real_t _yMin = _pointList->getAt(0).y();
  real_t _yMax = _pointList->getAt(0).y();
  real_t _xMax = _pointList->getAt(0).x();
  for (uint32_t _i = 1; _i < _size; _i++) {
    const real_t& _x = _pointList->getAt(_i).x();
    const real_t& _y = _pointList->getAt(_i).y();
    if (_x > _xMax)
      _xMax = _x;
    if (_y < _yMin)
      _yMin = _y;
    else
      if (_y > _yMax)
        _yMax = _y;
  };
  Vector3 _ll(-_xMax,-_xMax,-_yMin);
  Vector3 _ur(_xMax,_xMax,_yMax);
  __bbox = BoundingBoxPtr(new BoundingBox(_ll,_ur));
  GEOM_ASSERT(__bbox);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(revolution);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( Swung * swung )
{
  GEOM_ASSERT(swung);
  GEOM_BBOXCOMPUTER_CHECK_CACHE(swung);

// cpl
  const Curve2DArrayPtr& p= swung->getProfileList();

  p->getAt(0)->apply(*this);
  GEOM_ASSERT(__bbox);

  Vector3 _ll= __bbox->getLowerLeftCorner();
  Vector3 _ur= __bbox->getUpperRightCorner();

  uint32_t i= 0;
  uint32_t size = p->getSize();
  for( i= 1; i < size; i++ )
    {
    p->getAt(i)->apply(*this);
    GEOM_ASSERT(__bbox);
    _ll = Min(_ll,__bbox->getLowerLeftCorner());
    _ur = Max(_ur,__bbox->getUpperRightCorner());
    }

  real_t zmin= _ll.y(), zmax= _ur.y();
  real_t xmax= _ur.x();

  _ll.x()= -xmax; _ll.y()= -xmax; _ll.z()= zmin;
  _ur.x()=  xmax; _ur.y()=  xmax; _ur.z()= zmax;

  __bbox = BoundingBoxPtr(new BoundingBox(_ll,_ur));

  GEOM_ASSERT(__bbox);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(swung);

  return true;
}

/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( QuadSet * quadSet ) {
  GEOM_ASSERT(quadSet);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(quadSet);

  const Point3ArrayPtr& _pointList = quadSet->getPointList();
  pair<Vector3,Vector3> _bounds = _pointList->getBounds();
  __bbox = BoundingBoxPtr(new BoundingBox(_bounds.first,_bounds.second));
  GEOM_ASSERT(__bbox);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(quadSet);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( Scaled * scaled ) {
  GEOM_ASSERT(scaled);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(scaled);

  // We compute the bounding box of the children Geometry
  scaled->getGeometry()->apply(*this);
  if(!__bbox) return false;
	
  // We retrieve the matrix transformation attached to axisRotated
  Matrix4TransformationPtr _transformation;
  _transformation.cast(scaled->getTransformation());
  GEOM_ASSERT(_transformation);
  Matrix4 _matrix = _transformation->getMatrix();

  // We transform the computed bounding box
  GEOM_BBOXCOMPUTER_TRANSFORM_BBOX(_matrix);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(scaled);

  return true;
}

/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( Sphere * sphere ) {
  GEOM_ASSERT(sphere);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(sphere);

  real_t _radius = sphere->getRadius();
  Vector3 _ll(-_radius,-_radius,-_radius);
  Vector3 _ur(_radius,_radius,_radius);
  __bbox = BoundingBoxPtr(new BoundingBox(_ll,_ur));
  GEOM_ASSERT(__bbox);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(sphere);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( Tapered * tapered ) {
  GEOM_ASSERT(tapered);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(tapered);

  // We compute the Taper transformation associated to tapered.
  Transformation3DPtr _taper(tapered->getTransformation());

  // We discretize the children Primitive of tapered if needed.
  PrimitivePtr _primitive = tapered->getPrimitive();
  _primitive->apply(__discretizer);
  ExplicitModelPtr _explicit = __discretizer.getDiscretization();

  // We transform the points of the discretization
  Point3ArrayPtr _tPointList = _taper->transform(_explicit->getPointList());

  pair<Vector3,Vector3> _bounds = _tPointList->getBounds();
  __bbox = BoundingBoxPtr(new BoundingBox(_bounds.first,_bounds.second));
  GEOM_ASSERT(__bbox);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(tapered);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( Translated * translated ) {
  GEOM_ASSERT(translated);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(translated);

  translated->getGeometry()->apply(*this);
  if(!__bbox) return false;
	
  const Vector3& _ll = __bbox->getLowerLeftCorner();
  const Vector3& _ur = __bbox->getUpperRightCorner();
  const Vector3& _t = translated->getTranslation();

  __bbox = BoundingBoxPtr(new BoundingBox(_ll + _t, _ur + _t));

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(translated);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( TriangleSet * triangleSet ) {
  GEOM_ASSERT(triangleSet);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(triangleSet);

  const Point3ArrayPtr& _pointList = triangleSet->getPointList();
  pair<Vector3,Vector3> _bounds = _pointList->getBounds();
  __bbox = BoundingBoxPtr(new BoundingBox(_bounds.first,_bounds.second));
  GEOM_ASSERT(__bbox);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(triangleSet);

  return true;
}


/* ----------------------------------------------------------------------- */



bool BBoxComputer::process( BezierCurve2D * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(bezierCurve);

  // We discretize bezierCurve
  bezierCurve->apply(__discretizer);
  ExplicitModelPtr _explicitBezier = __discretizer.getDiscretization();
  if (!_explicitBezier) {
      Discretizer d; // because of tesselator;
      bezierCurve->apply(d);
      _explicitBezier = d.getDiscretization();
  }
  GEOM_ASSERT(_explicitBezier);

  // We compute the bounding box of the discretization of bezierCurve
  _explicitBezier->apply(*this);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(bezierCurve);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( Disc * disc ) {
    GEOM_ASSERT(disc);

    GEOM_BBOXCOMPUTER_CHECK_CACHE(disc);

    const real_t& _radius = disc->getRadius();
    Vector3 _ll(-_radius,-_radius,0);
    Vector3 _ur(_radius,_radius,0);
    __bbox = BoundingBoxPtr(new BoundingBox(_ll,_ur));

    GEOM_BBOXCOMPUTER_UPDATE_CACHE(disc);

    return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( NurbsCurve2D * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(nurbsCurve);

  // We first discretize the nurbs
  nurbsCurve->apply(__discretizer);
  ExplicitModelPtr _explicitNurbs = __discretizer.getDiscretization();
  if (!_explicitNurbs) {
      Discretizer d; // because of tesselator;
      nurbsCurve->apply(d);
      _explicitNurbs = d.getDiscretization();
  }

   // We compute the bounding box of the discretization of nurbsCurve
  _explicitNurbs->apply(*this);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(nurbsCurve);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( PointSet2D * pointSet ) {
  GEOM_ASSERT(pointSet);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(pointSet);

  const Point2ArrayPtr& _pointList = pointSet->getPointList();
  pair<Vector2,Vector2> _bounds = _pointList->getBounds();
  __bbox = BoundingBoxPtr(new BoundingBox(Vector3(_bounds.first,0),Vector3(_bounds.second,0)));
  GEOM_ASSERT(__bbox);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(pointSet);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BBoxComputer::process( Polyline2D * polyline ) {
  GEOM_ASSERT(polyline);

  GEOM_BBOXCOMPUTER_CHECK_CACHE(polyline);

  const Point2ArrayPtr& _pointList = polyline->getPointList();
  pair<Vector2,Vector2> _bounds = _pointList->getBounds();
  __bbox = BoundingBoxPtr(new BoundingBox(Vector3(_bounds.first,0),Vector3(_bounds.second,0)));
  GEOM_ASSERT(__bbox);

  GEOM_BBOXCOMPUTER_UPDATE_CACHE(polyline);

  return true;
}


/* ----------------------------------------------------------------------- */

bool BBoxComputer::process( Material * material ) {
  GEOM_ASSERT(material);
  // nothing to do as it cannot compute the bounding box of such an object
  __bbox = BoundingBoxPtr();
  return false;
}

bool BBoxComputer::process( ImageTexture * texture ) {
  GEOM_ASSERT(texture);
  // nothing to do as it cannot compute the bounding box of such an object
  __bbox = BoundingBoxPtr();
  return false;
}

bool BBoxComputer::process( MonoSpectral * monoSpectral ) {
  GEOM_ASSERT(monoSpectral);
  // nothing to do as it cannot compute the bounding box of such an object
  __bbox = BoundingBoxPtr();
  return false;
}

bool BBoxComputer::process( MultiSpectral * multiSpectral ) {
  GEOM_ASSERT(multiSpectral);
  // nothing to do as it cannot compute the bounding box of such an object
  __bbox = BoundingBoxPtr();
  return false;
}

/* ----------------------------------------------------------------------- */

bool BBoxComputer::process( Text * text ) {
  GEOM_ASSERT(text);
  // nothing to do as it cannot compute the bounding box of such an object
  __bbox = BoundingBoxPtr();
  return false;
}

bool BBoxComputer::process( Font * font ) {
  GEOM_ASSERT(font);
  // nothing to do as it cannot compute the bounding box of such an object
  __bbox = BoundingBoxPtr();
  return false;
}

/* ----------------------------------------------------------------------- */

bool BBoxComputer::process(const ScenePtr& scene){

    // Computes the global bounding box
    BoundingBoxPtr _bbox = BoundingBoxPtr();
    if (scene.isValid() && (! scene->isEmpty())) {
        Scene::const_iterator _i = scene->getBegin();
        while ((_i != scene->getEnd())&&(!((*(_i++))->applyGeometryOnly(*this))));
        if(__bbox){
            _bbox = BoundingBoxPtr(new BoundingBox(*__bbox));
        }
        else return false;
        while (_i != scene->getEnd()) {
            if((*(_i++))->applyGeometryOnly(*this)){
                _bbox->extend(__bbox);
            }
        };
        __bbox = _bbox;
        return true;
    }
    else return false;
}

bool BBoxComputer::process(const Scene& scene){

    // Computes the global bounding box
    BoundingBoxPtr _bbox = BoundingBoxPtr();
    if (! scene.isEmpty()) {
        Scene::const_iterator _i = scene.getBegin();
        while ((_i != scene.getEnd())&&(!((*(_i++))->applyGeometryOnly(*this))));
        if(__bbox){
            _bbox = BoundingBoxPtr(new BoundingBox(*__bbox));
        }
        else return false;
        while (_i != scene.getEnd()) {
            if((*(_i++))->applyGeometryOnly(*this)){
                _bbox->extend(__bbox);
            }
        };
        __bbox = _bbox;
        return true;
    }
    else return false;
}

/* ----------------------------------------------------------------------- */
