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



/*! \file geom_faceset.h
    \brief Definition of the geometry class FaceSet.
*/

#ifndef __geom_faceset_h__
#define __geom_faceset_h__

/* ----------------------------------------------------------------------- */

#include "mesh.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/container/indexarray.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

class TriangleSet;
class QuadSet;

/* ----------------------------------------------------------------------- */

/**
   \class FaceSet
   \brief A faceset
*/

class SG_API FaceSet : public Mesh
{

public:

  /// A structure which helps to build a FaceSet when parsing. 
  struct SG_API Builder : public Mesh::Builder {

    /// A pointer to the IndexList field.
    IndexArrayPtr * IndexList;

    /// A pointer to the NormalIndexList field.
    IndexArrayPtr * NormalIndexList;

    /// A pointer to the ColorIndexList field.
    IndexArrayPtr * ColorIndexList;

    /// A pointer to the TexCoordIndexList field.
    IndexArrayPtr * TexCoordIndexList;

	/// Constructor
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

	void FaceSetDestroy( );

    bool FaceSetValid( ) const;

  };

  /// Default Constructor. Build object is invalid.
  FaceSet();


  /** Constructs a FaceSet with the points \e points, the faces indices 
      \e indices, the face ordering \e ccw, the solid flag \e solid and the 
      skeleton \e skeleton.
      \pre
      - \e points must contain a minimum of 3 points;
      - \e indices must contain a minimum of 1 index. Each index within \e 
      indices 
      must contain a minimum of 3 elements and each element must be unique.
      - \e skeleton must be valid in the case it is non null.
      \post
      - \e self is valid. */
  FaceSet( const Point3ArrayPtr& points,
	       const IndexArrayPtr& indices,
		   bool normalPerVertex = DEFAULT_NORMALPERVERTEX,
		   bool ccw = DEFAULT_CCW,
		   bool solid = DEFAULT_SOLID,
		   const PolylinePtr& skeleton = DEFAULT_SKELETON);
  
  
  /** Constructs a FaceSet with the points \e points, the faces indices 
      \e indices, the face ordering \e ccw, the solid flag \e solid and the 
      the skeleton \e skeleton and the normals \e normals.
      normals don't have to be normalized.
      \pre
      - \e points must contain a minimum of 3 points;
      - \e indices must contain a minimum of 1 index. Each index within \e 
      indices 
      must contain a minimum of 3 elements and each element must be unique.
      - \e skeleton must be valid in the case it is non null.
      - \e normals must be as big as points;
      \post
      - \e self is valid. */
  FaceSet( const Point3ArrayPtr& points,
	       const IndexArrayPtr& indices,
		   const Point3ArrayPtr& normals ,
		   const IndexArrayPtr& nomalIndices = 0,
		   const Color4ArrayPtr& colors  = 0,
		   const IndexArrayPtr& colorIndices = 0,
		   const Point2ArrayPtr& texCoord = 0,
		   const IndexArrayPtr& texCoordIndices = 0,
		   bool normalPerVertex = DEFAULT_NORMALPERVERTEX,
		   bool colorPerVertex = DEFAULT_COLORPERVERTEX,
		   bool CCW = DEFAULT_CCW,
		   bool solid = DEFAULT_SOLID,
		   const PolylinePtr& skeleton = DEFAULT_SKELETON);

  /** Constructs a FaceSet from a TriangleSet.  */
  FaceSet(const TriangleSet&);

  /** Constructs a FaceSet from a TriangleSet.  */
  FaceSet(const QuadSet&);

  /// Destructor
  virtual ~FaceSet( );

  virtual bool apply( Action& action );
  
  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /// Returns \b IndexList value.
  const IndexArrayPtr& getIndexList( ) const;

  /// Returns \b IndexList field.
  IndexArrayPtr& getIndexList( );

  /** Returns the \e i-th index of \b IndexList.
      \pre
      - \e i must be belong to the range [0,size of \b IndexList[. */
  const Index& getIndexListAt( uint32_t i ) const;

  /** Returns the \e i-th index of \b IndexList.
      \pre
      - \e i must be belong to the range [0,size of \b IndexList[. */
  Index& getIndexListAt( uint32_t i );

  /// Returns the size of \b IndexList.
  virtual uint32_t getIndexListSize( ) const;
  
  /// Returns \b NormalIndexList values.
  inline const IndexArrayPtr& getNormalIndexList( ) const { return __normalIndexList; }

  /// Returns \b NormalIndexList field.
  inline IndexArrayPtr& getNormalIndexList( )  { return __normalIndexList; }

  /** Returns the \e i-th value of \b NormalIndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline const Index& getNormalIndexListAt( uint32_t i ) const 
  { GEOM_ASSERT(__normalIndexList.isValid() && i < __normalIndexList->getSize()) return __normalIndexList->getAt(i); }

  /** Returns the \e i-th value of \b IndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline Index& getNormalIndexListAt( uint32_t i )
  { GEOM_ASSERT(__normalIndexList.isValid() && i < __normalIndexList->getSize()) return __normalIndexList->getAt(i); }

  /// Returns the size of \b NormalIndexList.
  inline uint32_t getNormalIndexListSize( ) const { return (__normalIndexList.isValid()?__normalIndexList->getSize():0); }

  /// Returns \b ColorIndexList values.
  inline const IndexArrayPtr& getColorIndexList( ) const { return __colorIndexList; }

  /// Returns \b ColorIndexList field.
  inline IndexArrayPtr& getColorIndexList( )  { return __colorIndexList; }

  /** Returns the \e i-th value of \b ColorIndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline const Index& getColorIndexListAt( uint32_t i ) const 
  { GEOM_ASSERT(__colorIndexList.isValid() && i < __colorIndexList->getSize()) return __colorIndexList->getAt(i); }

  /** Returns the \e i-th value of \b IndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline Index& getColorIndexListAt( uint32_t i )
  { GEOM_ASSERT(__colorIndexList.isValid() && i < __colorIndexList->getSize()) return __colorIndexList->getAt(i); }

  /// Returns the size of \b ColorIndexList.
  inline uint32_t getColorIndexListSize( ) const { return (__colorIndexList.isValid()?__colorIndexList->getSize():0); }

  /// Returns \b TexCoordIndexList values.
  inline const IndexArrayPtr& getTexCoordIndexList( ) const { return __texCoordIndexList; }

  /// Returns \b TexCoordIndexList field.
  inline IndexArrayPtr& getTexCoordIndexList( )  { return __texCoordIndexList; }

  /** Returns the \e i-th value of \b TexCoordIndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline const Index& getTexCoordIndexListAt( uint32_t i ) const 
  { GEOM_ASSERT(__texCoordIndexList.isValid() && i < __texCoordIndexList->getSize()) return __texCoordIndexList->getAt(i); }

  /** Returns the \e i-th value of \b IndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline Index& getTexCoordIndexListAt( uint32_t i )
  { GEOM_ASSERT(__texCoordIndexList.isValid() && i < __texCoordIndexList->getSize()) return __texCoordIndexList->getAt(i); }

  /// Returns the size of \b TexCoordIndexList.
  inline uint32_t getTexCoordIndexListSize( ) const { return (__texCoordIndexList.isValid()?__texCoordIndexList->getSize():0); }

  /** Returns the \e i-th normal to (normally correxponding to the \e i-th face).
      \pre
	  - \e NormalList should be set
      - \e i must be belong to the range [0,size of \b NormalList[. */
  const TOOLS(Vector3)& getNormalAt( uint32_t i )  const ;
  
  /** Returns the normal at the \e j-th point of the \e i-th face.
      \pre
      - \e i must be belong to the range [0,size of \b IndexList[. 
      - \e j must belong to the range [0,2]. */
  const TOOLS(Vector3)& getNormalAt( uint32_t i, uint32_t j ) const ;

  /** Returns the \e j-th point of the \e i-th face.
      \warning
	  - \e PointList should be set
      - \e i must belong to the range [0,size of \b IndexList[;
      - \e j must belong to the range [0,2]. */
  const TOOLS(Vector3)& getFacePointAt( uint32_t i, uint32_t j ) const ;
    
  /** Returns the \e j-th texture coordinates of the \e i-th face.
      \warning
	  - \e TexCoordList should be set
      - \e i must be belong to the range [0,size of \b TexCoordList[. */
  const TOOLS(Vector2)& getTexCoordAt( uint32_t i ) const ;

  /** Returns the \e j-th texture coordinates of the \e i-th face.
      \warning
	  - \e TexCoordList should be set
      - \e i must belong to the range [0,size of \b IndexList[;
      - \e j must belong to the range [0,2]. */
  const TOOLS(Vector2)& getTexCoordAt( uint32_t i, uint32_t j ) const ;
    
  /** Returns the \e j-th colors of the \e i-th face.
      \warning
      - \e i must be belong to the range [0,size of \b ColorList[. */
  const Color4& getColorAt( uint32_t i ) const ;
    
  /** Returns the \e j-th colors of the \e i-th face.
      \warning
      - \e i must belong to the range [0,size of \b IndexList[;
      - \e j must belong to the range [0,2]. */
  const Color4& getColorAt( uint32_t i, uint32_t j ) const ;

  virtual bool isValid( ) const;

  virtual ExplicitModelPtr
  transform( const Transformation3DPtr& transformation ) const;

  // virtual TOOLS(bofstream)& write( TOOLS(bofstream)& stream ) const;

  virtual Point3ArrayPtr computeNormalPerVertex() const ;
  virtual Point3ArrayPtr computeNormalPerFace() const ;

   /// Returns whether \b NormalIndexList is set to its default value.
  inline bool isNormalIndexListToDefault() const { return __normalIndexList.isNull(); }

   /// Returns whether \b ColorIndexList is set to its default value.
  inline bool isColorIndexListToDefault() const { return __colorIndexList.isNull(); }

   /// Returns whether \b TexCoordIndexList is set to its default value.
  inline bool isTexCoordIndexListToDefault() const { return __texCoordIndexList.isNull(); }

  /// Returns the nb of points of the \b i-th face.
  virtual uint32_t getFaceSize( uint32_t i ) const { return __indexList->getAt(i).getSize(); }

  protected:

  /// The \b IndexList field.
  IndexArrayPtr __indexList;

  /// The \b NormalIndexList field.
  IndexArrayPtr __normalIndexList;

  /// The \b TexCoordIndexList field.
  IndexArrayPtr __texCoordIndexList;

  /// The \b ColorIndexList field.
  IndexArrayPtr __colorIndexList;

};

/// FaceSet Pointer
typedef RCPtr<FaceSet> FaceSetPtr;


/* ----------------------------------------------------------------------- */

// __geom_faceset_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

