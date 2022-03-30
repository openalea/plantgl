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



#include <plantgl/python/export_list.h>
#include <plantgl/python/extract_tuple.h>
#include <plantgl/python/extract_list.h>
#include <plantgl/algo/grid/regularpointgrid.h>
#include <boost/python.hpp>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python
/* ----------------------------------------------------------------------- */


template<class PointGrid>
 object py_query_ball_point(PointGrid * grid, typename PointGrid::VectorType point, real_t radius)
 { return make_list(grid->query_ball_point(point,radius))(); }

template<class PointGrid>
 object py_query_points_in_cone(PointGrid * grid, typename PointGrid::VectorType origin, typename PointGrid::VectorType direction, real_t radius, real_t angle)
 { return make_list(grid->query_points_in_cone(origin,direction,radius,angle))(); }

template<class PointGrid>
 object py_closest_point(PointGrid * grid, typename PointGrid::VectorType point, real_t maxdist = REAL_MAX)
 {
    typename PointGrid::PointIndex res;
     if (grid->closest_point(point,res,maxdist)) return object(res);
     else return object();
 }

template<class PointGrid>
 object py_query_voxels_in_cone(PointGrid * grid, typename PointGrid::VectorType origin, typename PointGrid::VectorType direction, real_t radius, real_t angle)
 {   return make_list(grid->query_voxels_in_cone(origin,direction,radius,angle))();  }

template<class PointGrid>
 object py_query_voxels_around_point(PointGrid * grid, typename PointGrid::VectorType center, real_t radius)
 { return make_list(grid->query_voxels_around_point(center,radius))(); }

template<class PointGrid>
 object py_query_voxels_in_box(PointGrid * grid, const typename PointGrid::Index center, object maxradius, object minradius = object())
 {
     int v = 0;
     typename PointGrid::Index cminradius(v);
     if (minradius != object())cminradius =  extract_tuple<typename PointGrid::Index>(minradius);
     return make_list(grid->query_voxels_in_box(center,extract_tuple<typename PointGrid::Index>(maxradius),cminradius))(); }

template<class PointGrid>
object py_get_enabled_point_indices(PointGrid * grid)
{ return make_list(grid->get_enabled_point_indices())(); }

template<class PointGrid>
object py_get_disabled_point_indices(PointGrid * grid)
{ return make_list(grid->get_disabled_point_indices())(); }

template<class PointGrid>
object  py_index(PointGrid * grid, typename PointGrid::CellId c){
     return make_pgl_tuple(grid->index(c))();
 }

template<class PointGrid>
bool py_isValidIndex(PointGrid * grid, object c){
    return grid->isValidIndex(extract_tuple<typename PointGrid::Index>(c));
}

template<class PointGrid>
typename PointGrid::VectorType py_getVoxelCenter(PointGrid * grid, object c){
     return grid->getVoxelCenter(extract_tuple<typename PointGrid::Index>(c));
}

template<class PointGrid>
typename PointGrid::VectorType py_getVoxelCenterFromId(PointGrid * grid,  typename PointGrid::CellId vid){
     return grid->getVoxelCenter(vid);
}

template<class PointGrid>
typename PointGrid::VectorType py_getVoxelLowerPoint(PointGrid * grid, object c){
     return grid->getVoxelLowerPoint(extract_tuple<typename PointGrid::Index>(c));
}

template<class PointGrid>
typename PointGrid::VectorType py_getVoxelLowerPointFromId(PointGrid * grid,  typename PointGrid::CellId vid){
     return grid->getVoxelLowerPoint(vid);
}

template<class PointGrid>
typename PointGrid::VectorType py_getVoxelUpperPoint(PointGrid * grid, object c){
     return grid->getVoxelUpperPoint(extract_tuple<typename PointGrid::Index>(c));
}

template<class PointGrid>
typename PointGrid::VectorType py_getVoxelUpperPointFromId(PointGrid * grid,  typename PointGrid::CellId vid){
     return grid->getVoxelUpperPoint(vid);
}

template<class PointGrid>
void py_enablepoints(PointGrid * grid, object plist ){
     return grid->enable_points(extract_vec<typename PointGrid::PointIndex>(plist));
}

template<class PointGrid>
void py_disablepoints(PointGrid * grid, object plist ){
     return grid->disable_points(extract_vec<typename PointGrid::PointIndex>(plist));
}

template<class PointGrid>
object py_getCorners(PointGrid * grid){
     return make_list(grid->getCorners())();
}

template<class PointGrid>
object py_getVoxelCorners(PointGrid * grid, object c){
     return make_list(grid->getVoxelCorners(extract_tuple<typename PointGrid::Index>(c)))();
}

template<class PointGrid>
object py_getVoxelCornersFromId(PointGrid * grid, typename PointGrid::CellId vid){
     return make_list(grid->getVoxelCorners(vid))();
}

template<class PointGrid>
object py_getMaxIndexDistanceToBorder(PointGrid * grid, object c){
     return make_pgl_tuple(grid->getMaxIndexDistanceToBorder(extract_tuple<typename PointGrid::Index>(c)));
}

template<class PointGrid>
object py_getDimensions(PointGrid * grid){
     return make_pgl_tuple(grid->dimensions());
}

template<class PointGrid>
object py_filter_enabled(PointGrid * grid, boost::python::object vidlist){
     return make_list(grid->filter_enabled(extract_vec<typename PointGrid::PointIndex>(vidlist)()))();
}

template<class PointGrid>
object py_filter_disabled(PointGrid * grid, boost::python::object vidlist){
     return make_list(grid->filter_disabled(extract_vec<typename PointGrid::PointIndex>(vidlist)()))();
}


template<class SpatialArray>
typename SpatialArray::CellId py_cellId(SpatialArray * grid, object c){
     return grid->cellId(extract_tuple<typename SpatialArray::Index>(c));
 }

template<class SpatialArray>
 object py_indexFromPoint(SpatialArray * grid, typename SpatialArray::VectorType v){
     return make_pgl_tuple(grid->indexFromPoint(v))();
 }

template<class SpatialArray>
size_t py_indexFromCoord1(SpatialArray * grid, typename SpatialArray::VectorType v, size_t dim){
     return grid->indexFromCoord(v,dim);
 }

template<class SpatialArray>
size_t  py_indexFromCoord2(SpatialArray * grid, real_t v, size_t dim){
     return grid->indexFromCoord(v,dim);
 }

 template<class SpatialArray>
 size_t py_nbDim(SpatialArray * grid){
     return grid->nbDimensions();
 }

 template<class PointGrid>
 object py_getVoxelPointIndices(PointGrid * grid, object c){
    extract<typename PointGrid::CellId> cidextractor(c);
    if (cidextractor.check()){
         return make_list(grid->getVoxelPointIndices(cidextractor()));
    }
    else {
     return make_list(grid->getVoxelPointIndices(extract_tuple<typename PointGrid::Index>(c)));
    }
 }

template<class PointGrid>
 object py_getVoxelPointIndicesFromId(PointGrid * grid,  typename PointGrid::CellId vid){
     return make_list(grid->getVoxelPointIndices(vid));
 }

template<class PointGrid>
size_t nbPointInVoxel(PointGrid * grid, object c){
    extract<typename PointGrid::CellId> cidextractor(c);
    if (cidextractor.check()){
     return grid->getVoxelPointIndices(cidextractor()).size();
    }
    else {
     return grid->getVoxelPointIndices(extract_tuple<typename PointGrid::Index>(c)).size();
    }
 }

template<class PointGrid>
size_t nbPointInVoxelFromId(PointGrid * grid, typename PointGrid::CellId vid){
     return grid->getVoxelPointIndices(vid).size();
 }

template<class SpatialArray>
class spatialarray_func : public boost::python::def_visitor<spatialarray_func<SpatialArray> >
{
    friend class boost::python::def_visitor_access;

    template <class classT>
    void visit(classT& c) const
    {
     c.def("size",&SpatialArray::size, "Return the number of element of the grid. Determined from its dimensions.")
     .def("valuesize",&SpatialArray::valuesize,"Return the size of the value container. Should be equal to size.")
     .def("dimensions",&py_getDimensions<SpatialArray>, "Return the dimensions of the grid in each axis")
     .def("getVoxelSize",&SpatialArray::getVoxelSize, "Return the size of the voxels of the grid.")
     .def("getOrigin",&SpatialArray::getOrigin, "Return the origin of the grid.")
     .def("indexFromPoint",&py_indexFromPoint<SpatialArray>)
     .def("cellIdFromPoint",&SpatialArray::cellIdFromPoint)
     .def("isValidId",&SpatialArray::isValidId)
     .def("isValidIndex",&py_isValidIndex<SpatialArray>)
     .def("getVoxelCenter",&py_getVoxelCenter<SpatialArray>)
     .def("getVoxelCenterFromId",&py_getVoxelCenterFromId<SpatialArray>)
     .def("getVoxelLowerPoint",&py_getVoxelLowerPoint<SpatialArray>)
     .def("getVoxelLowerPointFromId",&py_getVoxelLowerPointFromId<SpatialArray>)
     .def("getVoxelUpperPoint",&py_getVoxelUpperPoint<SpatialArray>)
     .def("getVoxelUpperPointFromId",&py_getVoxelUpperPointFromId<SpatialArray>)
     .def("getGridSize",&SpatialArray::getGridSize)
     .def("getLowerCorner",&SpatialArray::getLowerCorner)
     .def("getUpperCorner",&SpatialArray::getUpperCorner)
     .def("getCorners",&py_getCorners<SpatialArray>)
     .def("getVoxelCorners",&py_getVoxelCorners<SpatialArray>)
     .def("getVoxelCornersFromId",&py_getVoxelCornersFromId<SpatialArray>)
     .def("getMaxIndexDistanceToBorder",&py_getMaxIndexDistanceToBorder<SpatialArray>)
     .def("getMaxDistanceToBorder",&SpatialArray::getMaxDistanceToBorder) /**/
     .def("cellId",&py_cellId<SpatialArray>)
     .def("index",&py_index<SpatialArray>)
     .def("query_voxels_in_cone",&py_query_voxels_in_cone<SpatialArray>,bp::args("origin","direction","radius","angle"))
     .def("query_voxels_around_point",&py_query_voxels_around_point<SpatialArray>,bp::args("center","radius"))
     .def("query_voxels_in_box",&py_query_voxels_in_box<SpatialArray>,(bp::arg("center"),bp::arg("maxradius"),bp::arg("minradius")=bp::object()))
     .def("indexFromCoord",&py_indexFromCoord1<SpatialArray>)
     .def("indexFromCoord",&py_indexFromCoord2<SpatialArray>)
     .def("nbDimensions",&py_nbDim<SpatialArray>, "Return the number of dimensions of the grid")

         ;
    }
};



template<class PointGrid>
class pointgrid_func : public boost::python::def_visitor<pointgrid_func<PointGrid> >
{
    friend class boost::python::def_visitor_access;

    template <class classT>
    void visit(classT& c) const
    {
    c.def(init<real_t,typename PointGrid::PointContainerPtr>(args("voxelsize","points")))
     .def(init<typename PointGrid::PointContainerPtr,real_t>(args("points","voxelsizeratiofromglobal")))
     .def(init<typename PointGrid::VectorType,
               typename PointGrid::VectorType,
               typename PointGrid::VectorType,
               typename PointGrid::PointContainerPtr>(args("voxelsize","minpoint","maxpoint","points")))
     .def(spatialarray_func<PointGrid>())
     .def("query_ball_point",&py_query_ball_point<PointGrid>,bp::args("center","radius"))
     .def("query_points_in_cone",&py_query_points_in_cone<PointGrid>,bp::args("origin","direction","radius","angle"))
     .def("closest_point",&py_closest_point<PointGrid>,(bp::arg("point"),bp::arg("maxdist")=REAL_MAX))
     .def("enable_point",&PointGrid::enable_point)
     .def("disable_point",&PointGrid::disable_point)
     .def("is_point_enabled",&PointGrid::is_point_enabled)
     .def("get_enabled_points",&PointGrid::get_enabled_points)
     .def("get_disabled_points",&PointGrid::get_disabled_points)
     .def("get_enabled_point_indices",&py_get_enabled_point_indices<PointGrid>)
     .def("get_disabled_point_indices",&py_get_disabled_point_indices<PointGrid>)
     .def("enable_points",&py_enablepoints<PointGrid>)
     .def("disable_points",&py_disablepoints<PointGrid>)
     .def("nbFilledVoxels",&PointGrid::nbFilledVoxels)
     .def("filter_disabled",&py_filter_disabled<PointGrid>)
     .def("filter_enabled",&py_filter_enabled<PointGrid>)
     .def("getVoxelPointIndices",&py_getVoxelPointIndices<PointGrid>,bp::args("cellindex"),"Return the enabled points contained in the voxel")
     .def("getVoxelPointIndicesFromId",&py_getVoxelPointIndicesFromId<PointGrid>,bp::args("cellid"),"Return the enabled points contained in the voxel")
     .def("__getitem__",&py_getVoxelPointIndices<PointGrid>,bp::args("cellindex"),"Return the enabled points contained in the voxel")
     .def("nbPointInVoxel",&nbPointInVoxel<PointGrid>,bp::args("cellindex"),"Return the number of enabled points contained in the voxel")
     .def("nbPointInVoxelFromId",&nbPointInVoxelFromId<PointGrid>,bp::args("cellid"),"Return the number of enabled points contained in the voxel")
     .def("add_point",&PointGrid::add_point, "Add a point in the grid. Should be contained in the voxels.")
         ;
    }
};

