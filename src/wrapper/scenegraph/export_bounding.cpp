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



#include <plantgl/math/util_matrix.h>
#include <plantgl/scenegraph/geometry/boundingbox.h>

#include <plantgl/algo/base/discretizer.h>
#include <plantgl/algo/base/bboxcomputer.h>
#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/scene/scene.h>

#include <string>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"
#include <boost/python/make_constructor.hpp>


PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;
#define bp boost::python

DEF_POINTEE(BoundingBox)

std::string bbox_str(BoundingBox * bbox){
  std::stringstream ss;
  const Vector3& v = bbox->getLowerLeftCorner();
  const Vector3& w = bbox->getUpperRightCorner();
  ss << "BoundingBox(Vector3(" << v.x() << "," << v.y() << "," << v.z() << "),Vector3(" << w.x() << "," << w.y() << "," << w.z() << "))";
  return ss.str();
}

bool contains_bbox(BoundingBox * bbox, BoundingBox * bbox2){
    return bbox->include(*bbox2);
}

bool contains_v3(BoundingBox * bbox, Vector3 v){
    return intersection(*bbox,v);
}

bool intersect_bbox(BoundingBox * bbox, BoundingBox * bbox2){
    return bbox->intersect(*bbox2);
}

void extend_bbox(BoundingBox * bbox, BoundingBox * bbox2){
    bbox->extend(*bbox2);
}

void extend_bbox_v3(BoundingBox * bbox, Vector3 v){
    bbox->extend(v);
}

real_t distance_to_bbox(BoundingBox * bbox, BoundingBox * bbox2){
    return bbox->distance(*bbox2);
}

real_t distance_to_bbox_v3(BoundingBox * bbox, Vector3 v){
    return bbox->distance(v);
}


BoundingBox *  bbx_fromobj( boost::python::object o )
{
    Discretizer d;
    BBoxComputer bbc(d);
    extract<ScenePtr> e(o);
    if(!e.check()){
        SceneObject * geom = extract<SceneObject *>(o)();
        geom->apply(bbc);
    }
    else {
        bbc.process(e());
        // e()->apply(bbc);
    }
    // sc->apply(bbc);
    if (is_null_ptr(bbc.getBoundingBox())) return new BoundingBox();
    return new BoundingBox(*bbc.getBoundingBox());
}

void export_BoundingBox()
{
  class_< BoundingBox, BoundingBoxPtr, bases<RefCountObject> , boost::noncopyable >
    ("BoundingBox", "An axis aligned box represented by 2 opposite corners.", init< boost::python::optional < const Vector3&, const Vector3& > >
     ( "BoundingBox(Vector3 lowerLeft, Vector3 upperRight) "
       "Constructs a BoundingBox with the 2 opposing corners lowerLeft and upperRight.",
       (bp::arg("lowerLeft")= TOOLS(Vector3::ORIGIN),bp::arg("upperRight")=TOOLS(Vector3::ORIGIN))
       ) )
    .def( "__init__", make_constructor( bbx_fromobj ), "BoundingBox(geometry|scene) Constructs a BoundingBox from some geometries.")
    .def("set",&BoundingBox::set,"set(lowerLeft,upperRight)")
    .DEC_CT_PROPERTY(lowerLeftCorner,BoundingBox,LowerLeftCorner,Vector3)
    .DEC_CT_PROPERTY(upperRightCorner,BoundingBox,UpperRightCorner,Vector3)
    .def("change",&BoundingBox::change,"change(center)")
    .def("getCenter",&BoundingBox::getCenter)
    .def("getSize",&BoundingBox::getSize,
     "size of the half diagonal of self along the x-axis, y-axis and z-axis. ")
    .def("getXRange",&BoundingBox::getXRange)
    .def("getYRange",&BoundingBox::getYRange)
    .def("getZRange",&BoundingBox::getZRange)
    .def("getXMin",&BoundingBox::getXMin)
    .def("getYMin",&BoundingBox::getYMin)
    .def("getZMin",&BoundingBox::getZMin)
    .def("getXMax",&BoundingBox::getXMax)
    .def("getYMax",&BoundingBox::getYMax)
    .def("getZMax",&BoundingBox::getZMax)
    .def("__str__",&bbox_str)
    .def("__repr__",&bbox_str)
    .def("transform",(void (BoundingBox::*)(const Matrix4&))&BoundingBox::transform)
    .def("transform",(void (BoundingBox::*)(const Matrix3&))&BoundingBox::transform)
    .def("translate",&BoundingBox::translate)
    .def("scale",&BoundingBox::scale)
    .def( self == self )
    .def( self != self )
    .def( self + self )
    .def( self + other< Vector3 >() )
    .def( self | self )
    .def( self | other< Vector3 >() )
    .def( self & self )
    .def("contains",&contains_bbox)
    .def("contains",&contains_v3)
    .def("intersect",&intersect_bbox)
    .def("extend",&extend_bbox)
    .def("extend",&extend_bbox_v3)
    .def("distance",&distance_to_bbox)
    .def("distance",&distance_to_bbox_v3)
    .def("getId",&RefCountObject::uid)
     ;

}



