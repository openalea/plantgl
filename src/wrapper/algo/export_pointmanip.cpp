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
py_skeleton_from_distance_to_root_clusters(const Point3ArrayPtr points, uint32_t root, real_t binsize, uint32_t k, bool connect_all_points = false, bool verbose = false)
{
    TOOLS(Uint32Array1Ptr) group_parents; IndexArrayPtr group_components;
    Point3ArrayPtr group_centroids = skeleton_from_distance_to_root_clusters(points, root, binsize, k, group_parents, group_components, connect_all_points, verbose );
    return make_tuple(group_centroids, group_parents, group_components);
}

object
py_remove_nodes(const Index& toremove,
                Point3ArrayPtr nodes,
                Uint32Array1Ptr parents, 
                RealArrayPtr radii)
{
    remove_nodes(toremove,nodes,parents,radii);
    return make_tuple(nodes,parents,radii);
}


object
py_determine_children(const TOOLS(Uint32Array1Ptr) parents)
{
    uint32_t root;
    IndexArrayPtr children = determine_children(parents,root);
    return make_tuple(children,root);
}

#ifdef WITH_CGAL
#ifdef WITH_LAPACK
bp::object
translate_eigen_vectors(const std::vector<std::pair<real_t, TOOLS(Vector3)> > egv)
{
    boost::python::list res;
    for (std::vector<std::pair<real_t, TOOLS(Vector3)> >::const_iterator it = egv.begin(); it != egv.end();  ++it)
        res.append(make_tuple(it->first,it->second));
    return res;
}

bp::object
translate_eigen_vectors_set(const std::vector<std::vector<std::pair<real_t, TOOLS(Vector3)> > > egvs)
{
    boost::python::list res;
    for (std::vector<std::vector<std::pair<real_t, TOOLS(Vector3)> > >::const_iterator it = egvs.begin(); it != egvs.end();  ++it)
        res.append(translate_eigen_vectors(*it));
    return res;
}

bp::object
py_principal_curvatures_0(const Point3ArrayPtr points, uint32_t pid, const Index& group, size_t fitting_degree = 4, size_t monge_degree = 4)
{
    return translate_eigen_vectors(principal_curvatures(points,pid,group,fitting_degree,monge_degree));
}

bp::object
py_principal_curvatures_1(const Point3ArrayPtr points, const IndexArrayPtr groups, size_t fitting_degree = 4, size_t monge_degree = 4)
{
    return translate_eigen_vectors_set(principal_curvatures(points,groups,fitting_degree,monge_degree));
}

bp::object
py_principal_curvatures_2(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, real_t radius, size_t fitting_degree = 4, size_t monge_degree = 4)
{
    return translate_eigen_vectors_set(principal_curvatures(points,adjacencies,radius,fitting_degree,monge_degree));
}
#endif
#endif

void export_PointManip()
{
	def("contract_point2",&contract_point<Point2Array>,args("points","radius"));
	def("contract_point3",&contract_point<Point3Array>,args("points","radius"));
	def("contract_point4",&contract_point<Point4Array>,args("points","radius"));


#ifdef WITH_CGAL
    def("delaunay_point_connection",&delaunay_point_connection,args("points"));
    def("k_closest_points_from_delaunay",&k_closest_points_from_delaunay,args("points","k"));
#endif
#ifdef WITH_ANN
    def("k_closest_points_from_ann",&k_closest_points_from_ann,(bp::arg("points"),bp::arg("k"),bp::arg("symmetric")=false));
#endif

    def("symmetrize_connections",&symmetrize_connections,(bp::arg("adjacencies")));
    def("connect_all_connex_components",&connect_all_connex_components,(bp::arg("points"),bp::arg("adjacencies"),bp::arg("verbose")=false));


    def("r_neighborhood",&r_neighborhood,args("pid","points","adjacencies","radius"));
    def("r_neighborhoods",(IndexArrayPtr(*)(const Point3ArrayPtr, const IndexArrayPtr, const RealArrayPtr))&r_neighborhoods,args("points","adjacencies","radii"));
    def("r_neighborhoods",(IndexArrayPtr(*)(const Point3ArrayPtr, const IndexArrayPtr, real_t, bool))&r_neighborhoods,(bp::arg("points"),bp::arg("adjacencies"),bp::arg("radius"),bp::arg("verbose")=false));
    def("r_anisotropic_neighborhood",&r_anisotropic_neighborhood,args("pid","points","adjacencies","radius","direction","alpha","beta"));
    def("r_anisotropic_neighborhoods",
        (IndexArrayPtr (*)(const Point3ArrayPtr, const IndexArrayPtr, const RealArrayPtr, const Point3ArrayPtr, const real_t, const real_t ))&r_anisotropic_neighborhoods,
        args("points","adjacencies","radii","directions","alpha","beta"));
    def("r_anisotropic_neighborhoods",
        (IndexArrayPtr (*)(const Point3ArrayPtr, const IndexArrayPtr, const real_t, const Point3ArrayPtr, const real_t, const real_t ))&r_anisotropic_neighborhoods,
        args("points","adjacencies","radius","directions","alpha","beta"));

    def("k_neighborhood",&k_neighborhood,args("pid","points","adjacencies","k"));
    def("k_neighborhoods",&k_neighborhoods,args("points","adjacencies","k"));

    def("density_from_r_neighborhood",&density_from_r_neighborhood,args("pid","points","adjacencies","radius"));
    def("densities_from_r_neighborhood",&densities_from_r_neighborhood,args("points","adjacencies","radius"));
    def("max_neighborhood_distance",&max_neighborhood_distance,args("pid","points","adjacency"));

    def("density_from_k_neighborhood",&density_from_k_neighborhood,(bp::arg("pid"),bp::arg("points"),bp::arg("adjacencies"),bp::arg("k")=0),"Compute density of a point according to its k neighboordhood. If k is 0, its value is deduced from adjacencies.");
    def("densities_from_k_neighborhood",&densities_from_k_neighborhood,(bp::arg("points"),bp::arg("adjacencies"),bp::arg("k")=0),"Compute local densities of a set of points according to their k neighboordhood. If k is 0, its value is deduced from adjacencies.");

#ifdef WITH_CGAL
    def("pointset_orientation",&pointset_orientation,args("points","group"));
    def("pointsets_orientations",&pointsets_orientations,args("points","groups"));

#ifdef WITH_LAPACK
    def("principal_curvatures",&py_principal_curvatures_0,(bp::arg("points"),bp::arg("pid"),bp::arg("group"),bp::("fitting_degree")=4,bp::("monge_degree")=4));
    def("principal_curvatures",&py_principal_curvatures_1,(bp::arg(("points"),bp::arg("groups"),bp::("fitting_degree")=4,bp::("monge_degree")=4));
    def("principal_curvatures",&py_principal_curvatures_2,(bp::arg(("points"),bp::arg("adjacencies"),bp::arg("radius"),bp::("fitting_degree")=4,bp::("monge_degree")=4));
#endif
#endif

    def("centroid_of_group",&centroid_of_group,args("points","group"));
    def("centroids_of_groups",&centroids_of_groups,args("points","groups"));


    def("adaptive_radii",&adaptive_radii,(bp::arg("density"),bp::arg("minradius"),bp::arg("maxradius"),bp::arg("densityradiusmap")=NULL),"Compute a radius for each density value");
    def("adaptive_contration",&adaptive_contration,(bp::arg("points"),bp::arg("orientations"),bp::arg("adjacencies"),
                                                    bp::arg("density"),bp::arg("minradius"),bp::arg("maxradius"),
                                                    bp::arg("densityradiusmap")=NULL,
                                                    bp::arg("alpha")=1.0,bp::arg("beta")=1.0),"Contract the pointset with an adptive radius of contraction");

    def("get_sorted_element_order",&get_sorted_element_order,args("elements"));
    def("points_dijkstra_shortest_path",&py_points_dijkstra_shortest_path,args("points","adjacencies","root"));
    def("quotient_points_from_adjacency_graph",&quotient_points_from_adjacency_graph,args("binsize","points","adjacencies","distances_to_root"));
    def("quotient_adjacency_graph",&quotient_points_from_adjacency_graph,args("adjacencies","groups"));
    def("skeleton_from_distance_to_root_clusters",&py_skeleton_from_distance_to_root_clusters,
        (bp::arg("points"),bp::arg("root"),bp::arg("binsize"),bp::arg("k")=10,bp::arg("connect_all_points")=false,bp::arg("verbose")=false),"Implementation of Xu et al. 07 method for main branching system");

    def("determine_children", &py_determine_children);
    def("carried_length",&carried_length,bp::args("points","parents"));
    def("optimize_orientations",&optimize_orientations,bp::args("points","parents","weights"));
    def("optimize_positions",&optimize_positions,bp::args("points","orientations","parents","weights"));

    def("average_radius",&average_radius,(bp::arg("points"),bp::arg("nodes"),bp::arg("parents"),bp::arg("maxclosestnodes")=10));
    def("estimate_radii",&estimate_radii,(bp::arg("nodes"),bp::arg("parents"),bp::arg("weights"),bp::arg("averageradius"),bp::arg("pipeexponent")=2.5));

    def("filter_short_nodes",&filter_short_nodes,(bp::arg("nodes"),bp::arg("parents"),bp::arg("radii"),bp::arg("edgelengthfilter")=0.1,bp::arg("overlapfilter")=0.5));
    def("remove_nodes",&py_remove_nodes,(bp::arg("toremove"),bp::arg("nodes"),bp::arg("parents"),bp::arg("radii")));

}

/* ----------------------------------------------------------------------- */
