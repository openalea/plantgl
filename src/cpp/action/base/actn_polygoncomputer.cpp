/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
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

#include "actn_polygoncomputer.h"

#include "all_geometry.h"
#include "all_scene.h"
#include "geom_pointarray.h"
#include "geom_geometryarray2.h"
#include "geom_profile.h"
#include "Tools/util_math.h"

GEOM_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */

#define GEOM_TRANSFORMED(obj,field) { \
    GEOM_ASSERT(obj); \
    return obj->get##field()->apply(*this); \
   };


/* ----------------------------------------------------------------------- */
PolygonComputer::PolygonComputer():
  __polygon(0){
}

PolygonComputer::~PolygonComputer(){
}

void
PolygonComputer::clear(){
  __polygon = 0;
}

const uint32_t
PolygonComputer::getPolygonNb() const{
  return __polygon;
}

/* ----------------------------------------------------------------------- */


bool PolygonComputer::process(GeomShape * geomShape){
  return geomShape->getGeometry()->apply(*this);
}

bool PolygonComputer::process(GeomInline * geomInline){
  return process(geomInline->getScene());
}

bool PolygonComputer::process( AmapSymbol * amapSymbol ){
  __polygon = amapSymbol->getIndexListSize();
  return true;
}


bool PolygonComputer::process( AsymmetricHull * asymmetricHull ){
  __polygon = (asymmetricHull->getStacks()*2 -1) *
    (asymmetricHull->getSlices() *4) * 2;
  return true;
}


bool PolygonComputer::process( AxisRotated * axisRotated ){
  GEOM_TRANSFORMED(axisRotated,Geometry);
}

bool PolygonComputer::process( BezierPatch * bezierPatch ){
  __polygon  = (bezierPatch->getUStride() * bezierPatch->getVStride());
  return true;
}

bool PolygonComputer::process( Box * box ){
  __polygon = 6;
  return true;
}

bool PolygonComputer::process( Cone * cone ){
  if(cone->getSolid())
    __polygon = cone->getSlices() * 2;
  else
    __polygon = cone->getSlices();
  return true;
}

bool PolygonComputer::process( Cylinder * cylinder ){
  if(cylinder->getSolid())
    __polygon = cylinder->getSlices() * 3;
  else
    __polygon = cylinder->getSlices();
  return true;
}

bool PolygonComputer::process( ElevationGrid * elevationGrid ){
  __polygon = (elevationGrid->getXDim()-1)
    * (elevationGrid->getYDim( )-1);
    return true;
}

bool PolygonComputer::process( EulerRotated * eulerRotated ){
  GEOM_TRANSFORMED(eulerRotated,Geometry);
}

bool PolygonComputer::process( ExtrudedHull * extrudedHull ){


  const Curve2DPtr& _vcurve = extrudedHull->getVertical();

  real_t _start = _vcurve->getFirstKnot();
  uint32_t size =_vcurve->getStride();
  real_t _step =  (_vcurve->getLastKnot()-_start) / (real_t) size;

  Point2ArrayPtr _pointList(new Point2Array(size));

  for (uint32_t _i = 0; _i < size; _i++) {
    _pointList->setAt(_i,_vcurve->getPointAt(_start));
    _start += _step;
  };

  _pointList->setAt(size - 1, _vcurve->getPointAt(_vcurve->getLastKnot()));
  Point2ArrayPtr _vertical = _pointList;

  size++;
  pair<Point2Array::const_iterator,Point2Array::const_iterator>
    _minAndMax = _vertical->getYMinAndMax();
  Point2Array::const_iterator _begin = _vertical->getBegin();
  uint32_t _ndxBot = distance(_begin,_minAndMax.first);
  uint32_t _ndxTop = distance(_begin,_minAndMax.second);
  uint32_t stack1 = (uint32_t)abs((int)_ndxTop - (int)_ndxBot);
  uint32_t stack2 =  size - stack1;
  __polygon = ((max(stack1,stack2)-1) *
    (extrudedHull->getHorizontal()->getStride()+1));
  return true;
}

bool PolygonComputer::process( FaceSet * faceSet ){
  __polygon = faceSet->getIndexListSize();
  return true;
}

bool PolygonComputer::process( Frustum * frustum ){
    if(frustum->getSolid())
    __polygon = frustum->getSlices() * 3;
  else
    __polygon = frustum->getSlices();
  return true;
}

bool PolygonComputer::process( Extrusion * extrusion ){
  __polygon = ( extrusion->getCrossSection()->getStride()-1 )*
    (extrusion->getAxis()->getStride() ) ;
  if(extrusion->getSolid())
    __polygon += 2* (extrusion->getCrossSection()->getStride()-2);
  return true;
}

bool PolygonComputer::process( Group * group ){
  uint32_t nb = 0;
  GeometryArrayPtr _geometryList = group->getGeometryList();
   for (GeometryArray::const_iterator _i = _geometryList->getBegin()+1;
       _i != _geometryList->getEnd();
       _i++) {
      GeometryPtr geom = *_i;
      if(geom)
        if(geom->apply(*this))
          nb += __polygon;
   }
   __polygon= nb;
   return true;
}

bool PolygonComputer::process( IFS * ifs )
{
  GEOM_ASSERT(ifs);
GEOM_TRACE("process IFS");

  const Transform4ArrayPtr& tList= ifs->getTransfoList();
  uint32_t nbTransfos= tList->getSize();
  nbTransfos= IFS::power( nbTransfos, ifs->getDepth() );

  ifs->getGeometry()->apply(*this);
  __polygon*= nbTransfos;
//todo OK
  return true;
}

bool PolygonComputer::process( NurbsPatch * nurbsPatch ){
  __polygon  = (nurbsPatch->getUStride() * nurbsPatch->getVStride());
  return true;
}

bool PolygonComputer::process( Oriented * oriented ){
  GEOM_TRANSFORMED(oriented,Geometry);
}

bool PolygonComputer::process( Paraboloid * paraboloid ){
  __polygon  = (paraboloid->getSolid() ?
                paraboloid->getSlices() * paraboloid->getStacks() *2 :
                paraboloid->getSlices() * paraboloid->getStacks() *2
                - paraboloid->getSlices());
  return true;
}

bool PolygonComputer::process( QuadSet * quadSet ){
  __polygon = quadSet->getIndexListSize();
  return true;
}

bool PolygonComputer::process( Revolution * revolution ){
  __polygon = ( ( revolution->getProfile()->getStride() - 1)  *
                revolution->getSlices() * 2);
  return true;
}

bool PolygonComputer::process( Swung * swung )
{
  const uint32_t& stride= swung->getProfileInterpolation()->getStride();
  __polygon = ( ( stride - 1 ) * swung->getSlices() * 2 );
  return true;
}

bool PolygonComputer::process( Scaled * scaled ){
  GEOM_TRANSFORMED(scaled,Geometry);
}

bool PolygonComputer::process( Sphere * sphere ){
  __polygon = sphere->getSlices() * 2 * (sphere->getStacks() -1) ;
  return true;
}

bool PolygonComputer::process( Tapered * tapered ){
  GEOM_TRANSFORMED(tapered,Primitive);
}

bool PolygonComputer::process( Translated * translated ){
  GEOM_TRANSFORMED(translated,Geometry);
}

bool PolygonComputer::process( TriangleSet * triangleSet ){
  __polygon = triangleSet->getIndexListSize();
  return true;
}

bool PolygonComputer::process( Disc * disc ){
  __polygon = disc->getSlices();
  return true;
}

bool PolygonComputer::process( ScenePtr _scene) {
  GEOM_ASSERT(_scene);
  __polygon = 0;
  if(_scene->isEmpty()){
    __polygon = 0;
    return false;
  }
  uint32_t nb = 0;
  for(Scene::iterator _it = _scene->getBegin();
      _it != _scene->getEnd();
      _it++){
    if((*_it)->apply(*this))
      nb += __polygon;
  }
  __polygon = nb;
  return true;
}

/* ----------------------------------------------------------------------- */

const real_t GEOM(polygonNumber)(ScenePtr _scene){
  PolygonComputer p;
  if(p.process(_scene))return p.getPolygonNb();
  else return 0;
}

/* ----------------------------------------------------------------------- */
bool PolygonComputer::process( Material * material ){
  __polygon = 0;
  return false;
};
bool PolygonComputer::process( ImageTexture * texture ){
  __polygon = 0;
  return false;
};
bool PolygonComputer::process( MonoSpectral * monoSpectral ){
  __polygon = 0;
  return false;
};
bool PolygonComputer::process( MultiSpectral * multiSpectral ){
  __polygon = 0;
  return false;
};
bool PolygonComputer::process( BezierCurve * bezierCurve ){
  __polygon = 0;
  return false;
}
bool PolygonComputer::process( NurbsCurve * nurbsCurve ){
  __polygon = 0;
  return false;
}
bool PolygonComputer::process( PointSet * pointSet ){
  __polygon = 0;
  return false;
}
bool PolygonComputer::process( GeomPolyline * polyline ){
  __polygon = 0;
  return false;
}
bool PolygonComputer::process( BezierCurve2D * bezierCurve ){
  __polygon = 0;
  return false;
}

bool PolygonComputer::process( NurbsCurve2D * nurbsCurve ){
  __polygon = 0;
  return false;
}


bool PolygonComputer::process( PointSet2D * pointSet ){
  __polygon = 0;
  return false;
}

bool PolygonComputer::process( GeomPolyline2D * polyline ){
  __polygon = 0;
  return false;
}

bool PolygonComputer::process( Text * text ){
  __polygon = 0;
  return false;
}

bool PolygonComputer::process( Font * font ){
  __polygon = 0;
  return false;
}

