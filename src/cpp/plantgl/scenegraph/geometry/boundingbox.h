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

/*! \file geom_boundingbox.h
    \brief Definition of the class BoundingBox.
*/


#ifndef __geom_boundingbox_h__
#define __geom_boundingbox_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/math/util_vector.h>
#include <plantgl/tool/rcobject.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include "../sg_config.h"
#include <iostream>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE
class Matrix4;
class Matrix3;
PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class BoundingBox;
typedef RCPtr<BoundingBox> BoundingBoxPtr;

/* ----------------------------------------------------------------------- */

/**
    \class BoundingBox
    \brief An axis aligned box represented by 2 opposing corners.
*/

class SG_API BoundingBox : public RefCountObject
{

public:

  /// Constructs a degenerate BoundingBox reduced to the point \e point.
    BoundingBox( const Vector3& point = TOOLS(Vector3::ORIGIN) );

  /** \brief Constructs a BoundingBox with the 2 opposing corners \e lowLeft and
      \e upRight.

      \pre
      - \e lowLeft must be, coordinate by coordinate, less than \e upRight. */
  BoundingBox( const Vector3& lowLeft, const Vector3& upRight );

  /// Constructs a BoundingBox from the \e points.
  BoundingBox(const Point3ArrayPtr points);

  /// Destructor
  virtual ~BoundingBox( ) ;

  /** \brief Extends \e self in order to bound \e bbox.

      \pre
      - \e self must be valid;
      - \e bbox must be non null and valid. */
   BoundingBox& extend( const BoundingBoxPtr& bbox );

  /** \brief Extends \e self in order to bound \e bbox.

      \pre
      - \e self must be valid;
      - \e bbox must be valid. */
   BoundingBox& extend( const BoundingBox& bbox );

  /** \brief Extends \e self in order to bound \e point.

      \pre
      - \e self must be valid;
      - \e point must be valid. */
   BoundingBox& extend( const Vector3& point );

  /** \brief Extends \e self in order to bound \e bbox.

      \pre
      - \e self must be valid;
      - \e bbox must be valid. */
   BoundingBox& operator +=( const BoundingBox& bbox );

  /** \brief Extends \e self in order to bound \e point.

      \pre
      - \e self must be valid;
      - \e point must be valid. */
   BoundingBox& operator +=( const Vector3& point );

  /** \brief Extends \e self in order to bound \e bbox.

      \pre
      - \e self must be valid;
      - \e bbox must be valid. */
   BoundingBox& operator |=( const BoundingBox& bbox );

  /** \brief Extends \e self in order to bound \e point.

      \pre
      - \e self must be valid;
      - \e point must be valid. */
   BoundingBox& operator |=( const Vector3& point );


  /** \brief Intersection of \e self with \e bbox.

      \pre
      - \e self must be valid;
      - \e bbox must be valid. 
	  \post
	  - If self and bbox do not intersect, self will not be valid
	  */
   BoundingBox& operator &=( const BoundingBox& bbox );

   /** \brief Intersection between \e self and \e bbox.

      \pre
      - \e self must be valid;
      - \e bbox must be non null and valid. */
   bool intersect( const BoundingBoxPtr& bbox ) const;

   /** \brief Intersection between \e self and \e bbox.

      \pre
      - \e self must be valid;
      - \e bbox must be valid. */
   bool intersect( const BoundingBox& bbox ) const;

  /** \brief Intersection between \e self and \e point.

      \pre
      - \e self must be valid;
      - \e point must be valid. */
   bool intersect( const Vector3& point ) const;

   /** \brief Inclusion of \e bbox in \e self.

      \pre
      - \e self must be valid;
      - \e bbox must be valid. */
   bool include( const BoundingBox& bbox ) const;

   /** \brief Intersection between \e self and a segment defined by [ \e point1 , \e point2 ].

      \pre
      - \e self must be valid;
      - \e point1 and point2 must be valid. */
   bool intersect( const Vector3& point1, const Vector3& point2 ) const;

   /** \brief Compute distance between \e self and another bbox.

      \pre
      - \e self must be valid;
      - \e bbox  must be valid. */
   real_t distance(const BoundingBox& bbox) const;

   /** \brief Compute distance between \e self and a point.

      \pre
      - \e self must be valid;
      - \e point  must be valid. */
   real_t distance(const Vector3& point) const;

   /// Set new values.
  void set( const Vector3& lowLeft, const Vector3& upRight );

   /// Change values.
  void change( const Vector3& center, const Vector3& size );

   /** Returns the center of \e self.
       \pre
      - \e self must be valid. */
  Vector3 getCenter( ) const ;

  /** \brief Returns the lower left corner of \e self.

      \pre
      - \e self must be valid. */
  const Vector3& getLowerLeftCorner( ) const ;

  /** \brief Returns the lower left corner of \e self.

      \pre
      - \e self must be valid. */
  Vector3& getLowerLeftCorner( );

  /** \brief Returns the upper right corner of \e self.

      \pre
      - \e self must be valid. */
  const Vector3& getUpperRightCorner( ) const;

  /** \brief Returns the upper right corner of \e self.

      \pre
      - \e self must be valid. */
  Vector3& getUpperRightCorner( );

  /** \brief Returns the size of the half diagonal of \e self along the \c x-axis,
      \c y-axis and \c z-axis.

      \pre
      - \e self must be valid. */
  Vector3 getSize( ) const ;

  /// Get X range
  real_t getXRange() const;
  /// Get Y range
  real_t getYRange() const;
  /// Get Z range
  real_t getZRange() const;

  /// Get X min value
  real_t getXMin() const;
  /// Get Y min value
  real_t getYMin() const;
  /// Get Z min value
  real_t getZMin() const;
  
  /// Get X max value
  real_t getXMax() const;
  /// Get Y max value
  real_t getYMax() const;
  /// Get Z max value
  real_t getZMax() const;
  
  /// Returns whether \e self is valid.
  bool isValid( ) const;

  /** \brief Transform a bounding box by an affine transformation.

      The algorithm is taken from : Tranforming Axis-Aligned Bounding Boxes
      by James Arvo. Graphics Gems I - p 548-550.
      Meanwhile the degenerate interval used at the initialization 
	  of the algorithm is set to the center of the bounding box
      to be transformed.
  */
  void transform(const Matrix4& matrix);

  /** \brief Transform a bounding box by an affine transformation.

      The algorithm is taken from : Tranforming Axis-Aligned Bounding Boxes
      by James Arvo. Graphics Gems I - p 548-550.
      The only difference is that we do not have to take into account a
      translation. Meanwhile the degenerate interval used at the
      initialization of the algorithm is set to the center of the bounding box
      to be transformed.
  */
  void transform(const Matrix3& m);

  /// translation of self 
  void translate(const Vector3& t);

  /// scaling of self.
  void scale(const Vector3& t);

  bool operator==(const BoundingBox& other) const;

  inline bool operator!=(const BoundingBox& other) const { return !operator==(other); }

  /// Returns the addition of \e b1 and \e b2.
  friend SG_API BoundingBox operator+( const BoundingBox& b1, const BoundingBox& b2);

  /// Returns the addition of \e b1 and \e v2.
  friend SG_API BoundingBox operator+( const BoundingBox& b1, const Vector3& v2);

  /// Returns the addition of \e b1 and \e b2.
  friend SG_API BoundingBox operator|( const BoundingBox& b1, const BoundingBox& b2);

  /// Returns the addition of \e b1 and \e v2.
  friend SG_API BoundingBox operator|( const BoundingBox& b1, const Vector3& v2);

  /// Returns the intersection of \e b1 and \e b2.
  friend SG_API BoundingBox operator&( const BoundingBox& b1, const BoundingBox& b2);

  /// Returns whether \e b1 intersect \e b2.
  friend SG_API bool intersection( const BoundingBox& b1, const BoundingBox& b2) ;

  /// Returns whether \e b1 intersect \e v2.
  friend SG_API bool intersection( const BoundingBox& b1, const Vector3& v2);

protected:

  /// The lower left corner.
  Vector3 __ll;

  /// The upper right corner.
  Vector3 __ur;

};

/// BoundingBox Pointer
typedef RCPtr<BoundingBox> BoundingBoxPtr;

SG_API BoundingBox operator+( const BoundingBox& b1, const BoundingBox& b2);
SG_API BoundingBox operator+( const BoundingBox& b1, const Vector3& v2);
SG_API BoundingBox operator|( const BoundingBox& b1, const BoundingBox& b2);
SG_API BoundingBox operator|( const BoundingBox& b1, const Vector3& v2);
SG_API BoundingBox operator&( const BoundingBox& b1, const BoundingBox& b2);
SG_API bool intersection( const BoundingBox& b1, const BoundingBox& b2) ;
SG_API bool intersection( const BoundingBox& b1, const Vector3& v2);

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/// Write BoundingBox \b b to \b stream.
// SG_API std::ostream& operator<<( std::ostream& stream, PGL(BoundingBox&) b );

/* ----------------------------------------------------------------------- */
// __geom_bbox_h__
#endif

