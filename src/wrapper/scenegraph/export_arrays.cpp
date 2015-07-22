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

#include <plantgl/tool/util_array.h>
#include <plantgl/tool/util_tuple.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/container/colorarray.h>
#include <plantgl/scenegraph/container/indexarray_iterator.h>

#include <plantgl/python/exception.h>
#include <plantgl/python/export_refcountptr.h>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
#include "arrays_macro.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace boost::python;

typedef Uint32Array1 UIntArray;
typedef Uint32Array1Ptr UIntArrayPtr;

EXPORT_FUNCTION( c3a,  Color3Array )
EXPORT_FUNCTION( c4a,  Color4Array )
EXPORT_FUNCTION( i3a,  Index3Array )
EXPORT_FUNCTION( i4a,  Index4Array )
EXPORT_FUNCTION( inda, IndexArray )
EXPORT_FUNCTION( ra,   RealArray )
EXPORT_FUNCTION( uia,  UIntArray )

EXPORT_NUMPY( c3a, Color3, Color3Array, 0, 3, uchar_t )
EXPORT_NUMPY( c4a, Color4, Color4Array, 0, 4, uchar_t )
EXPORT_NUMPY( i3a, Index3, Index3Array, 0, 3, uint_t )
EXPORT_NUMPY( i4a, Index4, Index4Array, 0, 4, uint_t )
EXPORT_NUMPY( inda, Index, IndexArray, 0, 0, uint_t )
EXPORT_NUMPY_1DIM( ra, real_t, RealArray, 0, real_t )
EXPORT_NUMPY_1DIM( uia, uint32_t, UIntArray, 0, uint32_t)


template<class NumericArray>
object py_histogram(RCPtr<NumericArray> array, uint32_t nbbins)
{
    typedef typename NumericArray::element_type ValueType;
    ValueType minvalue, maxvalue;
    real_t binrange;
    Uint32Array1Ptr result = histogram<NumericArray>(array,nbbins,minvalue,maxvalue,binrange);
    return make_tuple(result,minvalue,maxvalue,binrange);
}

template<class Iterator>
int py_next_iter(Iterator * iter) {
    if (iter->atEnd()) {
         PyErr_SetString(PyExc_StopIteration, "index out of range");
         boost::python::throw_error_already_set();
    }
    int value = *(*iter);
    ++(*iter);
    return value;
}

template<class Iterator>
inline void nullfunc(Iterator * ) { }

typedef RealArray (RealArray::* Array2ArrayFunc) (const RealArray&) const;



#define LOADTYPE(type) if (classname == #type) result.append(boost::python::object(_bp.readArray<type>()));

object pgl_load_data(const std::string& fname)
{
    std::string cwd = get_cwd();
	chg_dir(get_dirname(fname));
    BinaryParser _bp(*PglErrorStream::error);
    boost::python::list result;
    if ( _bp.open(fname) && _bp.readHeader()){
        uint32_t nbelem = _bp.readUint32();
        // printf("Should read %i element.\n",nbelem);
        for(uint32_t elem = 0 ; elem < nbelem && !_bp.eof() ; ++elem){
            std::string classname = _bp.readString();
            // printf("Should read '%s'.\n",classname.c_str());
            LOADTYPE(RealArray)
            else LOADTYPE(Point2Array)
            else LOADTYPE(Point3Array)
            else LOADTYPE(Point4Array)
            else LOADTYPE(Index3Array)
            else LOADTYPE(Index4Array)
            else LOADTYPE(Index3Array)
            else LOADTYPE(IndexArray)
            else LOADTYPE(Uint32Array1)
        }
        _bp.close();
    }
	chg_dir(cwd);
    if (len(result) == 1) return result[0];
    return boost::python::tuple(result);

}


#define SAVETYPE(type) if (extract<type *>(a).check()) bp.dumpArray<type>(*extract<type *>(a)());

void pgl_save_one_data(const boost::python::object& a, BinaryPrinter& bp)
{
            SAVETYPE(RealArray)
            else SAVETYPE(Point2Array)
            else SAVETYPE(Point3Array)
            else SAVETYPE(Point4Array)
            else SAVETYPE(Index3Array)
            else SAVETYPE(Index4Array)
            else SAVETYPE(Index3Array)
            else SAVETYPE(IndexArray)
            else SAVETYPE(Uint32Array1)
            else {

                 PyErr_SetString(PyExc_ValueError, extract<char *>("Cannot save element "+str(a))());
                 boost::python::throw_error_already_set();
            }
}

bool pgl_save_data(const boost::python::object& a, const std::string& fname)
{
    leofstream stream(fname.c_str());
    if(!stream)return false;
    else {
	    std::string cwd = get_cwd();
		chg_dir(get_dirname(fname));
        BinaryPrinter _bp(stream);
        _bp.header();
        if (extract<RefCountObject *>(a).check()){
            _bp.writeUint32(1);
            pgl_save_one_data(a,_bp);
        }
        else {
            _bp.writeUint32(len(a));
            
		    boost::python::object iter_obj = boost::python::object( boost::python::handle<>( PyObject_GetIter( a.ptr() ) ) );
		    while( true )
		    {
			    boost::python::object obj; 
			    try  {  obj = iter_obj.attr( "next" )(); }
			    catch( boost::python::error_already_set ){ PyErr_Clear(); break; }
                pgl_save_one_data(obj,_bp);
		    }
        }
		chg_dir(cwd);
        return true;
    }
}

bool ra_is_valid(RealArray * a) {
    for(RealArray::const_iterator it = a->begin(); it != a->end(); ++it)
        if (!finite(*it)) return false;
    return true;
}
/*template <Array2ArrayFunc func>
RealArray * wrap_array_func(RealArray * array, const RealArray& v) {
    return new RealArray(array->*func(v)); }*/



struct IdValueSorter {
    const RealArray *  values;
    IdValueSorter(const RealArray *  _values) : values(_values) { }

    inline bool operator()(const uint32_t& a, const uint32_t& b){
        return values->getAt(a) < values->getAt(b);
    }
};


IndexArrayPtr py_cut(RealArray * a, RealArrayPtr bins, bool filteremptygroups = true)
{
    Index ids = TOOLS(range)<Index>(a->size(),0,1);
    std::stable_sort(ids.begin(),ids.end(),IdValueSorter(a));
    IndexArrayPtr result(new IndexArray());
    RealArray::const_iterator itbins = bins->begin();
    Index current;

    for (Index::const_iterator ita = ids.begin(); ita != ids.end(); ++ita)
    {
        while(itbins !=  bins->end() && a->getAt(*ita) > *itbins) {
               if (!filteremptygroups || !current.empty()) result->push_back(current);
               current = Index();
               ++itbins;
        }
        current.push_back(*ita);

    }
    if (!filteremptygroups || !current.empty()) result->push_back(current);
    return result;
}

void export_arrays()
{
  EXPORT_ARRAY_CT( c3a, Color3Array, "Color3Array([Index3(i,j,k),...])" )
    DEFINE_NUMPY( c3a );
  EXPORT_CONVERTER(Color3Array);
  EXPORT_ARRAY_CT( c4a, Color4Array, "Color4Array([Index4(i,j,k,l),...])" )
    DEFINE_NUMPY( c4a );
  EXPORT_CONVERTER(Color4Array);

  EXPORT_ARRAY_CT( i3a, Index3Array, "Index3Array([Index3(i,j,k),...])" )
    DEFINE_NUMPY( i3a );
  EXPORT_CONVERTER(Index3Array);
  EXPORT_ARRAY_CT( i4a, Index4Array, "Index4Array([Index4(i,j,k,l),...])" )
    .def( "triangulate", &Index4Array::triangulate)
    DEFINE_NUMPY( i4a );
  EXPORT_CONVERTER(Index4Array);
  EXPORT_ARRAY_CT( inda,IndexArray,  "IndexArray([Index([i,j,..]),...])" )
    .def( "triangulate", &IndexArray::triangulate)
    DEFINE_NUMPY( inda );
  EXPORT_CONVERTER(IndexArray);

  EXPORT_ARRAY_BT( ra, RealArray,  "RealArray([a,b,...])" )
    .def("log",(RealArrayPtr(RealArray::*)()const)&RealArray::log)
    .def("log",(RealArrayPtr(RealArray::*)(real_t)const)&RealArray::log,args("base"))
    .def("exp",&RealArray::exp)
    .def("sqrt",&RealArray::sqrt)
    .def("pow",&RealArray::pow,args("exponent"))
    // .def( "__add__",      &wrap_array_func<TOOLS::RealArray::operator + >   , boost::python::return_value_policy<boost::python::manage_new_object>() ) 
    // .def( "__add__",      (RealArray(RealArray::*)(real_t)const)           &RealArray::operator+   , boost::python::return_value_policy<boost::python::manage_new_object>() ) 
    // .def( "add",          &wrap_array_func<(RealArray(RealArray::*)(const RealArray&)const) &RealArray::operator+>   , boost::python::return_value_policy<boost::python::manage_new_object>() ) 
    // .def( "add",          (RealArray(RealArray::*)(real_t)const)           &RealArray::operator+   , boost::python::return_value_policy<boost::python::manage_new_object>() ) 
    .def( "__iadd__",     (RealArray&(RealArray::*)(real_t))               &RealArray::operator+=  , return_self<>() ) 
    .def( "__iadd__",     (RealArray&(RealArray::*)(const RealArray&))     &RealArray::operator+=  , return_self<>() ) 
    .def("cut", &py_cut, (arg("bins"),arg("filteremptygroups")=true))
    .def("isValid",&ra_is_valid)
    EXPORT_ARRAY_IO_FUNC( RealArray )

    DEFINE_NUMPY( ra );
  EXPORT_CONVERTER(RealArray);

  EXPORT_ARRAY_BT( uia, UIntArray,  "UIntArray([a,b,...])" )
  // EXPORT_ARRAY_IO_FUNC( UIntArray )
  DEFINE_NUMPY( uia );
  EXPORT_CONVERTER(UIntArray);

 def("histogram",&py_histogram<RealArray>);

  class_<IndexArrayPreOrderConstIterator>("IndexArrayPreOrderConstIterator",init<IndexArrayPtr,uint32_t>())
    .def("next",&py_next_iter<IndexArrayPreOrderConstIterator>)
    .def("atEnd",&IndexArrayPreOrderConstIterator::atEnd)
    .def("__iter__",&nullfunc<IndexArrayPreOrderConstIterator>, return_self<>())
    ;

   class_<IndexArrayPostOrderConstIterator>("IndexArrayPostOrderConstIterator",init<IndexArrayPtr,uint32_t>())
    .def("next",&py_next_iter<IndexArrayPostOrderConstIterator>)
    .def("atEnd",&IndexArrayPostOrderConstIterator::atEnd)
    .def("__iter__",&nullfunc<IndexArrayPostOrderConstIterator>, return_self<>())
    ;

   def("pgl_load_data",&pgl_load_data);
   def("pgl_save_data",&pgl_save_data);
}


