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



#ifndef __export_vector_h__
#define __export_vector_h__

#include <plantgl/math/util_vector.h>
#include <plantgl/python/exception.h>
#include <boost/python.hpp>
#include <boost/python/def_visitor.hpp>

using namespace boost::python;

template<class V>
real_t vec_getx(V * v){ return v->x(); }

template<class V>
real_t vec_gety(V * v){ return v->y(); }

template<class V>
real_t vec_getz(V * v){ return v->z(); }

template<class V>
real_t vec_getw(V * v){ return v->w(); }

template<class V>
void vec_setx(V * v, real_t val){ v->x() = val; }

template<class V>
void vec_sety(V * v, real_t val){ v->y() = val; }

template<class V>
void vec_setz(V * v, real_t val){ v->z() = val; }

template<class V>
void vec_setw(V * v, real_t val){ v->w() = val; }


template <class V>
real_t vec_size(V * v){ return v->size(); }

template <class V>
real_t vec_getAt(V * v, size_t i)
{
  if (i < v->size()) return v->getAt(i);
  else throw PythonExc_IndexError();
}

template <class V>
void vec_setAt(V * v, size_t i, real_t val)
{
  if (i < v->size()) v->setAt(i,val);
  else throw PythonExc_IndexError();
}

template <class V>
real_t vec_norm(const V& v){ return norm(v); }

template <class V>
real_t vec_normL1(const V& v){ return normL1(v); }

template <class V>
real_t vec_normLinf(const V& v){ return normLinf(v); }

template <class V>
real_t vec_normSquared(const V& v){ return normSquared(v); }

template <class V>
V vec_direction(const V& v){ return direction(v); }

template <class V>
V vec_abs(const V& v){ return abs(v); }

template<class V>
class vector_base_func : public boost::python::def_visitor<vector_base_func<V> >
{
    friend class boost::python::def_visitor_access;

    template <class classT>
    void visit(classT& c) const
    {
        c.def( self + self )
         .def( -self )
         .def( self / other< real_t >() )
         .def( self * other< real_t >() )
         .def( other< real_t >() * self )
         .def( self == self )
         .def( self != self )
         .def( self += self )
         .def( self /= other< real_t >() )
         .def( self - self )
         .def( self % self )
         .def( "cwiseProduct", & V::cwiseProduct, "component wise product." )
         .def( "normalize", & V::normalize, "Normalizes self and returns the norm before." )
         .def( "normed", & V::normed, "Return a normed version of self." )
         .def( "isNormalized", & V::isNormalized, "Returns whether self is normalized." )
         .def( "isOrthogonalTo", & V::isOrthogonalTo, args("v"), "Returns whether self is orthogonal to v." )
         .def( "isValid", & V::isValid , "Returns whether self is valid.")
         .def( "getMaxAbsCoord", &V::getMaxAbsCoord , "Returns the index of the maximum absolute coordinate.")
         .def( "getMinAbsCoord", &V::getMinAbsCoord , "Returns the index of the minimum absolute coordinate.")
         .def( "__abs__",          &vec_abs<V> ,         "Returns the absolute value of self.")
         .def( "__norm__",         &vec_norm<V> ,        "Returns the norm of self.")
         .def( "__normL1__",       &vec_normL1<V> ,      "Returns the L1 (Manhattan) norm of self.")
         .def( "__normLinf__",     &vec_normLinf<V> ,    "Returns the L-infinite norm of self.")
         .def( "__normSquared__",  &vec_normSquared<V> , "Returns the square of the norm of self." )
         .def( "__direction__",          &vec_direction<V> ,   "Returns the direction of self.")
         .def( "__getitem__", &vec_getAt<V> )
         .def( "__setitem__", &vec_setAt<V> )
         .def( "__len__", &vec_size<V> )
         ;
    }
};


template<class V>
class vector_crossdot : public boost::python::def_visitor<vector_crossdot<V> >
{
    friend class boost::python::def_visitor_access;

    template <class classT>
    void visit(classT& c) const
    {
        c.def( self * self )
         .def( self ^ self )
         ;
    }
};

template<class V>
V getOrigin() { return V::ORIGIN; }

template<class V>
void setOrigin(const V&) { }

template<class V>
class vector_dim2_func : public boost::python::def_visitor<vector_dim2_func<V> >
{
    friend class boost::python::def_visitor_access;

    template <class classT>
    void visit(classT& c) const
    {
        c.def(vector_base_func<V>())
         .add_property( "x", vec_getx<V>, vec_setx<V> )
         .add_property( "y", vec_gety<V>, vec_sety<V> )
         .add_static_property( "ORIGIN", make_getter(&V::ORIGIN) )
         .add_static_property( "OX", make_getter(V::OX))
         .add_static_property( "OY", make_getter(V::OY))
        ;
    }
};

template<class V>
class vector_dim3_func : public boost::python::def_visitor<vector_dim3_func<V> >
{
    friend class boost::python::def_visitor_access;

    template <class classT>
    void visit(classT& c) const
    {
        c.def(vector_dim2_func<V>())
         .add_property( "z", vec_getz<V>, vec_setz<V> )
         .add_static_property( "OZ", make_getter(V::OZ))
         .def("project",&V::project)
        ;
    }
};

template<class V>
class vector_dim4_func : public boost::python::def_visitor<vector_dim4_func<V> >
{
    friend class boost::python::def_visitor_access;

    template <class classT>
    void visit(classT& c) const
    {
        c.def(vector_dim3_func<V>())
         .add_property( "w", vec_getw<V>, vec_setw<V> )
         .add_static_property( "OW", make_getter(V::OW))
        ;
    }
};

template<class V>
void vec_setradius(V * v, real_t val){ v->radius = val; }

template<class V>
void vec_settheta(V * v, real_t val){ v->theta = val; }

template<class V>
void vec_setphi(V * v, real_t val){ v->phi = val; }

template<class V>
void vecp_setz(V * v, real_t val){ v->z = val; }


#endif
