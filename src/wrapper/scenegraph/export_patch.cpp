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

#include <tool/util_array.h>
#include <tool/util_array2.h>

#include <scenegraph/geometry/parametricmodel.h>
#include <scenegraph/container/pointmatrix.h>
#include <scenegraph/geometry/patch.h>
#include <scenegraph/geometry/bezierpatch.h>
#include <scenegraph/geometry/nurbspatch.h>

#include "../util/export_refcountptr.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(Patch)
DEF_POINTEE(BezierPatch)
DEF_POINTEE(NurbsPatch)

void export_Patch()
{
  class_< Patch, PatchPtr, bases< ParametricModel >,boost::noncopyable >
    ("Patch",no_init);

  implicitly_convertible< PatchPtr,ParametricModelPtr >();
}

void export_BezierPatch()
{
  class_< BezierPatch, BezierPatchPtr, bases< Patch >,boost::noncopyable >
    ("BezierPatch",init<Point4MatrixPtr, optional<uint32_t,uint32_t,bool> >
     ("BezierPatch(Point4Matrix ctrlPoints [,ustride,vstride,ccw])"));

  implicitly_convertible< BezierPatchPtr,PatchPtr >();
}

void export_NurbsPatch()
{
  class_< NurbsPatch, NurbsPatchPtr, bases< BezierPatch >,boost::noncopyable >
    ("NurbsPatch",init<Point4MatrixPtr, RealArrayPtr, RealArrayPtr, 
     optional<uint32_t,uint32_t,uint32_t,uint32_t,bool> >
     ("NurbsPatch(Point4Matrix ctrlPoints, RealArray uKnot,RealArray vKnot [,uDeg, vDeg,ustride,vstride,ccw])"));

  implicitly_convertible< NurbsPatchPtr,BezierPatchPtr >();

}


