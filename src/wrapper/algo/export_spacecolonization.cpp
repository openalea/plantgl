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

#include <plantgl/python/boost_python.h>
#include <plantgl/python/export_refcountptr.h>
#include <plantgl/algo/modelling/spacecolonization.h>
#include <plantgl/python/export_list.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;
#define bp boost::python

#define INHERIT_SIMPLE_FUNC(funcname) \
        virtual void funcname(size_t pid) \
        {  \
          if(bp::override f = this->get_override(#funcname)) \
		    bp::call<void>(f.ptr(), pid); \
          else return default_##funcname(pid); \
	    } \
        void default_##funcname(size_t pid) \
        { \
	      SpaceColonization::funcname(pid); \
	    } \

class PySpaceColonization : public SpaceColonization, public bp::wrapper<SpaceColonization>
{
public:
        PySpaceColonization(const Point3ArrayPtr _attractors,                           
                          real_t nodelength,
                          real_t kill_radius,
                          real_t perception_radius,
                          const Point3ArrayPtr initialskeletonnodes = Point3ArrayPtr(0), 
                          const Uint32ArrayPtr  initialskeletonparent = Uint32ArrayPtr(0),
                          const Index& _active_nodes = Index(0),
                          size_t spacetilingratio = 100):
            SpaceColonization(_attractors,nodelength, kill_radius, perception_radius, initialskeletonnodes, initialskeletonparent, _active_nodes, spacetilingratio),
                bp::wrapper<SpaceColonization>() { }

        PySpaceColonization(const Point3ArrayPtr _attractors,                           
                          real_t nodelength,
                          real_t kill_radius,
                          real_t perception_radius,
                          const TOOLS(Vector3)& rootnode, 
                          size_t spacetilingratio = 100):
            SpaceColonization(_attractors,nodelength, kill_radius, perception_radius, rootnode, spacetilingratio),
                bp::wrapper<SpaceColonization>() { }

            INHERIT_SIMPLE_FUNC(generate_buds);
            INHERIT_SIMPLE_FUNC(node_buds_preprocess);
            INHERIT_SIMPLE_FUNC(node_buds_postprocess);


    void py_add_bud(size_t pid, const TOOLS(Vector3)& direction, const Index& attractors){
        add_bud(pid, direction, AttractorList(attractors.begin(),attractors.end()));
    }

    bp::object py_lateral_directions(const TOOLS(Vector3)& dir, real_t angle, int nb){
        return make_list(lateral_directions(dir, angle, nb));
    }

};


typedef RCPtr<PySpaceColonization> PySpaceColonizationPtr;

void export_SpaceColonization()
{
      class_< PySpaceColonization, PySpaceColonizationPtr, bases<RefCountObject>, boost::noncopyable > 
        ("SpaceColonization", init<Point3ArrayPtr, real_t , real_t , real_t , Point3ArrayPtr,  Uint32Array1Ptr, Index, size_t >("Construct a SpaceColonization.",
                          (bp::arg("attractors"),bp::arg("nodelength"),bp::arg("kill_radius"),bp::arg("perception_radius"),bp::arg("initialskeletonnodes")=Point3ArrayPtr(0),
                           bp::arg("initialskeletonparent")=Uint32Array1Ptr(0),bp::arg("active")=Index(),bp::arg("spacetilingratio")=100) ))
        .def(init<Point3ArrayPtr, real_t , real_t , real_t , Vector3, size_t >("Construct a SpaceColonization.",
                          (bp::arg("attractors"),bp::arg("nodelength"),bp::arg("kill_radius"),bp::arg("perception_radius"),bp::arg("rootnode"),bp::arg("spacetilingratio")=100) ))
        .def("run",&SpaceColonization::run, "Apply as many step as it can")
        .def("step",&SpaceColonization::step, "First generate all buds, and then grow them")
        .def("growth",&SpaceColonization::growth, "Growth the current bud")
        .def("generate_all_buds",&SpaceColonization::generate_all_buds)
        .add_property("nodes",&SpaceColonization::get_nodes)
        .add_property("parents",&SpaceColonization::get_parents)
        .def("get_children",&SpaceColonization::get_children)
        .def("add_node",&SpaceColonization::add_node,(bp::arg("position"),bp::arg("parent")=SpaceColonization::NOID,bp::arg("active")=true))
        .def("active_nodes",&SpaceColonization::get_active_nodes,return_value_policy<return_by_value>())
        .def("activate_node",&SpaceColonization::activate_node)
        .def("activate_all",&SpaceColonization::activate_all)
        .def("activate_leaves",&SpaceColonization::activate_leaves)
        .def("desactivate_node",&SpaceColonization::desactivate_node)
        .add_property("grid",&SpaceColonization::get_grid)
        .def("add_bud", &PySpaceColonization::py_add_bud)
        .def("try_to_set_bud", &SpaceColonization::try_to_set_bud)
        .def("node_position", &SpaceColonization::node_position,return_value_policy<return_by_value>())
        .def("node_direction", &SpaceColonization::node_direction)
        .def("generate_buds", &SpaceColonization::generate_buds, &PySpaceColonization::default_generate_buds)
        .def("node_buds_preprocess", &SpaceColonization::node_buds_preprocess, &PySpaceColonization::default_node_buds_preprocess)
        .def("node_buds_postprocess", &SpaceColonization::node_buds_postprocess, &PySpaceColonization::default_node_buds_postprocess)
        .def("lateral_directions", &PySpaceColonization::py_lateral_directions)
        
        .add_property("nodelength",make_getter(&SpaceColonization::nodelength),make_setter(&SpaceColonization::nodelength))
        .add_property("kill_radius",make_getter(&SpaceColonization::kill_radius),make_setter(&SpaceColonization::kill_radius))
        .add_property("perception_radius",make_getter(&SpaceColonization::perception_radius),make_setter(&SpaceColonization::perception_radius))
        .add_property("coneangle",make_getter(&SpaceColonization::coneangle),make_setter(&SpaceColonization::coneangle))
        .add_property("min_nb_pt_per_bud",make_getter(&SpaceColonization::min_nb_pt),make_setter(&SpaceColonization::min_nb_pt))
        .def("setLengths", &SpaceColonization::setLengths,(bp::arg("node_length"),bp::arg("kill_radius_ratio") = 0.9,bp::arg("perception_radius_ratio") = 2.0))

        ;
      
      implicitly_convertible< SpaceColonizationPtr, RefCountObjectPtr >();
}

