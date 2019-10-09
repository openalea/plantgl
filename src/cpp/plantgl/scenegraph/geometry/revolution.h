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



/*! \file geom_revolution.h
    \brief Definition of the geometry class Revolution.
*/


#ifndef __geom_revolution_h__
#define __geom_revolution_h__

/* ----------------------------------------------------------------------- */

#include "sor.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Curve2D;
typedef RCPtr<Curve2D> Curve2DPtr;

/* ----------------------------------------------------------------------- */

/**
   \class Revolution
   \brief A revolution represented by a profile.
*/

/* ----------------------------------------------------------------------- */

class SG_API Revolution : public SOR
{

public:

  /// A structure which helps to build a Revolution when parsing.
  struct SG_API Builder : public SOR::Builder {

    /// A pointer to the PointList field.
    Curve2DPtr * Profile;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  Revolution();

  /// Constructs a Revolution from the 2D polyline \e points.
  Revolution( const Curve2DPtr& profile,
          uchar_t slices = DEFAULT_SLICES  );

  /// Destructor
  virtual ~Revolution( );

  PGL_OBJECT(Revolution)

  /// Returns PointList values.
  const Curve2DPtr& getProfile( ) const ;

  /// Returns PointList field.
  Curve2DPtr& getProfile( );

  virtual bool isAVolume( ) const;

  virtual bool isValid( ) const;

protected:

  /// The PointList field.
  Curve2DPtr __profile;

}; // Revolution

/// Revolution Pointer
typedef RCPtr<Revolution> RevolutionPtr;


/* ----------------------------------------------------------------------- */

// __geom_revolution_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

