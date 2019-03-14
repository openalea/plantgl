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

PGL_USING_NAMESPACE
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
RCPtr<MeshType> IndexedMesh<IndexArrayType>::mesh_copy(const MeshType& mesh, DeepCopier& copier) { 
  MeshType * ptr(new MeshType(mesh));
  copier.copy_attribute(ptr->getPointList());
  copier.copy_attribute(ptr->getIndexList());
  copier.copy_attribute(ptr->getSkeleton());
  copier.copy_attribute(ptr->getNormalList());
  copier.copy_attribute(ptr->getTexCoordList());
  copier.copy_attribute(ptr->getColorList());
  copier.copy_attribute(ptr->getNormalIndexList());
  copier.copy_attribute(ptr->getTexCoordIndexList());
  copier.copy_attribute(ptr->getColorIndexList());
  return RCPtr<MeshType>(ptr);
}

template<class IndexArrayType>
template<class MeshType>
bool IndexedMesh<IndexArrayType>::isAValidMesh( ) const {
  typename MeshType::Builder _builder;
  typedef typename MeshType::IndexArrayPtr IndexArrayPtrType;
  _builder.NormalPerVertex = const_cast<bool *>(&__normalPerVertex);
  _builder.ColorPerVertex = const_cast<bool *>(&__colorPerVertex);
  if(__pointList)_builder.PointList = const_cast<Point3ArrayPtr *>(&__pointList);
  if(__indexList)_builder.IndexList = const_cast<IndexArrayPtrType *>(&__indexList);
  if(__skeleton)_builder.Skeleton = const_cast<PolylinePtr *>(&__skeleton);
  if(__normalList)_builder.NormalList = const_cast<Point3ArrayPtr *>(&__normalList);
  if(__texCoordList)_builder.TexCoordList = const_cast<Point2ArrayPtr *>(&__texCoordList);
  if(__colorList)_builder.ColorList = const_cast<Color4ArrayPtr *>(&__colorList);
  if(__normalIndexList) _builder.NormalIndexList = const_cast<IndexArrayPtrType *>(&__normalIndexList);
  if(__texCoordIndexList) _builder.TexCoordIndexList = const_cast<IndexArrayPtrType *>(&__texCoordIndexList);
  if(__colorIndexList) _builder.ColorIndexList = const_cast<IndexArrayPtrType *>(&__colorIndexList);
  return _builder.isValid();
}

template<class IndexArrayType>
template<class MeshType>
bool IndexedMesh<IndexArrayType>::Builder<MeshType>::IndexedMeshValidity( ) const {
  typedef typename MeshType::IndexArray MeshIndexArrayType;

  if(!MeshValid())return false;

  // PointList field
  // Size check
  std::string classname = MeshType::getClassName();

  uint_t _pointListSize = (*PointList)->size();
  if (_pointListSize < 3) {
    pglErrorEx(PGLERRORMSG(INVALID_FIELD_SIZE_sss),classname.c_str(),"PointList","Must have more than 2 Points.");
    return false;
  };

  // IndexList field
  // Initialization check
  if (! IndexList) {
    pglErrorEx(PGLERRORMSG(UNINITIALIZED_FIELD_ss),classname.c_str(),"IndexList");
    return false;
  };
  // Size check
  uint_t _indexListSize = (*IndexList)->size();
  if (_indexListSize < 1) {
    pglErrorEx(PGLERRORMSG(INVALID_FIELD_SIZE_sss),classname.c_str(),"IndexList","Number of Index must be greater than 0.");
    return false;
  };

  // Indices check
  typename MeshIndexArrayType::iterator _it = (*IndexList)->begin();
  for (uint_t _i = 0; _i < _indexListSize; ++_i,++_it) {
    // Max index check
	  if (*(std::max_element(_it->begin(),_it->end())) >= _pointListSize) {
	pglErrorEx
	    (PGLERRORMSG(INVALID_FIELD_ITH_VALUE_ssss),classname.c_str(),"IndexList",number(_i+1).c_str(),(number(*std::max_element(_it->begin(),_it->end()))+" do not represent any point of the list.").c_str());
      }
      // Unique elements check
      else if (! _it->isUnique()) {
	  pglErrorEx
	      (PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),classname.c_str(),"IndexList",number(_i+1).c_str(),"Redundance : Already declared.");
	  // _it = (*IndexList)->Erase(_it);
      }
  };
  
  if (NormalList) {
	  uint_t _normalListSize = (*NormalList)->size();
	  if (!NormalIndexList){
		  if(NormalPerVertex && !*NormalPerVertex){
			  if(_normalListSize != _indexListSize){
				pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),classname.c_str(),"NormalList","Number of normals must be compatible to IndexList size.");
				return false;
			  }
		  }
		  else
		  {
			  if(_normalListSize != _pointListSize){
				pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),classname.c_str(),"NormalList","Number of normals must be compatible to PointList size.");
				return false;
			  }
		  }
	  }
	  else {
		  uint_t _normalIndexListSize = (*NormalIndexList)->size();
		  if(NormalPerVertex && !*NormalPerVertex){
			pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),classname.c_str(),"NormalPerVertex","If NormalIndexList is specified, NormalPerVertex should be True.");
			return false;
		  }
		  else
		  {
			  if(_normalIndexListSize != _indexListSize){
				pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),classname.c_str(),"NormalIndexList","Number of normals indices must be compatible to IndexList size.");
				return false;
			  }
		  }
		  // NormalIndexList values check
		  typename MeshIndexArrayType::iterator _it = (*NormalIndexList)->begin();
		  for (uint_t _i = 0; _i < _normalIndexListSize; ++_i,++_it) {
			// Max index check
			  if (*std::max_element(_it->begin(),_it->end()) >= _normalListSize) {
				pglErrorEx
					(PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),classname.c_str(),"NormalIndexList",number(_i+1).c_str(),"Do not represent any normal of the list.");
				return false;
			}
			// Size index check
			if (_it->size() != (*IndexList)->getAt(_i).size()) {
				pglErrorEx
					(PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),classname.c_str(),"NormalIndexList",number(_i+1).c_str(),"Normal indice size do no reflect actual polygon indice size.");
				return false;
			}
	  	 }

	  }
  }
  else 
  {
	  if (NormalIndexList){
		pglErrorEx(PGLERRORMSG(UNINITIALIZED_FIELD_ss),classname.c_str(),"NormalList");
		return false;
	  }
  }

  if (ColorList) {
	  uint_t _colorListSize = (*ColorList)->size();
	  if (!ColorIndexList){
		  if(ColorPerVertex && !*ColorPerVertex){
			  if(_colorListSize != _indexListSize){
				pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),classname.c_str(),"ColorList","Number of colors must be compatible to IndexList size.");
				return false;
			  }
		  }
		  else
		  {
			  if(_colorListSize != _pointListSize){
				pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),classname.c_str(),"ColorList","Number of colors must be compatible to PointList size.");
				return false;
			  }
		  }
	  }
	  else {
		  uint_t _colorIndexListSize = (*ColorIndexList)->size();
		  if(ColorPerVertex && !*ColorPerVertex){
			pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),classname.c_str(),"ColorPerVertex","If ColorIndexList is specified, ColorPerVertex should be True.");
			return false;
		  }
		  else
		  {
			  if(_colorIndexListSize != _indexListSize){
				pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),classname.c_str(),"ColorIndexList","Number of colors indices must be compatible to IndexList size.");
				return false;
			  }
		  }
		  // ColorIndexList values check
		  typename MeshIndexArrayType::iterator _it = (*ColorIndexList)->begin();
		  for (uint_t _i = 0; _i < _colorIndexListSize; ++_i,++_it) {
			// Max index check
			if (*(std::max_element(_it->begin(),_it->end())) >= _colorListSize) {
				pglErrorEx
					(PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),classname.c_str(),"ColorIndexList",number(_i+1).c_str(),"Do not represent any color of the list.");
				return false;
			}
			// Size index check
			if (_it->size() != (*IndexList)->getAt(_i).size()) {
				pglErrorEx
					(PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),classname.c_str(),"ColorIndexList",number(_i+1).c_str(),"Color indice size do no reflect actual polygon indice size.");
				return false;
			}
	  	 }

	  }
  }
  else 
  {
	  if (ColorIndexList){
		pglErrorEx(PGLERRORMSG(UNINITIALIZED_FIELD_ss),classname.c_str(),"ColorList");
		return false;
	  }
  }

  if (TexCoordList) {
	  uint_t _texCoordListSize = (*TexCoordList)->size();
	  if (!TexCoordIndexList){
		  if(_texCoordListSize != _pointListSize){
			pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),classname.c_str(),"TexCoordList","Number of TexCoord must be compatible to PointList size.");
			return false;
		  }
	  }
	  else {
		  uint_t _texCoordIndexListSize = (*TexCoordIndexList)->size();
		  if(_texCoordIndexListSize != _indexListSize){
			pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),classname.c_str(),"TexCoordIndexList","Number of TexCoord indices must be compatible to IndexList size.");
			return false;
		  }
		  // TexCoordIndexList values check
		  typename MeshIndexArrayType::iterator _it = (*TexCoordIndexList)->begin();
		  for (uint_t _i = 0; _i < _texCoordIndexListSize; ++_i,++_it) {
			// Max index check
			if (*(std::max_element(_it->begin(),_it->end())) >= _texCoordListSize) {
				pglErrorEx
					(PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),classname.c_str(),"TexCoordIndexList",number(_i+1).c_str(),"Do not represent any TexCoord of the list.");
				return false;
			}
			// Size index check
			if (_it->size() != (*IndexList)->getAt(_i).size()) {
				pglErrorEx
					(PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),classname.c_str(),"TexCoordIndexList",number(_i+1).c_str(),"TexCoord indice size do no reflect actual polygon indice size.");
				return false;
			}
	  	 }

	  }
  }
  else 
  {
	  if (TexCoordIndexList){
		pglErrorEx(PGLERRORMSG(UNINITIALIZED_FIELD_ss),classname.c_str(),"TexCoordList");
		return false;
	  }
  }

  return true;
}

template <class IndexArrayType>
void IndexedMesh<IndexArrayType>::setTexCoordIndexListAsIndexList()
{
	if (!this->__texCoordIndexList)
		return;
	std::vector<int> tmpCoordIndex(this->__texCoordList->size());
	int index = 0;
	for (typename IndexArrayType::const_iterator it = this->__texCoordIndexList->begin(); it != this->__texCoordIndexList->end(); ++it)
	{
		typename IndexArrayType::element_type current = this->__indexList->getAt(index);
		for (int i = 0; i < current.size(); i++)
		{
			tmpCoordIndex[it->operator[](i)] = current[i];
		}
		index++;
	}

	bool hasToModifyNormalList = this->__normalList && this->__normalPerVertex;
	bool hasToModifyColorList = this->__colorList && this->__colorPerVertex;

	Point3ArrayPtr newPoint3Array(new Point3Array());
	Point3ArrayPtr newNormalArray(new Point3Array());
	Color4ArrayPtr newColor4Array(new Color4Array());
	for (std::vector<int>::const_iterator it = tmpCoordIndex.begin(); it != tmpCoordIndex.end(); ++it)
	{
		newPoint3Array->push_back(this->__pointList->getAt(*it));
		if (hasToModifyNormalList)
			newNormalArray->push_back(this->__normalList->getAt(*it));
		if (hasToModifyColorList)
			newColor4Array->push_back(this->__colorList->getAt(*it));
	}

	this->__pointList = newPoint3Array;
	this->__indexList = this->__texCoordIndexList;
	this->__texCoordIndexList = IndexArrayPtr();
	if (hasToModifyNormalList)
		this->__normalList = newNormalArray;
	if (hasToModifyColorList)
		this->__colorList = newColor4Array;
}


// __geom_mesh_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
