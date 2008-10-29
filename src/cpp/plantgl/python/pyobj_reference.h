#ifndef __pyobj_reference__
#define __pyobj_reference__

#include <plantgl/tool/rcobject.h>

#ifdef WITH_REFCOUNTLISTENER

#include <boost/python.hpp>
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

	inline void intrusive_ptr_set_pyobject(TOOLS(RefCountObject) * ptr, 
                                                PyObject * pyobject)
	{  ptr->setRefCountListener(new PyObjectLink(pyobject,ptr)); }

	inline void intrusive_ptr_clear_pyobject(TOOLS(RefCountObject) * ptr)
	{ 
		if(ptr->getRefCountListener())
		((PyObjectLink *)ptr->getRefCountListener())->m_self = NULL; 
	}

	inline PyObject * intrusive_ptr_get_pyobject(TOOLS(RefCountObject) * ptr)
	{  
		if(ptr->getRefCountListener())
			return ((PyObjectLink *)ptr->getRefCountListener())->m_self; 
		else return NULL;
	}

	namespace python { namespace detail {
		// A magic boost function to associate a pyobject with a c++ object. Used normally for boost::python::wrapper
		inline void initialize_wrapper(PyObject* self, TOOLS(RefCountObject)* w){
			intrusive_ptr_set_pyobject(w,self);
		}
	}}
}

// A helper template function to define __del__ python function
template<class T>
void pydel(T * obj) {  boost::intrusive_ptr_clear_pyobject(obj); }

#endif


#endif

