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

#include "swung.h"

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

#include <scne_sceneobject.h>
#include <geom_geometry.h>

#include <util_vector.h>

#include <geom_sor.h>
#include <geom_swung.h>
#include <geom_curve.h>
#include <geom_profile.h>
#include <geom_geometryarray2.h>

#include "macro_refptr.h"

GEOM_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(Swung)

SwungPtr make_swung( object l, RealArrayPtr a, 
		     uchar_t slices, bool ccw, uint32_t degree, uint32_t stride ) 
{ 
  Curve2DArray * array = new Curve2DArray();
  try { 
      object iter_obj = object( handle<>( PyObject_GetIter( l.ptr() ) ) );
      while( 1 )
        {
          object obj = iter_obj.attr( "next" )();
          Curve2DPtr val = extract< Curve2DPtr >( obj );
          array->pushBack( val );
        }
    }
  catch( error_already_set )
    {
      PyErr_Clear();
    }
  return new Swung(array, a, slices, ccw, degree, stride);
}

void class_Swung()
{
  
  class_< Swung, SwungPtr, bases< SOR >,boost::noncopyable >
    ("Swung",no_init)
    .def( "__init__", make_constructor( make_swung ),
	  ( const char* )"Swung([Curve2D],RealArray(),slices,ccw,degree,stride)"  ) 
    ;

  implicitly_convertible< SwungPtr, SORPtr >();
}

