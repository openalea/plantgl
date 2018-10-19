/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2018 - Cirad/Inra/Inria
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://github.com/openalea/plantgl
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


/* ----------------------------------------------------------------------- */

#include "depthsortengine.h"
#include "projection_util.h"
#include <plantgl/scenegraph/geometry/boundingbox.h>
#include <plantgl/scenegraph/geometry/triangleset.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/algo/base/planeclipping.h>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

DepthSortEngine::DepthSortEngine() :
    __camera(0)
{
    setOrthographicCamera(-1, 1, -1, 1, -1, 1);
    lookAt(Vector3(0,1,0),Vector3(0,0,0),Vector3(0,0,1));

}

DepthSortEngine::~DepthSortEngine()
{

}

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <list>


struct FaceInfo2
{
  FaceInfo2(){}
  int nesting_level;
  bool in_domain(){ 
    return nesting_level%2 == 1;
  }
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2                                   Point_2;
typedef CGAL::Polygon_2<Kernel>                           Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel>                Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2>                   Pwh_list_2;

typedef CGAL::Triangulation_vertex_base_2<Kernel>                      Vb;
typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo2,Kernel>    Fbb;
typedef CGAL::Constrained_triangulation_face_base_2<Kernel,Fbb>        Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>                    TDS;
typedef CGAL::Exact_predicates_tag                                     Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<Kernel, TDS, Itag>  CDT;


inline Point_2 toPoint2(const Vector3& v) { return Point_2(v.x(),v.y()); }
inline Vector2 toVector2(const Point_2& v) { return Vector2(v.x(),v.y()); }


Polygon_2 toPolygon(Point3ArrayPtr polygon) {
    Polygon_2 pol;
    for (Point3Array::const_iterator it = polygon->begin(); it != polygon->end(); ++it){
        pol.push_back(toPoint2(*it));
    }
    return pol;
}


void 
mark_domains(CDT& ct, 
             CDT::Face_handle start, 
             int index, 
             std::list<CDT::Edge>& border )
{
  if(start->info().nesting_level != -1){
    return;
  }
  std::list<CDT::Face_handle> queue;
  queue.push_back(start);
  while(! queue.empty()){
    CDT::Face_handle fh = queue.front();
    queue.pop_front();
    if(fh->info().nesting_level == -1){
      fh->info().nesting_level = index;
      for(int i = 0; i < 3; i++){
        CDT::Edge e(fh,i);
        CDT::Face_handle n = fh->neighbor(i);
        if(n->info().nesting_level == -1){
          if(ct.is_constrained(e)) border.push_back(e);
          else queue.push_back(n);
        }
      }
    }
  }
}

//explore set of facets connected with non constrained edges,
//and attribute to each such set a nesting level.
//We start from facets incident to the infinite vertex, with a nesting
//level of 0. Then we recursively consider the non-explored facets incident 
//to constrained edges bounding the former set and increase the nesting level by 1.
//Facets in the domain are those with an odd nesting level.
void
mark_domains(CDT& cdt)
{
  printf("mark outside\n");  
  for(CDT::All_faces_iterator it = cdt.all_faces_begin(); it != cdt.all_faces_end(); ++it){
    it->info().nesting_level = -1;
  }
  printf("mark borders\n");  
  std::list<CDT::Edge> border;
  mark_domains(cdt, cdt.infinite_face(), 0, border);
  while(! border.empty()){
    printf("traverse edge\n");  
    CDT::Edge e = border.front();
    border.pop_front();
    CDT::Face_handle n = e.first->neighbor(e.second);
    if(n->info().nesting_level == -1){
      mark_domains(cdt, n, e.first->info().nesting_level+1, border);
    }
  }
}

std::list<Polygon_2> triangulate(const Polygon_with_holes_2& polygon) 
{
    std::list<Polygon_2> result;
    printf("create triangulation\n");
    bool hashole = (polygon.holes_begin() != polygon.holes_end());
    // if (!hashole) {
        if (polygon.outer_boundary().size() == 3 && !hashole){
                Polygon_2 p;
                for(auto itP = polygon.outer_boundary().vertices_begin(); itP != polygon.outer_boundary().vertices_end(); ++itP)
                    p.push_back(*itP);
                result.push_back(p);

        }
/*        else {
            CDT triangulation;
            triangulation.insert_constraint(polygon.outer_boundary().vertices_begin(), polygon.outer_boundary().vertices_end(), true);
            for (CDT::Finite_faces_iterator fit=triangulation.finite_faces_begin();
                                            fit!=triangulation.finite_faces_end();++fit)
            {
                Polygon_2 p;
                for (int i = 0; i < 3; ++i)
                    p.push_back(fit->vertex(i)->point());
                result.push_back(p);
            }

        }
    */
    //}
    else {
        CDT triangulation;
        triangulation.insert_constraint(polygon.outer_boundary().vertices_begin(), polygon.outer_boundary().vertices_end(), true);
        for(auto itP = polygon.outer_boundary().vertices_begin(); itP != polygon.outer_boundary().vertices_end(); ++itP)
            printf("p:%f %f\n",itP->x(),itP->y());
        
        for (typename Polygon_with_holes_2::Hole_const_iterator itH = polygon.holes_begin(); itH != polygon.holes_end(); ++itH){
            triangulation.insert_constraint(itH->vertices_begin(), itH->vertices_end(), true);
            for(auto itP = itH->vertices_begin(); itP != itH->vertices_end(); ++itP)
                printf("pH:%f %f\n",itP->x(),itP->y());
        }

        printf("mark domain\n");
        //Mark facets that are inside the domain bounded by the polygon
        mark_domains(triangulation);

        printf("retrieve triangles\n");
        for (CDT::Finite_faces_iterator fit=triangulation.finite_faces_begin();
                                        fit!=triangulation.finite_faces_end();++fit)
        {
            if ( fit->info().in_domain() ) {
                Polygon_2 p;
                for (int i = 0; i < 3; ++i)
                    p.push_back(fit->vertex(i)->point());
                result.push_back(p);
            }
        }
    }
    return result;

}

std::list<Polygon_2> triangulate(const Pwh_list_2& polygons) 
{
    std::list<Polygon_2> result;
    for(Pwh_list_2::const_iterator it = polygons.begin(); it != polygons.end(); ++it)
    {
        std::list<Polygon_2> cres = triangulate(*it);
        result.insert(result.end(), cres.begin(), cres.end());
    }
    return result;
}


Point3ArrayPtr project(const Polygon_2& proj, Point3ArrayPtr polygon)
{
    Point3ArrayPtr result(new Point3Array());

    real_t area = edgeFunction(polygon->getAt(0), polygon->getAt(1), polygon->getAt(2));

    for (Polygon_2::Vertex_iterator it = proj.vertices_begin() ; it != proj.vertices_end(); ++it)
    {
            Vector2 vertex = toVector2(*it);
            real_t w0 = edgeFunction(polygon->getAt(1), polygon->getAt(2), vertex) / area;
            real_t w1 = edgeFunction(polygon->getAt(2), polygon->getAt(0), vertex) / area;
            real_t w2 = edgeFunction(polygon->getAt(0), polygon->getAt(1), vertex) / area;

            result->push_back(polygon->getAt(0)*w0 + polygon->getAt(1)*w1 + polygon->getAt(2)*w2);
    }
    return result;
}

std::vector<Point3ArrayPtr> project(const std::list<Polygon_2>& projs, Point3ArrayPtr polygon)
{
    std::vector<Point3ArrayPtr> result;   
    for(std::list<Polygon_2>::const_iterator it = projs.begin(); it != projs.end(); ++it)
        result.push_back(project(*it, polygon));

    return result;

}


bool intersect(Point3ArrayPtr polygon1, Point3ArrayPtr polygon2, 
               std::vector<Point3ArrayPtr>& intersection1, std::vector<Point3ArrayPtr>& intersection2, 
               std::vector<Point3ArrayPtr>& rest1,  std::vector<Point3ArrayPtr>& rest2)
{
    Polygon_2 p1 = toPolygon(polygon1);
    Polygon_2 p2 = toPolygon(polygon2);
    Pwh_list_2 result;
    CGAL::intersection(p1, p2, std::back_inserter(result));

    printf("intersect:%lu\n",result.size());
    if (result.size() == 0) return false;

    std::list<Polygon_2> polys = triangulate(result);
    printf("nb pol:%lu\n",polys.size());

    intersection1 = project(polys, polygon1);
    intersection2 = project(polys, polygon2);

    Pwh_list_2 result2;
    CGAL::difference(p1, p2, std::back_inserter(result2));
    printf("diff1:%lu\n",result2.size());

    std::list<Polygon_2> polys2 = triangulate(result2);
    rest1 = project(polys2, polygon1);

    Pwh_list_2 result3;
    CGAL::difference(p2, p1, std::back_inserter(result3));
    printf("diff2:%lu\n",result3.size());

    std::list<Polygon_2> polys3 = triangulate(result3);
    rest2 = project(polys3, polygon2);

    return true;

}

DepthSortEngine::PolygonInfo DepthSortEngine::_toPolygonInfo(const Point3ArrayPtr& points, uint32_t id) const
{
    printf("polygon info : %u %u\n", id , points->size());
    DepthSortEngine::PolygonInfo pinfo;
    pinfo.points = points;
    std::pair<Vector3,Vector3> bounds = points->getBounds();
    pinfo.pmin = bounds.first;
    pinfo.pmax = bounds.second;
    pinfo.id = id;
    return pinfo;
}


DepthSortEngine::PolygonInfoList DepthSortEngine::_toPolygonInfo(const std::vector<Point3ArrayPtr>& polygons, uint32_t id) const
{
    PolygonInfoList result;   
    for (std::vector<Point3ArrayPtr>::const_iterator it = polygons.begin(); it != polygons.end() ; ++it) {
        result.push_back( _toPolygonInfo(*it, id));
    }
    return result;
}

std::pair<real_t, real_t> polygonzbounds(const DepthSortEngine::PolygonInfoList& polygons) {
    DepthSortEngine::PolygonInfoList::const_iterator it = polygons.begin();
    real_t zmin = it->pmin.z();
    real_t zmax = it->pmax.z();
    for (++it; it != polygons.end() ; ++it) { 
        if (zmin > it->pmin.z()) zmin = it->pmin.z();
        if (zmax < it->pmax.z()) zmin = it->pmax.z();
    }
    return std::pair<real_t,real_t>(zmin, zmax);
}

void DepthSortEngine::processTriangle(const TOOLS(Vector3)& v0, const TOOLS(Vector3)& v1, const TOOLS(Vector3)& v2, uint32_t id)
{
    Point3ArrayPtr points(new Point3Array());
    Vector3 v0Cam = __camera->worldToCamera(v0);
    Vector3 v1Cam = __camera->worldToCamera(v1);
    Vector3 v2Cam = __camera->worldToCamera(v2);

    v0Cam.z() *= -1;
    v1Cam.z() *= -1;
    v2Cam.z() *= -1;

    real_t direction = cross(Vector3(v1Cam.x()-v0Cam.x(),v1Cam.y()-v0Cam.y(), 0), Vector3(v2Cam.x()-v0Cam.x(),v2Cam.y()-v0Cam.y(), 0)).z();

    if (direction < 0) {
        printf("reverse ccw\n");
        points->push_back(v0Cam); points->push_back(v2Cam); points->push_back(v1Cam); 
    }
    else { // CCW
        points->push_back(v0Cam); points->push_back(v1Cam); points->push_back(v2Cam); 
    }

    PolygonInfo p = _toPolygonInfo(points, id);
    _processTriangle(p, __polygonlist.begin());
}

typename DepthSortEngine::PolygonInfoList::iterator DepthSortEngine::_processTriangle( PolygonInfo polygon, typename DepthSortEngine::PolygonInfoList::iterator begin)
{
    bool processed = false;
    printf("Process triangle %u\n", polygon.id);
    for (DepthSortEngine::PolygonInfoList::iterator it = begin; it != __polygonlist.end(); ) {
        uint32_t itid = it->id;
        if ((polygon.pmin.x() > it->pmax.x())||(polygon.pmax.x() < it->pmin.x())) { ++it; continue; }
        if ((polygon.pmin.y() > it->pmax.y())||(polygon.pmax.y() < it->pmin.y())) { ++it; continue; }
        else {
            std::vector<Point3ArrayPtr> intersection1;
            std::vector<Point3ArrayPtr> intersection2; 
            std::vector<Point3ArrayPtr> rest1;
            std::vector<Point3ArrayPtr> rest2;
            printf("Test intersection\n");
            if (!intersect(polygon.points, it->points, intersection1, intersection2, rest1, rest2)) { 
                printf("No intersection\n");
                ++it; continue; 
            }
            bool isbegin = (it == begin);
            printf("intersect\n");
            printf("%lu %lu %lu %lu\n", intersection1.size(), intersection2.size(), rest1.size(), rest2.size());

            PolygonInfoList mrest2 = _toPolygonInfo(rest2, itid);
            PolygonInfoList mrest1 = _toPolygonInfo(rest1, itid);
            
            // We should process depth comparison
            PolygonInfoList mintersection1 = _toPolygonInfo(intersection1, itid);
            printf("mintersection1 : %lu\n", mintersection1.size());
            std::pair<real_t, real_t> zbounds1 = polygonzbounds(mintersection1);
            real_t zmin1 = zbounds1.first; real_t zmax1 = zbounds1.second;

            PolygonInfoList mintersection2 = _toPolygonInfo(intersection2, itid);
            printf("mintersection2 : %lu\n", mintersection2.size());
            std::pair<real_t, real_t> zbounds2 = polygonzbounds(mintersection2);
            real_t zmin2 = zbounds2.first; real_t zmax2 = zbounds2.second;

            printf("i1: %f %f\n", zmin1, zmax1);
            printf("i2: %f %f\n", zmin2, zmax2);
            if (zmax1 <= zmin2) {
                // new polygon hide the previous one.
                // we remove the previous one and replace it by its non overlaping part.
                it = __polygonlist.erase(it);
                if (isbegin) {  begin = it; }
                if (mrest2.size() > 0) {
                    printf("rest2 : %lu\n", mrest2.size());
                    __polygonlist.insert(it, mrest2.begin(), mrest2.end());
                }
                continue; 
                // we continue to compare the triangle with other triangles

            }
            else if (zmax2 <= zmin1) {
                 // new polygon is hidden by the previous one.
                 // we keep the previous one.
                 // and test for the non overlapping area of the new one.
                PolygonInfoList::iterator it2 = it;
                ++it2;
                if (!mrest1.empty()){
                    for (PolygonInfoList::const_iterator itR1 = mrest1.begin(); itR1 != mrest1.end() ; ++itR1) {
                         _processTriangle(*itR1, it2);
                    }
                }
                processed = true;
                break;
            }
            else {
                it = __polygonlist.erase(it);
                if (isbegin) {  begin = it; }

                // We should cut individual triangles
                Vector3 p0 = mintersection1.begin()->points->getAt(0);
                Vector3 p1 = mintersection1.begin()->points->getAt(1);
                Vector3 p2 = mintersection1.begin()->points->getAt(2);
                Plane3 plane (cross(p1-p0,p2-p0), p0);
                for (PolygonInfoList::const_iterator itI2 = mintersection2.begin(); itI2 != mintersection2.end() ; ++itI2) {
                    Point3ArrayPtr points = plane_triangle_clip(plane, itI2->points->getAt(0), itI2->points->getAt(1), itI2->points->getAt(2));
                    printf("Clip triangle intersection2\n");
                    if (is_null_ptr(points) || points->size() < 3) continue;
                    if (points->size() == 3) {
                        __polygonlist.insert(it,_toPolygonInfo(points, itI2->id));
                    }
                    else {
                        Vector3& first = points->getAt(0);
                        for(Point3Array::const_iterator itP = points->begin()+1; itP != points->end()-1; ++itP){
                            Point3ArrayPtr lpoints(new Point3Array());
                            lpoints->push_back(first);
                            lpoints->push_back(*itP);
                            lpoints->push_back(*(itP+1));
                            __polygonlist.insert(it,_toPolygonInfo(lpoints, itI2->id));
                        }
                    } 
                }

                Vector3 p0b = mintersection2.begin()->points->getAt(0);
                Vector3 p1b = mintersection2.begin()->points->getAt(1);
                Vector3 p2b = mintersection2.begin()->points->getAt(2);
                Plane3 planeb (cross(p1b-p0b,p2b-p0b), p0b);
                for (PolygonInfoList::const_iterator itI2 = mintersection1.begin(); itI2 != mintersection1.end() ; ++itI2) {
                    Point3ArrayPtr points = plane_triangle_clip(planeb, itI2->points->getAt(0), itI2->points->getAt(1), itI2->points->getAt(2));
                    printf("Clip triangle intersection1\n");
                    if (is_null_ptr(points) || points->size() < 3) continue;
                    if (points->size() == 3) {
                        printf("result in triangle\n");
                        __polygonlist.insert(it,_toPolygonInfo(points, itI2->id));
                    }
                    else {
                        printf("result in polygon\n");
                        Vector3& first = points->getAt(0);
                        for(Point3Array::const_iterator itP = points->begin()+1; itP != points->end()-1; ++itP){
                            Point3ArrayPtr lpoints(new Point3Array());
                            lpoints->push_back(first);
                            lpoints->push_back(*itP);
                            lpoints->push_back(*(itP+1));
                            __polygonlist.insert(it,_toPolygonInfo(lpoints, itI2->id));
                        }
                    } 
                }

                // __polygonlist.insert(it, mintersection1.begin(), mintersection1.end());
                // __polygonlist.insert(it, mintersection2.begin(), mintersection2.end());                

                __polygonlist.insert(it, mrest2.begin(), mrest2.end());

                printf("rest1 : %lu\n", mrest1.size());
                // We should process depth comparison
                PolygonInfoList::iterator it2 = it;
                ++it2;
                for (PolygonInfoList::const_iterator itR1 = mrest1.begin(); itR1 != mrest1.end() ; ++itR1) {
                    _processTriangle(*itR1, it2);
                }
                

            //++it;
            }
        } 
    }
    if (!processed) {
        printf("add to list\n");
        __polygonlist.insert(begin, polygon);
    }
    return begin;
}

ScenePtr DepthSortEngine::getResult(Color4::eColor4Format format) const
{
    ScenePtr scene(new Scene());
    for(PolygonInfoList::const_iterator it = __polygonlist.begin(); it != __polygonlist.end(); ++it){
        Color4 col = Color4::fromUint(it->id, format);
        Material * mat = new Material(Color3(col));
        mat->getTransparency() = col.getAlphaClamped();
        scene->add(ShapePtr(new Shape(GeometryPtr(new TriangleSet(it->points,Index3ArrayPtr(new Index3Array(1,Index3(0,1,2))))), AppearancePtr(mat), it->id)));
    }
    return scene;
}

ScenePtr DepthSortEngine::getProjectionResult(Color4::eColor4Format format) const
{
    ScenePtr scene(new Scene());
    for(PolygonInfoList::const_iterator it = __polygonlist.begin(); it != __polygonlist.end(); ++it){
        Color4 col = Color4::fromUint(it->id, format);
        Material * mat = new Material(Color3(col));
        mat->getTransparency() = col.getAlphaClamped();
        Point3ArrayPtr points(it->points);
        for(Point3Array::iterator itP = points->begin(); itP != points->end(); ++itP)
            itP->z() = 0;
        scene->add(ShapePtr(new Shape(GeometryPtr(new TriangleSet(it->points,Index3ArrayPtr(new Index3Array(1,Index3(0,1,2))))), AppearancePtr(mat), it->id)));
    }
    return scene;
}

