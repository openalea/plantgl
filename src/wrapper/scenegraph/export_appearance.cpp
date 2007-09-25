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

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"

#include <plantgl/scenegraph/appearance/color.h>
#include <plantgl/scenegraph/appearance/appearance.h>
#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/appearance/spectrum.h>
#include <plantgl/scenegraph/appearance/monospectral.h>
#include <plantgl/scenegraph/appearance/multispectral.h>
#include <plantgl/tool/util_tuple.h>
#include <plantgl/tool/util_array.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(Appearance)
DEF_POINTEE(Spectrum)
DEF_POINTEE(MonoSpectral)
DEF_POINTEE(MultiSpectral)

void export_Appearance()
{
  class_< Appearance, AppearancePtr, bases< SceneObject >,  boost::noncopyable >
    ("Appearance", no_init);
}

void export_Spectrum()
{
  class_< Spectrum, SpectrumPtr, bases< Appearance >,  boost::noncopyable >
    ("Spectrum", no_init);

   implicitly_convertible<SpectrumPtr, AppearancePtr >();
}


void export_MonoSpectral()
{
  class_< MonoSpectral, MonoSpectralPtr, bases< Spectrum >,  boost::noncopyable >
    ("MonoSpectral", init< optional< real_t, real_t > >("MonoSpectral([ reflectance, transmittance ])"))
    .DEC_BT_PROPERTY(transmittance,MonoSpectral, Transmittance, real_t)
    .DEC_BT_PROPERTY(reflectance,MonoSpectral, Reflectance, real_t)
    ;

  implicitly_convertible< MonoSpectralPtr, SpectrumPtr >();
}
void export_MultiSpectral()
{
  class_< MultiSpectral, MultiSpectralPtr, bases< Spectrum >,  boost::noncopyable >
    ("MultiSpectral", init< RealArrayPtr, RealArrayPtr, optional < const Index3& > >
     ("MultiSpectral(reflectance= RealArray, transmittance= RealArray [, filter= index3])"));

  implicitly_convertible< MultiSpectralPtr, SpectrumPtr >();
}
