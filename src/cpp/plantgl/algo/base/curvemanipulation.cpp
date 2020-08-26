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


#include "curvemanipulation.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/container/geometryarray2.h>
#include <plantgl/scenegraph/geometry/group.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/geometry/pointset.h>

PGL_USING_NAMESPACE

#ifdef PGL_WITH_CGAL
#include <CGAL/basic.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_overlay_2.h>
#include <CGAL/Arr_default_overlay_traits.h>
#ifdef PGL_CGAL_USE_GMP
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

inline Vector2 toVec2(const Point_2& p) { return Vector2(to_double(p.x()),to_double(p.y())); }

inline Point_2 toPoint2(const Vector2& p) { return Point_2(p.x(),p.y()); }
inline Segment_2 toSegment(const Vector2& p1,const Vector2& p2) { return Segment_2(toPoint2(p1),toPoint2(p2)); }
#endif

GeometryPtr
Overlay::process(const Polyline2DPtr& p1, const Polyline2DPtr& p2)
{
    if (!p1 || !p2 || p1->getPointListSize() < 2 || p2->getPointListSize() < 2) return GeometryPtr();
#ifdef PGL_WITH_CGAL

  // Construct the first arrangement, containing a polyline 1.
  Arrangement_2          arr1;

  for (Point2Array::const_iterator it1 = p1->getPointList()->begin()+1; it1 != p1->getPointList()->end(); ++it1)
    insert_non_intersecting_curve(arr1,toSegment(*(it1-1),*it1));

  // to be a closed face, first and last point should be exactly the same.
  // However we should not duplicate the same point twice at the end.
  Vector2& fp1 = p1->getPointList()->getAt(0);
  Vector2& lp1 = *(p1->getPointList()->end()-1);
  if (fp1.x() != lp1.x() || fp1.y() != lp1.y())
    insert_non_intersecting_curve(arr1,toSegment(lp1,fp1));

  // std::cerr << arr1.number_of_vertices() << " " << arr1.number_of_edges() << " " << arr1.number_of_faces() << std::endl;

  // Mark just the bounded face.
  Arrangement_2::Face_iterator   fit;

  CGAL_assertion (arr1.number_of_faces() == 2);
  for (fit = arr1.faces_begin(); fit != arr1.faces_end(); ++fit)
    fit->set_data (fit != arr1.unbounded_face());

  // Construct the second arrangement.
  Arrangement_2          arr2;

  for (Point2Array::const_iterator it2 = p2->getPointList()->begin()+1; it2 != p2->getPointList()->end(); ++it2)
         insert(arr2,toSegment(*(it2-1),*it2));

  // to be a closed face, first and last point should be exactly the same.
  // However we should not duplicate the same point twice at the end.
  Vector2& fp2 = p2->getPointList()->getAt(0);
  Vector2& lp2 = *(p2->getPointList()->end()-1);
  if (fp2.x() != lp2.x() || fp2.y() != lp2.y())
    insert(arr2,toSegment(lp2,fp2));

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
      pointSet->push_back(toVec2(curr->target()->point()));
      ++curr;
    } while (curr != face->outer_ccb());
    if (pointSet->size() == 1){
        geomarray->push_back(GeometryPtr(new PointSet2D(pointSet)));
    }
    else if(pointSet->size() > 1){
        geomarray->push_back(GeometryPtr(new Polyline2D(pointSet)));
    }
  }
  if (geomarray->empty())return GeometryPtr();
  else if (geomarray->size() == 1) return geomarray->getAt(0);
  else return GeometryPtr(new Group(geomarray));

#else
#ifdef _MSC_VER
#pragma message("CGAL not included. Overlay routine will not work.")
#else
#warning "CGAL not included. Overlay routine will not work."
#endif
    pglError("CGAL not included. Overlay routine will not work.");
    return GeometryPtr();
#endif

}

#ifdef PGL_WITH_CGAL
#include <CGAL/Sweep_line_2_algorithms.h>
#endif

Point2ArrayPtr
CurveIntersection::compute(const std::vector<Polyline2DPtr>& lines)
{
#ifdef PGL_WITH_CGAL
   // Construct the first arrangement, containing a polyline 1.
    std::list<Segment_2>   arrs;
   // std::vector<Segment_2> segments;
    for(std::vector<Polyline2DPtr>::const_iterator itLines = lines.begin(); itLines != lines.end(); ++itLines){
      // Arrangement_2   arr;
      for (Point2Array::const_iterator it1 = (*itLines)->getPointList()->begin()+1; it1 != (*itLines)->getPointList()->end(); ++it1)
         // insert_non_intersecting_curve(arrs,Segment_2(Point_2((it1-1)->x(),(it1-1)->y()),Point_2(it1->x(),it1->y())));
         if (norm(*(it1-1)-*it1) > GEOM_EPSILON)
            arrs.push_back(toSegment(*(it1-1),*it1));
    }

   std::list<Point_2>     pts;
   // note the endpoints bool value, whether we include the segment points in the count or not
   CGAL::compute_intersection_points (arrs.begin(), arrs.end(), std::back_inserter (pts), false);
   Point2ArrayPtr     respts(new Point2Array());
   for(std::list<Point_2>::const_iterator itPoints = pts.begin(); itPoints != pts.end(); ++itPoints)
       respts->push_back(toVec2(*itPoints));
   return respts;
#else
#ifdef _MSC_VER
#pragma message("CGAL not included. CurveIntersection routine will not work.")
#else
#warning "CGAL not included. CurveIntersection routine will not work."
#endif
    pglError("CGAL not included. CurveIntersection routine will not work.");
    return Point2ArrayPtr();
#endif
}

bool
CurveIntersection::check(const std::vector<Polyline2DPtr>& lines)
{
#ifdef PGL_WITH_CGAL
   // Construct the first arrangement, containing a polyline 1.
    std::list<Segment_2>   arrs;
   // std::vector<Segment_2> segments;
    for(std::vector<Polyline2DPtr>::const_iterator itLines = lines.begin(); itLines != lines.end(); ++itLines){
      // Arrangement_2   arr;
      for (Point2Array::const_iterator it1 = (*itLines)->getPointList()->begin()+1; it1 != (*itLines)->getPointList()->end(); ++it1)
         // insert_non_intersecting_curve(arrs,Segment_2(Point_2((it1-1)->x(),(it1-1)->y()),Point_2(it1->x(),it1->y())));
         if (norm(*(it1-1)-*it1) > GEOM_EPSILON)
            arrs.push_back(toSegment(*(it1-1),*it1));
    }

   std::list<Point_2>     pts;
   // check whether an intersection exists
   return CGAL::do_curves_intersect (arrs.begin(), arrs.end());
#else
#ifdef _MSC_VER
#pragma message("CGAL not included. CurveIntersection routine will not work.")
#else
#warning "CGAL not included. CurveIntersection routine will not work."
#endif
    pglError("CGAL not included. CurveIntersection routine will not work.");
    return false;
#endif

}

#ifdef PGL_WITH_CGAL

typedef Kernel::Ray_3 CGALRay3;
typedef Kernel::Point_3 CGALPoint_3 ;
typedef Kernel::Direction_3 CGALDirection_3 ;
typedef Kernel::Segment_3 CGALSegment_3 ;

inline CGALPoint_3 toPoint3(const Vector3& v) { return CGALPoint_3(v.x(),v.y(),v.z()); }
inline CGALDirection_3 toDirection3(const Vector3& v) { return CGALDirection_3(v.x(),v.y(),v.z()); }
inline CGALSegment_3 toSegment3(const Vector3& u, const Vector3& v) { return CGALSegment_3(toPoint3(u),toPoint3(v)); }
#endif

real_t raySegmentDistance(const Ray& ray, const Vector3& segA,const Vector3& segB)
{
#ifdef PGL_WITH_CGAL
    Kernel::Compute_squared_distance_3 distcomputer;
    return to_double(distcomputer(CGALRay3(toPoint3(ray.getOrigin()),toDirection3(ray.getDirection())),toSegment3(segA,segB)));

#else
#ifdef _MSC_VER
#pragma message("CGAL not included. CurveIntersection routine will not work.")
#else
#warning "CGAL not included. CurveIntersection routine will not work."
#endif
    pglError("CGAL not included. CurveIntersection routine will not work.");
    return false;
#endif

}


/*
#ifdef PGL_WITH_CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#endif

IndexArrayPtr
determine_faces(const Point2ArrayPtr& points, std::vector<std::pair<uint32_t, uint32_t> > edges)
{
#ifdef PGL_WITH_CGAL
    typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
    typedef CGAL::Triangulation_vertex_base_2<K> Vb;
    typedef CGAL::Constrained_triangulation_face_base_2<K> Fb;
    typedef CGAL::Triangulation_data_structure_2<Vb,Fb> TDS;
    typedef CGAL::Exact_predicates_tag Itag;
    typedef CGAL::Constrained_Delaunay_triangulation_2<K, TDS, Itag> CDT;
    typedef CDT::Point Point;

     CDT cdt;
     for(std::vector<std::pair<uint32_t, uint32_t> >::const_iterator itedges = edges.begin(); itedges != edges.end(); ++itedges)
         cdt.insert_constraint( toPoint<Point>(points->getAt(itedges->first),toPoint<Point>(points->getAt(itedges->second));

    assert(cdt.is_valid());
    int count = 0;
    std::vector<pgl_hash_set<uint32_t> > trgroups;
    typedef pgl_hash_map<uint32_t,uint32_t> Tr2Map;
    Tr2Map tr2group;


    for (CDT::Finite_edges_iterator eit = cdt.finite_edges_begin(); eit != cdt.finite_edges_end(); ++eit){
        TDS::Face_handle face = eit->first;
        int vertex = eit->second;
        TDS::Face_handle nbg face->neighbor(vertex);
        Tr2Map::const_iterator ittrmap =  tr2group.find((uint32_t)face);
        int groupf1 = (ittrmap == tr2group.end()?-1:*ittrmap);
        Tr2Map::const_iterator ittrmap =  tr2group.find((uint32_t)nbg);
        int groupf2 = (ittrmap == tr2group.end()?-1:*ittrmap);

        if (cdt.is_constrained(*eit)){

        }

    }


#else
    return IndexArrayPtr();
#endif
}

*/

#include "../fitting/planargraph.h"

IndexArrayPtr PGL(determine_faces_from_edges)(const Point2ArrayPtr& points, const std::vector<std::pair<uint32_t, uint32_t> >& edges)
{
    typedef Wm5::PlanarGraph<Vector2> Graph;
    Graph graph;
    int i = 0;
    for (Point2Array::const_iterator it = points->begin(); it != points->end(); ++it)
        graph.InsertVertex(*it,i++);
    for (std::vector<std::pair<uint32_t, uint32_t> >::const_iterator itedge = edges.begin(); itedge != edges.end(); ++itedge)
        graph.InsertEdge(itedge->first,itedge->second);

    std::vector<Graph::Primitive*> mPrimitives;
    graph.ExtractPrimitives(mPrimitives);


    IndexArrayPtr result(new IndexArray());
    for (std::vector<Graph::Primitive*>::const_iterator itPrim = mPrimitives.begin(); itPrim != mPrimitives.end(); ++itPrim){
        if ((*itPrim)->Type == Graph::PT_MINIMAL_CYCLE){
            Index lresult;
            for (std::vector<std::pair<Vector2,int> >::const_iterator itSequence = (*itPrim)->Sequence.begin(); itSequence != (*itPrim)->Sequence.end(); ++itSequence)
                lresult.push_back(itSequence->second);
            result->push_back(lresult);
        }
    }

    return result;
}

IndexArrayPtr PGL(determine_faceedges_from_edges)(const Point2ArrayPtr& points, const std::vector<std::pair<uint32_t, uint32_t> >& edges)
{
    IndexArrayPtr presult = determine_faces_from_edges(points, edges);
    pgl_hash_map<uint32_t, pgl_hash_map<uint32_t,uint32_t> > pidedgemap;
    uint32_t eid = 0;
    for (std::vector<std::pair<uint32_t, uint32_t> >::const_iterator itedge = edges.begin(); itedge != edges.end(); ++itedge, ++eid)
    {
        pgl_hash_map<uint32_t, pgl_hash_map<uint32_t,uint32_t> >::const_iterator fpid = pidedgemap.find(std::min(itedge->first,itedge->second));
        pgl_hash_map<uint32_t,uint32_t> l;
        if  (fpid != pidedgemap.end()) l = fpid->second;
        l[std::max(itedge->first,itedge->second)] = eid;
        pidedgemap[std::min(itedge->first,itedge->second)] = l;

        /*fpid = pidedgemap.find(itedge->second);
        l = (fpid == pidedgemap.end()?pgl_hash_map<uint32_t,uint32_t>():fpid->second);
        l[itedge->first] = eid;
        pidedgemap[itedge->second] = l;*/
    }

    IndexArrayPtr eresult(new IndexArray());
    for(IndexArray::const_iterator itpid = presult->begin(); itpid != presult->end(); ++itpid){
        Index leres;
        uint32_t pidsetsize = itpid->size();
        for (uint32_t i = 0; i < pidsetsize; ++i){
            uint32_t p1 = itpid->getAt(i);
            uint32_t p2 = itpid->getAt((i!=pidsetsize-1?i+1:0));
            leres.push_back(pidedgemap[std::min(p1,p2)][std::max(p1,p2)]);

        }
        eresult->push_back(leres);
    }
    return eresult;
}
