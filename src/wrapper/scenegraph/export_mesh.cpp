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




#include <plantgl/scenegraph/geometry/mesh.h>

#include <plantgl/python/export_property.h>
#include <plantgl/python/export_refcountptr.h>
#include "export_sceneobject.h"

using namespace boost::python;

PGL_USING_NAMESPACE

DEF_POINTEE( Mesh )

void export_Mesh()
{
  class_<Mesh, MeshPtr, bases<ExplicitModel>, boost::noncopyable>( "Mesh", "Abstract base class for objects of type of mesh.", no_init )
      .def("indexListSize",&Mesh::getIndexListSize)
      .DEC_BT_NR_PROPERTY_WDV(solid,            Mesh, Solid,           bool,  DEFAULT_SOLID)
      .DEC_BT_NR_PROPERTY_WDV(ccw,              Mesh, CCW,             bool,  DEFAULT_CCW)
      .DEC_BT_NR_PROPERTY_WDV(normalPerVertex,  Mesh, NormalPerVertex, bool,  DEFAULT_NORMALPERVERTEX)
      .DEC_BT_NR_PROPERTY_WDV(colorPerVertex,   Mesh, ColorPerVertex,  bool,  DEFAULT_COLORPERVERTEX)
      .DEC_PTR_PROPERTY_WDV(skeleton,        Mesh, Skeleton,        PolylinePtr, DEFAULT_SKELETON)
      .DEC_PTR_PROPERTY_WD(normalList,      Mesh, NormalList,      Point3ArrayPtr)
      .DEC_PTR_PROPERTY_WD(texCoordList,    Mesh, TexCoordList,    Point2ArrayPtr)
      .def("computeNormalList",  (void (Mesh::*)())&Mesh::computeNormalList)
      .def("estimateNormalPerVertex",  (void (Mesh::*)())&Mesh::computeNormalPerVertex)
      .def("estimateNormalPerFace",  (void (Mesh::*)())&Mesh::computeNormalPerFace)
      .def( "pointAt",    (const Vector3& (Mesh::*)(uint_t) const)&Mesh::getPointAt,    return_value_policy<copy_const_reference>() )
      .def( "pointAt",    (const Vector3& (Mesh::*)(uint_t,uint_t) const)&Mesh::getFacePointAt,    return_value_policy<copy_const_reference>() )
      .def( "faceCenter",    &Mesh::getFaceCenter )
      .def( "faceSize",    &Mesh::getFaceSize )

      ;
  implicitly_convertible<MeshPtr, ExplicitModelPtr>();

}
