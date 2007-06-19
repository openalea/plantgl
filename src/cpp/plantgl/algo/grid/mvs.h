/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
 *
 *       $Source$
 *       $Id: mvs_mvs.h 1229 2003-08-27 05:23:24Z pradal $
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
 *
 *  ----------------------------------------------------------------------------
 *
 *                      GNU General Public Licence
 *
 *       This program is free software; you can redistribute it and/or
 *       modify it under the terms of the GNU General Public License as
 *       published by the Free Software Foundation; either version 2 of
 *       the License, or (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public
 *       License along with this program; see the file COPYING. If not,
 *       write to the Free Software Foundation, Inc., 59
 *       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
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

class ALGO_API Mvs : public TOOLS(RefCountObject)
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
