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

