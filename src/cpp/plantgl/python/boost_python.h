#ifndef __pgl_boost_h__
#define __pgl_boost_h__

#include <boost/python.hpp>
#define bp boost::python
#include <boost/version.hpp>


/* ----------------------------------------------------------------------- */

#if BOOST_VERSION < 103400
namespace boost { namespace python {
inline size_t len(bp::object t) {
	return boost::python::extract<size_t>(t.attr("__len__")())();
}
  } }

#endif


#endif

/* ----------------------------------------------------------------------- */
