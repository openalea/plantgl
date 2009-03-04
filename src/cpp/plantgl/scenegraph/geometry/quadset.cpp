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




#include "quadset.h"
#include "mesh_inline.h"


PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


QuadSet::QuadSet() : IndexedMesh<Index4Array>(){ }

QuadSet::QuadSet( const Point3ArrayPtr& points,
				  const Index4ArrayPtr& indices,
				  bool normalPerVertex,
				  bool ccw,
				  bool solid,
				  const PolylinePtr& skeleton ) :
  IndexedMesh<Index4Array>(points,indices,normalPerVertex,ccw,solid,skeleton){}

QuadSet::QuadSet( const Point3ArrayPtr& points,
				  const Index4ArrayPtr& indices,
				  const Point3ArrayPtr& normals ,
				  const Index4ArrayPtr& nomalIndices,
				  const Color4ArrayPtr& colors,
				  const Index4ArrayPtr& colorIndices,
				  const Point2ArrayPtr& texCoord,
				  const Index4ArrayPtr& texCoordIndices,
				  bool normalPerVertex,
				  bool colorPerVertex,
				  bool ccw,
				  bool solid,
				  const PolylinePtr& skeleton) :
    IndexedMesh<Index4Array>(points,indices,normals,nomalIndices,colors,colorIndices,
		 texCoord,texCoordIndices,normalPerVertex,colorPerVertex,ccw,solid,skeleton){ }


QuadSet::~QuadSet( ) {
}

SceneObjectPtr 
QuadSet::copy(DeepCopier& copier) const{
  return SceneObjectPtr(mesh_copy<QuadSet>(*this,copier));
}

bool QuadSet::isValid( ) const {
  return isAValidMesh<QuadSet>();
}


ExplicitModelPtr
QuadSet::transform( const Transformation3DPtr& transformation ) const 
{ return mesh_transform<QuadSet>(*this,transformation); }

/* ----------------------------------------------------------------------- */
