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
#include <plantgl/algo/grid/kdtree.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
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
	     .def("size", &KDTreeN::size, "Return the number of point in the kdtree.")
	     .def("__len__", &KDTreeN::size, "Return the number of point in the kdtree.")
        ;
    }
};

void export_KDtree()
{
  class_< KDTree2, KDTree2Ptr, boost::noncopyable > ("KDTree2", no_init )
	 .def(kdtree_func<KDTree2>());

  class_< KDTree3, KDTree3Ptr, boost::noncopyable > ("KDTree3", no_init )
	 .def(kdtree_func<KDTree3>());

  class_< KDTree4, KDTree4Ptr, boost::noncopyable > ("KDTree4", no_init )
	 .def(kdtree_func<KDTree4>());

#ifdef WITH_ANN

  class_< ANNKDTree2, ANNKDTree2Ptr, bases<KDTree2>, boost::noncopyable > 
      ("ANNKDTree2", init<Point2ArrayPtr>("Construct a KD-Tree from a set of 2D points.") );
  implicitly_convertible< ANNKDTree2Ptr, KDTree2Ptr >();

  class_< ANNKDTree3, ANNKDTree3Ptr, bases<KDTree3>, boost::noncopyable > 
      ("ANNKDTree3", init<Point3ArrayPtr>("Construct a KD-Tree from a set of 3D points.") );
  implicitly_convertible< ANNKDTree3Ptr, KDTree3Ptr >();

  class_< ANNKDTree4, ANNKDTree4Ptr, bases<KDTree4>, boost::noncopyable > 
      ("ANNKDTree4", init<Point4ArrayPtr>("Construct a KD-Tree from a set of 4D points.") );
  implicitly_convertible< ANNKDTree4Ptr, KDTree4Ptr >();

#endif
}


