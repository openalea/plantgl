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


/*! \file util_messages.h
    \brief Definition of error message parameter.
*/


#ifndef __util_messages_h__
#define __util_messages_h__


#include <plantgl/tool/errormsg.h>
#include "../sg_config.h"

/* ----------------------------------------------------------------------- */

/// Geom Error Message Array
#define PGL_ERR_MSG_ARRAY PGL_MESSAGES


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
enum  PGL_MESSAGE_LABELS  {
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
  FILE_ERR_s,
  FILE_OPEN_ERR_s,
  FILE_READ_ERR_s,
  FILE_WRITE_ERR_s
};


/// Geom Error Message Array
SG_API extern const char * PGL_MESSAGES[];


/* ----------------------------------------------------------------------- */

// __util_messages_h_
#endif
