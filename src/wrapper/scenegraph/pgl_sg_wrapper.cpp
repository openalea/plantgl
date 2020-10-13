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



#include <boost/python.hpp>
#include "export_scenegraph.h"
#include <plantgl/python/exception_core.h>
#include <plantgl/scenegraph/pgl_version.h>
#include <plantgl/scenegraph/scene/factory.h>
#include <plantgl/python/pyinterpreter.h>
#include <plantgl/python/export_list.h>
#include <iostream>
#if PGL_WITH_BOOST_NUMPY
#include <boost/python/numpy.hpp>
#endif


using namespace boost::python;

PGL_USING_NAMESPACE

void py_error_handler(const std::string& msg, const char * fname, int lineno){
    PythonInterpreterAcquirer py;
    PyErr_SetString(PyExc_ValueError, msg.c_str() );
    PyErr_Print();
    // boost::python::throw_error_already_set();
}

void py_warning_handler(const std::string& msg, const char * fname, int lineno){
    PythonInterpreterAcquirer py;
    PyErr_WarnExplicit(PyExc_Warning,msg.c_str(),"openalea.plantgl",lineno,fname,NULL);
}

void py_debug_handler(const std::string& msg, const char * fname, int lineno){
    PythonInterpreterAcquirer py;
    PyErr_WarnExplicit(PyExc_Warning,msg.c_str(),"openalea.plantgl",lineno,fname,NULL);
}

static bool py_error_style = false;

void set_python_error_style(bool value = true)
{
    if (py_error_style != value){
        py_error_style = value;
        if (value == true) {
            register_error_handler(&py_error_handler);
            register_warning_handler(&py_warning_handler);
            register_debug_handler(&py_debug_handler);
        }
        else {
            reset_error_handler();
            reset_warning_handler();
            reset_debug_handler();
        }
    }
}
bool get_python_error_style() { return py_error_style; }

boost::python::object py_get_pgl_supported_extensions() {
    return make_list(get_pgl_supported_extensions());
}

void module_sg()
{
#if PGL_WITH_BOOST_NUMPY
    boost::python::numpy::initialize();
#endif
    
    // check lib version.
    PGL_LIB_VERSION_CHECK

    define_stl_exceptions();
    set_python_error_style(true);


    export_SceneObject();

    export_arrays();
    export_arrays2();
    export_index();
    export_Color3();
    export_Color4();
    export_pointarrays();
    export_Image();


    export_BoundingBox();

    export_Appearance();
    export_Material();
    export_Texture2D();
    export_ImageTexture();
    export_Texture2DTransformation();

    export_Spectrum();
    export_MonoSpectral();
    export_MultiSpectral();

    export_Geometry();
    export_Primitive();

    export_Scene();
    export_Shape3D();
    export_Shape();

    export_ExplicitModel();
    export_LineicModel();
    export_ParametricModel();

    export_Mesh();
    export_Hull();
    export_Patch();
    export_SOR();
    export_PlanarModel();

    export_Curve2D();
    export_BezierCurve2D();
    export_NurbsCurve2D();

    export_geometryarrays();

    export_BezierCurve();
    export_NurbsCurve();

    export_ElevationGrid();
    export_Extrusion();

    export_AsymmetricHull();
    export_ExtrudedHull();
    export_Swung();

    export_SOR2D();
    export_Disc();

    export_BezierPatch();
    export_NurbsPatch();

    export_PointSet();
    export_PointSet2D();
    export_Polyline();
    export_Polyline2D();

    export_Box();
    export_Cone();
    export_Sphere();
    export_Revolution();
    export_Cylinder();
    export_Frustum();
    export_Paraboloid();

    export_Font();
    export_Text();

    export_TriangleSet();
    export_QuadSet();
    export_FaceSet();
    export_AmapSymbol();

    export_Group();
    export_Transformed();

    export_Transformation();
    export_Taper();
    export_Scaling();
    export_Translation();
    export_OrthonormalBasis3D();
    export_AxisRotation();
    export_EulerRotation();
    export_BaseOrientation();
    export_Transform4();

    export_Scaled();
    export_Translated();
    export_IFS();
    export_EulerRotated();
    export_AxisRotated();
    export_Oriented();
    export_Tapered();
    export_ScreenProjected();

    export_SceneCodec();
    export_SceneFactory();
    export_DeepCopier();

    export_Function();

    export_Plane();

    export_Progress();

    scope().attr("PGL_VERSION_STR") = getPGLVersionString();
    scope().attr("PGL_VERSION") = PGL_VERSION;
    def("get_pgl_version",&getPGLVersion);
    def("get_pgl_supported_extensions",&py_get_pgl_supported_extensions,"Gives all extensions supported by current version of PlantGL.");
    def("pgl_support_extension",&pgl_support_extension, args("ext"),"Tell wether PlantGL support a given extension");
    def("get_pgl_python_error_style",&get_python_error_style);
    def("set_pgl_python_error_style",&set_python_error_style);

    def("get_pgl_qt_version",&getPGLQtVersion, "Get the Qt version to which PlantGL is compiled with");
    def("get_pgl_qt_version_string",&getPGLQtVersionString, "Get the Qt version to which PlantGL is compiled with");

};

void finalize_sg()
{
    Py_AtExit( &SceneFactory::finalize );
}

#ifdef PGL_DEBUG
BOOST_PYTHON_MODULE(_pglsg_d)
#else
BOOST_PYTHON_MODULE(_pglsg)
#endif
{
  module_sg();
  finalize_sg();
};

