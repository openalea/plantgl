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


#include "pointmanipulation.h"


#ifdef WITH_CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
// #include <CGAL/Triangulation_3.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_3<uint32_t, K>    Vb;
typedef CGAL::Triangulation_data_structure_3<Vb>                    Tds;

typedef CGAL::Delaunay_triangulation_3<K, Tds>                      Triangulation;
// typedef CGAL::Triangulation_3<K,Tds>      Triangulation;


typedef Triangulation::Cell_handle    Cell_handle;
typedef Triangulation::Vertex_handle  Vertex_handle;
typedef Triangulation::Locate_type    Locate_type;
typedef Triangulation::Point          Point;
typedef Triangulation::Segment        Segment;

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

inline Point toPoint(const Vector3& v) { return Point(v.x(),v.y(),v.z()); }
inline Vector3 toVector3(const Point& v) { return Vector3(v.x(),v.y(),v.z()); }

#endif

IndexArrayPtr 
PGL::delaunay_point_connection(const Point3ArrayPtr points)
{
#ifdef WITH_CGAL
    Triangulation triangulation;
    uint32_t pointCount = 0;
    for (Point3Array::const_iterator it = points->begin(); it != points->end(); ++it)
        triangulation.insert(toPoint(*it))->info() = pointCount++;


    IndexArrayPtr result(new IndexArray(points->size()));
    for(Triangulation::Finite_edges_iterator it = triangulation.finite_edges_begin();
        it != triangulation.finite_edges_end(); ++it){
            uint32_t source = it->first->vertex(it->second)->info();
            uint32_t target = it->first->vertex(it->third)->info();
            result->getAt(source).push_back(target);
            result->getAt(target).push_back(source);
    } 
#else
    PointConnectionMap result;
#endif
    return result;
}


struct PointSorter {
    PointSorter(Point3ArrayPtr _points, uint32_t _ref) : points(_points), refpoint(_points->getAt(_ref)) {}
    Point3ArrayPtr points;
    const Vector3& refpoint;

    bool operator()(uint32_t i, uint32_t j){
        return norm(refpoint-points->getAt(i)) <  norm(refpoint-points->getAt(j));  
    }
};

 IndexArrayPtr 
PGL::k_closest_points_from_delaunay(const Point3ArrayPtr points, size_t k)
{
    IndexArrayPtr res = delaunay_point_connection(points);
    IndexArrayPtr filteredres(new IndexArray(res->size()));
    IndexArray::iterator ittarget = filteredres->begin();
    uint32_t pointid = 0;
    for (IndexArray::iterator it = res->begin(); it != res->end(); ++it, ++ittarget){
        if (it->size() <= k) *ittarget = *it;
        else {
            std::sort(it->begin(), it->end(),PointSorter(points,pointid));
            *ittarget = Index(it->begin(),it->begin()+k);
        }
        ++pointid;
    }
    return filteredres;
}

#include "dijkstra.h"

Index
PGL::r_neighboorhood(uint32_t pid, const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const real_t radius)
{
    struct PointDistance {
        const Point3ArrayPtr points;
        real_t operator()(uint32_t a, uint32_t b) const { return norm(points->getAt(a)-points->getAt(b)); }
        PointDistance(const Point3ArrayPtr _points) : points(_points) {}
    };

    struct PointDistance pdevaluator(points);

    NodeList lneighborhood = dijkstra_shortest_paths_in_a_range(adjacencies,pid,pdevaluator,radius);
    Index result;
    for(NodeList::const_iterator itn = lneighborhood.begin(); itn != lneighborhood.end(); ++itn)
          result.push_back(itn->id);
    return result;
}

IndexArrayPtr 
PGL::r_neighboorhoods(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const RealArrayPtr radii)
{
    struct PointDistance {
        const Point3ArrayPtr points;
        real_t operator()(uint32_t a, uint32_t b) const { return norm(points->getAt(a)-points->getAt(b)); }
        PointDistance(const Point3ArrayPtr _points) : points(_points) {}
    };

    struct PointDistance pdevaluator(points);
    uint32_t current = 0;
    IndexArrayPtr result(new IndexArray(points->size()));
    for(RealArray::const_iterator itradii = radii->begin(); itradii != radii->end(); ++itradii) {
        NodeList lneighborhood = dijkstra_shortest_paths_in_a_range(adjacencies,current,pdevaluator,*itradii);
        Index lres;
        for(NodeList::const_iterator itn = lneighborhood.begin(); itn != lneighborhood.end(); ++itn)
            lres.push_back(itn->id);
        result->setAt(current,lres);
    }
    return result;
}
