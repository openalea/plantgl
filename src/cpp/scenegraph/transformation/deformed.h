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


/*! \file geom_deformed.h
    \brief Definition of the transformation class Deformation and the transformed class Deformed.
*/


#ifndef __geom_deformed_h__
#define __geom_deformed_h__

/* ----------------------------------------------------------------------- */

#include "geom_transformed.h"

/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Primitive;
typedef RCPtr<Primitive> PrimitivePtr;

/* ----------------------------------------------------------------------- */

/** 
    \class Deformation
    \brief Abstract base class for axial deformations.
*/

class GEOM_API Deformation : public Transformation3D
{

public:

  /// Constructs a Deformation.
  Deformation( );

  /// Destructor.
  virtual ~Deformation( );
  
};

/// Deformation Pointer
typedef RCPtr<Deformation> DeformationPtr;


/* ----------------------------------------------------------------------- */


/** 
    \class Deformed
    \brief A deformed
*/


class GEOM_API Deformed : public Transformed
{

public: 

   /// A structure which helps to build an object of type of Transformed.
  struct Builder : public Transformed::Builder {

    /// Constructor
    Builder( );

    /// A pointer to the \b Primitive field.
    PrimitivePtr * Primitive;

    virtual void destroy( );

    virtual bool isValid( ) const;

    void DefDestroy( );

    bool DefValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  Deformed();

  /// Constructor.
  Deformed( const PrimitivePtr& primitive );

  /// Destructor.
  virtual ~Deformed( );

  virtual const GeometryPtr getGeometry( ) const;

  /** Returns \b Primitive value.*/
  const PrimitivePtr& getPrimitive( ) const;

  /** Returns \b Primitive field.*/
  PrimitivePtr& getPrimitive( );

  virtual bool isACurve( ) const;

  virtual bool isASurface( ) const ;

  virtual bool isAVolume( ) const ;

  virtual bool isExplicit( ) const;

protected:

  /// The \b Primitive field.
  PrimitivePtr __primitive;

};

/// Deformed Pointer
typedef RCPtr<Deformed> DeformedPtr;
 

/* ----------------------------------------------------------------------- */

// __geom_deformed_h__
/* ----------------------------------------------------------------------- */

GEOM_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

