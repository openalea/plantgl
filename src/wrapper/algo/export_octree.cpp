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



#include <plantgl/algo/grid/octree.h>
#include <plantgl/algo/raycasting/ray.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_list.h>
#include <plantgl/python/export_property.h>

PGL_USING_NAMESPACE
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
          init<const ScenePtr&,boost::python::optional< uint_t,uint_t, Octree::ConstructionMethod> >
              ("Octree(scene,maxscale,maxelements,method)",args("scene","maxscale","maxelements","method")))
     .def(init<const ScenePtr&,const Vector3&, const Vector3&,
              boost::python::optional<uint_t,uint_t,Octree::ConstructionMethod> >
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

