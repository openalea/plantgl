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


/*! \file geom_ifs.h
    \brief Definition of the transformation class IT and the transformed class IFS.
*/


#ifndef __geom_ifs_h__
#define __geom_ifs_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/tool/util_array.h>
#include <plantgl/math/util_matrix.h>
#include <plantgl/scenegraph/container/geometryarray.h>
#include "transformed.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

typedef Array1<Matrix4> Matrix4Array;
typedef RCPtr<Matrix4Array> Matrix4ArrayPtr;
PGL_DECLARE_TYPE(Matrix4Array)

class Transform4;
typedef RCPtr<Transform4> Transform4Ptr;
typedef ObjectArray<Transform4Ptr> Transform4Array;
typedef RCPtr<Transform4Array> Transform4ArrayPtr;

/* ----------------------------------------------------------------------- */

/**
    \class IT
    \brief IT : IterativeTransformation. (n ** depth) transformations ( n: size of TransfoList )
*/

class SG_API IT : public Transformation3D
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
  void apply( uchar_t depth, Matrix4Array::iterator& it, uint_t size );

  Matrix4ArrayPtr __transfoNodes;

};

/// IT Pointer
typedef RCPtr<IT> ITPtr;

/* ----------------------------------------------------------------------- */


/**
   \class IFS
   \brief IFS: Iterated Function System. Transform iteratively a geometry by a set of 3D transformations
*/

class SG_API IFS : public Transformed
{

public:

  /// The \e Depth field default value
  static const uchar_t DEFAULT_DEPTH;

  /// A structure which helps to build a IFS when parsing.
  struct SG_API Builder : public Transformed::Builder
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

  PGL_OBJECT(IFS)

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

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
 // __geom_ifs_h__

