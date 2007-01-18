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


#include "scne_sceneobject.h"

/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

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


class GEOM_API Appearance : public SceneObject
{

public:

  /// A structure which helps to build a Appearance when parsing.
  struct Builder : public SceneObject::Builder {

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

  /// Prints \e a to the output stream \e stream.
//  friend ostream& operator<<( ostream& stream, Appearance& a );

  /** Parses the input stream \e inputStream and returns the corresponding
      symbol table \e symbolTable.
     \pre
      - \e input must be valid;
      - \e output must be valid. */
  static bool parse( std::istream& inputStream,
                     std::ostream& outputStream,
                     AppearanceSymbolTable& symboltTable,
                     int max_errors = -1 );

  /** Parses the file named \e filename and returns the corresponding symbol
      table \e symbolTable.
     \pre
      - \e fileName must exist;
      - \e outputStream must be valid. */
  static bool parse( const std::string& fileName,
                     std::ostream& outputStream,
                     AppearanceSymbolTable& symbolTable,
                     int max_errors = -1 );

  virtual bool isTexture() const;

}; // class Appearance

/// Appearance Pointer
typedef RCPtr<Appearance> AppearancePtr;


/* ------------------------------------------------------------------------- */

// __appe_appearance_h__
/* ----------------------------------------------------------------------- */

GEOM_END_NAMESPACE

/// Write Appearance \b a in \b stream
GEOM_API std::ostream& operator<<( std::ostream& stream, GEOM(Appearance&) a );

/* ----------------------------------------------------------------------- */
#endif

