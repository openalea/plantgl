/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al. 
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use, 
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info". 
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability. 
 *       
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or 
 *   data to be ensured and,  more generally, to use and operate it in the 
 *   same conditions as regards security. 
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
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


#define SAVETYPE(type) if (extract<type *>(a).check()) bprinter.dumpArray<type>(*extract<type *>(a)());

void pgl_save_one_data(const boost::python::object& a, BinaryPrinter& bprinter)
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
    std::ofstream stream(fname.c_str(), std::ios::out | std::ios::binary);
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

            boost::python::object iter_obj = boost::python::object( boost::python::handle<PyObject>( PyObject_GetIter( a.ptr() ) ) );
            while( true )
            {
                boost::python::object obj;
                try  {  obj = boost::python::object(boost::python::handle<PyObject>(PyIter_Next(iter_obj.ptr()))); }
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
        if (!pglfinite(*it)) return false;
    return true;
}
/*template <Array2ArrayFunc func>
RealArray * wrap_array_func(RealArray * array, const RealArray& v) {
    return new RealArray(array->*func(v)); }*/

boost::python::object ra_getMinAndMax(RealArray * a, bool filterinf = false) {
    std::pair<RealArray::const_iterator,RealArray::const_iterator> res = a->getMinAndMax(filterinf);
    return make_tuple(*res.first,*res.second);
}

boost::python::object ra_getMinAndMaxIndex(RealArray * a, bool filterinf = false) {
    std::pair<RealArray::const_iterator,RealArray::const_iterator> res = a->getMinAndMax(filterinf);
    return make_tuple(std::distance<RealArray::const_iterator>(a->begin(),res.first),std::distance<RealArray::const_iterator>(a->begin(),res.second));
}

real_t ra_getMin(RealArray * a) {
    return *a->getMin();
}

size_t ra_getMinIndex(RealArray * a) {
    return std::distance<RealArray::const_iterator>(a->begin(),a->getMin());
}


real_t ra_getMax(RealArray * a) {
    return *a->getMax();
}

size_t ra_getMaxIndex(RealArray * a) {
    return std::distance<RealArray::const_iterator>(a->begin(),a->getMax());
}


struct IdValueSorter {
    const RealArray *  values;
    IdValueSorter(const RealArray *  _values) : values(_values) { }

    inline bool operator()(const uint32_t& a, const uint32_t& b){
        return values->getAt(a) < values->getAt(b);
    }
};


IndexArrayPtr py_cut(RealArray * a, RealArrayPtr bins, bool filteremptygroups = true)
{
    Index ids = PGL::range<Index>(a->size(),0,1);
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
    .def( "__isub__",     (RealArray&(RealArray::*)(real_t))               &RealArray::operator-=  , return_self<>() )
    .def( "__isub__",     (RealArray&(RealArray::*)(const RealArray&))     &RealArray::operator-=  , return_self<>() )
    .def( "__imul__",     (RealArray&(RealArray::*)(real_t))               &RealArray::operator*=  , return_self<>() )
    .def( "__idiv__",     (RealArray&(RealArray::*)(real_t))               &RealArray::operator/=  , return_self<>() )
    .def( "getMinAndMax", &ra_getMinAndMax , (boost::python::arg("filterinf")=false))
    .def( "getMinAndMaxIndex", &ra_getMinAndMaxIndex , (boost::python::arg("filterinf")=false))
    .def( "getMin", &ra_getMin )
    .def( "getMinIndex", &ra_getMinIndex )
    .def( "getMax", &ra_getMax )
    .def( "getMaxIndex", &ra_getMaxIndex )
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
    .def("__next__",&py_next_iter<IndexArrayPreOrderConstIterator>)
    .def("atEnd",&IndexArrayPreOrderConstIterator::atEnd)
    .def("__iter__",&nullfunc<IndexArrayPreOrderConstIterator>, return_self<>())
    ;

   class_<IndexArrayPostOrderConstIterator>("IndexArrayPostOrderConstIterator",init<IndexArrayPtr,uint32_t>())
    .def("next",&py_next_iter<IndexArrayPostOrderConstIterator>)
    .def("__next__",&py_next_iter<IndexArrayPostOrderConstIterator>)
    .def("atEnd",&IndexArrayPostOrderConstIterator::atEnd)
    .def("__iter__",&nullfunc<IndexArrayPostOrderConstIterator>, return_self<>())
    ;

   def("pgl_load_data",&pgl_load_data);
   def("pgl_save_data",&pgl_save_data);
}


