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




#include "faceset.h"
#include "triangleset.h"
#include "quadset.h"
#include "mesh_inline.h"

PGL_USING_NAMESPACE


/* ----------------------------------------------------------------------- */

FaceSet::FaceSet() :  IndexedMesh<IndexArray>(){ }

FaceSet::FaceSet( const Point3ArrayPtr& points,
                  const IndexArrayPtr& indices,
                  bool normalPerVertex,
                  bool ccw,
                  bool solid,
                  const PolylinePtr& skeleton  ) :
  IndexedMesh<IndexArray>(points,indices,normalPerVertex,ccw,solid,skeleton){ }

FaceSet::FaceSet( const Point3ArrayPtr& points,
                  const IndexArrayPtr& indices,
                  const Point3ArrayPtr& normals ,
                  const IndexArrayPtr& nomalIndices,
                  const Color4ArrayPtr& colors,
                  const IndexArrayPtr& colorIndices,
                  const Point2ArrayPtr& texCoord,
                  const IndexArrayPtr& texCoordIndices,
                  bool normalPerVertex,
                  bool colorPerVertex,
                  bool ccw,
                  bool solid,
                  const PolylinePtr& skeleton) :
  IndexedMesh<IndexArray>(points,indices,normals,nomalIndices,colors,colorIndices,
            texCoord,texCoordIndices,normalPerVertex,colorPerVertex,ccw,solid,skeleton)
  { }

/** Constructs a FaceSet from a TriangleSet.  */
FaceSet::FaceSet(const TriangleSet& obj) :
  IndexedMesh<IndexArray>(obj.__pointList,IndexArrayPtr(),
                          obj.__normalList,IndexArrayPtr(),
                          obj.__colorList,IndexArrayPtr(),
                          obj.__texCoordList,IndexArrayPtr(),
                          obj.__normalPerVertex,obj.__colorPerVertex,
                          obj.__ccw,obj.__solid,obj.__skeleton) {
  if( obj.getIndexList()) __indexList = IndexArrayPtr(new IndexArray(*obj.getIndexList())) ;
  if( obj.getNormalIndexList()) __normalIndexList = IndexArrayPtr(new IndexArray(*obj.getNormalIndexList())) ;
  if( obj.getColorIndexList()) __colorIndexList = IndexArrayPtr(new IndexArray(*obj.getColorIndexList())) ;
  if( obj.getTexCoordIndexList()) __texCoordIndexList = IndexArrayPtr(new IndexArray(*obj.getTexCoordIndexList())) ;
  GEOM_ASSERT(isValid());
}

  /** Constructs a FaceSet from a TriangleSet.  */
FaceSet::FaceSet(const QuadSet& obj)  :
  IndexedMesh<IndexArray>(obj.__pointList,IndexArrayPtr(),
                          obj.__normalList,IndexArrayPtr(),
                          obj.__colorList,IndexArrayPtr(),
                          obj.__texCoordList,IndexArrayPtr(),
                          obj.__normalPerVertex,obj.__colorPerVertex,
                          obj.__ccw,obj.__solid,obj.__skeleton) {
  if( obj.getIndexList()) __indexList = IndexArrayPtr(new IndexArray(*obj.getIndexList())) ;
  if( obj.getNormalIndexList()) __normalIndexList = IndexArrayPtr(new IndexArray(*obj.getNormalIndexList())) ;
  if( obj.getColorIndexList()) __colorIndexList = IndexArrayPtr(new IndexArray(*obj.getColorIndexList())) ;
  if( obj.getTexCoordIndexList()) __texCoordIndexList = IndexArrayPtr(new IndexArray(*obj.getTexCoordIndexList())) ;
  GEOM_ASSERT(isValid());
}


FaceSet::~FaceSet( ) {
}

bool FaceSet::isValid( ) const {
  return isAValidMesh<FaceSet>();
}

SceneObjectPtr
FaceSet::copy(DeepCopier& copier) const{
  return SceneObjectPtr(mesh_copy<FaceSet>(*this,copier));
}

ExplicitModelPtr
FaceSet::transform( const Transformation3DPtr& transformation ) const
{ return mesh_transform<FaceSet>(*this,transformation); }

/* ----------------------------------------------------------------------- */


