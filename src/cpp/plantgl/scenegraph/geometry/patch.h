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



/*! \file geom_patch.h
    \brief Definition of the geometry class Patch.
*/


#ifndef __geom_patch_h__
#define __geom_patch_h__

/* ----------------------------------------------------------------------- */

#include "parametricmodel.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Point4Matrix;
typedef RCPtr<Point4Matrix> Point4MatrixPtr;

/* ----------------------------------------------------------------------- */

/**
    \class Patch
    \brief Abstract base class for objects of type of 3D parametric patch.
*/

/* ----------------------------------------------------------------------- */

class SG_API Patch : public ParametricModel
{

public:

  static const bool DEFAULT_CCW;

  /// A structure which helps to build an object of type of Patch.
  struct SG_API Builder : public ParametricModel::Builder {

    /// A pointer to the \b CCW field.
    bool * CCW;

    /// Constructor.
    Builder( );

    /// Destructor.
    ~Builder( );

  };


  /// Constructor.
  Patch(bool ccw = DEFAULT_CCW);

  /// Destructor
  virtual ~Patch( ) ;

  /// Returns \e Control Points value.
  virtual Point4MatrixPtr getCtrlPoints( ) const = 0 ;


  virtual bool isACurve( ) const;

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const;

  /// Returns \e CCW value.
  const bool getCCW( ) const;

  /// Returns \e CCW field.
  bool& getCCW( );

  /// Returns whether \e CCW is set to its default value.
  bool isCCWToDefault( ) const;

protected:

  /// the ccw field
  bool __ccw;

};

/// Patch Pointer
typedef RCPtr<Patch> PatchPtr;


/* ----------------------------------------------------------------------- */

// __geom_patch_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

