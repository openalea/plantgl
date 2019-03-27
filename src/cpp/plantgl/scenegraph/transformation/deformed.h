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


/*! \file geom_deformed.h
    \brief Definition of the transformation class Deformation and the transformed class Deformed.
*/


#ifndef __geom_deformed_h__
#define __geom_deformed_h__

/* ----------------------------------------------------------------------- */

#include "transformed.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Primitive;
typedef RCPtr<Primitive> PrimitivePtr;

/* ----------------------------------------------------------------------- */

/**
    \class Deformation
    \brief Abstract base class for axial deformations.
*/

class SG_API Deformation : public Transformation3D
{

public:

  /// Constructs a Deformation.
  Deformation( );

  /// Destructor.
  virtual ~Deformation( );

};

/// Deformation Pointer
typedef RCPtr<Deformation> DeformationPtr;


/* ----------------------------------------------------------------------- */


/**
    \class Deformed
    \brief A deformed
*/


class SG_API Deformed : public Transformed
{

public:

   /// A structure which helps to build an object of type of Transformed.
  struct SG_API Builder : public Transformed::Builder {

    /// Constructor
    Builder( );

    /// A pointer to the \b Primitive field.
    PrimitivePtr * Primitive;

    virtual void destroy( );

    virtual bool isValid( ) const;

    void DefDestroy( );

    bool DefValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  Deformed();

  /// Constructor.
  Deformed( const PrimitivePtr& primitive );

  /// Destructor.
  virtual ~Deformed( );

  virtual const GeometryPtr getGeometry( ) const;

  /** Returns \b Primitive value.*/
  const PrimitivePtr& getPrimitive( ) const;

  /** Returns \b Primitive field.*/
  PrimitivePtr& getPrimitive( );

  virtual bool isACurve( ) const;

  virtual bool isASurface( ) const ;

  virtual bool isAVolume( ) const ;

  virtual bool isExplicit( ) const;

protected:

  /// The \b Primitive field.
  PrimitivePtr __primitive;

};

/// Deformed Pointer
typedef RCPtr<Deformed> DeformedPtr;


/* ----------------------------------------------------------------------- */

// __geom_deformed_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

