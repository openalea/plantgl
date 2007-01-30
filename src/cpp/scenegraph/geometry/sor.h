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


/*! \file geom_sor.h
    \brief Definition of the geometry class SOR and SOR2D.
*/


#ifndef __geom_sor_h__
#define __geom_sor_h__

/* ----------------------------------------------------------------------- */

#include "parametricmodel.h"
#include "planarmodel.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class SOR
    \brief Abstract base class for 3D objects of type of surface of revolution.
*/

/* ----------------------------------------------------------------------- */

class SG_API SOR : public ParametricModel
{

public:

  /// The \b Slices field default value.
  static const uchar_t DEFAULT_SLICES;

  /// A structure which helps to build an object of type of SOR.
  struct SG_API Builder : public ParametricModel::Builder {

    /// A pointer to the \b Slices field.
    uchar_t * Slices;

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

    virtual void destroy( );

    void SORDestroy( );

    virtual bool isValid( ) const;

    bool SORValid( ) const;

  };


  /// Constructor.
  SOR( uchar_t slices = DEFAULT_SLICES );

  /// Destructor
  virtual ~SOR( );

  /// Returns \b Slices value.
  const uchar_t& getSlices( ) const ;

  /// Returns \b Slices field.
  uchar_t& getSlices( );

  virtual bool isACurve( ) const ;

  virtual bool isASurface( ) const ;

  /// Returns whether \b Slices is set to its default value.
  bool isSlicesToDefault( ) const ;


protected:

  /// The \b Slices field.
  uchar_t __slices;

};

/// SOR Pointer
typedef RCPtr<SOR> SORPtr;


/* ----------------------------------------------------------------------- */

/**
    \class SOR2D
    \brief Abstract base class for 2D objects of type of surface of revolution.
*/

class SG_API SOR2D : public PlanarModel
{

public:

  /// A structure which helps to build an object of type of SOR2D.
  struct Builder : public PlanarModel::Builder {

    /// A pointer to the \b Slices field.
    uchar_t * Slices;

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

    virtual void destroy( );

    virtual bool isValid( ) const;

    void SOR2DDestroy( );

    bool SOR2DValid( ) const;

  };


  /// Constructor.
  SOR2D( uchar_t slices = SOR::DEFAULT_SLICES );

  /// Destructor
  virtual ~SOR2D( );

  /// Returns \b Slices value.
  const uchar_t& getSlices( ) const ;

  /// Returns \b Slices field .
  uchar_t& getSlices( ) ;

  virtual bool isACurve( ) const ;

  virtual bool isASurface( ) const ;

  /// Returns whether \b Slices is set to its default value.
  bool isSlicesToDefault( ) const ;


protected:

  /// The \b Slices field.
  uchar_t __slices;

};

/// SOR2D Pointer
typedef RCPtr<SOR2D> SOR2DPtr;


/* ----------------------------------------------------------------------- */

// __geom_sor_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

