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


/*! \file geom_cone.h
    \brief Definition of the geometry class Cone.
*/


#ifndef __geom_cone_h__
#define __geom_cone_h__

/* ----------------------------------------------------------------------- */

#include "sor.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class Cone
   \brief A cone.
*/

class SG_API Cone : public SOR
{

public:

  /// The \b Radius field default value.
  static const real_t DEFAULT_RADIUS;

  /// The \b Height field default value.
  static const real_t DEFAULT_HEIGHT;

  /// The \b Solid field default value.
  static const bool DEFAULT_SOLID;


  /// A structure which helps to build a Cone when parsing.
  struct SG_API Builder : public SOR::Builder {

    /// A pointer to the \b Height field.
    real_t * Height;

    /// A pointer to the \b Solid field.
    bool * Solid;

    /// A pointer to the \b Radius field.
    real_t * Radius;

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

    void ConeDestroy( );

    bool ConeValid( ) const;

  };


  /// Defautl constructor.
  Cone( const real_t& radius = DEFAULT_RADIUS,
    const real_t& height = DEFAULT_HEIGHT,
    bool solid = DEFAULT_SOLID,
    uchar_t slices = DEFAULT_SLICES );

  /// Destructor
  virtual ~Cone( ) ;

  PGL_OBJECT(Cone)

  /// Returns \b solid value.
  const bool getSolid( ) const;

  /// Returns \b solid field.
  bool& getSolid( );

  /// Returns \b Height value.
  const real_t& getHeight( ) const;

  /// Returns \b Height field.
  real_t& getHeight( );

  /// Returns \b Radius value.
  const real_t& getRadius( ) const ;

  /// Returns \b Radius field.
  real_t& getRadius( ) ;

  virtual bool isAVolume( ) const ;

  /// Returns whether \b Height is set to its default value.
  bool isHeightToDefault( ) const;

  /// Returns whether \b Solid is set to its default value.
  bool isSolidToDefault( ) const;

  /// Returns whether \b Radius is set to its default value.
  bool isRadiusToDefault( ) const ;

  virtual bool isValid( ) const;

protected:

  /// The \b Height field.
  real_t __height;

  /// The \b Solid field.
  bool __solid;

  /// The Radius field.
  real_t __radius;
};

/// Cone Pointer
typedef RCPtr<Cone> ConePtr;


/* ----------------------------------------------------------------------- */

// __geome_cone_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

