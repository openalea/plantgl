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

#ifndef __geom_mesh_inline_h__
#define __geom_mesh_inline_h__

/* ----------------------------------------------------------------------- */

#include "mesh.h"
#include <plantgl/tool/util_string.h>
// #include <plantgl/tool/bfstream.h>
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/transformation/transformed.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE


/* ----------------------------------------------------------------------- */

template<class IndexArrayType>
template<class MeshType>
ExplicitModelPtr IndexedMesh<IndexArrayType>::mesh_transform(const MeshType& mesh, const Transformation3DPtr& transformation ) 
{
  GEOM_ASSERT(transformation);

  PolylinePtr _tSkeleton = mesh.__skeleton;
  if (_tSkeleton)
    _tSkeleton = dynamic_pointer_cast<Polyline>(mesh.__skeleton->transform(transformation));

  Point3ArrayPtr _n = mesh.__normalList;
  if(_n){
      _n = transformation->transform(mesh.__normalList);
      _n->normalize();
  }
   
  return ExplicitModelPtr(new MeshType(transformation->transform(mesh.__pointList),mesh.__indexList,
                                    _n, mesh.__normalIndexList,
									mesh.__colorList, mesh.__colorIndexList,
									mesh.__texCoordList, mesh.__texCoordIndexList,
									mesh.__normalPerVertex, mesh.__colorPerVertex,
									mesh.__ccw, mesh.__solid, _tSkeleton));
}


template<class IndexArrayType>
template<class MeshType>
SceneObjectPtr IndexedMesh<IndexArrayType>::mesh_copy(const MeshType& mesh) { 
  MeshType * ptr(new MeshType(mesh));
  if(mesh.__pointList)ptr->getPointList() = Point3ArrayPtr(new Point3Array(*mesh.__pointList));
  if(mesh.__indexList)ptr->getIndexList() = MeshType::IndexArrayPtr(new IndexArray(*mesh.__indexList));
  if(mesh.__skeleton)ptr->getSkeleton() = dynamic_pointer_cast<Polyline>(mesh.__skeleton->copy());
  if(mesh.__normalList)ptr->getNormalList() = Point3ArrayPtr(new Point3Array(*mesh.__normalList));
  if(mesh.__texCoordList)ptr->getTexCoordList() = Point2ArrayPtr(new Point2Array(*mesh.__texCoordList));
  if(mesh.__colorList)ptr->getColorList() = Color4ArrayPtr(new Color4Array(*mesh.__colorList));
  if(mesh.__normalIndexList) ptr->getNormalIndexList() = MeshType::IndexArrayPtr(new MeshType::IndexArray(*mesh.__normalIndexList));
  if(mesh.__texCoordIndexList) ptr->getTexCoordIndexList() = MeshType::IndexArrayPtr(new MeshType::IndexArray(*mesh.__texCoordIndexList));
  if(mesh.__colorIndexList) ptr->getColorIndexList() = MeshType::IndexArrayPtr(new MeshType::IndexArray(*mesh.__colorIndexList));
  return SceneObjectPtr(ptr);
}

template<class IndexArrayType>
template<class InstanciedMesh>
bool IndexedMesh<IndexArrayType>::isAValidMesh( ) const {
   IndexedMesh<IndexArrayType>::Builder<InstanciedMesh> _builder;
  _builder.NormalPerVertex = const_cast<bool *>(&__normalPerVertex);
  _builder.ColorPerVertex = const_cast<bool *>(&__colorPerVertex);
  if(__pointList)_builder.PointList = const_cast<Point3ArrayPtr *>(&__pointList);
  if(__indexList)_builder.IndexList = const_cast<InstanciedMesh::IndexArrayPtr *>(&__indexList);
  if(__skeleton)_builder.Skeleton = const_cast<PolylinePtr *>(&__skeleton);
  if(__normalList)_builder.NormalList = const_cast<Point3ArrayPtr *>(&__normalList);
  if(__texCoordList)_builder.TexCoordList = const_cast<Point2ArrayPtr *>(&__texCoordList);
  if(__colorList)_builder.ColorList = const_cast<Color4ArrayPtr *>(&__colorList);
  if(__normalIndexList) _builder.NormalIndexList = const_cast<InstanciedMesh::IndexArrayPtr *>(&__normalIndexList);
  if(__texCoordIndexList) _builder.TexCoordIndexList = const_cast<InstanciedMesh::IndexArrayPtr *>(&__texCoordIndexList);
  if(__colorIndexList) _builder.ColorIndexList = const_cast<InstanciedMesh::IndexArrayPtr *>(&__colorIndexList);
  return _builder.isValid();
}

template<class IndexArrayType>
template<class InstanciedMesh>
bool IndexedMesh<IndexArrayType>::Builder<InstanciedMesh>::IndexedMeshValidity( ) const {
  if(!MeshValid())return false;

  // PointList field
  // Size check
  std::string classname = InstanciedMesh::getClassName();

  uint_t _pointListSize = (*PointList)->getSize();
  if (_pointListSize < 3) {
    genMessage(ERRORMSG(INVALID_FIELD_SIZE_sss),classname,"PointList","Must have more than 2 Points.");
    return false;
  };

  // IndexList field
  // Initialization check
  if (! IndexList) {
    genMessage(ERRORMSG(UNINITIALIZED_FIELD_ss),classname,"IndexList");
    return false;
  };
  // Size check
  uint_t _indexListSize = (*IndexList)->getSize();
  if (_indexListSize < 1) {
    genMessage(ERRORMSG(INVALID_FIELD_SIZE_sss),classname,"IndexList","Number of Index must be greater than 0.");
    return false;
  };

  // Indices check
  IndexArray::iterator _it = (*IndexList)->getBegin();
  for (uint_t _i = 0; _i < _indexListSize; ++_i,++_it) {
    // Max index check
	  if (*(std::max_element(_it->getBegin(),_it->getEnd())) >= _pointListSize) {
	genMessage
	    (ERRORMSG(INVALID_FIELD_ITH_VALUE_ssss),classname,"IndexList",number(_i+1).c_str(),"Do not represent any point of the list.");
      }
      // Unique elements check
      else if (! _it->isUnique()) {
	  genMessage
	      (WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),classname,"IndexList",number(_i+1).c_str(),"Redundance : Already declared.");
	  // _it = (*IndexList)->Erase(_it);
      }
  };
  
  if (NormalList) {
	  uint_t _normalListSize = (*NormalList)->getSize();
	  if (!NormalIndexList){
		  if(NormalPerVertex && !*NormalPerVertex){
			  if(_normalListSize != _indexListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),classname,"NormalList","Number of normals must be compatible to IndexList size.");
				return false;
			  }
		  }
		  else
		  {
			  if(_normalListSize != _pointListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),classname,"NormalList","Number of normals must be compatible to PointList size.");
				return false;
			  }
		  }
	  }
	  else {
		  uint_t _normalIndexListSize = (*NormalIndexList)->getSize();
		  if(NormalPerVertex && !*NormalPerVertex){
			genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),classname,"NormalPerVertex","If NormalIndexList is specified, NormalPerVertex should be True.");
			return false;
		  }
		  else
		  {
			  if(_normalIndexListSize != _indexListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),classname,"NormalIndexList","Number of normals indices must be compatible to IndexList size.");
				return false;
			  }
		  }
		  // NormalIndexList values check
		  IndexArray::iterator _it = (*NormalIndexList)->getBegin();
		  for (uint_t _i = 0; _i < _normalIndexListSize; ++_i,++_it) {
			// Max index check
			if (*(std::max_element(_it->getBegin(),_it->getEnd())) >= _normalListSize) {
				genMessage
					(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),classname,"NormalIndexList",number(_i+1).c_str(),"Do not represent any normal of the list.");
				return false;
			}
			// Size index check
			if (_it->getSize() != (*IndexList)->getAt(_i).getSize()) {
				genMessage
					(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),classname,"NormalIndexList",number(_i+1).c_str(),"Normal indice size do no reflect actual polygon indice size.");
				return false;
			}
	  	 }

	  }
  }
  else 
  {
	  if (NormalIndexList){
		genMessage(ERRORMSG(UNINITIALIZED_FIELD_ss),classname,"NormalList");
		return false;
	  }
  }

  if (ColorList) {
	  uint_t _colorListSize = (*ColorList)->getSize();
	  if (!ColorIndexList){
		  if(ColorPerVertex && !*ColorPerVertex){
			  if(_colorListSize != _indexListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),classname,"ColorList","Number of colors must be compatible to IndexList size.");
				return false;
			  }
		  }
		  else
		  {
			  if(_colorListSize != _pointListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),classname,"ColorList","Number of colors must be compatible to PointList size.");
				return false;
			  }
		  }
	  }
	  else {
		  uint_t _colorIndexListSize = (*ColorIndexList)->getSize();
		  if(ColorPerVertex && !*ColorPerVertex){
			genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),classname,"ColorPerVertex","If ColorIndexList is specified, ColorPerVertex should be True.");
			return false;
		  }
		  else
		  {
			  if(_colorIndexListSize != _indexListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),classname,"ColorIndexList","Number of colors indices must be compatible to IndexList size.");
				return false;
			  }
		  }
		  // ColorIndexList values check
		  IndexArray::iterator _it = (*ColorIndexList)->getBegin();
		  for (uint_t _i = 0; _i < _colorIndexListSize; ++_i,++_it) {
			// Max index check
			if (*(std::max_element(_it->getBegin(),_it->getEnd())) >= _colorListSize) {
				genMessage
					(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),classname,"ColorIndexList",number(_i+1).c_str(),"Do not represent any color of the list.");
				return false;
			}
			// Size index check
			if (_it->getSize() != (*IndexList)->getAt(_i).getSize()) {
				genMessage
					(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),classname,"ColorIndexList",number(_i+1).c_str(),"Color indice size do no reflect actual polygon indice size.");
				return false;
			}
	  	 }

	  }
  }
  else 
  {
	  if (ColorIndexList){
		genMessage(ERRORMSG(UNINITIALIZED_FIELD_ss),classname,"ColorList");
		return false;
	  }
  }

  if (TexCoordList) {
	  uint_t _texCoordListSize = (*TexCoordList)->getSize();
	  if (!TexCoordIndexList){
		  if(_texCoordListSize != _pointListSize){
			genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),classname,"TexCoordList","Number of TexCoord must be compatible to PointList size.");
			return false;
		  }
	  }
	  else {
		  uint_t _texCoordIndexListSize = (*TexCoordIndexList)->getSize();
		  if(_texCoordIndexListSize != _indexListSize){
			genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),classname,"TexCoordIndexList","Number of TexCoord indices must be compatible to IndexList size.");
			return false;
		  }
		  // TexCoordIndexList values check
		  IndexArray::iterator _it = (*TexCoordIndexList)->getBegin();
		  for (uint_t _i = 0; _i < _texCoordIndexListSize; ++_i,++_it) {
			// Max index check
			if (*(std::max_element(_it->getBegin(),_it->getEnd())) >= _texCoordListSize) {
				genMessage
					(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),classname,"TexCoordIndexList",number(_i+1).c_str(),"Do not represent any TexCoord of the list.");
				return false;
			}
			// Size index check
			if (_it->getSize() != (*IndexList)->getAt(_i).getSize()) {
				genMessage
					(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),classname,"TexCoordIndexList",number(_i+1).c_str(),"TexCoord indice size do no reflect actual polygon indice size.");
				return false;
			}
	  	 }

	  }
  }
  else 
  {
	  if (TexCoordIndexList){
		genMessage(ERRORMSG(UNINITIALIZED_FIELD_ss),classname,"TexCoordList");
		return false;
	  }
  }

  return true;
}


// __geom_mesh_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
