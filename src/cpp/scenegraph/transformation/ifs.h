/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C.Pradal (christophe.pradal@cirad.fr)
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

/*! \file geom_ifs.h
    \brief Definition of the transformation class IT and the transformed class IFS.
*/


#ifndef __geom_ifs_h__
#define __geom_ifs_h__

/* ----------------------------------------------------------------------- */

#include "util_array.h"
#include "Tools/util_matrix.h"
#include "geom_geometryarray.h"
#include "geom_transformed.h"

/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

typedef Array1<TOOLS(Matrix4)> Matrix4Array;
typedef RCPtr<Matrix4Array> Matrix4ArrayPtr;

class Transform4;
typedef RCPtr<Transform4> Transform4Ptr;
typedef ObjectArray<Transform4Ptr> Transform4Array;
typedef RCPtr<Transform4Array> Transform4ArrayPtr;

/* ----------------------------------------------------------------------- */

/**
    \class IT
    \brief IT : IterativeTransformation. (n ** depth) transformations ( n: size of TransfoList )
*/

class GEOM_API IT : public Transformation3D
{

public:

  IT( uchar_t depth,
      const Transform4ArrayPtr& transfoList );

  /// Destructor.
  virtual ~IT( );

  virtual bool isValid( ) const;

  virtual Point3ArrayPtr transform( const Point3ArrayPtr& points ) const;

  virtual Point4ArrayPtr transform( const Point4ArrayPtr& points ) const;

  virtual Point3MatrixPtr transform( const Point3MatrixPtr& points ) const;

  virtual Point4MatrixPtr transform( const Point4MatrixPtr& points ) const;

  const Matrix4ArrayPtr& getAllTransfo() const;

protected:

  /// The \b Depth field.
  uchar_t __depth;

  /// The \b Transformation List field.
  Transform4ArrayPtr __transfoList;

private:

  void init();
  void apply( uchar_t depth, Matrix4Array::iterator& it, uint32_t size );

  Matrix4ArrayPtr __transfoNodes;

};

/// IT Pointer
typedef RCPtr<IT> ITPtr;

/* ----------------------------------------------------------------------- */


/**
   \class IFS
   \brief IFS: Iterated Function System. Transform iteratively a geometry by a set of 3D transformations
*/

class GEOM_API IFS : public Transformed
{

public:

  /// The \e Depth field default value
  static const uchar_t DEFAULT_DEPTH;

  /// A structure which helps to build a IFS when parsing.
  struct Builder : public Transformed::Builder
    {
    /// A pointer to the \b Depth field.
    uchar_t * Depth;

    /// A pointer to the \b Transformation List field.
    Transform4ArrayPtr* TransfoList;

    /// A pointer to the \b Geometry field.
    GeometryPtr * Geometry;

    /// Constructor.
    Builder( );

    /// Destructor.
    virtual ~Builder( );

    virtual SceneObjectPtr build( ) const;

    virtual void destroy( );

    virtual bool isValid( ) const;
    };

  /// Default Constructor. Build object is invalid.
  IFS();

  /** Constructs an Iterative Function System object with
      the number of iterations \e depth and the list of
      transformations applied to the object \e geometry
      a each iteration.
      \warning
      - \e transfoList must be valid.
      - \e geometry must be non null and valid.
  */

  IFS( uchar_t depth,
       const Transform4ArrayPtr& transfoList,
       const GeometryPtr& geometry  );

  /// Destructor
  virtual ~IFS( );

  virtual bool apply( Action& action );

  /// Returns the transformation attached to \e self.
  virtual Transformation3DPtr getTransformation( ) const;

  /// Returns \b Depth value.
  const uchar_t getDepth( ) const;

  /// Returns \b CCW field.
  uchar_t& getDepth( );

  /// Returns \b Transformation List value.
  const Transform4ArrayPtr& getTransfoList( ) const;

  /// Returns \b Transformation List field.
  Transform4ArrayPtr& getTransfoList( );

  /// Returns Geometry value.
  virtual const GeometryPtr getGeometry( ) const;

  /// Returns Geometry field.
  GeometryPtr& getGeometry( );

  virtual bool isACurve( ) const;

  virtual bool isASurface( ) const;

  virtual bool isAVolume( ) const;

  virtual bool isExplicit( ) const;

  /// Returns whether \b CCW is set to its default value.
  bool isDepthToDefault( ) const;

  virtual bool isValid( ) const;

  /// Deep copy of \e this.
  virtual SceneObjectPtr copy() const ;

  template <class T> static T power( const T& a, uchar_t b )
    {
    T r= a;
    for( uchar_t i= 0; i < b-1; i++ )
      r*= a;
    return r;
    }

protected:

  /// The \b Depth field.
  uchar_t __depth;

  /// The \b Transformation List field.
  Transform4ArrayPtr __transfoList;

  /// The \b Geometry field.
  GeometryPtr __geometry;

}; // IFS

/// IFS Pointer
typedef RCPtr<IFS> IFSPtr;


/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */

GEOM_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
 // __geom_ifs_h__

