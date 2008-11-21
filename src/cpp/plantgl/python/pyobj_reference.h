#ifndef __pyobj_reference__
#define __pyobj_reference__

#ifdef PYTHON_DWA2002810_HPP
#ifdef __GNUC__
#warning boost/python.hpp already included
#endif
#endif

#include <plantgl/tool/rcobject.h>

#ifdef WITH_REFCOUNTLISTENER

//
#include <Python.h>
#include <iostream>
/*!
    \class PyObjectLink
	\brief This class is used to propagates reference count of a RefCountObject to its PyObject.
*/
class PyObjectLink : public TOOLS(RefCountListener) {
public:
	PyObjectLink(PyObject * self = NULL, TOOLS(RefCountObject) * pgl_object = NULL) : 
	  m_self(self) {
		  if (pgl_object) {
				size_t rc = pgl_object->use_count();
				if (m_self) {
					for (size_t i = 1; i < rc; ++i) { Py_INCREF(m_self); }
				}
		  }
	}
    
	virtual void referenceAdded(TOOLS(RefCountObject) * obj) 
	  {   // One of the reference count is due to the py object. We dont want to consider it
		  if (m_self && obj->use_count() > 1) Py_INCREF(m_self);  
	  }

	virtual void referenceRemoved(TOOLS(RefCountObject) * obj)  
	  { if (m_self && obj->use_count() > 1) Py_DECREF(m_self); }
	virtual void objectDeleted(TOOLS(RefCountObject) * obj) 
	  { m_self = NULL; }

	PyObject * m_self;
};

namespace boost {

	inline void intrusive_ptr_set_pyobject(TOOLS(RefCountObject) * ptr, 
                                                PyObject * pyobject)
	{  ptr->setRefCountListener(new PyObjectLink(pyobject,ptr)); }

	inline void intrusive_ptr_clear_pyobject(TOOLS(RefCountObject) * ptr)
	{ 
		if(ptr->getRefCountListener())
		((PyObjectLink *)ptr->getRefCountListener())->m_self = NULL; 
	}

	inline PyObject * intrusive_ptr_get_pyobject(const TOOLS(RefCountObject) * ptr)
	{  
		if(ptr->getRefCountListener())
			return ((PyObjectLink *)ptr->getRefCountListener())->m_self; 
		else return NULL;
	}
}

namespace boost { namespace python { namespace detail {
   	// A magic boost function to associate a pyobject with a c++ object. Normally used for boost::python::wrapper
	inline void initialize_wrapper(PyObject* self, TOOLS(RefCountObject)* w){
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

#endif


#endif

