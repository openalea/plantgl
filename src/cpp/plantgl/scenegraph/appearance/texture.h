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


/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class ImageTexture
    \brief The material of a textured object.
*/



class SG_API ImageTexture : public Material
{

public:

  /// The default mimpaing value
  static bool DEFAULT_MIPMAPING;

  /// A structure which helps to build a Material when parsing.
  struct SG_API Builder : public Material::Builder {

    /// A pointer to the \b FileName field.
	std::string * FileName;

    /// A pointer to the \b Mipmaping field.
	bool * Mipmaping;

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
            bool mipmaping = DEFAULT_MIPMAPING,
            const Color3& ambient = DEFAULT_AMBIENT,
            const real_t& diffuse = DEFAULT_DIFFUSE,
            const Color3& specular = DEFAULT_SPECULAR,
            const Color3& emission = DEFAULT_EMISSION,
            const real_t& shininess = DEFAULT_SHININESS,
            const real_t& transparency = DEFAULT_TRANSPARENCY);

  /// constructor.
  ImageTexture(  const std::string& filename,
            const std::string& name,
            bool mipmaping = DEFAULT_MIPMAPING,
            const Color3& ambient = DEFAULT_AMBIENT,
            const real_t& diffuse = DEFAULT_DIFFUSE,
            const Color3& specular = DEFAULT_SPECULAR,
            const Color3& emission = DEFAULT_EMISSION,
            const real_t& shininess = DEFAULT_SHININESS,
            const real_t& transparency = DEFAULT_TRANSPARENCY);

  /// Destructor.
  virtual ~ImageTexture( ) ;

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /// Returns \b Filename value.
  const std::string& getFilename( ) const ;

  /// Returns \b Filename field.
  std::string& getFilename( ) ;

  /// Returns \b Mipmaping value.
  const bool getMipmaping( ) const { return __mipmaping; }

  /// Returns \b Mipmaping field.
  bool& getMipmaping( ) { return __mipmaping; }

  /// Returns whether \b Mipmaping is set to default.
  bool isMipmapingToDefault( ) { return __mipmaping == DEFAULT_MIPMAPING; }

  /// Returns whether \e self id valid.
  virtual bool isValid( ) const;

  virtual bool isTexture() const;

protected:

   /// The \b Filename field.
  std::string __filename;

  bool __mipmaping;

};

/// ImageTexture Pointer
typedef RCPtr<ImageTexture> ImageTexturePtr;

/* ----------------------------------------------------------------------- */

// __appe_texture_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

