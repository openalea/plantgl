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


/*! \file geom_mesh.h
    \brief Definition of the geometry class Mesh.
*/

#ifndef __geom_mesh_h__
#define __geom_mesh_h__

/* ----------------------------------------------------------------------- */

#include "geom_explicitmodel.h"
#ifndef GEOM_FWDEF
#include "geom_polyline.h"
#endif
/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE
class bofstream;
TOOLS_END_NAMESPACE

/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */
#ifdef GEOM_FWDEF
class GeomPolyline;
typedef RCPtr<GeomPolyline> GeomPolylinePtr;
class Point2Array;
typedef RCPtr<Point2Array> Point2ArrayPtr;
#endif

/* ----------------------------------------------------------------------- */

/**
   \class Mesh
   \brief Abstract base class for objects of type of \b Mesh.
*/

class GEOM_API Mesh : public ExplicitModel
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
  static const GeomPolylinePtr DEFAULT_SKELETON;


  /// A structure which helps to build an object of type of Mesh.
  struct Builder : public ExplicitModel::Builder {

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
    GeomPolylinePtr * Skeleton;

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
        const GeomPolylinePtr& skeleton = DEFAULT_SKELETON);


  /// Constructor.
  Mesh( const Point3ArrayPtr& points,
        const Point3ArrayPtr& normals ,
        const Color4ArrayPtr& colors  = 0,
        const Point2ArrayPtr& texCoord = 0,
        bool NormalPerVertex = DEFAULT_NORMALPERVERTEX,
        bool colorPerVertex = DEFAULT_COLORPERVERTEX,
        bool CCW = DEFAULT_CCW,
        bool solid = DEFAULT_SOLID,
        const GeomPolylinePtr& skeleton = DEFAULT_SKELETON);

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
  const GeomPolylinePtr& getSkeleton( ) const;

  /// Returns \b Skeleton field.
  GeomPolylinePtr& getSkeleton( );

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


  void computeNormalList(bool pervertex);
  virtual Point3ArrayPtr computeNormalPerVertex() const = 0;
  virtual Point3ArrayPtr computeNormalPerFace() const = 0;

  inline bool hasNormalList() const { return __normalList.isValid(); }
  inline void checkNormalList() { if(!hasNormalList())computeNormalList(); }
  inline void computeNormalList() { computeNormalList(__normalPerVertex); };

  bool hasTexCoordList() const;

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
  GeomPolylinePtr __skeleton;

}; // Mesh

/// Mesh Pointer
typedef RCPtr<Mesh> MeshPtr;


/* ----------------------------------------------------------------------- */


// __geom_mesh_h__
/* ----------------------------------------------------------------------- */

GEOM_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

