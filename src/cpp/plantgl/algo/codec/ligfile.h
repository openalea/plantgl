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

PGL_BEGIN_NAMESPACE
class bifstream;
class bofstream;
PGL_END_NAMESPACE

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
                   const long range  , const Vector3 dirp , const Vector3 dirs,
                   const Vector3 dirt, const Vector3 origin ,
                   const float base_dia, const float sommit_dia , const long entity_number);
    
    void setValues(const Vector3 dirp , const Vector3 dirs,
                   const Vector3 dirt, const Vector3 origin ,
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
    bool read( bifstream& stream);

    /// Read \e l in the input stream \e stream.
    friend CODEC_API bifstream& operator>>( bifstream& stream, LigRecord& l);

    /// Prints \e l to the output stream \e stream.
    friend CODEC_API bofstream& operator<<( bofstream& stream, const LigRecord& l );

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

CODEC_API bifstream& operator>>( bifstream& stream, LigRecord& l);
CODEC_API bofstream& operator<<( bofstream& stream, const LigRecord& l );

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
