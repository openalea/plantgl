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
/*! \file dtafile.h
    \brief Definition of the parsing class Dtafile.
*/

#ifndef __dtafile_h__
#define __dtafile_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/tool/rcobject.h>
#include <vector>
#include <string>

/* ----------------------------------------------------------------------- */

#include "codec_config.h"
PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Scene;
typedef RCPtr<Scene> ScenePtr;
class Shape;
typedef RCPtr<Shape> ShapePtr;
class Material;
typedef RCPtr<Material> MaterialPtr;
class AmapSymbol;
typedef RCPtr<AmapSymbol> AmapSymbolPtr;


/* ----------------------------------------------------------------------- */


/**
   \class Dtafile
   \brief A Dta File
*/

class CODEC_API Dtafile
{

public:


  /** Constructs an Dtafile with the datas stored in the file specified
      by \e fileName.
      \pre
      - \e fileName must exist and must be readable. */
  Dtafile( const std::string& fileName,  const std::string& __symbol_path );

  /// Destructor
  virtual ~Dtafile( );

  /// Returns \b FileName value.
  const std::string& getFileName( ) const ;

  /// Return the scene.
  const ScenePtr& getScene() const ;

  /// Return the number of element contains in the dta file.
  const unsigned int getSize() const;

  /// Get the symbol associate to the identifier id.
  virtual AmapSymbolPtr getSymbol(uint32_t id) const ;

  /// Get the Material associate to the identifier id.
  virtual MaterialPtr getMaterial(uint32_t id) const ;

  /// Get the Dta Info associate to the identifier id encapsulate in a Shape.
  virtual const ShapePtr getdtainfo(uint32_t id) const ;

  /// Test object validity.
  virtual bool isValid( ) const;

  /// Read the symbol table in the mtg file wrote by amapsim.
  virtual bool readSymbolTable(const std::string & mtg_filename);

  /// Write AML code on \e stream to associate entity with Geom
  virtual std::ostream& writeAMLCode(std::ostream& stream);

  /*!
    Return the symbol number associate to id if a symbol table has been associate.
    Return -1 else;
  */
  virtual int getSymbolNumber(const int id) const;

//  virtual bofstream& write( bofstream& stream ) const;


  protected:

  /// The \b FileName field.
  std::string _fileName;

  /// The path to the symbol describe in the dta file.
  std::string _symbol_path;

  /// The Scene parsed.
  ScenePtr _scene;

  /// The symbol table linearity field.
  bool linear;

  /// The symbol table field.
  std::vector< std::pair<int,int> > * symbolTable;


};

// typedef RCPtr<Dtafile> DtafilePtr;


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
// __dtafile_h__
#endif

