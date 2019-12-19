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



/*! \file geom_paraboloid.h
    \brief Definition of the geometry class Paraboloid.
*/


#ifndef __geom_paraboloid_h__
#define __geom_paraboloid_h__

/* ----------------------------------------------------------------------- */

#include "cone.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class Paraboloid
   \brief A paraboloid.
*/

/* ----------------------------------------------------------------------- */

class SG_API Paraboloid : public Cone
{

public:

  /// The \b Shape field default value.
  static const real_t DEFAULT_SHAPE;

  /// The \b Stacks field default value.
  static const uchar_t DEFAULT_STACKS;


  /// A structure which helps to build a Paraboloid when parsing.
  struct SG_API Builder : public Cone::Builder {

    /// A pointer to the \b Shape field.
    real_t * Shape;

    /// A pointer to the \b Stacks field.
    uchar_t * Stacks;

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };


  /// Defautl constructor.
  Paraboloid( const real_t& radius = DEFAULT_RADIUS,
          const real_t& height = DEFAULT_HEIGHT,
          const real_t& shape =  DEFAULT_SHAPE,
          bool solid = DEFAULT_SOLID,
          uchar_t slices = DEFAULT_SLICES,
          uchar_t stacks = DEFAULT_STACKS );

  /// Destructor
  virtual ~Paraboloid( ) ;

  PGL_OBJECT(Paraboloid)

  /// Returns \b Shape value.
  const real_t& getShape( ) const ;

  /// Returns \b Shape field.
  real_t& getShape( );

  /// Returns \b Stacks value.
  const uchar_t& getStacks( ) const ;

  /// Returns \b Stacks field.
  uchar_t& getStacks( ) ;

  /// Returns whether \b Shape is set to its default value.
  bool isShapeToDefault( ) const ;

  /// Returns whether \b Stacks is set to its default value.
  bool isStacksToDefault( ) const ;

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

protected:

  /// The \b Shape field.
  real_t __shape;

  /// The \b Stacks field.
  uchar_t __stacks;

};

typedef RCPtr<Paraboloid> ParaboloidPtr;


/* ----------------------------------------------------------------------- */

// __geom_paraboloid_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

