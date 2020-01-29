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
  virtual AmapSymbolPtr getSymbol(uint_t id) const ;

  /// Get the Material associate to the identifier id.
  virtual MaterialPtr getMaterial(uint_t id) const ;

  /// Get the Dta Info associate to the identifier id encapsulate in a Shape.
  virtual const ShapePtr getdtainfo(uint_t id) const ;

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

