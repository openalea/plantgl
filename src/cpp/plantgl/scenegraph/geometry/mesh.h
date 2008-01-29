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
        const Color4ArrayPtr& colors  = 0,
        const Point2ArrayPtr& texCoord = 0,
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
  virtual uint32_t getIndexListSize( ) const =0;

  /** Returns the \e j-th point in the point list.
      \warning
	  - \e PointList should be set
      - \e i must be belong to the range [0,size of \b PointList[. */
  const TOOLS(Vector3)& getPointAt( uint32_t i ) const ;

  /** Returns the \e j-th point of the \e i-th face.
      \warning
	  - \e PointList should be set
      - \e i must belong to the range [0,size of \b IndexList[;
      - \e j must belong to the range [0,2]. */
  virtual const TOOLS(Vector3)& getFacePointAt( uint32_t i, uint32_t j ) const = 0;

  void computeNormalList(bool pervertex);
  virtual Point3ArrayPtr computeNormalPerVertex() const = 0;
  virtual Point3ArrayPtr computeNormalPerFace() const = 0;

  inline bool hasNormalList() const { return __normalList.isValid(); }
  inline void checkNormalList() { if(!hasNormalList())computeNormalList(); }
  inline void computeNormalList() { computeNormalList(__normalPerVertex); };

  bool hasTexCoordList() const;

  /// Returns the nb of points of the \b i-th face.
  virtual uint32_t getFaceSize( uint32_t i ) const  = 0;

  /// Returns the center of the \b i-th face.
  TOOLS(Vector3) getFaceCenter( uint32_t i ) const ;

  /// flip the order of corners around the given face
  void flipFace ( uint32_t i );

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


// __geom_mesh_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

