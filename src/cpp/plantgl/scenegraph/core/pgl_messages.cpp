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




#include <plantgl/scenegraph/core/pgl_messages.h>


/* ----------------------------------------------------------------------- */


const char * PGL_MESSAGES[] = {
  // UNNAMED_OBJECT
  "Object is unnamed, cannot record it.",
  // DECLARED_OBJECT_ss
  "<%s> Object '%s' already declared.",
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
  // FILE_ERR_s
  "Cannot access file '%s'.",
  // FILE_OPEN_ERR_s
  "Cannot open file '%s'.",
  // FILE_READ_ERR_s
  "Cannot read file '%s'.",
  // FILE_WRITE_ERR_s
  "Cannot write file '%s'."

};


/* ----------------------------------------------------------------------- */

