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



#include <plantgl/algo/fitting/skeleton.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include <plantgl/python/extract_list.h>
#include <plantgl/python/exception.h>

#include <boost/python.hpp>
#include <string>
#include <sstream>

#include <plantgl/python/export_list.h>

using namespace boost::python;
#define bp boost::python

PGL_USING_NAMESPACE


object
pyGetChordalAxisTransform
(Polyline2DPtr shape, double areaMaxFilter)
{
  std::list<Polyline2DPtr> rlist = Skeleton::getChordalAxisTransform(shape, areaMaxFilter);
  return make_list< std::list<Polyline2DPtr> >(rlist)();
}

TriangleSetPtr
pyGetDelaunayConstrained2DTriangulation
(Polyline2DPtr shape)
{
  return Skeleton::getDelaunayConstrained2DTriangulation(shape);
}

Polyline2DPtr
pyRemoveLoopsInShape
(Polyline2DPtr shape)
{
  return Skeleton::removeLoopsInShape(shape);
}

object
pyGetSkeletonInformation
(Polyline2DPtr shape, double areaMaxFilter)
{
  std::list<Vector2> ends;
  std::list<Vector2> end_tgts;
  std::list<Vector2> bumps_ends;
  std::list<Vector2> bumps_tgts;
  std::list<Polyline2DPtr> polyline_bumps;
  std::list<Polyline2DPtr> skeleton;
  skeleton = Skeleton::getSkeletonInformation(shape,
                          areaMaxFilter,
                          &ends,
                          &end_tgts,
                          &bumps_ends,
                          &bumps_tgts,
                          &polyline_bumps);
  return make_tuple(make_list< std::list<Polyline2DPtr> >(skeleton)(),
            make_list< std::list<Vector2> >(ends)(),
            make_list< std::list<Vector2> >(end_tgts)(),
            make_list< std::list<Vector2> >(bumps_ends)(),
            make_list< std::list<Vector2> >(bumps_tgts)(),
            make_list< std::list<Polyline2DPtr> >(polyline_bumps)());
  //  return make_list< std::list<Polyline2DPtr> >(skeleton)();
}

void export_Skeleton()
{
  class_<Skeleton, boost::noncopyable>("Skeleton",
                       "Skeleton defines a static method to get the skeleton of a shape defined by a polyline with chordal axis transform. See prasad 97 for details on chordal axis transform\n" )
    .def("getChordalAxisTransform", &pyGetChordalAxisTransform)
    .staticmethod("getChordalAxisTransform")
    .def("getSkeletonInformation", &pyGetSkeletonInformation)
    .staticmethod("getSkeletonInformation")
    .def("removeLoopsInShape", &pyRemoveLoopsInShape)
    .staticmethod("removeLoopsInShape")
    .def("getDelaunayConstrained2DTriangulation", &pyGetDelaunayConstrained2DTriangulation)
    .staticmethod("getDelaunayConstrained2DTriangulation");
}

