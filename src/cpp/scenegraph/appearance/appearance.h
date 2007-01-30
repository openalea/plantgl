/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
 *
 *       $Source$
 *       $Id$
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


/*! \file appe_appearance.h
    \brief Definition of the appearance class Appearance.
*/


#ifndef __appe_appearance_h__
#define __appe_appearance_h__


#include <scenegraph/core/sceneobject.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


class Appearance;
typedef RCPtr<Appearance> AppearancePtr;
// typedef SymbolTable<AppearancePtr> AppearanceSymbolTable;
class AppearanceSymbolTable;


/* ------------------------------------------------------------------------- */

/**
    \class Appearance
    \brief Abstract base class for all appearance objects.
*/


class SG_API Appearance : public SceneObject
{

public:

  /// A structure which helps to build a Appearance when parsing.
  struct SG_API Builder : public SceneObject::Builder {

    /// A constructor. It inits all the pointers to null.
    Builder( );

    /// A destructor.
    virtual ~Builder( );

    /** Returns a pointer to a Geometry constructed using the values
        contained within \e self. If \e self is not valid, it returns a
        null pointer. */
    virtual SceneObjectPtr build( ) const = 0;

  };


  /// Default constructor.
  Appearance( ) ;

  /// Destructor
  virtual ~Appearance( );

  virtual bool isTexture() const;

}; // class Appearance

/// Appearance Pointer
typedef RCPtr<Appearance> AppearancePtr;


/* ------------------------------------------------------------------------- */

// __appe_appearance_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/// Write Appearance \b a in \b stream
SG_API std::ostream& operator<<( std::ostream& stream, PGL(Appearance&) a );

/* ----------------------------------------------------------------------- */
#endif

