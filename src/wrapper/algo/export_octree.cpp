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

#include <plantgl/algo/grid/octree.h>
#include <plantgl/algo/raycasting/ray.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_list.h>
#include <plantgl/python/export_property.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;



DEF_POINTEE(Octree)
DEF_POINTEE(Mvs)

std::string get_mvs_name(Mvs * obj){ return obj->getName(); } 
ScenePtr get_mvs_scene(Mvs * obj){ return obj->getScene(); } 
void set_mvs_scene(Mvs * obj, ScenePtr sc){ obj->setScene(sc); } 

void export_Mvs()
{
  class_< Mvs,MvsPtr, boost::noncopyable >("Mvs", no_init)
    .def("getName", &Mvs::getName, return_value_policy< copy_const_reference >())
    .def("isNamed", &Mvs::isNamed)
    .def("setName", &Mvs::setName)
    .add_property("name",&get_mvs_name,&Mvs::setName)
    .def("isValid", &Mvs::isValid)
    .add_property("scene",&get_mvs_scene,&set_mvs_scene)
    ;
}


Vector3* intersect( Octree* self, Vector3* point, Vector3* direction )
{
  Ray ray(*point, *direction);
  Vector3 res;
  bool ok= self->intersect( ray, res );
  if( ok )
      return new Vector3( res );
  else
      return 0;
}


Vector3 get_oct_center(Octree * oc) { return oc->getCenter(); }
Vector3 get_oct_size(Octree * oc) { return oc->getSize(); }

object get_oct_details(Octree * oc) 
{ return make_list<std::vector<std::vector<uint_t> > ,
                   list_converter<std::vector<uint_t> > >
                   (oc->getDetails())(); }

object get_oct_sizes(Octree * oc) 
{ return make_list(oc->getSizes())(); }


object oct_intersect(Octree * oct, const Ray& ray) {
    Vector3 res;
    bool touch = oct->intersect(ray,res);
    if(touch)return object(res);
    else return object();
}

object oct_findfirstpoint(Octree * oct, const Ray& ray) {
    Vector3 res;
    bool touch = oct->intersect(ray,res);
    if(touch)return object(res);
    else return object();
}

void export_Octree()
{
  scope octree = class_< Octree, OctreePtr, boost::noncopyable >("Octree", 
          init<const ScenePtr&,optional< uint_t,uint_t, Octree::ConstructionMethod> >
              ("Octree(scene,maxscale,maxelements,method)",args("scene","maxscale","maxelements","method")))
     .def(init<const ScenePtr&,const Vector3&, const Vector3&, 
              optional<uint_t,uint_t,Octree::ConstructionMethod> >
              ("Octree(scene,center,size,maxscale,maxelements,method)",args("scene","center","size","maxscale","maxelements","method")))
     .add_property("center",&get_oct_center)
     .add_property("size",&get_oct_size)
     .add_property("depth",&Octree::getDepth)
     .def("getRepresentation",&Octree::getRepresentation)
     .def("getVolume",&Octree::getVolume)
     .def("getDetails",&get_oct_details)
     .def("getSizes",&get_oct_sizes)
     .def("contains",&Octree::contains)
     .def("intersection",&oct_intersect)
     .def("findFirstPoint",&oct_findfirstpoint)
    ;

  enum_<Octree::ConstructionMethod>("ConstructionMethod")
      .value("TriangleBased",Octree::TriangleBased)
      .value("ShapeBased",Octree::ShapeBased)
	  .export_values()
      ;
}

