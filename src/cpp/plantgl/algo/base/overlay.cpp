/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
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

#include "overlay.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/container/geometryarray2.h>
#include <plantgl/scenegraph/geometry/group.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/geometry/pointset.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

#ifdef WITH_CGAL
#include <CGAL/basic.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_overlay.h>
#include <CGAL/Arr_default_overlay_traits.h>
#ifdef CGAL_USE_GMP
  // GMP is installed. Use the GMP rational number-type.
  #include <CGAL/Gmpq.h>
  typedef CGAL::Gmpq                                    Number_type;
#else
  // GMP is not installed. Use CGAL's exact rational number-type.
  #include <CGAL/MP_Float.h>
  #include <CGAL/Quotient.h>
  typedef CGAL::Quotient<CGAL::MP_Float>                Number_type;
#endif

typedef CGAL::Cartesian<Number_type>                     Kernel;
typedef CGAL::Arr_segment_traits_2<Kernel>               Traits_2;
typedef Traits_2::Point_2                                Point_2;
typedef Traits_2::X_monotone_curve_2                     Segment_2;
//typedef CGAL::Arrangement_2<Traits_2>                    Arrangement_2;
//typedef CGAL::Arr_default_overlay_traits<Arrangement_2>  Overlay_traits;
typedef CGAL::Arr_face_extended_dcel<Traits_2, bool>     Dcel;
typedef CGAL::Arrangement_2<Traits_2, Dcel>              Arrangement_2;
typedef CGAL::Arr_face_overlay_traits<Arrangement_2,
                                      Arrangement_2,
                                      Arrangement_2,
                                      std::logical_and<bool> > Overlay_traits;

Vector2 toVec2(const Point_2& p) { return Vector2(to_double(p.x()),to_double(p.y())); }
#endif

bool Overlay::supportCGAL() {
#ifdef WITH_CGAL
	return true;
#else
	return false;
#endif
}

GeometryPtr 
Overlay::process(const Polyline2DPtr& p1, const Polyline2DPtr& p2)
{
	if (!p1 || !p2 || p1->getPointListSize() < 2 || p2->getPointListSize() < 2) return GeometryPtr();
#ifdef WITH_CGAL

  // Construct the first arrangement, containing a polyline 1.
  Arrangement_2          arr1;

  for (Point2Array::const_iterator it1 = p1->getPointList()->getBegin()+1; it1 != p1->getPointList()->getEnd(); ++it1)
	insert_non_intersecting_curve(arr1,Segment_2(Point_2((it1-1)->x(),(it1-1)->y()),Point_2(it1->x(),it1->y())));
  Vector2& fp1 = p1->getPointList()->getAt(0);
  Vector2& lp1 = *(p1->getPointList()->getEnd()-1);

  // to be a closed face, first and last point should be exactly the same.
  // However we should not duplicate the same point twice at the end.
  if (fp1.x() != lp1.x() || fp1.y() != lp1.y())
	insert_non_intersecting_curve(arr1,Segment_2(Point_2(lp1.x(),lp1.y()),Point_2(fp1.x(),fp1.y())));

  // std::cerr << arr1.number_of_vertices() << " " << arr1.number_of_edges() << " " << arr1.number_of_faces() << std::endl;

  // Mark just the bounded face.
  Arrangement_2::Face_iterator   fit;

  CGAL_assertion (arr1.number_of_faces() == 2);
  for (fit = arr1.faces_begin(); fit != arr1.faces_end(); ++fit)
    fit->set_data (fit != arr1.unbounded_face());

  // Construct the second arrangement, containing a rhombus-shaped face.
  Arrangement_2          arr2;

  for (Point2Array::const_iterator it2 = p2->getPointList()->getBegin()+1; it2 != p2->getPointList()->getEnd(); ++it2)
	 	insert_curve(arr2,Segment_2(Point_2((it2-1)->x(),(it2-1)->y()),Point_2(it2->x(),it2->y())));
  Vector2& fp2 = p2->getPointList()->getAt(0);
  Vector2& lp2 = *(p2->getPointList()->getEnd()-1);

  // to be a closed face, first and last point should be exactly the same.
  // However we should not duplicate the same point twice at the end.
  if (fp2.x() != lp2.x() || fp2.y() != lp2.y())
	insert_curve(arr2,Segment_2(Point_2(lp2.x(),lp2.y()),Point_2(fp2.x(),fp2.y()))); 

  // std::cerr << arr2.number_of_vertices() << " " << arr2.number_of_edges() << " " << arr2.number_of_faces() << std::endl;

  CGAL_assertion (arr2.number_of_faces() == 2);
  for (fit = arr2.faces_begin(); fit != arr2.faces_end(); ++fit)
    fit->set_data (fit != arr2.unbounded_face());

  // Compute the overlay of the two arrangements.
  Arrangement_2          overlay_arr;
  Overlay_traits         overlay_traits;

  overlay (arr1, arr2, overlay_arr, overlay_traits);

  // std::cerr << overlay_arr.number_of_vertices() << " " << overlay_arr.number_of_edges() << " " << overlay_arr.number_of_faces() << std::endl;

  // conversion between cgal structures and plantgl ones.
  GeometryArrayPtr geomarray(new GeometryArray(0));
  for (Arrangement_2::Face_iterator face = overlay_arr.faces_begin(); face != overlay_arr.faces_end(); ++face)
  {
    if (face->is_fictitious () || face->is_unbounded())
		continue;
    if (! face->data())
      continue;
   
    Arrangement_2::Ccb_halfedge_circulator curr = face->outer_ccb();
	Point2ArrayPtr pointSet( new Point2Array(1,toVec2(curr->source()->point())));
    do
    {
	  pointSet->pushBack(toVec2(curr->target()->point()));
      ++curr;
    } while (curr != face->outer_ccb());
	if (pointSet->getSize() == 1){
		geomarray->pushBack(GeometryPtr(new PointSet2D(pointSet)));
	}
	else if(pointSet->getSize() > 1){
		geomarray->pushBack(GeometryPtr(new Polyline2D(pointSet)));
	}
  }
  if (geomarray->isEmpty())return GeometryPtr();
  else if (geomarray->getSize() == 1) return geomarray->getAt(0);
  else return GeometryPtr(new Group(geomarray));

#else
#ifdef _MSC_VER
#pragma message("CGAL not included. Overlay routine will not work.")
#else
#warning "CGAL not included. Overlay routine will not work."
#endif
	return GeometryPtr();
#endif

}