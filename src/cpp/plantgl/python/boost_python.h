#ifndef __pgl_boost_h__
#define __pgl_boost_h__

#include <boost/python.hpp>
#define bp boost::python
#include <boost/version.hpp>


/* ----------------------------------------------------------------------- */

#if BOOST_VERSION < 103400
namespace boost { namespace python {
inline size_t len(bp::object t) { return (size_t)PySequence_Size(t.ptr()); }
  } }

#endif

#if (BOOST_VERSION < 104100) && (PY_VERSION_HEX > 020602)
#ifdef __GNUC__
#warning "Upgrade your version of boost python (recommanded >=1.41)."
#else
#pragma message("Upgrade your version of boost python (recommanded >=1.41).")
#endif
#endif


#endif

/* ----------------------------------------------------------------------- */
