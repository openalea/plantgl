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

class SG_API TriangleSet : public IndexedMesh<Index3Array>
{

public:
   friend class FaceSet;

  typedef IndexedMesh<Index3Array>::Builder<TriangleSet> Builder;

  static std::string getClassName() { return "TriangleSet"; }

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
           const Index3ArrayPtr& nomalIndices = Index3ArrayPtr(),
           const Color4ArrayPtr& colors  = Color4ArrayPtr(),
           const Index3ArrayPtr& colorIndices = Index3ArrayPtr(),
           const Point2ArrayPtr& texCoord = Point2ArrayPtr(),
           const Index3ArrayPtr& texCoordIndices = Index3ArrayPtr(),
           bool normalPerVertex = DEFAULT_NORMALPERVERTEX,
           bool colorPerVertex = DEFAULT_COLORPERVERTEX,
           bool CCW = DEFAULT_CCW,
           bool solid = DEFAULT_SOLID,
           const PolylinePtr& skeleton = DEFAULT_SKELETON);

  /// Destructor
  virtual ~TriangleSet( );

  PGL_OBJECT(TriangleSet)

  virtual bool isValid() const;

  virtual ExplicitModelPtr
  transform( const Transformation3DPtr& transformation ) const;

};

/// TriangleSet Pointer
typedef RCPtr<TriangleSet> TriangleSetPtr;


/* ----------------------------------------------------------------------- */

// __geom_triangleset_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

