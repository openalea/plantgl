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


/*! \file appe_ImageTexture.h
    \brief Definition of the appearance class ImageTexture.
*/


#ifndef __appe_imagetexture_h__
#define __appe_imagetexture_h__


#include "appe_material.h"
#include "appe_color.h"


/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class ImageTexture
    \brief The material of a textured object.
*/



class GEOM_API ImageTexture : public Material
{

public:

  /// A structure which helps to build a Material when parsing.
  struct Builder : public Material::Builder {

    /// A pointer to the \b Ambient field.
	std::string * FileName;

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
            const Color3& ambient = DEFAULT_AMBIENT,
            const real_t& diffuse = DEFAULT_DIFFUSE,
            const Color3& specular = DEFAULT_SPECULAR,
            const Color3& emission = DEFAULT_EMISSION,
            const real_t& shininess = DEFAULT_SHININESS,
            const real_t& transparency = DEFAULT_TRANSPARENCY);

  /// constructor.
  ImageTexture(  const std::string& filename,
            const std::string& name,
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

  /// Returns whether \e self id valid.
  virtual bool isValid( ) const;

  virtual bool isTexture() const;

protected:

   /// The \b Filename field.
  std::string __filename;

};

/// ImageTexture Pointer
typedef RCPtr<ImageTexture> ImageTexturePtr;

/* ----------------------------------------------------------------------- */

// __appe_texture_h__
/* ----------------------------------------------------------------------- */

GEOM_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

