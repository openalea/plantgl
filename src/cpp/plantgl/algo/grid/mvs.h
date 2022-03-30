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


/*! \file mvs_mvs.h
    \brief Definition of Mvs.
*/



#ifndef __mvs_mvs_h__
#define __mvs_mvs_h__

/* ----------------------------------------------------------------------- */

#include "../algo_config.h"
#include <plantgl/tool/rcobject.h>
#include <plantgl/scenegraph/scene/scene.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
    \class Mvs
    \brief base class for \b Multiscale Voxel Space.
*/

/* ----------------------------------------------------------------------- */

class ALGO_API Mvs : public RefCountObject
{

public:

  /** Default constructor. */
  Mvs();

  /// Destructor
  virtual ~Mvs( );

  ///  Set the scene \e scene to \e self.
  virtual bool setScene( const ScenePtr&  scene) = 0;

  virtual const ScenePtr& getScene() const{ return __scene; }

  /// Returns the name of \e self.
  const std::string& getName( ) const {
    return __name;
  }

  /// Returns whether \e self is named.
  bool isNamed( ) const {
    return ! __name.empty();
  }

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const  = 0;

   /// Sets the name of \e self.
  void setName( const std::string& name )
  { __name = name; }


protected:

  /// Self's name
  std::string __name;

  /// Scene in the mvs.
  ScenePtr __scene;

}; // class Mvs

/// Mvs Pointer
typedef RCPtr<Mvs> MvsPtr;


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
// __mvs_mvs_h__
#endif
