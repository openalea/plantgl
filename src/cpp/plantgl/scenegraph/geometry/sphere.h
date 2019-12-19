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



/*! \file geom_sphere.h
    \brief Definition of the geometry class Sphere.
*/


#ifndef __geom_sphere_h__
#define __geom_sphere_h__

/* ----------------------------------------------------------------------- */

#include "sor.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class Sphere
   \brief A sphere.
*/

/* ----------------------------------------------------------------------- */

class SG_API Sphere : public SOR
{

public:

  /// The \b Stacks field default value.
  static const uchar_t DEFAULT_STACKS;

  /// The \b Radius field default value.
  static const real_t DEFAULT_RADIUS;

  /// A structure which helps to build a Sphere when parsing.
  struct SG_API Builder : public SOR::Builder {

    /// A pointer to the Stacks field.
    uchar_t * Stacks;

    /// A pointer to the \b Radius field.
    real_t * Radius;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };


  /// Default constructor.
  Sphere( const real_t& radius = DEFAULT_RADIUS,
      uchar_t Slices = DEFAULT_SLICES,
      uchar_t Stacks = DEFAULT_STACKS);

  /// Destructor
  virtual ~Sphere( );

  PGL_OBJECT(Sphere)

  /// Returns \b Stacks value.
  const uchar_t& getStacks( ) const;

  /// Returns \b Stacks field.
  uchar_t& getStacks( );

  /// Returns \b Radius value.
  const real_t& getRadius( ) const;

  /// Returns \b Radius field.
  real_t& getRadius( );

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const;

  /// Returns whether \b Stacks is set to its default value.
  bool isStacksToDefault( ) const;

  /// Returns whether \b Radius is set to its default value.
  bool isRadiusToDefault( ) const;

  virtual bool isValid( ) const;

protected:

  /// The \b Stackes field.
  uchar_t __stacks;


  /// The Radius field.
  real_t __radius;
};

/// Sphere Pointer
typedef RCPtr<Sphere> SpherePtr;


/* ----------------------------------------------------------------------- */

// __geom_sphere_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

