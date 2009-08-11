/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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
TOOLS_USING_NAMESPACE


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

