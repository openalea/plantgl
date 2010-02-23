
#include <plantgl/python/pyobj_reference.h>

template<class T>
RCPtr<T> py_recast(T * obj) { 
#ifdef MAINTAIN_PYTHON_OBJECT_ID
	boost::intrusive_ptr_clear_pyobject(obj);
#endif
	return RCPtr<T>(obj); 
}

#define DEF_PGLBASE(T) def("deepcopy",&T::casted_deepcopy<T>) \
                       .def("deepcopy",&T::casted_deepcopy_using<T>, args("copier")) \
		       .def("recast",&py_recast<T>)
 


