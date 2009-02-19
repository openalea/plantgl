/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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

#ifndef __export_property_h__
#define __export_property_h__

#include <plantgl/tool/util_types.h>
#include <plantgl/math/util_math.h>

template <class U,class T, const U& (T::* func)() const >
U get_prop_bt_from_class(T * obj){  return (obj->*func)(); }

template <class U,class T, const U (T::* func)() const >
U get_prop_bt_nr_from_class(T * obj){  return (obj->*func)(); }

template <class U,class T, U& (T::* func)() >
void set_prop_bt_from_class(T * obj, U val){  (obj->*func)() = val; }

template <class U,class T, const U& (T::* func)() const >
const U& get_prop_ct_from_class(T * obj){  return (obj->*func)(); }

template <class U,class T, U& (T::* func)() >
void set_prop_ct_from_class(T * obj, const U& val){  (obj->*func)() = val; }

template <class U,class T, const U& (T::* func)() const >
U get_prop_ptr_from_class(T * obj){  return (obj->*func)(); }

template <class U,class T, const U (T::* func)() const >
U get_prop_ptr_nr_from_class(T * obj){  return (obj->*func)(); }

template <class U,class T, U& (T::* func)() >
void set_prop_ptr_from_class(T * obj, U val){  (obj->*func)() = val; }

template <class T, real_t& (T::* func)() >
void set_prop_ang_from_class(T * obj, real_t val){  (obj->*func)() = (real_t) fmod((double)val,(double)2 * GEOM_PI); }

template <class T, const T * static_property> 
T retrieve_static_ptr_property() { return *static_property; }

/* --------------------
  Type of the element :
   _BT : Basic Type : real_t, int, uint_t, ...
   _CT : Complex Type : Vector3, Color3, Index, ...
   _PTR : Pointer : GeometryPtr

   _NR : Not using reference
   -------------------- */

#define DEC_DEFAULTVALTEST(_CLASS,PROP) \
  .def("is"#PROP"ToDefault",&_CLASS::is##PROP##ToDefault,return_value_policy<return_by_value>()) \

#define DEC_DEFAULTVAL(_CLASS,DEFAULTVAL) \
   .add_static_property(#DEFAULTVAL,make_getter(&_CLASS::DEFAULTVAL))

/* In case of Null SmartPtr, make_getter do not translate correctly the value.
   We thus create the wrapping function 'retrieve_static_ptr_property' 
   to get the static value which will be translate correctly as normal function result */
#define DEC_DEFAULTVAL_PTR(_CLASS,DEFAULTVAL,TYPE) \
   .add_static_property(#DEFAULTVAL,&retrieve_static_ptr_property<TYPE,&_CLASS::DEFAULTVAL>)

#define DEC_BT_PROPERTY(PROPNAME,_CLASS,PROP,TYPE) \
    add_property(#PROPNAME,&get_prop_bt_from_class<TYPE,_CLASS,&_CLASS::get##PROP>, \
                           &set_prop_bt_from_class<TYPE,_CLASS,&_CLASS::get##PROP>) 

#define DEC_BT_NR_PROPERTY(PROPNAME,_CLASS,PROP,TYPE) \
    add_property(#PROPNAME,&get_prop_bt_nr_from_class<TYPE,_CLASS,&_CLASS::get##PROP>, \
                           &set_prop_bt_from_class<TYPE,_CLASS,&_CLASS::get##PROP>) 

#define DEC_BT_PROPERTY_WD(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_BT_PROPERTY(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_DEFAULTVALTEST(_CLASS,PROP)
 
#define DEC_BT_PROPERTY_WDV(PROPNAME,_CLASS,PROP,TYPE,DEFAULTVAL) \
  DEC_BT_PROPERTY(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_DEFAULTVALTEST(_CLASS,PROP) \
  DEC_DEFAULTVAL(_CLASS,DEFAULTVAL)
 
#define DEC_BT_NR_PROPERTY_WD(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_BT_NR_PROPERTY(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_DEFAULTVALTEST(_CLASS,PROP)
 

#define DEC_BT_NR_PROPERTY_WDV(PROPNAME,_CLASS,PROP,TYPE,DEFAULTVAL) \
  DEC_BT_NR_PROPERTY(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_DEFAULTVALTEST(_CLASS,PROP) \
  DEC_DEFAULTVAL(_CLASS,DEFAULTVAL)
 

#define DEC_ANGLE_PROPERTY(PROPNAME,_CLASS,PROP) \
    add_property(#PROPNAME,&get_prop_bt_from_class<real_t,_CLASS,&_CLASS::get##PROP>, \
                           &set_prop_ang_from_class<_CLASS,&_CLASS::get##PROP>) 

#define DEC_ANGLE_PROPERTY_WD(PROPNAME,_CLASS,PROP) \
  DEC_ANGLE_PROPERTY(PROPNAME,_CLASS,PROP) \
  DEC_DEFAULTVALTEST(_CLASS,PROP)
 
#define DEC_ANGLE_PROPERTY_WDV(PROPNAME,_CLASS,PROP,DEFAULTVAL) \
  DEC_ANGLE_PROPERTY(PROPNAME,_CLASS,PROP) \
  DEC_DEFAULTVALTEST(_CLASS,PROP) \
  DEC_DEFAULTVAL(_CLASS,DEFAULTVAL)
 

#define DEC_ANGLE_NR_PROPERTY(PROPNAME,_CLASS,PROP) \
    add_property(#PROPNAME,&get_prop_bt_nr_from_class<real_t,_CLASS,&_CLASS::get##PROP>, \
                           &set_prop_ang_from_class<_CLASS,&_CLASS::get##PROP>) 

#define DEC_ANGLE_NR_PROPERTY_WD(PROPNAME,_CLASS,PROP) \
  DEC_ANGLE_NR_PROPERTY(PROPNAME,_CLASS,PROP) \
  DEC_DEFAULTVALTEST(_CLASS,PROP)
 
#define DEC_ANGLE_NR_PROPERTY_WDV(PROPNAME,_CLASS,PROP,DEFAULTVAL) \
  DEC_ANGLE_NR_PROPERTY(PROPNAME,_CLASS,PROP) \
  DEC_DEFAULTVALTEST(_CLASS,PROP) \
  DEC_DEFAULTVAL(_CLASS,DEFAULTVAL)
 

#define DEC_CT_PROPERTY(PROPNAME,_CLASS,PROP,TYPE) \
    add_property(#PROPNAME,make_function(&get_prop_ct_from_class<TYPE,_CLASS,&_CLASS::get##PROP>,return_internal_reference<1>()), \
                           &set_prop_ct_from_class<TYPE,_CLASS,&_CLASS::get##PROP>) 

#define DEC_CT_PROPERTY_WD(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_CT_PROPERTY(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_DEFAULTVALTEST(_CLASS,PROP)

#define DEC_CT_PROPERTY_WDV(PROPNAME,_CLASS,PROP,TYPE,DEFAULTVAL) \
  DEC_CT_PROPERTY(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_DEFAULTVALTEST(_CLASS,PROP) \
  DEC_DEFAULTVAL(_CLASS,DEFAULTVAL)


#define DEC_PTR_PROPERTY(PROPNAME,_CLASS,PROP,TYPE) \
    add_property(#PROPNAME,&get_prop_ptr_from_class<TYPE,_CLASS,&_CLASS::get##PROP>, \
                           &set_prop_ptr_from_class<TYPE,_CLASS,&_CLASS::get##PROP> ) 

#define DEC_PTR_NR_PROPERTY(PROPNAME,_CLASS,PROP,TYPE) \
    add_property(#PROPNAME,&get_prop_ptr_nr_from_class<TYPE,_CLASS,&_CLASS::get##PROP>, \
                           &set_prop_ptr_from_class<TYPE,_CLASS,&_CLASS::get##PROP> ) 

#define DEC_PTR_PROPERTY_RO(PROPNAME,_CLASS,PROP,TYPE) \
    add_property(#PROPNAME,&get_prop_ptr_from_class<TYPE,_CLASS,&_CLASS::get##PROP>) 

#define DEC_PTR_PROPERTY_WD(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_PTR_PROPERTY(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_DEFAULTVALTEST(_CLASS,PROP)
 
#define DEC_PTR_PROPERTY_WDV(PROPNAME,_CLASS,PROP,TYPE,DEFAULTVAL) \
  DEC_PTR_PROPERTY(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_DEFAULTVALTEST(_CLASS,PROP) \
  DEC_DEFAULTVAL_PTR(_CLASS,DEFAULTVAL,TYPE)
 
#define DEC_PTR_NR_PROPERTY_WD(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_PTR_NR_PROPERTY(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_DEFAULTVALTEST(_CLASS,PROP)
 
#define DEC_PTR_NR_PROPERTY_WDV(PROPNAME,_CLASS,PROP,TYPE,DEFAULTVAL) \
  DEC_PTR_NR_PROPERTY(PROPNAME,_CLASS,PROP,TYPE) \
  DEC_DEFAULTVALTEST(_CLASS,PROP)\
  DEC_DEFAULTVAL_PTR(_CLASS,DEFAULTVAL,TYPE)
 

#endif
