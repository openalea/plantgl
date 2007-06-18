#include <algorithm>
#include <tool/util_array.h>
#include <tool/util_tuple.h>
#include <math/util_math.h>
#include <scenegraph/container/pointarray.h>
#include <scenegraph/transformation/ifs.h>
#include <scenegraph/transformation/mattransformed.h>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

#include "../util/exception.h"
#include "../util/export_refcountptr.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace boost::python;

#include "arrays_macro.h"

boost::python::object * sorting_method = NULL;

template <class T>
bool py_comp(const T& a, const T& b){
    int res = extract<int>((*sorting_method)(object(a),object(b)));
    return res < 0;
}
template <class T>
void py_sort(T * pts, boost::python::object sort_method){
    boost::python::object * old_sorting_method = sorting_method;
    sorting_method = & sort_method;
    std::stable_sort(pts->getBegin(),pts->getEnd(),py_comp<T::element_type>);
    sorting_method = old_sorting_method;
}

EXPORT_FUNCTION( p2a, Vector2, Point2Array )
EXPORT_FUNCTION( p3a, Vector3, Point3Array )
EXPORT_FUNCTION( p4a, Vector4, Point4Array )
EXPORT_FUNCTION( t4a, Transform4Ptr, Transform4Array )
EXPORT_FUNCTION( m4a, Matrix4, Matrix4Array )

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
    std::pair<T::const_iterator,T::const_iterator> index = pts->getXMinAndMax();
    return make_tuple(distance(pts->getBegin(),index.first),distance(pts->getBegin(),index.second)); 
}

template<class T>
object pa_yminmaxindex(const T * pts){ 
    if(pts->isEmpty()) return object();
    std::pair<T::const_iterator,T::const_iterator> index = pts->getYMinAndMax();
    return make_tuple(distance(pts->getBegin(),index.first),distance(pts->getBegin(),index.second)); 
}

template<class T>
object pa_zminmaxindex(const T * pts){ 
    if(pts->isEmpty()) return object();
    std::pair<T::const_iterator,T::const_iterator> index = pts->getZMinAndMax();
    return make_tuple(distance(pts->getBegin(),index.first),distance(pts->getBegin(),index.second)); 
}

template<class T>
object pa_wminmaxindex(const T * pts){ 
    if(pts->isEmpty()) return object();
    std::pair<T::const_iterator,T::const_iterator> index = pts->getWMinAndMax();
    return make_tuple(distance(pts->getBegin(),index.first),distance(pts->getBegin(),index.second)); 
}

template<class T>
object pa_bounds(const T * pts){ 
    if(pts->isEmpty()) return object();
    std::pair<T::element_type,T::element_type> bounds = pts->getBounds();
    return make_tuple(bounds.first,bounds.second); 
}

void export_pointarrays()
{
  EXPORT_ARRAY( p2a, Point2Array, "Point2Array([Vector2(x,y),...])")
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
    .def( "hausdorff_distance", &hausdorff_distance<Point2Array>); 
    DEFINE_NUMPY( p2a );
  EXPORT_CONVERTER(p2a);

  EXPORT_ARRAY( p3a, Point3Array, "Point3Array([Vector3(x,y,z),...])")
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
    .def( "hausdorff_distance", &hausdorff_distance<Point3Array>); 
    DEFINE_NUMPY( p3a );
  EXPORT_CONVERTER(p3a);

  EXPORT_ARRAY( p4a, Point4Array, "Point4Array([Vector4(x,y,z,w),...])")
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
    .def( "hausdorff_distance", &hausdorff_distance<Point4Array>); 
    DEFINE_NUMPY( p4a );
  EXPORT_CONVERTER(p4a);


  EXPORT_ARRAY( t4a, Transform4Array,"Transform4Array([Transform4(...),...])" );
  EXPORT_CONVERTER(t4a);
  EXPORT_ARRAY( m4a, Matrix4Array,"Matrix4Array([Matrix4(...),...])" );
  EXPORT_CONVERTER(m4a);
}



