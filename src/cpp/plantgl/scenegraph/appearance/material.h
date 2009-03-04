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


/*! \file appe_material.h
    \brief Definition of the appearance class Material.
*/


#ifndef __appe_material_h__
#define __appe_material_h__


#include "appearance.h"
#include "color.h"


/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class Material
    \brief The material of an object.
*/



class SG_API Material : public Appearance
{

public:

  /// The default \b Ambient field value.
  static const Color3 DEFAULT_AMBIENT;

  /// The default \b Diffuse field value.
  static const real_t DEFAULT_DIFFUSE;

  /// The default \b Specular field value.
  static const Color3 DEFAULT_SPECULAR;

  /// The default \b Emission field value.
  static const Color3 DEFAULT_EMISSION;

  /// The default \b Shininess field value.
  static const real_t DEFAULT_SHININESS;

  /// The default \b Transparency field value.
  static const real_t DEFAULT_TRANSPARENCY;

  /// The default \b Material.
  static const AppearancePtr DEFAULT_MATERIAL;

  /// A structure which helps to build a Material when parsing.
  struct SG_API Builder : public Appearance::Builder {

    /// A pointer to the \b Ambient field.
    Color3 * Ambient;

    /// A pointer to the \b Diffuse field.
    real_t * Diffuse;

    /// A pointer to the \b Specular field.
    Color3 * Specular;

    /// A pointer to the \b Emission field.
    Color3 * Emission;

    /// A pointer to the \b Shininess field.
    real_t * Shininess;

    /// A pointer to the \b Transparency field.
    real_t * Transparency;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

    bool MaterialValidity( ) const;

  };


  /// Default constructor.
  Material( const Color3& ambient = DEFAULT_AMBIENT,
            const real_t& diffuse = DEFAULT_DIFFUSE,
            const Color3& specular = DEFAULT_SPECULAR,
            const Color3& emission = DEFAULT_EMISSION,
            const real_t& shininess = DEFAULT_SHININESS,
            const real_t& transparency = DEFAULT_TRANSPARENCY);

  /// Default constructor.
  Material( const std::string& name,
            const Color3& ambient = DEFAULT_AMBIENT,
            const real_t& diffuse = DEFAULT_DIFFUSE,
            const Color3& specular = DEFAULT_SPECULAR,
            const Color3& emission = DEFAULT_EMISSION,
            const real_t& shininess = DEFAULT_SHININESS,
            const real_t& transparency = DEFAULT_TRANSPARENCY);

  /// Destructor.
  virtual ~Material( ) ;

  PGL_OBJECT(Material)

  /// Returns \b Ambient value.
  const Color3& getAmbient( ) const ;

  /// Returns \b Ambient field.
  Color3& getAmbient( );

  /// Returns \b Diffuse value.
  const real_t& getDiffuse( ) const ;

  /// Returns \b Diffuse field.
  real_t& getDiffuse( );

  /// Returns \b Diffuse color.
  Color3 getDiffuseColor( ) const ;

  /// Returns \b Specular value.
  const Color3& getSpecular( ) const ;

  /// Returns \b Specular field.
  Color3& getSpecular( );

  /// Returns \b Emission value.
  const Color3& getEmission( ) const;

  /// Returns \b Emission field.
  Color3& getEmission( );

  /// Returns \b Shininess value.
  const real_t& getShininess( ) const ;

  /// Returns \b Shininess field.
  real_t& getShininess( );

  /// Returns \b Transparency value.
  const real_t& getTransparency( ) const ;

  /// Returns \b Transparency field.
  real_t& getTransparency( ) ;

  /// Returns whether \b Ambient field is set to its default value.
  bool isAmbientToDefault( ) const;

  /// Returns whether \b Diffuse field is set to its default value.
  bool isDiffuseToDefault( ) const ;

  /// Returns whether \b Specular field is set to its default value.
  bool isSpecularToDefault( ) const ;

  /// Returns whether \b Emission field is set to its default value.
  bool isEmissionToDefault( ) const;

  /// Returns whether \b Shininess field is set to its default value.
  bool isShininessToDefault( ) const;

  /// Returns whether \b Transparency field is set to its default value.
  bool isTransparencyToDefault( ) const;

  /// Returns whether \e self id valid.
  virtual bool isValid( ) const;

  bool isSimilar(const Material&) const;

protected:

  /// The \b Ambient field.
  Color3 __ambient;

  /// The \b Diffuse field.
  real_t __diffuse;

  /// The \b Specular field.
  Color3 __specular;

  /// The \b Emission field.
  Color3 __emission;

  /// The \b Shininess field.
  real_t __shininess;

  /// The \b Transparency field.
  real_t __transparency;

};

/// Material Pointer
typedef RCPtr<Material> MaterialPtr;

/* ----------------------------------------------------------------------- */

// __appe_material_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

