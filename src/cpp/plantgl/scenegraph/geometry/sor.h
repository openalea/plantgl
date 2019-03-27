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



/*! \file geom_sor.h
    \brief Definition of the geometry class SOR and SOR2D.
*/


#ifndef __geom_sor_h__
#define __geom_sor_h__

/* ----------------------------------------------------------------------- */

#include "parametricmodel.h"
#include "planarmodel.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class SOR
    \brief Abstract base class for 3D objects of type of surface of revolution.
*/

/* ----------------------------------------------------------------------- */

class SG_API SOR : public ParametricModel
{

public:

  /// The \b Slices field default value.
  static const uchar_t DEFAULT_SLICES;

  /// A structure which helps to build an object of type of SOR.
  struct SG_API Builder : public ParametricModel::Builder {

    /// A pointer to the \b Slices field.
    uchar_t * Slices;

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

    virtual void destroy( );

    void SORDestroy( );

    virtual bool isValid( ) const;

    bool SORValid( ) const;

  };


  /// Constructor.
  SOR( uchar_t slices = DEFAULT_SLICES );

  /// Destructor
  virtual ~SOR( );

  /// Returns \b Slices value.
  const uchar_t& getSlices( ) const ;

  /// Returns \b Slices field.
  uchar_t& getSlices( );

  virtual bool isACurve( ) const ;

  virtual bool isASurface( ) const ;

  /// Returns whether \b Slices is set to its default value.
  bool isSlicesToDefault( ) const ;


protected:

  /// The \b Slices field.
  uchar_t __slices;

};

/// SOR Pointer
typedef RCPtr<SOR> SORPtr;


/* ----------------------------------------------------------------------- */

/**
    \class SOR2D
    \brief Abstract base class for 2D objects of type of surface of revolution.
*/

class SG_API SOR2D : public PlanarModel
{

public:

  /// A structure which helps to build an object of type of SOR2D.
  struct Builder : public PlanarModel::Builder {

    /// A pointer to the \b Slices field.
    uchar_t * Slices;

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

    virtual void destroy( );

    virtual bool isValid( ) const;

    void SOR2DDestroy( );

    bool SOR2DValid( ) const;

  };


  /// Constructor.
  SOR2D( uchar_t slices = SOR::DEFAULT_SLICES );

  /// Destructor
  virtual ~SOR2D( );

  /// Returns \b Slices value.
  const uchar_t& getSlices( ) const ;

  /// Returns \b Slices field .
  uchar_t& getSlices( ) ;

  virtual bool isACurve( ) const ;

  virtual bool isASurface( ) const ;

  /// Returns whether \b Slices is set to its default value.
  bool isSlicesToDefault( ) const ;


protected:

  /// The \b Slices field.
  uchar_t __slices;

};

/// SOR2D Pointer
typedef RCPtr<SOR2D> SOR2DPtr;


/* ----------------------------------------------------------------------- */

// __geom_sor_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

