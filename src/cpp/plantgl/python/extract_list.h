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


#ifndef __extract_list_h__
#define __extract_list_h__

#include <vector>
#include <map>
#include "boost_python.h"
#include "exception.h"
#include "pyseq_iterator.h"

/* ----------------------------------------------------------------------- */

template<class T,
    template < typename > class extractor_t = boost::python::extract,
    class result_type = std::vector<T> >
struct extract_vec {

    typedef T element_type;
    typedef extractor_t<T> extractor_type;

    extract_vec(boost::python::object _pylist):pylist(_pylist) {}
    boost::python::object pylist;

    result_type extract() const {
        result_type result;
        if (pylist.ptr() == Py_None) return result;

        PySeqIterator iter_obj ( pylist );
        while(iter_obj.is_valid()){
            element_type val = extractor_type( iter_obj.next() )();
            result.push_back( val );

        }

        return result;
    }

    inline result_type operator()() const { return extract(); }
    inline operator result_type () const { return extract(); }
};

/* ----------------------------------------------------------------------- */

template<class T, class U = T,
    template < typename > class extractor_t0 = boost::python::extract,
    template < typename > class extractor_t1 = boost::python::extract,
    class result_type = std::vector<std::pair<T,U> > >
struct extract_vec_pair {

    typedef T element_type0;
    typedef U element_type1;
    typedef extractor_t0<T> extractor_type0;
    typedef extractor_t1<U> extractor_type1;

    extract_vec_pair(boost::python::object _pylist):pylist(_pylist) {}
    boost::python::object pylist;

    result_type extract() const {
        result_type result;
        if (pylist.ptr() == Py_None) return result;

        PySeqIterator iter_obj ( pylist );
        while(iter_obj.is_valid()){
            boost::python::object obj = iter_obj.next() ;
            element_type0 val0 = extractor_type0( obj[0] )();
            element_type1 val1 = extractor_type1( obj[1] )();
            result.push_back( std::pair<element_type0,element_type1>(val0,val1) );

        }

        return result;
    }

    inline result_type operator()() const { return extract(); }
    inline operator result_type () const { return extract(); }
};

/* ----------------------------------------------------------------------- */

template<class T, class U = T,
    template < typename > class extractor_t0 = boost::python::extract,
    template < typename > class extractor_t1 = boost::python::extract,
    class result_type = std::pair<T,U> >
struct extract_pair {

    typedef T element_type0;
    typedef U element_type1;
    typedef extractor_t0<T> extractor_type0;
    typedef extractor_t1<U> extractor_type1;

    extract_pair(boost::python::object _pylist):pylist(_pylist) {}
    boost::python::object pylist;

    result_type extract() const {
        if (pylist.ptr() == Py_None) return result_type();
        element_type0 val0 = extractor_type0( pylist[0] )();
        element_type1 val1 = extractor_type1( pylist[1] )();
        return std::pair<element_type0,element_type1>(val0,val1);
    }

    inline result_type operator()() const { return extract(); }
    inline operator result_type () const { return extract(); }
};

/* ----------------------------------------------------------------------- */

template<class key,  class value,
     template < typename> class key_extractor_t = boost::python::extract,
     template < typename> class value_extractor_t = boost::python::extract,
     class result_type = std::map<key,value> >
struct extract_dict {

    typedef key key_type;
    typedef value value_type;
    typedef key_extractor_t<key>  key_extractor_type;
    typedef value_extractor_t<value>  value_extractor_type;

        extract_dict(boost::python::object _pydict):
      pydict(boost::python::extract<boost::python::dict>(_pydict)) {}

    extract_dict(boost::python::dict _pydict):
      pydict(_pydict) {}

    boost::python::dict pydict;

    result_type extract() const {
        result_type result;

        PySeqIterator iter_obj(pydict.iteritems(), true);
        while(iter_obj.is_valid()){
            boost::python::object obj = iter_obj.next();
            key_type k = key_extractor_type( obj[0] );
            value_type v = value_extractor_type( obj[1] );
            result[k] = v ;
        }
        return result;
    }
    inline result_type operator()() const { return extract(); }
    inline operator result_type () const { return extract(); }
};


#endif

/* ----------------------------------------------------------------------- */
