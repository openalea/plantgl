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
/*! \file ligfile.h
    \brief Definition of the parsing class Ligfile.
*/


#ifndef __ligfile_h__
#define __ligfile_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/tool/rcobject.h>
#include <plantgl/math/util_vector.h>
#include <string>
#include <vector>

/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE
class bifstream;
class bofstream;
TOOLS_END_NAMESPACE

/* ----------------------------------------------------------------------- */

#include "codec_config.h"

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Scene;
typedef RCPtr<Scene> ScenePtr;
class Geometry;
typedef RCPtr<Geometry> GeometryPtr;
class Dtafile;
class Discretizer;

/* ----------------------------------------------------------------------- */

/**
   \class LigRecord
   \brief A Record of a Linetree file in the AMAP format.
*/

/* ----------------------------------------------------------------------- */

class CODEC_API LigRecord {

    public :

        /// Construtor
        LigRecord();

    /// Destructor
    ~LigRecord();

    /// Set values to \e this.
    void setValues(const long symbol , const long val1 , const long val2,
				   const long range  , const TOOLS(Vector3) dirp , const TOOLS(Vector3) dirs,
				   const TOOLS(Vector3) dirt, const TOOLS(Vector3) origin ,
				   const float base_dia, const float sommit_dia , const long entity_number);
    
    void setValues(const TOOLS(Vector3) dirp , const TOOLS(Vector3) dirs,
				   const TOOLS(Vector3) dirt, const TOOLS(Vector3) origin ,
				   const float base_dia, const float sommit_dia );

    /// Compute the transformation represented by its values.
    GeometryPtr getTransformed(GeometryPtr primitive) const;

    /// Compute the transformation represented by its values.
    GeometryPtr getTransformed(GeometryPtr primitive, Discretizer& discretizer) const;

    /// Return the id if the symbol.
    const long& getSymbolNumber() const;

     /// Return the id if the entity.
    const long& getEntityNumber() const;

    /// read values on \e stream.
    bool read( TOOLS(bifstream)& stream);

    /// Read \e l in the input stream \e stream.
    friend CODEC_API TOOLS(bifstream)& operator>>( TOOLS(bifstream)& stream, LigRecord& l);

    /// Prints \e l to the output stream \e stream.
    friend CODEC_API TOOLS(bofstream)& operator<<( TOOLS(bofstream)& stream, const LigRecord& l );

//    protected :

  long  __symbol;
  long  __val1;
  long  __val2;
  long  __range;
  float __matrix[3][4];
  float __base_dia;
  float __sommit_dia;
  long  __entity_number;
};

CODEC_API TOOLS(bifstream)& operator>>( TOOLS(bifstream)& stream, LigRecord& l);
CODEC_API TOOLS(bofstream)& operator<<( TOOLS(bofstream)& stream, const LigRecord& l );

/* ----------------------------------------------------------------------- */


/**
   \class Ligfile
   \brief A File contening Linetree Information in the AMAP format.
*/
class CODEC_API Ligfile {

public:


  /** Constructs an Ligfile with the datas stored in the file specified
      by \e fileName.
      \pre
      - \e fileName must exist and must be readable. */
  Ligfile( const std::string& fileName, bool bigendian = true);

  /// Destructor
  virtual ~Ligfile( );

  /// Returns \b FileName value.
  const std::string& getFileName( ) const;

  /// Returns \b the number of elements.
  const unsigned int getSize( ) const;

  /// Return the scene.
  ScenePtr computeScene(const Dtafile& _dtafile) const;


  /// Test object validity.
  virtual bool isValid( ) const;

  static bool isBigEndian( const std::string& fileName );

  protected:

  /// The \b FileName field.
  std::string _fileName;

  /// The record table field.
  std::vector< LigRecord > * recordTable;


};

// typedef RCPtr<Ligfile> LigfilePtr;


/* ----------------------------------------------------------------------- */

/// Read a Linetree from files \e ligFile, \e dtaFile and \e smbpath. Error output will be made on \e output.
ScenePtr CODEC_API readLineTree(std::string ligFile, std::string dtaFile, std::string smbpath, bool bigendian = true,std::ostream& output = std::cerr);


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
 // __ligfile_h__
