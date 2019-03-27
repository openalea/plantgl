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


/*! \file appe_monospectral.h
    \brief Definition of the appearance class MonoSpectral.
*/

#ifndef __appe_monospectral_h__
#define __appe_monospectral_h__


#include "spectrum.h"


/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class MonoSpectral
    \brief Mono spectral material property of an object.
*/



class SG_API MonoSpectral : public Spectrum
{

public:

  /// The default \b Reflectance field value.
  static const real_t DEFAULT_REFLECTANCE;

  /// The default \b Transmittance field value.
  static const real_t DEFAULT_TRANSMITTANCE;


  /// A structure which helps to build a MonoSpectral when parsing.
  struct SG_API Builder : public Spectrum::Builder {

    /// A pointer to the \b Reflectance field.
    real_t * Reflectance;

    /// A pointer to the \b Transmittance field.
    real_t * Transmittance;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };


  /// Default constructor.
  MonoSpectral( const real_t& reflectance = DEFAULT_REFLECTANCE,
        const real_t& transmittance = DEFAULT_TRANSMITTANCE);

  /// Destructor.
  virtual ~MonoSpectral( ) ;

  PGL_OBJECT(MonoSpectral)

  /// Returns \b Reflectance value.
  const real_t& getReflectance( ) const ;

  /// Returns \b Reflectance field.
  real_t& getReflectance( ) ;

  /// Returns \b Transmittance value.
  const real_t& getTransmittance( ) const ;

  /// Returns \b Transmittance field.
  real_t& getTransmittance( );

  /// Returns whether \b Reflectance field is set to its default value.
  bool isReflectanceToDefault( ) const ;

  /// Returns whether \b Transmittance field is set to its default value.
  bool isTransmittanceToDefault( ) const ;

  /// Returns whether \e self id valid.
  virtual bool isValid( ) const;

protected:

  /// The \b Reflectance field.
  real_t __reflectance;

  /// The \b Transmittance field.
  real_t __transmittance;

};

/// MonoSpectral Pointer
typedef RCPtr<MonoSpectral> MonoSpectralPtr;


/* ----------------------------------------------------------------------- */

// __appe_monospectral_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

