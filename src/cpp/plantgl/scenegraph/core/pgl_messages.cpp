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



#include <plantgl/scenegraph/core/pgl_messages.h>


/* ----------------------------------------------------------------------- */


const char * GEOM_MESSAGES[] = {
  // UNNAMED_OBJECT
  "Object is unnamed, cannot record it.",
  // DECLARED_OBJECT_ss
  "<%s> Object '%s' already declared. %s",
  // UNDECLARED_OBJECT_s
  "Object '%s' is undeclared, cannot record it.",
  // INITIALIZED_FIELD_ss,
  "<%s> Field '%s' already initialized, replace the existing value(s).",
  // UNINITIALIZED_FIELD_ss,
  "<%s> Field '%s' is uninitialized.",
  // INVALID_FIELD_VALUE_sss
  "<%s> Field '%s' has a bad value : %s",
  // INVALID_FIELD_SIZE_sss
  "<%s> Field '%s' has a bad size : %s",
  // INVALID_FIELD_ITH_VALUE_ssss
  "<%s> Field '%s' has a bad '%s'th value : %s",
  // CANNOT_DISCRETIZE_ss
  "<%s> Cannot discretize this geometric object. %s",
  // CANNOT_DEFORM_ss
  "<%s> Cannot deform this geometric object. %s",
  // INVALID_FIELD_VECTOR_TYPE_ssss
  "<%s> Field '%s' has a bad '%s'th value : %s",
  // INVALID_TYPE_sss,
  "<%s> Invalid Type for '%s'. Found '%s'",

};


/* ----------------------------------------------------------------------- */

