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

#include <plantgl/scenegraph/geometry/sor.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(SOR)

void export_SOR()
{
  class_< SOR, SORPtr, bases< ParametricModel >,boost::noncopyable >
	("SOR","Abstract base class for 3D objects of type of surface of revolution.",no_init)
  .DEC_BT_PROPERTY_WDV(slices,SOR,Slices,uchar_t,DEFAULT_SLICES);
  implicitly_convertible<SORPtr,ParametricModelPtr >();
}


void export_SOR2D()
{
  class_<SOR2D,SOR2DPtr, bases<PlanarModel>, boost::noncopyable>
   ( "SOR2D", "Abstract base class for 2D objects of type of surface of revolution.", no_init )
   .DEC_BT_PROPERTY_WD(slices,SOR2D,Slices,uchar_t);
    ;

  implicitly_convertible<SOR2DPtr, PlanarModelPtr>();

}