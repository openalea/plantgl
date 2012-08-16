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

#include "intersection.h"
#include "tesselator.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/connect_holes.h>
#include <list>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2                                   Point_2;
typedef CGAL::Polygon_2<Kernel>                           Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel>                Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2>                   Pwh_list_2;

#include "cgalwrap.h"


std::pair<Point2ArrayPtr, IndexArrayPtr> internal_polygon_intersection(Polygon_2 P, Polygon_2 Q)
{
  // Compute the intersection of P and Q.
  Pwh_list_2                  intR;
  CGAL::intersection (P, Q, std::back_inserter(intR));

  IndexArrayPtr indexresult(new IndexArray());
  Point2ArrayPtr pointresult(new Point2Array());

  for(Pwh_list_2::const_iterator  itph = intR.begin(); itph != intR.end(); ++itph){
      std::list<Point_2>  outpts;
      CGAL::connect_holes (*itph, std::back_inserter (outpts));

      Point2ArrayPtr lpointresult(new Point2Array(outpts.size()));
      Point2Array::iterator itrespoints = lpointresult->begin();
      for(std::list<Point_2>::iterator itrescgalpoint = outpts.begin(); itrescgalpoint != outpts.end(); ++itrescgalpoint){
          *itrespoints = toVector2(*itrescgalpoint);
          ++itrespoints;
      }

      IndexArrayPtr lindexresult = polygonization(lpointresult, eConvexTriangulation);
      size_t nbpoints = pointresult->size();
      if (nbpoints > 0) shift_all_indices(lindexresult, nbpoints);
      indexresult->insert(indexresult->end(),lindexresult->begin(),lindexresult->end());
      pointresult->insert(pointresult->end(),lpointresult->begin(),lpointresult->end());
  }
  return std::pair<Point2ArrayPtr, IndexArrayPtr>(pointresult,indexresult);
}

std::pair<Point2ArrayPtr, IndexArrayPtr> PGL(polygon_intersection)(Point2ArrayPtr polygon1, Point2ArrayPtr polygon2)
{
// Construct the two input polygons.
  Polygon_2 P;
  for(Point2Array::const_iterator it = polygon1->begin(); it != polygon1->end(); ++it)
      P.push_back(toPoint2<Point_2>(*it));

  Polygon_2 Q;
  for(Point2Array::const_iterator it = polygon2->begin(); it != polygon2->end(); ++it)
      Q.push_back(toPoint2<Point_2>(*it));

  return internal_polygon_intersection(P,Q);
}

std::pair<Point2ArrayPtr, IndexArrayPtr> PGL(polygon_intersection)(Point2ArrayPtr points, Index polygon1, Index polygon2)
{
// Construct the two input polygons.
  Polygon_2 P;
  for(Index::const_iterator it = polygon1.begin(); it != polygon1.end(); ++it)
      P.push_back(toPoint2<Point_2>(points->getAt(*it)));

  Polygon_2 Q;
  for(Index::const_iterator it = polygon2.begin(); it != polygon2.end(); ++it)
      Q.push_back(toPoint2<Point_2>(points->getAt(*it)));

  return internal_polygon_intersection(P,Q);
}

