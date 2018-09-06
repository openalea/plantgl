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


/*! \file geom_mesh.h
    \brief Definition of the geometry class Mesh.
*/

#ifndef __geom_mesh_h__
#define __geom_mesh_h__

/* ----------------------------------------------------------------------- */

#include "explicitmodel.h"
#include "polyline.h"
/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE
class bofstream;
TOOLS_END_NAMESPACE

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE


/* ----------------------------------------------------------------------- */

/**
   \class Mesh
   \brief Abstract base class for objects of type of \b Mesh.
*/

class SG_API Mesh : public ExplicitModel
{

public:

  /// The default \b CCW field value.
  static const bool DEFAULT_CCW;

  /// The default \b Solid field value.
  static const bool DEFAULT_SOLID;

  /// The default \b NormalPerVertex field value.
  static const bool DEFAULT_NORMALPERVERTEX;

  /// The default \b ColorPerVertex field value.
  static const bool DEFAULT_COLORPERVERTEX;

  /// The default \b Skeleton field value.
  static const PolylinePtr DEFAULT_SKELETON;

  /// The default normal value
  static const TOOLS(Vector3) DEFAULT_NORMAL_VALUE;

  /// A structure which helps to build an object of type of Mesh.
  struct SG_API Builder : public ExplicitModel::Builder {

    /// A pointer to the \b CCW field.
    bool * CCW;

    /// A pointer to the \b NormalPerVertex field.
    bool * NormalPerVertex;

    /// A pointer to the \b ColorPerVertex field.
    bool * ColorPerVertex;

    /// A pointer to the \b Solid field.
    bool * Solid;

	/// A pointer to the \b NormalList field.
    Point3ArrayPtr * NormalList;

    /// A pointer to the \b TexCoordList field.
    Point2ArrayPtr * TexCoordList;

    /// A pointer to the \b Skeleton field.
    PolylinePtr * Skeleton;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual void destroy( );

    void MeshDestroy( );

    virtual bool isValid( ) const;

    bool MeshValid( ) const;
  };


  /// Default Constructor. Build object is invalid.
  Mesh();

  /// Constructor.
  Mesh( const Point3ArrayPtr& points,
        bool NormalPerVertex = DEFAULT_NORMALPERVERTEX,
        bool CCW = DEFAULT_CCW,
        bool solid = DEFAULT_SOLID,
        const PolylinePtr& skeleton = DEFAULT_SKELETON);


  /// Constructor.
  Mesh( const Point3ArrayPtr& points,
        const Point3ArrayPtr& normals ,
        const Color4ArrayPtr& colors  = Color4ArrayPtr(),
        const Point2ArrayPtr& texCoord = Point2ArrayPtr(),
        bool NormalPerVertex = DEFAULT_NORMALPERVERTEX,
        bool colorPerVertex = DEFAULT_COLORPERVERTEX,
        bool CCW = DEFAULT_CCW,
        bool solid = DEFAULT_SOLID,
        const PolylinePtr& skeleton = DEFAULT_SKELETON);

  /// Destructor
  virtual ~Mesh( );
  
  /// Returns \b CCW value.
  const bool getCCW( ) const;

  /// Returns \b CCW field.
  bool& getCCW( );

  /// Returns \b NormalList values.
  Point3ArrayPtr& getNormalList( );

  /// Returns \b NormalList values.
  const Point3ArrayPtr& getNormalList( ) const ;

  /// Returns \b TexCoordList values.
  Point2ArrayPtr& getTexCoordList( );

  /// Returns \b NormalList values.
  const Point2ArrayPtr& getTexCoordList( ) const ;

  /// Returns \b Skeleton value.
  const PolylinePtr& getSkeleton( ) const;

  /// Returns \b Skeleton field.
  PolylinePtr& getSkeleton( );

  /// Returns \b Solid value.
  const bool getSolid( ) const;

  /// Returns \b Solid field.
  bool& getSolid( );

  /// Returns \b ColorPerVertex value.
  inline const bool getColorPerVertex( ) const { return __colorPerVertex; }

  /// Returns \b ColorPerVertex field.
  inline bool& getColorPerVertex( ) { return __colorPerVertex; }

  /// Returns \b NormalPerVertex value.
  inline const bool getNormalPerVertex( ) const { return __normalPerVertex; }

  /// Returns \b NormalPerVertex value.
  inline bool& getNormalPerVertex( ) { return __normalPerVertex; }

  virtual bool isACurve( ) const ;

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const;


  /// Returns whether \b CCW is set to its default value.
  bool isCCWToDefault( ) const ;

  /// Returns whether \b Skeleton is set to its default value.
  bool isSkeletonToDefault( ) const;

  /// Returns whether \b Solid is set to its default value.
  bool isSolidToDefault( ) const;

  /// Returns whether \b NormalPerVertex is set to its default value.
  bool isNormalPerVertexToDefault() const;

  /// Returns whether \b Normal is set to its default value.
  bool isNormalListToDefault() const;

  /// Returns whether \b ColorPerVertex is set to its default value.
  bool isColorPerVertexToDefault() const;

  /// Returns whether \b TexCoord is set to its default value.
  bool isTexCoordListToDefault() const;

  /** Writes \e self to the output binary stream \e stream.
      \pre
      - \e stream must be valid. */
  // virtual TOOLS(bofstream)& write( TOOLS(bofstream)& stream ) const = 0;

  /// Returns the size of \b IndexList i.e. the number of polygon.
  virtual uint_t getIndexListSize( ) const =0;

  /** Returns the \e j-th point in the point list.
      \warning
	  - \e PointList should be set
      - \e i must be belong to the range [0,size of \b PointList[. */
  inline const TOOLS(Vector3)& getPointAt( uint_t i ) const {
	GEOM_ASSERT(__pointList.isValid());
	GEOM_ASSERT(i < __pointList->size());
	return __pointList->getAt(i);
  }

  /// Returns the nb of points of the \b i-th face.
  virtual uint_t getFaceSize( uint_t i ) const  = 0;

  /** Returns the index of the \e j-th point of the i-th face.
      \warning
      - \e i must be belong to the range [0,size of \b IndexList]. 
	  - \e j must belong to the range [0,getFaceSize(i)]. */
  virtual uint_t getFacePointIndexAt( uint_t i, uint_t j ) const = 0;

  /** Returns the index of the \e j-th point of the i-th face.
      \warning
      - \e i must be belong to the range [0,size of \b IndexList]. 
	  - \e j must belong to the range [0,getFaceSize(i)]. */
  virtual uint_t getFaceNormalIndexAt( uint_t i, uint_t j ) const = 0;

   
  /** Returns the index of the \e j-th point of the i-th face.
      \warning
      - \e i must be belong to the range [0,size of \b IndexList]. 
	  - \e j must belong to the range [0,getFaceSize(i)]. */
  virtual uint_t getFaceTexCoordIndexAt( uint_t i, uint_t j ) const = 0;

  /** Returns the index of the \e j-th point of the i-th face.
      \warning
      - \e i must be belong to the range [0,size of \b IndexList]. 
	  - \e j must belong to the range [0,getFaceSize(i)]. */
  virtual uint_t getFaceColorIndexAt( uint_t i, uint_t j ) const = 0;

  /** Returns the \e j-th point of the \e i-th face.
      \warning
	  - \e PointList should be set
      - \e i must belong to the range [0,size of \b IndexList[;
      - \e j must belong to the range [0,getFaceSize(i)]. */
  inline const TOOLS(Vector3)& getFacePointAt( uint_t i, uint_t j ) const  {
	GEOM_ASSERT(__pointList.isValid());
	GEOM_ASSERT(i < getIndexListSize());
	GEOM_ASSERT(j < getFaceSize(i));
	return __pointList->getAt(getFacePointIndexAt(i,j));
  }

  /// Returns the center of the \b i-th face.
  TOOLS(Vector3) getFaceCenter( uint_t i ) const ;

  /** Returns the \e i-th normal to (normally correxponding to the \e i-th face).
      \pre
	  - \e NormalList should be set
      - \e i must be belong to the range [0,size of \b NormalList]. */
  inline const TOOLS(Vector3)& getNormalAt( uint_t i )  const  {
	GEOM_ASSERT(is_valid_ptr(__normalList));
	GEOM_ASSERT(i < __normalList->size());
	return __normalList->getAt(i);
  }

  /** Returns the normal at the \e j-th point of the \e i-th face.
      \pre
      - \e i must be belong to the range [0,size of \b IndexList[. 
      - \e j must belong to the range [0,2]. */
  inline const TOOLS(Vector3)& getFaceNormalAt( uint_t i, uint_t j ) const {
	GEOM_ASSERT(is_valid_ptr(__normalList));
	GEOM_ASSERT(i < __indexList->size());
	GEOM_ASSERT(j < getFaceSize(i));
	return __normalList->getAt(getFaceNormalIndexAt(i,j));
  }

  /** Returns the \e j-th texture coordinates of the \e i-th face.
      \warning
	  - \e TexCoordList should be set
      - \e i must be belong to the range [0,size of \b TexCoordList[. */
  const TOOLS(Vector2)& getTexCoordAt( uint_t i ) const  {
	GEOM_ASSERT(is_valid_ptr(__texCoordList));
	GEOM_ASSERT(i < __texCoordList->size());
	return __texCoordList->getAt(i);
  }

  /** Returns the \e j-th texture coordinates of the \e i-th face.
      \warning
	  - \e TexCoordList should be set
      - \e i must belong to the range [0,size of \b IndexList[;
      - \e j must belong to the range [0,2]. */
  const TOOLS(Vector2)& getFaceTexCoordAt( uint_t i, uint_t j ) const {
	GEOM_ASSERT(is_valid_ptr(__texCoordList));
	GEOM_ASSERT(i < __indexList->size());
	GEOM_ASSERT(j < getFaceSize(i));
	return __texCoordList->getAt(getFaceTexCoordIndexAt(i,j));
	}
   
  /** Returns the \e j-th colors of the \e i-th face.
      \warning
      - \e i must be belong to the range [0,size of \b ColorList[. */
  const Color4& getColorAt( uint_t i ) const {
	GEOM_ASSERT(is_valid_ptr(__colorList));
	GEOM_ASSERT(i < __indexList->size());
	return __colorList->getAt(i);
  }
    
  /** Returns the \e j-th colors of the \e i-th face.
      \warning
      - \e i must belong to the range [0,size of \b IndexList[;
      - \e j must belong to the range [0,2]. */
  const Color4& getFaceColorAt( uint_t i, uint_t j ) const {
	GEOM_ASSERT(is_valid_ptr(__colorList));
	GEOM_ASSERT(i < __indexList->size());
	GEOM_ASSERT(j < getFaceSize(i));
	return __colorList->getAt(getFaceColorIndexAt(i,j));
  }

#ifndef PGL_NO_DEPRECATED
  inline attribute_deprecated const TOOLS(Vector3)& getNormalAt( uint_t i, uint_t j ) const 
  { return getFaceNormalAt(i,j); }

  inline attribute_deprecated const TOOLS(Vector2)& getTexCoordAt( uint_t i, uint_t j ) const 
  { return getFaceTexCoordAt(i,j); }

  inline attribute_deprecated const Color4& getColorAt( uint_t i, uint_t j ) const 
  { return getFaceColorAt(i,j); }
#endif

  void computeNormalList(bool pervertex);
  Point3ArrayPtr computeNormalPerVertex() const;
  Point3ArrayPtr computeNormalPerFace() const;

  inline bool hasNormalList() const { return is_valid_ptr(__normalList); }
  inline void checkNormalList() { if(!hasNormalList())computeNormalList(); }
  inline void computeNormalList() { computeNormalList(__normalPerVertex); };

  bool hasTexCoordList() const;

  /// flip the order of corners around the given face
  // void flipFace ( uint32_t i );

  protected:

  /// The CCW field.
  bool __ccw;

  /// The Solid field.
  bool __solid;

  /// The normalPerVertex field.
  bool __normalPerVertex;

  /// The colorPerVertex field.
  bool __colorPerVertex;

  /// The \b Normal List field.
  Point3ArrayPtr __normalList;

  /// The \b Texture Coordinates List field.
  Point2ArrayPtr __texCoordList;

  /// The Skeleton field.
  PolylinePtr __skeleton;

}; // Mesh

/// Mesh Pointer
typedef RCPtr<Mesh> MeshPtr;


/* ----------------------------------------------------------------------- */

template <class IndexArrayType>
class IndexedMesh : public Mesh
{

public:

  typedef IndexArrayType IndexArray;
  typedef typename IndexArray::element_type IndexType;
  typedef RCPtr<IndexArray> IndexArrayPtr;

  /// A structure which helps to build a TriangleSet when parsing. 
  template <class InstanciedMesh>
  struct Builder : public Mesh::Builder {

    typedef InstanciedMesh MeshType;
	typedef typename MeshType::IndexArray IndexArray;
	typedef RCPtr<IndexArray> IndexArrayPtr;

    /// A pointer to the IndexList field.
    IndexArrayPtr * IndexList;

    /// A pointer to the NormalIndexList field.
    IndexArrayPtr * NormalIndexList;

    /// A pointer to the ColorIndexList field.
    IndexArrayPtr * ColorIndexList;

    /// A pointer to the TexCoordIndexList field.
    IndexArrayPtr * TexCoordIndexList;

    /// Constructor.
    Builder( ):
		Mesh::Builder(), IndexList(0), NormalIndexList(0),
		ColorIndexList(0), TexCoordIndexList(0) {}

    /// Destructor.
	virtual ~Builder( ) { }

    virtual SceneObjectPtr build( ) const 
		{
			if (isValid()){
				return SceneObjectPtr(new InstanciedMesh(*PointList,
									  *IndexList,
									  NormalList?*NormalList:Point3ArrayPtr(),
									  NormalIndexList?*NormalIndexList:IndexArrayPtr(),
									  ColorList?*ColorList:Color4ArrayPtr(),
									  ColorIndexList?*ColorIndexList:IndexArrayPtr(),
									  TexCoordList?*TexCoordList:Point2ArrayPtr(),
									  TexCoordIndexList?*TexCoordIndexList:IndexArrayPtr(),
									  NormalPerVertex? *NormalPerVertex : DEFAULT_NORMALPERVERTEX,
									  ColorPerVertex? *ColorPerVertex: DEFAULT_COLORPERVERTEX,
									  CCW ? *CCW : DEFAULT_CCW,
									  Solid ? *Solid : DEFAULT_SOLID,
									  Skeleton ? *Skeleton : DEFAULT_SKELETON));
			}
			return SceneObjectPtr();
		}

    void indexedMeshDestroy( ){
		MeshDestroy();
		if (IndexList)         { delete IndexList; IndexList = NULL; }
		if (NormalIndexList)   { delete NormalIndexList; NormalIndexList = NULL; }
		if (ColorIndexList)    { delete ColorIndexList; ColorIndexList = NULL; }
		if (TexCoordIndexList) { delete TexCoordIndexList; TexCoordIndexList = NULL; };
	}

	virtual void destroy( ){ indexedMeshDestroy(); }

    bool IndexedMeshValidity( ) const;

	virtual bool isValid( ) const { return IndexedMeshValidity(); }

  };


  /// Default Constructor. Build object is invalid.
  IndexedMesh<IndexArrayType>():
	Mesh(), __indexList(), __normalIndexList(), __texCoordIndexList(), __colorIndexList(){}

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
  IndexedMesh<IndexArrayType>( const Point3ArrayPtr& points,
	           const IndexArrayPtr& indices,
			   bool normalPerVertex = DEFAULT_NORMALPERVERTEX,
			   bool ccw = DEFAULT_CCW,
			   bool solid = DEFAULT_SOLID,
			   const PolylinePtr& skeleton = DEFAULT_SKELETON ) :
    Mesh(points,normalPerVertex,ccw,solid,skeleton),
    __indexList(indices), __normalIndexList(), __texCoordIndexList(), __colorIndexList()
	{ GEOM_ASSERT(isValid()); }


  /** Constructs a Mesh with the points \e points, the triangles 
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
  IndexedMesh<IndexArrayType>( const Point3ArrayPtr& points,
	           const IndexArrayPtr& indices,
			   const Point3ArrayPtr& normals ,
	           const IndexArrayPtr& nomalIndices = IndexArrayPtr(),
			   const Color4ArrayPtr& colors  = Color4ArrayPtr(),
	           const IndexArrayPtr& colorIndices = IndexArrayPtr(),
			   const Point2ArrayPtr& texCoord = Point2ArrayPtr(),
	           const IndexArrayPtr& texCoordIndices = IndexArrayPtr(),
			   bool normalPerVertex = DEFAULT_NORMALPERVERTEX,
			   bool colorPerVertex = DEFAULT_COLORPERVERTEX,
			   bool ccw = DEFAULT_CCW,
			   bool solid = DEFAULT_SOLID,
			   const PolylinePtr& skeleton = DEFAULT_SKELETON) :
    Mesh(points,normals,colors,texCoord,normalPerVertex,colorPerVertex,ccw,solid,skeleton),
    __indexList(indices), __normalIndexList(nomalIndices), __colorIndexList(colorIndices),
	__texCoordIndexList(texCoordIndices){ GEOM_ASSERT(isValid()); }

  /// Destructor
  virtual ~IndexedMesh<IndexArrayType>( ) { }

  // Check Validity
  template<class InstanciedMesh>
  bool isAValidMesh( ) const;

  void setTexCoordIndexListAsIndexList();

  /// Returns the size of \b IndexList.
  virtual uint_t getIndexListSize( ) const 
  { return (__indexList?__indexList->size():0); }

  /// Returns the nb of points of the \b i-th face.
  virtual uint_t getFaceSize( uint_t i ) const 
  { return __indexList->getIndexSizeAt(i); }

  /** Returns the index of the \e j-th point of the i-th face.
      \warning
      - \e i must be belong to the range [0,size of \b IndexList]. 
	  - \e j must belong to the range [0,getFaceSize(i)]. */
  virtual uint_t getFacePointIndexAt( uint_t i, uint_t j ) const
  { GEOM_ASSERT(i < __indexList->size());
	GEOM_ASSERT(j < getFaceSize(i));
	return __indexList->getAt(i).getAt(j); }

  /** Returns the index of the \e j-th point of the i-th face.
      \warning
      - \e i must be belong to the range [0,size of \b IndexList]. 
	  - \e j must belong to the range [0,getFaceSize(i)]. */
  virtual uint_t getFaceNormalIndexAt( uint_t i, uint_t j ) const 
  {
	GEOM_ASSERT(i < __indexList->size());
	GEOM_ASSERT(j < getFaceSize(i));
    if (!__normalPerVertex) return i;
	if ( __normalIndexList) return __normalIndexList->getAt(i).getAt(j);
	else  return __indexList->getAt(i).getAt(j);
   }

  /** Returns the index of the \e j-th point of the i-th face.
      \warning
      - \e i must be belong to the range [0,size of \b IndexList]. 
	  - \e j must belong to the range [0,getFaceSize(i)]. */
  virtual uint_t getFaceTexCoordIndexAt( uint_t i, uint_t j ) const 
  {
	GEOM_ASSERT(i < __indexList->size());
	GEOM_ASSERT(j < getFaceSize(i));
	if( __texCoordIndexList) return __texCoordIndexList->getAt(i).getAt(j);
	else  return __indexList->getAt(i).getAt(j);
   }

  /** Returns the index of the \e j-th point of the i-th face.
      \warning
      - \e i must be belong to the range [0,size of \b IndexList]. 
	  - \e j must belong to the range [0,getFaceSize(i)]. */
  virtual uint_t getFaceColorIndexAt( uint_t i, uint_t j ) const 
  {
	GEOM_ASSERT(i < __indexList->size());
	GEOM_ASSERT(j < getFaceSize(i));
    if (!__colorPerVertex) return i;
	if ( __colorIndexList) return __colorIndexList->getAt(i).getAt(j);
	else  return __indexList->getAt(i).getAt(j);
   }

  /// Returns \b IndexList values.
  inline const IndexArrayPtr& getIndexList( ) const 
  { return __indexList; } 

  /// Returns \b IndexList field.
  inline IndexArrayPtr& getIndexList( ) 
  { return __indexList; } 

  /** Returns the \e i-th value of \b IndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline const IndexType& getIndexListAt( uint_t i ) const
  { GEOM_ASSERT(i < __indexList->size()); return __indexList->getAt(i); }

  /** Returns the \e i-th value of \b IndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline IndexType& getIndexListAt( uint_t i )
  { GEOM_ASSERT(i < __indexList->size()); return __indexList->getAt(i); }

  /// Returns \b NormalIndexList values.
  inline const IndexArrayPtr& getNormalIndexList( ) const 
  { return __normalIndexList; }

  /// Returns \b NormalIndexList field.
  inline IndexArrayPtr& getNormalIndexList( )  
  { return __normalIndexList; }

  /** Returns the \e i-th value of \b NormalIndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline const IndexType& getNormalIndexListAt( uint_t i ) const 
  { GEOM_ASSERT(is_valid_ptr(__normalIndexList) && i < __normalIndexList->size()); 
    return __normalIndexList->getAt(i); }

  /** Returns the \e i-th value of \b IndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline IndexType& getNormalIndexListAt( uint_t i )
  { GEOM_ASSERT(is_valid_ptr(__normalIndexList) && i < __normalIndexList->size()); 
    return __normalIndexList->getAt(i); }

  /// Returns the size of \b NormalIndexList.
  inline uint_t getNormalIndexListSize( ) const 
  { return (__normalIndexList?__normalIndexList->size():0); }

  /// Returns \b ColorIndexList values.
  inline const IndexArrayPtr& getColorIndexList( ) const 
  { return __colorIndexList; }

  /// Returns \b ColorIndexList field.
  inline IndexArrayPtr& getColorIndexList( )  
  { return __colorIndexList; }

  /** Returns the \e i-th value of \b ColorIndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline const IndexType& getColorIndexListAt( uint_t i ) const 
  { GEOM_ASSERT(is_valid_ptr(__colorIndexList) && i < __colorIndexList->size());
    return __colorIndexList->getAt(i); }

  /** Returns the \e i-th value of \b IndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline IndexType& getColorIndexListAt( uint_t i )
  { GEOM_ASSERT(is_valid_ptr(__colorIndexList) && i < __colorIndexList->size());
    return __colorIndexList->getAt(i); }

  /// Returns the size of \b ColorIndexList.
  inline uint_t getColorIndexListSize( ) const 
  { return (__colorIndexList?__colorIndexList->size():0); }

  /// Returns \b TexCoordIndexList values.
  inline const IndexArrayPtr& getTexCoordIndexList( ) const 
  { return __texCoordIndexList; }

  /// Returns \b TexCoordIndexList field.
  inline IndexArrayPtr& getTexCoordIndexList( )
  { return __texCoordIndexList; }

  /** Returns the \e i-th value of \b TexCoordIndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline const IndexType& getTexCoordIndexListAt( uint_t i ) const 
  { GEOM_ASSERT(is_valid_ptr(__texCoordIndexList) && i < __texCoordIndexList->size());
    return __texCoordIndexList->getAt(i); }

  /** Returns the \e i-th value of \b IndexList.
      \pre
      - \e i must belong to the range [0,size of \b IndexList). */
  inline IndexType& getTexCoordIndexListAt( uint_t i )
  { GEOM_ASSERT(is_valid_ptr(__texCoordIndexList) && i < __texCoordIndexList->size());
    return __texCoordIndexList->getAt(i); }

  /// Returns the size of \b TexCoordIndexList.
  inline uint_t getTexCoordIndexListSize( ) const 
  { return (__texCoordIndexList?__texCoordIndexList->size():0); }


   /// Returns whether \b NormalIndexList is set to its default value.
  inline bool isNormalIndexListToDefault() const 
  { return (!__normalIndexList); }

   /// Returns whether \b ColorIndexList is set to its default value.
  inline bool isColorIndexListToDefault() const 
  { return (!__colorIndexList); }

   /// Returns whether \b TexCoordIndexList is set to its default value.
  inline bool isTexCoordIndexListToDefault() const 
  { return (!__texCoordIndexList); }

  protected:

  /// The \b IndexList field.
  IndexArrayPtr __indexList;

  /// The \b NormalIndexList field.
  IndexArrayPtr __normalIndexList;

  /// The \b ColorIndexList field.
  IndexArrayPtr __colorIndexList;

  /// The \b TexCoordIndexList field.
  IndexArrayPtr __texCoordIndexList;


  template<class MeshType>
  static ExplicitModelPtr mesh_transform(const MeshType& mesh, 
							             const Transformation3DPtr& transformation )  ;

  template<class MeshType>
  static RCPtr<MeshType> mesh_copy(const MeshType& mesh, DeepCopier& copier);

};

// __geom_mesh_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

