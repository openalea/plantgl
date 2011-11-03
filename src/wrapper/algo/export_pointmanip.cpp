/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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

#include <plantgl/algo/base/pointmanipulation.h>
#include <boost/python.hpp>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python

/* ----------------------------------------------------------------------- */


object py_points_dijkstra_shortest_path(const Point3ArrayPtr points, 
			                            const IndexArrayPtr adjacencies, 
                                        uint32_t root)
{
    std::pair<TOOLS(Uint32Array1Ptr),TOOLS(RealArrayPtr)> result = points_dijkstra_shortest_path(points,adjacencies,root);
    return make_tuple(result.first,result.second);
}

object
py_skeleton_from_distance_to_root_clusters(const Point3ArrayPtr points, uint32_t root, real_t binsize, uint32_t k, bool verbose = false)
{
    TOOLS(Uint32Array1Ptr) group_parents; IndexArrayPtr group_components;
    Point3ArrayPtr group_centroids = skeleton_from_distance_to_root_clusters(points, root, binsize, k, group_parents, group_components, verbose );
    return make_tuple(group_centroids, group_parents, group_components);
}

void export_PointManip()
{
	def("compress_point2",&compress_point<Point2Array>,args("points","radius"));
	def("compress_point3",&compress_point<Point3Array>,args("points","radius"));
	def("compress_point4",&compress_point<Point4Array>,args("points","radius"));

#ifdef WITH_CGAL
    def("delaunay_point_connection",&delaunay_point_connection,args("points"));
    def("k_closest_points_from_delaunay",&k_closest_points_from_delaunay,args("points","k"));
#endif
#ifdef WITH_ANN
    def("k_closest_points_from_ann",&k_closest_points_from_ann,args("points","k"));
#endif


    def("r_neighboorhood",&r_neighboorhood,args("pid","points","adjacencies","radius"));
    def("r_neighboorhoods",(IndexArrayPtr(*)(const Point3ArrayPtr, const IndexArrayPtr, const RealArrayPtr))&r_neighboorhoods,args("points","adjacencies","radius"));
    def("r_neighboorhoods",(IndexArrayPtr(*)(const Point3ArrayPtr, const IndexArrayPtr, real_t))&r_neighboorhoods,args("points","adjacencies","radii"));
    def("r_anisotropic_neighboorhood",&r_anisotropic_neighboorhood,args("pid","points","adjacencies","radius","direction","alpha","beta"));
    def("r_anisotropic_neighboorhoods",
        (IndexArrayPtr (*)(const Point3ArrayPtr, const IndexArrayPtr, const RealArrayPtr, const Point3ArrayPtr, const real_t, const real_t ))&r_anisotropic_neighboorhoods,
        args("points","adjacencies","radii","directions","alpha","beta"));
    def("r_anisotropic_neighboorhoods",
        (IndexArrayPtr (*)(const Point3ArrayPtr, const IndexArrayPtr, const real_t, const Point3ArrayPtr, const real_t, const real_t ))&r_anisotropic_neighboorhoods,
        args("points","adjacencies","radius","directions","alpha","beta"));

    def("density_from_r_neighboorhood",&density_from_r_neighboorhood,args("pid","points","adjacencies","radius"));
    def("densities_from_r_neighboorhood",&densities_from_r_neighboorhood,args("points","adjacencies","radius"));
    def("max_neighboorhood_distance",&max_neighboorhood_distance,args("pid","points","adjacency"));
    def("density_from_k_neighboorhood",&density_from_k_neighboorhood,args("pid","points","adjacencies"));
    def("densities_from_k_neighboorhood",&densities_from_k_neighboorhood,args("points","adjacencies"));

#ifdef WITH_CGAL
    def("pointset_orientation",&pointset_orientation,args("points","group"));
    def("pointsets_orientations",&pointsets_orientations,args("points","groups"));
#endif

    def("centroid_of_group",&centroid_of_group,args("points","group"));
    def("centroids_of_groups",&centroids_of_groups,args("points","groups"));

    def("get_sorted_element_order",&get_sorted_element_order,args("elements"));
    def("points_dijkstra_shortest_path",&py_points_dijkstra_shortest_path,args("points","adjacencies","root"));
    def("quotient_points_from_adjacency_graph",&quotient_points_from_adjacency_graph,args("binsize","points","adjacencies","distances_to_root"));
    def("quotient_adjacency_graph",&quotient_points_from_adjacency_graph,args("adjacencies","groups"));
    def("skeleton_from_distance_to_root_clusters",&py_skeleton_from_distance_to_root_clusters,(bp::arg("points"),bp::arg("root"),bp::arg("binsize"),bp::arg("k")=10,bp::arg("verbose")=false),"Implementation of Xu et al. 07 method for main branching system");

}

/* ----------------------------------------------------------------------- */
