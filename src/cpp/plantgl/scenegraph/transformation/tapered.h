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



/*! \file geom_tapered.h
    \brief Definition of the transformation class Taper
	and the transformed class Tapered.
*/


#ifndef __geom_tapered_h__
#define __geom_tapered_h__

/* ----------------------------------------------------------------------- */

#include "deformed.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/** 
    \class Taper
    \brief A Taper axial deformation.
    It scale the radius cylindrical coordinate of a point according its
    z coordinate.
*/

/* ----------------------------------------------------------------------- */

class SG_API Taper : public Deformation
{

public:

  /** Constructs a Taper with \e baseRadius and \e topRadius.
      \pre
      - \e baseRadius must be greater or equal to 0;
      - \e topRadius must be greate ror equal to 0;
      \post
      - \e self is valid. */
  Taper( const real_t& baseRadius, 
	 const real_t& topRadius );

  /// Destructor.
  virtual ~Taper( );

  /// Returns the base radius of \e self.
  const real_t& getBaseRadius( ) const ;

  /// Returns the top radius of \e self.
  real_t getTopRadius( ) const;

  virtual bool isValid( ) const;

  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const;

  virtual Point4ArrayPtr transform( const Point4ArrayPtr& points ) const;

  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const;

  virtual Point4MatrixPtr transform( const Point4MatrixPtr& points ) const;

protected:

  /// The base radius
  real_t __baseRadius;

  /// The delta radius = __baseRadius - __topRadius
  real_t __deltaRadius;

};

/// Taper Pointer
typedef RCPtr<Taper> TaperPtr;

 
/* ----------------------------------------------------------------------- */

/** 
    \class Tapered
    \brief A geometry object transformed by an arbitrary taper 
    transformation.
*/


class SG_API Tapered : public Deformed
{

public:

  /// The default BaseRadius field value.
  static const real_t DEFAULT_BASE_RADIUS;

  /// The default TopRadius field value.
  static const real_t DEFAULT_TOP_RADIUS;  


  /// A structure which helps to build a Translated when parsing. 
  struct SG_API Builder : public Deformed::Builder {  

    /// A pointer to the \b BaseRadius field.
    real_t * BaseRadius;

    /// A pointer to the \b TopRadius field.
    real_t * TopRadius; 
    
    /// Constructor.
    Builder( );  

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );
    
    virtual bool isValid( ) const;

  };
    
  /// Default Constructor. Build object is invalid.
  Tapered();

  /// Default constructor
  explicit Tapered( const real_t& baseRadius,
		    const real_t& topRadius,
		    const PrimitivePtr& primitive );

  /// Destructor
  virtual ~Tapered( );

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /// Returns BaseRadius value.
  const real_t& getBaseRadius( ) const;
    
  /// Returns BaseRadius field.
  real_t& getBaseRadius( );
    
  virtual Transformation3DPtr getTransformation( ) const;

  /// Returns TopRadius value.
  const real_t&  getTopRadius( ) const;

  /// Returns TopRadius field.
  real_t&  getTopRadius( );

  /// Returns whether BaseRadius is set to its default value.
  bool isBaseRadiusToDefault( ) const;

  /// Returns whether TopRadius is set to its default value.
  bool isTopRadiusToDefault( ) const ;
 
  virtual bool isValid( ) const;

protected:

  /// The BaseRadius field.
  real_t __baseRadius;

  /// The TopRadius field.
  real_t __topRadius;

};

/// Tapered Pointer
typedef RCPtr<Tapered> TaperedPtr;
 

/* ----------------------------------------------------------------------- */

// __geom_tapered_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

