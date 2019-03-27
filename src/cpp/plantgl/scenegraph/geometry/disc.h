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



/*! \file geom_disc.h
    \brief Definition of the geometry class Disc.
*/


#ifndef __geom_disc_h__
#define __geom_disc_h__

/* ----------------------------------------------------------------------- */

#include "sor.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class Disc
   \brief A 2D disc.
*/

/* ----------------------------------------------------------------------- */

class SG_API Disc : public SOR2D
{

public:

  /// The \b Radius field default value.
  static const real_t DEFAULT_RADIUS;


  /// A structure which helps to build a Disc when parsing.
  struct SG_API Builder : public SOR2D::Builder {

    /// A pointer to the \b Radius field.
    real_t * Radius;

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };


  /// Defautl constructor.
  Disc( const real_t& radius = DEFAULT_RADIUS,
    uchar_t Slices = SOR::DEFAULT_SLICES);

  /// Destructor
  virtual ~Disc( );

  PGL_OBJECT(Disc)

  /// Returns \b Radius value.
  const real_t& getRadius( ) const;

  /// Returns \b Radius field.
  real_t& getRadius( );

  virtual bool isAVolume( ) const;

  /// Returns whether \b Radius is set to its default value.
  bool isRadiusToDefault( ) const ;

  virtual bool isValid( ) const;


protected:

  /// The Radius field.
  real_t __radius;

};

/// Disc Pointer
typedef RCPtr<Disc> DiscPtr;


/* ----------------------------------------------------------------------- */

// __geom_disc_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

