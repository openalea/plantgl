/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       GeomPy: Python wrapper for the Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR AMAP 
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
 *
 *  ----------------------------------------------------------------------------
 *
 *                      GNU General Public Licence
 *
 *       This program is free software; you can redistribute it and/or
 *       modify it under the terms of the GNU General Public License as
 *       published by the Free Software Foundation; either version 2 of
 *       the License, or (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public
 *       License along with this program; see the file COPYING. If not,
 *       write to the Free Software Foundation, Inc., 59
 *       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  ----------------------------------------------------------------------------
 */
#include <math/util_math.h>

#define SETGET(_CLASS,PROP,TYPE)\
TYPE get_##_CLASS##_##PROP(_CLASS * obj){ \
  return obj->get##PROP(); \
} \
void set_##_CLASS##_##PROP(_CLASS * obj, TYPE v){ \
  obj->get##PROP() = v; \
} \

#define SETGET_ANGLE(_CLASS,PROP)\
real_t get_##_CLASS##_##PROP(_CLASS * obj){ \
  return obj->get##PROP(); \
} \
void set_##_CLASS##_##PROP(_CLASS * obj, real_t v){ \
  obj->get##PROP() =(real_t) fmod((double)v,(double)2 * GEOM_PI); \
} \


#define DEC_DEFAULTVAL(PROPNAME,_CLASS,PROP,TYPE) \
  .def("is"#PROP"ToDefault",&_CLASS::is##PROP##ToDefault,return_value_policy<return_by_value>()) \


#define DEC_SETGET(PROPNAME,_CLASS,PROP,TYPE) \
  add_property(#PROPNAME,get_##_CLASS##_##PROP,set_##_CLASS##_##PROP) \


#define DEC_SETGET_WD(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_SETGET(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_DEFAULTVAL(PROPNAME,_CLASS,PROP,TYPE)
 

#define DEC_PROP(PROPNAME,_CLASS,PROP,TYPE) \
 add_property(#PROPNAME, \
	make_getter((const TYPE& (_CLASS::*)() const)&_CLASS::get##PROP, \
				 return_internal_reference<>()), \
	make_setter((TYPE& (_CLASS::*)())&_CLASS::get##PROP, \
				return_internal_reference<>())) \


#define DEC_PROP_WD(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_PROP(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_DEFAULTVAL(PROPNAME,_CLASS,PROP,TYPE)
