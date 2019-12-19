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


/*! \file geom_extrudedhull.h
    \brief Definition of the geometry class ExtrudedHull.
*/


#ifndef __geom_extrudedhull_h__
#define __geom_extrudedhull_h__

/* ----------------------------------------------------------------------- */

#include "hull.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Curve2D;
typedef RCPtr<Curve2D>  Curve2DPtr;

/* ----------------------------------------------------------------------- */

/**
   \class ExtrudedHull
   \brief A hull extruded by a vertical and an horizontal profiles.
*/

class SG_API ExtrudedHull : public Hull
{

public:

  /// The \b CCW field default value.
  static const bool DEFAULT_CCW;

  /// A structure which helps to build a ExtrudedHull when parsing.
  struct SG_API Builder : public Hull::Builder {

    /// A pointer to the \b CCW field.
    bool * CCW;

    /// A pointer to the \b Horizontal field.
    Curve2DPtr * Horizontal;

    /// A pointer to the \b Orientation field.
    // real_t * Orientation;

    /// A pointer to the \b Vertical field.
    Curve2DPtr * Vertical;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  ExtrudedHull();

  /** Constructs a ExtrudedHull with the profiles \e horizontal and
      \e vertical.
      \pre
      - \e horizontal must contain at leat 2 points;
      - \e vertival must contain at leat 2 points.
      \post
      - \e self is valid. */
  ExtrudedHull(  const Curve2DPtr& vertical,
         const Curve2DPtr& horizontal,
         bool ccw = DEFAULT_CCW);

  /// Destructor
  virtual ~ExtrudedHull( );

  PGL_OBJECT(ExtrudedHull)

  /// Returns \b CCW value.
  const bool getCCW( ) const;

  /// Returns \b CCW field.
  bool& getCCW( );

  /// Returns \b Horizontal value.
  const Curve2DPtr& getHorizontal( ) const;

  /// Returns \b Horizontal field.
  Curve2DPtr& getHorizontal( );

  /// Returns \b Vertical value.
  const Curve2DPtr& getVertical( ) const;

  /// Returns \b Vertical field.
  Curve2DPtr& getVertical( ) ;

  /// Returns whether \b CCW is set to its default value.
  bool isCCWToDefault( ) const;

  virtual bool isValid( ) const;

protected:

  /// The \b Vertical field.
  Curve2DPtr __vertical;

  /// The \b Horizontal field.
  Curve2DPtr __horizontal;

  /// The \b CCW field.
  bool __ccw;

}; // ExtrudedHull

/// ExtrudedHull Pointer
typedef RCPtr<ExtrudedHull> ExtrudedHullPtr;


/* ----------------------------------------------------------------------- */

// __geom_extrudedhull_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

