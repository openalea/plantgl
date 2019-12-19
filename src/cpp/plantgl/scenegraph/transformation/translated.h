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
  Translation( const Vector3& v );

  /// Destructor.
  virtual ~Translation( );

  /// Return Matrix4(1,0,0,__vector.x(),  0,1,0,__vector.y(),  0,0,1,__vector.z(),  0,0,0,1);
  virtual Matrix4 getMatrix( ) const;

  virtual bool isValid( ) const;

  /// Returns the displacement \e self represents.
  const Vector3& getVector( ) const;

  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const;

  virtual Point4ArrayPtr transform( const Point4ArrayPtr& points ) const;

  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const;

  virtual Point4MatrixPtr transform( const Point4MatrixPtr& points ) const;

protected:

  /// The translation vector.
  Vector3 __vector;
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
  static const Vector3 DEFAULT_TRANSLATION;


  /// A structure which helps to build a Translated when parsing.
  struct SG_API Builder : public MatrixTransformed::Builder {

    /// A pointer to the \b Translation field.
    Vector3 * Translation;

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
  explicit Translated( const Vector3& translation,
                       const GeometryPtr& geometry );

  /// Destructor
  virtual ~Translated( );

  PGL_OBJECT(Translated)

  virtual Transformation3DPtr getTransformation( ) const ;

  /// Returns Translation value.
  const Vector3& getTranslation( ) const ;

  /// Returns Translation field.
  Vector3& getTranslation( ) ;

  /// Returns whether Translation is set to its default value.
  bool isTranslationToDefault( ) const ;

  virtual bool isValid( ) const;

protected:

  /// The Translation field.
  Vector3 __translation;

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
  Translation2D( const Vector2& v );

  /// Destructor.
  virtual ~Translation2D( );

  /// Return Matrix3(1,0,__vector.x(), 0,1,__vector.y(),  0,0,1);
  virtual Matrix3 getMatrix( ) const;

  virtual bool isValid( ) const;

  /// Returns the displacement \e self represents.
  const Vector2& getVector( ) const ;

  virtual Point2ArrayPtr transform( const Point2ArrayPtr& points ) const;

  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const;

  virtual Point2MatrixPtr transform( const Point2MatrixPtr& points ) const;

  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const;

protected:

  /// The translation vector.
  Vector2 __vector;
};

/// Translation2D Pointer
typedef RCPtr<Translation2D> Translation2DPtr;


/* ----------------------------------------------------------------------- */

// __geom_translated_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

