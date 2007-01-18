/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
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





#include "geom_triangleset.h"
#include "util_messages.h"
#include "Tools/util_string.h"
#include "Tools/bfstream.h"
#include "geom_pointarray.h"
#include "geom_polyline.h"
#include "geom_transformed.h"
#include "geom_indexarray.h"

GEOM_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


TriangleSet::Builder::Builder() :
  Mesh::Builder(),
  IndexList(0),
  NormalIndexList(0),
  ColorIndexList(0),
  TexCoordIndexList(0)
  {
}


TriangleSet::Builder::~Builder() {
  //nothing to do
}


SceneObjectPtr TriangleSet::Builder::build( ) const {
  if (isValid()){
	TriangleSet * t = new TriangleSet(*PointList,
									  *IndexList,
									  NormalList?*NormalList:NULL,
									  NormalIndexList?*NormalIndexList:NULL,
									  ColorList?*ColorList:NULL,
									  ColorIndexList?*ColorIndexList:NULL,
									  TexCoordList?*TexCoordList:NULL,
									  TexCoordIndexList?*TexCoordIndexList:NULL,
									  NormalPerVertex? *NormalPerVertex : DEFAULT_NORMALPERVERTEX,
									  ColorPerVertex? *ColorPerVertex: DEFAULT_COLORPERVERTEX,
									  CCW ? *CCW : DEFAULT_CCW,
									  Solid ? *Solid : DEFAULT_SOLID,
									  Skeleton ? *Skeleton : DEFAULT_SKELETON);
    return SceneObjectPtr(t);
  }
  return SceneObjectPtr();
}


void TriangleSet::Builder::destroy( ) {
  MeshDestroy();
  if (IndexList) delete IndexList;
  if (NormalIndexList) delete NormalIndexList;
  if (ColorIndexList) delete ColorIndexList;
  if (TexCoordIndexList) delete TexCoordIndexList;
}


bool TriangleSet::Builder::isValid( ) const {
  if(!MeshValid()) return false;

  // PointList size check
  uint32_t _pointListSize = (*PointList)->getSize();
  if (_pointListSize < 3) {
    genMessage(ERRORMSG(INVALID_FIELD_SIZE_sss),"TriangleSet","PointList","Must have more than 2 points.");
    return false;
  };

  // IndexList field
  if (! IndexList) {
    genMessage(ERRORMSG(UNINITIALIZED_FIELD_ss),"TriangleSet","IndexList");
    return false;
  };

  // IndexList size check
  uint32_t _indexListSize = (*IndexList)->getSize();
  if (_indexListSize < 1) {
    genMessage(ERRORMSG(INVALID_FIELD_SIZE_sss),"TriangleSet","IndexList","Number of Index must be greater than 0.");
    return false;
  };

  // IndexList values check
  for (uint32_t _i = 0; _i < _indexListSize; _i++) {
    // Max index check
    if ((*(*IndexList)->getAt(_i).getMax()) >= _pointListSize) {
        genMessage
            (WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"TriangleSet","IndexList",number(_i+1).c_str(),"Do not represent any point of the list.");
        return false;
    };
    // Unique elements check
    if (! (*IndexList)->getAt(_i).isUnique()) {
        genMessage
            (WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"TriangleSet","IndexList",number(_i+1).c_str(),"Redundance : Already declared.");
    };
  };

  if (NormalList) {
	  uint32_t _normalListSize = (*NormalList)->getSize();
	  if (!NormalIndexList){
		  if(NormalPerVertex && !*NormalPerVertex){
			  if(_normalListSize != _indexListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","NormalList","Number of normals must be compatible to IndexList size.");
				return false;
			  }
		  }
		  else
		  {
			  if(_normalListSize != _pointListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","NormalList","Number of normals must be compatible to PointList size.");
				return false;
			  }
		  }
	  }
	  else {
		  uint32_t _normalIndexListSize = (*NormalIndexList)->getSize();
		  if(NormalPerVertex && !*NormalPerVertex){
			genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","NormalPerVertex","If NormalIndexList is specified, NormalPerVertex should be True.");
			return false;
		  }
		  else
		  {
			  if(_normalIndexListSize != _indexListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","NormalIndexList","Number of normals indices must be compatible to IndexList size.");
				return false;
			  }
		  }
		  // NormalIndexList values check
		  for (uint32_t _i = 0; _i < _normalIndexListSize; _i++) {
			// Max index check
			if ((*(*NormalIndexList)->getAt(_i).getMax()) >= _normalListSize) {
				genMessage
					(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"TriangleSet","NormalIndexList",number(_i+1).c_str(),"Do not represent any normal of the list.");
				return false;
			}
	  	 }

	  }
  }
  else 
  {
	  if (NormalIndexList){
		genMessage(ERRORMSG(UNINITIALIZED_FIELD_ss),"TriangleSet","NormalList");
		return false;
	  }
  }

  if (ColorList) {
	  uint32_t _colorListSize = (*ColorList)->getSize();
	  if (!ColorIndexList){
		  if(ColorPerVertex && !*ColorPerVertex){
			  if(_colorListSize != _indexListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","ColorList","Number of colors must be compatible to IndexList size.");
				return false;
			  }
		  }
		  else
		  {
			  if(_colorListSize != _pointListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","ColorList","Number of colors must be compatible to PointList size.");
				return false;
			  }
		  }
	  }
	  else {
		  uint32_t _colorIndexListSize = (*ColorIndexList)->getSize();
		  if(ColorPerVertex && !*ColorPerVertex){
			genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","ColorPerVertex","If ColorIndexList is specified, ColorPerVertex should be True.");
			return false;
		  }
		  else
		  {
			  if(_colorIndexListSize != _indexListSize){
				genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","ColorIndexList","Number of colors indices must be compatible to IndexList size.");
				return false;
			  }
		  }
		  // ColorIndexList values check
		  for (uint32_t _i = 0; _i < _colorIndexListSize; _i++) {
			// Max index check
			if ((*(*ColorIndexList)->getAt(_i).getMax()) >= _colorListSize) {
				genMessage
					(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"TriangleSet","ColorIndexList",number(_i+1).c_str(),"Do not represent any color of the list.");
				return false;
			}
	  	 }

	  }
  }
  else 
  {
	  if (ColorIndexList){
		genMessage(ERRORMSG(UNINITIALIZED_FIELD_ss),"TriangleSet","ColorList");
		return false;
	  }
  }

  if (TexCoordList) {
	  uint32_t _texCoordListSize = (*TexCoordList)->getSize();
	  if (!TexCoordIndexList){
		  if(_texCoordListSize != _pointListSize){
			genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","TexCoordList","Number of TexCoord must be compatible to PointList size.");
			return false;
		  }
	  }
	  else {
		  uint32_t _texCoordIndexListSize = (*TexCoordIndexList)->getSize();
		  if(_texCoordIndexListSize != _indexListSize){
			genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"TriangleSet","TexCoordIndexList","Number of TexCoord indices must be compatible to IndexList size.");
			return false;
		  }
		  // TexCoordIndexList values check
		  for (uint32_t _i = 0; _i < _texCoordIndexListSize; _i++) {
			// Max index check
			if ((*(*TexCoordIndexList)->getAt(_i).getMax()) >= _texCoordListSize) {
				genMessage
					(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"TriangleSet","TexCoordIndexList",number(_i+1).c_str(),"Do not represent any TexCoord of the list.");
				return false;
			}
	  	 }

	  }
  }
  else 
  {
	  if (TexCoordIndexList){
		genMessage(ERRORMSG(UNINITIALIZED_FIELD_ss),"TriangleSet","TexCoordList");
		return false;
	  }
  }

  return true;
}

/* ----------------------------------------------------------------------- */

TriangleSet::TriangleSet() :
    Mesh(),
    __indexList(0),
	__normalIndexList(0),
	__texCoordIndexList(0),
	__colorIndexList(0){
}

TriangleSet::TriangleSet( const Point3ArrayPtr& points,
                          const Index3ArrayPtr& indices,
						  bool normalPerVertex,
                          bool ccw,
                          bool solid,
                          const GeomPolylinePtr& skeleton ) :
    Mesh(points,normalPerVertex,ccw,solid,skeleton),
    __indexList(indices),
	__normalIndexList(0),
	__texCoordIndexList(0),
	__colorIndexList(0){
    GEOM_ASSERT(isValid());
}

TriangleSet::TriangleSet( const Point3ArrayPtr& points,
						  const Index3ArrayPtr& indices,
						  const Point3ArrayPtr& normals ,
						  const Index3ArrayPtr& nomalIndices,
						  const Color4ArrayPtr& colors,
						  const Index3ArrayPtr& colorIndices,
						  const Point2ArrayPtr& texCoord,
						  const Index3ArrayPtr& texCoordIndices,
						  bool normalPerVertex,
						  bool colorPerVertex,
						  bool ccw,
						  bool solid,
						  const GeomPolylinePtr& skeleton) :
    Mesh(points,normals,colors,texCoord,normalPerVertex,colorPerVertex,ccw,solid,skeleton),
    __indexList(indices),
	__normalIndexList(nomalIndices),
	__colorIndexList(colorIndices),
	__texCoordIndexList(texCoordIndices){
    GEOM_ASSERT(isValid());
}

TriangleSet::~TriangleSet( ) {
}

bool TriangleSet::apply( Action& action ) {
  return action.process(this);
}

const Index3ArrayPtr& 
TriangleSet::getIndexList( ) const {
  return __indexList;
} 

Index3ArrayPtr&
TriangleSet::getIndexList( ) {
  return __indexList;
}

const Index3& 
TriangleSet::getIndexListAt( uint32_t i ) const {
  GEOM_ASSERT(i < __indexList->getSize());
  return __indexList->getAt(i);
}

Index3& 
TriangleSet::getIndexListAt( uint32_t i ) {
  GEOM_ASSERT(i < __indexList->getSize());
  return __indexList->getAt(i);
}

uint32_t 
TriangleSet::getIndexListSize( ) const {
  return __indexList->getSize();
}

SceneObjectPtr 
TriangleSet::copy() const{
  TriangleSetPtr ptr(new TriangleSet(*this));
  if(__pointList)ptr->getPointList() = Point3ArrayPtr(new Point3Array(*__pointList));
  if(__indexList)ptr->getIndexList() = Index3ArrayPtr(new Index3Array(*__indexList));
  if(__skeleton)ptr->getSkeleton().cast(__skeleton->copy());
  if(__normalList)ptr->getNormalList() = Point3ArrayPtr(new Point3Array(*__normalList));
  if(__texCoordList)ptr->getTexCoordList() = Point2ArrayPtr(new Point2Array(*__texCoordList));
  if(__colorList)ptr->getColorList() = Color4ArrayPtr(new Color4Array(*__colorList));
  if(__normalIndexList) ptr->getNormalIndexList() = Index3ArrayPtr(new Index3Array(*__normalIndexList));
  if(__texCoordIndexList) ptr->getTexCoordIndexList() = Index3ArrayPtr(new Index3Array(*__texCoordIndexList));
  if(__colorIndexList) ptr->getColorIndexList() = Index3ArrayPtr(new Index3Array(*__colorIndexList));
  return SceneObjectPtr(ptr);
}

bool TriangleSet::isValid( ) const {
  Builder _builder;
  _builder.NormalPerVertex = const_cast<bool *>(&__normalPerVertex);
  _builder.ColorPerVertex = const_cast<bool *>(&__colorPerVertex);
  _builder.PointList = const_cast<Point3ArrayPtr *>(&__pointList);
  _builder.IndexList = const_cast<Index3ArrayPtr *>(&__indexList);
  if(__skeleton)_builder.Skeleton = const_cast<GeomPolylinePtr *>(&__skeleton);
  if(__normalList)_builder.NormalList = const_cast<Point3ArrayPtr *>(&__normalList);
  if(__texCoordList)_builder.TexCoordList = const_cast<Point2ArrayPtr *>(&__texCoordList);
  if(__colorList)_builder.ColorList = const_cast<Color4ArrayPtr *>(&__colorList);
  if(__normalIndexList) _builder.NormalIndexList = const_cast<Index3ArrayPtr *>(&__normalIndexList);
  if(__texCoordIndexList) _builder.TexCoordIndexList = const_cast<Index3ArrayPtr *>(&__texCoordIndexList);
  if(__colorIndexList) _builder.ColorIndexList = const_cast<Index3ArrayPtr *>(&__colorIndexList);
  return _builder.isValid();
}


ExplicitModelPtr
TriangleSet::transform( const Transformation3DPtr& transformation ) const {
  GEOM_ASSERT(transformation);

  GeomPolylinePtr _tSkeleton = __skeleton;
  if (_tSkeleton)
    _tSkeleton.cast(__skeleton->transform(transformation));

  Point3ArrayPtr _n = __normalList;
  if(_n){
      _n = transformation->transform(__normalList);
      _n->normalize();
  }
   
  return ExplicitModelPtr(new TriangleSet(transformation->transform(__pointList),
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
bofstream& TriangleSet::write( bofstream& stream ) const {
  GEOM_ASSERT(stream);

  // Points
  uint32_t _pointListSize = __pointList->getSize();
  stream << _pointListSize;
  for (uint32_t _i1 = 0; _i1 < _pointListSize; _i1++) {
    const Vector3& _point = __pointList->getAt(_i1);
    stream << _point.x() << _point.y() << _point.z();
  };

  // Faces
  uint32_t _indexListSize = __indexList->getSize();
  stream << _indexListSize;
  for (uint32_t _i = 0; _i < _indexListSize; _i++) {
    const Index3& _index = __indexList->getAt(_i);
    stream << _index.getAt(0) << _index.getAt(1) << _index.getAt(2);
  };

  // CCW
  stream << __ccw;

  // Solid
  stream << __solid;

  // Skeleton
  if (__skeleton) {
    stream << true;
    const Point3ArrayPtr& _skelPointList = __skeleton->getPointList();
    uint32_t _skelPointListSize = _skelPointList->getSize();
    for (uint32_t _i2 = 0; _i2 < _skelPointListSize; _i2++) {
      const Vector3& _point = _skelPointList->getAt(_i2);
      stream << _point.x() << _point.y() << _point.z();
    };
  }
  else
    stream << false;

  return stream;
}*/


/* ----------------------------------------------------------------------- */

Point3ArrayPtr 
TriangleSet::computeNormalPerVertex() const {
    Point3ArrayPtr normalList(new Point3Array(__pointList->getSize()));
    for(uint32_t j=0; j < __indexList->getSize(); j++){
        Vector3 _norm = cross(getPointAt(j,__ccw ? 1 : 2) - getPointAt(j,0),
                              getPointAt(j,__ccw ? 2 : 1) - getPointAt(j,0));
        for(uint32_t i = 0; i < 3; i++){
            uint32_t _index = __indexList->getAt(j).getAt(i);
            normalList->setAt(_index,normalList->getAt(_index)+_norm);
        }
    }
    for(Point3Array::iterator _it=normalList->getBegin();_it!=normalList->getEnd();_it++)
        _it->normalize();
	return normalList;
}

Point3ArrayPtr 
TriangleSet::computeNormalPerFace() const {
    Point3ArrayPtr normalList(new Point3Array(__indexList->getSize())); 
    for(uint32_t j=0; j < __indexList->getSize(); j++){ 
	    normalList->setAt(j,cross(getPointAt(j,__ccw ? 1 : 2) - getPointAt(j,0), 
			      getPointAt(j,__ccw ? 2 : 1) - getPointAt(j,0))); 
    }
    for(Point3Array::iterator _it=normalList->getBegin();_it!=normalList->getEnd();_it++)
	_it->normalize();
	return normalList;
}

/* ----------------------------------------------------------------------- */

const Vector3& TriangleSet::getPointAt( uint32_t i ) const {
  GEOM_ASSERT(__pointList.isValid());
  GEOM_ASSERT(i < __pointList->getSize());
  return __pointList->getAt(i);
}

const Vector3& TriangleSet::getPointAt( uint32_t i, uint32_t j ) const {
  GEOM_ASSERT(__pointList.isValid());
  GEOM_ASSERT(i < __indexList->getSize());
  GEOM_ASSERT(j < 3);
  return __pointList->getAt(__indexList->getAt(i).getAt(j));
}

const Vector3& TriangleSet::getNormalAt( uint32_t i ) const {
  GEOM_ASSERT(__normalList.isValid());
  GEOM_ASSERT(i < __normalList->getSize());
  return __normalList->getAt(i);
}

const Vector3& TriangleSet::getNormalAt( uint32_t i, uint32_t j )  const {
  GEOM_ASSERT(__normalList.isValid());
  GEOM_ASSERT(i < __indexList->getSize());
  GEOM_ASSERT(j < 3);
  if( __normalIndexList)
	  return __normalList->getAt(__normalIndexList->getAt(i).getAt(j));
  else 
	  return __normalList->getAt(__indexList->getAt(i).getAt(j));
}

const Color4& TriangleSet::getColorAt( uint32_t i ) const {
  GEOM_ASSERT(__colorList.isValid());
  GEOM_ASSERT(i < __indexList->getSize());
  return __colorList->getAt(i);
}

const Color4& TriangleSet::getColorAt( uint32_t i, uint32_t j ) const {
  GEOM_ASSERT(__colorList.isValid());
  GEOM_ASSERT(i < __indexList->getSize());
  GEOM_ASSERT(j < 3);
  if (__colorIndexList)
	return __colorList->getAt(__colorIndexList->getAt(i).getAt(j));
  else
	return __colorList->getAt(__indexList->getAt(i).getAt(j));
}

const Vector2& TriangleSet::getTexCoordAt( uint32_t i ) const {
  GEOM_ASSERT(__texCoordList.isValid());
  GEOM_ASSERT(i < __texCoordList->getSize());
  return __texCoordList->getAt(i);
}

const Vector2& TriangleSet::getTexCoordAt( uint32_t i, uint32_t j ) const {
  GEOM_ASSERT(__texCoordList.isValid());
  GEOM_ASSERT(i < __indexList->getSize());
  GEOM_ASSERT(j < 3);
  if (__texCoordIndexList)
	return __texCoordList->getAt(__texCoordIndexList->getAt(i).getAt(j));
  else
	return __texCoordList->getAt(__indexList->getAt(i).getAt(j));
}

/* ----------------------------------------------------------------------- */

