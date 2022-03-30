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


#ifndef __export_list_h__
#define __export_list_h__

#include <boost/python.hpp>
#include <vector>
#include <plantgl/tool/util_hashmap.h>
#include "exception.h"

/* ----------------------------------------------------------------------- */
template <class T>
struct make_object {
    typedef T obj_type;

    const obj_type& __c_obj;

    make_object(const T& c_obj) : __c_obj(c_obj) {}
    boost::python::object operator()() const { return boost::python::object(__c_obj); }
};

template <class Iterator, class Translator>
static boost::python::object list_convert(Iterator beg, Iterator end)  {
        boost::python::list l;
        for (Iterator it = beg; it != end; ++it)
            l.append(Translator(*it)());
        return l;
    }

template <class T, class Translator = make_object<typename T::value_type> >
struct list_converter {
    typedef T list_type;
    typedef typename T::const_iterator list_const_iterator;
    typedef typename T::value_type list_element_type;

    list_const_iterator __c_list_begin;
    list_const_iterator __c_list_end;

    list_converter(const T& c_list): __c_list_begin(c_list.begin()),__c_list_end(c_list.end()){}
    list_converter(const list_const_iterator& c_list_begin,
              const list_const_iterator& c_list_end):
            __c_list_begin(c_list_begin),__c_list_end(c_list_end){}


    inline boost::python::object convert() const { return list_convert<list_const_iterator,Translator>(__c_list_begin,__c_list_end); }
    inline boost::python::object operator()() const { return convert(); }
    inline operator boost::python::object () const { return convert(); }
};

template <class T, class Translator>
list_converter<T,Translator> make_list(const T& c_list) { return list_converter<T,Translator>(c_list); }

template <class T>
list_converter<T> make_list(const T& c_list) { return list_converter<T>(c_list); }

template<class T>
list_converter<T, list_converter<typename T::value_type> >
make_list_of_list(const T& l){
    return list_converter<T, list_converter<typename T::value_type> >(l);
}

/* ----------------------------------------------------------------------- */

template <class T, class Translator = make_object<typename T::value_type> >
struct tuple_converter {
    typedef T list_type;
    typedef typename T::const_iterator list_const_iterator;
    typedef typename T::value_type list_element_type;

    list_const_iterator __c_list_begin;
    list_const_iterator __c_list_end;

    tuple_converter(const T& c_list): __c_list_begin(c_list.begin()),__c_list_end(c_list.end()){}
    tuple_converter(const list_const_iterator& c_list_begin,
              const list_const_iterator& c_list_end):
            __c_list_begin(c_list_begin),__c_list_end(c_list_end){}

    inline boost::python::object convert() const {
        return boost::python::tuple(list_convert<list_const_iterator,Translator>(__c_list_begin,__c_list_end));
    }
    inline boost::python::object operator()() const { return convert(); }
    inline operator boost::python::object () const { return convert(); }
};

template <class T, class Translator>
tuple_converter<T> make_pgl_tuple(const T& c_tuple) { return tuple_converter<T,Translator>(c_tuple); }

template <class T>
tuple_converter<T> make_pgl_tuple(const T& c_tuple) { return tuple_converter<T>(c_tuple); }

/* ----------------------------------------------------------------------- */

template <class T,
          class KeyTranslator = make_object<typename T::key_type>,
#if (defined(USING_UNORDERED_MAP)) || defined(WIN32_STL_EXTENSION)
          class ValueTranslator = make_object<typename T::mapped_type>
#else
          class ValueTranslator = make_object<typename T::data_type>
#endif
         ,
#if (defined(USING_UNORDERED_MAP)) || defined(WIN32_STL_EXTENSION)
          class ValueType = typename T::mapped_type
#else
          class ValueType = typename T::data_type
#endif
         >
struct dict_converter {
    typedef T dict_type;
    typedef typename T::const_iterator dict_const_iterator;
    typedef typename T::key_type dict_key_type;
    typedef ValueType dict_value_type;

    dict_const_iterator __c_dict_begin;
    dict_const_iterator __c_dict_end;

    dict_converter(const T& c_dict): __c_dict_begin(c_dict.begin()),__c_dict_end(c_dict.end()){}
    dict_converter(const dict_const_iterator& c_dict_begin,const dict_const_iterator& c_dict_end):
        __c_dict_begin(c_dict_begin),__c_dict_end(c_dict_end){}

    boost::python::object convert() const {
        boost::python::dict d;
        for (dict_const_iterator it = __c_dict_begin; it != __c_dict_end; ++it)
            d[KeyTranslator(it->first)()] = ValueTranslator(it->second)();
        return d;
    }
    inline boost::python::object operator()() const { return convert(); }
    inline operator boost::python::object () const { return convert(); }
};

template <class T, class KeyTranslator, class ValueTranslator>
dict_converter<T,KeyTranslator,ValueTranslator> make_dict(const T& c_dict)
{ return dict_converter<T,KeyTranslator,ValueTranslator>(c_dict); }

template <class T, class KeyTranslator>
dict_converter<T,KeyTranslator> make_dict(const T& c_dict)
{ return dict_converter<T,KeyTranslator>(c_dict); }

template <class T>
dict_converter<T> make_dict(const T& c_dict)
{ return dict_converter<T>(c_dict); }


template<class T, class U>
inline boost::python::object make_pair_tuple(const std::pair<T,U>& value) { return boost::python::make_tuple(value.first, value.second); }

#endif


/* ----------------------------------------------------------------------- */
