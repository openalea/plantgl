/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s):  F. Boudon (frederic.boudon@cirad.fr)
 *
 *       $Source$
 *       $Id: actn_voxelintersection.cpp 2472 2007-01-11 17:35:33Z boudon $
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

#define INTERIOR_TEST

#define GEOM_TRACE_F(type,obj);

#ifndef GEOM_TRACE_F
#define GEOM_TRACE_F(type,obj) cerr << type << ' ' << obj->getName() << " processing with " \
     << __voxel->getLowerLeftCorner() << "," << __voxel->getUpperRightCorner()  << endl;
#endif

#include "voxelintersection.h"

#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/pgl_scene.h>
#include <plantgl/math/util_math.h>

#include "../raycasting/util_intersection.h"
#include "../raycasting/rayintersection.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */

#ifdef INTERIOR_TEST

#define GEOM_INTERIOR_TEST(obj) \
    if(obj->getSolid()){ \
      Vector3 _center = __voxel->getCenter(); \
      if(!obj->apply(__bboxComputer))return false; \
      Vector3 _dir = __bboxComputer.getBoundingBox()->getLowerLeftCorner() - Vector3(1,1,1) - _center; \
      Ray ray(_center,_dir); \
      RayIntersection rintersect(__bboxComputer.getDiscretizer()); \
      rintersect.setRay(ray); \
      if(!obj->apply(rintersect))return false; \
      else if ( rintersect.getIntersection().isValid() && \
                rintersect.getIntersection()->getSize() % 2 == 1 ){ \
        __filled = true; \
        return true; \
      } \
    } \

#else

#define GEOM_INTERIOR_TEST(obj);

#endif


#define GEOM_INTERSECTION_TRANSF(obj)\
  GEOM_ASSERT(obj); \
  Matrix4TransformationPtr mat; \
  if(mat.cast(obj->getTransformation())){ \
   __transformstack.push_back(mat->getMatrix()); \
   return obj->getGeometry()->apply(*this); } \
  else return false; \


#define GEOM_INIT_MATRIX(_matrix)\
  Matrix4 _matrix; \
  for(vector<Matrix4>::const_iterator _it = __transformstack.begin(); \
      _it != __transformstack.end(); _it++) \
      _matrix *= *_it; \
  _matrix = inverse(_matrix); \


#define GEOM_INIT_VOXEL(origin,xdir,ydir,zdir) \
  Vector3 origin = __voxel->getLowerLeftCorner(); \
  Vector3 xdir( __voxel->getLowerLeftCorner().x() , origin.y() , origin.z() ); \
  Vector3 ydir( origin.x() , __voxel->getLowerLeftCorner().y() , origin.z() ); \
  Vector3 zdir( origin.x() , origin.y() , __voxel->getLowerLeftCorner().z() ); \
  GEOM_INIT_MATRIX(matrix); \
  origin = matrix * origin; \
  xdir = matrix * xdir; \
  ydir = matrix * ydir; \
  zdir = matrix * zdir; \

#if GEOM_DEBUG
#define GEOM_DISCRETIZE(geom) \
   cerr << "Apply on discretrisation" << endl; \
   GEOM_ASSERT(geom); \
   if(!geom->apply(__bboxComputer.getDiscretizer()))return false; \
   else return __bboxComputer.getDiscretizer().getDiscretization()->apply(*this); \

#else
#define GEOM_DISCRETIZE(geom) \
   GEOM_ASSERT(geom); \
   if(!geom->apply(__bboxComputer.getDiscretizer()))return false; \
   else return __bboxComputer.getDiscretizer().getDiscretization()->apply(*this); \

#endif
/* ----------------------------------------------------------------------- */

VoxelIntersection::VoxelIntersection(BBoxComputer& bboxcomputer) :
    __filled(false),
    __voxel(NULL),
    __bboxComputer(bboxcomputer){
}

VoxelIntersection::~VoxelIntersection(){
    /// Nothing to do.
}

void
VoxelIntersection::setVoxel(Voxel * voxel){
    __voxel = voxel;
}


/* ----------------------------------------------------------------------- */
#define GEOM_TEST_CLASS(obj,result) \
        PrimitivePtr primitive; \
        if(!primitive.cast(obj)){ \
            GroupPtr group; \
            if(!group.cast(obj)){ \
              ScaledPtr sc; \
              if(!sc.cast(obj)) { \
               TranslatedPtr tr; \
               if(!tr.cast(obj)){ \
                 if(!obj->apply(__bboxComputer.getDiscretizer())){ \
                   result = false; \
                   cerr << "Error with discretizer !" << endl; \
                 } \
                 else { \
                   ExplicitModelPtr a(__bboxComputer.getDiscretizer().getDiscretization()); \
                   if(a){ \
                    if(obj->isNamed())a->setName(obj->getName()); \
                    result =  a->apply(*this);  \
                   } \
                   else cerr << "Error with discretization !" << endl; \
                 } \
               } \
               else{ \
                 result =  tr->apply(*this); \
               } \
              } \
              else{ \
                result =  sc->apply(*this); \
              } \
            } \
            else{ \
              result = group->apply(*this); \
            } \
        } \
        else{ \
          result =  primitive->apply(*this); \
        } \


bool VoxelIntersection::process(Shape * geomshape){
    GEOM_ASSERT(geomshape);GEOM_TRACE_F("geomshape",geomshape);
    __filled = false;
    geomshape->apply(__bboxComputer);
    if(!intersection(*__voxel,__bboxComputer.getBoundingBox())){
      return false;
    }
    else {
      bool result = false;
      GEOM_TEST_CLASS(geomshape->getGeometry(),result);
      return result;
    }
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process(Inline * geomInline){
    GEOM_ASSERT(_inline);GEOM_TRACE_F("inline",geomInline);
    geomInline->apply(__bboxComputer);
    if(!intersection(*__voxel,__bboxComputer.getBoundingBox())) return false;
    else {
        cerr << "Not yet implemented !" << endl;
        return true;
    }
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( AmapSymbol * amapSymbol ){
    GEOM_ASSERT(amapSymbol);GEOM_TRACE_F("amapSymbol",amapSymbol);
    __filled = false;
    Point3ArrayPtr points(amapSymbol->getPointList());
    /// Intersection of the facet Vertex.
    for(Point3Array::iterator _it1 = points->getBegin();
        _it1 != points->getEnd();
        _it1++)
        if(__voxel->intersect( *_it1))return true;

    /// Intersection of the facet edge.
    for(IndexArray::iterator _it2 = amapSymbol->getIndexList()->getBegin();
        _it2 != amapSymbol->getIndexList()->getEnd()-1;
        _it2++){
        if(__voxel->intersect( points->getAt(_it2->getAt(0)),
                               points->getAt(_it2->getAt(1))))
            return true;
        if(__voxel->intersect( points->getAt(_it2->getAt(1)),
                               points->getAt(_it2->getAt(2))))
            return true;
        if(_it2->getSize() == 4){
            if(__voxel->intersect( points->getAt(_it2->getAt(2)),
                                   points->getAt(_it2->getAt(3))))
                return true;
            if(__voxel->intersect( points->getAt(_it2->getAt(3)),
                                   points->getAt(_it2->getAt(0))))
                return true;
        }
        else if(_it2->getSize() == 3) {
            if(__voxel->intersect( points->getAt(_it2->getAt(2)),
                                   points->getAt(_it2->getAt(0))))
                return true;
        }
    }
    /// Intersection of the voxel edge.
    for(IndexArray::iterator _it = amapSymbol->getIndexList()->getBegin();
        _it != amapSymbol->getIndexList()->getEnd();
        _it++){
        if(_it->getSize() == 3){
            if(__voxel->intersect( points->getAt(_it->getAt(0)),
                                   points->getAt(_it->getAt(1)),
                                   points->getAt(_it->getAt(2))))
                return true;
        }
        else if(_it->getSize() == 4)
            if(__voxel->intersect( points->getAt(_it->getAt(0)),
                                   points->getAt(_it->getAt(1)),
                                   points->getAt(_it->getAt(2)),
                                   points->getAt(_it->getAt(3))))
                return true;
    }

#ifdef INTERIOR_TEST
    GEOM_INTERIOR_TEST(amapSymbol);
#endif
    return false;
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( AsymmetricHull * asymmetricHull ) {
  GEOM_DISCRETIZE(asymmetricHull);
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( AxisRotated * axisRotated ) {
    GEOM_INTERSECTION_TRANSF(axisRotated);
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( BezierCurve * bezierCurve ) {
  GEOM_DISCRETIZE(bezierCurve);
}

/* ----------------------------------------------------------------------- */



bool VoxelIntersection::process( BezierPatch * bezierPatch ) {
  GEOM_DISCRETIZE(bezierPatch);
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( Box * box ) {
  GEOM_ASSERT(box);GEOM_TRACE_F("box",box);
  const Vector3& ll = __voxel->getLowerLeftCorner();
  const Vector3& ur = __voxel->getUpperRightCorner();
  const Vector3& size = box->getSize();

  //  cerr << "Box processing" << endl;

  //   Test of the bounding box give :
  if(ll.x() > size.x() || ur.x() < -size.x()) return false;
  if(ll.y() > size.y() || ur.y() < -size.y()) return false;
  if(ll.z() > size.z() || ur.z() < -size.z()) return false;

  if(ll.x() > -size.x() && ll.y() > -size.y() && ll.z() > -size.z() &&
     ur.x() < size.x() && ur.y() < size.y() && ur.z() < size.z()){
    __filled = true;
  }
  else  __filled = false;
  return true;

  /** Olivier Puech Code -- detects if the center of the voxel is inside the box surface.

  cout<<"Stack size : "<<(__transformstack.size())<<endl;
  Vector4 BoxSize(abs(box->getSize()), 0);
  Matrix4 _matrix;

  if(__transformstack.empty())
    _matrix.set(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
  else
    _matrix *= (*(__transformstack.rend()-1));

  Vector3 scale,rotate,translate;
 // cout<<"Matrix:"<<_matrix<<endl;
  _matrix.getTransformation(scale,rotate,translate);
 // cout<<"Scale:"<<scale<<endl;
  Vector4 VoxelCenterInIJKBox(abs(inverse(_matrix) * Vector4((ll + ur)/2,0)));
  if( (BoxSize.x()*scale.x() > VoxelCenterInIJKBox.x()) &&
      (BoxSize.y()*scale.y() > VoxelCenterInIJKBox.y()) &&
      (BoxSize.z()*scale.z() > VoxelCenterInIJKBox.z())  )
        { __filled = true ; return true ; }
  else  { __filled = false; return false; }
  **/

}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( Cone * cone ) {
  GEOM_ASSERT(cone);GEOM_TRACE_F("cone",cone);
//  cerr << "Process cone" << endl;

  /* To test if a point is inside the cone,
     we use r = sqrt(x*x +y*y) and z (cylindrical coordinates)
     and the equation
           R * ( H -  z)
     r <  --------------
               H
  */

  const Vector3& ll = __voxel->getLowerLeftCorner();
  const Vector3& ur = __voxel->getUpperRightCorner();
  __filled = false;

  /// BBox test give ll.z() < cone->getHeight() && ur.z() > 0.
    if(ur.z() <= 0 || ll.z() > cone->getHeight()) {
      return false;
    }

    if(cone->getSolid()){
      if(ll.z() >= 0 &&
         ur.z() <= cone->getHeight() ){

         /// If the farest point of the voxel from Origin is include in cone,
         /// the voxel is include in cone

                  Vector2 pmax(max(fabs(ll.x()),fabs(ur.x())),max(fabs(ll.y()),fabs(ur.y())));

         if(norm(pmax) <=  (( cone->getRadius() * ( cone->getHeight() - ur.z() ) ) / cone->getHeight() )){
             __filled = true;
             return true;
         }
     }


     /// If the nearest point of the voxel from the origin is inside the cone,
     /// Then the voxel intersect cone volume.
     Vector2 pmin(min(fabs(ll.x()),fabs(ur.x())),
                  min(fabs(ll.y()),fabs(ur.y())));

     real_t height = min(fabs(ll.z()),fabs(ur.z()));
     if( ll.x() <= 0 && ur.x() >= 0 ) pmin.x() = 0;
     if( ll.y() <= 0 && ur.y() >= 0 ) pmin.y() = 0;
     if( ll.z() <= 0 && ur.z() >= 0 ) height = 0;

     if(norm(pmin) <  ((  cone->getRadius() * ( cone->getHeight() - height ) ) / cone->getHeight())){
         return true;
     }
  }
  else {
      /// If the farest point of the voxel from the origin is outside the cone
      /// and the nearest point of the voxel from the origin is inside,
      /// Then the voxel intersect cone surface.

      Vector2 pmax(max(fabs(ll.x()),fabs(ur.x())),
                   max(fabs(ll.y()),fabs(ur.y())));

      Vector2 pmin(min(fabs(ll.x()),fabs(ur.x())),
                   min(fabs(ll.y()),fabs(ur.y())));

      real_t height = min(fabs(ll.z()),fabs(ur.z()));
      if( ll.x() <= 0 && ur.x() >= 0 ) pmin.x() = 0;
      if( ll.y() <= 0 && ur.y() >= 0 ) pmin.y() = 0;
      if( ll.z() <= 0 && ur.z() >= 0 ) height = 0;

     if( ( norm(pmin) <=  ((  cone->getRadius() * ( cone->getHeight() - height ) ) / cone->getHeight()) )&&
         ( norm(pmax) >=  (( cone->getRadius() * ( cone->getHeight() - ur.z() ) ) / cone->getHeight() ))){
         return true;
     }
  }

  return false;

}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( Cylinder * cylinder ) {
  GEOM_ASSERT(cylinder);GEOM_TRACE_F("cylinder",cylinder);
//  cerr << "process cylinder" << endl;

  /* To test if a point is inside the cylinder,
     we use r = sqrt(x*x +y*y) and z (cylindrical coordinates)
     and the equation r < R and 0 < z < H
  */

  if(__voxel->getLowerLeftCorner().z() > cylinder->getHeight() ||
     __voxel->getUpperRightCorner().z() < 0 )return false; // BBox Test

  const Vector3& ll = __voxel->getLowerLeftCorner();
  const Vector3& ur = __voxel->getUpperRightCorner();
  const real_t& radius = cylinder->getRadius();
  __filled = false;

  if(cylinder->getSolid()) {
     if(ll.z() >= 0 &&
        ur.z() <= cylinder->getHeight() ){

         Vector2 pmax(max(fabs(ll.x()),fabs(ur.x())),
                      max(fabs(ll.y()),fabs(ur.y())));

         if(norm(pmax) <=  radius){
             __filled = true;
             return true;
         }
     }
     Vector2 pmin(min(fabs(ll.x()),fabs(ur.x())),
                  min(fabs(ll.y()),fabs(ur.y())));

     if( ll.x() <= 0 && ur.x() >= 0 ) pmin.x() = 0;
     if( ll.y() <= 0 && ur.y() >= 0 ) pmin.y() = 0;

     if(norm(pmin) <=  radius){
         return true;
     }
  }
  else {

      Vector2 pmax(max(fabs(ll.x()),fabs(ur.x())),
                   max(fabs(ll.y()),fabs(ur.y())));

      Vector2 pmin(min(fabs(ll.x()),fabs(ur.x())),
                   min(fabs(ll.y()),fabs(ur.y())));

      if( ll.x() <= 0 && ur.x() >= 0 ) pmin.x() = 0;
      if( ll.y() <= 0 && ur.y() >= 0 ) pmin.y() = 0;

      if(norm(pmin) <=  radius && norm(pmax) >=  radius){
          return true;
      }

  }
//  cerr << "Return false" << endl;
  return false;

}

/* ----------------------------------------------------------------------- */



bool VoxelIntersection::process( ElevationGrid * elevationGrid ) {
  GEOM_DISCRETIZE(elevationGrid);
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( EulerRotated * eulerRotated ) {
  GEOM_INTERSECTION_TRANSF(eulerRotated);
/*  GEOM_ASSERT(eulerRotated);
    GEOM_TRACE_F("eulerRotated",eulerRotated);
    #warning function for EulerRotated not implemented
    return false;*/
}

/* ----------------------------------------------------------------------- */



bool VoxelIntersection::process( ExtrudedHull * extrudedHull ) {
  GEOM_DISCRETIZE(extrudedHull);
/*  GEOM_ASSERT(extrudedHull);
  __filled = false;

  const Vector3 ll(__voxel->getLowerLeftCorner());
  const Vector3 ur = __voxel->getUpperRightCorner();
  __voxel->getLowerLeftCorner() = Vector3(ll.x(),ll.y(),0);
  __voxel->getUpperRightCorner() = Vector3(ur.x(),ur.y(),0);

  if(extrudedHull->getHorizontal()->apply(__bboxComputer.getDiscretizer()))return false;
  PolylinePtr _hor = PolylinePtr(__bboxComputer.getDiscretizer().getDiscretization());
  bool _horcond = false;
  bool _insidehorcond = false;

  for(Point3Array::iterator _it = _hor->getPointList()->getBegin();
      _it != polyline->getPointList()->getEnd() && !_horcond;
      _it++){
    if(__voxel->intersect( *_it))horcond = true;
    if(_it->x() > ur.x
  for(Point3Array::iterator _it = _hor->getPointList()->getBegin();
      _it != _hor->getPointList()->getEnd()-1 && !_horcond;
      _it++)
    if(__voxel->intersect(*_it,*(_it+1)))_horcond = true;
  if(!horcond){

    if(__voxel->intersect(*(_hor->getPointList()->getEnd()-1),_hor->getPointListAt(0)))
  }
  if(extrudedHull->getVertical()->apply(__bboxComputer.getDiscretizer()))return false;
  PolylinePtr ver = PolylinePtr(__bboxComputer.getDiscretizer().getDiscretization());
  return false;*/
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( FaceSet * faceSet ) {
    GEOM_ASSERT(faceSet);GEOM_TRACE_F("faceSet",faceSet);
    __filled = false;
    Point3ArrayPtr points(faceSet->getPointList());
    /// Intersection of the facet Vertex.
    for(Point3Array::iterator _it1 = points->getBegin();
        _it1 != points->getEnd();
        _it1++)
        if(__voxel->intersect( *_it1))return true;

  /// Intersection of the facet edge.
    for(IndexArray::iterator _it2 = faceSet->getIndexList()->getBegin();
        _it2 != faceSet->getIndexList()->getEnd()-1;
        _it2++){
        if(__voxel->intersect( points->getAt(_it2->getAt(0)),
                               points->getAt(_it2->getAt(1))))
            return true;
        if(__voxel->intersect( points->getAt(_it2->getAt(1)),
                               points->getAt(_it2->getAt(2))))
            return true;
        if(_it2->getSize() == 4){
            if(__voxel->intersect( points->getAt(_it2->getAt(2)),
                                   points->getAt(_it2->getAt(3))))
                return true;
            if(__voxel->intersect( points->getAt(_it2->getAt(3)),
                                   points->getAt(_it2->getAt(0))))
                return true;
        }
        else if(_it2->getSize() == 3) {
            if(__voxel->intersect( points->getAt(_it2->getAt(2)),
                                   points->getAt(_it2->getAt(0))))
                return true;
        }
    }
    /// Intersection of the voxel edge.
    for(IndexArray::iterator _it = faceSet->getIndexList()->getBegin();
        _it != faceSet->getIndexList()->getEnd();
        _it++){
        if(_it->getSize() == 3){
            if(__voxel->intersect( points->getAt(_it->getAt(0)),
                                   points->getAt(_it->getAt(1)),
                                   points->getAt(_it->getAt(2))))
                return true;
        }
        else if(_it->getSize() == 4)
            if(__voxel->intersect( points->getAt(_it->getAt(0)),
                                   points->getAt(_it->getAt(1)),
                                   points->getAt(_it->getAt(2)),
                                   points->getAt(_it->getAt(3))))
                return true;
    }

#ifdef INTERIOR_TEST
    GEOM_INTERIOR_TEST(faceSet);
#endif
    return false;
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( Frustum * frustum ) {
  GEOM_ASSERT(frustum);GEOM_TRACE_F("frustum",frustum);

  /* To test if a point is inside the frustum,
     we use r = sqrt(x*x +y*y) and z (cylindrical coordinates)
     and the equation
                   z *R ( 1 - taper )
     r <   R  -   -------------------
                     H
  */

  __filled = false;
  const Vector3& ll = __voxel->getLowerLeftCorner();
  const Vector3& ur = __voxel->getUpperRightCorner();
  const real_t& radius = frustum->getRadius();
  real_t factor = radius *  ( 1 - frustum->getTaper() ) / frustum->getHeight();

  if(frustum->getSolid()){
     if(__voxel->getLowerLeftCorner().z() >= 0 &&
     __voxel->getUpperRightCorner().z() <= frustum->getHeight() ){

         /// If the farest point of the voxel from Origin is include in frustum,
         /// the voxel is include in frustum
         Vector2 pmax(max(fabs(ll.x()),fabs(ur.x())),
                      max(fabs(ll.y()),fabs(ur.y())));

         if(norm(pmax)<  radius - ur.z() * factor ){
             __filled = true;
             return true;
         }
     }


     /// the nearest point of the voxel from the origin is inside,
     /// Then the voxel intersect frustum volume.
     Vector2 pmin(min(fabs(ll.x()),fabs(ur.x())),
                  min(fabs(ll.y()),fabs(ur.y())));

     real_t height = min(fabs(ll.z()),fabs(ur.z()));
     if( ll.x() <= 0 && ur.x() >= 0 ) pmin.x() = 0;
     if( ll.y() <= 0 && ur.y() >= 0 ) pmin.y() = 0;
     if( ll.z() <= 0 && ur.z() >= 0 ) height = 0;

     if(norm(pmin)<  radius - height * factor){
         return true;
     }
  }
  else {
      /// If the farest point of the voxel from the origin is outside the frustum and
      /// the nearest point of the voxel from the origin is inside,
      /// Then the voxel intersect frustum surface.


      Vector2 pmax(max(fabs(ll.x()),fabs(ur.x())),
                   max(fabs(ll.y()),fabs(ur.y())));

      Vector2 pmin(min(fabs(ll.x()),fabs(ur.x())),
                   min(fabs(ll.y()),fabs(ur.y())));

      real_t height = min(fabs(ll.z()),fabs(ur.z()));
      if( ll.x() <= 0 && ur.x() >= 0 ) pmin.x() = 0;
      if( ll.y() <= 0 && ur.y() >= 0 ) pmin.y() = 0;
      if( ll.z() <= 0 && ur.z() >= 0 ) height = 0;

     if( ( norm(pmin) <=  radius - height * factor )&&
         ( norm(pmax) >=  radius - ur.z() * factor )){
         return true;
     }
  }

  return false;

}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( Extrusion * extrusion ) {
  GEOM_DISCRETIZE(extrusion);
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( Group * group ) {
  GEOM_ASSERT(group);GEOM_TRACE_F("group",group);
//  cerr << "Process group" << endl;
//  GEOM_DISCRETIZE(group);

  const GeometryArrayPtr& _l = group->getGeometryList();
  __filled = false;
  bool b = false;
  BoundingBoxPtr bb;
  Matrix4 _matrix;
  for(vector<Matrix4>::reverse_iterator _it1 = __transformstack.rbegin();
      _it1 != __transformstack.rend(); _it1++)
    _matrix *= *_it1;

  for(GeometryArray::const_iterator _it = _l->getBegin() ; _it != _l->getEnd() && ! __filled ; _it++){
//    cerr << "Apply to '" << (*_it)->getName() << '\'' << endl;
    // b = b || (*_it)->apply(*this);
    (*_it)->apply(__bboxComputer);
//    bb = __bboxComputer.getBoundingBox();
//    bb->transform(_matrix);

/*    if(!intersection(*__voxel,bb)){
      cerr << "No intersection" << endl;
      return false;
    }
    else {*/
      bool result = false;
      GEOM_TEST_CLASS((*_it),result);
       b = b || result;
//    }
  }
//  cerr << "Result for group = " << (b?"True":"False") << endl;
  return b;

  return false;
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( IFS * ifs ) {
//todo OK
GEOM_ASSERT(ifs);
GEOM_TRACE("process IFS");
//  GEOM_DISCRETIZE(ifs);

  uint_t size= __transformstack.size();

  ITPtr transfos = ITPtr::Cast( ifs->getTransformation() );
  GEOM_ASSERT(transfos);
  const Matrix4ArrayPtr& matrixList= transfos->getAllTransfo();
  GEOM_ASSERT(matrixList);

  Matrix4Array::const_iterator m_it= matrixList->getBegin();
  __transformstack.push_back(*m_it);
  m_it++;

  __filled = false;
  bool b = false;
  GEOM_TEST_CLASS((ifs->getGeometry()),b);

  while( m_it != matrixList->getEnd() )
    {
    __transformstack[size]= *m_it;

    bool result = false;
    GEOM_TEST_CLASS((ifs->getGeometry()),result);
    b = b || result;

    m_it++;
    }

  return b;
}

/* ----------------------------------------------------------------------- */



bool VoxelIntersection::process( NurbsCurve * nurbsCurve ) {
  GEOM_DISCRETIZE(nurbsCurve);
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( NurbsPatch * nurbsPatch ) {
  GEOM_DISCRETIZE(nurbsPatch);
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( Oriented * oriented ) {
  GEOM_INTERSECTION_TRANSF(oriented);
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( Paraboloid * paraboloid ) {
  GEOM_ASSERT(paraboloid);GEOM_TRACE_F("paraboloid",paraboloid);

  /* To test if a point is inside the paraboloid,
     we use r = sqrt(x*x +y*y) and z (cylindrical coordinates)
     and the equation
                    ( r ) shape
     h <  H * ( 1 - (---)       )
                    ( R )
  */

    __filled = false;
  const Vector3& ll = __voxel->getLowerLeftCorner();
  const Vector3& ur = __voxel->getUpperRightCorner();

  /// BBox test give ll.z() < paraboloid->getHeight() && ur.z() > 0.
    if(ll.z() > paraboloid->getHeight() || ur.z() < 0)return false;

  const real_t& radius = paraboloid->getRadius();
  const real_t& shape = paraboloid->getShape();

  if(paraboloid->getSolid()){
     if(__voxel->getLowerLeftCorner().z() >= 0 &&
     __voxel->getUpperRightCorner().z() <= paraboloid->getHeight() ){

         /// If the farest point of the voxel from Origin is include in paraboloid,
         /// the voxel is include in paraboloid
         Vector2 pmax(max(fabs(ll.x()),fabs(ur.x())),
                      max(fabs(ll.y()),fabs(ur.y())));

         if(ur.z() <= paraboloid->getHeight() *(1 - pow( (norm(pmax)/radius) , shape)) ){
             __filled = true;
             return true;
         }
     }


     /// the nearest point of the voxel from the origin is inside,
     /// Then the voxel intersect paraboloid volume.
     Vector2 pmin(min(fabs(ll.x()),fabs(ur.x())),
                  min(fabs(ll.y()),fabs(ur.y())));

     real_t height = min(fabs(ll.z()),fabs(ur.z()));
     if( ll.x() <= 0 && ur.x() >= 0 ) pmin.x() = 0;
     if( ll.y() <= 0 && ur.y() >= 0 ) pmin.y() = 0;
     if( ll.z() <= 0 && ur.z() >= 0 ) height = 0;

     if(height <= paraboloid->getHeight() *(1 - pow( (norm(pmin)/radius) , shape))){
         return true;
     }
  }
  else {
      /// If the farest point of the voxel from the origin is outside the paraboloid and
      /// the nearest point of the voxel from the origin is inside,
      /// Then the voxel intersect paraboloid surface.

      Vector2 pmax(max(fabs(ll.x()),fabs(ur.x())),
                   max(fabs(ll.y()),fabs(ur.y())));

      Vector2 pmin(min(fabs(ll.x()),fabs(ur.x())),
                   min(fabs(ll.y()),fabs(ur.y())));

      real_t height = min(fabs(ll.z()),fabs(ur.z()));
      if( ll.x() <= 0 && ur.x() >= 0 ) pmin.x() = 0;
      if( ll.y() <= 0 && ur.y() >= 0 ) pmin.y() = 0;
      if( ll.z() <= 0 && ur.z() >= 0 ) height = 0;

      if ( ( ur.z() >= paraboloid->getHeight() *(1 - pow( (norm(pmax)/radius) , shape)) ) &&
           ( height <= paraboloid->getHeight() *(1 - pow( (norm(pmin)/radius) , shape)) ) ){
          return true;
      }
  }

  return false;

}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( PointSet * pointSet ) {
    GEOM_ASSERT(pointSet);GEOM_TRACE_F("pointSet",pointSet);
    __filled = false;
    for(Point3Array::iterator _it = pointSet->getPointList()->getBegin();
        _it != pointSet->getPointList()->getEnd();
        _it++)
        if(intersection(*__voxel, *_it))return true;
    return false;
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( Polyline * polyline ) {
    GEOM_ASSERT(polyline);GEOM_TRACE_F("polyline",polyline);
    __filled = false;
    for(Point3Array::iterator _it1 = polyline->getPointList()->getBegin();
        _it1 != polyline->getPointList()->getEnd();
        _it1++)
        if(__voxel->intersect( *_it1))return true;
    for(Point3Array::iterator _it = polyline->getPointList()->getBegin();
        _it != polyline->getPointList()->getEnd()-1;
        _it++)
        if(__voxel->intersect(*_it,*(_it+1)))return true;
    return false;
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( Revolution * revolution ) {
  GEOM_ASSERT(revolution);GEOM_TRACE_F("revolution",revolution);
  __filled = false;
  const Curve2DPtr& curve = revolution->getProfile();
  if(!curve->apply(__bboxComputer.getDiscretizer())) return false;
  const Point3ArrayPtr& points = __bboxComputer.getDiscretizer().getDiscretization()->getPointList();
  if(points->getAt(0).x() == 0.0 ||
     (points->getEnd()-1)->x() == 0.0) {
      /// revolution represent a volume.
    if(!revolution->apply(__bboxComputer.getDiscretizer()))return false; \
    else return __bboxComputer.getDiscretizer().getDiscretization()->apply(*this); \
  }
  else {
      /// revolution represent a surface.

      const Vector3& ur = __voxel->getUpperRightCorner();
      const Vector3& ll = __voxel->getLowerLeftCorner();
      const real_t& ztop = __voxel->getUpperRightCorner().z();
      const real_t& zbottom = __voxel->getLowerLeftCorner().z();

      real_t rmax = norm(Vector2(max(fabs(ur.x()),fabs(ll.x())),max(fabs(ur.y()),fabs(ll.y()))));
      Vector2 vmin(min(fabs(ur.x()),fabs(ll.x())),min(fabs(ur.y()),fabs(ll.y())));
      if(ur.x() > 0 && ll.x() < 0) vmin.x() = 0;
      if(ur.y() > 0 && ll.y() < 0) vmin.y() = 0;
      real_t rmin = norm(vmin);

      for(Point3Array::iterator _it = points->getBegin();
          _it != points->getEnd();
          _it++)
          if(_it->y() <= ztop && _it->y() >= zbottom &&
             _it->x() <= rmax && _it->x() >= rmin )return true;
      return false;
  }
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( QuadSet * quadSet ) {
  GEOM_ASSERT(quadSet);GEOM_TRACE_F("quadSet",quadSet);
  __filled = false;
  Point3ArrayPtr points(quadSet->getPointList());
  for(Point3Array::iterator _it1 = points->getBegin();
      _it1 != points->getEnd();
      _it1++)
    if(__voxel->intersect( *_it1)){
      return true;
    }
  for(Index4Array::iterator _it2 = quadSet->getIndexList()->getBegin();
      _it2 != quadSet->getIndexList()->getEnd()-1;
      _it2++){
    if(__voxel->intersect( points->getAt(_it2->getAt(0)),
                           points->getAt(_it2->getAt(1)))){
      return true;
    }
    if(__voxel->intersect( points->getAt(_it2->getAt(1)),
                           points->getAt(_it2->getAt(2)))){
      return true;
    }
    if(__voxel->intersect( points->getAt(_it2->getAt(2)),
                           points->getAt(_it2->getAt(3)))){
      return true;
    }
    if(__voxel->intersect( points->getAt(_it2->getAt(3)),
                           points->getAt(_it2->getAt(0)))){
      return true;
    }
  }
  for(Index4Array::iterator _it = quadSet->getIndexList()->getBegin();
      _it != quadSet->getIndexList()->getEnd();
      _it++){
    if(__voxel->intersect( points->getAt(_it->getAt(0)),
                           points->getAt(_it->getAt(1)),
                           points->getAt(_it->getAt(2)),
                           points->getAt(_it->getAt(3)))){
      return true;
    }
  }
#ifdef INTERIOR_TEST
    GEOM_INTERIOR_TEST(quadSet);
#endif
  return false;
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( Scaled * scaled ) {
  GEOM_ASSERT(scaled);GEOM_TRACE_F("scaled",scaled);

  Vector3 ll =  __voxel->getLowerLeftCorner();
  Vector3 ur = __voxel->getUpperRightCorner();
  const Vector3 & s = scaled->getScale();
  if(fabs(s.x()) > GEOM_EPSILON && fabs(s.y()) > GEOM_EPSILON && fabs(s.z()) > GEOM_EPSILON){
    __voxel->getLowerLeftCorner() = Vector3(ll.x() / s.x(), ll.y() / s.y(), ll.z() / s.z());
    __voxel->getUpperRightCorner() = Vector3(ur.x() / s.x(), ur.y() / s.y(), ur.z() / s.z());

    bool result = false;
    GEOM_TEST_CLASS(scaled->getGeometry(),result);

    __voxel->getLowerLeftCorner()=ll;
    __voxel->getUpperRightCorner()=ur;

    return result;
  }
  else {
    GEOM_INTERSECTION_TRANSF(scaled);
  }
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( Sphere * sphere ) {
  GEOM_ASSERT(sphere);GEOM_TRACE_F("sphere",sphere);

  const Vector3& ll = __voxel->getLowerLeftCorner();
  const Vector3& ur = __voxel->getUpperRightCorner();
  Vector3 pmax(max(fabs(ll.x()),fabs(ur.x())),
               max(fabs(ll.y()),fabs(ur.y())),
               max(fabs(ll.z()),fabs(ur.z())));

  if(norm(pmax) <=  sphere->getRadius()){
      /// Include in
      __filled = true;
      return true;
  }

  __filled = false;

  Vector3 pmin(min(fabs(ll.x()),fabs(ur.x())),
               min(fabs(ll.y()),fabs(ur.y())),
               min(fabs(ll.z()),fabs(ur.z())));
  if( ll.x() <= 0 && ur.x() >= 0 ) pmin.x() = 0;
  if( ll.y() <= 0 && ur.y() >= 0 ) pmin.y() = 0;
  if( ll.z() <= 0 && ur.z() >= 0 ) pmin.z() = 0;

  if(norm(pmin)<=  sphere->getRadius()){
      return true;
  }
  return false;
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( Swung * swung ) {
  GEOM_DISCRETIZE(swung);
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( Tapered * tapered ) {
  GEOM_DISCRETIZE(tapered);
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( Translated * translated ) {
  GEOM_ASSERT(translated);GEOM_TRACE_F("translated",translated);

  __voxel->getLowerLeftCorner()-=translated->getTranslation();
  __voxel->getUpperRightCorner()-=translated->getTranslation();



  bool result = false;
  GEOM_TEST_CLASS(translated->getGeometry(),result);

  __voxel->getLowerLeftCorner()+=translated->getTranslation();
  __voxel->getUpperRightCorner()+=translated->getTranslation();

  return result;
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( TriangleSet * triangleSet ) {
  GEOM_ASSERT(triangleSet);
  GEOM_TRACE_F("triangleSet",triangleSet);
  __filled = false;
  Point3ArrayPtr points(triangleSet->getPointList());
#ifndef GEOM_DEBUG
  for(Point3Array::iterator _it1 = points->getBegin();
      _it1 != points->getEnd();
      _it1++)
    if(__voxel->intersect( *_it1))return true;
  for(Index3Array::iterator _it2 = triangleSet->getIndexList()->getBegin();
      _it2 != triangleSet->getIndexList()->getEnd()-1;
      _it2++){
    if(__voxel->intersect( points->getAt(_it2->getAt(0)),
                           points->getAt(_it2->getAt(1))))
      return true;
    if(__voxel->intersect( points->getAt(_it2->getAt(1)),
                           points->getAt(_it2->getAt(2))))
      return true;
    if(__voxel->intersect( points->getAt(_it2->getAt(2)),
                           points->getAt(_it2->getAt(0))))
      return true;
  }
#endif
#ifdef GEOM_DEBUG
  cerr << "Test edges of the voxel in the triangle" << endl;
#endif
  for(Index3Array::iterator _it = triangleSet->getIndexList()->getBegin();
      _it != triangleSet->getIndexList()->getEnd();
      _it++){
    if(__voxel->intersect( points->getAt(_it->getAt(0)),
                           points->getAt(_it->getAt(1)),
                           points->getAt(_it->getAt(2))))
      return true;
  }

#ifdef INTERIOR_TEST
  GEOM_INTERIOR_TEST(triangleSet);
#endif
  return false;
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( BezierCurve2D * bezierCurve ) {
  GEOM_DISCRETIZE(bezierCurve);
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( Disc * disc ) {
    GEOM_ASSERT(disc);GEOM_TRACE_F("disc",disc);
    __filled = false;

    const Vector3& ll = __voxel->getLowerLeftCorner();
    const Vector3& ur = __voxel->getUpperRightCorner();

    Vector2 pmin(min(fabs(ll.x()),fabs(ur.x())),
                 min(fabs(ll.y()),fabs(ur.y())));

    if( ll.x() <= 0 && ur.x() >= 0 ) pmin.x() = 0;
    if( ll.y() <= 0 && ur.y() >= 0 ) pmin.y() = 0;

    if(norm(pmin)<  disc->getRadius()){
        return true;
    }
    return false;

}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( NurbsCurve2D * nurbsCurve ) {
  GEOM_DISCRETIZE(nurbsCurve);
}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( PointSet2D * pointSet ) {
  GEOM_ASSERT(pointSet);GEOM_TRACE_F("pointSet",pointSet);
  // if(__ll.z() > 0 || __ur.z() < 0) return false;
    __filled = false;
    for(Point2Array::iterator _it = pointSet->getPointList()->getBegin();
        _it != pointSet->getPointList()->getEnd();
        _it++)
        if(__voxel->intersect( *_it))return true;
    return false;}

/* ----------------------------------------------------------------------- */


bool VoxelIntersection::process( Polyline2D * polyline ) {
  GEOM_ASSERT(polyline);GEOM_TRACE_F("polyline",polyline);
//  if(__ll.z() > 0 || __ur.z() < 0) return false;
  __filled = false;
  for(Point2Array::iterator _it1 = polyline->getPointList()->getBegin();
      _it1 != polyline->getPointList()->getEnd();
      _it1++)
    if(__voxel->intersect( *_it1))return true;
  for(Point2Array::iterator _it = polyline->getPointList()->getBegin();
      _it != polyline->getPointList()->getEnd()-1;
      _it++)
    if(__voxel->intersect(*_it,*(_it+1)))return true;
  return false;
}

/* ----------------------------------------------------------------------- */


//bool VoxelIntersection::process(ScenePtr& scene){
//}

/* ----------------------------------------------------------------------- */

