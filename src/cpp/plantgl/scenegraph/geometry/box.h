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


/*! \file geom_box.h
    \brief Definition of the geometry class Box.
*/



#ifndef __geom_box_h__
#define __geom_box_h__

/* ----------------------------------------------------------------------- */

#include "parametricmodel.h"
#include <plantgl/math/util_vector.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class Box
    \brief A box
*/

/* ----------------------------------------------------------------------- */

class SG_API Box : public ParametricModel
{

public:

  /// A structure which helps to build a Box when parsing.
  struct SG_API Builder : public ParametricModel::Builder {

    /// A pointer to the \b Size field.
    Vector3 * Size;

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };


  /// The default value fot the Size field.
  static const Vector3 DEFAULT_SIZE;


  /// Constructs a Box with \e size.
  Box( const Vector3& size = DEFAULT_SIZE );

  /// Destructor
  virtual ~Box( );

  PGL_OBJECT(Box)

  /// Returns the back lower left corner.
  Vector3 getBackLowerLeftCorner( ) const;

  /// Returns the back upper left corner.
  Vector3 getBackLowerRightCorner( ) const;

  /// Returns the normal to the back side of \e self.
  Vector3 getBackNormal( ) const;

  /// Returns the back upper left corner.
  Vector3 getBackUpperLeftCorner( ) const;

  /// Returns the back upper left corner.
  Vector3 getBackUpperRightCorner( ) const;

  /// Returns the front lower left corner.
  Vector3 getFrontLowerLeftCorner( ) const;

  /// Returns the front lower left corner.
  Vector3 getFrontLowerRightCorner( ) const;

  /// Returns the back upper left corner.
  Vector3 getFrontUpperLeftCorner( ) const ;

  /// Returns the back upper left corner.
  Vector3 getFrontUpperRightCorner( ) const;

  /// Returns the normal to the front side of \e self.
  Vector3 getFrontNormal( ) const;

  /// Returns the normal to the left side of \e self.
  Vector3 getLeftNormal( ) const ;

  /// Returns the normal to the lower side of \e self.
  Vector3 getLowerNormal( ) const;

  /// Returns the normal to the right side of \e self.
  Vector3 getRightNormal( ) const;

  /// Returns the normal to the upper side of \e self.
  Vector3 getUpperNormal( ) const;

  /// Returns Size value.
  const Vector3& getSize( ) const;

  /// Returns Size field.
  Vector3& getSize( );

  virtual bool isACurve( ) const;

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const;

  /// Returns whether Size is set to its default value.
  bool isSizeToDefault( ) const ;

  virtual bool isValid( ) const;

protected:

  /// The Size field.
  Vector3 __size;

};

/// Box Pointer
typedef RCPtr<Box> BoxPtr;


/* ----------------------------------------------------------------------- */

// __geom_box_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

