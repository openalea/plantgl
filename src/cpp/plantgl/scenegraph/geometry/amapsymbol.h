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


/*! \file geom_amapsymbol.h
    \brief Definition of the geometry class AmapSymbol.
*/



#ifndef __geom_amapsymbol_h__
#define __geom_amapsymbol_h__

/* ----------------------------------------------------------------------- */

#include "faceset.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE
class beifstream;
PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class AmapSymbol
   \brief An Amap Symbol
*/

class SG_API AmapSymbol : public FaceSet
{

public:

  /// A structure which helps to build a AmapSymbol when parsing.
  struct SG_API Builder : public FaceSet::Builder {

    /// A pointer to the \b FileName field.
    std::string * FileName;

    /// Constructor.
    Builder( );

    /// Detructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  AmapSymbol();

  /** Constructs an AmapSymbol with the datas stored in the file specified
      by \e fileName. The \e solid flag specifies whether the AmapSymbol is
      closed.
      \pre
      - \e fileName must exist and must be readable. */
  AmapSymbol( const std::string& fileName, bool solid=FaceSet::DEFAULT_SOLID );

  /** Constructs an AmapSymbol with the FaceSet \e faceSet.
      \pre
      - \e faceSet must be non null and valid. */
  AmapSymbol( const FaceSetPtr& faceSet );

  /// Destructor
  virtual ~AmapSymbol( );

  PGL_OBJECT(AmapSymbol)

  /// Returns \b FileName value.
  const std::string& getFileName( ) const ;

  /// Returns \b FileName field.
  std::string& getFileName( ) ;

  virtual bool isValid( ) const;


  /** Fill \e this with the datas stored in the file specified
      by \e fileName. The \e solid flag specifies whether the AmapSymbol is
      closed.
      \pre
      - \e fileName must exist and must be readable. */
  bool readFile(const std::string& filename);

  virtual bofstream& write( bofstream& stream ) const;

  /** Read the datas stored in the file specified
      in stream. */
  virtual beifstream& read( beifstream& stream );

#ifndef PGL_NO_DEPRECATED
  attribute_deprecated inline const Vector3& getTexCoord3At( uint_t i, uint_t j ) const
  { return getFaceTexCoord3At(i,j); }
  
  attribute_deprecated inline Vector3& getTexCoord3At( uint_t i, uint_t j )
  { return getFaceTexCoord3At(i,j); }
#endif

  const Vector3& getFaceTexCoord3At( uint_t i, uint_t j ) const;
  Vector3& getFaceTexCoord3At( uint_t i, uint_t j );

  const Point3ArrayPtr& getTexCoord3List() const 
  { return __texCoord3List; }

  Point3ArrayPtr& getTexCoord3List() 
  { return __texCoord3List; }

  bool hasTexCoord3List() const 
  { return is_valid_ptr(__texCoord3List); }

  protected:

  /// The \b FileName field.
  std::string __fileName;

  Point3ArrayPtr __texCoord3List;
};

/// AmapSymbol Pointer
typedef RCPtr<AmapSymbol> AmapSymbolPtr;


/* ----------------------------------------------------------------------- */

// __geom_amapsymbol_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

