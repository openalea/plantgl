/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
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

#include <algorithm>
#include <plantgl/tool/util_array.h>
#include <plantgl/tool/util_tuple.h>
#include <plantgl/math/util_math.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/transformation/ifs.h>
#include <plantgl/scenegraph/transformation/mattransformed.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/exception.h>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace boost::python;

#include "arrays_macro.h"

boost::python::object * compare_method = NULL;

template <class T>
bool py_comp(const T& a, const T& b){
    int res = extract<int>((*compare_method)(object(a),object(b)));
    return res < 0;
}
template <class T>
void py_sort(T * pts, boost::python::object cmp_method){
    boost::python::object * old_compare_method = compare_method ;
    compare_method  = & cmp_method;
    std::stable_sort(pts->getBegin(),pts->getEnd(),py_comp<typename T::element_type>);
    compare_method  = old_compare_method;
}

template <class T>
bool py_comp_x(const T& a, const T& b){
    return a.x() < b.x();
}

template <class T>
bool py_comp_y(const T& a, const T& b){
    return a.y() < b.y();
}

template <class T>
bool py_comp_z(const T& a, const T& b){
    return a.z() < b.z();
}

template <class T>
bool py_comp_w(const T& a, const T& b){
    return a.w() < b.w();
}

template <class T>
void py_sort_x(T * pts){
    std::stable_sort(pts->getBegin(),pts->getEnd(),py_comp_x<typename T::element_type>);
}

template <class T>
void py_sort_y(T * pts){
    std::stable_sort(pts->getBegin(),pts->getEnd(),py_comp_x<typename T::element_type>);
}

template <class T>
void py_sort_z(T * pts){
    std::stable_sort(pts->getBegin(),pts->getEnd(),py_comp_z<typename T::element_type>);
}

template <class T>
void py_sort_w(T * pts){
    std::stable_sort(pts->getBegin(),pts->getEnd(),py_comp_w<typename T::element_type>);
}

template <class T>
boost::python::list py_partition(T * pts, boost::python::object cmp_method){
    boost::python::list rlist;
    typename T::const_iterator itPrevious = pts->getBegin();
    RCPtr<T> c_pointset (new T());
    c_pointset->pushBack(*itPrevious);
    rlist.append(object(c_pointset));
    for(typename T::const_iterator it = pts->getBegin()+1; it != pts->getEnd(); ++it) {
        if (cmp_method(object(*it),object(*itPrevious)) == 0) { 
            // True (1) means that there are in the same group. False (0) in 2 differents groups.
            c_pointset = RCPtr<T>(new T());
            c_pointset->pushBack(*it);
            rlist.append(object(c_pointset));
            itPrevious = it;
        }
        else { c_pointset->pushBack(*it); }
    }
    return rlist;
}

EXPORT_FUNCTION( p2a, Point2Array )
EXPORT_FUNCTION( p3a, Point3Array )
EXPORT_FUNCTION( p4a, Point4Array )
EXPORT_FUNCTION( t4a, Transform4Array )
EXPORT_FUNCTION( m4a, Matrix4Array )

EXPORT_NUMPY( p2a, Vector2, Point2Array, 0, 2, real_t )
EXPORT_NUMPY( p3a, Vector3, Point3Array, 0, 3, real_t )
EXPORT_NUMPY( p4a, Vector4, Point4Array, 0, 4, real_t )

Point3Array * p3_from_p2(Point2ArrayPtr pts, real_t z)
{ return new Point3Array(*pts,z); }

Point4Array * p4_from_p2(Point2ArrayPtr pts, real_t z, real_t w)
{ return new Point4Array(*pts,z,w); }

Point4Array * p4_from_p3(Point3ArrayPtr pts,  real_t w)
{ return new Point4Array(*pts,w); }

template<class T>
int pa_xminindex(const T * pts){ if(pts->isEmpty()) return -1; return distance(pts->getBegin(),pts->getXMin()); }

template<class T>
int pa_xmaxindex(const T * pts){ if(pts->isEmpty()) return -1; return distance(pts->getBegin(),pts->getXMax()); }

template<class T>
int pa_yminindex(const T * pts){ if(pts->isEmpty()) return -1; return distance(pts->getBegin(),pts->getYMin()); }

template<class T>
int pa_ymaxindex(const T * pts){ if(pts->isEmpty()) return -1; return distance(pts->getBegin(),pts->getYMax()); }

template<class T>
int pa_zminindex(const T * pts){ if(pts->isEmpty()) return -1; return distance(pts->getBegin(),pts->getZMin()); }

template<class T>
int pa_zmaxindex(const T * pts){ if(pts->isEmpty()) return -1; return distance(pts->getBegin(),pts->getZMax()); }

template<class T>
int pa_wminindex(const T * pts){ if(pts->isEmpty()) return -1; return distance(pts->getBegin(),pts->getWMin()); }

template<class T>
int pa_wmaxindex(const T * pts){ if(pts->isEmpty()) return -1; return distance(pts->getBegin(),pts->getWMax()); }

template<class T>
object pa_xminmaxindex(const T * pts){ 
    if(pts->isEmpty()) return object();
    std::pair<typename T::const_iterator,typename T::const_iterator> index = pts->getXMinAndMax();
    return make_tuple(distance(pts->getBegin(),index.first),distance(pts->getBegin(),index.second)); 
}

template<class T>
object pa_yminmaxindex(const T * pts){ 
    if(pts->isEmpty()) return object();
    std::pair<typename T::const_iterator,typename T::const_iterator> index = pts->getYMinAndMax();
    return make_tuple(distance(pts->getBegin(),index.first),distance(pts->getBegin(),index.second)); 
}

template<class T>
object pa_zminmaxindex(const T * pts){ 
    if(pts->isEmpty()) return object();
    std::pair<typename T::const_iterator,typename T::const_iterator> index = pts->getZMinAndMax();
    return make_tuple(distance(pts->getBegin(),index.first),distance(pts->getBegin(),index.second)); 
}

template<class T>
object pa_wminmaxindex(const T * pts){ 
    if(pts->isEmpty()) return object();
    std::pair<typename T::const_iterator,typename T::const_iterator> index = pts->getWMinAndMax();
    return make_tuple(distance(pts->getBegin(),index.first),distance(pts->getBegin(),index.second)); 
}

template<class T>
object pa_bounds(const T * pts){ 
    if(pts->isEmpty()) return object();
    std::pair<typename T::element_type,typename T::element_type> bounds = pts->getBounds();
    return make_tuple(bounds.first,bounds.second); 
}

void export_pointarrays()
{
  EXPORT_ARRAY_CT( p2a, Point2Array, "Point2Array([Vector2(x,y),...])")
    .def( "getLength", &Point2Array::getLength) 
    .def( "getCenter", &Point2Array::getCenter) 
    .def( "getExtent", &Point2Array::getExtent) 
    .def( "getBounds", &pa_bounds<Point2Array>) 
    .def( "normalize", &Point2Array::normalize) 
    .def( "getXMinIndex", &pa_xminindex<Point2Array>) 
    .def( "getYMinIndex", &pa_yminindex<Point2Array>) 
    .def( "getXMaxIndex", &pa_xmaxindex<Point2Array>) 
    .def( "getYMaxIndex", &pa_ymaxindex<Point2Array>) 
    .def( "getXMinAndMaxIndex", &pa_xminmaxindex<Point2Array>) 
    .def( "getYMinAndMaxIndex", &pa_yminmaxindex<Point2Array>) 
    .def( "sort", &py_sort<Point2Array>) 
    .def( "sortX", &py_sort_x<Point2Array>) 
    .def( "sortY", &py_sort_y<Point2Array>) 
    .def( "partition", &py_partition<Point2Array>) 
    .def( "hausdorff_distance", &hausdorff_distance<Point2Array>)
    .def( "transform", &Point2Array::transform)
    DEFINE_NUMPY( p2a );
  EXPORT_CONVERTER(Point2Array);

  EXPORT_ARRAY_CT( p3a, Point3Array, "Point3Array([Vector3(x,y,z),...])")
    .def( "__init__", make_constructor( p3_from_p2 ), "Point3Array(Point2Array a, z)" ) 
    .def( "getLength", &Point3Array::getLength) 
    .def( "getCenter", &Point3Array::getCenter) 
    .def( "getExtent", &Point3Array::getExtent) 
    .def( "getBounds", &pa_bounds<Point3Array>) 
    .def( "project",   &Point3Array::project) 
    .def( "normalize", &Point3Array::normalize) 
    .def( "getXMinIndex", &pa_xminindex<Point3Array>) 
    .def( "getYMinIndex", &pa_yminindex<Point3Array>) 
    .def( "getZMinIndex", &pa_zminindex<Point3Array>) 
    .def( "getXMaxIndex", &pa_xmaxindex<Point3Array>) 
    .def( "getYMaxIndex", &pa_ymaxindex<Point3Array>) 
    .def( "getZMaxIndex", &pa_zmaxindex<Point3Array>) 
    .def( "getXMinAndMaxIndex", &pa_xminmaxindex<Point3Array>) 
    .def( "getYMinAndMaxIndex", &pa_yminmaxindex<Point3Array>) 
    .def( "getZMinAndMaxIndex", &pa_zminmaxindex<Point3Array>) 
    .def( "sort", &py_sort<Point3Array>) 
    .def( "sortX", &py_sort_x<Point3Array>) 
    .def( "sortY", &py_sort_y<Point3Array>) 
    .def( "sortZ", &py_sort_z<Point3Array>) 
    .def( "partition", &py_partition<Point3Array>) 
    .def( "hausdorff_distance", &hausdorff_distance<Point3Array>)
    .def( "transform", (void(Point3Array::*)(const Matrix3&))&Point3Array::transform)
    .def( "transform", (void(Point3Array::*)(const Matrix4&))&Point3Array::transform)
    DEFINE_NUMPY( p3a );
  EXPORT_CONVERTER(Point3Array);

  EXPORT_ARRAY_CT( p4a, Point4Array, "Point4Array([Vector4(x,y,z,w),...])")
    .def( "__init__", make_constructor( p4_from_p2 ), "Point4Array(Point2Array a, z, w)" ) 
    .def( "__init__", make_constructor( p4_from_p3 ), "Point4Array(Point3Array a, w)" ) 
    .def( "getLength", &Point4Array::getLength) 
    .def( "getCenter", &Point4Array::getCenter) 
    .def( "getExtent", &Point4Array::getExtent) 
    .def( "getBounds", &pa_bounds<Point4Array>) 
    .def( "project",   &Point4Array::project) 
    .def( "normalize", &Point4Array::normalize) 
    .def( "getXMinIndex", &pa_xminindex<Point4Array>) 
    .def( "getYMinIndex", &pa_yminindex<Point4Array>) 
    .def( "getZMinIndex", &pa_zminindex<Point4Array>) 
    .def( "getWMinIndex", &pa_wminindex<Point4Array>) 
    .def( "getXMaxIndex", &pa_xmaxindex<Point4Array>) 
    .def( "getYMaxIndex", &pa_ymaxindex<Point4Array>) 
    .def( "getZMaxIndex", &pa_zmaxindex<Point4Array>) 
    .def( "getWMaxIndex", &pa_wmaxindex<Point4Array>) 
    .def( "getXMinAndMaxIndex", &pa_xminmaxindex<Point4Array>) 
    .def( "getYMinAndMaxIndex", &pa_yminmaxindex<Point4Array>) 
    .def( "getZMinAndMaxIndex", &pa_zminmaxindex<Point4Array>) 
    .def( "getWMinAndMaxIndex", &pa_wminmaxindex<Point4Array>) 
    .def( "sort", &py_sort<Point4Array>) 
    .def( "sortX", &py_sort_x<Point4Array>) 
    .def( "sortY", &py_sort_y<Point4Array>) 
    .def( "sortZ", &py_sort_z<Point4Array>) 
    .def( "sortW", &py_sort_w<Point4Array>) 
    .def( "partition", &py_partition<Point4Array>) 
    .def( "hausdorff_distance", &hausdorff_distance<Point4Array>)
    .def( "transform", &Point4Array::transform)
    DEFINE_NUMPY( p4a );
  EXPORT_CONVERTER(Point4Array);


  EXPORT_ARRAY_PTR( t4a, Transform4Array,"Transform4Array([Transform4(...),...])" );
  EXPORT_CONVERTER(Transform4Array);
  EXPORT_ARRAY_CT( m4a, Matrix4Array,"Matrix4Array([Matrix4(...),...])" );
  EXPORT_CONVERTER(Matrix4Array);
}



