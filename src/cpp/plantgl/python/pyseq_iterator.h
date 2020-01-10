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


#ifndef __pyseqiterator_h__
#define __pyseqiterator_h__

#include "boost_python.h"

/* ----------------------------------------------------------------------- */

class PySeqIterator {
public:

    PySeqIterator(boost::python::object seq, bool isiterator = false) :
        __iter_obj( ), __valid(true)
        {
            if (isiterator){
                __iter_obj = seq;
                __valid = (__iter_obj != boost::python::object());
            }
            else {
                PyObject * pyiter = PyObject_GetIter( seq.ptr() ) ;
                __valid = (pyiter != NULL);
                __iter_obj =  boost::python::object(boost::python::handle<>( pyiter ) );

            }
            _next();
        }

    inline bool is_valid() const { return __valid;}

    inline boost::python::object next() ;

protected:
    inline void _next() ;

    boost::python::object __iter_obj;
    boost::python::object __next_obj;
    bool __valid;

};


boost::python::object PySeqIterator::next() 
{
    boost::python::object result = __next_obj;
    _next();
    return result;   

}

void PySeqIterator::_next() 
{
    if (__valid) {
        PyObject * item = PyIter_Next(__iter_obj.ptr());
        __valid = (item != NULL); 
        if (__valid)
            __next_obj = boost::python::object( boost::python::handle<PyObject>(item));
        else
            __next_obj = boost::python::object();
    }
}



#endif

/* ----------------------------------------------------------------------- */

