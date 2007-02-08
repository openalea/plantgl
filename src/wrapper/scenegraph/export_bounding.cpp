/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       GeomPy: Python wrapper for the Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR AMAP 
 *
 *       File author(s): C. Pradal (christophe.pradal@cirad.fr)
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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

#include "bounding.h"
#include "bboxcomputer.h"

#include "Tools/util_matrix.h"
#include "geom_boundingbox.h"

#include "actn_discretizer.h"
#include "actn_bboxcomputer.h"
#include "geom_geometry.h"
#include "scne_scene.h"

#include <string>

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

#include "macro_refptr.h"
#include "macro_property.h"

GEOM_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(BoundingBox)

SETGET(BoundingBox,LowerLeftCorner,Vector3)
SETGET(BoundingBox,UpperRightCorner,Vector3)

std::string bbox_str(BoundingBox * bbox){
  std::stringstream ss;
  const Vector3& v = bbox->getLowerLeftCorner();
  const Vector3& w = bbox->getUpperRightCorner();
  ss << "BoundingBox(Vector3(" << v.x() << "," << v.y() << "," << v.z() << "),Vector3(" << w.x() << "," << w.y() << "," << w.z() << "))";
  return ss.str();
}

bool contains_bbox(BoundingBox * bbox, BoundingBox * bbox2){
	return intersection(*bbox,*bbox2);
}

bool contains_v3(BoundingBox * bbox, Vector3 v){
	return intersection(*bbox,v);
}

BoundingBoxPtr  bbx_fromobj( boost::python::object o ) 
{ 
	extract<Scene> e(o);
	if(!e.check()){
		Discretizer d;
		BBoxComputer bbc(d);
		SceneObject * geom = extract<SceneObject *>(o)();
		geom->apply(bbc);
//		boost::python::call_method<bool>(o.ptr(),"apply", bbc );
		return bbc.getBoundingBox();
	}
	else {
		Discretizer d;
		BBoxComputer bbc(d);
		bbc.process(e());
		return bbc.getBoundingBox();
	}
}


void class_BoundingBox()
{
  class_< BoundingBox, BoundingBoxPtr /*, boost::noncopyable*/ >
    ("BoundingBox", init< const Vector3&, optional < const Vector3& > > 
     ( args("lowerLeft","upperRight"),
	 "BoundingBox(Vector3 lowerLeft, Vector3 upperRight)" 
       "Constructs a BoundingBox with the 2 opposing corners lowerLeft and upperRight.") )
    .def( "__init__", make_constructor( bbx_fromobj ) ) 
	.DEC_SETGET(lowerLeftCorner,BoundingBox,LowerLeftCorner,Vector3)
	.DEC_SETGET(upperRightCorner,BoundingBox,UpperRightCorner,Vector3)
    .def("set",&BoundingBox::set,"set(lowerLeft,upperRight)")
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
    .def( self + self )
    .def( self + other< Vector3 >() )
    .def( self | self )
    .def( self | other< Vector3 >() )
    .def( self & self )
	.def("contains",&contains_bbox)	 
	.def("contains",&contains_v3)	 
	 ;

}

void module_bounding()
{
class_BoundingBox();
class_BBoxComputer();
}



