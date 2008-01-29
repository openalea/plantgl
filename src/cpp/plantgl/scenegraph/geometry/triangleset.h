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


/*! \file geom_triangleset.h
    \brief Definition of the geometry class TriangleSet.
*/

#ifndef __geom_triangleset_h__
#define __geom_triangleset_h__

/* ----------------------------------------------------------------------- */

#include "mesh.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/container/indexarray.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class TriangleSet
   \brief A triangleset
*/

/* ----------------------------------------------------------------------- */

class SG_API TriangleSet : public Mesh
{

public:

  /// A structure which helps to build a TriangleSet when parsing. 
  struct SG_API Builder : Mesh::Builder {

    /// A pointer to the IndexList field.
    Index3ArrayPtr * IndexList;

    /// A pointer to the NormalIndexList field.
    Index3ArrayPtr * NormalIndexList;

    /// A pointer to the ColorIndexList field.
    Index3ArrayPtr * ColorIndexList;

    /// A pointer to the TexCoordIndexList field.
    Index3ArrayPtr * TexCoordIndexList;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;

  };


  /// Default Constructor. Build object is invalid.
  TriangleSet();

  /** Constructs a TriangleSet with the points \e points, the triangles 
      indices \e indices, the face ordering \e ccw, the solid flag \e solid 
      and the skeleton \e skeleton.
      \pre
      - \e points must contain a minimum of 3 points;
      - \ indices must contain a minimum of 1 index. Each element of an
      index must be unique.
      - \e skeleton must be valid in the case it is non null.
      \post
      - \e self is valid. */
  TriangleSet( const Point3ArrayPtr& points,
	           const Index3ArrayPtr& indices,
			   bool normalPerVertex = DEFAULT_NORMALPERVERTEX,
			   bool ccw = DEFAULT_CCW,
			   bool solid = DEFAULT_SOLID,
			   const PolylinePtr& skeleton = DEFAULT_SKELETON );


  /** Constructs a TriangleSet with the points \e points, the triangles 
      indices \e indices, the face ordering \e ccw, the solid flag \e solid,
      the skeleton \e skeleton and the normals \e normals.
      normals don't have to be normalized.
      \pre
      - \e points must contain a minimum of 3 points;
      - \e indices must contain a minimum of 1 index. Each element of an
      index must be unique.
      - \e skeleton must be valid in the case it is non null.
      - \e normals must be as big as points;
      - \e pointsColor must ne as big as points
      \post
      - \e self is valid. */
  TriangleSet( const Point3ArrayPtr& points,
	           const Index3ArrayPtr& indices,
			   const Point3ArrayPtr& normals ,
	           const Index3ArrayPtr& nomalIndices = 0,
			   const Color4ArrayPtr& colors  = 0,
	           const Index3ArrayPtr& colorIndices = 0,
			   const Point2ArrayPtr& texCoord = 0,
	           const Index3ArrayPtr& texCoordIndices = 0,
			   bool normalPerVertex = DEFAULT_NORMALPERVERTEX,
			   bool colorPerVertex = DEFAULT_COLORPERVERTEX,
			   bool CCW = DEFAULT_CCW,
			   bool solid = DEFAULT_SOLID,
			   const PolylinePtr& skeleton = DEFAULT_SKELETON);

  /// Destructor
  virtual ~TriangleSet( );

  virtual bool apply( Action& action );

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  /// Returns \b IndexList values.
  const Index3ArrayPtr& getIndexList( ) const ;

  /// Returns \b IndexList field.
  Index3ArrayPtr& getIndexList( );

  /** Returns the \e i-th value of \b IndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  const Index3& getIndexListAt( uint_t i ) const;

  /** Returns the \e i-th value of \b IndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  Index3& getIndexListAt( uint_t i );

  /// Returns the size of \b IndexList.
  virtual uint_t getIndexListSize( ) const;

  /// Returns \b NormalIndexList values.
  inline const Index3ArrayPtr& getNormalIndexList( ) const { return __normalIndexList; }

  /// Returns \b NormalIndexList field.
  inline Index3ArrayPtr& getNormalIndexList( )  { return __normalIndexList; }

  /** Returns the \e i-th value of \b NormalIndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline const Index3& getNormalIndexListAt( uint_t i ) const 
  { GEOM_ASSERT(__normalIndexList.isValid() && i < __normalIndexList->getSize()) return __normalIndexList->getAt(i); }

  /** Returns the \e i-th value of \b IndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline Index3& getNormalIndexListAt( uint_t i )
  { GEOM_ASSERT(__normalIndexList.isValid() && i < __normalIndexList->getSize()) return __normalIndexList->getAt(i); }

  /// Returns the size of \b NormalIndexList.
  inline uint_t getNormalIndexListSize( ) const { return (__normalIndexList.isValid()?__normalIndexList->getSize():0); }

  /// Returns \b ColorIndexList values.
  inline const Index3ArrayPtr& getColorIndexList( ) const { return __colorIndexList; }

  /// Returns \b ColorIndexList field.
  inline Index3ArrayPtr& getColorIndexList( )  { return __colorIndexList; }

  /** Returns the \e i-th value of \b ColorIndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline const Index3& getColorIndexListAt( uint_t i ) const 
  { GEOM_ASSERT(__colorIndexList.isValid() && i < __colorIndexList->getSize()) return __colorIndexList->getAt(i); }

  /** Returns the \e i-th value of \b IndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline Index3& getColorIndexListAt( uint_t i )
  { GEOM_ASSERT(__colorIndexList.isValid() && i < __colorIndexList->getSize()) return __colorIndexList->getAt(i); }

  /// Returns the size of \b ColorIndexList.
  inline uint_t getColorIndexListSize( ) const { return (__colorIndexList.isValid()?__colorIndexList->getSize():0); }

  /// Returns \b TexCoordIndexList values.
  inline const Index3ArrayPtr& getTexCoordIndexList( ) const { return __texCoordIndexList; }

  /// Returns \b TexCoordIndexList field.
  inline Index3ArrayPtr& getTexCoordIndexList( )  { return __texCoordIndexList; }

  /** Returns the \e i-th value of \b TexCoordIndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline const Index3& getTexCoordIndexListAt( uint_t i ) const 
  { GEOM_ASSERT(__texCoordIndexList.isValid() && i < __texCoordIndexList->getSize()) return __texCoordIndexList->getAt(i); }

  /** Returns the \e i-th value of \b IndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline Index3& getTexCoordIndexListAt( uint_t i )
  { GEOM_ASSERT(__texCoordIndexList.isValid() && i < __texCoordIndexList->getSize()) return __texCoordIndexList->getAt(i); }

  /// Returns the size of \b TexCoordIndexList.
  inline uint_t getTexCoordIndexListSize( ) const { return (__texCoordIndexList.isValid()?__texCoordIndexList->getSize():0); }

  /** Returns the \e i-th normal to (normally correxponding to the \e i-th face).
      \pre
	  - \e NormalList should be set
      - \e i must be belong to the range [0,size of \b NormalList[. */
  const TOOLS(Vector3)& getNormalAt( uint_t i )  const ;
  
  /** Returns the normal at the \e j-th point of the \e i-th face.
      \pre
      - \e i must be belong to the range [0,size of \b IndexList[. 
      - \e j must belong to the range [0,2]. */
  const TOOLS(Vector3)& getNormalAt( uint_t i, uint_t j ) const ;

  /** Returns the \e j-th point of the \e i-th face.
      \warning
	  - \e PointList should be set
      - \e i must belong to the range [0,size of \b IndexList[;
      - \e j must belong to the range [0,2]. */
  const TOOLS(Vector3)& getFacePointAt( uint_t i, uint_t j ) const ;
    
  /** Returns the \e j-th texture coordinates of the \e i-th face.
      \warning
	  - \e TexCoordList should be set
      - \e i must be belong to the range [0,size of \b TexCoordList[. */
  const TOOLS(Vector2)& getTexCoordAt( uint_t i ) const ;

  /** Returns the \e j-th texture coordinates of the \e i-th face.
      \warning
	  - \e TexCoordList should be set
      - \e i must belong to the range [0,size of \b IndexList[;
      - \e j must belong to the range [0,2]. */
  const TOOLS(Vector2)& getTexCoordAt( uint_t i, uint_t j ) const ;
    
  /** Returns the \e j-th colors of the \e i-th face.
      \warning
      - \e i must be belong to the range [0,size of \b ColorList[. */
  const Color4& getColorAt( uint_t i ) const ;
    
  /** Returns the \e j-th colors of the \e i-th face.
      \warning
      - \e i must belong to the range [0,size of \b IndexList[;
      - \e j must belong to the range [0,2]. */
  const Color4& getColorAt( uint_t i, uint_t j ) const ;
    
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
  virtual uint_t getFaceSize( uint_t i ) const { return 3; }

  protected:

  /// The \b IndexList field.
  Index3ArrayPtr __indexList;

  /// The \b NormalIndexList field.
  Index3ArrayPtr __normalIndexList;

  /// The \b ColorIndexList field.
  Index3ArrayPtr __colorIndexList;

  /// The \b TexCoordIndexList field.
  Index3ArrayPtr __texCoordIndexList;

};

/// TriangleSet Pointer
typedef RCPtr<TriangleSet> TriangleSetPtr;


/* ----------------------------------------------------------------------- */

// __geom_triangleset_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

