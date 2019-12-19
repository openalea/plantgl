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



#include <plantgl/scenegraph/transformation/ifs.h>
#include <plantgl/scenegraph/transformation/mattransformed.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"
#include <boost/python/make_constructor.hpp>

PGL_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python

DEF_POINTEE(IFS)


Matrix4ArrayPtr ifs_getAllTransformations(IFS * ifs)
{
  ITPtr transfos = dynamic_pointer_cast<IT>( ifs->getTransformation() );
  GEOM_ASSERT(transfos);
  const Matrix4ArrayPtr& matrixList= transfos->getAllTransfo();
  GEOM_ASSERT(matrixList);
  return matrixList;
}

void export_IFS()
{
  class_< IFS, IFSPtr, bases< Transformed > , boost::noncopyable >
    ("IFS",
     "The IFS is a recursive transformation applied to an object. Transformation are specified as a set of affine transformations.\n"
     "It is a method for generating fractals and the complexity is exponential with regard to numbers of iterations.",
    init< uchar_t, const Transform4ArrayPtr&, const GeometryPtr& >
       ("IFS(depth, transfoList, geometry)",
       (bp::arg("depth")=IFS::DEFAULT_DEPTH,
        bp::arg("transfoList")=Transform4ArrayPtr(),
        bp::arg("geometry")=GeometryPtr())))
    .DEF_PGLBASE(IFS)
    .DEC_BT_NR_PROPERTY_WDV(depth,IFS,Depth,uchar_t,DEFAULT_DEPTH)
    .DEC_PTR_PROPERTY(transfoList,IFS,TransfoList,Transform4ArrayPtr)
    .DEC_PTR_NR_PROPERTY(geometry,IFS,Geometry,GeometryPtr)
    .def("getAllTransformations",&ifs_getAllTransformations);
    ;

  implicitly_convertible< IFSPtr, TransformedPtr >();
}

