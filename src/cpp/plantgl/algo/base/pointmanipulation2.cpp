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
#include <plantgl/scenegraph/container/indexarray_iterator.h>

PGL_USING_NAMESPACE


#ifdef WITH_CGAL

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
// #include <CGAL/Triangulation_3.h>

#include "cgalwrap.h"

# ifdef WITH_EIGEN
#   define CGAL_AND_SVD_SOLVER_ENABLED
# else
#  ifdef WITH_LAPACK
#   define CGAL_AND_SVD_SOLVER_ENABLED
#  endif

# endif


#ifdef CGAL_AND_SVD_SOLVER_ENABLED

# ifdef WITH_LAPACK
#define CGAL_LAPACK_ENABLED
# else
#define CGAL_EIGEN3_ENABLED
# endif

#include <CGAL/Monge_via_jet_fitting.h>

#endif

#include <CGAL/Cartesian.h>

#endif

IndexArrayPtr
PGL::delaunay_point_connection(const Point3ArrayPtr points) {
#ifdef WITH_CGAL

  typedef CGAL::Exact_predicates_inexact_constructions_kernel TK;
  typedef CGAL::Triangulation_vertex_base_with_info_3<uint32_t, TK> TVb;
  typedef CGAL::Triangulation_data_structure_3<TVb> Tds;

  typedef CGAL::Delaunay_triangulation_3<TK, Tds> Triangulation;
  // typedef CGAL::Triangulation_3<K,Tds>      Triangulation;


  typedef Triangulation::Cell_handle TCell_handle;
  typedef Triangulation::Vertex_handle TVertex_handle;
  typedef Triangulation::Locate_type TLocate_type;
  typedef Triangulation::Point TPoint;
  typedef Triangulation::Segment TSegment;

  Triangulation triangulation;
  uint32_t pointCount = 0;
  for (Point3Array::const_iterator it = points->begin(); it != points->end(); ++it)
    triangulation.insert(toPoint3<TPoint>(*it))->info() = pointCount++;


  IndexArrayPtr result(new IndexArray(points->size(), Index()));
  for (Triangulation::Finite_edges_iterator it = triangulation.finite_edges_begin();
       it != triangulation.finite_edges_end(); ++it) {
    uint32_t source = it->first->vertex(it->second)->info();
    uint32_t target = it->first->vertex(it->third)->info();
    result->getAt(source).push_back(target);
    result->getAt(target).push_back(source);
  }
#else
#ifdef _MSC_VER
#pragma message("function 'delaunay_point_connection' disabled. CGAL needed.")
#else
#warning "function 'delaunay_point_connection' disabled. CGAL needed"
#endif

  IndexArrayPtr result;
#endif
  return result;
}

Index3ArrayPtr
PGL::delaunay_triangulation(const Point3ArrayPtr points) {
#ifdef WITH_CGAL

  typedef CGAL::Exact_predicates_inexact_constructions_kernel TK;
  typedef CGAL::Triangulation_vertex_base_with_info_3<uint32_t, TK> TVb;
  typedef CGAL::Triangulation_data_structure_3<TVb> Tds;

  typedef CGAL::Delaunay_triangulation_3<TK, Tds> Triangulation;
  // typedef CGAL::Triangulation_3<K,Tds>      Triangulation;


  typedef Triangulation::Cell_handle TCell_handle;
  typedef Triangulation::Vertex_handle TVertex_handle;
  typedef Triangulation::Locate_type TLocate_type;
  typedef Triangulation::Point TPoint;
  typedef Triangulation::Segment TSegment;

  Triangulation triangulation;
  uint32_t pointCount = 0;
  for (Point3Array::const_iterator it = points->begin(); it != points->end(); ++it)
    triangulation.insert(toPoint3<TPoint>(*it))->info() = pointCount++;


  Index3ArrayPtr result(new Index3Array());
  for (Triangulation::Finite_facets_iterator it = triangulation.finite_facets_begin();
       it != triangulation.finite_facets_end(); ++it) {
    Index3 ind;
    int j = 0;
    for (int i = 0; i < 4; ++i) {
      if (i != it->second) {
        ind[j] = it->first->vertex(i)->info();
        ++j;
      }
    }
    result->push_back(ind);
  }
#else
#ifdef _MSC_VER
#pragma message("function 'delaunay_point_connection' disabled. CGAL needed.")
#else
#warning "function 'delaunay_point_connection' disabled. CGAL needed"
#endif

  Index3ArrayPtr result;
#endif
  return result;
}


#ifdef WITH_CGAL

#include <CGAL/linear_least_squares_fitting_3.h>

#endif

std::pair<Vector3, Vector3> PGL::pointset_plane(const Point3ArrayPtr points, const Index &group) {
#ifdef WITH_CGAL
  typedef CGAL::Cartesian<real_t> CK;
  typedef CK::Point_3 CPoint;
  typedef CK::Plane_3 CPlane;

  std::list<CPoint> pointdata;
  if (!group.empty())
    pointdata = toPoint3List<CPoint>(points, group);
  else
    
    pointdata = toPoint3List<CPoint>(points);

  CPlane plane;
  CPoint center;
  linear_least_squares_fitting_3(pointdata.begin(), pointdata.end(), plane, center, CGAL::Dimension_tag<0>());

  Vector3 p = toVector3(center);
  Vector3 dir = dir2Vector3(plane.orthogonal_direction());
  return std::pair<Vector3, Vector3>(p, dir);
#else
#ifdef _MSC_VER
#pragma message("function 'pointset_plane' disabled. CGAL needed.")
#else
#warning "function 'pointset_plane' disabled. CGAL needed"
#endif

  return std::pair<Vector3, Vector3>(Vector3(0,0,0), Vector3(0,0,0));
#endif
}

Vector3 PGL::pointset_orientation(const Point3ArrayPtr points, const Index &group) {
#ifdef WITH_CGAL
  typedef CGAL::Cartesian<real_t> CK;
  typedef CK::Point_3 CPoint;
  typedef CK::Line_3 CLine;

  std::list<CPoint> pointdata;
  if (!group.empty())
    pointdata = toPoint3List<CPoint>(points, group);
  else
    pointdata = toPoint3List<CPoint>(points);

  CLine line;
  linear_least_squares_fitting_3(pointdata.begin(), pointdata.end(), line, CGAL::Dimension_tag<0>());

  return toVector3(line.to_vector());
#else
#ifdef _MSC_VER
#pragma message("function 'pointset_orientation' disabled. CGAL needed.")
#else
#warning "function 'pointset_orientation' disabled. CGAL needed"
#endif

  return Vector3(0,0,0);
#endif
}

ALGO_API Vector3
PGL::triangleset_orientation(const Point3ArrayPtr points, const Index3ArrayPtr triangles) {
#ifdef WITH_CGAL
  typedef CGAL::Cartesian<real_t> CK;
  typedef CK::Point_3 CPoint;
  typedef CK::Line_3 CLine;
  typedef CK::Triangle_3 CTriangle;

  std::list<CTriangle> cgaltriangles;
  for (Index3Array::const_iterator it = triangles->begin(); it != triangles->end(); ++it)
    cgaltriangles.push_back(
            CTriangle(toPoint3<CPoint>(points->getAt(it->getAt(0))),
                      toPoint3<CPoint>(points->getAt(it->getAt(1))),
                      toPoint3<CPoint>(points->getAt(it->getAt(2)))
            ));


  CLine line;
  linear_least_squares_fitting_3(cgaltriangles.begin(), cgaltriangles.end(), line, CGAL::Dimension_tag<0>());

  return toVector3(line.to_vector());
#else
#ifdef _MSC_VER
#pragma message("function 'pointset_orientation' disabled. CGAL needed.")
#else
#warning "function 'pointset_orientation' disabled. CGAL needed"
#endif

  return Vector3(0,0,0);
#endif
}


CurvatureInfo
PGL::principal_curvatures(const Point3ArrayPtr points, uint32_t pid, const Index &group, size_t fitting_degree, size_t monge_degree) {
  CurvatureInfo result;
#ifdef CGAL_AND_SVD_SOLVER_ENABLED

  typedef CGAL::Cartesian<real_t>  Data_Kernel;
  typedef Data_Kernel::Point_3     DPoint;
  typedef CGAL::Monge_via_jet_fitting<Data_Kernel> My_Monge_via_jet_fitting;
  typedef My_Monge_via_jet_fitting::Monge_form     My_Monge_form;

std::vector<DPoint> in_points;
in_points.push_back(toPoint3<DPoint>(points->getAt(pid)));

for(Index::const_iterator itNg = group.begin(); itNg != group.end(); ++itNg)
    if (*itNg != pid) in_points.push_back(toPoint3<DPoint>(points->getAt(*itNg)));

My_Monge_form monge_form;
My_Monge_via_jet_fitting monge_fit;
monge_form = monge_fit(in_points.begin(), in_points.end(), fitting_degree, monge_degree);

result.origin = toVector3(monge_form.origin());
result.maximal_principal_direction = toVector3(monge_form.maximal_principal_direction());
result.maximal_curvature = monge_form.principal_curvatures(0);
result.minimal_principal_direction = toVector3(monge_form.minimal_principal_direction());
result.minimal_curvature = monge_form.principal_curvatures(1);
result.normal = toVector3(monge_form.normal_direction());

#else
#ifdef _MSC_VER
#pragma message("function 'principal_curvatures' disabled. CGAL and LAPACK or EIGEN needed.")
#else
#warning "function 'principal_curvatures' disabled. CGAL and LAPACK or EIGEN needed"
#endif
#endif
  return result;

}

std::vector<CurvatureInfo>
PGL::principal_curvatures(const Point3ArrayPtr points, const IndexArrayPtr groups, size_t fitting_degree, size_t monge_degree) {
  std::vector<CurvatureInfo> result;
  uint32_t i = 0;
  for (IndexArray::const_iterator it = groups->begin(); it != groups->end(); ++it, ++i)
    result.push_back(principal_curvatures(points, i, *it, fitting_degree, monge_degree));
  return result;
}

std::vector<CurvatureInfo>
PGL::principal_curvatures(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, real_t radius, size_t fitting_degree, size_t monge_degree) {
  std::vector<CurvatureInfo> result;
  uint32_t nbPoints = points->size();

  for (uint32_t i = 0; i < nbPoints; ++i) {
    Index ng = r_neighborhood(i, points, adjacencies, radius);
    result.push_back(principal_curvatures(points, i, ng, fitting_degree, monge_degree));
  }
  return result;

}

Vector3 PGL::pointset_normal(const Point3ArrayPtr points, const Index &group) {
#ifdef WITH_CGAL
  typedef CGAL::Cartesian<real_t> CK;
  typedef CK::Point_3 CPoint;
  typedef CK::Plane_3 CPlane;

  std::list<CPoint> pointdata;
  // for (Point3Array::const_iterator it = points->begin(); it != points->end() ; ++it)
  for (Index::const_iterator it = group.begin(); it != group.end(); ++it)
    pointdata.push_back(toPoint3<CPoint>(points->getAt(*it)));

  CPlane plane;
  linear_least_squares_fitting_3(pointdata.begin(), pointdata.end(), plane, CGAL::Dimension_tag<0>());

  return dir2Vector3(plane.orthogonal_direction());
#else
#ifdef _MSC_VER
#pragma message("function 'pointset_normal' disabled. CGAL needed.")
#else
#warning "function 'pointset_normal' disabled. CGAL needed"
#endif

  return Vector3(0,0,0);
#endif
}

Point3ArrayPtr
PGL::pointsets_normals(const Point3ArrayPtr points, const IndexArrayPtr groups) {
  Point3ArrayPtr result(new Point3Array(points->size()));
  uint32_t i = 0;
  for (IndexArray::const_iterator it = groups->begin(); it != groups->end(); ++it, ++i) {
    result->setAt(i, pointset_normal(points, *it));
  }
  return result;
}


real_t mean_over(const Point3ArrayPtr points, const Index &section, int i) {
  real_t v = 0;
  for (Index::const_iterator it = section.begin(); it != section.end(); ++it)
    v += points->getAt(*it).getAt(i);
  return v / section.size();
}


real_t mean_over(const Point3ArrayPtr points, const Index &section, int i, int j) {
  real_t v = 0;
  for (Index::const_iterator it = section.begin(); it != section.end(); ++it)
    v += points->getAt(*it).getAt(i) * points->getAt(*it).getAt(j);
  return v / section.size();
}


#ifdef WITH_CGAL
#if CGAL_VERSION_NR > 1040800000

#ifdef CGAL_EIGEN3_ENABLED
#include <CGAL/Eigen_diagonalize_traits.h>
#else

#include <CGAL/Diagonalize_traits.h>

#endif

#else
#include <CGAL/eigen.h>
#endif

#endif

Vector3 PGL::section_normal(const Point3ArrayPtr pointnormals, const Index &section) {
#ifdef WITH_CGAL


  real_t mx = mean_over(pointnormals, section, 0);
  real_t mx2 = mean_over(pointnormals, section, 0, 0);
  real_t my = mean_over(pointnormals, section, 1);
  real_t my2 = mean_over(pointnormals, section, 1, 1);
  real_t mz = mean_over(pointnormals, section, 2);
  real_t mz2 = mean_over(pointnormals, section, 2, 2);
  real_t mxy = mean_over(pointnormals, section, 0, 1);
  real_t mxz = mean_over(pointnormals, section, 0, 2);
  real_t myz = mean_over(pointnormals, section, 1, 2);
  real_t mxyxy = 2 * mxy - 2 * mx * my;
  real_t mxzxz = 2 * mxz - 2 * mx * mz;
  real_t myzyz = 2 * myz - 2 * my * mz;


#if CGAL_VERSION_NR > 1040800000
#ifdef CGAL_EIGEN3_ENABLED
  typedef CGAL::Eigen_diagonalize_traits<real_t> Diagonalize;
#else
  typedef CGAL::Diagonalize_traits<real_t> Diagonalize;
#endif

  Diagonalize::Covariance_matrix covariance;
  covariance[0] = mx2 - mx * mx;
  covariance[1] = mxyxy;
  covariance[2] = my2 - my * my;
  covariance[3] = mxzxz;
  covariance[4] = myzyz;
  covariance[5] = mz2 - mz * mz;

  Diagonalize::Vector eigen_values;
  Diagonalize::Matrix eigen_vectors;
  Diagonalize::diagonalize_selfadjoint_covariance_matrix(covariance, eigen_values, eigen_vectors);
#else
  real_t covariance[6];
  covariance[0] = mx2-mx*mx;
  covariance[1] = mxyxy;
  covariance[2] = my2-my*my;
  covariance[3] = mxzxz;
  covariance[4] = myzyz;
  covariance[5] = mz2-mz*mz;

  real_t eigen_values[3];
  real_t eigen_vectors[9];
  CGAL::internal::eigen_symmetric<real_t>(covariance,3,eigen_vectors,eigen_values);
#endif

  if (eigen_values[2] < eigen_values[1] && eigen_values[2] < eigen_values[0])
    return Vector3(eigen_vectors[6], eigen_vectors[7], eigen_vectors[8]);
  else if (eigen_values[1] < eigen_values[0])
    return Vector3(eigen_vectors[3], eigen_vectors[4], eigen_vectors[5]);
  else
    return Vector3(eigen_vectors[0], eigen_vectors[1], eigen_vectors[2]);

#else
  return Vector3::ORIGIN;
#endif
}

Point3ArrayPtr PGL::sections_normals(const Point3ArrayPtr pointnormals, const IndexArrayPtr &sections) {
  size_t nbpoints = pointnormals->size();
  Point3ArrayPtr result(new Point3Array(nbpoints));
  Point3Array::iterator itres = result->begin();
  for (IndexArray::const_iterator itsection = sections->begin(); itsection != sections->end(); ++itsection) {
    *itres = section_normal(pointnormals, *itsection);
  }
  return result;
}
