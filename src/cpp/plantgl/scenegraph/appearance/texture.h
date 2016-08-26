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


/*! \file appe_ImageTexture.h
    \brief Definition of the appearance class ImageTexture.
*/


#ifndef __appe_imagetexture_h__
#define __appe_imagetexture_h__


#include "material.h"
#include "color.h"
#include <plantgl/math/util_vector.h>



/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

class Point2Array;
class Point3Array;
typedef RCPtr<Point2Array> Point2ArrayPtr;
typedef RCPtr<Point3Array> Point3ArrayPtr;

/* ----------------------------------------------------------------------- */


class SG_API Texture2DTransformation : public SceneObject {

public:

  /// The default scaling value
  static const TOOLS(Vector2) DEFAULT_SCALE;

  /// The default translation value
  static const TOOLS(Vector2) DEFAULT_TRANSLATION;

  /// The default center value
  static const TOOLS(Vector2) DEFAULT_ROTATIONCENTER;

  /// The default center value
  static const real_t DEFAULT_ROTATIONANGLE;

  /// A structure which helps to build a Material when parsing.
  struct SG_API Builder : public SceneObject::Builder {

    /// A pointer to the \b Scaling field.
	TOOLS(Vector2) * Scale;

    /// A pointer to the \b Translation field.
	TOOLS(Vector2) * Translation;

    /// A pointer to the \b RotationCenter field.
	TOOLS(Vector2) * RotationCenter;

    /// A pointer to the \b RotationAngle field.
    real_t * RotationAngle;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  Texture2DTransformation(const TOOLS(Vector2)& scaling = DEFAULT_SCALE, 
						const TOOLS(Vector2)& translation = DEFAULT_TRANSLATION, 
						const TOOLS(Vector2)& rotationCenter = DEFAULT_ROTATIONCENTER, 
						real_t rotationAngle = DEFAULT_ROTATIONANGLE);

  Texture2DTransformation(const std::string& name,
						const TOOLS(Vector2)& scale = DEFAULT_SCALE, 
						const TOOLS(Vector2)& translation = DEFAULT_TRANSLATION, 
						const TOOLS(Vector2)& rotationCenter = DEFAULT_ROTATIONCENTER, 
						real_t rotationAngle = DEFAULT_ROTATIONANGLE);
  /// Destructor.
  virtual ~Texture2DTransformation( ) ;

  PGL_OBJECT(Texture2DTransformation)

  /// Returns whether \e self id valid.
  virtual bool isValid( ) const;

  // The scaling property
  PGL_OBJECT_PROPERTY_WITH_DEFAULT(Scale,TOOLS(Vector2),DEFAULT_SCALE);

  // The translation property
  PGL_OBJECT_PROPERTY_WITH_DEFAULT(Translation,TOOLS(Vector2),DEFAULT_TRANSLATION);

  // The rotationCenter property
  PGL_OBJECT_PROPERTY_WITH_DEFAULT(RotationCenter,TOOLS(Vector2),DEFAULT_ROTATIONCENTER);

  // The rotationAngle property
  PGL_OBJECT_PROPERTY_WITH_DEFAULT(RotationAngle,real_t,DEFAULT_ROTATIONANGLE);

  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const;

  virtual Point2ArrayPtr transform( const Point2ArrayPtr& points ) const;

  TOOLS(Vector2) transform( const TOOLS(Vector2)& point ) const;


};

/// ImageTexture Pointer
typedef RCPtr<Texture2DTransformation> Texture2DTransformationPtr;

/**
    \class ImageTexture
    \brief The material of a textured object.
*/

class SG_API ImageTexture : public SceneObject
{

public:

  /// The default mipmaping value
  static const bool DEFAULT_MIPMAPING;

  /// The default repeatS value
  static const bool DEFAULT_REPEATS;

  /// The default repeatT value
  static const bool DEFAULT_REPEATT;


  /// A structure which helps to build a Material when parsing.
  struct SG_API Builder : public SceneObject::Builder {

    /// A pointer to the \b FileName field.
	std::string * FileName;

    /// A pointer to the \b Mipmaping field.
	bool * Mipmaping;

    /// A pointer to the \b RepeatS field.
    bool * RepeatS;

    /// A pointer to the \b RepeatT field.
    bool * RepeatT;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default constructor (invalid).
  ImageTexture( );

  /// constructor.
  ImageTexture(  const std::string& filename,
				 bool repeatS = DEFAULT_REPEATS,
				 bool repeatT = DEFAULT_REPEATT,
				 bool mipmaping = DEFAULT_MIPMAPING);

  /// constructor.
  ImageTexture(  const std::string& name,
				 const std::string& filename,
				 bool repeatS = DEFAULT_REPEATS,
				 bool repeatT = DEFAULT_REPEATT,
				 bool mipmaping = DEFAULT_MIPMAPING);

  /// Destructor.
  virtual ~ImageTexture( ) ;

  PGL_OBJECT(ImageTexture)

  // The Filename property
  PGL_OBJECT_PROPERTY(Filename,std::string);

  // The Mipmaping property
  PGL_OBJECT_PROPERTY_WITH_DEFAULT(Mipmaping,bool,DEFAULT_MIPMAPING);

  // The RepeatS property
  PGL_OBJECT_PROPERTY_WITH_DEFAULT(RepeatS,bool,DEFAULT_REPEATS);

  // The RepeatT property
  PGL_OBJECT_PROPERTY_WITH_DEFAULT(RepeatT,bool,DEFAULT_REPEATT);


  /// Returns whether \e self id valid.
  virtual bool isValid( ) const;

  // virtual bool isSimilar(const Material&) const;


};

/// ImageTexture Pointer
typedef RCPtr<ImageTexture> ImageTexturePtr;

/* ----------------------------------------------------------------------- */


class SG_API Texture2D : public Appearance
{
public:
  
  /// The default transformation value
  static const Texture2DTransformationPtr DEFAULT_TRANSFORMATION;

  /// The default base color value
  static const Color4 DEFAULT_BASECOLOR;

  /// A structure which helps to build a Material when parsing.
  struct SG_API Builder : public Appearance::Builder {

    /// A pointer to the \b Image field.
	ImageTexturePtr * Image;

    /// A pointer to the \b Transformation field.
	Texture2DTransformationPtr * Transformation;

    /// A pointer to the \b Transparency field.
    Color4 * BaseColor;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  Texture2D();

  Texture2D(const ImageTexturePtr& image, 
		    const Texture2DTransformationPtr& transformation = DEFAULT_TRANSFORMATION,
            const Color4& basecolor = DEFAULT_BASECOLOR);

  Texture2D(const std::string& name,
			const ImageTexturePtr& image, 
			const Texture2DTransformationPtr& transformation = DEFAULT_TRANSFORMATION,
            const Color4& basecolor = DEFAULT_BASECOLOR);

  ~Texture2D();

  virtual bool isTexture() const { return true; }

  PGL_OBJECT(Texture2D)

  // The Filename property
  PGL_OBJECT_PROPERTY(Image,ImageTexturePtr);

  // The Mipmaping property
  PGL_OBJECT_PROPERTY_WITH_DEFAULT(Transformation,Texture2DTransformationPtr,DEFAULT_TRANSFORMATION);

  // The BaseColor property
  PGL_OBJECT_PROPERTY_WITH_DEFAULT(BaseColor,Color4,DEFAULT_BASECOLOR);
  
  /// Returns whether \e self id valid.
  virtual bool isValid( ) const;

};

/// TextureAppearance Pointer
typedef RCPtr<Texture2D> Texture2DPtr;


/* ----------------------------------------------------------------------- */

// __appe_texture_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

