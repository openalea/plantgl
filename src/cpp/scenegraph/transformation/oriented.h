/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
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


/*! \file geom_oriented.h
    \brief Definition of the transformation class BaseOrientation 
	and the transformed class Oriented.
*/


#ifndef __geom_oriented_h__
#define __geom_oriented_h__

/* ----------------------------------------------------------------------- */

#include "orthotransformed.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class BaseOrientation
    \brief An Orthonormal transformation.
*/

/* ----------------------------------------------------------------------- */

class SG_API BaseOrientation : public OrthonormalBasis3D
{

public:

  /// A structure which helps to build an BaseOrientation when parsing.
  struct Builder : public OrthonormalBasis3D::Builder
    {

    /// A pointer to the \b Primary field.
    TOOLS(Vector3) * Primary;

     /// A pointer to the \b Secondary field.
    TOOLS(Vector3) * Secondary;

    /// Constructor.
    Builder( );

    /// Desructor.
    virtual ~Builder( );

    virtual OrthonormalBasis3DPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;
    };

  /** Constructs an Orientation Base with the directions \e primary
      and \e secondary.
      \warning
      - \e primary must be normalized;
      - \e secondary must be normalized and orthogonal to \e primary;
  */
  BaseOrientation( const TOOLS(Vector3)& primary,
                   const TOOLS(Vector3)& secondary ) ;

  /// Destructor.
  virtual ~BaseOrientation( );

protected:

  /// The \b Primary field.
  TOOLS(Vector3) __primary;

  /// The \b Secondary field.
  TOOLS(Vector3) __secondary;
};

/// BaseOrientation Pointer
typedef RCPtr<BaseOrientation> BaseOrientationPtr;

/* ----------------------------------------------------------------------- */


/**
    \class Oriented
    \brief A geometry object transformed by change of coordinates using
    orthonormal basis.
*/


class SG_API Oriented : public OrthoTransformed
{

public:

  /// The \b Primary value.
  static const TOOLS(Vector3) DEFAULT_PRIMARY;

  /// The \b Secondary value.
  static const TOOLS(Vector3) DEFAULT_SECONDARY;


  /// A structure which helps to build a Oriented when parsing.
  struct Builder : public OrthoTransformed::Builder {

    /// A pointer to the \b Primary field.
    TOOLS(Vector3) * Primary;

     /// A pointer to the \b Secondary field.
    TOOLS(Vector3) * Secondary;

    /// Constructor.
    Builder( );

    /// Desructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  Oriented();

  /** Constructs a Oriented \e goemetry with the directions \e primary
      and \e secondary.
      \warning
      - \e primary must be normalized;
      - \e secondary must be normalized and orthogonal to \e primary;
      - \e geometry must be non null. */
  Oriented( const TOOLS(Vector3)& primary,
            const TOOLS(Vector3)& secondary,
            const GeometryPtr& geometry );

  /// Destructor
  virtual ~Oriented( );

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /// Returns \b Primary value.
  const TOOLS(Vector3)& getPrimary( ) const;

  /// Returns \b Primary field.
  TOOLS(Vector3)& getPrimary( );

  /// Returns \b Secondary value.
  const TOOLS(Vector3)& getSecondary( ) const ;

  /// Returns \b Secondary field.
  TOOLS(Vector3)& getSecondary( ) ;

  virtual Transformation3DPtr getTransformation( ) const;

  virtual OrthonormalBasis3DPtr getOrthonormalBasis() const;


  /// Returns whether \b Primary is set to its default first
  bool isPrimaryToDefault( ) const ;

  /// Returns whether \b Secondary is set to its default value.
  bool isSecondaryToDefault( ) const;

  virtual bool isValid( ) const;



protected:

  /// The \b Primary field.
  TOOLS(Vector3) __primary;

  /// The \b Secondary field.
  TOOLS(Vector3) __secondary;

};

/// Oriented Pointer
typedef RCPtr<Oriented> OrientedPtr;


/* ----------------------------------------------------------------------- */

// __geom_oriented_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

