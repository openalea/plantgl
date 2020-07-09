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



// Array Macro
#include <plantgl/python/extract_pgl.h>
#include <plantgl/python/extract_list.h>
#include <plantgl/python/pyseq_iterator.h>
#include <plantgl/tool/util_array.h>
#include <boost/python/def_visitor.hpp>
#include <plantgl/scenegraph/container/indexarray.h>

PGL_USING(Index)


template<class T>
RCPtr<T> extract_array_from_list( boost::python::object l )
{
  boost::python::extract<int> e_int( l );
  if( e_int.check() )
    { return RCPtr<T>(new T( e_int() ));  }

  return extract_pgllist<T>(l).toRCPtr(true);
}

template<class T>
struct array_from_list {
  array_from_list() {
    boost::python::converter::registry::push_back( &convertible, &construct, boost::python::type_id<T>());
  }
  static void* convertible(PyObject* py_obj){
    if( py_obj !=  Py_None && !PySequence_Check( py_obj )) return 0;
    return py_obj;
  }
  static void construct( PyObject* obj, boost::python::converter::rvalue_from_python_stage1_data* data){
   typedef boost::python::converter::rvalue_from_python_storage<T> vector_storage_t;
   vector_storage_t* the_storage = reinterpret_cast<vector_storage_t*>( data );
   void* memory_chunk = the_storage->storage.bytes;
   if (obj != Py_None){
        boost::python::list py_sequence( boost::python::handle<PyObject>( boost::python::borrowed( obj ) ) );
        RCPtr<T> result= extract_array_from_list<T>(py_sequence);
        new (memory_chunk) T (*result);
   }
   else { new (memory_chunk) T(0); }
   data->convertible = memory_chunk;
  }
};

template<class T>
struct array_ptr_from_list {
  array_ptr_from_list() {
    boost::python::converter::registry::push_back( &convertible, &construct, boost::python::type_id< RCPtr<T> >());
  }
  static void* convertible(PyObject* py_obj){
    if( py_obj !=  Py_None && !PySequence_Check( py_obj ) ) return 0;
    return py_obj;
  }
  static void construct( PyObject* obj, boost::python::converter::rvalue_from_python_stage1_data* data){
   typedef boost::python::converter::rvalue_from_python_storage<T> vector_storage_t;
   vector_storage_t* the_storage = reinterpret_cast<vector_storage_t*>( data );
   void* memory_chunk = the_storage->storage.bytes;
   RCPtr<T> result;
   if (obj != Py_None){
    boost::python::list py_sequence( boost::python::handle<PyObject>( boost::python::borrowed( obj ) ) );
    result = extract_array_from_list<T>(py_sequence);
   }
   new (memory_chunk) RCPtr<T> (result);
   data->convertible = memory_chunk;
  }
};


#define EXPORT_FUNCTION2(PREFIX,ARRAY) \
std::string PREFIX##_str( ARRAY * a ) \
{ \
  assert (a); \
  std::stringstream ss; \
  ss << #ARRAY << "(";  \
  if(!a->empty()){ \
    ss << '['; \
    for(ARRAY::const_iterator it = a->begin(); it != a->end(); ++it){ \
        if (it != a->begin()) ss << ","; \
        ss << extract<std::string>(boost::python::str(boost::python::object(*it)))(); \
    } \
    ss << ']'; } \
  ss << ")"; \
  return ss.str(); \
} \

template<class T>
typename T::element_type array_bt_getitem( T * a, int pos )
{
  size_t len = a->size();
  if( pos < 0 && pos >= -(int)len ) return a->getAt( len + pos );
  else if( pos < len ) return a->getAt( pos );
  else throw PythonExc_IndexError();
}

template<class T>
typename T::element_type& array_ct_getitem( T * a, int pos )
{
  size_t len = a->size();
  if( pos < 0 && pos >= -(int)len ) return a->getAt( len + pos );
  else if( pos < len ) return a->getAt( pos );
  else throw PythonExc_IndexError();
}

template<class T>
typename T::element_type array_ptr_getitem( T * a, int pos )
{
  size_t len = a->size();
  if( pos < 0 && pos >= -(int)len ) return a->getAt( len + pos );
  else if( pos < len ) return a->getAt( pos );
  else throw PythonExc_IndexError();
}

inline void extract_slice(boost::python::slice sl, int& beg, int& end, int& step, size_t msize) {
  
  beg = 0;
  if (sl.start() != boost::python::object()){
    beg = boost::python::extract<int>(sl.start())();
  }
  if( beg >= -(int)msize && beg < 0  )  beg += msize;
  else if( beg >= msize ) throw PythonExc_IndexError();

  end = msize;
  if (sl.stop() != boost::python::object()){
    end = boost::python::extract<int>(sl.stop())();
  }
  if( end >= -(int)msize && end < 0  )  end += msize;
  else if( end > msize ) throw PythonExc_IndexError();

  step = 1;
  if (sl.step() != boost::python::object()){
    step = boost::python::extract<int>(sl.step())();
  }
}

template<class T>
T * array_getitem_slice( T * array, boost::python::slice sl )
{
  size_t llen = array->size();
  int beg, end, step;
  extract_slice(sl, beg, end, step, llen);

  if (step == 1) {
    return new T(array->begin()+beg,array->begin()+end);
  }
  else {
    typename T::iterator it = array->begin()+beg;
    T * res = new T(it, it+1);
    it += step;
    for(int i = beg+step; i < end; i+= step){
        res->push_back(*it);
        it += step;
    }
    return res;    
  }
}



template<class T>
typename T::element_type array_popitem( T * a, int pos )
{
  size_t len = a->size();
  if (a->empty()) throw PythonExc_IndexError();
  if( pos < 0 && pos >= -(int)len ) pos = len + pos;
  else if( pos >= len ) throw PythonExc_IndexError();
  typename T::element_type elem =  a->getAt( pos );
  a->erase(a->begin() + pos);
  return elem;
}

template<class T>
typename T::element_type array_poplastitem( T * a)
{ return array_popitem(a,-1); }


template<class T>
void array_setitem( T * array, int pos, typename T::element_type v )
{
  size_t len = array->size();
  if( pos < 0 && pos >= -(int)len ) array->setAt( len + pos, v );
  else if( pos < len ) array->setAt( pos, v );
  else throw PythonExc_IndexError();
}

template<class T>
void array_setsliceitem( T * array, boost::python::slice pos, typename T::element_type v )
{

  size_t llen = array->size();
  int beg, end, step;
  extract_slice(pos, beg, end, step, llen);

  for(int i = beg; i < end; i+= step){
        array->setAt(i, v) ;
    }

}

template<class T>
void array_setlistitem( T * array, boost::python::list pos, typename T::element_type v )
{
  size_t llen = array->size();
  {
      PySeqIterator positer(pos);
      while (positer.is_valid()) {
        int lpos = boost::python::extract<int>(positer.next())();
        if( lpos < -(int)llen && lpos >= llen  )  throw PythonExc_IndexError();
      }
  }

  PySeqIterator positer(pos);
  while (positer.is_valid()) {
    int lpos = boost::python::extract<int>(positer.next())();
    if( lpos < 0  )  lpos += llen;
    array->setAt(lpos, v) ;
  }
}

template<class T>
void array_setsliceitem_list( T * array, boost::python::slice pos, boost::python::list v )
{

  size_t llen = array->size();
  int beg, end, step;
  extract_slice(pos, beg, end, step, llen);

  int nbelem = (end - beg) / step;

  if( nbelem != len(v)  )  throw PythonExc_IndexError();

  PySeqIterator valueiter(v);
  for(int i = beg; i < end; i+= step){
        array->setAt(i, boost::python::extract<typename T::element_type>(valueiter.next())()) ;
    }

}

template<class T>
void array_setlistitem_list( T * array, boost::python::list pos, boost::python::list v )
{
  if( len(pos) != len(v)  )  throw PythonExc_IndexError();

  size_t llen = array->size();
  {
      PySeqIterator positer(pos);
      while (positer.is_valid()) {
        int lpos = boost::python::extract<int>(positer.next())();
        if( lpos < -(int)llen && lpos >= llen  )  throw PythonExc_IndexError();
      }
  }


  PySeqIterator valueiter(v);
  PySeqIterator positer(pos);
  while (positer.is_valid()) {
    int lpos = boost::python::extract<int>(positer.next())();
    if( lpos < 0  )  lpos += llen;
    array->setAt(lpos, boost::python::extract<typename T::element_type>(valueiter.next())()) ;
  }
}

template<class T>
void array_insertitem( T * array, int pos, typename T::element_type v )
{
  size_t len = array->size();
  if( pos < 0 && pos >= -(int)len ) array->insert( array->begin() + (len + pos), v );
  else if( pos < len ) array->insert( array->begin() + pos, v );
  else if( pos == len ) array->push_back( v );
  else throw PythonExc_IndexError();
}

template<class T>
void array_delitem( T * array, int pos )
{
  if( pos < 0 && pos >= -(int)array->size() ) array->erase( array->begin() + (array->size() + pos) );
  else if( pos < array->size() ) array->erase( array->begin() + pos );
  else throw PythonExc_IndexError();
}

template<class T>
void array_delitem_slice( T * array, boost::python::slice pos )
{
  size_t llen = array->size();
  int beg, end, step;
  extract_slice(pos, beg, end, step, llen);

  if (step == 1) {
    array->erase( array->begin()+beg,array->begin()+end);
  }
  else {
    for(int i = end; i >= beg; i-= step){
        if (i < llen) array->erase(array->begin() + i);
    }
  }

}


template<class T>
bool array_contains( T * array, typename T::element_type v )
{
  return array->contains(v);
}


template<class T>
T * array_addarray( T * array, T * array2 )
{
    T * array3 = new T(array->begin(),array->end());
    array3->insert(array3->end(),array2->begin(),array2->end());
    return array3;
}

template<class T>
T * array_iaddarray( T * array, T * array2 )
{
    array->insert(array->end(),array2->begin(),array2->end());
    return array;
}

template<class T>
void array_appenditem( T * array, typename T::element_type v )
{ array->push_back(v); }

template<class T>
void array_appendarray( T * array, T * array2 )
{   array->insert(array->end(),array2->begin(),array2->end()); }


template<class T>
void array_prependitem( T * array, typename T::element_type v )
{ array->insert(array->begin(),v); }

template<class T>
void array_prependarray( T * array, T * array2 )
{   array->insert(array->begin(),array2->begin(),array2->end()); }


template<class T>
size_t array_len( T * a )
{  return a->size();}

template<class T>
size_t array_id( T * a )
{ return (size_t)a; }

template <class T>
boost::python::object py_split(T * pts, boost::python::object split_method){
    boost::python::dict result;
    for(typename T::const_iterator it = pts->begin()+1; it != pts->end(); ++it) {
        boost::python::object svalue = split_method(boost::python::object(*it));
        if (result.has_key(svalue)) {
            boost::python::extract<T *>(result[svalue])()->push_back(*it);
        }
        else {
            T * newset = new T(); newset->push_back(*it);
            result[svalue] = boost::python::object(newset);
        }
    }
    return result;
}

template <class T>
boost::python::object py_split_indices(T * pts, boost::python::object split_method){
    boost::python::dict result;
    uint32_t pid = 0;
    for(typename T::const_iterator it = pts->begin()+1; it != pts->end(); ++it, ++pid) {
        boost::python::object svalue = split_method(boost::python::object(*it));
        if (result.has_key(svalue)) {
            boost::python::extract<boost::python::list>(result[svalue])().append(pid);
        }
        else {
            boost::python::list newset; newset.append(pid);
            result[svalue] = newset;
        }
    }
    return result;
}

template <class T>
T * py_subset(T * pts, boost::python::object subsetindices){
    T * subobj = new T();
    size_t nbelem = pts->size();
    boost::python::extract<Index> indexex(subsetindices);
    if (indexex.check()){
        Index index = indexex();
        for(Index::const_iterator it = index.begin(); it != index.end(); ++it) {
            subobj->push_back( pts->getAt(*it) );
            if (*it >= nbelem) {
                delete subobj;
                throw PythonExc_IndexError(boost::python::extract<char *>(boost::python::str(*it)+" out of range")());
            }
        }
    }
    else {
        boost::python::object iter_obj = boost::python::object( boost::python::handle<PyObject>( PyObject_GetIter( subsetindices.ptr() ) ) );
        while( true )
        {
            boost::python::object obj;
            try  {  obj = boost::python::object(boost::python::handle<PyObject>(PyIter_Next(iter_obj.ptr()))); }
            catch( boost::python::error_already_set ){ PyErr_Clear(); break; }
            int val = boost::python::extract<int>( obj )();
            if (val < 0) val += nbelem;
            if (val < 0 || val >= nbelem) {
                delete subobj;
                throw PythonExc_IndexError(boost::python::extract<char *>(boost::python::str(val)+" out of range")());
            }
            subobj->push_back( pts->getAt(val) );
        }
    }
    return subobj;
}

template <class T>
T * py_opposite_subset(T * pts, boost::python::object subsetindices){
    T * subobj = new T();
    size_t nbelem = pts->size();
    std::vector<bool> subsetinfo(nbelem);
    std::vector<int> csubsetindices = extract_vec<int>(subsetindices);

    for (std::vector<int>::iterator it = csubsetindices.begin(); it != csubsetindices.end(); ++it)
    {
        if (*it < 0) *it += nbelem;
        if (*it < 0 || *it >= nbelem) {
            delete subobj;
            throw PythonExc_IndexError(boost::python::extract<char *>(boost::python::str(*it)+" out of range")());
        }
        subsetinfo[*it] = true;
    }

    uint_t index = 0;
    for (std::vector<bool>::const_iterator it = subsetinfo.begin(); it != subsetinfo.end(); ++it, index++) {
      if (!*it)
        subobj->push_back(pts->getAt(index));
    }
    return subobj;
}

template <class T>
boost::python::object py_split_subset(T * pts, boost::python::object subsetindices){
    T * subobj = new T();
    T * subobjopp = new T();

    size_t nbelem = pts->size();
    std::vector<bool> insubset(nbelem, false);

    boost::python::extract<Index> indexex(subsetindices);
    if (indexex.check()){
        Index index = indexex();
        for(Index::const_iterator it = index.begin(); it != index.end(); ++it) {
            subobj->push_back( pts->getAt(*it) );
            insubset[*it] = true;
            if (*it >= nbelem) {
                delete subobj; delete subobjopp;
                throw PythonExc_IndexError(boost::python::extract<char *>(boost::python::str(*it)+" out of range")());
            }
        }
    }
    else {
        boost::python::object iter_obj = boost::python::object( boost::python::handle<PyObject>( PyObject_GetIter( subsetindices.ptr() ) ) );
        while( true )
        {
            boost::python::object obj;
            try  {  obj = boost::python::object(boost::python::handle<PyObject>(PyIter_Next(iter_obj.ptr()))); }
            catch( boost::python::error_already_set ){ PyErr_Clear(); break; }
            int val = boost::python::extract<int>( obj )();
            if (val < 0) val += nbelem;
            if (val < 0 || val >= nbelem) {
                delete subobj; delete subobjopp;
                throw PythonExc_IndexError(boost::python::extract<char *>(boost::python::str(val)+" out of range")());
            }
            subobj->push_back( pts->getAt(val) );
        }
    }

    size_t itv = 0;
    for (std::vector<bool>::const_iterator iti = insubset.begin(); iti != insubset.end(); ++iti, ++itv)
    {
        if (!(*iti)) { subobjopp->push_back(pts->getAt(itv)); }
    }

    return boost::python::make_tuple(subobj,subobjopp);
}

template <class T>
T * array_getitem_list(T * pts, boost::python::list subsetindices){
    return py_subset(pts,subsetindices);
}
 

#include <plantgl/tool/dirnames.h>
#include <plantgl/tool/bfstream.h>
#include <plantgl/tool/util_enviro.h>
#include <plantgl/algo/codec/binaryprinter.h>

template<class T>
bool save(T * a, std::string fname)
{
    std::ofstream stream(fname.c_str(), std::ios::out | std::ios::binary);
    if(!stream)return false;
    else {
        std::string cwd = get_cwd();
        chg_dir(get_dirname(fname));
        BinaryPrinter _bp(stream);
        _bp.header();
        _bp.writeUint32(1);
        _bp.dumpArray(*a);
        chg_dir(cwd);
        return true;
    }
}

#include <plantgl/algo/codec/scne_binaryparser.h>

template<class T>
RCPtr<T> load(std::string fname)
{
        std::string cwd = get_cwd();
        chg_dir(get_dirname(fname));
        BinaryParser _bp(*PglErrorStream::error);
        RCPtr<T> result;
        if ( _bp.open(fname) && _bp.readHeader()) {
            uint32_t nbelem = _bp.readUint32();
            if (nbelem > 1) result = _bp.loadArray<T>();
        }
        chg_dir(cwd);
        return result;
}

template<class T>
struct array_pickle_suite : boost::python::pickle_suite
{
    static boost::python::tuple getinitargs(T const& ar)
    {
        boost::python::list l;
        for(typename T::const_iterator it = ar.begin(); it != ar.end(); ++it)
            l.append(*it);
        return boost::python::make_tuple(l);
    }
};

template<class ARRAY>
class array_func : public boost::python::def_visitor<array_func<ARRAY> >
{
    friend class boost::python::def_visitor_access;

    template <class classT>
    void visit(classT& c) const
    {
        c.def( "__getitem__", &array_getitem_slice<ARRAY>, boost::python::return_value_policy<boost::python::manage_new_object>() ) \
        .def( "__getitem__",  &array_getitem_list<ARRAY>, boost::python::return_value_policy<boost::python::manage_new_object>() ) \
        .def( "__setitem__",  &array_setitem<ARRAY>   ) \
        .def( "__setitem__",  &array_setsliceitem<ARRAY>   ) \
        .def( "__setitem__",  &array_setlistitem<ARRAY>   ) \
        .def( "__setitem__",  &array_setsliceitem_list<ARRAY>   ) \
        .def( "__setitem__",  &array_setlistitem_list<ARRAY>   ) \
        .def( "__delitem__",  &array_delitem<ARRAY>   ) \
        .def( "__delitem__",  &array_delitem_slice<ARRAY>  ) \
        .def( "__contains__", &array_contains<ARRAY>  ) \
        .def( "__add__",      &array_addarray<ARRAY>   , boost::python::return_value_policy<boost::python::manage_new_object>() ) \
        .def( "__iadd__",     &array_iaddarray<ARRAY> , boost::python::return_internal_reference<1>() ) \
        .def( "__len__",      &array_len<ARRAY> ) \
        .def( "__iter__",     boost::python::iterator<ARRAY>() ) \
        .def( "empty",        &ARRAY::empty ) \
        .def( "reverse",      &ARRAY::reverse ) \
        .def( "clear",        &ARRAY::clear ) \
        .def( "insert",       &array_insertitem<ARRAY> ) \
        .def( "append",       &array_appenditem<ARRAY> ) \
        .def( "append",       &array_appendarray<ARRAY> ) \
        .def( "prepend",      &array_prependitem<ARRAY> ) \
        .def( "prepend",      &array_prependarray<ARRAY> ) \
        .def( "pop",          &array_popitem<ARRAY> ) \
        .def( "pop",          &array_poplastitem<ARRAY> ) \
        .def( "getId",        &array_id<ARRAY> ) \
        .def( "split",        &py_split<ARRAY>, "Split the list into 2. Each element is tested with the split method that should return True or False" ) \
        .def( "split_indices", &py_split_indices<ARRAY>, "Split the list into 2. Return list of indices of elements of the 2 subsets. Each element is tested with the split method that should return True or False" ) \
        .def( "subset",        &py_subset<ARRAY>, "Return a subset of the list. Should gives the indices of the subset as arguments.", boost::python::return_value_policy<boost::python::manage_new_object>() ) \
        .def( "opposite_subset",        &py_opposite_subset<ARRAY>, "Return a subset of the list. Should gives the indices that you do not want in the resulting subset as arguments.", boost::python::return_value_policy<boost::python::manage_new_object>() ) \
        .def( "split_subset", &py_split_subset<ARRAY>, "Return a subset of the list and its complementay subset. Arg is the indices of the first subset.")
        .def_pickle(array_pickle_suite<ARRAY>())
        ;
    }
};

#define EXPORT_ARRAY_IO_FUNC( ARRAY ) \
    .def( "save",         &save<ARRAY> ) \
    .def( "load",         &load<ARRAY> ) \
    .staticmethod("load")

#define EXPORT_ARRAY_FUNC_COMMON( ARRAY, PREFIX ) \
    .def(array_func<ARRAY>()) \
    .def( "__repr__",     &PREFIX##_str ) \
    .def( "__str__",      &PREFIX##_str ) \

#define EXPORT_ARRAY_FUNC_BT( ARRAY, PREFIX ) \
    .def( "__getitem__",  &array_bt_getitem<ARRAY> ) \
    EXPORT_ARRAY_FUNC_COMMON( ARRAY, PREFIX ) \

#define EXPORT_ARRAY_FUNC_CT( ARRAY, PREFIX ) \
    .def( "__getitem__",  &array_ct_getitem<ARRAY>, return_internal_reference<1>() ) \
    EXPORT_ARRAY_FUNC_COMMON( ARRAY, PREFIX ) \
    EXPORT_ARRAY_IO_FUNC( ARRAY )

#define EXPORT_ARRAY_FUNC_PTR( ARRAY, PREFIX ) \
    .def( "__getitem__",  &array_ptr_getitem<ARRAY> ) \
    EXPORT_ARRAY_FUNC_COMMON( ARRAY, PREFIX ) \




#define EXPORT_CLASS_ARRAY( PREFIX, ARRAY, STRING )\
class_< ARRAY, ARRAY##Ptr, bases<RefCountObject> >( #ARRAY , init<size_t>(#ARRAY "(int size)", args("size") ) ) \
    .def( "__init__", make_constructor( &extract_array_from_list<ARRAY> ), STRING ) \


/* --------------------
  Type of the element :
   _BT : Basic Type : real_t, int, uint_t, ...
   _CT : Complex Type : Vector3, Color3, Index, ...
   _PTR : Pointer : GeometryPtr
   -------------------- */

#define EXPORT_ARRAY_BT( PREFIX, ARRAY, STRING )\
    EXPORT_CLASS_ARRAY( PREFIX, ARRAY, STRING ) \
    EXPORT_ARRAY_FUNC_BT( ARRAY, PREFIX ) \

#define EXPORT_ARRAY_PTR( PREFIX, ARRAY, STRING )\
    EXPORT_CLASS_ARRAY( PREFIX, ARRAY, STRING ) \
    EXPORT_ARRAY_FUNC_PTR( ARRAY, PREFIX ) \

#define EXPORT_ARRAY_CT( PREFIX, ARRAY, STRING )\
    EXPORT_CLASS_ARRAY( PREFIX, ARRAY, STRING ) \
    EXPORT_ARRAY_FUNC_CT( ARRAY, PREFIX ) \

#define EXPORT_CONVERTER( ARRAY )\
    array_from_list<ARRAY>(); \
    array_ptr_from_list<ARRAY>(); \

#define EXPORT_FUNCTION( PREFIX, ARRAY) \
  DEF_POINTEE( ARRAY ) \
  EXPORT_FUNCTION2( PREFIX, ARRAY)

#ifdef USE_NUMPY
#define PY_ARRAY_UNIQUE_SYMBOL PlantGL_NUMPY_API_SYMBOL
#define NO_IMPORT_ARRAY
#include <numpy/arrayobject.h>
#include <sstream>

#define CHECK_TYPE( store, C_TYPE )\
  if( typeid( C_TYPE ) == typeid( int ) )\
    store = PyArray_INT; \
  else if( typeid( C_TYPE ) == typeid( unsigned int ) )\
    store = PyArray_UINT; \
  else if( typeid( C_TYPE ) == typeid( long ) )\
    store = PyArray_LONG; \
  else if( typeid( C_TYPE ) == typeid( unsigned long ) )\
    store = PyArray_ULONG; \
  else if( typeid( C_TYPE ) == typeid( char ) )\
    store = PyArray_BYTE; \
  else if( typeid( C_TYPE ) == typeid( unsigned char ) )\
    store = PyArray_UBYTE; \
  else if( typeid( C_TYPE ) == typeid( float ) )\
    store = PyArray_FLOAT; \
  else if( typeid( C_TYPE ) == typeid( double ) )\
    store = PyArray_DOUBLE; \
  else\
    {\
    PyErr_SetString( PyExc_ValueError, "Error, the wanted type is unknown" );\
    throw error_already_set();\
    }

#define EXPORT_NUMPY( PREFIX, T, ARRAY, DIM0, DIM1, C_TYPE ) \
ARRAY##Ptr PREFIX##_fromnumpy( boost::python::object l ) \
{\
  size_t dim0 = DIM0;\
  size_t dim1 = DIM1;\
  PyObject* obj = l.ptr(); \
  if( PyArray_Check( obj ) ) \
    { \
    PyArrayObject *a_obj = ( PyArrayObject* )obj; \
    if( a_obj->nd != 2 )\
      {\
      PyErr_SetString(PyExc_TypeError, "The array as argument must have 2 dimensions" );\
      throw error_already_set();\
      }\
    if( dim0 != 0 && a_obj->dimensions[ 0 ] != dim0 ) \
      {\
      std::stringstream ss;\
      ss << "The first dimension of the argument must be of size " << dim0;\
      PyErr_SetString(PyExc_TypeError, ss.str().c_str() );\
      throw error_already_set();\
      }\
    if( dim1 != 0 && a_obj->dimensions[ 1 ] != dim1 ) \
      {\
      std::stringstream ss;\
      ss << "The second dimension of the argument must be of size " << dim1;\
      PyErr_SetString(PyExc_TypeError, ss.str().c_str() );\
      throw error_already_set();\
      }\
    int a_type = 0; \
    int a_require = NPY_CONTIGUOUS|NPY_FORCECAST; \
    CHECK_TYPE( a_type, C_TYPE );\
    PyObject *array = PyArray_FromArray( a_obj, PyArray_DescrFromType( a_type ), a_require ); \
    if( array == NULL )\
      {\
      throw error_already_set();\
      }\
    int nb_elmt0 = PyArray_DIM( array, 0 ); \
    int nb_elmt1 = PyArray_DIM( array, 1 ); \
    int i, j; \
    ARRAY##Ptr result = new ARRAY( nb_elmt0 ); \
    for( i = 0 ; i < nb_elmt0 ; ++i ) \
      {\
      T elmt( nb_elmt1 );\
      for( j = 0 ; j < nb_elmt1 ; ++j )\
        {\
        elmt.setAt( j, *( C_TYPE* )PyArray_GETPTR2( array, i, j ) );\
        }\
      result->setAt( i, elmt );\
      }\
    Py_DECREF( array );\
    return result;\
    } \
  return PREFIX##_fromlist( l ); \
}\

#define EXPORT_NUMPY_1DIM( PREFIX, T, ARRAY, DIM, C_TYPE ) \
ARRAY##Ptr PREFIX##_fromnumpy( boost::python::object l ) \
{\
  size_t dim = DIM;\
  PyObject* obj = l.ptr(); \
  if( PyArray_Check( obj ) ) \
    { \
    PyArrayObject *a_obj = ( PyArrayObject* )obj; \
    if( a_obj->nd != 1 )\
      {\
      PyErr_SetString(PyExc_TypeError, "The array as argument must have 1 dimension" );\
      throw error_already_set();\
      }\
    if( dim != 0 && a_obj->dimensions[ 0 ] != dim ) \
      {\
      std::stringstream ss;\
      ss << "The argument must be of size " << dim;\
      PyErr_SetString(PyExc_TypeError, ss.str().c_str() );\
      throw error_already_set();\
      }\
    int a_type = 0; \
    int a_require = NPY_CONTIGUOUS|NPY_FORCECAST; \
    CHECK_TYPE( a_type, C_TYPE );\
    PyObject *array = PyArray_FromArray( a_obj, PyArray_DescrFromType( a_type ), a_require ); \
    if( array == NULL )\
      {\
      throw error_already_set();\
      }\
    int nb_elmt = PyArray_DIM( array, 0 ); \
    int i, j; \
    ARRAY##Ptr result = new ARRAY( nb_elmt ); \
    for( i = 0 ; i < nb_elmt ; ++i ) \
      {\
      result->setAt( i, *( C_TYPE*) PyArray_GETPTR1( array, i ) );\
      }\
    Py_DECREF( array );\
    return result;\
    } \
  return PREFIX##_fromlist( l ); \
}\

#define DEFINE_NUMPY( PREFIX ) .def( "__init__", make_constructor( PREFIX##_fromnumpy ) )

#else

#define EXPORT_NUMPY( PREFIX, T, ARRAY, DIM0, DIM1, C_TYPE )
#define EXPORT_NUMPY_1DIM( PREFIX, T, ARRAY, DIM, C_TYPE )

#define DEFINE_NUMPY( PREFIX )

#endif

