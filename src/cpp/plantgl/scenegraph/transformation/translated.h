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



/*! \file geom_translated.h
    \brief Definition of the transformation class Translation, Translation2D
	and the transformed class Translated.
*/

#ifndef __geom_translated_h__
#define __geom_translated_h__

/* ----------------------------------------------------------------------- */

#include "mattransformed.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class Translation
    \brief A Translation of a vector.
*/

/* ----------------------------------------------------------------------- */

class SG_API Translation : public Matrix4Transformation
{

public:

  /** Constructs a Translation with the Vector3 \e v.
      \pre
      - \e v must be valid.
      \post
      - \e self is valid. */
  Translation( const TOOLS(Vector3)& v );

  /// Destructor.
  virtual ~Translation( );

  /// Return Matrix4(1,0,0,__vector.x(),  0,1,0,__vector.y(),  0,0,1,__vector.z(),  0,0,0,1);
  virtual TOOLS(Matrix4) getMatrix( ) const;

  virtual bool isValid( ) const;

  /// Returns the displacement \e self represents.
  const TOOLS(Vector3)& getVector( ) const;

  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const;

  virtual Point4ArrayPtr transform( const Point4ArrayPtr& points ) const;

  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const;

  virtual Point4MatrixPtr transform( const Point4MatrixPtr& points ) const;

protected:

  /// The translation vector.
  TOOLS(Vector3) __vector;
};

/// Translation Pointer
typedef RCPtr<Translation> TranslationPtr;


/* ----------------------------------------------------------------------- */

/**
    \class Translated
    \brief A 3D geometry object translated by an arbitrary translation vector.
*/


class SG_API Translated : public MatrixTransformed
{

public:

  /// The Translation field default value
  static const TOOLS(Vector3) DEFAULT_TRANSLATION;


  /// A structure which helps to build a Translated when parsing.
  struct SG_API Builder : public MatrixTransformed::Builder {

    /// A pointer to the \b Translation field.
    TOOLS(Vector3) * Translation;

    /// Constructor.
    Builder( );

    /// Drestructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  Translated();

  /** Constructs a Translated object with the translation \e translation
      and the Geometry object \e geometry.
      \pre
      - \e translation must be valid,
      - \e geometry must be non null and valid. */
  explicit Translated( const TOOLS(Vector3)& translation,
                       const GeometryPtr& geometry );

  /// Destructor
  virtual ~Translated( );

  PGL_OBJECT(Translated)

  virtual Transformation3DPtr getTransformation( ) const ;

  /// Returns Translation value.
  const TOOLS(Vector3)& getTranslation( ) const ;

  /// Returns Translation field.
  TOOLS(Vector3)& getTranslation( ) ;

  /// Returns whether Translation is set to its default value.
  bool isTranslationToDefault( ) const ;

  virtual bool isValid( ) const;

protected:

  /// The Translation field.
  TOOLS(Vector3) __translation;

};

/// Translated Pointer
typedef RCPtr<Translated> TranslatedPtr;


/* ----------------------------------------------------------------------- */

/**
    \class Translation2D
    \brief A 2D Translation of a vector.
*/

class SG_API Translation2D : public Matrix3Transformation
{

public:

  /** Constructs a Translation with the Vector3 \e v.
      \pre
      - \e v must be valid.
      \post
      - \e self is valid. */
  Translation2D( const TOOLS(Vector2)& v );

  /// Destructor.
  virtual ~Translation2D( );

  /// Return Matrix3(1,0,__vector.x(), 0,1,__vector.y(),  0,0,1);
  virtual TOOLS(Matrix3) getMatrix( ) const;

  virtual bool isValid( ) const;

  /// Returns the displacement \e self represents.
  const TOOLS(Vector2)& getVector( ) const ;

  virtual Point2ArrayPtr transform( const Point2ArrayPtr& points ) const;

  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const;

  virtual Point2MatrixPtr transform( const Point2MatrixPtr& points ) const;

  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const;

protected:

  /// The translation vector.
  TOOLS(Vector2) __vector;
};

/// Translation2D Pointer
typedef RCPtr<Translation2D> Translation2DPtr;


/* ----------------------------------------------------------------------- */

// __geom_translated_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

