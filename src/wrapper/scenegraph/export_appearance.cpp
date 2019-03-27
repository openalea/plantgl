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




#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include "export_sceneobject.h"


#include <plantgl/scenegraph/appearance/color.h>
#include <plantgl/scenegraph/appearance/appearance.h>
#include <plantgl/scenegraph/appearance/spectrum.h>
#include <plantgl/scenegraph/appearance/monospectral.h>
#include <plantgl/scenegraph/appearance/multispectral.h>
#include <plantgl/tool/util_tuple.h>
#include <plantgl/tool/util_array.h>

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(Appearance)
DEF_POINTEE(Spectrum)
DEF_POINTEE(MonoSpectral)
DEF_POINTEE(MultiSpectral)

void export_Appearance()
{
  class_< Appearance, AppearancePtr, bases< SceneObject >,  boost::noncopyable >
    ("Appearance", "Abstract base class for all appearance objects.", no_init)
    .def("isTexture",&Appearance::isTexture,"Tell wether self is of type texture");
}

void export_Spectrum()
{
  class_< Spectrum, SpectrumPtr, bases< Appearance >,  boost::noncopyable >
    ("Spectrum", "Abstract base class for all spectrum objects.", no_init);

   implicitly_convertible<SpectrumPtr, AppearancePtr >();
}


void export_MonoSpectral()
{
  class_< MonoSpectral, MonoSpectralPtr, bases< Spectrum >,  boost::noncopyable >
    ("MonoSpectral", "Mono spectral appearance property of an object.", init< optional< real_t, real_t > >("MonoSpectral([ reflectance, transmittance ])"))
    .DEC_BT_PROPERTY(transmittance,MonoSpectral, Transmittance, real_t)
    .DEC_BT_PROPERTY(reflectance,MonoSpectral, Reflectance, real_t)
    .DEF_PGLBASE(MonoSpectral)
    ;

  implicitly_convertible< MonoSpectralPtr, SpectrumPtr >();
}
void export_MultiSpectral()
{
  class_< MultiSpectral, MultiSpectralPtr, bases< Spectrum >,  boost::noncopyable >
    ("MultiSpectral", "Multi spectral appearance property of an object.", init< RealArrayPtr, RealArrayPtr, optional < const Index3& > >
     ("MultiSpectral(reflectance= RealArray, transmittance= RealArray [, filter= index3])"))
    .DEF_PGLBASE(MultiSpectral)
     ;

  implicitly_convertible< MultiSpectralPtr, SpectrumPtr >();
}
