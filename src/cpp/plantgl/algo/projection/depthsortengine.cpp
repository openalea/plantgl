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
             


/* ----------------------------------------------------------------------- */

#include "depthsortengine.h"
#include "projection_util.h"
#include <plantgl/scenegraph/geometry/boundingbox.h>
#include <plantgl/scenegraph/geometry/triangleset.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/algo/base/planeclipping.h>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE

DepthSortEngine::DepthSortEngine(eIdPolicy idPolicy) :
    ProjectionEngine(idPolicy)
{
}

DepthSortEngine::~DepthSortEngine()
{

}


void DepthSortEngine::iprocess(TriangleSetPtr triangles, AppearancePtr appearance, uint32_t id, ProjectionCameraPtr camera, uint32_t threadid )
{
	#ifdef PGL_WITH_CGAL

    /*
    const Point3ArrayPtr points(triangles->getPointList());
    const Index3ArrayPtr indices(triangles->getIndexList());

    bool ccw = triangles->getCCW();

    triangles->checkNormalList();

    bool hasColor = triangles->hasColorList();
    */
    size_t nbfaces = triangles->getIndexListSize();

    for(uint32_t itidx = 0; itidx < nbfaces; ++itidx){
        //printf("triangle %i on %i\n", itidx, nbfaces);
        const Vector3& v0 = triangles->getFacePointAt(itidx,0);
        const Vector3& v1 = triangles->getFacePointAt(itidx,1);
        const Vector3& v2 = triangles->getFacePointAt(itidx,2);

        processTriangle(v0, v1, v2, getNewPrimitiveId(id));

    }  

	#endif
}

void DepthSortEngine::iprocess(PolylinePtr polyline, MaterialPtr material, uint32_t id, ProjectionCameraPtr camera, uint32_t threadid )
{

}

void DepthSortEngine::iprocess(PointSetPtr pointset, MaterialPtr material, uint32_t id, ProjectionCameraPtr camera, uint32_t threadid )
{

}



#ifdef PGL_WITH_CGAL
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/intersections.h>
#include <list>
#include <iterator>


struct FaceInfo2
{
  FaceInfo2(){}
  int nesting_level;
  bool in_domain(){ 
    return nesting_level%2 == 1;
  }
};

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2                                   Point_2;
typedef CGAL::Polygon_2<Kernel>                           Polygon_2;
typedef CGAL::Triangle_2<Kernel>                          Triangle_2;
typedef CGAL::Polygon_with_holes_2<Kernel>                Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2>                   Pwh_list_2;

typedef CGAL::Triangulation_vertex_base_2<Kernel>                      Vb;
typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo2,Kernel>    Fbb;
typedef CGAL::Constrained_triangulation_face_base_2<Kernel,Fbb>        Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>                    TDS;
typedef CGAL::Exact_predicates_tag                                     Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<Kernel, TDS, Itag>  CDT;


inline Point_2 toPoint2(const Vector3& v) { return Point_2(v.x(),v.y()); }
inline Vector2 toVector2(const Point_2& v) { return Vector2(CGAL::to_double(v.x()),CGAL::to_double(v.y())); }


Polygon_2 toPolygon(Point3ArrayPtr polygon) {
    Polygon_2 pol;
    for (Point3Array::const_iterator it = polygon->begin(); it != polygon->end(); ++it){
        pol.push_back(toPoint2(*it));
    }
    return pol;
}

Triangle_2 toTriangle(Point3ArrayPtr polygon) {
    Triangle_2 pol(toPoint2(polygon->getAt(0)),toPoint2(polygon->getAt(1)),toPoint2(polygon->getAt(2)));
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
  for(CDT::All_faces_iterator it = cdt.all_faces_begin(); it != cdt.all_faces_end(); ++it){
    it->info().nesting_level = -1;
  }
  std::list<CDT::Edge> border;
  mark_domains(cdt, cdt.infinite_face(), 0, border);
  while(! border.empty()){
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
    bool hashole = (polygon.holes_begin() != polygon.holes_end());
    if (polygon.outer_boundary().size() == 3 && !hashole){
            Polygon_2 p;
            for(auto itP = polygon.outer_boundary().vertices_begin(); itP != polygon.outer_boundary().vertices_end(); ++itP)
                p.push_back(*itP);
            if(p.area() > GEOM_EPSILON)
                result.push_back(p);

    }
    else {
        CDT triangulation;
        triangulation.insert_constraint(polygon.outer_boundary().vertices_begin(), polygon.outer_boundary().vertices_end(), true);
       
        for (typename Polygon_with_holes_2::Hole_const_iterator itH = polygon.holes_begin(); itH != polygon.holes_end(); ++itH){
            triangulation.insert_constraint(itH->vertices_begin(), itH->vertices_end(), true);
        }

        //Mark facets that are inside the domain bounded by the polygon
        mark_domains(triangulation);

        for (CDT::Finite_faces_iterator fit=triangulation.finite_faces_begin();
                                        fit!=triangulation.finite_faces_end();++fit)
        {
            if ( fit->info().in_domain() ) {
                Polygon_2 p;
                for (int i = 0; i < 3; ++i)
                    p.push_back(fit->vertex(i)->point());
                if(p.area() > GEOM_EPSILON)
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
            real_t w0 = edgeFunction(polygon->getAt(1), polygon->getAt(2), vertex);
            real_t w1 = edgeFunction(polygon->getAt(2), polygon->getAt(0), vertex);
            real_t w2 = edgeFunction(polygon->getAt(0), polygon->getAt(1), vertex);

            if (fabs(area) <= GEOM_EPSILON) {
                real_t sumw = w0 + w1 + w2;
                w0 /= sumw;
                w1 /= sumw;
                w2 /= sumw;
            }
            else {
                w0 /= area;
                w1 /= area;
                w2 /= area;                
            }

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

template<class Kernel, class Container>
void print_polygon (const CGAL::Polygon_2<Kernel, Container>& P)
{
  typename CGAL::Polygon_2<Kernel, Container>::Vertex_const_iterator vit;
  std::cout << "[ " << P.size() << " simple:" << (P.is_simple()?"True":"False") << " orientation:" << (P.orientation()?"ccw":"cw")<< " area:" << P.area() << " vertices:";
  for (vit = P.vertices_begin(); vit != P.vertices_end(); ++vit)
    std::cout << " (" << *vit << ')';
  std::cout << " ]" << std::endl;
}

/**
 * Compute the intersection of two polygons.
 *
 * Given two polygons, compute the intersection of the two polygons and
 * store the result in intersection1 and intersection2. Also compute the
 * difference of the first polygon with the second and store the result in
 * rest1 and the difference of the second polygon with the first and store the
 * result in rest2.
 *
 * @param polygon1 The first polygon.
 * @param polygon2 The second polygon.
 * @param intersection1 The intersection of the two polygons projected on the
 * first polygon.
 * @param intersection2 The intersection of the two polygons projected on the
 * second polygon.
 * @param rest1 The difference of the first polygon with the second projected
 * on the first polygon.
 * @param rest2 The difference of the second polygon with the first projected on
 * the second polygon.
 * @return true if the intersection is not empty, false otherwise.
 */
bool intersect(Point3ArrayPtr polygon1, Point3ArrayPtr polygon2, 
               std::vector<Point3ArrayPtr>& intersection1, std::vector<Point3ArrayPtr>& intersection2, 
               std::vector<Point3ArrayPtr>& rest1,  std::vector<Point3ArrayPtr>& rest2)
{
    Polygon_2 p1 = toPolygon(polygon1);
    if (p1.area() < GEOM_EPSILON) return false;
    Polygon_2 p2 = toPolygon(polygon2);
    if (p2.area() < GEOM_EPSILON) return false;
    assert( p1.is_counterclockwise_oriented ());
    assert( p2.is_counterclockwise_oriented ());
    // print_polygon(p1);
    // print_polygon(p2);

    Pwh_list_2 result;
    CGAL::intersection(p1, p2, std::back_inserter(result));

    if (result.size() == 0) return false;

    std::list<Polygon_2> polys = triangulate(result);
    if (polys.size() == 0) return false;

    intersection1 = project(polys, polygon1);
    intersection2 = project(polys, polygon2);

    Pwh_list_2 result2;
    CGAL::difference(p1, p2, std::back_inserter(result2));

    std::list<Polygon_2> polys2 = triangulate(result2);
    rest1 = project(polys2, polygon1);

    Pwh_list_2 result3;
    CGAL::difference(p2, p1, std::back_inserter(result3));

    std::list<Polygon_2> polys3 = triangulate(result3);
    rest2 = project(polys3, polygon2);

    return true;

}

DepthSortEngine::PolygonInfo _toPolygonInfo(const Point3ArrayPtr& points, uint32_t id) 
{
    // printf("polygon info : %u %u\n", id , points->size());
    DepthSortEngine::PolygonInfo pinfo;
    pinfo.points = points;
    std::pair<Vector3,Vector3> bounds = points->getBounds();
    pinfo.pmin = bounds.first;
    pinfo.pmax = bounds.second;
    pinfo.id = id;
    return pinfo;
}

DepthSortEngine::PolygonInfoList _toPolygonInfo(const std::vector<Point3ArrayPtr>& polygons, uint32_t id) 
{
    DepthSortEngine::PolygonInfoList result;   
    for (std::vector<Point3ArrayPtr>::const_iterator it = polygons.begin(); it != polygons.end() ; ++it) {
        result.push_back( _toPolygonInfo(*it, id));
    }
    return result;
}

bool DepthSortEngine::PolygonInfo::bbx_intersect(const PolygonInfo& other) const{
    if ((pmin.x() > other.pmax.x())||(pmax.x() < other.pmin.x())) { return false; }
    if ((pmin.y() > other.pmax.y())||(pmax.y() < other.pmin.y())) { return false; }
    return true;
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

DepthSortEngine::PolygonInfoList::iterator DepthSortEngine::removePolygon(DepthSortEngine::PolygonInfoList::iterator it)
{
    return __polygonlist.erase(it);
}

DepthSortEngine::PolygonInfoSet::iterator DepthSortEngine::appendPolygon(const PolygonInfo& polygon)
{
    return __polygonlist.insert(__polygonlist.end(), polygon);
}

DepthSortEngine::PolygonInfoIteratorList DepthSortEngine::appendPolygons(DepthSortEngine::PolygonInfoList::iterator begin, DepthSortEngine::PolygonInfoList::iterator end)
{
    DepthSortEngine::PolygonInfoIteratorList result;
    for(DepthSortEngine::PolygonInfoList::iterator it = begin; it != end; ++it)
        result.push_back(appendPolygon(*it));
    return result;
}

/*
DepthSortEngine::PolygonInfoIteratorList DepthSortEngine::getIntersectingPolygons(const PolygonInfo& polygon)
{
    DepthSortEngine::PolygonInfoIteratorList result;
    for (DepthSortEngine::PolygonInfoSet::iterator it = __polygonlist.begin(); it != __polygonlist.end(); ++it) {
        if (!polygon.bbx_intersect(*it)) { continue; }
        result.push_back(it);
    }
    return result;
}

DepthSortEngine::PolygonInfoSet::iterator DepthSortEngine::getNextIntersectingPolygon(const PolygonInfo& polygon, DepthSortEngine::PolygonInfoSet::iterator begin)
{
    for (DepthSortEngine::PolygonInfoSet::iterator it = begin; it != __polygonlist.end(); ++it) {
        if (polygon.bbx_intersect(*it)) { return it; }
    }
    return __polygonlist.end();
}
*/

void swap(Vector3& v1, Vector3& v2){
    Vector3 vTemp = v1;
    v1 = v2;
    v2 = vTemp;    
}

void DepthSortEngine::processTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, uint32_t id)
{
    // setlocale(LC_ALL, "en_GB");

    Vector3 v0Cam = __camera->worldToCamera(v0);
    Vector3 v1Cam = __camera->worldToCamera(v1);
    Vector3 v2Cam = __camera->worldToCamera(v2);

    v0Cam.z() *= -1;
    v1Cam.z() *= -1;
    v2Cam.z() *= -1;

    if(v1Cam.x() < v0Cam.x())
    {
        if(v1Cam.x() < v2Cam.x() ){
            swap(v0Cam,v1Cam);
        }
        else if (fabs(v1Cam.x() - v2Cam.x())< GEOM_EPSILON) {
            if (v1Cam.y() < v2Cam.y()) swap(v0Cam,v1Cam);
            else swap(v0Cam,v2Cam);
        }
        else {
            swap(v0Cam,v2Cam);
        }
    }
    else if(v2Cam.x() < v0Cam.x() ){
        swap(v0Cam,v2Cam);
    }
    else if (fabs(v1Cam.x() - v0Cam.x())< GEOM_EPSILON) {
        if (v1Cam.y() < v0Cam.y()) swap(v0Cam,v1Cam);
    }
    else if (fabs(v2Cam.x() - v0Cam.x())< GEOM_EPSILON) {
        if (v2Cam.y() < v0Cam.y()) swap(v0Cam,v2Cam);
    }


    real_t direction  = cross(Vector2(v1Cam.x()-v0Cam.x(),v1Cam.y()-v0Cam.y()), Vector2(v2Cam.x()-v0Cam.x(),v2Cam.y()-v0Cam.y()));

    Point3ArrayPtr points(new Point3Array());
    if (fabs(direction) < GEOM_EPSILON) {
        return;
    }
    else if (direction < 0) {
        points->push_back(v0Cam); points->push_back(v2Cam); points->push_back(v1Cam); 
    }
    else { // CCW
        points->push_back(v0Cam); points->push_back(v1Cam); points->push_back(v2Cam); 
    }

    PolygonInfo p = _toPolygonInfo(points, id);
    _processTriangle(p);
}

DepthSortEngine::PolygonInfoList clip_polygons(const DepthSortEngine::PolygonInfoList& polygons, const Plane3& plane){
    DepthSortEngine::PolygonInfoList result;
    for (DepthSortEngine::PolygonInfoList::const_iterator it = polygons.begin(); it != polygons.end() ; ++it) {
        Point3ArrayPtr points = plane_triangle_clip(plane, it->points->getAt(0), it->points->getAt(1), it->points->getAt(2));
        if (is_null_ptr(points) || points->size() < 3) {
            return result;
        }
        if (points->size() == 3) {
            result.push_back(_toPolygonInfo(points, it->id));
        }
        else {
            Vector3& first = points->getAt(0);
            for(Point3Array::const_iterator itP = points->begin()+1; itP != points->end()-1; ++itP){
                Point3ArrayPtr lpoints(new Point3Array());
                lpoints->push_back(first);
                lpoints->push_back(*itP);
                lpoints->push_back(*(itP+1));
                result.push_back(_toPolygonInfo(lpoints, it->id));
            }
        } 
    }
    return result;
}


enum DEPTHCMP {IN_FRONT = -1, SAMEDEPTH = 0, BEHIND = 1, MIXED = 2, NOTVALID = 100};

/**
 * Compares the depth of two real number values.
 *
 * @param a The first depth value.
 * @param b The second depth value.
 * @return IN_FRONT if the first value is significantly greater than the second,
 *         BEHIND if the first value is significantly less than the second,
 *         SAMEDEPTH if the values are approximately equal within a small epsilon.
 */
enum DEPTHCMP compare_depth(real_t a, real_t b){
    if (a - b > GEOM_EPSILON) return BEHIND;
    else if (a - b < -GEOM_EPSILON) return IN_FRONT;
    else return SAMEDEPTH;
}

/**
 * Compares the depth of two lists of polygons.
 *
 * @param a The first list of polygons.
 * @param b The second list of polygons.
 * @return -1 if the first list is "in front of" the second list (i.e. the first list is closer to the camera), 
 *          0 if the two lists are of the same depth,
 *          1 if the second list is in front of the first, 
 *          2 if they have mixed depth.
 */
enum DEPTHCMP compare_depths(std::vector<Point3ArrayPtr> a, std::vector<Point3ArrayPtr> b){
    if (a.size() != b.size()) return NOTVALID;
    enum DEPTHCMP result = compare_depth(a[0]->getAt(0).z(),b[0]->getAt(0).z());  
    for (uint32_t i = 0; i < a.size(); ++i) {
        Point3ArrayPtr p1 = a[i];
        Point3ArrayPtr p2 = b[i];
        Point3Array::const_iterator it = p1->begin();
        Point3Array::const_iterator it2 = p2->begin();
        for (; it != p1->end() && it2 != p2->end(); ++it, ++it2) {
            enum DEPTHCMP lresult = compare_depth(it->z(),it2->z());
            if (result == SAMEDEPTH) result = lresult;
            else if (lresult == -result) return MIXED;
        }
        if (it != p1->end() || it2 != p2->end()) return NOTVALID;
    }
    return result;
}

void
DepthSortEngine::_processTriangle( PolygonInfo polygon, 
                                   uint32_t begin, 
                                   bool insertTriangle)
{

    bool processed = false;

    PolygonInfoList toAppend;
    PolygonInfoList::iterator it = __polygonlist.begin();
    for (uint32_t i = 0 ; i < begin && it != __polygonlist.end(); ++i) ++it;
    while ( it != __polygonlist.end() ) {

        PolygonInfo& current = *it;
        uint32_t itid = current.id;
        if (!polygon.bbx_intersect(current)) { ++it; continue; }
        else {
            std::vector<Point3ArrayPtr> intersection1;
            std::vector<Point3ArrayPtr> intersection2; 
            std::vector<Point3ArrayPtr> restPolygon;
            std::vector<Point3ArrayPtr> restCurrent;
            if (!intersect(polygon.points, current.points, intersection1, intersection2, restPolygon, restCurrent)) { 
                ++it; continue; 
            }

            PolygonInfoList mrestCurrent = _toPolygonInfo(restCurrent, itid);
            PolygonInfoList mrestPolygon = _toPolygonInfo(restPolygon, polygon.id);
            
            PolygonInfoList mintersection1 = _toPolygonInfo(intersection1, polygon.id);
            PolygonInfoList mintersection2 = _toPolygonInfo(intersection2, itid);

            // We should process depth comparison
            enum DEPTHCMP depthcmp = compare_depths(intersection1, intersection2);

            if (depthcmp <= 0) {
                // new polygon hide the previous one.
                // we remove the previous one and replace it by its non overlaping part.
                it = removePolygon(it);                

                if (insertTriangle && mrestCurrent.size() > 0) {
                    toAppend.insert(toAppend.end(),mrestCurrent.begin(), mrestCurrent.end());
                }

                continue; 
                // we continue to compare the triangle with other triangles

            }
            else if (depthcmp == BEHIND) {
                 // new polygon is hidden by the previous one.
                 // we keep the previous one.
                 // and test for the non overlapping area of the new one.
                if (mrestPolygon.empty()){
                        processed = true;
                        break;
                }
                else if(mrestPolygon.size() == 1) {
                        polygon = mrestPolygon.front();
                        ++it;
                        continue;
                }
                else {
                        uint32_t dist = std::distance( __polygonlist.begin(),it)+1;
                        for (PolygonInfoList::const_iterator itR1 = mrestPolygon.begin(); 
                                                             itR1 != mrestPolygon.end() ; 
                                                             ++itR1) {
                            _processTriangle(*itR1, dist, insertTriangle);
                        }
                        processed = true;
                        break;
                }
            }
            else  if (depthcmp == MIXED){

                it = removePolygon(it);

                // We should cut individual triangles
                Vector3 p0 = mintersection1.begin()->points->getAt(0);
                Vector3 p1 = mintersection1.begin()->points->getAt(1);
                Vector3 p2 = mintersection1.begin()->points->getAt(2);                
                Plane3 plane (-cross(p1-p0,p2-p0), p0);

                PolygonInfoList clipped = clip_polygons(mintersection2, plane);
                toAppend.insert(toAppend.end(),clipped.begin(), clipped.end());

                Vector3 p0b = mintersection2.begin()->points->getAt(0);
                Vector3 p1b = mintersection2.begin()->points->getAt(1);
                Vector3 p2b = mintersection2.begin()->points->getAt(2);
                Plane3 planeb (-cross(p1b-p0b,p2b-p0b), p0b);

                if (insertTriangle) {
                    clipped = clip_polygons(mintersection1, planeb);
                    toAppend.insert(toAppend.end(),clipped.begin(), clipped.end());
                }

                if (mrestCurrent.size() > 0) {
                    toAppend.insert(toAppend.end(),mrestCurrent.begin(), mrestCurrent.end());
                }

                if (insertTriangle && !mrestPolygon.empty()){
                    if(mrestPolygon.size() == 1) {
                        polygon = mrestPolygon.front();
                        ++it;
                        continue;
                    }
                    else {
                        uint32_t dist = std::distance( __polygonlist.begin(),it)+1;
                        for (PolygonInfoList::const_iterator itR1 = mrestPolygon.begin(); 
                                                             itR1 != mrestPolygon.end() ; 
                                                             ++itR1) {
                            _processTriangle(*itR1, dist, insertTriangle);
                        }
                        processed = true;
                        break;
                    }
                }


                processed = true;
                break;
            }
            else {
                printf("error in comparison of polygon depth\n");
            }
        } 
    }
    if (!processed) {
        appendPolygon(polygon);
    }
    appendPolygons(toAppend.begin(), toAppend.end());
}

ScenePtr DepthSortEngine::getResult(Color4::eColor4Format format, bool cameraCoordinates) const
{
    ScenePtr scene(new Scene());
    for(PolygonInfoList::const_iterator it = __polygonlist.begin(); it != __polygonlist.end(); ++it){
        Color4 col = Color4::fromUint(it->id, format);
        Material * mat = new Material(Color3(col));
        mat->getTransparency() = col.getAlphaClamped();
        Point3ArrayPtr lpoints = it->points;
        if (!cameraCoordinates) {
            lpoints = Point3ArrayPtr(new Point3Array(*lpoints));
            for(Point3Array::iterator itP = lpoints->begin(); itP != lpoints->end(); ++itP){
                itP->z() *= -1;
                *itP = __camera->cameraToWorld(*itP);
            }
        }
        scene->add(ShapePtr(new Shape(GeometryPtr(new TriangleSet(lpoints,Index3ArrayPtr(new Index3Array(1,Index3(0,1,2))))), AppearancePtr(mat), it->id)));
    }
    return scene;
}

ScenePtr DepthSortEngine::getSurfaceResult( bool cameraCoordinates) const
{
    printf("DepthSortEngine::getSurfaceResult\n");
    ScenePtr scene(new Scene());
    real_t minsurf = REAL_MAX;
    real_t maxsurf = -REAL_MAX;
    for(PolygonInfoList::const_iterator it = __polygonlist.begin(); it != __polygonlist.end(); ++it){
        real_t csurface = fabs(edgeFunction(it->points->getAt(0),
                                            it->points->getAt(1),
                                            it->points->getAt(2)))/2;
        if (csurface < minsurf) {
            minsurf = csurface;
        }
        if (csurface > maxsurf) {
            maxsurf = csurface;
        }
    }

    for(PolygonInfoList::const_iterator it = __polygonlist.begin(); it != __polygonlist.end(); ++it){
        Color3 col = Color3::interpolate(Color3::BLUE, Color3::RED, (fabs(edgeFunction(it->points->getAt(0), it->points->getAt(1), it->points->getAt(2)))/2-minsurf)/(maxsurf-minsurf));
        Material * mat = new Material(Color3(col));
        Point3ArrayPtr lpoints = it->points;
        if (!cameraCoordinates) {
            lpoints = Point3ArrayPtr(new Point3Array(*lpoints));
            for(Point3Array::iterator itP = lpoints->begin(); itP != lpoints->end(); ++itP){
                itP->z() *= -1;
                *itP = __camera->cameraToWorld(*itP);
            }
        }
        scene->add(ShapePtr(new Shape(GeometryPtr(new TriangleSet(lpoints,Index3ArrayPtr(new Index3Array(1,Index3(0,1,2))))), AppearancePtr(mat), it->id)));
    }
    printf("minsurf %f maxsurf %f\n", minsurf, maxsurf);
    return scene;
}

ScenePtr DepthSortEngine::getProjectionResult(Color4::eColor4Format format, bool cameraCoordinates) const
{
    ScenePtr scene(new Scene());
    for(PolygonInfoList::const_iterator it = __polygonlist.begin(); it != __polygonlist.end(); ++it){
        Color4 col = Color4::fromUint(it->id, format);
        Material * mat = new Material(Color3(col));
        mat->getTransparency() = col.getAlphaClamped();
        Point3ArrayPtr lpoints =  Point3ArrayPtr(new Point3Array(*it->points));
        if (!cameraCoordinates) {
            for(Point3Array::iterator itP = lpoints->begin(); itP != lpoints->end(); ++itP){
                itP->z() = 0;
                *itP = __camera->cameraToWorld(*itP);
            }
        }
        else {
            for(Point3Array::iterator itP = lpoints->begin(); itP != lpoints->end(); ++itP){
                itP->z() = 0;
            }
        }
        scene->add(ShapePtr(new Shape(GeometryPtr(new TriangleSet(lpoints,Index3ArrayPtr(new Index3Array(1,Index3(0,1,2))))), AppearancePtr(mat), it->id)));
    }
    return scene;
}


ScenePtr DepthSortEngine::getSurfaceProjectionResult( bool cameraCoordinates) const
{
    ScenePtr scene(new Scene());
    real_t minsurf =REAL_MAX;
    real_t maxsurf = -REAL_MAX;
    for(PolygonInfoList::const_iterator it = __polygonlist.begin(); it != __polygonlist.end(); ++it){
        real_t csurface = fabs(edgeFunction(Vector2(it->points->getAt(0).x(),it->points->getAt(0).y()),
                                            Vector2(it->points->getAt(1).x(),it->points->getAt(1).y()),
                                            Vector2(it->points->getAt(2).x(),it->points->getAt(2).y())))/2;
        if (csurface < minsurf) {
            minsurf = csurface;
        }
        if (csurface > maxsurf) {
            maxsurf = csurface;
        }
    }
    for(PolygonInfoList::const_iterator it = __polygonlist.begin(); it != __polygonlist.end(); ++it){
        Color3 col = Color3::interpolate(Color3::BLUE, Color3::RED, (fabs(edgeFunction(it->points->getAt(0), it->points->getAt(1), it->points->getAt(2)))/2-minsurf)/(maxsurf-minsurf));
        Material * mat = new Material(Color3(col));
        Point3ArrayPtr lpoints =  Point3ArrayPtr(new Point3Array(*it->points));
        if (!cameraCoordinates) {
            for(Point3Array::iterator itP = lpoints->begin(); itP != lpoints->end(); ++itP){
                itP->z() = 0;
                *itP = __camera->cameraToWorld(*itP);
            }
        }
        else {
            for(Point3Array::iterator itP = lpoints->begin(); itP != lpoints->end(); ++itP){
                itP->z() = 0;
            }
        }
        scene->add(ShapePtr(new Shape(GeometryPtr(new TriangleSet(lpoints,Index3ArrayPtr(new Index3Array(1,Index3(0,1,2))))), AppearancePtr(mat), it->id)));
    }
    return scene;
}

pgl_hash_map<uint32_t,real_t> DepthSortEngine::idsurfaces() const {
    pgl_hash_map<uint32_t,real_t> result;
    for(PolygonInfoList::const_iterator it = __polygonlist.begin(); it != __polygonlist.end(); ++it){
        real_t csurface = fabs(edgeFunction(it->points->getAt(0), it->points->getAt(1), it->points->getAt(2)))/2;
        if (result.find(it->id) == result.end()) {
            result[it->id] = csurface;
        }
        else {
            result[it->id] += csurface;
        }
    }
    return result;
}
#endif
