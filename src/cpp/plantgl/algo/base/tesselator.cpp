/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
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




#include "tesselator.h"

#include <plantgl/scenegraph/geometry/amapsymbol.h>
#include <plantgl/scenegraph/geometry/faceset.h>
#include <plantgl/scenegraph/geometry/quadset.h>
#include <plantgl/scenegraph/geometry/triangleset.h>
#include <plantgl/scenegraph/geometry/bezierpatch.h>
#include <plantgl/scenegraph/geometry/box.h>
#include <plantgl/scenegraph/geometry/cylinder.h>
#include <plantgl/scenegraph/geometry/extrusion.h>
#include <plantgl/scenegraph/geometry/frustum.h>
#include <plantgl/scenegraph/geometry/nurbspatch.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/geometry/profile.h>

#include <plantgl/scenegraph/transformation/orthotransformed.h>
#include <plantgl/pgl_container.h>

#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */


#define GEOM_TESSELATOR_CHECK_CACHE(geom) \
if(!geom->unique()){ \
  Cache<ExplicitModelPtr>::Iterator _it = __cache.find(geom->getId()); \
  if (! (_it == __cache.end())) { \
    __discretization = _it->second; \
    return true; \
  }} else __discretization= ExplicitModelPtr(); 


#define GEOM_TESSELATOR_UPDATE_CACHE(geom) \
if(!geom->unique()){ \
  if(geom->isNamed())__discretization->setName(geom->getName()); \
  __cache.insert(geom->getId(),__discretization); \
}


/* ----------------------------------------------------------------------- */


Tesselator::Tesselator( ) :
  Discretizer() {
}

Tesselator::~Tesselator( )
{
}

TriangleSetPtr Tesselator::getTriangulation( ) const {
  return TriangleSetPtr(dynamic_pointer_cast<TriangleSet>(__discretization));
}

/* ----------------------------------------------------------------------- */

bool Tesselator::process( AmapSymbol * amapSymbol ) {
  GEOM_ASSERT(amapSymbol);
  GEOM_TESSELATOR_CHECK_CACHE(amapSymbol);
  __discretization = ExplicitModelPtr(new TriangleSet(amapSymbol->getPointList(),
				     amapSymbol->getIndexList()->triangulate(),
				     amapSymbol->getNormalList(),
					 (amapSymbol->getNormalIndexList()?amapSymbol->getNormalIndexList()->triangulate():Index3ArrayPtr()),
				     amapSymbol->getColorList(),
					 (amapSymbol->getColorIndexList()?amapSymbol->getColorIndexList()->triangulate():Index3ArrayPtr()),
				     amapSymbol->getTexCoordList(),
					 (amapSymbol->getTexCoordIndexList()?amapSymbol->getTexCoordIndexList()->triangulate():Index3ArrayPtr()),
				     amapSymbol->getNormalPerVertex(),
				     amapSymbol->getColorPerVertex(),
				     amapSymbol->getCCW(),
				     amapSymbol->getSolid(),
				     amapSymbol->getSkeleton()));
  GEOM_TESSELATOR_UPDATE_CACHE(amapSymbol);
  return true;
}

bool Tesselator::process( BezierCurve * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);
  // nothing to do as it cannot be transformed into a TriangleSet.
  __discretization = ExplicitModelPtr();
  return false;
}

bool Tesselator::process( Box * box ) {
  GEOM_ASSERT(box);

  GEOM_TESSELATOR_CHECK_CACHE(box);

  const Vector3& _size = box->getSize();

  Point3ArrayPtr _pointList(new Point3Array(8));
  Index3ArrayPtr _indexList(new Index3Array(12));
  Point3ArrayPtr _normalList(new Point3Array(12));

  _pointList->setAt(0,Vector3(_size.x(),-_size.y(),-_size.z()));
  _pointList->setAt(1,Vector3(-_size.x(),-_size.y(),-_size.z()));
  _pointList->setAt(2,Vector3(-_size.x(),_size.y(),-_size.z()));
  _pointList->setAt(3,Vector3(_size.x(),_size.y(),-_size.z()));
  _pointList->setAt(4,Vector3(_size.x(),-_size.y(),_size.z()));
  _pointList->setAt(5,Vector3(-_size.x(),-_size.y(),_size.z()));
  _pointList->setAt(6,Vector3(-_size.x(),_size.y(),_size.z()));
  _pointList->setAt(7,Vector3(_size.x(),_size.y(),_size.z()));

  _indexList->setAt(0,Index3(0,1,2));
  _indexList->setAt(1,Index3(0,2,3));
  _indexList->setAt(2,Index3(0,3,7));
  _indexList->setAt(3,Index3(0,7,4));
  _indexList->setAt(4,Index3(1,0,4));
  _indexList->setAt(5,Index3(1,4,5));
  _indexList->setAt(6,Index3(2,1,5));
  _indexList->setAt(7,Index3(2,5,6));
  _indexList->setAt(8,Index3(3,2,6));
  _indexList->setAt(9,Index3(3,6,7));
  _indexList->setAt(10,Index3(4,7,6));
  _indexList->setAt(11,Index3(4,6,5));


  PolylinePtr _skeleton(new Polyline(Vector3(0,0,-_size.z()),
                                     Vector3(0,0,_size.z())));

  __discretization = ExplicitModelPtr(new TriangleSet(_pointList,
                                                      _indexList,
                                                      false, // normal per vertex
                                                      true, // CCW
                                                      true, // solid
                                                      _skeleton));

  GEOM_TESSELATOR_UPDATE_CACHE(box);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Tesselator::process( BezierPatch * bezierPatch ) {
  GEOM_ASSERT(bezierPatch);

  GEOM_TESSELATOR_CHECK_CACHE(bezierPatch);

  const uint_t _uStride = bezierPatch->getUStride();
  const uint_t _vStride = bezierPatch->getVStride();

  const real_t _uStride1 = bezierPatch->getUStride() - 1;
  const real_t _vStride1 = bezierPatch->getVStride() - 1;

  Point3ArrayPtr _pointList(new Point3Array(_uStride * _vStride));
  Index3ArrayPtr _indexList(new Index3Array(2 * (_uStride - 1) * (_vStride - 1)));

  uint_t _cur = 0;

  uint_t _pointCount = 0;
  uint_t _indexCount = 0;

  for ( real_t _u = 0 ; _u < _uStride1 ; _u ++){

    for (real_t _v = 0; _v < _vStride1; _v ++) {

      _pointList->setAt(_pointCount++,
			bezierPatch->getPointAt((_u/_uStride1),
						(_v/_vStride1)));
      _indexList->setAt(_indexCount++,Index3(_cur, 
					     _cur + 1,
					     _cur + _vStride + 1));

      _indexList->setAt(_indexCount++,Index3(_cur, 
					     _cur + _vStride + 1,
					     _cur + _vStride));

      _cur++;

    };

    _pointList->setAt(_pointCount++,
                      bezierPatch->getPointAt((_u/_uStride1),1.0));

    _cur++;

  };

  for (real_t _v = 0; _v < _vStride1; _v ++)
    _pointList->setAt(_pointCount++,
                      bezierPatch->getPointAt(1.0,(_v/_vStride1)));

  _pointList->setAt(_pointCount,bezierPatch->getPointAt(1.0,1.0));

  PolylinePtr _skeleton(new Polyline(Vector3(0,0,0),
                                     Vector3(0,0,0)));

  TriangleSet * t = new TriangleSet(_pointList, _indexList, true, bezierPatch->getCCW(), // CCW
                                    false, _skeleton);
  if(__computeTexCoord) t->getTexCoordList() = gridTexCoord(_pointList,_uStride,_vStride);

  __discretization = ExplicitModelPtr(t);
  GEOM_TESSELATOR_UPDATE_CACHE(bezierPatch);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Tesselator::process( Cylinder * cylinder ) {
  GEOM_ASSERT(cylinder);

  GEOM_TESSELATOR_CHECK_CACHE(cylinder);

  real_t _radius = cylinder->getRadius();
  real_t _height = cylinder->getHeight();
  bool _solid = cylinder->getSolid();
  uint_t _slices = cylinder->getSlices();

  uint_t _offset = (_solid ? 2 : 0);

  Point3ArrayPtr _pointList(new Point3Array((_slices * 2) + _offset));
  Index3ArrayPtr _indexList(new Index3Array(_slices * (2 + _offset)));

  Point2ArrayPtr _texCoordList;
  Index3ArrayPtr _texIndexList;
  if(__computeTexCoord){
	_texCoordList = Point2ArrayPtr(new Point2Array(((_slices+1) * (2 + (_solid ? 1 : 0))) + (_solid ? 1 : 0)));
	_texIndexList= Index3ArrayPtr(new Index3Array(_slices * (2 + _offset) ));
  }

  uint_t _cur = 0;
  uint_t _next = 2;

  uint_t _curtex = 0;
  uint_t _nexttexstep = 2+ (_solid ? 1 : 0);
  uint_t _nexttex = _nexttexstep;

  uint_t _base = 2 * _slices;
  uint_t _top = _base + 1;
  uint_t _basetex = (2+ (_solid ? 1 : 0)) * (_slices+1);

  uint_t _pointsCount = 0;
  uint_t _texCoordCount = 0;
  uint_t _facesCount = 0;
  uint_t _texFacesCount = 0;
  real_t _angleStep = GEOM_TWO_PI / _slices;

  if (_solid){
    _pointList->setAt(_top,Vector3(0,0,_height));
	if(__computeTexCoord)
		_texCoordList->setAt(_basetex,Vector2(0.5,0.5));
  }

  for (uint_t _i = 0; _i < _slices; _i++) {
    real_t cosa = cos(_i * _angleStep);
	real_t sina = sin(_i * _angleStep);
    real_t _x = cosa * _radius;
    real_t _y = sina * _radius;

    _pointList->setAt(_pointsCount++,Vector3(_x,_y,0));
    _pointList->setAt(_pointsCount++,Vector3(_x,_y,_height));

    _indexList->setAt(_facesCount++,Index3(_cur,_next,_cur + 1));
    _indexList->setAt(_facesCount++,Index3(_cur + 1,_next,_next + 1));

    if (_solid) {
      _indexList->setAt(_facesCount++,Index3(_cur + 1,_next + 1,_top));
      _indexList->setAt(_facesCount++,Index3(_cur,_base,_next));
    }

	if(__computeTexCoord){
		real_t u = real_t(_i)/_slices;
		_texCoordList->setAt(_texCoordCount++,Vector2(u,0));
		_texCoordList->setAt(_texCoordCount++,Vector2(u,1));

		_texIndexList->setAt(_texFacesCount++,Index3(_curtex,_nexttex ,_curtex + 1));
		_texIndexList->setAt(_texFacesCount++,Index3(_curtex+1,_nexttex ,_nexttex+1));

		if (_solid){
			_texCoordList->setAt(_texCoordCount++,Vector2(0.5 * cosa + 0.5 , 0.5 * sina + 0.5));
			_texIndexList->setAt(_texFacesCount++,Index3(_curtex+2,_nexttex+2,_basetex));
			_texIndexList->setAt(_texFacesCount++,Index3(_curtex+2,_basetex,_nexttex+2));
		}
	}


    _cur = _next;
    _next = (_next + 2 ) % (2 * _slices);
    _curtex = _nexttex;
	_nexttex += _nexttexstep ; 
  }

  if(__computeTexCoord){
		_texCoordList->setAt(_texCoordCount++,Vector2(1,0));
		_texCoordList->setAt(_texCoordCount++,Vector2(1,1));
		if (_solid)
			_texCoordList->setAt(_texCoordCount++,Vector2(1.0, 0.5));
  }


  PolylinePtr _skeleton(new Polyline(Vector3(0,0,0),
                                     Vector3(0,0,_height)));

  TriangleSet * t = new TriangleSet(_pointList,_indexList,
								    true, true, // CCW
									_solid, _skeleton);

  t->getTexCoordList() = _texCoordList;
  t->getTexCoordIndexList() = _texIndexList;
  __discretization = ExplicitModelPtr(t);

  GEOM_TESSELATOR_UPDATE_CACHE(cylinder);
  return true;
}


/* ----------------------------------------------------------------------- */


bool Tesselator::process( Extrusion * extrusion ){
    GEOM_ASSERT(extrusion);
    GEOM_TESSELATOR_CHECK_CACHE(extrusion);
    /// Hack for bug with tesselation of curve.
	Discretizer d;
	d.computeTexCoord(texCoordComputed());
	d.process(extrusion);
	if(d.getDiscretization()){
	  d.getDiscretization()->apply(*this);
	  __cache.remove(d.getDiscretization()->getId());
	}
    GEOM_TESSELATOR_UPDATE_CACHE(extrusion);
    return true;
}

/* ----------------------------------------------------------------------- */

bool Tesselator::process( FaceSet * faceSet ) {
  GEOM_ASSERT(faceSet);
  GEOM_TESSELATOR_CHECK_CACHE(faceSet);

  TriangleSet * tr = new TriangleSet(faceSet->getPointList(),
				     faceSet->getIndexList()->triangulate(),
				     faceSet->getNormalList(),
					 (faceSet->getNormalIndexList()?faceSet->getNormalIndexList()->triangulate():Index3ArrayPtr()),
				     faceSet->getColorList(),
					 (faceSet->getColorIndexList()?faceSet->getColorIndexList()->triangulate():Index3ArrayPtr()),
				     faceSet->getTexCoordList(),
					 (faceSet->getTexCoordIndexList()?faceSet->getTexCoordIndexList()->triangulate():Index3ArrayPtr()),
				     faceSet->getNormalPerVertex(),
				     faceSet->getColorPerVertex(),
				     faceSet->getCCW(),
				     faceSet->getSolid(),
				     faceSet->getSkeleton());

  if (!faceSet->getNormalPerVertex() && faceSet->getNormalList()){
	  Point3ArrayPtr _nml( new Point3Array(tr->getIndexList()->size()));
	  Point3Array::iterator _it = _nml->begin();
	  Point3Array::const_iterator _it2 = faceSet->getNormalList()->begin();
	  
	  for (IndexArray::iterator _itInd = faceSet->getIndexList()->begin();
		   _itInd != faceSet->getIndexList()->end(); ++_itInd)
	  {
		  if(_itInd->size() >=3){
			  for (uint_t i = 0 ; i < _itInd->size() - 2; ++i)
			  { *_it = *_it2; ++_it; }
		  }
		  ++_it2;
	  }
	  tr->getNormalList() = _nml;
  }

  if (!faceSet->getColorPerVertex() && faceSet->getColorList()){
	  Color4ArrayPtr _cl( new Color4Array(tr->getIndexList()->size()));
	  Color4Array::iterator _it = _cl->begin();
	  Color4Array::const_iterator _it2 = faceSet->getColorList()->begin();

	  for (IndexArray::iterator _itInd = faceSet->getIndexList()->begin();
		   _itInd != faceSet->getIndexList()->end(); ++_itInd)
	  {
		  if(_itInd->size() >=3){
			for (uint_t i = 0 ; i < _itInd->size() - 2; ++i)
				{ *_it = *_it2; ++_it; }
		  }
		  ++_it2;
	  }

	  tr->getColorList() = _cl;
  }

  __discretization = ExplicitModelPtr(tr);


  GEOM_TESSELATOR_UPDATE_CACHE(faceSet);
  return true;
}

/* ----------------------------------------------------------------------- */


bool Tesselator::process( Frustum * frustum ) {
  GEOM_ASSERT(frustum);

  GEOM_TESSELATOR_CHECK_CACHE(frustum);

  real_t _radius = frustum->getRadius();
  real_t _height = frustum->getHeight();
  real_t _taper = frustum->getTaper();
  bool _solid = frustum->getSolid();
  uint_t _slices = frustum->getSlices();

  uint_t _offset = (_solid ? 2 : 0);

  Point3ArrayPtr _pointList(new Point3Array((_slices * 2) + _offset));
  Index3ArrayPtr _indexList(new Index3Array(_slices * (2 + _offset)));

  Point2ArrayPtr _texCoordList;
  Index3ArrayPtr _texIndexList;
  if(__computeTexCoord){
	_texCoordList = Point2ArrayPtr(new Point2Array(((_slices+1) * (2 + (_solid ? 1 : 0))) + (_solid ? 1 : 0)));
	_texIndexList = Index3ArrayPtr(new Index3Array(_slices * (2 + _offset)));
  }

  uint_t _cur = 0;
  uint_t _next = 2;

  uint_t _curtex = 0;
  uint_t _nexttexstep = 2+ (_solid ? 1 : 0);
  uint_t _nexttex = _nexttexstep;

  uint_t _base = 2 * _slices;
  uint_t _top = _base + 1;
  uint_t _basetex = (2+ (_solid ? 1 : 0)) * (_slices+1);

  uint_t _pointsCount = 0;
  uint_t _texCoordCount = 0;
  uint_t _facesCount = 0;
  uint_t _texFacesCount = 0;

  real_t _angleStep = GEOM_TWO_PI / _slices;

  if (_solid){
    _pointList->setAt(_top,Vector3(0,0,_height));
	if(__computeTexCoord)
		_texCoordList->setAt(_basetex,Vector2(0.5,0.5));
  }

  for (uint_t _i = 0; _i < _slices; _i++) {
    real_t cosa = cos(_i * _angleStep);
	real_t sina = sin(_i * _angleStep);
    real_t _x = cosa * _radius;
    real_t _y = sina * _radius;

    _pointList->setAt(_pointsCount++,Vector3(_x,_y,0));
    _pointList->setAt(_pointsCount++,Vector3(_x * _taper,
                                             _y * _taper,
                                             _height));

    _indexList->setAt(_facesCount++,Index3(_cur,_next,_cur + 1));
    _indexList->setAt(_facesCount++,Index3(_cur + 1,_next,_next + 1));

    if (_solid) {
      _indexList->setAt(_facesCount++,Index3(_cur + 1,_next + 1,_top));
      _indexList->setAt(_facesCount++,Index3(_cur,_base,_next));
    };

	if(__computeTexCoord){
		real_t u = real_t(_i)/_slices;
		_texCoordList->setAt(_texCoordCount++,Vector2(u,0));
		_texCoordList->setAt(_texCoordCount++,Vector2(u,1));

		_texIndexList->setAt(_texFacesCount++,Index3(_curtex,_nexttex ,_curtex + 1));
		_texIndexList->setAt(_texFacesCount++,Index3(_curtex+1,_nexttex ,_nexttex+1));

		if (_solid){
			_texCoordList->setAt(_texCoordCount++,Vector2(0.5 * cosa + 0.5 , 0.5 * sina + 0.5));
			_texIndexList->setAt(_texFacesCount++,Index3(_curtex+2,_nexttex+2,_basetex));
			_texIndexList->setAt(_texFacesCount++,Index3(_curtex+2,_basetex,_nexttex+2));
		}
	}

    _cur = _next;
    _next = (_next + 2 ) % (2 * _slices);
    _curtex = _nexttex;
	_nexttex += _nexttexstep ; 
  }

  if(__computeTexCoord){
		_texCoordList->setAt(_texCoordCount++,Vector2(1,0));
		_texCoordList->setAt(_texCoordCount++,Vector2(1,1));
		if (_solid)
			_texCoordList->setAt(_texCoordCount++,Vector2(1.0, 0.5));
  }

  PolylinePtr _skeleton(new Polyline(Vector3(0,0,0),
                                     Vector3(0,0,_height)));

  TriangleSet * t = new TriangleSet(_pointList, _indexList,
									true, true, // CCW 
									_solid, _skeleton);

  t->getTexCoordList() = _texCoordList;
  t->getTexCoordIndexList() = _texIndexList;
  __discretization = ExplicitModelPtr(t);

  GEOM_TESSELATOR_UPDATE_CACHE(frustum);
  return true;
}


/* ----------------------------------------------------------------------- */

bool Tesselator::process( QuadSet * quadSet ) {
  GEOM_ASSERT(quadSet);
  GEOM_TESSELATOR_CHECK_CACHE(quadSet);


  TriangleSet * tr = new TriangleSet(quadSet->getPointList(),
				     quadSet->getIndexList()->triangulate(),
				     quadSet->getNormalList(),
					 (quadSet->getNormalIndexList()?quadSet->getNormalIndexList()->triangulate():Index3ArrayPtr()),
				     quadSet->getColorList(),
					 (quadSet->getColorIndexList()?quadSet->getColorIndexList()->triangulate():Index3ArrayPtr()),
				     quadSet->getTexCoordList(),
					 (quadSet->getTexCoordIndexList()?quadSet->getTexCoordIndexList()->triangulate():Index3ArrayPtr()),
				     quadSet->getNormalPerVertex(),
				     quadSet->getColorPerVertex(),
				     quadSet->getCCW(),
				     quadSet->getSolid(),
				     quadSet->getSkeleton());

  if (!quadSet->getNormalPerVertex() && quadSet->getNormalList()){
	  Point3ArrayPtr _nml( new Point3Array(quadSet->getNormalList()->size()*2));
	  Point3Array::iterator _it = _nml->begin();
	  for (Point3Array::const_iterator _it2 = quadSet->getNormalList()->begin();
		   _it2 != quadSet->getNormalList()->end(); ++_it2)
	  {
		  *_it = *_it2; ++_it;
		  *_it = *_it2; ++_it;	
	  }
	  tr->getNormalList() = _nml;
  }

  if (!quadSet->getColorPerVertex() && quadSet->getColorList()){
	  Color4ArrayPtr _cl( new Color4Array(quadSet->getColorList()->size()*2));
	  Color4Array::iterator _it = _cl->begin();
	  for (Color4Array::const_iterator _it2 = quadSet->getColorList()->begin();
		   _it2 != quadSet->getColorList()->end(); ++_it2)
	  {
		  *_it = *_it2; ++_it;
		  *_it = *_it2; ++_it;	
	  }
	  tr->getColorList() = _cl;
  }

  __discretization = ExplicitModelPtr(tr);

  GEOM_TESSELATOR_UPDATE_CACHE(quadSet);
  return true;
}

/* ----------------------------------------------------------------------- */

bool Tesselator::process( NurbsCurve * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);
  __discretization = ExplicitModelPtr();
  return false;
}

/* ----------------------------------------------------------------------- */


bool Tesselator::process( NurbsPatch * nurbsPatch ) {
  GEOM_ASSERT(nurbsPatch);

  GEOM_TESSELATOR_CHECK_CACHE(nurbsPatch);

  const uint_t _uStride = nurbsPatch->getUStride();
  const uint_t _vStride = nurbsPatch->getVStride();

  const real_t _uStride1 = nurbsPatch->getUStride() - 1;
  const real_t _vStride1 = nurbsPatch->getVStride() - 1;


  Point3ArrayPtr _pointList(new Point3Array(_uStride * _vStride));
  Index3ArrayPtr _indexList(new Index3Array(2 * (_uStride - 1) * (_vStride - 1)));

  uint_t _cur = 0;

  uint_t _pointCount = 0;
  uint_t _indexCount = 0;
  real_t _ufirst=nurbsPatch->getFirstUKnot();
  real_t _ulast=nurbsPatch->getLastUKnot();
  real_t _uinter=_ulast-_ufirst;
  real_t _vfirst=nurbsPatch->getFirstVKnot();
  real_t _vlast=nurbsPatch->getLastVKnot();
  real_t _vinter=_vlast-_vfirst;

  for ( real_t _u = 0 ; _u < _uStride1 ; _u ++){

    for (real_t _v = 0; _v < _vStride1; _v ++) {

      _pointList->setAt(_pointCount++,
                        nurbsPatch->getPointAt((_ufirst +
                                                (_u * _uinter) / _uStride1),
                                               (_vfirst +
                                                (_v * _vinter) / _vStride1)));

      _indexList->setAt(_indexCount++,
                        Index3(_cur,
                               _cur + 1,
                               _cur + _vStride + 1));
       _indexList->setAt(_indexCount++,
                        Index3(_cur,
                               _cur + _vStride + 1,
                               _cur + _vStride));

      _cur++;
    };

    _pointList->setAt(_pointCount++,
                      nurbsPatch->getPointAt((_u / _uStride1),_vlast));

    _cur++;

  };

  for (real_t _v = 0; _v < _vStride1; _v ++)
      _pointList->setAt(_pointCount++,
                        nurbsPatch->getPointAt(_ulast,
                                               (_vfirst +
                                                (_v * _vinter) / _vStride1)));

  _pointList->setAt(_pointCount,nurbsPatch->getPointAt(_ulast,_vlast));

  PolylinePtr _skeleton(new Polyline(Vector3(0,0,0),
                                     Vector3(0,0,0)));

  TriangleSet * t = new TriangleSet(_pointList,_indexList, true, nurbsPatch->getCCW(), // CCW
									false, _skeleton);

  if(__computeTexCoord)t->getTexCoordList() = gridTexCoord(_pointList,_uStride,_vStride);

  __discretization = ExplicitModelPtr(t);
  GEOM_TESSELATOR_UPDATE_CACHE(nurbsPatch);
  return true;
}

/* ----------------------------------------------------------------------- */

bool Tesselator::process( PointSet * pointSet ) {
  GEOM_ASSERT(pointSet);
  // nothing to do as quadSet is already an ExplicitModel
  __discretization = ExplicitModelPtr();
  return false;
}

bool Tesselator::process( Polyline * polyline ) {
  GEOM_ASSERT(polyline);
  // nothing to do as it cannot be transformed into a TriangleSet.
  __discretization = ExplicitModelPtr();
  return false;
}

bool Tesselator::process( BezierCurve2D * bezierCurve ){
  GEOM_ASSERT(bezierCurve);
  // nothing to do as it cannot be transformed into a TriangleSet.
  __discretization = ExplicitModelPtr();
  return false;
}

bool Tesselator::process( NurbsCurve2D * nurbsCurve ){
  GEOM_ASSERT(nurbsCurve);
  // nothing to do as it cannot be transformed into a TriangleSet.
  __discretization = ExplicitModelPtr();
  return false;
}

bool Tesselator::process( PointSet2D * pointSet ){
  GEOM_ASSERT(pointSet);
  // nothing to do as it cannot be transformed into a TriangleSet.
  __discretization = ExplicitModelPtr();
  return false;
}

bool Tesselator::process( Polyline2D * polyline ){
  GEOM_ASSERT(polyline);
  // nothing to do as it cannot be transformed into a TriangleSet.
  __discretization = ExplicitModelPtr();
  return false;
}
