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


