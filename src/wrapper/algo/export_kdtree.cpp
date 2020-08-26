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



#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/boost_python.h>
#include <plantgl/algo/grid/kdtree.h>

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;
#define bp boost::python

template<class KDTreeN>
class kdtree_func : public boost::python::def_visitor<kdtree_func<KDTreeN> >
{
    friend class boost::python::def_visitor_access;

    template <class classT>
    void visit(classT& c) const
    {
        c.def("k_closest_points", &KDTreeN::k_closest_points, (bp::arg("point"),bp::arg("k"),bp::arg("maxdist")= REAL_MAX),"Return the k closest points of point")
         .def("k_nearest_neighbors", &KDTreeN::k_nearest_neighbors,args("k"), "Return the k closest points for each point in the kdtree")
         .def("r_nearest_neighbors", &KDTreeN::r_nearest_neighbors,args("radius"), "Return points at a distance inf of radius for each point in the kdtree")
         .def("size", &KDTreeN::size, "Return the number of point in the kdtree.")
         .def("__len__", &KDTreeN::size, "Return the number of point in the kdtree.")
        ;
    }
};

#ifdef PGL_WITH_ANN

KDTree2Ptr init_kdtree2(const Point2ArrayPtr points) { return KDTree2Ptr(new ANNKDTree2(points)); }
KDTree3Ptr init_kdtree3(const Point3ArrayPtr points) { return KDTree3Ptr(new ANNKDTree3(points)); }
KDTree4Ptr init_kdtree4(const Point4ArrayPtr points) { return KDTree4Ptr(new ANNKDTree4(points)); }

#endif

void export_KDtree()
{
  class_< AbstractKDTree2, KDTree2Ptr, boost::noncopyable > ("AbstractKDTree2", no_init )
     .def(kdtree_func<AbstractKDTree2>());

  class_< AbstractKDTree3, KDTree3Ptr, boost::noncopyable > ("AbstractKDTree3", no_init )
     .def(kdtree_func<AbstractKDTree3>());

  class_< AbstractKDTree4, KDTree4Ptr, boost::noncopyable > ("AbstractKDTree4", no_init )
     .def(kdtree_func<AbstractKDTree4>());

#ifdef PGL_WITH_ANN

  class_< ANNKDTree2, ANNKDTree2Ptr, bases<AbstractKDTree2>, boost::noncopyable >
      ("ANNKDTree2", init<Point2ArrayPtr>("Construct a KD-Tree from a set of 2D points.") );
  implicitly_convertible< ANNKDTree2Ptr, KDTree2Ptr >();

  class_< ANNKDTree3, ANNKDTree3Ptr, bases<AbstractKDTree3>, boost::noncopyable >
      ("ANNKDTree3", init<Point3ArrayPtr>("Construct a KD-Tree from a set of 3D points.") );
  implicitly_convertible< ANNKDTree3Ptr, KDTree3Ptr >();

  class_< ANNKDTree4, ANNKDTree4Ptr, bases<AbstractKDTree4>, boost::noncopyable >
      ("ANNKDTree4", init<Point4ArrayPtr>("Construct a KD-Tree from a set of 4D points.") );
  implicitly_convertible< ANNKDTree4Ptr, KDTree4Ptr >();

  def("KDTree2", init_kdtree2, args("points"), "Construct a KD-Tree from a set of 2D points.");
  def("KDTree3", init_kdtree3, args("points"), "Construct a KD-Tree from a set of 3D points.");
  def("KDTree4", init_kdtree4, args("points"), "Construct a KD-Tree from a set of 4D points.");

#endif
}


