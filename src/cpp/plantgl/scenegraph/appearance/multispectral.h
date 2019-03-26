/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP
 *
 *       File author(s): F. Boudon et al.
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


/*! \file appe_multispectral.h
    \brief Definition of the appearance class MultiSpectral.
*/

#ifndef __appe_multispectral_h__
#define __appe_multispectral_h__


/* ----------------------------------------------------------------------- */

#include "spectrum.h"
#include <plantgl/tool/util_tuple.h>
#ifndef GEOM_FWDEF
#include <plantgl/tool/util_array.h>
#endif

/* ----------------------------------------------------------------------- */


#ifdef GEOM_FWDEF
PGL_BEGIN_NAMESPACE
class RealArray;
typedef RCPtr<RealArray> RealArrayPtr;
PGL_END_NAMESPACE
#endif


/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

typedef Uint32Tuple3 Index3;

/* ----------------------------------------------------------------------- */

/**
    \class MultiSpectral
    \brief The multi spectral appereance property of an object.
*/


class SG_API MultiSpectral : public Spectrum
{

public:

  /// The default \b Filter field value.
  static const Index3 DEFAULT_RGB_INDEX;


  /// A structure which helps to build a MultiSpectral when parsing.
  struct SG_API Builder : public Spectrum::Builder {

    /// A pointer to the \b Reflectance field.
    RealArrayPtr * Reflectance;

    /// A pointer to the \b Transmittance field.
    RealArrayPtr * Transmittance;

    /// A pointer to the \b Filter field.
    Index3 * Filter;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  MultiSpectral();

  /// Constructor.
  MultiSpectral( const RealArrayPtr& reflectance,
         const RealArrayPtr& transmittance,
         const Index3& rgbIndex = DEFAULT_RGB_INDEX );

  /// Destructor.
  virtual ~MultiSpectral( ) ;

  PGL_OBJECT(MultiSpectral)

  /// Returns \b Reflectance values.
  const RealArrayPtr& getReflectance( ) const ;

  /// Returns \b Reflectance field.
  RealArrayPtr& getReflectance( );

  /** Returns the \e i-th value of \b Reflectance.
      \pre
      - \e i must belong to the range [0,size(\b Reflectance)[. */
  const real_t& getReflectanceAt( uint_t i ) const;

  /** Returns the \e i-th field of \b Reflectance.
      \pre
      - \e i must belong to the range [0,size(\b Reflectance)[. */
  real_t& getReflectanceAt( uint_t i ) ;

  /// Returns a const iterator to the beginning of \b Reflectance.
/*  RealArray::const_iterator getReflectanceBegin( ) const {
    return __reflectance->begin();
  }

  /// Returns a const iterator to the beginning of \b Reflectance.
  RealArray::const_iterator getReflectanceEnd( ) const {
    return __reflectance->end();
  }
*/

  /// Returns the size of \b Reflectance.
  uint_t getReflectanceSize( ) const ;

  /// Returns \b Transmittance values.
  const RealArrayPtr& getTransmittance( ) const ;

  /// Returns \b Transmittance field.
  RealArrayPtr& getTransmittance( );

  /** Returns the \e i-th value of \b Transmittance.
      \pre
      - \e i must belong to the range [0,size(\b Transmittance)[. */
  const real_t& getTransmittanceAt( uint_t i ) const;

  /** Returns the \e i-th field of \b Transmittance.
      \pre
      - \e i must belong to the range [0,size(\b Transmittance)[. */
  real_t& getTransmittanceAt( uint_t i );

  /// Returns a const iterator to the beginning of \b Transmittance.
/*  RealArray::const_iterator getTransmittanceBegin( ) const {
    return __transmittance->begin();
  }

  /// Returns a const iterator to the beginning of \b Transmittance.
  RealArray::const_iterator getTransmittanceEnd( ) const {
    return __transmittance->end();
  }*/

  /// Returns the size of \b Transmittance.
  uint_t getTransmittanceSize( ) const;

  /// Returns \b Filter value.
  const Index3& getFilter( ) const;

  /// Returns \b Filter field.
  Index3& getFilter( ) ;

  /// Returns whether \b Filter is set to its default value.
  bool isFilterToDefault( ) const;

  /// Returns whether \e self id valid.
  virtual bool isValid( ) const;

protected:

  /// The \b Reflectance field.
  RealArrayPtr __reflectance;

  /// The \b Transmittance field.
  RealArrayPtr __transmittance;

  /// The \b rgbIndex field.
  Index3 __filter;

};

/// MultiSpectral Pointer
typedef RCPtr<MultiSpectral> MultiSpectralPtr;


/* ----------------------------------------------------------------------- */

// __appe_multispectral_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

