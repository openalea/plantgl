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



#include <plantgl/scenegraph/geometry/triangleset.h>
#include "export_mesh.h"

PGL_USING_NAMESPACE

using namespace boost::python;
#define bp boost::python

DEF_POINTEE( TriangleSet )

bp::object py_circumsphere(const Vector3& a, const Vector3& b, const Vector3& c){
    std::pair<Vector3, real_t> result = circumsphere(a, b, c);
    return bp::make_tuple(result.first, result.second);
}

void export_TriangleSet()
{
    class_<TriangleSet, TriangleSetPtr, bases<Mesh>, boost::noncopyable>
    ( "TriangleSet",
      "A TriangleSet describes a surface formed by a set of connected triangles, i.e. three sided polygons.\n"
      " Triangles are specified using set of tuples of 3 indices (Index3) pointing to a list of points.",no_init)
      .def(mesh_func<TriangleSet>())
      .DEF_PGLBASE(TriangleSet)
      ;

  implicitly_convertible<TriangleSetPtr, MeshPtr>();

  def("circumsphere", &py_circumsphere);
}

