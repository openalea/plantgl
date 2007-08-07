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

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/geometry/sor.h>
#include <plantgl/scenegraph/geometry/swung.h>
#include <plantgl/scenegraph/geometry/curve.h>
#include <plantgl/scenegraph/geometry/profile.h>
#include <plantgl/scenegraph/container/geometryarray2.h>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"
#include "../util/extract_list.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(Swung)

SwungPtr make_swung( boost::python::list profiles, boost::python::list angles, 
		     uchar_t slices, bool ccw, uint32_t degree, uint32_t stride ) 
{ 
  Curve2DArrayPtr profilearray= extract_pgllist<Curve2DArray>(profiles)();
  RealArrayPtr anglesarray= extract_pgllist<RealArray>(angles)();
  return new Swung(profilearray, anglesarray, slices, ccw, degree, stride);
}

SwungPtr make_swung5( boost::python::list profiles, boost::python::list angles, 
		     uchar_t slices, bool ccw, uint32_t degree) 
{ 
	return make_swung(profiles,angles,slices,ccw,degree,Swung::DEFAULT_STRIDE);
}

SwungPtr make_swung4( boost::python::list profiles, boost::python::list angles, 
		     uchar_t slices, bool ccw) 
{ 
	return make_swung(profiles,angles,slices,ccw,Swung::DEFAULT_DEGREE,Swung::DEFAULT_STRIDE);
}

SwungPtr make_swung3( boost::python::list profiles, 
					  boost::python::list angles, 
					  uchar_t slices) 
{ 
	return make_swung(profiles,angles,slices,Swung::DEFAULT_CCW,Swung::DEFAULT_DEGREE,Swung::DEFAULT_STRIDE);
}

SwungPtr make_swung2( boost::python::list profiles, 
					  boost::python::list angles) 
{ 
	return make_swung(profiles,angles,Swung::DEFAULT_SLICES,Swung::DEFAULT_CCW,Swung::DEFAULT_DEGREE,Swung::DEFAULT_STRIDE);
}


void export_Swung()
{
  
  class_< Swung, SwungPtr, bases< SOR >,boost::noncopyable >
    ("Swung",no_init)
    .def( "__init__", make_constructor( make_swung , default_call_policies(), 
                                        args("profiles","angles","slices","ccw","degree","stride")),
                     (const char *)"Swung([Curve2D] profiles,list angles [,slices,ccw,degree,stride])" ) 
    .def( "__init__", make_constructor( make_swung5, default_call_policies(), 
                                        args("profiles","angles","slices","ccw","degree") )) 
    .def( "__init__", make_constructor( make_swung4, default_call_policies(), 
                                        args("profiles","angles","slices","ccw") ) ) 
    .def( "__init__", make_constructor( make_swung3, default_call_policies(), 
                                        args("profiles","angles","slices") ) ) 
    .def( "__init__", make_constructor( make_swung2, default_call_policies(), 
                                        args("profiles","angles") )) 
	.DEC_BT_NR_PROPERTY_WD(ccw,      Swung,CCW,             bool)
	.DEC_BT_PROPERTY_WD(degree,   Swung,Degree,          uint32_t)
	.DEC_BT_PROPERTY_WD(stride,   Swung,Stride,          uint32_t)
	.DEC_PTR_PROPERTY(angleList,   Swung,AngleList,       RealArrayPtr)
	.DEC_PTR_PROPERTY(profileList, Swung,ProfileList,     Curve2DArrayPtr)
    ;

  implicitly_convertible< SwungPtr, SORPtr >();
}
