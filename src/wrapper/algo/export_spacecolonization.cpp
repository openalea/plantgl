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



#include <plantgl/algo/modelling/spacecolonization.h>
#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/boost_python.h>
#include <plantgl/python/export_list.h>

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;
#define bp boost::python

#define INHERIT_SIMPLE_FUNC1(base, funcname) \
        virtual void funcname(size_t pid) \
        {  \
          if(bp::override f = this->get_override(#funcname)) \
            bp::call<void>(f.ptr(), pid); \
          else return default_##funcname(pid); \
        } \
        void default_##funcname(size_t pid) \
        { \
          base::funcname(pid); \
        } \

#define INHERIT_SIMPLE_FUNC0(base, funcname) \
        virtual void funcname() \
        {  \
          if(bp::override f = this->get_override(#funcname)) \
            bp::call<void>(f.ptr()); \
          else return default_##funcname(); \
        } \
        void default_##funcname() \
        { \
          base::funcname(); \
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
                          const Vector3& rootnode,
                          size_t spacetilingratio = 100):
            SpaceColonization(_attractors,nodelength, kill_radius, perception_radius, rootnode, spacetilingratio),
                bp::wrapper<SpaceColonization>() { }

            INHERIT_SIMPLE_FUNC1(SpaceColonization,generate_buds);
            INHERIT_SIMPLE_FUNC1(SpaceColonization,node_buds_preprocess);
            INHERIT_SIMPLE_FUNC1(SpaceColonization,node_buds_postprocess);
            INHERIT_SIMPLE_FUNC0(SpaceColonization,StartEach);
            INHERIT_SIMPLE_FUNC0(SpaceColonization,EndEach);


    void py_add_bud(size_t pid, const Vector3& direction, const Index& attractors){
        add_bud(pid, direction, AttractorList(attractors.begin(),attractors.end()));
    }

    bp::object py_lateral_directions(const Vector3& dir, real_t angle, int nb){
        return make_list(lateral_directions(dir, angle, nb));
    }

};

typedef RCPtr<PySpaceColonization> PySpaceColonizationPtr;


class PyGraphColonization : public GraphColonization, public bp::wrapper<GraphColonization>
{
public:

        PyGraphColonization(const Point3ArrayPtr attractors,
                                  real_t perception_radius,
                                  const IndexArrayPtr graph,
                                  uint32_t root,
                                  real_t powerdistance = 1,
                                  size_t spacetilingratio = 100):
            GraphColonization(attractors, perception_radius, graph, root,powerdistance, spacetilingratio),
                bp::wrapper<GraphColonization>() { }

            INHERIT_SIMPLE_FUNC1(GraphColonization,generate_buds);
            INHERIT_SIMPLE_FUNC1(SpaceColonization,node_buds_preprocess);
            INHERIT_SIMPLE_FUNC1(SpaceColonization,node_buds_postprocess);
            INHERIT_SIMPLE_FUNC0(SpaceColonization,StartEach);
            INHERIT_SIMPLE_FUNC0(SpaceColonization,EndEach);

    void py_add_bud(size_t pid, const Index& attractors, real_t level){
        add_bud(pid, AttractorList(attractors.begin(),attractors.end()),level);
    }
};


typedef RCPtr<PyGraphColonization> PyGraphColonizationPtr;

#define BASESCA(CLASS) \
        .def("run",&CLASS::run, "Apply as many steps as it can") \
        .def("iterate",&CLASS::iterate, args("nbsteps"), "Apply a given number of steps.") \
        .def("step",&CLASS::step, "First generate all buds, and then grow them") \
        .def("growth",&CLASS::growth, "Growth the current bud") \
        .def("generate_all_buds",&CLASS::generate_all_buds) \
        .add_property("nodes",&CLASS::get_nodes) \
        .add_property("parents",&CLASS::get_parents) \
        .def("get_children",&CLASS::get_children) \
        .def("active_nodes",&CLASS::get_active_nodes,return_value_policy<return_by_value>()) \
        .def("activate_node",&CLASS::activate_node) \
        .def("activate_all",&CLASS::activate_all) \
        .def("activate_leaves",&CLASS::activate_leaves) \
        .def("desactivate_node",&CLASS::desactivate_node) \
        .add_property("grid",&CLASS::get_grid) \
        .def("try_to_set_bud", &CLASS::try_to_set_bud) \
        .def("node_position", &CLASS::node_position,return_value_policy<return_by_value>()) \
        .def("node_direction", &CLASS::node_direction) \
        .def("generate_buds", &CLASS::generate_buds, &Py##CLASS::default_generate_buds) \
        .def("node_buds_preprocess", &CLASS::node_buds_preprocess, &Py##CLASS::default_node_buds_preprocess) \
        .def("node_buds_postprocess", &CLASS::node_buds_postprocess, &Py##CLASS::default_node_buds_postprocess) \
        .def("StartEach", &CLASS::StartEach, &Py##CLASS::default_StartEach) \
        .def("EndEach", &CLASS::EndEach, &Py##CLASS::default_EndEach) \
        .def("nbLatentBud", &CLASS::nbLatentBud) \
        .def_readwrite("nodelength",&CLASS::nodelength)  \
        .def_readwrite("kill_radius",&CLASS::kill_radius) \
        .def_readwrite("perception_radius",&CLASS::perception_radius) \
        .def_readwrite("coneangle",&SpaceColonization::coneangle) \
        .def_readwrite("min_nb_pt_per_bud",&CLASS::min_nb_pt_per_bud) \
        .def_readwrite("insertion_angle",&SpaceColonization::insertion_angle) \
        .def_readwrite("nb_buds_per_whorl",&CLASS::nb_buds_per_whorl) \
        .def("setLengths", &CLASS::setLengths,(bp::arg("node_length"),bp::arg("kill_radius_ratio") = 0.9,bp::arg("perception_radius_ratio") = 2.0))


void gc_nodecomponents(GraphColonization * gc, IndexArrayPtr nodecomponents) { gc->nodecomponents = nodecomponents; }

void export_SpaceColonization()
{
      class_< PySpaceColonization, PySpaceColonizationPtr, bases<RefCountObject>, boost::noncopyable >
        ("SpaceColonization", init<Point3ArrayPtr, real_t , real_t , real_t , Point3ArrayPtr,  Uint32Array1Ptr, Index, size_t >("Construct a SpaceColonization.",
                          (bp::arg("attractors"),bp::arg("nodelength"),bp::arg("kill_radius"),bp::arg("perception_radius"),bp::arg("initialskeletonnodes")=Point3ArrayPtr(0),
                           bp::arg("initialskeletonparent")=Uint32Array1Ptr(0),bp::arg("active")=Index(),bp::arg("spacetilingratio")=100) ))
        .def(init<Point3ArrayPtr, real_t , real_t , real_t , Vector3, size_t >("Construct a SpaceColonization.",
                          (bp::arg("attractors"),bp::arg("nodelength"),bp::arg("kill_radius"),bp::arg("perception_radius"),bp::arg("rootnode"),bp::arg("spacetilingratio")=100) ))
        BASESCA(SpaceColonization)
        .def("add_node",&SpaceColonization::add_node,(bp::arg("position"),bp::arg("parent")=SpaceColonization::NOID,bp::arg("active")=true))
        .def("add_bud", &PySpaceColonization::py_add_bud)
        .def("lateral_directions", &PySpaceColonization::py_lateral_directions)
        .def("node_attractors",&GraphColonization::node_attractors,(bp::arg("pid")),return_value_policy<return_by_value>())

        ;

      implicitly_convertible< SpaceColonizationPtr, RefCountObjectPtr >();


      class_< PyGraphColonization, PyGraphColonizationPtr, boost::noncopyable >
        ("GraphColonization", init<Point3ArrayPtr, real_t , const IndexArrayPtr, uint32_t, optional<real_t, size_t> >("Construct a GraphColonization.",
                             bp::args("attractors","perception_radius","graph","root","powerdistance","spacetilingratio") ))
        BASESCA(GraphColonization)
        .def_readwrite("graph",&GraphColonization::graph)
        .def_readwrite("use_jonction_points",&GraphColonization::use_jonction_points)
        .def_readwrite("distances_from_root",&GraphColonization::distances_from_root)
        .def_readwrite("root",&GraphColonization::root)
        .add_property("nodecomponents",&GraphColonization::get_nodecomponents,&gc_nodecomponents)
        .def_readwrite("nodelevels",&GraphColonization::nodelevels)
        .def_readwrite("nodelevels",&GraphColonization::nodelevels)
        .def("add_node",&GraphColonization::add_node,(bp::arg("position"),bp::arg("level"),bp::arg("components"),bp::arg("parent")=SpaceColonization::NOID,bp::arg("active")=true))
        .def("node_level",&GraphColonization::node_level,(bp::arg("pid")))
        .def("node_components",&GraphColonization::node_components,(bp::arg("pid")),return_value_policy<return_by_value>())
        .def("add_bud",&PyGraphColonization::py_add_bud,(bp::arg("pid"),bp::arg("attractors"),bp::arg("level")))
        .def("junction_components",&GraphColonization::junction_components,(bp::arg("nodeid1"),bp::arg("nodeid2")))
        .def("junction_point",&GraphColonization::junction_point,(bp::arg("nodeid1"),bp::arg("nodeid2")))
        ;
}

