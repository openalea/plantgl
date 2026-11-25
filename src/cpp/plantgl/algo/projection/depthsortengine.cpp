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
#include "projectionrenderer.h"
#include <plantgl/scenegraph/geometry/boundingbox.h>
#include <plantgl/scenegraph/geometry/triangleset.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/algo/base/planeclipping.h>
#include <plantgl/algo/base/tesselator.h>

#include <stdexcept>

#ifdef PGL_WITH_CGAL
#include <array>
#include <atomic>
#include <condition_variable>
#include <future>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <CGAL/config.h>
#include <limits>
#include <mutex>
#include <optional>
#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>
#endif

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE

struct DepthSortEngine::PolygonIndex {
    unsigned intersectionThreads = 1;
#ifdef PGL_WITH_CGAL
    std::unique_ptr<boost::asio::thread_pool> intersectionPool;
    using Point = boost::geometry::model::point<real_t, 2, boost::geometry::cs::cartesian>;
    using Box = boost::geometry::model::box<Point>;
    using Entry = std::pair<Box, PolygonInfoList::iterator>;

    static Box box(const PolygonInfo& polygon)
    {
        return Box(Point(polygon.pmin.x(), polygon.pmin.y()),
                   Point(polygon.pmax.x(), polygon.pmax.y()));
    }

    void insert(PolygonInfoList::iterator it)
    {
        tree.insert(Entry(box(*it), it));
    }

    void erase(PolygonInfoList::iterator it)
    {
        tree.remove(Entry(box(*it), it));
    }

    std::vector<PolygonInfoList::iterator> query(const PolygonInfo& polygon, std::size_t from) const
    {
        std::vector<PolygonInfoList::iterator> candidates;
        const auto intersects = boost::geometry::index::intersects(box(polygon));
        for (auto it = tree.qbegin(intersects); it != tree.qend(); ++it) {
            if (it->second->insertionOrder >= from)
                candidates.push_back(it->second);
        }
        // Pop from the back in list order, including for equal-depth polygons.
        std::sort(candidates.begin(), candidates.end(), [](auto a, auto b) {
            return a->insertionOrder > b->insertionOrder;
        });
        return candidates;
    }

    boost::geometry::index::rtree<Entry, boost::geometry::index::quadratic<16>> tree;
    std::size_t nextOrder = 0;
#endif
};

DepthSortEngine::DepthSortEngine(eIdPolicy idPolicy) :
    ProjectionEngine(idPolicy), __occlusionPolicy(eFullOcclusion),
    __polygonIndex(new PolygonIndex)
{
}

DepthSortEngine::DepthSortEngine(const DepthSortEngine& other) :
    ProjectionEngine(other), __polygonlist(other.__polygonlist),
    __occlusionPolicy(other.__occlusionPolicy), __polygonIndex(new PolygonIndex)
{
#ifdef PGL_WITH_CGAL
    for (auto it = __polygonlist.begin(); it != __polygonlist.end(); ++it)
        __polygonIndex->insert(it);
    __polygonIndex->nextOrder = other.__polygonIndex->nextOrder;
#endif
    __polygonIndex->intersectionThreads = other.__polygonIndex->intersectionThreads;
}

DepthSortEngine& DepthSortEngine::operator=(const DepthSortEngine& other)
{
    if (this != &other) {
        DepthSortEngine copy(other);
        ProjectionEngine::operator=(copy);
        __polygonlist.swap(copy.__polygonlist);
        __polygonIndex.swap(copy.__polygonIndex);
        __occlusionPolicy = copy.__occlusionPolicy;
    }
    return *this;
}

DepthSortEngine::~DepthSortEngine() = default;

unsigned DepthSortEngine::getIntersectionThreads() const
{
    return __polygonIndex->intersectionThreads;
}

void DepthSortEngine::setIntersectionThreads(unsigned count)
{
    if (count == 0) throw std::invalid_argument("intersectionThreads must be at least 1");
#if defined(PGL_WITH_CGAL) && !defined(CGAL_HAS_THREADS)
    if (count > 1) throw std::invalid_argument("CGAL was built without thread support");
#endif
    if (count == __polygonIndex->intersectionThreads) return;
#ifdef PGL_WITH_CGAL
    __polygonIndex->intersectionPool.reset();
#endif
    __polygonIndex->intersectionThreads = count;
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


Polygon_2 toPolygon(const Point3ArrayPtr& polygon) {
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

static void appendTriangle(std::list<Polygon_2>& triangles,
                           const Point_2& a, const Point_2& b, const Point_2& c)
{
    if (CGAL::area(a, b, c) > GEOM_EPSILON) {
        const Point_2 vertices[] = {a, b, c};
        triangles.emplace_back(std::begin(vertices), std::end(vertices));
    }
}

static bool triangulateConvexQuad(const Polygon_2& boundary, std::list<Polygon_2>& triangles)
{
    const auto& a = boundary[0];
    const auto& b = boundary[1];
    const auto& c = boundary[2];
    const auto& d = boundary[3];
    if (CGAL::orientation(a, b, c) != CGAL::LEFT_TURN ||
        CGAL::orientation(b, c, d) != CGAL::LEFT_TURN ||
        CGAL::orientation(c, d, a) != CGAL::LEFT_TURN ||
        CGAL::orientation(d, a, b) != CGAL::LEFT_TURN)
        return false;

    const auto side = Kernel().side_of_oriented_circle_2_object()(a, b, c, d);
    if (side == CGAL::ON_ORIENTED_BOUNDARY) return false;

    // Preserve the face and vertex order of incremental CDT insertion.
    if (side == CGAL::ON_NEGATIVE_SIDE) {
        appendTriangle(triangles, a, b, c);
        appendTriangle(triangles, d, a, c);
    }
    else {
        appendTriangle(triangles, d, b, c);
        appendTriangle(triangles, d, a, b);
    }
    return true;
}

static void markSmallContourExterior(CDT::Face_handle face)
{
    if (face->info().nesting_level == 0) return;
    face->info().nesting_level = 0;
    for (int edge = 0; edge < 3; ++edge) {
        if (!face->is_constrained(edge))
            markSmallContourExterior(face->neighbor(edge));
    }
}

std::list<Polygon_2> triangulate(const Polygon_with_holes_2& polygon)
{
    std::list<Polygon_2> result;
    const Polygon_2& boundary = polygon.outer_boundary();
    const bool hasHoles = polygon.holes_begin() != polygon.holes_end();
    if (!hasHoles && boundary.size() == 3) {
        appendTriangle(result, boundary[0], boundary[1], boundary[2]);
        return result;
    }
    if (!hasHoles && boundary.size() == 4 && triangulateConvexQuad(boundary, result))
        return result;

    CDT triangulation;
    triangulation.insert_constraint(boundary.vertices_begin(), boundary.vertices_end(), true);
    for (auto hole = polygon.holes_begin(); hole != polygon.holes_end(); ++hole)
        triangulation.insert_constraint(hole->vertices_begin(), hole->vertices_end(), true);

    if (!hasHoles && boundary.size() <= 6 && triangulation.dimension() == 2 &&
        triangulation.number_of_vertices() == boundary.size()) {
        // A single contour has only two domains. With at most six vertices,
        // the exterior flood can use the call stack.
        for (auto face = triangulation.all_faces_begin(); face != triangulation.all_faces_end(); ++face)
            face->info().nesting_level = 1;
        markSmallContourExterior(triangulation.infinite_face());
    }
    else {
        mark_domains(triangulation);
    }

    for (auto face = triangulation.finite_faces_begin(); face != triangulation.finite_faces_end(); ++face) {
        if (face->info().in_domain())
            appendTriangle(result, face->vertex(0)->point(), face->vertex(1)->point(), face->vertex(2)->point());
    }
    return result;
}

std::list<Polygon_2> triangulate(const Pwh_list_2& polygons) 
{
    std::list<Polygon_2> result;
    for(Pwh_list_2::const_iterator it = polygons.begin(); it != polygons.end(); ++it)
    {
        std::list<Polygon_2> cres = triangulate(*it);
        result.splice(result.end(), cres);
    }
    return result;
}


static Vector3 projectVertex(const Vector2& vertex, const Point3Array& polygon, real_t area)
{
    real_t w0 = edgeFunction(polygon.getAt(1), polygon.getAt(2), vertex);
    real_t w1 = edgeFunction(polygon.getAt(2), polygon.getAt(0), vertex);
    real_t w2 = edgeFunction(polygon.getAt(0), polygon.getAt(1), vertex);
    const real_t denominator = fabs(area) <= GEOM_EPSILON ? w0 + w1 + w2 : area;
    w0 /= denominator;
    w1 /= denominator;
    w2 /= denominator;
    return polygon.getAt(0)*w0 + polygon.getAt(1)*w1 + polygon.getAt(2)*w2;
}

Point3ArrayPtr project(const Polygon_2& proj, Point3ArrayPtr polygon)
{
    Point3ArrayPtr result(new Point3Array());
    const real_t area = edgeFunction(polygon->getAt(0), polygon->getAt(1), polygon->getAt(2));
    for (const auto& vertex : proj)
        result->push_back(projectVertex(toVector2(vertex), *polygon, area));
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

static bool allTrianglesAboveAreaThreshold(const Polygon_2& boundary)
{
    // The triangulator drops small faces. Check that no choice of diagonal
    // can drop a boundary vertex from the depth comparison.
    const auto end = boundary.vertices_end();
    for (auto a = boundary.vertices_begin(); a != end; ++a)
        for (auto b = std::next(a); b != end; ++b)
            for (auto c = std::next(b); c != end; ++c)
                if (CGAL::area(*a, *b, *c) <= GEOM_EPSILON)
                    return false;
    return true;
}

struct TriangleOverlap {
    Polygon_2 boundary;
    // Bits 0–2 identify edges of the first triangle, bits 3–5 the second.
    std::array<unsigned, 6> incidentEdges{};
    bool canTraceDifference = false;
    Kernel::FT area = 0;
};

static bool hasAxisAlignedCorner(const Polygon_2& triangle)
{
    bool horizontal = false, vertical = false;
    for (unsigned i = 0; i < 3; ++i) {
        const auto& a = triangle[i];
        const auto& b = triangle[(i + 1) % 3];
        horizontal |= a.y() == b.y();
        vertical |= a.x() == b.x();
    }
    return horizontal && vertical;
}

// Use the original supporting lines, as the general polygon operation does.
static bool differenceVertex(const Polygon_2& subject, const Polygon_2& mask,
                             unsigned subjectEdges, unsigned maskEdges, Point_2& point)
{
    for (unsigned i = 0; i < 3; ++i) {
        const unsigned corner = (1u << i) | (1u << ((i + 2) % 3));
        if ((maskEdges & corner) == corner) {
            point = mask[i];
            return true;
        }
    }

    for (unsigned i = 0; i < 3; ++i) {
        if (subjectEdges != (1u << i)) continue;
        for (unsigned j = 0; j < 3; ++j) {
            if (maskEdges != (1u << j)) continue;
            const Kernel::Line_2 subjectLine(subject[i], subject[(i + 1) % 3]);
            const Kernel::Line_2 maskLine(mask[j], mask[(j + 1) % 3]);
            const CGAL::Object crossing = CGAL::intersection(subjectLine, maskLine);
            return crossing.assign(point);
        }
    }
    return false;
}

static bool noTriangleAboveAreaCutoff(const Polygon_2& boundary)
{
    std::list<Polygon_2> triangles;
    if (boundary.size() == 4 && triangulateConvexQuad(boundary, triangles))
        return triangles.empty();

    const auto end = boundary.vertices_end();
    for (auto a = boundary.vertices_begin(); a != end; ++a)
        for (auto b = std::next(a); b != end; ++b)
            for (auto c = std::next(b); c != end; ++c)
                if (CGAL::abs(CGAL::area(*a, *b, *c)) > GEOM_EPSILON)
                    return false;
    return true;
}

// Follow the subject boundary outside the overlap and the overlap boundary
// in reverse inside the subject. Accept a single contour or full coverage.
static bool triangleDifferenceBoundary(const Polygon_2& subject, const Polygon_2& mask,
                                       const TriangleOverlap& overlap, unsigned edgeOffset,
                                       Polygon_2& boundary, bool useSupportingLines,
                                       std::optional<Kernel::FT>& remainderArea)
{
    constexpr int subjectSize = 3;
    constexpr int maxVertices = 9;
    constexpr int noVertex = -1;
    const int n = static_cast<int>(overlap.boundary.size());
    std::array<unsigned, 6> support{};
    std::array<int, 6> vertex{};
    std::array<int, 3> coveredEdge;
    coveredEdge.fill(noVertex);
    for (int i = 0; i < n; ++i) {
        support[i] = (overlap.incidentEdges[i] >> edgeOffset) & 7u;
        vertex[i] = subjectSize + i;
        for (int k = 0; k < subjectSize; ++k) {
            const unsigned atCorner = (1u << k) | (1u << ((k + 2) % 3));
            if ((support[i] & atCorner) == atCorner) vertex[i] = k;
        }
    }

    std::array<int, maxVertices> successor;
    successor.fill(noVertex);
    int edgeCount = 0;
    auto append = [&](int from, int to) {
        if (from == to) return true;
        if (successor[from] != noVertex) return false;
        successor[from] = to;
        ++edgeCount;
        return true;
    };

    bool sharesEdge = false;
    for (int i = 0; i < n; ++i) {
        const int next = (i + 1) % n;
        const unsigned shared = support[i] & support[next];
        if (shared != 0) {
            for (int edge = 0; edge < subjectSize; ++edge) {
                if (shared != (1u << edge)) continue;
                if (coveredEdge[edge] != noVertex) return false;
                coveredEdge[edge] = i;
                sharesEdge = true;
            }
        }
        else if (!append(vertex[next], vertex[i])) return false;
    }
    if (!sharesEdge) return false;

    for (int edge = 0; edge < subjectSize; ++edge) {
        const int next = (edge + 1) % subjectSize;
        const int covered = coveredEdge[edge];
        // A lone contact in the interior of an edge can pinch the contour.
        for (int i = 0; i < n; ++i) {
            if (vertex[i] >= subjectSize && (support[i] & (1u << edge)) &&
                (covered == noVertex || (i != covered && i != (covered + 1) % n)))
                return false;
        }
        if (covered == noVertex) {
            if (!append(edge, next)) return false;
        }
        else if (!append(edge, vertex[covered]) || !append(vertex[(covered + 1) % n], next))
            return false;
    }
    if (edgeCount == 0) return true;
    if (edgeCount < 3) return false;

    const int start = static_cast<int>(std::find_if(successor.begin(), successor.end(),
        [](int next) { return next != noVertex; }) - successor.begin());
    int current = start;
    bool testDiscard = false;
    for (int count = 0; count < edgeCount; ++count) {
        if (current == noVertex || (count > 0 && current == start)) return false;
        if (current < subjectSize) {
            boundary.push_back(subject[current]);
        }
        else {
            const int index = current - subjectSize;
            const auto& original = overlap.boundary[index];
            if (testDiscard) {
                boundary.push_back(original);
            }
            else {
                const auto x = CGAL::to_interval(original.x());
                const auto y = CGAL::to_interval(original.y());
                if (x.first == x.second && y.first == y.second) {
                    boundary.push_back(Point_2(x.first, y.first));
                }
                else if (useSupportingLines) {
                    const unsigned maskEdges = (overlap.incidentEdges[index] >> (3 - edgeOffset)) & 7u;
                    Point_2 point;
                    if (!differenceVertex(subject, mask, support[index], maskEdges, point)) return false;
                    boundary.push_back(point);
                }
                else {
                    remainderArea = subject.area() - overlap.area;
                    if (*remainderArea <= GEOM_EPSILON) return false;
                    // A simple contour can have at most edgeCount - 2 faces.
                    if (*remainderArea > (edgeCount - 2) * GEOM_EPSILON) return false;
                    testDiscard = true;
                    boundary.push_back(original);
                }
            }
        }
        current = successor[current];
    }
    if (current != start) return false;
    if (testDiscard) {
        if (!noTriangleAboveAreaCutoff(boundary)) return false;
        boundary = Polygon_2();
    }
    return true;
}

static std::vector<Point3ArrayPtr> projectedDifference(const Polygon_2& subject,
                                                       const Polygon_2& mask,
                                                       const Point3ArrayPtr& points,
                                                       const TriangleOverlap* overlap = nullptr,
                                                       unsigned edgeOffset = 0)
{
    std::optional<Kernel::FT> remainderArea;
    if (overlap && overlap->canTraceDifference) {
        Polygon_2 boundary;
        // For aligned right triangles, recompute uncertain vertices directly
        // on their original edges. Other shapes retain the interval check.
        if (triangleDifferenceBoundary(subject, mask, *overlap, edgeOffset, boundary,
                                       hasAxisAlignedCorner(subject) && hasAxisAlignedCorner(mask),
                                       remainderArea)) {
            if (boundary.is_empty()) return {};
            return project(triangulate(Polygon_with_holes_2(boundary)), points);
        }
    }

    // The overlap is contained in the subject. If the entire remainder is
    // below the face-area cutoff, every triangle in it would be discarded.
    if (overlap && !overlap->boundary.is_empty()) {
        if (!remainderArea) remainderArea = subject.area() - overlap->area;
        if (*remainderArea <= GEOM_EPSILON) return {};
    }

    Pwh_list_2 result;
    CGAL::difference(subject, mask, std::back_inserter(result));
    return project(triangulate(result), points);
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
    __polygonIndex->erase(it);
    return __polygonlist.erase(it);
}

DepthSortEngine::PolygonInfoSet::iterator DepthSortEngine::appendPolygon(const PolygonInfo& polygon)
{
    auto it = __polygonlist.insert(__polygonlist.end(), polygon);
    it->insertionOrder = __polygonIndex->nextOrder++;
    __polygonIndex->insert(it);
    return it;
}

DepthSortEngine::PolygonInfoIteratorList DepthSortEngine::appendPolygons(DepthSortEngine::PolygonInfoList::iterator begin, DepthSortEngine::PolygonInfoList::iterator end)
{
    DepthSortEngine::PolygonInfoIteratorList result;
    for(DepthSortEngine::PolygonInfoList::iterator it = begin; it != end; ++it)
        result.push_back(appendPolygon(*it));
    return result;
}

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
    if (__occlusionPolicy != eOccluded){
        _processTriangle(p, 0, __occlusionPolicy == eFullOcclusion);
    }
    else {
        // In occluded mode we just store the polygon.
        appendPolygon(p);
    }
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


enum DEPTHCMP { IN_FRONT = -1, SAMEDEPTH = 0, BEHIND = 1, MIXED = 2 };

static DEPTHCMP compare_depth(real_t a, real_t b)
{
    if (a - b > GEOM_EPSILON) return BEHIND;
    if (a - b < -GEOM_EPSILON) return IN_FRONT;
    return SAMEDEPTH;
}

// Clip the first triangle by the three closed half-planes of the second.
static TriangleOverlap intersectTriangles(const Polygon_2& subject, const Polygon_2& clip)
{
    TriangleOverlap overlap{subject};
    for (std::size_t i = 0; i < 3; ++i)
        overlap.incidentEdges[i] = (1u << i) | (1u << ((i + 2) % 3));
    for (std::size_t edge = 0; edge < 3 && overlap.boundary.size() >= 3; ++edge) {
        const auto& a = clip[edge];
        const auto& b = clip[(edge + 1) % 3];
        const unsigned clipEdge = 1u << (edge + 3);
        TriangleOverlap clipped;
        auto append = [&](const Point_2& point, unsigned incident) {
            clipped.incidentEdges[clipped.boundary.size()] = incident;
            clipped.boundary.push_back(point);
        };
        std::size_t previous = overlap.boundary.size() - 1;
        auto previousSide = CGAL::orientation(a, b, overlap.boundary[previous]);
        for (std::size_t current = 0; current < overlap.boundary.size(); ++current) {
            const auto& from = overlap.boundary[previous];
            const auto& to = overlap.boundary[current];
            const auto side = CGAL::orientation(a, b, to);
            if (previousSide != CGAL::COLLINEAR && side != CGAL::COLLINEAR && side != previousSide) {
                const auto before = CGAL::area(a, b, from);
                const auto after = CGAL::area(a, b, to);
                const Kernel::FT fraction = before / (before - after);
                append(from + (to - from) * fraction,
                       (overlap.incidentEdges[previous] & overlap.incidentEdges[current]) | clipEdge);
            }
            if (side != CGAL::RIGHT_TURN)
                append(to, overlap.incidentEdges[current] | (side == CGAL::COLLINEAR ? clipEdge : 0u));
            previous = current;
            previousSide = side;
        }
        overlap = std::move(clipped);
    }
    return overlap;
}

static bool compareOverlapDepth(const Polygon_2& p1, const Polygon_2& p2,
                                const Point3Array& polygon1, const Point3Array& polygon2,
                                DEPTHCMP& comparison, std::list<Polygon_2>& overlapTriangles,
                                TriangleOverlap& specializedOverlap)
{
    if (p1.area() < GEOM_EPSILON || p2.area() < GEOM_EPSILON) return false;
    assert(p1.is_counterclockwise_oriented());
    assert(p2.is_counterclockwise_oriented());

    const real_t area1 = edgeFunction(polygon1.getAt(0), polygon1.getAt(1), polygon1.getAt(2));
    const real_t area2 = edgeFunction(polygon2.getAt(0), polygon2.getAt(1), polygon2.getAt(2));
    auto compareVertices = [&](const Polygon_2& polygon, DEPTHCMP depth, bool* nearTie = nullptr) {
        for (const auto& point : polygon) {
            const Vector2 vertex = toVector2(point);
            const real_t z1 = projectVertex(vertex, polygon1, area1).z();
            const real_t z2 = projectVertex(vertex, polygon2, area2).z();
            if (nearTie) {
                const real_t scale = std::max({real_t(1), std::abs(z1), std::abs(z2)});
                const real_t margin = 64 * std::numeric_limits<real_t>::epsilon() * scale;
                *nearTie |= std::abs(std::abs(z1 - z2) - GEOM_EPSILON) <= margin;
            }
            const DEPTHCMP atVertex = compare_depth(z1, z2);
            if (depth == SAMEDEPTH) depth = atVertex;
            else if (atVertex == -depth) return MIXED;
        }
        return depth;
    };

    if (p1.size() == 3 && p2.size() == 3) {
        TriangleOverlap clipped = intersectTriangles(p1, p2);
        const auto& boundary = clipped.boundary;
        // A point or edge contact has no projected area.
        if (boundary.size() < 3) return false;
        clipped.area = boundary.area();
        if (clipped.area == 0) return false;
        bool nearTie = false;
        comparison = compareVertices(boundary, SAMEDEPTH, &nearTie);
        clipped.canTraceDifference =
            !nearTie && (comparison != MIXED || (hasAxisAlignedCorner(p1) && hasAxisAlignedCorner(p2))) &&
            allTrianglesAboveAreaThreshold(boundary);
        specializedOverlap = std::move(clipped);
        if (specializedOverlap.canTraceDifference && comparison != MIXED) return true;
    }

    // Keep CGAL's original boundary for mixed-depth clipping and for cases
    // where the area or depth thresholds can affect the decision.
    Pwh_list_2 overlap;
    CGAL::intersection(p1, p2, std::back_inserter(overlap));
    if (overlap.empty()) return false;

    const Polygon_2& boundary = overlap.front().outer_boundary();
    const bool singleBoundary = overlap.size() == 1 &&
        overlap.front().holes_begin() == overlap.front().holes_end();
    if (singleBoundary) {
        if (boundary.size() == 3 && boundary.area() <= GEOM_EPSILON) return false;
        // The depth difference is affine over the overlap of the two triangles.
        comparison = compareVertices(boundary, SAMEDEPTH);
        if (comparison != MIXED &&
            (boundary.size() == 3 || allTrianglesAboveAreaThreshold(boundary)))
            return true;
        if (boundary.size() == 3) {
            overlapTriangles.push_back(boundary);
            return true;
        }
    }

    // Mixed depths need triangles for clipping. Thin overlaps use the
    // retained faces for depth comparison as well, preserving the area threshold.
    overlapTriangles = triangulate(overlap);
    if (overlapTriangles.empty()) return false;
    if (singleBoundary && comparison == MIXED && overlapTriangles.size() == boundary.size() - 2)
        return true;
    comparison = SAMEDEPTH;
    for (const auto& triangle : overlapTriangles) {
        comparison = compareVertices(triangle, comparison);
        if (comparison == MIXED) break;
    }
    return true;
}

struct CandidateIntersection {
    Polygon_2 incoming;
    Polygon_2 indexed;
    std::list<Polygon_2> overlapTriangles;
    TriangleOverlap overlap;
    DEPTHCMP depth = SAMEDEPTH;
    bool intersects = false;
    std::exception_ptr error;
};

static CandidateIntersection prepareIntersection(const Point3ArrayPtr& incoming,
                                                 const Point3ArrayPtr& indexed)
{
    CandidateIntersection result;
    // Build independent CGAL values per pair. Lazy exact coordinates can
    // mutate their internal caches even during nominally const operations.
    result.incoming = toPolygon(incoming);
    result.indexed = toPolygon(indexed);
    result.intersects = compareOverlapDepth(result.incoming, result.indexed,
        *incoming, *indexed, result.depth, result.overlapTriangles, result.overlap);
    return result;
}

class CandidateIntersectionBatch {
public:
    CandidateIntersectionBatch(const Point3ArrayPtr& incoming,
        const std::vector<DepthSortEngine::PolygonInfoList::iterator>& candidates,
        std::size_t count) : incoming(incoming), slots(count)
    {
        for (std::size_t i = 0; i < count; ++i)
            slots[i].points = candidates[candidates.size() - 1 - i]->points;
    }

    ~CandidateIntersectionBatch()
    {
        cancel();
        for (auto& worker : workers) worker.wait();
    }

    void start(boost::asio::thread_pool& pool, std::size_t threadCount)
    {
        const std::size_t count = std::min(threadCount, slots.size()) - 1;
        workers.reserve(count);
        for (std::size_t i = 0; i < count; ++i) {
            std::packaged_task<void()> task([this] { prepare(); });
            auto future = task.get_future();
            boost::asio::post(pool, std::move(task));
            workers.push_back(std::move(future));
        }
    }

    void cancel() { cancelled.store(true, std::memory_order_relaxed); }
    bool finished() const { return consumed == slots.size(); }

    CandidateIntersection take()
    {
        Slot& slot = slots[consumed++];
        if (consumed == 1) return prepareIntersection(incoming, slot.points);

        std::unique_lock<std::mutex> lock(mutex);
        ready.wait(lock, [&] { return slot.ready; });
        return std::move(slot.result);
    }

private:
    struct Slot {
        Point3ArrayPtr points;
        CandidateIntersection result;
        bool ready = false;
    };

    void prepare()
    {
        while (!cancelled.load(std::memory_order_relaxed)) {
            const std::size_t index = next.fetch_add(1, std::memory_order_relaxed);
            if (index >= slots.size()) break;
            Slot& slot = slots[index];
            try {
                slot.result = prepareIntersection(incoming, slot.points);
            }
            catch (...) {
                // A speculative error matters only if this result is consumed.
                slot.result.error = std::current_exception();
            }
            {
                std::lock_guard<std::mutex> lock(mutex);
                slot.ready = true;
            }
            ready.notify_one();
        }
    }

    Point3ArrayPtr incoming;
    std::vector<Slot> slots;
    std::vector<std::future<void>> workers;
    std::mutex mutex;
    std::condition_variable ready;
    std::atomic<std::size_t> next{1};
    std::atomic<bool> cancelled{false};
    std::size_t consumed = 0;
};

void
DepthSortEngine::_processTriangle( PolygonInfo polygon,
                                   uint32_t begin,
                                   bool insertTriangle)
{

    bool processed = false;

    PolygonInfoList toAppend;
    auto first = __polygonlist.begin();
    std::advance(first, std::min<std::size_t>(begin, __polygonlist.size()));
    auto queryFrom = [&](PolygonInfoList::iterator from) {
        return __polygonIndex->query(polygon, from == __polygonlist.end()
            ? __polygonIndex->nextOrder : from->insertionOrder);
    };
    auto candidates = queryFrom(first);
    std::optional<CandidateIntersectionBatch> batch;
    constexpr std::size_t minParallelCandidates = 16;
    constexpr std::size_t maxParallelCandidates = 32;
    const std::size_t threadCount =
        std::min<std::size_t>(__polygonIndex->intersectionThreads, maxParallelCandidates);
    while (!candidates.empty()) {
        if (batch && batch->finished()) batch.reset();
        if (!batch && threadCount > 1 && candidates.size() >= minParallelCandidates) {
            const std::size_t batchSize = std::min({candidates.size(), threadCount * 4, maxParallelCandidates});
            if (!__polygonIndex->intersectionPool)
                __polygonIndex->intersectionPool = std::make_unique<boost::asio::thread_pool>(threadCount - 1);
            batch.emplace(polygon.points, candidates, batchSize);
            batch->start(*__polygonIndex->intersectionPool, threadCount);
        }

        auto it = candidates.back();
        candidates.pop_back();
        const PolygonInfo& current = *it;
        const uint32_t itid = current.id;
        CandidateIntersection result = batch ? batch->take()
            : prepareIntersection(polygon.points, current.points);
        if (result.error) std::rethrow_exception(result.error);
        if (!result.intersects) continue;

        const auto& projectedPolygon = result.incoming;
        const auto& projectedCurrent = result.indexed;
        const auto& overlapTriangles = result.overlapTriangles;
        const auto& specializedOverlap = result.overlap;
        const DEPTHCMP depthcmp = result.depth;
        if (depthcmp > 0 && batch) batch->cancel();

        if (depthcmp <= 0) {
            // new polygon hide the previous one.
            // we remove the previous one and replace it by its non overlaping part.
            PolygonInfoList mrestCurrent = _toPolygonInfo(
                projectedDifference(projectedCurrent, projectedPolygon, current.points, &specializedOverlap, 3), itid);
            removePolygon(it);

            toAppend.insert(toAppend.end(), mrestCurrent.begin(), mrestCurrent.end());
        }
        else if (depthcmp == BEHIND) {
            // Keep the previous polygon and continue with the visible remainder.
            PolygonInfoList mrestPolygon = _toPolygonInfo(
                projectedDifference(projectedPolygon, projectedCurrent, polygon.points, &specializedOverlap), polygon.id);
            if (mrestPolygon.empty()){
                processed = true;
                break;
            }
            else if(mrestPolygon.size() == 1) {
                polygon = mrestPolygon.front();
                batch.reset();
                candidates = queryFrom(std::next(it));
                continue;
            }
            else {
                batch.reset();
                uint32_t dist = std::distance( __polygonlist.begin(),it)+1;
                for (const auto& rest : mrestPolygon)
                    _processTriangle(rest, dist, insertTriangle);
                processed = true;
                break;
            }
        }
        else if (depthcmp == MIXED) {
            PolygonInfoList mintersection1 = _toPolygonInfo(project(overlapTriangles, polygon.points), polygon.id);
            PolygonInfoList mintersection2 = _toPolygonInfo(project(overlapTriangles, current.points), itid);
            // The remainder of the incoming polygon may be processed recursively.
            // Preserve CGAL's fragment order on that path.
            PolygonInfoList mrestPolygon = _toPolygonInfo(
                projectedDifference(projectedPolygon, projectedCurrent, polygon.points), polygon.id);
            PolygonInfoList mrestCurrent = _toPolygonInfo(
                projectedDifference(projectedCurrent, projectedPolygon, current.points, &specializedOverlap, 3), itid);

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

            toAppend.insert(toAppend.end(), mrestCurrent.begin(), mrestCurrent.end());

            if (!mrestPolygon.empty()){
                if(mrestPolygon.size() == 1) {
                    polygon = mrestPolygon.front();
                    batch.reset();
                    // Preserve the legacy mixed-depth traversal: skip the successor,
                    // or restart at the front when the erased node was last.
                    candidates = queryFrom(it == __polygonlist.end()
                        ? __polygonlist.begin() : std::next(it));
                    continue;
                }
                else {
                    batch.reset();
                    uint32_t dist = std::distance( __polygonlist.begin(),it)+1;
                    for (const auto& rest : mrestPolygon)
                        _processTriangle(rest, dist, insertTriangle);
                    processed = true;
                    break;
                }
            }


            processed = true;
            break;
        }
    }
    if (!processed && insertTriangle) {
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

void DepthSortEngine::process(ScenePtr scene, ScenePtr occludedOnly, ScenePtr occludingOnly) {
    Discretizer d;
    Tesselator t;
    ProjectionRenderer r(*this, t, d);
    eOcclusionPolicy prevPolicy = getOcclusionPolicy();
    beginProcess();
    if (is_valid_ptr(occludedOnly)){
        setOcclusionPolicy(eOccluded);
        occludedOnly->apply(r);
    }
    if (is_valid_ptr(scene)){
        setOcclusionPolicy(eFullOcclusion);
        scene->apply(r);
    } 
    if (is_valid_ptr(occludingOnly)){
        setOcclusionPolicy(eOccluding);
        occludingOnly->apply(r);
    }
    endProcess();
    setOcclusionPolicy(prevPolicy); 
}


#endif
