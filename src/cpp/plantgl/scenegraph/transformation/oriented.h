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



/*! \file geom_oriented.h
    \brief Definition of the transformation class BaseOrientation
    and the transformed class Oriented.
*/


#ifndef __geom_oriented_h__
#define __geom_oriented_h__

/* ----------------------------------------------------------------------- */

#include "orthotransformed.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class BaseOrientation
    \brief An Orthonormal transformation.
*/

/* ----------------------------------------------------------------------- */

class SG_API BaseOrientation : public OrthonormalBasis3D
{

public:

  /// A structure which helps to build an BaseOrientation when parsing.
  struct SG_API Builder : public OrthonormalBasis3D::Builder
    {

    /// A pointer to the \b Primary field.
    Vector3 * Primary;

     /// A pointer to the \b Secondary field.
    Vector3 * Secondary;

    /// Constructor.
    Builder( );

    /// Desructor.
    virtual ~Builder( );

    virtual OrthonormalBasis3DPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;
    };

  /** Constructs an Orientation Base with the directions \e primary
      and \e secondary.
      \warning
      - \e primary must be normalized;
      - \e secondary must be normalized and orthogonal to \e primary;
  */
  BaseOrientation( const Vector3& primary,
                   const Vector3& secondary ) ;

  /// Destructor.
  virtual ~BaseOrientation( );

protected:

  /// The \b Primary field.
  Vector3 __primary;

  /// The \b Secondary field.
  Vector3 __secondary;
};

/// BaseOrientation Pointer
typedef RCPtr<BaseOrientation> BaseOrientationPtr;

/* ----------------------------------------------------------------------- */


/**
    \class Oriented
    \brief A geometry object transformed by change of coordinates using
    orthonormal basis.
*/


class SG_API Oriented : public OrthoTransformed
{

public:

  /// The \b Primary value.
  static const Vector3 DEFAULT_PRIMARY;

  /// The \b Secondary value.
  static const Vector3 DEFAULT_SECONDARY;


  /// A structure which helps to build a Oriented when parsing.
  struct SG_API Builder : public OrthoTransformed::Builder {

    /// A pointer to the \b Primary field.
    Vector3 * Primary;

     /// A pointer to the \b Secondary field.
    Vector3 * Secondary;

    /// Constructor.
    Builder( );

    /// Desructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  Oriented();

  /** Constructs a Oriented \e goemetry with the directions \e primary
      and \e secondary.
      \warning
      - \e primary must be normalized;
      - \e secondary must be normalized and orthogonal to \e primary;
      - \e geometry must be non null. */
  Oriented( const Vector3& primary,
            const Vector3& secondary,
            const GeometryPtr& geometry );

  /// Destructor
  virtual ~Oriented( );

  PGL_OBJECT(Oriented)

  /// Returns \b Primary value.
  const Vector3& getPrimary( ) const;

  /// Returns \b Primary field.
  Vector3& getPrimary( );

  /// Returns \b Secondary value.
  const Vector3& getSecondary( ) const ;

  /// Returns \b Secondary field.
  Vector3& getSecondary( ) ;

  virtual Transformation3DPtr getTransformation( ) const;

  virtual OrthonormalBasis3DPtr getOrthonormalBasis() const;


  /// Returns whether \b Primary is set to its default first
  bool isPrimaryToDefault( ) const ;

  /// Returns whether \b Secondary is set to its default value.
  bool isSecondaryToDefault( ) const;

  virtual bool isValid( ) const;



protected:

  /// The \b Primary field.
  Vector3 __primary;

  /// The \b Secondary field.
  Vector3 __secondary;

};

/// Oriented Pointer
typedef RCPtr<Oriented> OrientedPtr;


/* ----------------------------------------------------------------------- */

// __geom_oriented_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

