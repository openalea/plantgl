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

/*! \file util_messages.h
    \brief Definition of error message parameter.
*/


#ifndef __util_messages_h__
#define __util_messages_h__


#include <plantgl/tool/errormsg.h>
#include "sceneobject.h"

/* ----------------------------------------------------------------------- */

/// Name of the module
#define MODULE_NAME "GEOM"
/// Geom Error Message Array
#define ERR_MSG_ARRAY GEOM_MESSAGES
/// Geom Comment Stream
#define OUTPUT_COMMENT_STREAM *(SceneObject::commentStream)
/// Geom Warning Stream
#define OUTPUT_WARNING_STREAM *(SceneObject::warningStream)
/// Geom Error Stream
#define OUTPUT_ERROR_STREAM  *(SceneObject::errorStream)
/// Geom Error Value
#define ERROR_VALUE -1


/* ----------------------------------------------------------------------- */

  /*! \enum GEOM_MESSAGE_LABELS    
  Geom Error Message Label Identifiant.
  */
  /*!
  \var GEOM_MESSAGE_LABELS UNNAMED_OBJECT
  For message : "Object is unnamed, cannot record it.".
  */
  /*!
  \var GEOM_MESSAGE_LABELS DECLARED_OBJECT_ss
   For message : "< \b type > Object ' \b name ' already declared.".
  */
  /*!
  \var GEOM_MESSAGE_LABELS UNDECLARED_OBJECT_s
    For message : "Object ' \b name ' is undeclared, cannot record it.".
  */
  /*!
  \var GEOM_MESSAGE_LABELS INITIALIZED_FIELD_ss
    For message : "< \b type > Field 'fieldname' already initialized, replace the existing value(s).".
  */
  /*!
  \var GEOM_MESSAGE_LABELS UNINITIALIZED_FIELD_ss
    For message : "< \b type > Field 'fieldname' is uninitialized, cannot create the object.".
  */
  /*!
  \var GEOM_MESSAGE_LABELS INVALID_FIELD_VALUE_sss
    For message : "< \b type > Field ' \b fieldname ' has a bad value : \b value ".
  */
  /*!
  \var GEOM_MESSAGE_LABELS INVALID_FIELD_SIZE_sss
    For message : "< \b type > Field ' \b fieldname ' has a bad size. \b size ".
  */
  /*!
  \var GEOM_MESSAGE_LABELS INVALID_FIELD_ITH_VALUE_ssss
    For message : "< \b type > Field ' \b fieldname ' has a bad ' \b i 'th value. \b value ".
  */
  /*!
  \var GEOM_MESSAGE_LABELS CANNOT_DISCRETIZE_ss
    For message : "< \b type > Cannot discretize this geometric object. \b comment ".
  */
  /*!
  \var GEOM_MESSAGE_LABELS CANNOT_DEFORM_ss
    For message : "< \b type > Cannot deform this geometric object. \b comment ".
  */
  /*!
  \var GEOM_MESSAGE_LABELS INVALID_FIELD_VECTOR_TYPE_ssss
    For message : "< \b type > Field ' \b fieldname ' has a bad ' \b i 'th value. \b comment ".
  */
  /*!
  \var GEOM_MESSAGE_LABELS INVALID_TYPE_sss
   For message : "< \b type > Invalid Type for ' \b obj '. Found ' \b type '".
  */
enum  GEOM_MESSAGE_LABELS  {
  UNNAMED_OBJECT = COMMON_ERR_OFFSET,
  DECLARED_OBJECT_ss,
  UNDECLARED_OBJECT_s,
  INITIALIZED_FIELD_ss,
  UNINITIALIZED_FIELD_ss,
  INVALID_FIELD_VALUE_sss,
  INVALID_FIELD_SIZE_sss,
  INVALID_FIELD_ITH_VALUE_ssss,
  CANNOT_DISCRETIZE_ss,
  CANNOT_DEFORM_ss,
  INVALID_FIELD_VECTOR_TYPE_ssss,
  INVALID_TYPE_sss,
};


/// Geom Error Message Array
SG_API extern const char * GEOM_MESSAGES[];


/* ----------------------------------------------------------------------- */

// __util_messages_h_
#endif
