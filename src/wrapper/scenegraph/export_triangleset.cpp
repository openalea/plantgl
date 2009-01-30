/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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

#include <plantgl/scenegraph/geometry/triangleset.h>
#include <plantgl/scenegraph/geometry/quadset.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/geometry/faceset.h>
#include <plantgl/scenegraph/geometry/amapsymbol.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/container/pointarray.h>

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>

#include <boost/python.hpp>
#include <string>
#include <sstream>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace boost::python;

DEF_POINTEE( TriangleSet )
DEF_POINTEE( FaceSet )
DEF_POINTEE( AmapSymbol )

/*
struct tr_pickle_suite : boost::python::pickle_suite
{
	static tuple getinitargs(TriangleSet const& tr)
	{
		return make_tuple();
	}
	static tuple getstate(TriangleSet const& tr)
	{
		int _default = (tr.getPointList()        ?1:0)+
					 (tr.getIndexList()        ?1<<1:0)+
					 (tr.getNormalList()       ?1<<2:0)+
					 (tr.getNormalIndexList()  ?1<<3:0)+
					 (tr.getColorList()        ?1<<4:0)+
					 (tr.getColorIndexList()   ?1<<5:0)+
					 (tr.getTexCoordList()     ?1<<6:0)+
					 (tr.getTexCoordIndexList()?1<<7:0)+
					 (tr.getSkeleton()         ?1<<8:0);
		boost::python::list l;
		l.append(tr.getNormalPerVertex());
		l.append(tr.getColorPerVertex());
		l.append(tr.getCCW());
		l.append(tr.getSolid());
		l.append(_default);

		if(tr.getPointList())l.append(tr.getPointList());
		if(tr.getIndexList())l.append(tr.getIndexList());
		if(tr.getNormalList())l.append(tr.getNormalList());
		if(tr.getNormalIndexList())l.append(tr.getNormalIndexList());
		if(tr.getColorList())l.append(tr.getColorList());
		if(tr.getColorIndexList())l.append(tr.getColorIndexList());
		if(tr.getTexCoordList())l.append(tr.getTexCoordList());
		if(tr.getTexCoordIndexList())l.append(tr.getTexCoordIndexList());
		if(tr.getSkeleton())l.append(tr.getSkeleton());

		return tuple(l);
	}
	static void setstate(TriangleSet& tr, tuple state)
	{

		tr.getNormalPerVertex() = extract<bool>(state[0])();
		tr.getColorPerVertex() =  extract<bool>(state[1])();
		tr.getCCW() = extract<bool>(state[2])();
		tr.getSolid() = extract<bool>(state[3])();

		int _default = extract<int>(state[4])();
		int index = 5;

		if (_default & 1)      tr.getPointList() = extract<Point3ArrayPtr>(state[index++])();
		if (_default & 1 << 1) tr.getIndexList() = extract<Index3ArrayPtr>(state[index++])();
		if (_default & 1 << 2) tr.getNormalList() = extract<Point3ArrayPtr>(state[index++])();
		if (_default & 1 << 3) tr.getNormalIndexList() = extract<Index3ArrayPtr>(state[index++])();
		if (_default & 1 << 4) tr.getColorList() = extract<Color4ArrayPtr>(state[index++])();
		if (_default & 1 << 5) tr.getColorIndexList() = extract<Index3ArrayPtr>(state[index++])();
		if (_default & 1 << 6) tr.getTexCoordList() = extract<Point2ArrayPtr>(state[index++])();
		if (_default & 1 << 7) tr.getTexCoordIndexList() = extract<Index3ArrayPtr>(state[index++])();
		if (_default & 1 << 8) tr.getSkeleton() = extract<PolylinePtr>(state[index++])();
	}
};*/


void export_TriangleSet()
{
  class_<TriangleSet, TriangleSetPtr, bases<Mesh>, boost::noncopyable>
	( "TriangleSet", 
	  "A TriangleSet describes a surface formed by a set of connected triangles, i.e. three sided polygons.\n Triangles are specified using set of tuples of 3 indices (Index3) pointing to a list of points.",
	init<>("TriangleSet()") )
    .def( init<Point3ArrayPtr,Index3ArrayPtr,optional<bool,bool,bool,PolylinePtr> >
				( "TriangleSet(Point3Array points, Index3Array indices [,bool normalPerVertex, bool ccw, bool solid, Polyline skeleton])",
				args("points","indices","normalPerVertex","ccw","solid","skeleton")) )
    .def( init<Point3ArrayPtr, Index3ArrayPtr, Point3ArrayPtr,
	           optional<Index3ArrayPtr,Color4ArrayPtr,Index3ArrayPtr,
			            Point2ArrayPtr,Index3ArrayPtr,
						bool, bool, bool, bool, PolylinePtr> >
			("TriangleSet(Point3Array points, Index3Array indices, Point3Array normal[, Index3Array nomalIndices, "
			 "Color4Array colors, Index3Array colorIndices, Point2Array texCoord, Index3Array texCoordIndices, "
			 "bool normalPerVertex, bool colorPerVertex, bool ccw, bool solid, Polyline skeleton])",
	         args("points","indices","normals","nomalIndices", "colors", "colorIndices",
	              "texCoord", "texCoordIndices", "normalPerVertex", "colorPerVertex", "ccw","solid","skeleton")))

	.def( "copy", &TriangleSet::copy )
	.DEC_PTR_PROPERTY(indexList,            TriangleSet,IndexList,        Index3ArrayPtr)
	.DEC_PTR_PROPERTY_WD(normalIndexList,   TriangleSet,NormalIndexList,  Index3ArrayPtr)
	.DEC_PTR_PROPERTY_WD(colorIndexList,    TriangleSet,ColorIndexList,   Index3ArrayPtr)
	.DEC_PTR_PROPERTY_WD(texCoordIndexList, TriangleSet,TexCoordIndexList,Index3ArrayPtr)
 
    .def( "normalAt",   (const Vector3& (TriangleSet::*)(uint_t,uint_t) const)&TriangleSet::getNormalAt ,  return_value_policy<copy_const_reference>())
    .def( "colorAt",    (const Color4&  (TriangleSet::*)(uint_t,uint_t) const)&TriangleSet::getColorAt,    return_value_policy<copy_const_reference>() )
    .def( "texCoordAt", (const Vector2& (TriangleSet::*)(uint_t,uint_t) const)&TriangleSet::getTexCoordAt, return_value_policy<copy_const_reference>() )

    .def( "normalAt",   (const Vector3& (TriangleSet::*)(uint_t) const)&TriangleSet::getNormalAt,   return_value_policy<copy_const_reference>() )
    .def( "colorAt",    (const Color4&  (TriangleSet::*)(uint_t) const)&TriangleSet::getColorAt,    return_value_policy<copy_const_reference>() )
    .def( "texCoordAt", (const Vector2& (TriangleSet::*)(uint_t) const)&TriangleSet::getTexCoordAt, return_value_policy<copy_const_reference>() )

    .def( "indexAt",        (const Index3&  (TriangleSet::*)(uint_t) const)&TriangleSet::getIndexListAt,         return_value_policy<copy_const_reference>() )
    .def( "normalIndexAt",  (const Index3&  (TriangleSet::*)(uint_t) const)&TriangleSet::getNormalIndexListAt,   return_value_policy<copy_const_reference>() )
    .def( "colorIndexAt",   (const Index3&  (TriangleSet::*)(uint_t) const)&TriangleSet::getColorIndexListAt,    return_value_policy<copy_const_reference>() )
    .def( "texCoordIndexAt",(const Index3&  (TriangleSet::*)(uint_t) const)&TriangleSet::getTexCoordIndexListAt, return_value_policy<copy_const_reference>() )

	// .def_pickle(tr_pickle_suite());
    ;

  implicitly_convertible<TriangleSetPtr, MeshPtr>();
}


void export_FaceSet()
{
  class_<FaceSet, FaceSetPtr, bases<Mesh>, boost::noncopyable>
	( "FaceSet", "A FaceSet describes a surface formed by a set of connected faces.\n Faces are specified using set of tuples of n indices (Index) pointing to a list of points.", init<> ( "FaceSet()"))
    .def( init<Point3ArrayPtr,IndexArrayPtr,optional<bool,bool,bool,PolylinePtr> >
				( "FaceSet(Point3Array points, IndexArray indices [,bool normalPerVertex, bool ccw, bool solid, Polyline skeleton])",
				args("points","indices","normalPerVertex","ccw","solid","skeleton")) )
    .def( init<Point3ArrayPtr, IndexArrayPtr, Point3ArrayPtr,
	           optional<IndexArrayPtr,Color4ArrayPtr,IndexArrayPtr,
			            Point2ArrayPtr,IndexArrayPtr,
						bool, bool, bool, bool, PolylinePtr> >
			("FaceSet (Point3Array points, IndexArray indices, Point3Array normal[, IndexArray nomalIndices, "
			 "Color4Array colors, IndexArray colorIndices, Point2Array texCoord, IndexArray texCoordIndices, "
			 "bool normalPerVertex, bool colorPerVertex, bool ccw, bool solid, Polyline skeleton])",
	         args("points","indices","normals","nomalIndices", "colors", "colorIndices",
	              "texCoord", "texCoordIndices", "normalPerVertex", "colorPerVertex", "ccw","solid","skeleton")))
    .def( "copy", &FaceSet::copy )

	.DEC_PTR_PROPERTY(indexList,            FaceSet,IndexList,        IndexArrayPtr)
	.DEC_PTR_PROPERTY_WD(normalIndexList,   FaceSet,NormalIndexList,  IndexArrayPtr)
	.DEC_PTR_PROPERTY_WD(colorIndexList,    FaceSet,ColorIndexList,   IndexArrayPtr)
	.DEC_PTR_PROPERTY_WD(texCoordIndexList, FaceSet,TexCoordIndexList,IndexArrayPtr)
 
    .def( "normalAt",   (const Vector3& (FaceSet::*)(uint_t,uint_t) const)&FaceSet::getNormalAt ,  return_value_policy<copy_const_reference>())
    .def( "colorAt",    (const Color4&  (FaceSet::*)(uint_t,uint_t) const)&FaceSet::getColorAt,    return_value_policy<copy_const_reference>() )
    .def( "texCoordAt", (const Vector2& (FaceSet::*)(uint_t,uint_t) const)&FaceSet::getTexCoordAt, return_value_policy<copy_const_reference>() )

    .def( "normalAt",   (const Vector3& (FaceSet::*)(uint_t) const)&FaceSet::getNormalAt,   return_value_policy<copy_const_reference>() )
    .def( "colorAt",    (const Color4&  (FaceSet::*)(uint_t) const)&FaceSet::getColorAt,    return_value_policy<copy_const_reference>() )
    .def( "texCoordAt", (const Vector2& (FaceSet::*)(uint_t) const)&FaceSet::getTexCoordAt, return_value_policy<copy_const_reference>() )

    .def( "indexAt",        (const Index&  (FaceSet::*)(uint_t) const)&FaceSet::getIndexListAt,         return_value_policy<copy_const_reference>() )
    .def( "normalIndexAt",  (const Index&  (FaceSet::*)(uint_t) const)&FaceSet::getNormalIndexListAt,   return_value_policy<copy_const_reference>() )
    .def( "colorIndexAt",   (const Index&  (FaceSet::*)(uint_t) const)&FaceSet::getColorIndexListAt,    return_value_policy<copy_const_reference>() )
    .def( "texCoordIndexAt",(const Index&  (FaceSet::*)(uint_t) const)&FaceSet::getTexCoordIndexListAt, return_value_policy<copy_const_reference>() )
    ;
  implicitly_convertible<FaceSetPtr, MeshPtr>();
}

void export_AmapSymbol()
{
  class_<AmapSymbol, AmapSymbolPtr, bases<FaceSet> >
    ( "AmapSymbol", 
	  "The AmapSymbol describes an object of class of Mesh stored in the SMB file format of the Amap software."
	  "This is provided for ascendant compatibility.", 
	  init< optional<std::string,bool> >("AmapSymbol(filename)"))
    .def("readFile",&AmapSymbol::readFile)
	.DEC_BT_PROPERTY(filename,AmapSymbol,FileName,std::string)
    ;
  implicitly_convertible<AmapSymbolPtr, FaceSetPtr>();

}
