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

#include <math/util_vector.h>

#include <scenegraph/geometry/primitive.h>
#include <scenegraph/geometry/parametricmodel.h>
#include <scenegraph/geometry/box.h>


#include "../util/export_refcountptr.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(Box)
DEF_POINTEE(Primitive)
DEF_POINTEE(ParametricModel)
DEF_POINTEE(Geometry)

void export_Primitive()
{
  
  class_< Primitive, PrimitivePtr, bases< Geometry >,boost::noncopyable >("Primitive",no_init);
  class_< ParametricModel, ParametricModelPtr, bases< Primitive >,boost::noncopyable >("ParametricModel",no_init);

  implicitly_convertible< PrimitivePtr, GeometryPtr >();
  implicitly_convertible< ParametricModelPtr,PrimitivePtr >();
}

void export_Box()
{
  class_< Box, BoxPtr, bases< ParametricModel > , boost::noncopyable >
    ("Box", init< const Vector3& >("Box(Vector3(x,y,z))") );

  implicitly_convertible<BoxPtr, ParametricModelPtr >();
}


