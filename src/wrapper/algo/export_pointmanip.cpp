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
#include <plantgl/python/export_list.h>
#include <plantgl/python/extract_list.h>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python


#ifdef WITH_LAPACK 
#   define CGAL_AND_SVD_SOLVER_ENABLED
#else
#ifdef WITH_EIGEN
#   define CGAL_AND_SVD_SOLVER_ENABLED
#endif
#endif
/* ----------------------------------------------------------------------- */


object py_points_dijkstra_shortest_path(const Point3ArrayPtr points, 
                                        const IndexArrayPtr adjacencies, 
                                        uint32_t root)
{
    return make_pair_tuple(points_dijkstra_shortest_path(points,adjacencies,root));
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
    if (is_valid_ptr(radii)) return make_tuple(nodes,parents,radii);
    else return make_tuple(nodes,parents);
}

object
py_merge_nodes(IndexArrayPtr tomerge,
                Point3ArrayPtr nodes,
                Uint32Array1Ptr parents, 
                RealArrayPtr radii,
                RealArrayPtr weights)
{
    merge_nodes(tomerge,nodes,parents,radii,weights);
    return make_tuple(nodes,parents,radii);
}


object
py_determine_children(const TOOLS(Uint32Array1Ptr) parents)
{
    uint32_t root;
    IndexArrayPtr children = determine_children(parents,root);
    return make_tuple(children,root);
}

#ifdef CGAL_AND_SVD_SOLVER_ENABLED

bp::object
translate_pc_info(const CurvatureInfo& egv)
{
    return make_tuple(egv.origin,
                      make_tuple(egv.maximal_principal_direction,egv.maximal_curvature),
                      make_tuple(egv.minimal_principal_direction,egv.minimal_curvature),
                      egv.normal);
}

bp::object
translate_pc_info_set(const std::vector<CurvatureInfo> egvs)
{
    boost::python::list res;
    for (std::vector<CurvatureInfo>::const_iterator it = egvs.begin(); it != egvs.end();  ++it)
        res.append(translate_pc_info(*it));
    return res;
}

bp::object
py_principal_curvatures_0(const Point3ArrayPtr points, uint32_t pid, const Index& group, size_t fitting_degree = 4, size_t monge_degree = 4)
{
    return translate_pc_info(principal_curvatures(points,pid,group,fitting_degree,monge_degree));
}

bp::object
py_principal_curvatures_1(const Point3ArrayPtr points, const IndexArrayPtr groups, size_t fitting_degree = 4, size_t monge_degree = 4)
{
    return translate_pc_info_set(principal_curvatures(points,groups,fitting_degree,monge_degree));
}

bp::object
py_principal_curvatures_2(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, real_t radius, size_t fitting_degree = 4, size_t monge_degree = 4)
{
    return translate_pc_info_set(principal_curvatures(points,adjacencies,radius,fitting_degree,monge_degree));
}

#endif

object
py_node_intersection_test(const TOOLS(Vector3)& root, real_t rootradius, 
                                     const TOOLS(Vector3)& p1,   real_t radius1, 
                                     const TOOLS(Vector3)& p2,   real_t radius2,
                                     real_t overlapfilter,
                                     bool verbose = false)
{
    ScenePtr * visu = NULL;
    if (verbose) visu = new ScenePtr();
    bool res = node_intersection_test(root,rootradius,p1, radius1, p2, radius2, overlapfilter, verbose, visu );
    return make_tuple(res,visu);
}

object
py_node_continuity_test(const TOOLS(Vector3)& node, real_t radius, 
                        const TOOLS(Vector3)& parent,   real_t parentradius, 
                        const TOOLS(Vector3)& child,   real_t childradius,
                        real_t overlapfilter, bool verbose = false)
{
    ScenePtr * visu = NULL;
    if (verbose) visu = new ScenePtr();
    bool res = node_continuity_test(node,radius,parent, parentradius, child, childradius, overlapfilter, verbose, visu );
    return make_tuple(res,visu);
}

object py_estimate_pointset_circle(const Point3ArrayPtr points, const Index&  group, bp::object direction, bool bounding = false)
{
    std::pair<TOOLS(Vector3),real_t> res;
    if(direction == bp::object()) res = pointset_circle(points, group, bounding);
    else res = pointset_circle(points, group, extract<Vector3>(direction)(), bounding);
    return make_pair_tuple(res);
}

object py_estimate_pointsets_circles(const Point3ArrayPtr points, const IndexArrayPtr  groups, const Point3ArrayPtr directions = Point3ArrayPtr(0), bool bounding = false)
{
    return make_pair_tuple(pointsets_circles(points, groups, directions, bounding));
}

object py_estimate_pointsets_section_circles(const Point3ArrayPtr points, const IndexArrayPtr  adjacencies, const Point3ArrayPtr directions, real_t width, bool bounding = false)
{
    return make_pair_tuple(pointsets_section_circles(points, adjacencies, directions, width, bounding));
}

object py_adaptive_section_circles(const Point3ArrayPtr points, const IndexArrayPtr  adjacencies, const Point3ArrayPtr directions, const RealArrayPtr widths, const RealArrayPtr maxradii)
{
    return make_pair_tuple(adaptive_section_circles(points, adjacencies, directions, widths, maxradii));
}

object py_adaptive_section_circles2(const Point3ArrayPtr points, const IndexArrayPtr  adjacencies, const Point3ArrayPtr directions, const real_t widths, const RealArrayPtr maxradii)
{
    return make_pair_tuple(adaptive_section_circles(points, adjacencies, directions, widths, maxradii));
}

object
py_adaptive_section_contration(const Point3ArrayPtr points, 
                     const Point3ArrayPtr orientations,
                     const IndexArrayPtr adjacencies, 
                     const RealArrayPtr density,
                     real_t minradius, real_t maxradius,
                     QuantisedFunctionPtr densityradiusmap,
                     const real_t alpha, 
	                 const real_t beta)
{
    return make_pair_tuple(adaptive_section_contration(points, orientations, adjacencies, density, minradius, maxradius, densityradiusmap, alpha, beta ));
}

object py_findClosestFromSubset(const Vector3& o, Point3ArrayPtr pts, const Index& index)
{
    return make_pair_tuple(findClosestFromSubset(o, pts, index));
}


object py_next_quotient_points_from_adjacency_graph(const real_t initiallevel,
                                            const real_t binsize,
                                            const Index& currents,
			                                const IndexArrayPtr adjacencies, 
			                                const TOOLS(RealArrayPtr) distances_to_root)
{
    return make_pair_tuple(next_quotient_points_from_adjacency_graph(initiallevel,binsize,currents,adjacencies,distances_to_root));
}

object  py_cluster_junction_points(const IndexArrayPtr pointtoppology, const Index& group1, const Index& group2)
{
    return make_pair_tuple(cluster_junction_points(pointtoppology, group1,group2));
}


// to control display of progress
static boost::python::object pyprogressfunction;

void py_c_progressfunc(const char * msg, float percent)
{
    pyprogressfunction(msg,percent);
}

void py_register_progressstatus_func(boost::python::object func)
{
    pyprogressfunction = func;
    register_progressstatus_func(py_c_progressfunc);
}

void py_unregister_progressstatus_func(){
    pyprogressfunction = boost::python::object();
    unregister_progressstatus_func();
}

boost::python::object py_find_min_max(const Point3ArrayPtr point, const int &boundMaxPourcent)
{
    return make_pair_tuple(find_min_max(point, boundMaxPourcent));
}

boost::python::object py_find_min_max2(const Point3ArrayPtr point, const Vector3 &center, const Vector3 &direction)
{
    return make_pair_tuple(find_min_max(point, center, direction));
}

void export_PointManip()
{
    def("pgl_register_progressstatus_func",&py_register_progressstatus_func,args("func"));
    def("pgl_unregister_progressstatus_func",&py_unregister_progressstatus_func);


    def("contract_point2",&contract_point<Point2Array>,args("points","radius"));
    def("contract_point3",&contract_point<Point3Array>,args("points","radius"));
    def("contract_point4",&contract_point<Point4Array>,args("points","radius"));

    def("select_not_ground", &select_not_ground, args("point", "kclosest"));
    def("find_min_max", &py_find_min_max, args("point", "boundMaxPourcent"));
    def("find_min_max", &py_find_min_max2, args("point", "boundMaxPourcent", "direction"));
    def("get_shortest_path", &get_shortest_path, args("points", "kclosest", "point_begin", "point_end"));

#ifdef WITH_CGAL
    def("delaunay_point_connection",&delaunay_point_connection,args("points"));
    def("delaunay_triangulation",&delaunay_triangulation,args("points"));
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
    def("r_neighborhoods_mt",(IndexArrayPtr(*)(const Point3ArrayPtr, const IndexArrayPtr, real_t, bool))&r_neighborhoods_mt,(bp::arg("points"),bp::arg("adjacencies"),bp::arg("radius"),bp::arg("verbose")=false));
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
    def("densities_from_r_neighborhood",(RealArrayPtr(*)(const Point3ArrayPtr, const IndexArrayPtr, const real_t))&densities_from_r_neighborhood,args("points","adjacencies","radius"));
    def("densities_from_r_neighborhood",(RealArrayPtr(*)(const IndexArrayPtr, const real_t))&densities_from_r_neighborhood,args("neighborhood","radius"));

    def("pointset_max_distance",(real_t (*)(uint32_t, const Point3ArrayPtr, const Index&))&pointset_max_distance,args("pid","points","group"));
    def("pointset_max_distance",(real_t (*)(const Vector3&, const Point3ArrayPtr, const Index&))&pointset_max_distance,args("center","points","group"));
    def("pointset_mean_distance",&pointset_mean_distance<Index>,args("center","points","group"));
    def("pointset_mean_distances",&pointset_mean_distances<IndexArray>,args("points","groups"));
    def("pointset_mean_radial_distance",&pointset_mean_radial_distance,args("center","direction","points","group"));
    def("pointset_max_radial_distance",&pointset_max_radial_distance,args("center","direction","points","group"));

    def("pointset_covariance",&pointset_covariance,(arg("points"),arg("group")=Index()));

    def("density_from_k_neighborhood",&density_from_k_neighborhood,(bp::arg("pid"),bp::arg("points"),bp::arg("adjacencies"),bp::arg("k")=0),"Compute density of a point according to its k neighboordhood. If k is 0, its value is deduced from adjacencies.");
    def("densities_from_k_neighborhood",&densities_from_k_neighborhood,(bp::arg("points"),bp::arg("adjacencies"),bp::arg("k")=0),"Compute local densities of a set of points according to their k neighboordhood. If k is 0, its value is deduced from adjacencies.");

#ifdef WITH_CGAL
    def("pointset_orientation",&pointset_orientation,args("points","group"));
    def("pointsets_orientations",&pointsets_orientations,args("points","groups"));
    def("pointset_normal",&pointset_normal,(bp::arg("points"),bp::arg("groups")));
    def("pointsets_normals",&pointsets_normals,(bp::arg("points"),bp::arg("groups")));
    def("triangleset_orientation",&triangleset_orientation,args("points","triangles"));

#ifdef CGAL_AND_SVD_SOLVER_ENABLED
    def("principal_curvatures",&py_principal_curvatures_0,(bp::arg("points"),bp::arg("pid"),bp::arg("group"),bp::arg("fitting_degree")=4,bp::arg("monge_degree")=4),
        "Compute principal curvature information. Return a tuple with folowin informations: (origin,(maximal_curvature_direction,maximal_curvature),(minimal_curvature_direction,minimal_curvature),normal)");
    def("principal_curvatures",&py_principal_curvatures_1,(bp::arg("points"),bp::arg("groups"),bp::arg("fitting_degree")=4,bp::arg("monge_degree")=4));
    def("principal_curvatures",&py_principal_curvatures_2,(bp::arg("points"),bp::arg("adjacencies"),bp::arg("radius"),bp::arg("fitting_degree")=4,bp::arg("monge_degree")=4));
#endif
#endif
    def("pointsets_orient_normals",(Point3ArrayPtr (*)(const Point3ArrayPtr, const Point3ArrayPtr, const IndexArrayPtr ))&pointsets_orient_normals,(bp::arg("normals"),bp::arg("points"),bp::arg("adjacencies")));
    def("pointsets_orient_normals",(Point3ArrayPtr (*)(const Point3ArrayPtr, uint32_t, const IndexArrayPtr ))&pointsets_orient_normals,(bp::arg("normals"),bp::arg("source"),bp::arg("adjacencies")));

    def("point_section",(Index (*)(uint32_t, const Point3ArrayPtr, const IndexArrayPtr, const TOOLS(Vector3)&, real_t))         &point_section,args("pid","points","adjacencies","direction","width"));
    def("point_section",(Index (*)(uint32_t, const Point3ArrayPtr, const IndexArrayPtr, const TOOLS(Vector3)&, real_t, real_t)) &point_section,args("pid","points","adjacencies","direction","width","maxradius"));
    def("points_sections",&points_sections,args("points","adjacencies","directions","width"));
    def("section_normal",&section_normal,args("pointnormals","section"));
    def("sections_normals",&sections_normals,args("pointnormals","sections"));
    
    def("pointset_circle",&py_estimate_pointset_circle,(bp::arg("points"),bp::arg("group"),bp::arg("direction")=bp::object(),bp::arg("bounding")=false));
    def("pointsets_circles",&py_estimate_pointsets_circles,(arg("points"),bp::arg("groups"),bp::arg("directions")=Point3ArrayPtr(0),bp::arg("bounding")=false));

    def("pointsets_section_circles",&py_estimate_pointsets_section_circles,(arg("points"),bp::arg("adjacencies"),bp::arg("directions"),bp::arg("width"),bp::arg("bounding")=false));
    def("adaptive_section_circles",&py_adaptive_section_circles,(arg("points"),bp::arg("adjacencies"),bp::arg("directions"),bp::arg("widths"),bp::arg("maxradii")));
    def("adaptive_section_circles",&py_adaptive_section_circles2,(arg("points"),bp::arg("adjacencies"),bp::arg("directions"),bp::arg("widths"),bp::arg("maxradii")));

    def("centroid_of_group",&centroid_of_group<Index>,args("","group"));
    def("centroids_of_groups",&centroids_of_groups<IndexArray>,args("points","groups"));
    def("points_clusters",&points_clusters,args("points","clustercentroid"));
    def("cluster_points",&cluster_points,args("points","clustercentroid"));


    def("adaptive_radii",&adaptive_radii,(bp::arg("density"),bp::arg("minradius"),bp::arg("maxradius"),bp::arg("densityradiusmap")=QuantisedFunctionPtr(0)),"Compute a radius for each density value");
    def("adaptive_contration",&adaptive_contration,(bp::arg("points"),bp::arg("orientations"),bp::arg("adjacencies"),
                                                    bp::arg("densities"),bp::arg("minradius"),bp::arg("maxradius"),
                                                    bp::arg("densityradiusmap")=NULL,
                                                    bp::arg("alpha")=1.0,bp::arg("beta")=1.0),"Contract the pointset with an adptive radius of contraction");

    def("adaptive_section_contration",&py_adaptive_section_contration,(bp::arg("points"),bp::arg("orientations"),bp::arg("adjacencies"),
                                                    bp::arg("densities"),bp::arg("minradius"),bp::arg("maxradius"),
                                                    bp::arg("densityradiusmap")=NULL,
                                                    bp::arg("alpha")=1.0,bp::arg("beta")=1.0),"Contract the pointset with an adptive radius of contraction");

    def("get_sorted_element_order",&get_sorted_element_order,args("elements"));
    def("points_dijkstra_shortest_path",&py_points_dijkstra_shortest_path,args("points","adjacencies","root"));
    def("quotient_points_from_adjacency_graph",&quotient_points_from_adjacency_graph,args("binsize","points","adjacencies","distances_to_root"));
    def("quotient_adjacency_graph",&quotient_adjacency_graph,args("adjacencies","groups"));
    def("skeleton_from_distance_to_root_clusters",&py_skeleton_from_distance_to_root_clusters,
        (bp::arg("points"),bp::arg("root"),bp::arg("binsize"),bp::arg("k")=10,bp::arg("connect_all_points")=false,bp::arg("verbose")=false),"Implementation of Xu et al. 07 method for main branching system");

    def("points_in_range_from_root",&points_in_range_from_root,args("initiallevel","binsize","distances_to_root"));
    def("next_quotient_points_from_adjacency_graph",&py_next_quotient_points_from_adjacency_graph,args("initiallevel","binsize","current","adjacencies","distances_to_root"));
    
    def("determine_children", &py_determine_children);
    def("carried_length",&carried_length,bp::args("points","parents"));
    def("subtrees_size",(Uint32Array1Ptr(*)(const Uint32Array1Ptr))&subtrees_size,bp::args("parents"));
    def("subtrees_size",(Uint32Array1Ptr(*)(const IndexArrayPtr, uint32_t))&subtrees_size,bp::args("children","root"));

    def("optimize_orientations",&optimize_orientations,bp::args("points","parents","weights"));
    def("optimize_positions",&optimize_positions,bp::args("points","orientations","parents","weights"));

    def("average_radius",&average_radius,(bp::arg("points"),bp::arg("nodes"),bp::arg("parents"),bp::arg("maxclosestnodes")=10));
    def("distance_to_shape",&distance_to_shape,(bp::arg("points"),bp::arg("nodes"),bp::arg("parents"),bp::arg("radii"),bp::arg("maxclosestnodes")=10));
    def("average_distance_to_shape",&average_distance_to_shape,(bp::arg("points"),bp::arg("nodes"),bp::arg("parents"),bp::arg("radii"),bp::arg("maxclosestnodes")=10));
    def("points_at_distance_from_skeleton",&points_at_distance_from_skeleton,(bp::arg("points"),bp::arg("nodes"),bp::arg("parents"),bp::arg("distance"),bp::arg("maxclosestnodes")=10),"Return indices of all point which are below a given distance ot the skeleton. If distance is negative, return point above a distance");

    def("estimate_radii_from_points",&estimate_radii_from_points,(bp::arg("points"),bp::arg("nodes"),bp::arg("parents"),bp::arg("maxmethod")=false,bp::arg("maxclosestnodes")=10));
    def("estimate_radii_from_pipemodel",&estimate_radii_from_pipemodel,(bp::arg("nodes"),bp::arg("parents"),bp::arg("weights"),bp::arg("averageradius"),bp::arg("pipeexponent")=2.5));
    
    def("min_max_mean_edge_length",(Vector3 (*)(const Point3ArrayPtr, Uint32Array1Ptr))&min_max_mean_edge_length,(bp::arg("points"),bp::arg("parents")));
    def("min_max_mean_edge_length",(Vector3 (*)(const Point3ArrayPtr, IndexArrayPtr))&min_max_mean_edge_length,(bp::arg("points"),bp::arg("graph")));

    def("node_continuity_test",&py_node_continuity_test,(bp::arg("node"),bp::arg("radius"),bp::arg("parent"),bp::arg("parentradius"),bp::arg("child"),bp::arg("radius"),bp::arg("overlapfilter")=0.5,bp::arg("verbose")=false));
    def("node_intersection_test",&py_node_intersection_test,(bp::arg("parent"),bp::arg("parentradius"),bp::arg("node1"),bp::arg("radius1"),bp::arg("node2"),bp::arg("radius2"),bp::arg("overlapfilter")=0.5,bp::arg("verbose")=false));
    def("detect_short_nodes",&detect_short_nodes,(bp::arg("nodes"),bp::arg("parents"),bp::arg("edgelengthfilter")=0.001));
    def("remove_nodes",&py_remove_nodes,(bp::arg("toremove"),bp::arg("nodes"),bp::arg("parents"),bp::arg("radii")=RealArrayPtr(0)));
    def("detect_similar_nodes",&detect_similar_nodes,(bp::arg("nodes"),bp::arg("parents"),bp::arg("radii"),bp::arg("weight"),bp::arg("overlapfilter")=0.5));
    def("merge_nodes",&py_merge_nodes,(bp::arg("tomerge"),bp::arg("nodes"),bp::arg("parents"),bp::arg("radii"),bp::arg("weight")));

    def("pointset_mean_direction",&pointset_mean_direction,(bp::arg("origin"),bp::arg("points"),bp::arg("group")=Index()));
    def("pointset_directions",&pointset_directions,(bp::arg("origin"),bp::arg("points"),bp::arg("group")=Index()));
    def("pointset_angulardirections",&pointset_angulardirections,(bp::arg("points"),bp::arg("origin")=Vector3::ORIGIN,bp::arg("group")=Index()));

    def("findClosestFromSubset",&py_findClosestFromSubset,(bp::arg("origin"),bp::arg("points"),bp::arg("group")=Index()));

    def("orientations_distances",&orientations_distances,(bp::arg("orientations"),bp::arg("group")=Index()));
    def("orientations_similarities",&orientations_similarities,(bp::arg("orientations"),bp::arg("group")=Index()));

    def("cluster_junction_points",&py_cluster_junction_points,(bp::arg("pointtoppology"),bp::arg("group1"),bp::arg("group2")));

    def("pointset_median",&pointset_median,(bp::arg("points")));
    def("approx_pointset_median",&approx_pointset_median,(bp::arg("points"),bp::arg("nbIterMax")=200));
}


/* ----------------------------------------------------------------------- */
