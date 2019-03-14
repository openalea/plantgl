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
#include "pointmanipulation.h"
#include <plantgl/algo/grid/kdtree.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/geometry/pointset.h>

PGL_USING_NAMESPACE

#ifdef WITH_CGAL

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


std::pair<Point2ArrayPtr, IndexArrayPtr> internal_polygon2ds_intersection(Polygon_2 P, Polygon_2 Q)
{
  IndexArrayPtr indexresult(new IndexArray());
  Point2ArrayPtr pointresult(new Point2Array());

  // Compute the intersection of P and Q.
  Pwh_list_2                  intR;
  CGAL::intersection (P, Q, std::back_inserter(intR));

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
#else
#define RETURN_NONE return std::pair<Point2ArrayPtr, IndexArrayPtr>(Point2ArrayPtr(),IndexArrayPtr());
#endif

std::pair<Point2ArrayPtr, IndexArrayPtr> PGL(polygon2ds_intersection)(Point2ArrayPtr polygon1, Point2ArrayPtr polygon2)
{
#ifdef WITH_CGAL
  // Construct the two input polygons.
  Polygon_2 P;
  for(Point2Array::const_iterator it = polygon1->begin(); it != polygon1->end(); ++it)
      P.push_back(toPoint2<Point_2>(*it));

  Polygon_2 Q;
  for(Point2Array::const_iterator it = polygon2->begin(); it != polygon2->end(); ++it)
      Q.push_back(toPoint2<Point_2>(*it));

  return internal_polygon2ds_intersection(P,Q);
#else
  RETURN_NONE
#endif
}

std::pair<Point2ArrayPtr, IndexArrayPtr> PGL(polygon2ds_intersection)(Point2ArrayPtr points, Index polygon1, Index polygon2)
{
#ifdef WITH_CGAL
  // Construct the two input polygons.
  Polygon_2 P;
  for(Index::const_iterator it = polygon1.begin(); it != polygon1.end(); ++it)
      P.push_back(toPoint2<Point_2>(points->getAt(*it)));

  Polygon_2 Q;
  for(Index::const_iterator it = polygon2.begin(); it != polygon2.end(); ++it)
      Q.push_back(toPoint2<Point_2>(points->getAt(*it)));

  return internal_polygon2ds_intersection(P,Q);
#else
  RETURN_NONE
#endif
}

/*
#ifdef WITH_CGAL
#include <CGAL/Gmpz.h>
#include <CGAL/Extended_homogeneous.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>

// typedef CGAL::Simple_cartesian<real_t>     PolyKernel;
typedef CGAL::Extended_homogeneous<CGAL::Gmpz>  PolyKernel;
typedef CGAL::Polyhedron_3<PolyKernel>     Polyhedron;
typedef CGAL::Nef_polyhedron_3<PolyKernel>     Nef_polyhedron;
typedef Polyhedron::Point_3                PolyPoint;
typedef PolyKernel::Plane_3  Plane_3;

Polyhedron make_cube_3(const Vector3& bbmin, const Vector3& bbmax) {
    /// Adapted from http://www.cgal.org/Manual/latest/doc_html/cgal_manual/Polyhedron/Chapter_main.html section 25.3.7
    Polyhedron P;
    CGAL_precondition( P.is_valid());
    typedef Polyhedron::Halfedge_handle Halfedge_handle;
    Halfedge_handle h = P.make_tetrahedron( PolyPoint( bbmax.x(), bbmin.y(), bbmin.z()),
                                            PolyPoint( bbmin.x(), bbmin.y(), bbmax.z()),
                                            PolyPoint( bbmin.x(), bbmin.y(), bbmin.z()),
                                            PolyPoint( bbmin.x(), bbmax.y(), bbmin.z()));
    Halfedge_handle g = h->next()->opposite()->next();             // Fig. (a)
    P.split_edge( h->next());
    P.split_edge( g->next());
    P.split_edge( g);                                              // Fig. (b)
    h->next()->vertex()->point()     = PolyPoint( bbmax.x(), bbmin.y(), bbmax.z());
    g->next()->vertex()->point()     = PolyPoint( bbmin.x(), bbmax.y(), bbmax.z());
    g->opposite()->vertex()->point() = PolyPoint( bbmax.x(), bbmax.y(), bbmin.z());            // Fig. (c)
    Halfedge_handle f = P.split_facet( g->next(),
                                       g->next()->next()->next()); // Fig. (d)
    Halfedge_handle e = P.split_edge( f);
    e->vertex()->point() = PolyPoint( bbmax.x(), bbmax.y(), bbmax.z());                        // Fig. (e)
    P.split_facet( e, f->next()->next());                          // Fig. (f)
    CGAL_postcondition( P.is_valid());
    return P;
}

Nef_polyhedron make_nef_cube_3(const Vector3& bbmin, const Vector3& bbmax) {
    /// Adapted from http://www.cgal.org/Manual/latest/doc_html/cgal_manual/Nef_3/Chapter_main.html#Subsection_29.5.2
    Nef_polyhedron N1(Plane_3(1,0,0, bbmin.x()));
    Nef_polyhedron N2(Plane_3(-1,0,0, -bbmax.x()));
    Nef_polyhedron N3(Plane_3(0,1,0, bbmin.y()));
    Nef_polyhedron N4(Plane_3(0,-1,0, -bbmax.y()));
    Nef_polyhedron N5(Plane_3(0,0,1, bbmin.z()));
    Nef_polyhedron N6(Plane_3(0,0,-1, -bbmax.z()));


    Nef_polyhedron Cube = N1 * N2 * N3 * N4 * N5 * N6;

    CGAL_postcondition( Cube.is_valid());
    return Cube;
}

#endif

std::pair<Point3ArrayPtr, IndexArrayPtr> 
PGL(box_triangle_intersection)(const Vector3& bbxmin, const Vector3& bbxmax, const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
#ifdef WITH_CGAL

    Nef_polyhedron N1 = make_nef_cube_3(bbxmin,bbxmax);
    printf("cube created\n");
    Polyhedron Q;
    Q.make_triangle(toPoint3<PolyPoint>(p1),toPoint3<PolyPoint>(p2),toPoint3<PolyPoint>(p3));
    printf("triangle created\n");
    Nef_polyhedron N2(Q);
    CGAL_precondition(N2.is_valid());

    Nef_polyhedron Nresult = N1 * N2;

    
    Polyhedron result;
    Nresult.convert_to_polyhedron(result);

    Point3ArrayPtr points(new Point3Array(result.size_of_vertices()));
    Point3Array::iterator itPoint = points->begin();

    Polyhedron::Vertex_const_iterator v;
    CGAL_forall_vertices(v,result) { *itPoint = toVector3(v->point()); }

    IndexArrayPtr resultingfaces;

    // CGAL_forall_facets(f,result){
    for(Polyhedron::Facet_iterator f = result.facets_begin(); f != result.facets_end(); ++f){
        Index face;
        Polyhedron::Halfedge_around_facet_circulator j = f->facet_begin(); 
        do{
            face.push_back(std::distance(result.vertices_begin(),j->vertex()));
        } while (++j != f->facet_begin());
        resultingfaces->push_back(face);
    }

    return std::pair<Point3ArrayPtr, IndexArrayPtr> (points,resultingfaces);

#else
    return std::pair<Point3ArrayPtr, IndexArrayPtr> (Point3ArrayPtr(), IndexArrayPtr());
#endif
}*/

Vector3 PGL(_plane_segment_intersection)(const Plane3& plane, 
                                    real_t d1,
                                    const Vector3& p1, 
                                    const Vector3& p2)
{
    Vector3 segdir = p2-p1;
    real_t r = (plane.getD()-d1)/ dot(plane.getNormal(), segdir);
    return p1+segdir*r;
}

bool PGL(plane_segment_intersection)(const Plane3& plane, 
                                     const Vector3& p1, 
                                     const Vector3& p2,
                                     Vector3& result)
{
    real_t d1 = dot(plane.getNormal(),p1);
    real_t d2 = dot(plane.getNormal(),p2);
    if ((d1 > plane.getD()) == (d2 > plane.getD())) return false;
    result = _plane_segment_intersection(plane, d1, p1, p2);
    return true;
}




int tri_tri_intersect_with_isectline(real_t V0[3],real_t V1[3],real_t V2[3],
                     real_t U0[3],real_t U1[3],real_t U2[3],int *coplanar,
                     real_t isectpt1[3],real_t isectpt2[3]);

#define CONVERT(vname) \
     double  vname##a[3]; \
     vname##a[0] = vname.x(); \
     vname##a[1] = vname.y(); \
     vname##a[2] = vname.z();

IntersectionType  PGL(triangle_triangle_intersection)(const Vector3& t11, const Vector3& t12, const Vector3& t13, 
                                          const Vector3& t21, const Vector3& t22, const Vector3& t23, 
                                          Vector3& intersectionstart, Vector3& intersectionend)
{
    int coplanar;
    real_t isectpt1[3];
    real_t isectpt2[3];

    CONVERT(t11); CONVERT(t12); CONVERT(t13); 
    CONVERT(t21); CONVERT(t22); CONVERT(t23); 
    
    int res = tri_tri_intersect_with_isectline(t11a,t12a,t13a,t21a,t22a,t23a, &coplanar,isectpt1, isectpt2);  
    if (res) {
        if (coplanar) return CoPlanar;
        intersectionstart = Vector3(isectpt1[0],isectpt1[1],isectpt1[2]);
        intersectionend = Vector3(isectpt2[0],isectpt2[1],isectpt2[2]);

        return Intersection;
    }
    return NoIntersection;
}

std::pair<std::vector<std::pair<uint32_t,uint32_t> >,GeometryArrayPtr> 
PGL(auto_intersection)(Point3ArrayPtr points, Index3ArrayPtr triangles)
{
#ifdef WITH_ANN
    std::vector<std::pair<uint32_t,uint32_t> > intersectionpair;
    GeometryArrayPtr intersectionresult(new GeometryArray());
    Point3ArrayPtr centroids = centroids_of_groups(points, triangles);
    real_t maxsize = 0;
    Point3Array::const_iterator itorigin = centroids->begin();
    for(Index3Array::const_iterator it = triangles->begin(); it != triangles->end(); ++it, ++itorigin ) 
    {
        real_t size = norm(*itorigin-points->getAt(it->getAt(0)));
        if (maxsize < size) maxsize = size;
    }

    ANNKDTree3 kdtree(centroids);
    IndexArrayPtr nbgs = kdtree.r_nearest_neighbors(2*maxsize);
    Vector3 intersectionstart, intersectionend;

    Index3Array::const_iterator ittr = triangles->begin();
    size_t idtr = 0;
    for (IndexArray::const_iterator itnbgs = nbgs->begin(); itnbgs != nbgs->end(); ++itnbgs, ++ittr, ++idtr)
    {
        for (Index::const_iterator itnbg = itnbgs->begin(); itnbg != itnbgs->end(); ++itnbg)
        {
            if (idtr < *itnbg){ // We want to compare only once every pair of triangles.
                const Index3& nbg = triangles->getAt(*itnbg);
                int intersection = triangle_triangle_intersection(points->getAt(ittr->getAt(0)), points->getAt(ittr->getAt(1)),points->getAt(ittr->getAt(2)),
                                                                  points->getAt(nbg.getAt(0)), points->getAt(nbg.getAt(1)),points->getAt(nbg.getAt(2)),
                                                                  intersectionstart, intersectionend);

                if (intersection != NoIntersection){
                    if (intersection == Intersection){
                        std::vector<uint32_t> common_points;
                        for(Index3::const_iterator ittrpt = ittr->begin(); ittrpt != ittr->end(); ++ittrpt )
                            for (Index3::const_iterator itnbpt = nbg.begin(); itnbpt != nbg.end(); ++itnbpt )
                                if (*ittrpt == *itnbpt) common_points.push_back(*ittrpt);

                        int nbintersection = 2;
                        if (norm(intersectionstart-intersectionend) < GEOM_EPSILON) {
                            // it is actually one point which intersect
                            nbintersection = 1;
                        }

                        /// We check here if it is not an edge or a vertex which is in common.
                        /// We should not  care about such intersection in case of auto intersection 
                        if (common_points.size() == 1 && nbintersection == 1){
                            // We test if it is a common point
                            if (norm(points->getAt(common_points[0]) - intersectionstart) < GEOM_EPSILON)
                                continue;
                            else {
                                intersectionpair.push_back(std::pair<uint32_t,uint32_t>(idtr, *itnbg));
                                intersectionresult->push_back(GeometryPtr(new PointSet(Point3ArrayPtr(new Point3Array(1,intersectionstart)))));
                            }
                        }

                        if (common_points.size() == 2){
                            // We test if it is a common edge
                            if (nbintersection == 2){
                                if ( norm(intersectionstart-points->getAt(common_points[0])) < GEOM_EPSILON ) {
                                    if ( norm(intersectionend-points->getAt(common_points[1])) < GEOM_EPSILON ) 
                                        continue;
                                }
                                else if ( norm(intersectionstart-points->getAt(common_points[1])) < GEOM_EPSILON ) {
                                     if ( norm(intersectionend-points->getAt(common_points[0])) < GEOM_EPSILON ) 
                                        continue;
                                } 
                            }
                            // This case should not occur. We test if it is a common point
                            else if (nbintersection == 1){
                                if ( norm(intersectionstart-points->getAt(common_points[0])) < GEOM_EPSILON ||
                                     norm(intersectionstart-points->getAt(common_points[1])) < GEOM_EPSILON )
                                    continue;
                            }                             
                        }

                        intersectionpair.push_back(std::pair<uint32_t,uint32_t>(idtr, *itnbg));
                        intersectionresult->push_back(GeometryPtr(new Polyline(Point3ArrayPtr(new Point3Array(intersectionstart, intersectionend)))));
                    }
                    else { // coplanar
                        intersectionpair.push_back(std::pair<uint32_t,uint32_t>(idtr, *itnbg));
                        intersectionresult->push_back(GeometryPtr());

                    }

                }
            }

        }
    }
    return std::pair<std::vector<std::pair<uint32_t,uint32_t> >,GeometryArrayPtr> (intersectionpair, intersectionresult);
#else
    #ifdef _MSC_VER
    #pragma message("function 'auto_intersection' disabled. ANN needed.")
    #else
    #warning "function 'auto_intersection' disabled. ANN needed"
    #endif
    return std::pair<std::vector<std::pair<uint32_t,uint32_t> >,GeometryArrayPtr> ();
#endif
}
