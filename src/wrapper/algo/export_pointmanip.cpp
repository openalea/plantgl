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



#include <plantgl/algo/base/pointmanipulation.h>
#include <boost/python.hpp>
#include <plantgl/python/export_list.h>
#include <plantgl/python/extract_list.h>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
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
                                        uint32_t root) {
  return make_pair_tuple(points_dijkstra_shortest_path(points, adjacencies, root));
}

object
py_skeleton_from_distance_to_root_clusters(const Point3ArrayPtr points, uint32_t root, real_t binsize, uint32_t k, bool connect_all_points = false, bool verbose = false) {
  Uint32Array1Ptr group_parents;
  IndexArrayPtr group_components;
  Point3ArrayPtr group_centroids = skeleton_from_distance_to_root_clusters(points, root, binsize, k, group_parents, group_components, connect_all_points, verbose);
  return make_tuple(group_centroids, group_parents, group_components);
}

object
py_remove_nodes(const Index &toremove,
                Point3ArrayPtr nodes,
                Uint32Array1Ptr parents,
                RealArrayPtr radii) {
  remove_nodes(toremove, nodes, parents, radii);
  if (is_valid_ptr(radii)) return make_tuple(nodes, parents, radii);
  else return make_tuple(nodes, parents);
}

object
py_merge_nodes(IndexArrayPtr tomerge,
               Point3ArrayPtr nodes,
               Uint32Array1Ptr parents,
               RealArrayPtr radii,
               RealArrayPtr weights) {
  merge_nodes(tomerge, nodes, parents, radii, weights);
  return make_tuple(nodes, parents, radii);
}


object
py_determine_children(const Uint32Array1Ptr parents) {
  uint32_t root;
  IndexArrayPtr children = determine_children(parents, root);
  return make_tuple(children, root);
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
py_node_intersection_test(const Vector3 &root, real_t rootradius,
                          const Vector3 &p1, real_t radius1,
                          const Vector3 &p2, real_t radius2,
                          real_t overlapfilter,
                          bool verbose = false) {
  ScenePtr *visu = NULL;
  if (verbose) visu = new ScenePtr();
  bool res = node_intersection_test(root, rootradius, p1, radius1, p2, radius2, overlapfilter, verbose, visu);
  return make_tuple(res, visu);
}

object
py_node_continuity_test(const Vector3 &node, real_t radius,
                        const Vector3 &parent, real_t parentradius,
                        const Vector3 &child, real_t childradius,
                        real_t overlapfilter, bool verbose = false) {
  ScenePtr *visu = NULL;
  if (verbose) visu = new ScenePtr();
  bool res = node_continuity_test(node, radius, parent, parentradius, child, childradius, overlapfilter, verbose, visu);
  return make_tuple(res, visu);
}

object py_estimate_pointset_circle(const Point3ArrayPtr points, const Index &group, bp::object direction, bool bounding = false) {
  std::pair<Vector3, real_t> res;
  if (direction == bp::object()) res = pointset_circle(points, group, bounding);
  else res = pointset_circle(points, group, extract<Vector3>(direction)(), bounding);
  return make_pair_tuple(res);
}

object py_estimate_pointsets_circles(const Point3ArrayPtr points, const IndexArrayPtr groups, const Point3ArrayPtr directions = Point3ArrayPtr(0), bool bounding = false) {
  return make_pair_tuple(pointsets_circles(points, groups, directions, bounding));
}

object py_estimate_pointsets_section_circles(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const Point3ArrayPtr directions, real_t width, bool bounding = false) {
  return make_pair_tuple(pointsets_section_circles(points, adjacencies, directions, width, bounding));
}

object py_adaptive_section_circles(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const Point3ArrayPtr directions, const RealArrayPtr widths, const RealArrayPtr maxradii) {
  return make_pair_tuple(adaptive_section_circles(points, adjacencies, directions, widths, maxradii));
}

object py_adaptive_section_circles2(const Point3ArrayPtr points, const IndexArrayPtr adjacencies, const Point3ArrayPtr directions, const real_t widths, const RealArrayPtr maxradii) {
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
                               const real_t beta) {
  return make_pair_tuple(adaptive_section_contration(points, orientations, adjacencies, density, minradius, maxradius, densityradiusmap, alpha, beta));
}

object py_findClosestFromSubset(const Vector3 &o, Point3ArrayPtr pts, const Index &index) {
  return make_pair_tuple(findClosestFromSubset(o, pts, index));
}


object py_next_quotient_points_from_adjacency_graph(const real_t initiallevel,
                                                    const real_t binsize,
                                                    const Index &currents,
                                                    const IndexArrayPtr adjacencies,
                                                    const RealArrayPtr distances_to_root) {
  return make_pair_tuple(next_quotient_points_from_adjacency_graph(initiallevel, binsize, currents, adjacencies, distances_to_root));
}

object py_cluster_junction_points(const IndexArrayPtr pointtoppology, const Index &group1, const Index &group2) {
  return make_pair_tuple(cluster_junction_points(pointtoppology, group1, group2));
}

boost::python::object py_add_baricenter_points_of_path(const Point3ArrayPtr &point, IndexArrayPtr &kclosest, const Index &path, const real_t &radius) {
  return make_pair_tuple(add_baricenter_points_of_path(point, kclosest, path, radius));
}

boost::python::object py_select_pole_points(const Point3ArrayPtr &point, const real_t &radius, const uint_t &iterations, const real_t &tolerance) {
  return make_pair_tuple(select_pole_points(point, radius, iterations, tolerance));
}

boost::python::object py_select_pole_points_mt(const Point3ArrayPtr &point, const real_t &radius, const uint_t &iterations, const real_t &tolerance) {
  return make_pair_tuple(select_pole_points_mt(point, radius, iterations, tolerance));
}

#ifdef WITH_CGAL
boost::python::object py_pointset_plane(const Point3ArrayPtr points, const Index &group) {
  return make_pair_tuple(pointset_plane(points, group));
}
#endif

void export_PointManip() {
  def("contract_point2", &contract_point<Point2Array>, args("points", "radius"));
  def("contract_point3", &contract_point<Point3Array>, args("points", "radius"));
  def("contract_point4", &contract_point<Point4Array>, args("points", "radius"));


  def("generate_point_color", &generate_point_color, args("point"));
  def("select_soil", &select_soil, args("point", "kclosest", "topHeightPourcent", "bottomThreshold"));
  def("get_shortest_path", &get_shortest_path, args("points", "kclosest", "point_begin", "point_end"));
  def("add_baricenter_points_of_path", &py_add_baricenter_points_of_path, args("point", "kclosest", "path", "radius"));
  def("get_radii_of_path", &get_radii_of_path, args("point", "kclosest", "path", "around_radius"));
  def("select_wire_from_path", &select_wire_from_path, args("point", "path", "radius", "radii"));
  def("filter_min_densities", &filter_min_densities, args("densities", "densityratio"));
  def("filter_max_densities", &filter_max_densities, args("densities", "densityratio"));
  def("select_pole_points", &py_select_pole_points, (bp::arg("point"), bp::arg("radius"), bp::arg("iterations"), bp::arg("tolerance") = -1.0));
  def("select_pole_points_mt", &py_select_pole_points_mt, (bp::arg("point"), bp::arg("radius"), bp::arg("iterations"), bp::arg("tolerance") = -1.0));

#ifdef WITH_CGAL
  def("delaunay_point_connection", &delaunay_point_connection, args("points"));
  def("delaunay_triangulation", &delaunay_triangulation, args("points"));
  def("k_closest_points_from_delaunay", &k_closest_points_from_delaunay, args("points", "k"));
#endif
#ifdef WITH_ANN
  def("k_closest_points_from_ann", &k_closest_points_from_ann, (bp::arg("points"), bp::arg("k"), bp::arg("symmetric") = false));
#endif

  def("symmetrize_connections", &symmetrize_connections, (bp::arg("adjacencies")));
  def("get_all_connex_components", &get_all_connex_components, (bp::arg("points"), bp::arg("adjacencies"), bp::arg("verbose") = false));
  def("connect_all_connex_components", &connect_all_connex_components, (bp::arg("points"), bp::arg("adjacencies"), bp::arg("verbose") = false));


  def("r_neighborhood", &r_neighborhood, args("pid", "points", "adjacencies", "radius"));
  def("r_neighborhoods", (IndexArrayPtr(*)(const Point3ArrayPtr, const IndexArrayPtr, const RealArrayPtr)) &r_neighborhoods, args("points", "adjacencies", "radii"));
  def("r_neighborhoods", (IndexArrayPtr(*)(const Point3ArrayPtr, const IndexArrayPtr, real_t, bool)) &r_neighborhoods, (bp::arg("points"), bp::arg("adjacencies"), bp::arg("radius"), bp::arg("verbose") = false));
  def("r_neighborhoods_mt", (IndexArrayPtr(*)(const Point3ArrayPtr, const IndexArrayPtr, real_t, bool)) &r_neighborhoods_mt, (bp::arg("points"), bp::arg("adjacencies"), bp::arg("radius"), bp::arg("verbose") = false));
  def("r_anisotropic_neighborhood", &r_anisotropic_neighborhood, args("pid", "points", "adjacencies", "radius", "direction", "alpha", "beta"));
  def("r_anisotropic_neighborhoods", (IndexArrayPtr (*)(const Point3ArrayPtr, const IndexArrayPtr, const RealArrayPtr, const Point3ArrayPtr, const real_t, const real_t)) &r_anisotropic_neighborhoods, args("points", "adjacencies", "radii", "directions", "alpha", "beta"));
  def("r_anisotropic_neighborhoods", (IndexArrayPtr (*)(const Point3ArrayPtr, const IndexArrayPtr, const real_t, const Point3ArrayPtr, const real_t, const real_t)) &r_anisotropic_neighborhoods, args("points", "adjacencies", "radius", "directions", "alpha", "beta"));

  def("k_neighborhood", &k_neighborhood, args("pid", "points", "adjacencies", "k"));
  def("k_neighborhoods", &k_neighborhoods, args("points", "adjacencies", "k"));

  def("density_from_r_neighborhood", &density_from_r_neighborhood, args("pid", "points", "adjacencies", "radius"));
  def("densities_from_r_neighborhood", (RealArrayPtr(*)(const Point3ArrayPtr, const IndexArrayPtr, const real_t)) &densities_from_r_neighborhood, args("points", "adjacencies", "radius"));
  def("densities_from_r_neighborhood", (RealArrayPtr(*)(const IndexArrayPtr, const real_t)) &densities_from_r_neighborhood, args("neighborhood", "radius"));

  def("pointset_max_distance", (real_t (*)(uint32_t, const Point3ArrayPtr, const Index &)) &pointset_max_distance, args("pid", "points", "group"));
  def("pointset_max_distance", (real_t (*)(const Vector3 &, const Point3ArrayPtr, const Index &)) &pointset_max_distance, args("center", "points", "group"));
  def("pointset_mean_distance", &pointset_mean_distance<Index>, args("center", "points", "group"));
  def("pointset_mean_distances", &pointset_mean_distances<IndexArray>, args("points", "groups"));
  def("pointset_mean_radial_distance", &pointset_mean_radial_distance, args("center", "direction", "points", "group"));
  def("pointset_max_radial_distance", &pointset_max_radial_distance, args("center", "direction", "points", "group"));

  def("pointset_covariance", &pointset_covariance, (arg("points"), arg("group") = Index()));

  def("density_from_k_neighborhood", &density_from_k_neighborhood, (bp::arg("pid"), bp::arg("points"), bp::arg("adjacencies"), bp::arg("k") = 0), "Compute density of a point according to its k neighboordhood. If k is 0, its value is deduced from adjacencies.");
  def("densities_from_k_neighborhood", &densities_from_k_neighborhood, (bp::arg("points"), bp::arg("adjacencies"), bp::arg("k") = 0), "Compute local densities of a set of points according to their k neighboordhood. If k is 0, its value is deduced from adjacencies.");

#ifdef WITH_CGAL
  def("pointset_plane", &py_pointset_plane, args("points", "group"));
  def("pointset_orientation", &pointset_orientation, args("points", "group"));
  def("pointsets_orientations", &pointsets_orientations, args("points", "groups"));
  def("pointset_normal", &pointset_normal, (bp::arg("points"), bp::arg("groups")));
  def("pointsets_normals", &pointsets_normals, (bp::arg("points"), bp::arg("groups")));
  def("triangleset_orientation", &triangleset_orientation, args("points", "triangles"));

#ifdef CGAL_AND_SVD_SOLVER_ENABLED
  def("principal_curvatures",&py_principal_curvatures_0,(bp::arg("points"),bp::arg("pid"),bp::arg("group"),bp::arg("fitting_degree")=4,bp::arg("monge_degree")=4),
      "Compute principal curvature information. Return a tuple with folowin informations: (origin,(maximal_curvature_direction,maximal_curvature),(minimal_curvature_direction,minimal_curvature),normal)");
  def("principal_curvatures",&py_principal_curvatures_1,(bp::arg("points"),bp::arg("groups"),bp::arg("fitting_degree")=4,bp::arg("monge_degree")=4));
  def("principal_curvatures",&py_principal_curvatures_2,(bp::arg("points"),bp::arg("adjacencies"),bp::arg("radius"),bp::arg("fitting_degree")=4,bp::arg("monge_degree")=4));
#endif
#endif
  def("pointsets_orient_normals", (Point3ArrayPtr (*)(const Point3ArrayPtr, const Point3ArrayPtr, const IndexArrayPtr)) &pointsets_orient_normals, (bp::arg("normals"), bp::arg("points"), bp::arg("adjacencies")));
  def("pointsets_orient_normals", (Point3ArrayPtr (*)(const Point3ArrayPtr, uint32_t, const IndexArrayPtr)) &pointsets_orient_normals, (bp::arg("normals"), bp::arg("source"), bp::arg("adjacencies")));

  def("point_section", (Index (*)(uint32_t, const Point3ArrayPtr, const IndexArrayPtr, const Vector3 &, real_t)) &point_section, args("pid", "points", "adjacencies", "direction", "width"));
  def("point_section", (Index (*)(uint32_t, const Point3ArrayPtr, const IndexArrayPtr, const Vector3 &, real_t, real_t)) &point_section, args("pid", "points", "adjacencies", "direction", "width", "maxradius"));
  def("points_sections", &points_sections, args("points", "adjacencies", "directions", "width"));
  def("section_normal", &section_normal, args("pointnormals", "section"));
  def("sections_normals", &sections_normals, args("pointnormals", "sections"));

  def("pointset_circle", &py_estimate_pointset_circle, (bp::arg("points"), bp::arg("group"), bp::arg("direction") = bp::object(), bp::arg("bounding") = false));
  def("pointsets_circles", &py_estimate_pointsets_circles, (arg("points"), bp::arg("groups"), bp::arg("directions") = Point3ArrayPtr(0), bp::arg("bounding") = false));

  def("pointsets_section_circles", &py_estimate_pointsets_section_circles, (arg("points"), bp::arg("adjacencies"), bp::arg("directions"), bp::arg("width"), bp::arg("bounding") = false));
  def("adaptive_section_circles", &py_adaptive_section_circles, (arg("points"), bp::arg("adjacencies"), bp::arg("directions"), bp::arg("widths"), bp::arg("maxradii")));
  def("adaptive_section_circles", &py_adaptive_section_circles2, (arg("points"), bp::arg("adjacencies"), bp::arg("directions"), bp::arg("widths"), bp::arg("maxradii")));

  def("centroid_of_group", &centroid_of_group<Index>, args("", "group"));
  def("centroids_of_groups", &centroids_of_groups<IndexArray>, args("points", "groups"));
  def("points_clusters", &points_clusters, args("points", "clustercentroid"));
  def("cluster_points", &cluster_points, args("points", "clustercentroid"));


  def("adaptive_radii", &adaptive_radii, (bp::arg("density"), bp::arg("minradius"), bp::arg("maxradius"), bp::arg("densityradiusmap") = QuantisedFunctionPtr(0)), "Compute a radius for each density value");
  def("adaptive_contration", &adaptive_contration, (bp::arg("points"), bp::arg("orientations"), bp::arg("adjacencies"),
          bp::arg("densities"), bp::arg("minradius"), bp::arg("maxradius"),
                  bp::arg("densityradiusmap") = NULL,
                  bp::arg("alpha") = 1.0, bp::arg("beta") = 1.0), "Contract the pointset with an adptive radius of contraction");

  def("adaptive_section_contration", &py_adaptive_section_contration, (bp::arg("points"), bp::arg("orientations"), bp::arg("adjacencies"),
          bp::arg("densities"), bp::arg("minradius"), bp::arg("maxradius"),
                  bp::arg("densityradiusmap") = NULL,
                  bp::arg("alpha") = 1.0, bp::arg("beta") = 1.0), "Contract the pointset with an adptive radius of contraction");

  def("get_sorted_element_order", &get_sorted_element_order, args("elements"));
  def("points_dijkstra_shortest_path", &py_points_dijkstra_shortest_path, args("points", "adjacencies", "root"));
  def("quotient_points_from_adjacency_graph", &quotient_points_from_adjacency_graph, args("binsize", "points", "adjacencies", "distances_to_root"));
  def("quotient_adjacency_graph", &quotient_adjacency_graph, args("adjacencies", "groups"));
  def("skeleton_from_distance_to_root_clusters", &py_skeleton_from_distance_to_root_clusters,
      (bp::arg("points"), bp::arg("root"), bp::arg("binsize"), bp::arg("k") = 10, bp::arg("connect_all_points") = false, bp::arg("verbose") = false), "Implementation of Xu et al. 07 method for main branching system");

  def("points_in_range_from_root", &points_in_range_from_root, args("initiallevel", "binsize", "distances_to_root"));
  def("next_quotient_points_from_adjacency_graph", &py_next_quotient_points_from_adjacency_graph, args("initiallevel", "binsize", "current", "adjacencies", "distances_to_root"));

  def("determine_children", &py_determine_children);
  def("carried_length", &carried_length, bp::args("points", "parents"));
  def("subtrees_size", (Uint32Array1Ptr(*)(const Uint32Array1Ptr)) &subtrees_size, bp::args("parents"));
  def("subtrees_size", (Uint32Array1Ptr(*)(const IndexArrayPtr, uint32_t)) &subtrees_size, bp::args("children", "root"));

  def("optimize_orientations", &optimize_orientations, bp::args("points", "parents", "weights"));
  def("optimize_positions", &optimize_positions, bp::args("points", "orientations", "parents", "weights"));

  def("average_radius", &average_radius, (bp::arg("points"), bp::arg("nodes"), bp::arg("parents"), bp::arg("maxclosestnodes") = 10));
  def("distance_to_shape", &distance_to_shape, (bp::arg("points"), bp::arg("nodes"), bp::arg("parents"), bp::arg("radii"), bp::arg("maxclosestnodes") = 10));
  def("average_distance_to_shape", &average_distance_to_shape, (bp::arg("points"), bp::arg("nodes"), bp::arg("parents"), bp::arg("radii"), bp::arg("maxclosestnodes") = 10));
  def("points_at_distance_from_skeleton", &points_at_distance_from_skeleton, (bp::arg("points"), bp::arg("nodes"), bp::arg("parents"), bp::arg("distance"), bp::arg("maxclosestnodes") = 10),
      "Return indices of all point which are below a given distance ot the skeleton. If distance is negative, return point above a distance");

  def("estimate_radii_from_points", &estimate_radii_from_points, (bp::arg("points"), bp::arg("nodes"), bp::arg("parents"), bp::arg("maxmethod") = false, bp::arg("maxclosestnodes") = 10));
  def("estimate_radii_from_pipemodel", &estimate_radii_from_pipemodel, (bp::arg("nodes"), bp::arg("parents"), bp::arg("weights"), bp::arg("averageradius"), bp::arg("pipeexponent") = 2.5));

  def("min_max_mean_edge_length", (Vector3 (*)(const Point3ArrayPtr, Uint32Array1Ptr)) &min_max_mean_edge_length, (bp::arg("points"), bp::arg("parents")));
  def("min_max_mean_edge_length", (Vector3 (*)(const Point3ArrayPtr, IndexArrayPtr)) &min_max_mean_edge_length, (bp::arg("points"), bp::arg("graph")));

  def("node_continuity_test", &py_node_continuity_test, (bp::arg("node"), bp::arg("radius"), bp::arg("parent"), bp::arg("parentradius"), bp::arg("child"), bp::arg("radius"), bp::arg("overlapfilter") = 0.5, bp::arg("verbose") = false));
  def("node_intersection_test", &py_node_intersection_test, (bp::arg("parent"), bp::arg("parentradius"), bp::arg("node1"), bp::arg("radius1"), bp::arg("node2"), bp::arg("radius2"), bp::arg("overlapfilter") = 0.5, bp::arg("verbose") = false));
  def("detect_short_nodes", &detect_short_nodes, (bp::arg("nodes"), bp::arg("parents"), bp::arg("edgelengthfilter") = 0.001));
  def("remove_nodes", &py_remove_nodes, (bp::arg("toremove"), bp::arg("nodes"), bp::arg("parents"), bp::arg("radii") = RealArrayPtr(0)));
  def("detect_similar_nodes", &detect_similar_nodes, (bp::arg("nodes"), bp::arg("parents"), bp::arg("radii"), bp::arg("weight"), bp::arg("overlapfilter") = 0.5));
  def("merge_nodes", &py_merge_nodes, (bp::arg("tomerge"), bp::arg("nodes"), bp::arg("parents"), bp::arg("radii"), bp::arg("weight")));

  def("pointset_mean_direction", &pointset_mean_direction, (bp::arg("origin"), bp::arg("points"), bp::arg("group") = Index()));
  def("pointset_directions", &pointset_directions, (bp::arg("origin"), bp::arg("points"), bp::arg("group") = Index()));
  def("pointset_angulardirections", &pointset_angulardirections, (bp::arg("points"), bp::arg("origin") = Vector3::ORIGIN, bp::arg("group") = Index()));

  def("findClosestFromSubset", &py_findClosestFromSubset, (bp::arg("origin"), bp::arg("points"), bp::arg("group") = Index()));

  def("orientations_distances", &orientations_distances, (bp::arg("orientations"), bp::arg("group") = Index()));
  def("orientations_similarities", &orientations_similarities, (bp::arg("orientations"), bp::arg("group") = Index()));

  def("cluster_junction_points", &py_cluster_junction_points, (bp::arg("pointtoppology"), bp::arg("group1"), bp::arg("group2")));

  def("pointset_median", &pointset_median, (bp::arg("points")));
  def("approx_pointset_median", &approx_pointset_median, (bp::arg("points"), bp::arg("nbIterMax") = 200));
}


/* ----------------------------------------------------------------------- */
