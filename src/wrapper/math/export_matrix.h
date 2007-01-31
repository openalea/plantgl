/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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

#ifndef __export_matrix_h__
#define __export_matrix_h__

#define GET_ELT(MATRIX) \
real_t MATRIX##_getElt( const MATRIX & m, tuple& t ) \
{ \
  if( PyObject_Size(t.ptr()) != 2 ) \
    throw PythonExc_ValueError(); \
  extract<uchar_t> i(t[0]); \
  extract<uchar_t> j(t[1]); \
  if (i.check() && j.check() ) \
    return m(i(),j()); \
  else \
    throw PythonExc_TypeError(); \
} \

#define SET_ELT(MATRIX) \
void MATRIX##_setElt(  MATRIX & m, tuple& t, real_t v ) \
{ \
  if( PyObject_Length(t.ptr()) != 2 ) \
    throw PythonExc_ValueError(); \
  extract<uchar_t> i(t[0]); \
  extract<uchar_t> j(t[1]); \
  if (i.check() && j.check() ) \
    m(i(),j())= v; \
  else \
    throw PythonExc_TypeError(); \
} \

#endif