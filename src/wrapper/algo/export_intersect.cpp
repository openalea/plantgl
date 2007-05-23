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

#include "../util/export_property.h"
#include <algo/raycasting/util_intersection.h>
#include <algo/raycasting/rayintersection.h>
#include <algo/base/discretizer.h>
#include <scenegraph/geometry/geometry.h>
#include <scenegraph/container/pointarray.h>

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;



void export_SegIntersection()
{
    def("segmentIntersect", (bool (*)(const TOOLS(Vector2)&,const TOOLS(Vector2)&,const TOOLS(Vector2)&,const TOOLS(Vector2)&))&intersectSegment,args("segA1","segA2","segB1","segB2"));
    def("segmentIntersect", (bool (*)(const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&))&intersectSegment,args("segA1","segA2","point"));
    def("segmentIntersect", (bool (*)(const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&))&intersectSegment,args("segA1","segA2","segB1","segB2"));
    def("segmentIntersect", (bool (*)(const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&))&intersectSegment,args("segA1","segA2","tr1","tr2","tr3"));
    def("segmentIntersect", (bool (*)(const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&,const TOOLS(Vector3)&))&intersectSegment,args("segA1","segA2","qd1","qd2","qd3","qd3"));

}

SETGET(Ray,Origin,Vector3)
SETGET(Ray,Direction,Vector3)

object ray_intersect_seg(Ray * ray, const Vector3& p1, const Vector3& p2)
{
    Vector3 res;
    int ret = ray->intersect(p1,p2,res);
    if (ret == 1) return object(res);
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


void export_Ray()
{
  class_< Ray > ("Ray", init<optional<const Vector3&, const Vector3&> >("Ray(Vector3 origin, Vector3 direction)", args("origin","direction") ))
      .def("isValid",&Ray::isValid)
      .DEC_SETGET("origin",Ray,Origin,Vector3)
      .DEC_SETGET("direction",Ray,Direction,Vector3)
      .def("intersect",(bool(Ray::*)(const Vector3&) const)&Ray::intersect,args("point"))
      .def("intersect",(bool(Ray::*)(const Vector2&) const)&Ray::intersect,args("point"))
      .def("intersect",ray_intersect_seg,args("p1","p2"))
      .def("intersect",ray_intersect_seg2,args("p1","p2"))
      .def("intersect",ray_intersect_tr,args("p1","p2","p3"))
      .def("intersect",ray_intersect_qd,args("p1","p2","p3","p4"))
      .def("intersect",ray_intersect_bs,args("center","radius"))
      .def("isIntersecting",(bool(Ray::*)(const BoundingBox&) const)&Ray::intersect,args("bbox"))
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

