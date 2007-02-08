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

#include "../util/export_refcountptr.h"

#include <scenegraph/appearance/color.h>
#include <scenegraph/appearance/appearance.h>
#include <scenegraph/appearance/material.h>
#include <scenegraph/appearance/spectrum.h>
#include <scenegraph/appearance/monospectral.h>
#include <scenegraph/appearance/multispectral.h>
#include <tool/util_tuple.h>
#include <tool/util_array.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(Appearance)
DEF_POINTEE(Spectrum)
DEF_POINTEE(MonoSpectral)
DEF_POINTEE(MultiSpectral)

void class_PureAppearance()
{
  class_< Appearance, AppearancePtr, bases< SceneObject >,  boost::noncopyable >
    ("Appearance", no_init);
}

void class_Spectrum()
{
  class_< Spectrum, SpectrumPtr, bases< Appearance >,  boost::noncopyable >
    ("Spectrum", no_init);

   implicitly_convertible<SpectrumPtr, AppearancePtr >();
}

void ms_rset(MonoSpectral*s, real_t re)
{
  s->getReflectance()= re;
}

real_t ms_rget(MonoSpectral*s)
{
  return s->getReflectance();
}
void ms_tset(MonoSpectral*s, real_t re)
{
  s->getTransmittance()= re;
}

real_t ms_tget(MonoSpectral*s)
{
  return s->getTransmittance();
}

void class_MonoSpectral()
{
  class_< MonoSpectral, MonoSpectralPtr, bases< Spectrum >,  boost::noncopyable >
    ("MonoSpectral", init< optional< real_t, real_t > >("MonoSpectral([ reflectance, transmittance ])"))
    .add_property("transmittance",ms_tget, ms_tset)
    .add_property("reflectance",ms_rget, ms_rset)
    ;

  implicitly_convertible< MonoSpectralPtr, SpectrumPtr >();
}
void class_MultiSpectral()
{
  class_< MultiSpectral, MultiSpectralPtr, bases< Spectrum >,  boost::noncopyable >
    ("MultiSpectral", init< RealArrayPtr, RealArrayPtr, optional < const Index3& > >
     ("MultiSpectral(reflectance= RealArray, transmittance= RealArray [, filter= index3])"));

  implicitly_convertible< MultiSpectralPtr, SpectrumPtr >();
}
