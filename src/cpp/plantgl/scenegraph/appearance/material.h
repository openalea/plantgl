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

  virtual bool isSimilar(const Material&) const;


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

SG_API MaterialPtr interpolate(const MaterialPtr m1, const MaterialPtr m2, real_t t = 0.5);

/* ----------------------------------------------------------------------- */

// __appe_material_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

