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

