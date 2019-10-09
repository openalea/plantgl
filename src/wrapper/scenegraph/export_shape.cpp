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



#include <plantgl/scenegraph/scene/scene.h>
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/appearance/appearance.h>
#include <string>

#include <plantgl/python/export_property.h>
#include <plantgl/python/export_refcountptr.h>
#include <boost/python/make_constructor.hpp>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;

#define bp boost::python

DEF_POINTEE(Shape)
DEF_POINTEE(Shape3D)

void export_Shape3D()
{
  class_< Shape3D, Shape3DPtr, bases< SceneObject >, boost::noncopyable >("Shape3D",
      "Abstract base class for shape that can be stored into a scene.",no_init);

  implicitly_convertible<Shape3DPtr, SceneObjectPtr >();
}

struct sh_pickle_suite : boost::python::pickle_suite
{
    static boost::python::tuple getinitargs(Shape const& sh)
    {
        return boost::python::make_tuple<GeometryPtr,AppearancePtr,uint_t,uint_t>
                    (sh.getGeometry(),sh.getAppearance(),sh.getId(),sh.getParentId());
    }
};

uint_t sh_getptrid(const Shape * sh) { return sh->SceneObject::getId(); }

void export_Shape()
{
  class_< Shape, ShapePtr, bases< Shape3D > , boost::noncopyable >("Shape",
      "A Shape is composed of a Geometry object and an Appearance object.\n"
      "There is an optional id to identy the shape and parent id to store shape relationship.",
      init< boost::python::optional<const GeometryPtr&, const AppearancePtr &, uint_t, uint_t > >
            ("Shape( geometry, appearance, id, parentId )",
                (bp::arg("geometry")   = GeometryPtr(),
                 bp::arg("appearance") = Material::DEFAULT_MATERIAL,
                 bp::arg("id")         = Shape::NOID,
                 bp::arg("parentId")   = Shape::NOID )))
    .def(init< boost::python::optional<const GeometryPtr&, const ImageTexturePtr &, uint_t, uint_t > >
            ("Shape( geometry, appearance, id, parentId )",
                (bp::arg("geometry"),
                 bp::arg("appearance"),
                 bp::arg("id")         = Shape::NOID,
                 bp::arg("parentId")   = Shape::NOID )))
    .add_static_property("NOID",make_getter(&Shape::NOID))
    .DEF_PGLBASE(Shape)
    .DEC_PTR_PROPERTY(appearance, Shape,Appearance, AppearancePtr)
    .DEC_PTR_PROPERTY(geometry, Shape, Geometry,GeometryPtr)
    .def_readwrite("id", &Shape::id)
    .def_readwrite("parentId", &Shape::parentId)
    .def("getSceneObjectId", &sh_getptrid)
    .def("setComputedName", &Shape::setComputedName)
    .def_pickle(sh_pickle_suite());
    ;

    /*
    boost::python::converter::intrusive_ptr_from_python<Shape>();
    register_ptr_to_python< intrusive_ptr<Shape> >();
    */


  implicitly_convertible<ShapePtr, Shape3DPtr >();
}
