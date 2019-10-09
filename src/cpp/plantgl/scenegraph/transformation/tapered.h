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

  PGL_OBJECT(Tapered)

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

