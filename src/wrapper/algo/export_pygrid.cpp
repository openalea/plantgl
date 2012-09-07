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
 *       This program is distributed in the hope that it will be
 *       useful, but WITHOUT ANY WARRANTY; without even the implied
 *       warranty of MERCHANTABILITY or FITNESS For A PARTICULAR
 *       PURPOSE. See the GNU General Public License for more details.
 *       * You should have received a copy of the GNU General Public
 *       License along with this program; see the file COPYING. If
 *       not, write to the Free Software Foundation, Inc., 59 Temple
 *       Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  ----------------------------------------------------------------------------
 */

#include "export_grid.h"
#include <plantgl/tool/util_spatialarray.h>

/* ----------------------------------------------------------------------- */
/*
template<class T>
class NumpyContainer {
protected:

	typedef T element_type;
	typedef bp::numeric::array container_type;
	typedef size_t CellId;  
protected:

    NumpyContainer(size_t size = 0) : __values(element_type()) { initialize(size); }
    container_type __values;

public:

	inline element_type getAt(const CellId& cid) const
    { return bp::extract<element_type>(__values[cid])(); }

	inline element_type getAt(const CellId& cid)
    { return bp::extract<element_type>(__values[cid])(); }

    inline void setAt(const CellId& cid, const T& value) 
    { __values[cid] = value; }

    /// Returns whether \e self is empty.
    static inline bool is_empty(const CellId& cid) { return false; }

	inline size_t valuesize() const { return len(__values); }

    /// Returns whether \e self is empty.
    inline bool empty( ) const { return len(__values) == 0; }

    /// Clear \e self.
    inline void clear( ) { initialize(0); }

    void initialize(const size_t size) { 
        printf("initialize %i\n", size);
		__values = container_type(element_type());
        if (size > 0) __values.resize(size);
	}

    container_type get_array() const { return __values; }
};*/


class IsNone {
public:
    static inline bool is_empty(const boost::python::object& value) { return value == boost::python::object(); }
};

#define DEFAULT_PYTHON_OBJ_TYPE boost::python::object
typedef SpatialArrayN<DEFAULT_PYTHON_OBJ_TYPE, Vector2, 2, VectorContainer<DEFAULT_PYTHON_OBJ_TYPE, IsNone> > Py2Grid;
typedef SpatialArrayN<DEFAULT_PYTHON_OBJ_TYPE, Vector3, 3, VectorContainer<DEFAULT_PYTHON_OBJ_TYPE, IsNone> > Py3Grid;
typedef SpatialArrayN<DEFAULT_PYTHON_OBJ_TYPE, Vector4, 4, VectorContainer<DEFAULT_PYTHON_OBJ_TYPE, IsNone> > Py4Grid;

template<class T>
class NoneContainer {
protected:

	typedef T element_type;
	typedef size_t CellId;  

protected:

    NoneContainer(size_t size = 0) { }

public:

/*	inline element_type getAt(const CellId& cid) const
    { return bp::extract<element_type>(__values[cid])(); }

	inline element_type getAt(const CellId& cid)
    { return bp::extract<element_type>(__values[cid])(); }

    inline void setAt(const CellId& cid, const T& value) { }
*/
	inline element_type getAt(const CellId& cid) const
    { return boost::python::object(); }

	inline element_type getAt(const CellId& cid)
    { return boost::python::object(); }

    inline void setAt(const CellId& cid, const T& value) { }


    /// Returns whether \e self is empty.
    static inline bool is_empty(const CellId& cid) { return false; }


	inline size_t valuesize() const { return 0; }

    /// Returns whether \e self is empty.
    inline bool empty( ) const { return true; }

    /// Clear \e self.
    inline void clear( ) {  }

    void initialize(const size_t size) { }

};

typedef SpatialArrayN<bp::object, Vector2, 2, NoneContainer<bp::object> > Py2GridIndexing;
typedef SpatialArrayN<bp::object, Vector3, 3, NoneContainer<bp::object> > Py3GridIndexing;
typedef SpatialArrayN<bp::object, Vector4, 4, NoneContainer<bp::object> > Py4GridIndexing;

void export_PyGrid()
{

  class_< Py2Grid > ("Grid2", init<Vector2, Vector2, Vector2>
     ( "Construct a regular 2D grid.", args("voxelsize","minpoint","maxpoint") ))
 	 .def(spatialarray_func<Py2Grid>())
     // .def("get_array",&Py2Grid::get_array)
    ;
  
  class_< Py3Grid > ("Grid3", init<Vector3, Vector3, Vector3>
     ( "Construct a regular 3D grid.", args("voxelsize","minpoint","maxpoint") ))
 	 .def(spatialarray_func<Py3Grid>())
     // .def("get_array",&Py3Grid::get_array)
    ;
  
  class_< Py4Grid > ("Grid4", init<Vector4, Vector4, Vector4>
     ( "Construct a regular 4D grid.", args("voxelsize","minpoint","maxpoint") ))
 	 .def(spatialarray_func<Py4Grid>())
     // .def("get_array",&Py4Grid::get_array)
    ;
  
  class_< Py3GridIndexing > ("Grid3Indexing", init<Vector3, Vector3, Vector3>
     ( "Construct a regular 3D grid indexing.", args("voxelsize","minpoint","maxpoint") ))
 	 .def(spatialarray_func<Py3GridIndexing>())
    ;

  def("is_point_in_cone",&is_point_in_cone<Vector2>);
  def("is_point_in_cone",&is_point_in_cone<Vector3>);
  def("is_point_in_cone",&is_point_in_cone<Vector4>);
}

/* ----------------------------------------------------------------------- */


