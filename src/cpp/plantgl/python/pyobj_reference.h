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

#ifndef __pyobj_reference__
#define __pyobj_reference__

#ifdef PYTHON_DWA2002810_HPP
#ifdef __GNUC__
#warning boost/python.hpp already included
#endif
#endif

#include <plantgl/tool/rcobject.h>

#ifndef WITH_REFCOUNTLISTENER
#undef MAINTAIN_PYTHON_OBJECT_ID
#endif

#ifdef MAINTAIN_PYTHON_OBJECT_ID
//
#include <Python.h>
#include <iostream>
/*!
    \class PyObjectLink
    \brief This class is used to propagates reference count of a RefCountObject to its PyObject.
*/
class PyObjectLink : public RefCountListener {
public:
    PyObjectLink(PyObject * self = NULL, RefCountObject * pgl_object = NULL) :
      m_self(self) {
          if (pgl_object) {
                size_t rc = pgl_object->use_count();
                if (m_self) {
                    for (size_t i = 1; i < rc; ++i) { Py_INCREF(m_self); }
                }
          }
    }

    virtual void referenceAdded(RefCountObject * obj)
      {   // One of the reference count is due to the py object. We dont want to consider it
          if (m_self && obj->use_count() > 1) Py_INCREF(m_self);
      }

    virtual void referenceRemoved(RefCountObject * obj)
      { if (m_self && obj->use_count() > 1) Py_DECREF(m_self); }
    virtual void objectDeleted(RefCountObject * obj)
      { m_self = NULL; }

    PyObject * m_self;
};

namespace boost {

    inline void intrusive_ptr_set_pyobject(RefCountObject * ptr,
                                                PyObject * pyobject)
    {  ptr->setRefCountListener(new PyObjectLink(pyobject,ptr)); }

    inline void intrusive_ptr_clear_pyobject(RefCountObject * ptr)
    {
        if(ptr->getRefCountListener())
        ((PyObjectLink *)ptr->getRefCountListener())->m_self = NULL;
    }

    inline PyObject * intrusive_ptr_get_pyobject(const RefCountObject * ptr)
    {
        if(ptr->getRefCountListener())
            return ((PyObjectLink *)ptr->getRefCountListener())->m_self;
        else return NULL;
    }
}

namespace boost { namespace python { namespace detail {
    // A magic boost function to associate a pyobject with a c++ object. Normally used for boost::python::wrapper
    inline void initialize_wrapper(PyObject* self, RefCountObject* w){
        intrusive_ptr_set_pyobject(w,self);
    }

}}}

#define BOOST_INITIALIZE_WRAPPER_FIX_DECLARE(T) \
   class T; \
   namespace boost { namespace python { namespace detail { \
    void initialize_wrapper(PyObject* self, T * w); \
    }}} \

// resolve conflict between initialize_wrapper for RefCountObject and for boost::python::base
#define BOOST_INITIALIZE_WRAPPER_FIX(T) \
    namespace boost { namespace python { namespace detail { \
        inline void initialize_wrapper(PyObject* self, T * w){ \
            boost::python::detail::initialize_wrapper(self,(boost::python::detail::wrapper_base*)w); \
            boost::intrusive_ptr_set_pyobject(w,self); \
        } \
    }}} \

// A helper template function to define __del__ python function
template<class T>
void pydel(T * obj) {  boost::intrusive_ptr_clear_pyobject(obj); }

#else

#define BOOST_INITIALIZE_WRAPPER_FIX_DECLARE(T)
#define BOOST_INITIALIZE_WRAPPER_FIX(T)

#endif


#endif

