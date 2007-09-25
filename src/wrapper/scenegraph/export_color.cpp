/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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

#include <plantgl/scenegraph/appearance/color.h>
#include "../util/tuple_converter.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

#define EXPORT(PREFIX,TYPE,CHANNEL) \
int PREFIX##_get##CHANNEL( TYPE* v ) \
{ \
  return (int)v->get##CHANNEL(); \
} \
\
void PREFIX##_set##CHANNEL( TYPE* v, int val ) \
{ \
  if (val >= 0 && val < 256) \
	v->get##CHANNEL() = (uchar_t)val; \
} \
\
real_t PREFIX##_get##CHANNEL##Clamped( TYPE* v ) \
{ \
  return v->get##CHANNEL##Clamped(); \
} \
\
void PREFIX##_set##CHANNEL##Clamped( TYPE* v, real_t val ) \
{ \
  if (val >= 0.0 && val <= 1.0) \
	v->get##CHANNEL() = (uchar_t)(val*255); \
} \

std::string col3_str( Color3* v ) 
{ 
  stringstream ss; 
  ss << "Color3(" 
	 <<(int)v->getRed() << ',' 
	 <<(int)v->getGreen() << ','  
	 <<(int)v->getBlue() << ')' ; 
  return ss.str(); 
} 

std::string col4_str( Color4* v ) 
{ 
  stringstream ss; 
  ss << "Color4(" 
	 <<(int)v->getRed() << ',' 
	 <<(int)v->getGreen() << ','  
	 <<(int)v->getBlue() << ','  
	 <<(int)v->getAlpha() << ')' ; 
  return ss.str(); 
} 

EXPORT(col3,Color3,Red)
EXPORT(col3,Color3,Green)
EXPORT(col3,Color3,Blue)

EXPORT(col4,Color4,Red)
EXPORT(col4,Color4,Green)
EXPORT(col4,Color4,Blue)
EXPORT(col4,Color4,Alpha)

struct col3_pickle_suite : boost::python::pickle_suite
{
	static tuple getinitargs(Color3 const& c)
	{
		return make_tuple(c.getRed(),c.getGreen(),c.getBlue());
	}
};

struct col4_pickle_suite : boost::python::pickle_suite
{
	static tuple getinitargs(Color4 const& c)
	{
		return make_tuple(c.getRed(),c.getGreen(),c.getBlue(),c.getAlpha());
	}
};

void export_Color3()
{
  class_< Color3 >("Color3", init< const Color3 & >())
    .def(init< uchar_t, uchar_t, uchar_t >("Color3(red,green,blue)"))
    .def(init< optional< unsigned char > >())
    .def(init< const unsigned char * >())
    .def( self == self )
    .def( self != self )
    .add_property( "red", col3_getRed, col3_setRed )
    .add_property( "green", col3_getGreen, col3_setGreen  )
    .add_property( "blue", col3_getBlue, col3_setBlue  )
    .def( "clampedRed", col3_getRedClamped)
	.def( "setClampedRed", col3_setRedClamped )
    .def( "clampedGreen", col3_getGreenClamped)
	.def( "setClampedGreen", col3_setGreenClamped )
    .def( "clampedBlue", col3_getBlueClamped)
	.def( "setClampedBlue", col3_setBlueClamped )
    .def( "__str__", col3_str )
    .def( "__repr__", col3_str )
	.def_pickle(col3_pickle_suite());
    ;

	pgltuple_from_tuple<Color3,3>();
}

void export_Color4()
{
  class_< Color4 >("Color4", init< const Color4 & >())
    .def(init< uchar_t, uchar_t, uchar_t , uchar_t>("Color4(red,green,blue,alpha)"))
    .def(init< optional< unsigned char > >())
    .def(init< const unsigned char * >())
    .def( self == self )
    .def( self != self )
    .add_property( "red", col4_getRed, col4_setRed )
    .add_property( "green", col4_getGreen, col4_setGreen  )
    .add_property( "blue", col4_getBlue, col4_setBlue  )
    .add_property( "alpha", col4_getAlpha, col4_setAlpha  )
    .def( "clampedRed", col4_getRedClamped)
	.def( "setClampedRed", col4_setRedClamped )
    .def( "clampedGreen", col4_getGreenClamped)
	.def( "setClampedGreen", col4_setGreenClamped )
    .def( "clampedBlue", col4_getBlueClamped)
	.def( "setClampedBlue", col4_setBlueClamped )
    .def( "clampedAlpha", col4_getAlphaClamped)
	.def( "setClampedAlpha", col4_setAlphaClamped )
    .def( "__str__", col4_str )
    .def( "__repr__", col4_str )
	.def_pickle(col4_pickle_suite());
    ;
	pgltuple_from_tuple<Color4,4>();
}
