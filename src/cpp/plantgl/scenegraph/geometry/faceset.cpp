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
#include "polyline.h"
#include <plantgl/scenegraph/transformation/transformed.h>
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/tool/bfstream.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/container/colorarray.h>
PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


FaceSet::Builder::Builder( ) :
  Mesh::Builder(),
  IndexList(0),
  NormalIndexList(0),
  ColorIndexList(0),
  TexCoordIndexList(0) {
}


FaceSet::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr FaceSet::Builder::build( ) const {
  if (isValid()){
	FaceSet * f = new FaceSet(*PointList,
							  *IndexList,
							  NormalList?*NormalList:Point3ArrayPtr(),
							  NormalIndexList?*NormalIndexList:IndexArrayPtr(),
							  ColorList?*ColorList:Color4ArrayPtr(),
							  ColorIndexList?*ColorIndexList:IndexArrayPtr(),
							  TexCoordList?*TexCoordList:Point2ArrayPtr(),
							  TexCoordIndexList?*TexCoordIndexList:IndexArrayPtr(),
							  NormalPerVertex? *NormalPerVertex : DEFAULT_NORMALPERVERTEX,
							  ColorPerVertex? *ColorPerVertex: DEFAULT_COLORPERVERTEX,
							  CCW ? *CCW : DEFAULT_CCW,
							  Solid ? *Solid : DEFAULT_SOLID,
							  Skeleton ? *Skeleton : DEFAULT_SKELETON);
   return SceneObjectPtr(f);
  }
  return SceneObjectPtr();
}


void FaceSet::Builder::destroy( ) {
	FaceSetDestroy( );
}

void FaceSet::Builder::FaceSetDestroy( ) {
  MeshDestroy();
  if (IndexList) delete IndexList;
  if (NormalIndexList) delete NormalIndexList;
  if (ColorIndexList) delete ColorIndexList;
  if (TexCoordIndexList) delete TexCoordIndexList;
}


bool FaceSet::Builder::isValid( ) const {
	return FaceSetValid( );
}
bool FaceSet::Builder::FaceSetValid( ) const {
  if(!MeshValid())return false;

  // PointList field
  // Size check
  uint_t _pointListSize = (*PointList)->getSize();
  if (_pointListSize < 3) {
    genMessage(ERRORMSG(INVALID_FIELD_SIZE_sss),"FaceSet","PointList","Must have more than 2 Points.");
    return false;
  };

  // IndexList field
  // Initialization check
  if (! IndexList) {
    genMessage(ERRORMSG(UNINITIALIZED_FIELD_ss),"FaceSet","IndexList");
    return false;
  };
  // Size check
  uint_t _indexListSize = (*IndexList)->getSize();
  if (_indexListSize < 1) {
    genMessage(ERRORMSG(INVALID_FIELD_SIZE_sss),"FaceSet","IndexList","Number of Index must be greater than 0.");
    return false;
  };

  // Indices check
  IndexArray::iterator _it = (*IndexList)->getBegin();
  for (uint_t _i = 0; _i < _indexListSize; ++_i,++_it) {
    // Max index check
	  if (*(std::max_element(_it->getBegin(),_it->getEnd())) >= _pointListSize) {
	genMessage
	    (ERRORMSG(INVALID_FIELD_ITH_VALUE_ssss),"FaceSet","IndexList",number(_i+1).c_str(),"Do not represent any point of the list.");
	// _it = (*IndexList)->Erase(_it);
      }
      // Unique elements check
      else if (! _it->isUnique()) {
	  genMessage
	      (WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"FaceSet","IndexList",number(_i+1).c_str(),"Redundance : Already declared.");
	  // _it = (*IndexList)->Erase(_it);
      }
  };
  
  if (NormalList) {
	  uint_t _normalListSize = (*NormalList)->getSize();
	  if (!NormalIndexList){
		  if(NormalPerVertex && !*NormalPerVertex){
			  if(_normalListSize != _indexListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"FaceSet","NormalList","Number of normals must be compatible to IndexList size.");
				return false;
			  }
		  }
		  else
		  {
			  if(_normalListSize != _pointListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"FaceSet","NormalList","Number of normals must be compatible to PointList size.");
				return false;
			  }
		  }
	  }
	  else {
		  uint_t _normalIndexListSize = (*NormalIndexList)->getSize();
		  if(NormalPerVertex && !*NormalPerVertex){
			genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"FaceSet","NormalPerVertex","If NormalIndexList is specified, NormalPerVertex should be True.");
			return false;
		  }
		  else
		  {
			  if(_normalIndexListSize != _indexListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"FaceSet","NormalIndexList","Number of normals indices must be compatible to IndexList size.");
				return false;
			  }
		  }
		  // NormalIndexList values check
		  IndexArray::iterator _it = (*NormalIndexList)->getBegin();
		  for (uint_t _i = 0; _i < _normalIndexListSize; ++_i,++_it) {
			// Max index check
			if (*(std::max_element(_it->getBegin(),_it->getEnd())) >= _normalListSize) {
				genMessage
					(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"FaceSet","NormalIndexList",number(_i+1).c_str(),"Do not represent any normal of the list.");
				return false;
			}
			// Size index check
			if (_it->getSize() != (*IndexList)->getAt(_i).getSize()) {
				genMessage
					(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"FaceSet","NormalIndexList",number(_i+1).c_str(),"Normal indice size do no reflect actual polygon indice size.");
				return false;
			}
	  	 }

	  }
  }
  else 
  {
	  if (NormalIndexList){
		genMessage(ERRORMSG(UNINITIALIZED_FIELD_ss),"FaceSet","NormalList");
		return false;
	  }
  }

  if (ColorList) {
	  uint_t _colorListSize = (*ColorList)->getSize();
	  if (!ColorIndexList){
		  if(ColorPerVertex && !*ColorPerVertex){
			  if(_colorListSize != _indexListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"FaceSet","ColorList","Number of colors must be compatible to IndexList size.");
				return false;
			  }
		  }
		  else
		  {
			  if(_colorListSize != _pointListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"FaceSet","ColorList","Number of colors must be compatible to PointList size.");
				return false;
			  }
		  }
	  }
	  else {
		  uint_t _colorIndexListSize = (*ColorIndexList)->getSize();
		  if(ColorPerVertex && !*ColorPerVertex){
			genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"FaceSet","ColorPerVertex","If ColorIndexList is specified, ColorPerVertex should be True.");
			return false;
		  }
		  else
		  {
			  if(_colorIndexListSize != _indexListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"FaceSet","ColorIndexList","Number of colors indices must be compatible to IndexList size.");
				return false;
			  }
		  }
		  // ColorIndexList values check
		  IndexArray::iterator _it = (*ColorIndexList)->getBegin();
		  for (uint_t _i = 0; _i < _colorIndexListSize; ++_i,++_it) {
			// Max index check
			if (*(std::max_element(_it->getBegin(),_it->getEnd())) >= _colorListSize) {
				genMessage
					(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"FaceSet","ColorIndexList",number(_i+1).c_str(),"Do not represent any color of the list.");
				return false;
			}
			// Size index check
			if (_it->getSize() != (*IndexList)->getAt(_i).getSize()) {
				genMessage
					(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"FaceSet","ColorIndexList",number(_i+1).c_str(),"Color indice size do no reflect actual polygon indice size.");
				return false;
			}
	  	 }

	  }
  }
  else 
  {
	  if (ColorIndexList){
		genMessage(ERRORMSG(UNINITIALIZED_FIELD_ss),"FaceSet","ColorList");
		return false;
	  }
  }

  if (TexCoordList) {
	  uint_t _texCoordListSize = (*TexCoordList)->getSize();
	  if (!TexCoordIndexList){
		  if(_texCoordListSize != _pointListSize){
			genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"FaceSet","TexCoordList","Number of TexCoord must be compatible to PointList size.");
			return false;
		  }
	  }
	  else {
		  uint_t _texCoordIndexListSize = (*TexCoordIndexList)->getSize();
		  if(_texCoordIndexListSize != _indexListSize){
			genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"FaceSet","TexCoordIndexList","Number of TexCoord indices must be compatible to IndexList size.");
			return false;
		  }
		  // TexCoordIndexList values check
		  IndexArray::iterator _it = (*TexCoordIndexList)->getBegin();
		  for (uint_t _i = 0; _i < _texCoordIndexListSize; ++_i,++_it) {
			// Max index check
			if (*(std::max_element(_it->getBegin(),_it->getEnd())) >= _texCoordListSize) {
				genMessage
					(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"FaceSet","TexCoordIndexList",number(_i+1).c_str(),"Do not represent any TexCoord of the list.");
				return false;
			}
			// Size index check
			if (_it->getSize() != (*IndexList)->getAt(_i).getSize()) {
				genMessage
					(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"FaceSet","TexCoordIndexList",number(_i+1).c_str(),"TexCoord indice size do no reflect actual polygon indice size.");
				return false;
			}
	  	 }

	  }
  }
  else 
  {
	  if (TexCoordIndexList){
		genMessage(ERRORMSG(UNINITIALIZED_FIELD_ss),"FaceSet","TexCoordList");
		return false;
	  }
  }

  return true;
}


/* ----------------------------------------------------------------------- */

FaceSet::FaceSet() :
  Mesh(),
    __indexList(),
	__normalIndexList(),
	__texCoordIndexList(),
	__colorIndexList() {
}

FaceSet::FaceSet( const Point3ArrayPtr& points,
                  const IndexArrayPtr& indices,
				  bool normalPerVertex,
				  bool ccw,
				  bool solid,
				  const PolylinePtr& skeleton  ) :
  Mesh(points,normalPerVertex,ccw,solid,skeleton),
  __indexList(indices) {
  GEOM_ASSERT(isValid());
}

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
  Mesh(points,normals,colors,texCoord,normalPerVertex,colorPerVertex,ccw,solid,skeleton),
  __indexList(indices),
  __normalIndexList(nomalIndices),
  __colorIndexList(colorIndices),
  __texCoordIndexList(texCoordIndices) {
  GEOM_ASSERT(isValid());
}

/** Constructs a FaceSet from a TriangleSet.  */
FaceSet::FaceSet(const TriangleSet& obj) :
  Mesh(obj),
  __indexList( ),
  __normalIndexList(),
  __colorIndexList(),
  __texCoordIndexList() {
  if( obj.getIndexList()) __indexList = IndexArrayPtr(new IndexArray(*obj.getIndexList())) ;
  if( obj.getNormalIndexList()) __normalIndexList = IndexArrayPtr(new IndexArray(*obj.getNormalIndexList())) ;
  if( obj.getColorIndexList()) __colorIndexList = IndexArrayPtr(new IndexArray(*obj.getColorIndexList())) ;
  if( obj.getTexCoordIndexList()) __texCoordIndexList = IndexArrayPtr(new IndexArray(*obj.getTexCoordIndexList())) ;
  GEOM_ASSERT(isValid());
}

  /** Constructs a FaceSet from a TriangleSet.  */
FaceSet::FaceSet(const QuadSet& obj)  :
  Mesh(obj),
  __indexList(),
  __normalIndexList(),
  __colorIndexList(),
  __texCoordIndexList() {
  GEOM_ASSERT(isValid());
  if( obj.getIndexList()) __indexList = IndexArrayPtr(new IndexArray(*obj.getIndexList())) ;
  if( obj.getNormalIndexList()) __normalIndexList = IndexArrayPtr(new IndexArray(*obj.getNormalIndexList())) ;
  if( obj.getColorIndexList()) __colorIndexList = IndexArrayPtr(new IndexArray(*obj.getColorIndexList())) ;
  if( obj.getTexCoordIndexList()) __texCoordIndexList = IndexArrayPtr(new IndexArray(*obj.getTexCoordIndexList())) ;
}


FaceSet::~FaceSet( ) {
}

bool FaceSet::isValid( ) const {
  Builder _builder;
  _builder.NormalPerVertex = const_cast<bool *>(&__normalPerVertex);
  _builder.ColorPerVertex = const_cast<bool *>(&__colorPerVertex);
  if(__pointList)_builder.PointList = const_cast<Point3ArrayPtr *>(&__pointList);
  if(__indexList)_builder.IndexList = const_cast<IndexArrayPtr *>(&__indexList);
  if(__skeleton)_builder.Skeleton = const_cast<PolylinePtr *>(&__skeleton);
  if(__normalList)_builder.NormalList = const_cast<Point3ArrayPtr *>(&__normalList);
  if(__texCoordList)_builder.TexCoordList = const_cast<Point2ArrayPtr *>(&__texCoordList);
  if(__colorList)_builder.ColorList = const_cast<Color4ArrayPtr *>(&__colorList);
  if(__normalIndexList) _builder.NormalIndexList = const_cast<IndexArrayPtr *>(&__normalIndexList);
  if(__texCoordIndexList) _builder.TexCoordIndexList = const_cast<IndexArrayPtr *>(&__texCoordIndexList);
  if(__colorIndexList) _builder.ColorIndexList = const_cast<IndexArrayPtr *>(&__colorIndexList);
  return _builder.isValid();
}

bool FaceSet::apply( Action& action ) {
  return action.process(this);      
}

SceneObjectPtr 
FaceSet::copy() const{
  FaceSetPtr ptr(new FaceSet(*this));
  if(__pointList)ptr->getPointList() = Point3ArrayPtr(new Point3Array(*__pointList));
  if(__indexList)ptr->getIndexList() = IndexArrayPtr(new IndexArray(*__indexList));
  if(__skeleton)ptr->getSkeleton()= dynamic_pointer_cast<Polyline>(__skeleton->copy());
  if(__normalList)ptr->getNormalList() = Point3ArrayPtr(new Point3Array(*__normalList));
  if(__texCoordList)ptr->getTexCoordList() = Point2ArrayPtr(new Point2Array(*__texCoordList));
  if(__colorList)ptr->getColorList() = Color4ArrayPtr(new Color4Array(*__colorList));
  if(__normalIndexList) ptr->getNormalIndexList() = IndexArrayPtr(new IndexArray(*__normalIndexList));
  if(__texCoordIndexList) ptr->getTexCoordIndexList() = IndexArrayPtr(new IndexArray(*__texCoordIndexList));
  if(__colorIndexList) ptr->getColorIndexList() = IndexArrayPtr(new IndexArray(*__colorIndexList));
  return SceneObjectPtr(ptr);
}

ExplicitModelPtr 
FaceSet::transform( const Transformation3DPtr& transformation ) const {
  GEOM_ASSERT(transformation);

  PolylinePtr _tSkeleton = __skeleton;
  if (_tSkeleton)
    _tSkeleton= dynamic_pointer_cast<Polyline>(__skeleton->transform(transformation));

  Point3ArrayPtr _n = __normalList;
  if(_n){
      _n = transformation->transform(__normalList);
      _n->normalize();
  }

  return ExplicitModelPtr(new FaceSet(transformation->transform(__pointList),
										  __indexList,
                                          _n,
										  __normalIndexList,
										  __colorList,
										  __colorIndexList,
										  __texCoordList,
										  __texCoordIndexList,
										  __normalPerVertex,
										  __colorPerVertex,
                                          __ccw,
                                          __solid,
                                          _tSkeleton));
}

/*
bofstream& FaceSet::write( bofstream& stream ) const {
  GEOM_ASSERT(stream);

  // Points
  uint_t _pointListSize = __pointList->getSize();
  stream << _pointListSize;
  for (uint_t _l = 0; _l < _pointListSize; _l++) {
    const Vector3& _point = __pointList->getAt(_l);
    stream << _point.x() << _point.y() << _point.z();
  };

  // Faces
  uint_t _indexListSize = __indexList->getSize();
  stream << _indexListSize;
  for (uint_t _i = 0; _i < _indexListSize; _i++) {
    const Index& _index = __indexList->getAt(_i);
    uint_t _indexSize = _index.getSize();
    stream << _indexSize;
    for (uint_t _j = 0; _j < _indexSize; _j++)
      stream << _index.getAt(_j);
  };

  // CCW
  stream << __ccw;

  // Solid
  stream << __solid;

  // Skeleton
  if (__skeleton) {
    stream << true;
    const Point3ArrayPtr& _skelPointList = __skeleton->getPointList();
    uint_t _skelPointListSize = _skelPointList->getSize();
    for (uint_t _i = 0; _i < _skelPointListSize; _i++) {
      const Vector3& _point = _skelPointList->getAt(_i);
      stream << _point.x() << _point.y() << _point.z();
    };
  }
  else
    stream << false;

  return stream;
}
*/

/* ----------------------------------------------------------------------- */

Point3ArrayPtr FaceSet::computeNormalPerVertex() const {
    Point3ArrayPtr normalList(new Point3Array(__pointList->getSize())); 
    for(uint_t j=0; j < __indexList->getSize(); j++){ 
	  Vector3 _norm = cross(getFacePointAt(j,__ccw ? 1 : 2) - getFacePointAt(j,0), 
		getFacePointAt(j,__ccw ? 2 : 1) - getFacePointAt(j,0)); 
	  for(uint_t i = 0; i <__indexList->getAt(j).getSize() ; i++){ 
		uint_t _index = __indexList->getAt(j).getAt(i); 
		normalList->setAt(_index,normalList->getAt(_index)+_norm); 
	  } 
    }
    for(Point3Array::iterator _it=normalList->getBegin();_it!=normalList->getEnd();_it++)
	_it->normalize();
	return normalList;
}

Point3ArrayPtr FaceSet::computeNormalPerFace() const {
    Point3ArrayPtr normalList(new Point3Array(__indexList->getSize())); 
    for(uint_t j=0; j < __indexList->getSize(); j++){ 
	    normalList->setAt(j,cross(getFacePointAt(j,__ccw ? 1 : 2) - getFacePointAt(j,0), 
			      getFacePointAt(j,__ccw ? 2 : 1) - getFacePointAt(j,0))); 
    }
    for(Point3Array::iterator _it=normalList->getBegin();_it!=normalList->getEnd();_it++)
	_it->normalize();
	return normalList;
}

/* ----------------------------------------------------------------------- */

const IndexArrayPtr& FaceSet::getIndexList( ) const {
  return __indexList;
}

IndexArrayPtr& FaceSet::getIndexList( ) {
  return __indexList;
} 

const Index& FaceSet::getIndexListAt( uint_t i ) const {
  GEOM_ASSERT(i < __indexList->getSize());
  return __indexList->getAt(i);
}

Index& FaceSet::getIndexListAt( uint_t i ){
  GEOM_ASSERT(i < __indexList->getSize());
  return __indexList->getAt(i);
}

uint_t FaceSet::getIndexListSize( ) const {
  return __indexList->getSize();
}

/* ----------------------------------------------------------------------- */

const Vector3& FaceSet::getFacePointAt( uint_t i, uint_t j ) const {
  GEOM_ASSERT(__pointList.isValid());
  GEOM_ASSERT(i < __indexList->getSize());
  GEOM_ASSERT(j < __indexList->getAt(i).getSize());
  return __pointList->getAt(__indexList->getAt(i).getAt(j));
}

const Vector3& FaceSet::getNormalAt( uint_t i ) const {
  GEOM_ASSERT(__normalList.isValid());
  GEOM_ASSERT(i < __normalList->getSize());
  return __normalList->getAt(i);
}

const Vector3& FaceSet::getNormalAt( uint_t i, uint_t j )  const {
  GEOM_ASSERT(__normalList.isValid());
  GEOM_ASSERT(i < __indexList->getSize());
  GEOM_ASSERT(j < __indexList->getAt(i).getSize());
  if( __normalIndexList)
	  return __normalList->getAt(__normalIndexList->getAt(i).getAt(j));
  else 
	  return __normalList->getAt(__indexList->getAt(i).getAt(j));
}

const Color4& FaceSet::getColorAt( uint_t i ) const {
  GEOM_ASSERT(__colorList.isValid());
  GEOM_ASSERT(i < __indexList->getSize());
  return __colorList->getAt(i);
}

const Color4& FaceSet::getColorAt( uint_t i, uint_t j ) const {
  GEOM_ASSERT(__colorList.isValid());
  GEOM_ASSERT(i < __indexList->getSize());
  GEOM_ASSERT(j < __indexList->getAt(i).getSize());
  if (__colorIndexList)
	return __colorList->getAt(__colorIndexList->getAt(i).getAt(j));
  else
	return __colorList->getAt(__indexList->getAt(i).getAt(j));
}

const Vector2& FaceSet::getTexCoordAt( uint_t i ) const {
  GEOM_ASSERT(__texCoordList.isValid());
  GEOM_ASSERT(i < __texCoordList->getSize());
  return __texCoordList->getAt(i);
}

const Vector2& FaceSet::getTexCoordAt( uint_t i, uint_t j ) const {
  GEOM_ASSERT(__texCoordList.isValid());
  GEOM_ASSERT(i < __indexList->getSize());
  GEOM_ASSERT(j < __indexList->getAt(i).getSize());
  if (__texCoordIndexList)
	return __texCoordList->getAt(__texCoordIndexList->getAt(i).getAt(j));
  else
	return __texCoordList->getAt(__indexList->getAt(i).getAt(j));
}

/* ----------------------------------------------------------------------- */


