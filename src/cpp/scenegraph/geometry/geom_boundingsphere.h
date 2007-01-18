/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) 
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


/*! \file geom_boundingsphere.h
    \brief Definition of the class BoundingSphere.
*/


#ifndef __geom_boundingsphere_h__
#define __geom_boundingsphere_h__

/* ----------------------------------------------------------------------- */

#include "Tools/util_vector.h"
#include "Tools/rcobject.h"
#include "geom_namespace.h"
#include <iostream>

/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE
class Matrix4;
TOOLS_END_NAMESPACE

/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class BoundingSphere;
typedef RCPtr<BoundingSphere> BoundingSpherePtr;

/* ----------------------------------------------------------------------- */

/**
    \class BoundingSphere
    \brief A sphere used to give the bound of GEOM object.
*/

class GEOM_API BoundingSphere : public TOOLS(RefCountObject)
{

public:

  /// Constructs a degenerate BoundingSphere reduced to the point \e point.
  BoundingSphere( const TOOLS(Vector3)& point );

  /// Constructs a degenerate BoundingSphere reduced to the point \e point.
  BoundingSphere( const TOOLS(Vector2)& point );

  /** Constructs a BoundingSphere with the 2 opposing corners \e lowLeft and
      \e upRight.
      \pre
      - \e lowLeft must be, coordinate by coordinate, less than \e upRight. */
  BoundingSphere( const TOOLS(Vector3)& center, const real_t& radius );

  /** Constructs a BoundingSphere with the 2 opposing corners \e lowLeft and
      \e upRight.
      \pre
      - \e lowLeft must be, coordinate by coordinate, less than \e upRight. */
  BoundingSphere( const TOOLS(Vector3)& point1, const TOOLS(Vector3)& point2 );

  /// Destructor
  virtual ~BoundingSphere( ) ;

  /** Extends \e self in order to bound \e bsphere.
      \pre
      - \e self must be valid;
      - \e bsphere must be non null and valid. */
   BoundingSphere& extend( const BoundingSpherePtr& bsphere );

  /** Extends \e self in order to bound \e bsphere.
      \pre
      - \e self must be valid;
      - \e bsphere must be valid. */
   BoundingSphere& extend( const BoundingSphere& bsphere );

  /** Extends \e self in order to bound \e point.
      \pre
      - \e self must be valid;
      - \e point must be valid. */
   BoundingSphere& extend( const TOOLS(Vector3)& point );

  /** Extends \e self in order to bound \e point.
      \pre
      - \e self must be valid;
      - \e point must be valid. */
   BoundingSphere& extend( const TOOLS(Vector2)& point );

  /** Extends \e self in order to bound \e bsphere.
      \pre
      - \e self must be valid;
      - \e bsphere must be non null and valid. */
   BoundingSphere& operator+=( const BoundingSpherePtr& bsphere );

  /** Extends \e self in order to bound \e bsphere.
      \pre
      - \e self must be valid;
      - \e bsphere must be valid. */
   BoundingSphere& operator+=( const BoundingSphere& bsphere );

  /** Extends \e self in order to bound \e point.
      \pre
      - \e self must be valid;
      - \e point must be valid. */
   BoundingSphere& operator+=( const TOOLS(Vector3)& point );

   /** Extends \e self in order to bound \e point.
      \pre
      - \e self must be valid;
      - \e point must be valid. */
   BoundingSphere& operator+=( const TOOLS(Vector2)& point );

   /** Intersection between \e self and \e bsphere.
      \pre
      - \e self must be valid;
      - \e bsphere must be non null and valid. */
   bool intersect( const BoundingSpherePtr& bsphere ) const;

   /** Intersection between \e self and \e bsphere.
      \pre
      - \e self must be valid;
      - \e bsphere must be valid. */
   bool intersect( const BoundingSphere& bsphere ) const;


  /** Intersection between \e self and \e point.
      \pre
      - \e self must be valid;
      - \e point must be valid. */
   bool intersect( const TOOLS(Vector3)& point ) const;

  /** Intersection between \e self and a segment defined by [ \e point1 , \e point2 ].
      \pre
      - \e self must be valid;
      - \e point1 and point2 must be valid. */
   bool intersect( const TOOLS(Vector3)& point1, const TOOLS(Vector3)& point2 ) const;

   /// Change values.
  void change( const TOOLS(Vector3)& center, const real_t& size );

   /** Returns the center of \e self.
       \pre
      - \e self must be valid. */
  const TOOLS(Vector3)& getCenter( ) const ;

   /** Returns the center of \e self.
       \pre
      - \e self must be valid. */
  TOOLS(Vector3)& getCenter( );

  /** Returns the radius of \e self.
      \pre
      - \e self must be valid. */
  const real_t& getRadius( ) const ;

  /** Returns the radius of \e self.
      \pre
      - \e self must be valid. */
  real_t& getRadius( ) ;

  /// Returns whether \e self is valid.
  bool isValid( ) const;

  /** Transform a bounding sphere by an affine transformation.
  */
  void transform(const TOOLS(Matrix4)& matrix);

  /// Returns the result of the addition of \e b1 and \e b2.
  friend GEOM_API BoundingSphere operator+( const BoundingSphere& b1, const BoundingSphere& b2);

  /// Returns the result of the addition of \e b1 and \e v2.
  friend GEOM_API BoundingSphere operator+( const BoundingSphere& b1, const TOOLS(Vector3)& v2);

  /// Returns whether \e b1 intersect \e b2.
  friend GEOM_API bool intersection( const BoundingSphere& b1, const BoundingSphere& b2) ;

  /// Returns whether \e b1 intersect \e v2.
  friend GEOM_API bool intersection( const BoundingSphere& b1, const TOOLS(Vector3)& v2);

//  friend std::ostream& operator<<( std::ostream& stream, BoundingSphere& b );

protected:

  /// The center.
  TOOLS(Vector3) __center;

  /// The radius.
  real_t __radius;

};

/// BoundingSphere Pointer
typedef RCPtr<BoundingSphere> BoundingSpherePtr;


/* ----------------------------------------------------------------------- */

GEOM_END_NAMESPACE

/// Write BoundingSphere \b in \b stream.
GEOM_API std::ostream& operator<<( std::ostream& stream, GEOM(BoundingSphere&) b );

/* ----------------------------------------------------------------------- */
// __geom_boundingsphere_h__
#endif

