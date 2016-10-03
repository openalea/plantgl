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

#include <plantgl/python/export_property.h>
#include <plantgl/algo/raycasting/util_intersection.h>
#include <plantgl/algo/raycasting/rayintersection.h>
#include <plantgl/algo/base/discretizer.h>
#include <plantgl/algo/base/intersection.h>
#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/container/pointarray.h>

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;
#define bp boost::python


void export_SegIntersection()
{
    def("segmentIntersect", (bool (*)(const TOOLS(Vector2)&,const TOOLS(Vector2)&,const TOOLS(Vector2)&,const TOOLS(Vector2)&))&intersectSegment,args("segA1","segA2","segB1","segB2"));
    def("segmentIntersect", (bool (*)(const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&))&intersectSegment,args("segA1","segA2","point"));
    def("segmentIntersect", (bool (*)(const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&))&intersectSegment,args("segA1","segA2","segB1","segB2"));
    def("segmentIntersect", (bool (*)(const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&))&intersectSegment,args("segA1","segA2","tr1","tr2","tr3"));
    def("segmentIntersect", (bool (*)(const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&))&intersectSegment,args("segA1","segA2","qd1","qd2","qd3","qd3"));

}

object ray_intersect_seg(Ray * ray, const Vector3& p1, const Vector3& p2)
{
    Vector3 res;
    int ret = ray->intersect(p1,p2,res);
    if (ret == 1) return object(res);
    else return object(ret);
}

object ray_intersect_ray(Ray * ray, const Ray& ray2)
{
    Vector3 res;
    real_t t;
    int ret = ray->intersect(ray2,res,t);
    if (ret == 1) return make_tuple(res,t);
    else return object(ret);
}

object ray_intersect_seg2(Ray * ray, const Vector2& p1, const Vector2& p2)
{
    Vector2 res;
    int ret = ray->intersect(p1,p2,res);
    if (ret == 1) return object(res);
    else return object(ret);
}

object ray_intersect_tr(Ray * ray, const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
    Vector3 res;
    int ret = ray->intersect(p1,p2,p3,res);
    if (ret == 1) return object(res);
    else return object(ret);
}

object ray_intersect_qd(Ray * ray, const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4)
{
    Vector3 res;
    int ret = ray->intersect(p1,p2,p3,p4,res);
    if (ret == 1) return object(res);
    else return object(ret);
}

object ray_intersect_bs(Ray * ray, const Vector3& center, real_t radius)
{
    Vector3 res1, res2;
    int ret = ray->intersect(center,radius,res1,res2);
    if (ret == 1) return object(res1);
    if (ret == 2) return make_tuple(res1,res2);
    else return object(ret);
}

object ray_intersect_bbx(Ray * ray, const BoundingBox& bbox)
{
    real_t tnear,tfar;
    if (ray->intersect(bbox,tnear,tfar))
        return make_tuple(tnear,tfar);
    else return object();
}


void export_Ray()
{
  class_< Ray > ("Ray", init<optional<const Vector3&, const Vector3&> >("Ray(Vector3 origin, Vector3 direction)", args("origin","direction") ))
      .def("isValid",&Ray::isValid)
      .DEC_CT_PROPERTY(origin,Ray,Origin,Vector3)
      .DEC_CT_PROPERTY(direction,Ray,Direction,Vector3)
      .def("__call__",&Ray::getAt,args("t"))
      .def("getAt",&Ray::getAt,args("t"))
      .def("intersect",(bool(Ray::*)(const Vector3&) const)&Ray::intersect,args("point"))
      .def("intersect",(bool(Ray::*)(const Vector2&) const)&Ray::intersect,args("point"))
      .def("intersect",ray_intersect_ray,args("ray"))
      .def("intersect",ray_intersect_seg,args("p1","p2"))
      .def("intersect",ray_intersect_seg2,args("p1","p2"))
      .def("intersect",ray_intersect_tr,args("p1","p2","p3"))
      .def("intersect",ray_intersect_qd,args("p1","p2","p3","p4"))
      .def("intersect",ray_intersect_bs,args("center","radius"))
      .def("intersect",ray_intersect_bbx,args("bbox"))
      //.def("isIntersecting",(bool(Ray::*)(const BoundingBox&) const)&Ray::intersect,args("bbox"))
      .def("isIntersecting",(bool(Ray::*)(const BoundingSphere&) const)&Ray::intersect,args("bsphere"))
      ;
}


void set_ray( RayIntersection* self, Vector3* point, Vector3* direction )
{
  Ray ray(*point, *direction);
  self->setRay(ray);
}

void run( RayIntersection* self, GeometryPtr geom )
{ 
  geom->apply(*self);
}

void export_RayIntersection()
{
  class_< RayIntersection, bases < Action > > ("RayIntersection", init<Discretizer&>("RayIntersection(Discretizer d)", args("discretizer")) )
    .add_property("intersection",&RayIntersection::getIntersection, "Return the list of result points" )
    .def("setRay",&set_ray,"setRay( Vector3 point, Vector3 direction)",args("point", "direction"))
    .def("setRay",&RayIntersection::setRay,"setRay( Ray ray )",args("ray"))
    .def("run",&run, "run(geom) - Compute intersections between a ray and the geometry.")
    ;
}


object py_polygon2ds_intersection_1(Point2ArrayPtr polygon1, Point2ArrayPtr polygon2)
{
    std::pair<Point2ArrayPtr, IndexArrayPtr> result = polygon2ds_intersection(polygon1, polygon2);
    return make_tuple(result.first,result.second);
}

object py_polygon2ds_intersection_2(Point2ArrayPtr points, Index polygon1, Index polygon2)
{
    std::pair<Point2ArrayPtr, IndexArrayPtr> result = polygon2ds_intersection(points, polygon1, polygon2);
    return make_tuple(result.first,result.second);
}

object py_triangle_triangle_intersection(const TOOLS(Vector3)& t11, const TOOLS(Vector3)& t12, const TOOLS(Vector3)& t13, 
                                      const TOOLS(Vector3)& t21, const TOOLS(Vector3)& t22, const TOOLS(Vector3)& t23)
{
   Vector3 intersectionstart, intersectionend;
   IntersectionType res = triangle_triangle_intersection(t11, t12, t13, t21, t22, t23, intersectionstart,intersectionend);
   if (res == NoIntersection) return object();
   if (res == Intersection) return make_tuple(intersectionstart, intersectionend);
   else return object(1);

}



void export_Intersection()
{
    def("polygon2ds_intersection",&py_polygon2ds_intersection_1, "Compute intersection between two 2D polygons.", bp::args("polygon1", "polygon1"));
    def("polygon2ds_intersection",&py_polygon2ds_intersection_2, "Compute intersection between two 2D polygons.", bp::args("points", "polygon1", "polygon1"));
    def("triangle_triangle_intersection", &py_triangle_triangle_intersection);
}

