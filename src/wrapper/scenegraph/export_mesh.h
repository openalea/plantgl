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

#include <plantgl/scenegraph/geometry/polyline.h>
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
#define bp boost::python

DEF_POINTEE( TriangleSet )

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

template<class MeshType, class IndexArrayPtrType, const IndexArrayPtrType& (IndexedMesh<typename MeshType::IndexArray>::* func)() const>
IndexArrayPtrType get_prop_from_indexedmesh(const MeshType * obj)
{ return (obj->*func)(); }

template<class MeshType, class IndexArrayPtrType, IndexArrayPtrType& (IndexedMesh<typename MeshType::IndexArray>::* func)()>
void set_prop_from_indexedmesh(MeshType * obj, IndexArrayPtrType value){ (obj->*func)() = value; }

#define DEC_MESH_PROPERTY(PROPNAME,MeshClass,PROP,TYPE) \
    add_property(#PROPNAME,&get_prop_from_indexedmesh<MeshClass,MeshClass::IndexArrayPtr,&IndexedMesh<MeshClass::IndexArray>::get##PROP>, \
	&set_prop_from_indexedmesh<MeshClass,MeshClass::IndexArrayPtr,&IndexedMesh<MeshClass::IndexArray>::get##PROP> )

#define DEC_MESH_PROPERTY_WD(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_MESH_PROPERTY(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_DEFAULTVALTEST(_CLASS,PROP)


template<class MeshType>
class mesh_func : public boost::python::def_visitor<mesh_func<MeshType> >
{
    friend class boost::python::def_visitor_access;

    template <class classT>
    void visit(classT& c) const
    {
	  c.def(init<Point3ArrayPtr, MeshType::IndexArrayPtr, Point3ArrayPtr,
	           optional<MeshType::IndexArrayPtr,Color4ArrayPtr,MeshType::IndexArrayPtr,
			            Point2ArrayPtr,MeshType::IndexArrayPtr,
						bool, bool, bool, bool, PolylinePtr> >
			("__init__(Point3Array pointList, IndexArray indexList [, Point3Array normalList, IndexArray nomalIndexList, "
			 "Color4Array colorList, IndexArray colorIndexList, Point2Array texCoordList, IndexArray texCoordIndexList, "
			 "bool normalPerVertex, bool colorPerVertex, bool ccw, bool solid, Polyline skeleton])",
	         (bp::arg("pointList")         = Point3ArrayPtr(),
			  bp::arg("indexList")         = MeshType::IndexArrayPtr(),
			  bp::arg("normalList")        = Point3ArrayPtr(),
			  bp::arg("nomalIndexList")    = MeshType::IndexArrayPtr(),
			  bp::arg("colorList")         = Color4ArrayPtr(),
			  bp::arg("colorIndexList")    = MeshType::IndexArrayPtr(),
			  bp::arg("texCoordList")      = Point2ArrayPtr(),
			  bp::arg("texCoordIndexList") = MeshType::IndexArrayPtr(),
			  bp::arg("normalPerVertex")= Mesh::DEFAULT_NORMALPERVERTEX,
			  bp::arg("colorPerVertex") = Mesh::DEFAULT_COLORPERVERTEX,
			  bp::arg("ccw")            = Mesh::DEFAULT_CCW,
			  bp::arg("solid")          = Mesh::DEFAULT_SOLID,
			  bp::arg("skeleton")       = Mesh::DEFAULT_SKELETON)))
	.def( "deepcopy", &MeshType::copy )

	.DEC_MESH_PROPERTY(indexList,            MeshType, IndexList,        IndexArrayPtrType)
	.DEC_MESH_PROPERTY_WD(normalIndexList,   MeshType, NormalIndexList,  IndexArrayPtrType)
	.DEC_MESH_PROPERTY_WD(colorIndexList,    MeshType, ColorIndexList,   IndexArrayPtrType)
	.DEC_MESH_PROPERTY_WD(texCoordIndexList, MeshType, TexCoordIndexList,IndexArrayPtrType)
 
    .def( "faceNormalAt",   (const Vector3& (MeshType::*)(uint_t,uint_t) const)&MeshType::getFaceNormalAt ,  return_value_policy<copy_const_reference>())
    .def( "faceColorAt",    (const Color4&  (MeshType::*)(uint_t,uint_t) const)&MeshType::getFaceColorAt,    return_value_policy<copy_const_reference>() )
    .def( "faceTexCoordAt", (const Vector2& (MeshType::*)(uint_t,uint_t) const)&MeshType::getFaceTexCoordAt, return_value_policy<copy_const_reference>() )

    .def( "normalAt",   (const Vector3& (MeshType::*)(uint_t) const)&MeshType::getNormalAt,   return_value_policy<copy_const_reference>() )
    .def( "colorAt",    (const Color4&  (MeshType::*)(uint_t) const)&MeshType::getColorAt,    return_value_policy<copy_const_reference>() )
    .def( "texCoordAt", (const Vector2& (MeshType::*)(uint_t) const)&MeshType::getTexCoordAt, return_value_policy<copy_const_reference>() )

	.def( "indexAt",        (const MeshType::IndexArray::element_type&  (MeshType::*)(uint_t) const)&MeshType::getIndexListAt,         return_value_policy<copy_const_reference>() )
    .def( "normalIndexAt",  (const MeshType::IndexArray::element_type&  (MeshType::*)(uint_t) const)&MeshType::getNormalIndexListAt,   return_value_policy<copy_const_reference>() )
    .def( "colorIndexAt",   (const MeshType::IndexArray::element_type&  (MeshType::*)(uint_t) const)&MeshType::getColorIndexListAt,    return_value_policy<copy_const_reference>() )
    .def( "texCoordIndexAt",(const MeshType::IndexArray::element_type&  (MeshType::*)(uint_t) const)&MeshType::getTexCoordIndexListAt, return_value_policy<copy_const_reference>() )
        ;
    }
};

