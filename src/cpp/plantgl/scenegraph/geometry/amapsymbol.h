/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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

/*! \file geom_amapsymbol.h
    \brief Definition of the geometry class AmapSymbol.
*/



#ifndef __geom_amapsymbol_h__
#define __geom_amapsymbol_h__

/* ----------------------------------------------------------------------- */

#include "faceset.h"

/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE
class beifstream;
TOOLS_END_NAMESPACE

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

  virtual TOOLS(bofstream)& write( TOOLS(bofstream)& stream ) const;

  /** Read the datas stored in the file specified
      in stream. */
  virtual TOOLS(beifstream)& read( TOOLS(beifstream)& stream );

#ifndef PGL_NO_DEPRECATED
  attribute_deprecated inline const TOOLS(Vector3)& getTexCoord3At( uint_t i, uint_t j ) const
  { return getFaceTexCoord3At(i,j); }
  attribute_deprecated inline TOOLS(Vector3)& getTexCoord3At( uint_t i, uint_t j )
  { return getFaceTexCoord3At(i,j); }
#endif

  const TOOLS(Vector3)& getFaceTexCoord3At( uint_t i, uint_t j ) const;  
  TOOLS(Vector3)& getFaceTexCoord3At( uint_t i, uint_t j );

  const Point3ArrayPtr& getTexCoord3List() const{
	return __texCoord3List; }

  Point3ArrayPtr& getTexCoord3List() {
	return __texCoord3List; }

  bool hasTexCoord3List() const {
	return (__texCoord3List); }

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

