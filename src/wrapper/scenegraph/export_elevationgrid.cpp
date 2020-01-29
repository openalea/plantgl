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



#include <plantgl/scenegraph/geometry/elevationgrid.h>
#include <plantgl/tool/util_array2.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

using namespace boost::python;
#define bp boost::python

PGL_USING_NAMESPACE

DEF_POINTEE(ElevationGrid)

real_t eg_getHeightAt(ElevationGrid *  e, uint_t i, uint_t j)
{ return e->getHeightAt(i,j); }


void export_ElevationGrid()
{
  class_< ElevationGrid, ElevationGridPtr, bases< Patch >,boost::noncopyable >
    ("ElevationGrid","A regular grid of elevations.",init<RealArray2Ptr, optional<real_t,real_t,bool> >
     ("ElevationGrid(heightList [,xspacing,yspacing,ccw])",
     (bp::arg("heightList"),
     bp::arg("xspacing")=ElevationGrid::DEFAULT_X_SPACING,
      bp::arg("yspacing")=ElevationGrid::DEFAULT_Y_SPACING,
      bp::arg("ccw")=ElevationGrid::DEFAULT_CCW)))
    .def( "getPointAt", &ElevationGrid::getPointAt )
    .def( "getHeightAt", &eg_getHeightAt )
    .def( "getXDim", &ElevationGrid::getXDim )
    .def( "getYDim", &ElevationGrid::getYDim )
    .def( "getXSize", &ElevationGrid::getXSize )
    .def( "getYSize", &ElevationGrid::getYSize )
    .DEC_BT_PROPERTY_WDV(xspacing,ElevationGrid,XSpacing,real_t,DEFAULT_X_SPACING)
    .DEC_BT_PROPERTY_WDV(yspacing,ElevationGrid,YSpacing,real_t,DEFAULT_Y_SPACING)
    .DEC_PTR_PROPERTY(heightList,ElevationGrid,HeightList,RealArray2Ptr)
    .DEF_PGLBASE(ElevationGrid)
    ;

  implicitly_convertible< ElevationGridPtr,PatchPtr >();
}
