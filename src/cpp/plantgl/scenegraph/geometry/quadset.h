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



/*! \file geom_quadset.h
    \brief Definition of the geometry class QuadSet.
*/


#ifndef __geom_quadset_h__
#define __geom_quadset_h__

/* ----------------------------------------------------------------------- */

#include "mesh.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/container/indexarray.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class QuadSet
   \brief A quadset
*/

/* ----------------------------------------------------------------------- */

class SG_API QuadSet : public IndexedMesh<Index4Array>
{

public:
  friend class FaceSet;

  typedef IndexedMesh<Index4Array>::Builder<QuadSet> Builder;

  static std::string getClassName() { return "QuadSet"; }

  /// Default Constructor. Build object is invalid.
  QuadSet();

  /** Constructs a QuadSet with the points \e points, the quadrilaterals
      indices \e indices, the face ordering \e ccw, the solid flag \e solid
      and the skeleton \e skeleton.
      \pre
      - \e points must contain a minimum of 3 points;
      - \ indices must contain a minimum of 1 index. Each element of an
      index must be unique.
      - \e skeleton must be valid in the case it is non null.
      \post
      - \e self is valid. */
  QuadSet( const Point3ArrayPtr& points,
       const Index4ArrayPtr& indices,
       bool normalPerVertex = DEFAULT_NORMALPERVERTEX,
       bool ccw = DEFAULT_CCW,
       bool solid = DEFAULT_SOLID,
       const PolylinePtr& skeleton = DEFAULT_SKELETON);


  /** Constructs a QuadSet with the points \e points, the quadrilaterals
      indices \e indices, the face ordering \e ccw, the solid flag \e solid
      the skeleton \e skeleton and the normals \e normals.
      normals don't have to be normalized.
      \pre
      - \e points must contain a minimum of 3 points;
      - \ indices must contain a minimum of 1 index. Each element of an
      index must be unique.
      - \e skeleton must be valid in the case it is non null.
      - \e normals must be as big as points;
      \post
      - \e self is valid. */
  QuadSet( const Point3ArrayPtr& points,
           const Index4ArrayPtr& indices,
           const Point3ArrayPtr& normals,
           const Index4ArrayPtr& nomalIndices = Index4ArrayPtr(),
           const Color4ArrayPtr& colors  = Color4ArrayPtr(),
           const Index4ArrayPtr& colorIndices = Index4ArrayPtr(),
           const Point2ArrayPtr& texCoord = Point2ArrayPtr(),
           const Index4ArrayPtr& texCoordIndices = Index4ArrayPtr(),
           bool normalPerVertex = DEFAULT_NORMALPERVERTEX,
           bool colorPerVertex = DEFAULT_COLORPERVERTEX,
           bool ccw = DEFAULT_CCW,
           bool solid = DEFAULT_SOLID,
           const PolylinePtr& skeleton = DEFAULT_SKELETON);

  /// Destructor
  virtual ~QuadSet( );

  PGL_OBJECT(QuadSet)

  virtual bool isValid( ) const;

  virtual ExplicitModelPtr
  transform( const Transformation3DPtr& transformation ) const;

};

/// QuadSet Pointer
typedef RCPtr<QuadSet> QuadSetPtr;


/* ----------------------------------------------------------------------- */

// __geom_quadset_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

