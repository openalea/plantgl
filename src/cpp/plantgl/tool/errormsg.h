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


/*! \file errormsg.h
    \brief Procedure to generate error messages using parameters:
*/

#ifndef PGL_ERROR_MESSAGES_HEADER
#define PGL_ERROR_MESSAGES_HEADER

#include <iostream>
#include <string>
#include "tools_config.h"

/* ----------------------------------------------------------------------- */


  /*! \enum PglErrorType
  Error Message Type.
  */
  /*!
  \var PglErrorType NO_MESSAGE
  Error without message.
  */
  /*!
  \var PglErrorType ERROR_MESSAGE
   Error with message.
  */
  /*!
  \var PglErrorType WARNING_MESSAGE
    Warning message.
  */
enum PglErrorType { PGL_NO_MESSAGE, PGL_ERROR_MESSAGE, PGL_WARNING_MESSAGE, PGL_DEBUG_MESSAGE};

/* ----------------------------------------------------------------------- */

typedef void (* error_msg_handler_func) ( const std::string&, const char*, int );


TOOLS_API void register_error_handler(error_msg_handler_func f = NULL);
TOOLS_API void register_warning_handler(error_msg_handler_func f = NULL);
TOOLS_API void register_debug_handler(error_msg_handler_func f = NULL);
inline void reset_error_handler() { register_error_handler(); }
inline void reset_warning_handler() { register_warning_handler(); }
inline void reset_debug_handler() { register_debug_handler(); }

TOOLS_API void pglDebug(const char* msg ...);
TOOLS_API void pglWarning(const char* msg ...);
TOOLS_API void pglError(const char* msg ...);
TOOLS_API void pglError(PglErrorType errtype, const char* msg ...);

TOOLS_API void pglDebugEx(const char* file, int line, const char* msg ...);
TOOLS_API void pglWarningEx(const char* file, int line, const char* msg ...);
TOOLS_API void pglErrorEx(const char* file, int line, const char* msg ...);
TOOLS_API void pglErrorEx(PglErrorType errtype, const char* file, int line, const char* msg ...);

TOOLS_API void pglDebugEx(const char* file, int line, const std::string& msg );
TOOLS_API void pglWarningEx(const char* file, int line, const std::string& msg);
TOOLS_API void pglErrorEx(const char* file, int line, const std::string& msg);
TOOLS_API void pglErrorEx(PglErrorType errtype, const char* file, int line, const std::string& msg);

/* ----------------------------------------------------------------------- */
/*
  Namespace for the different Pgl Error Stream.
*/
class TOOLS_API PglErrorStream {
public:
  /// A stream in which builder put their comment message.
  static std::ostream * debug;

  /// A stream in which builder put their warning message.
  static std::ostream * warning;

  /// A stream in which builder put their error message.
  static std::ostream * error;

  /**
    \class PglStreamBinder
    During the lifetime of such object, the construction stream becomes the new PglErrorStreams.
    At deletion previous streams are reintegrated
  */
  class TOOLS_API Binder {
  public:

    Binder(std::ostream& error, std::ostream& warning, std::ostream& debug);
    Binder(std::ostream& stream);
    ~Binder();

  protected:
    std::ostream * previousDebug;
    std::ostream * previoudWarning;
    std::ostream * previousError;
    error_msg_handler_func previousDebugPrinter;
    error_msg_handler_func previousWarningPrinter;
    error_msg_handler_func previousErrorPrinter;
  };
};

/* ----------------------------------------------------------------------- */


/// General messages that can be used by different modules
extern const char* common_err_messages[];


// ******************************************************
// General messages that can be used by different modules
// ******************************************************

/// Max number of error messages in the Kernel
#define  COMMON_ERR_OFFSET      1000

/// General Aml messages that can be used by different modules
extern TOOLS_API const char* common_err_msgs_aml[];

  /*! \enum COMON_ERRORLABEL
  General Error Message Label Identifiant.
  */
  /*!
  \var COMON_ERRORLABEL C_NYI_ERR
  For message : "Sorry: message not yet implemented".
  */
  /*!
  \var COMON_ERRORLABEL C_VAR_BAD_VALUE_sss
   For message : "variable \b name : bad value \b value : should be \b value".
  */
  /*!
  \var COMON_ERRORLABEL C_FILE_ERR_s
    For message : "file access: cannot access file \b filename ".
  */
  /*!
  \var COMON_ERRORLABEL C_FILE_OPEN_ERR_s
    For message : "file access: cannot open file \b filename".
  */
  /*!
  \var COMON_ERRORLABEL C_FILE_READ_ERR_s
    For message : "file access: cannot read file \b filename ".
  */
  /*!
  \var COMON_ERRORLABEL C_FILE_WRITE_ERR_s
    For message : "file access: cannot write file \b filename ".
  */
enum  COMON_ERRORLABEL {
  C_NYI_ERR = 0,
  C_VAR_BAD_VALUE_sss,
  C_FILE_ERR_s,
  C_FILE_OPEN_ERR_s,
  C_FILE_READ_ERR_s,
  C_FILE_WRITE_ERR_s,

};

/* ----------------------------------------------------------------------- */

/// preparation of genError() arguments
#define _PGL_ERRORARGS_(i,t)   t,__FILE__,__LINE__,\
                          (((i)<COMMON_ERR_OFFSET)?\
                            common_err_msgs_aml[i]:\
                            PGL_ERR_MSG_ARRAY[(i-COMMON_ERR_OFFSET)<0?0:i-COMMON_ERR_OFFSET])
/** \brief Error message

 To generate an error/warning message with parameters:
 pglErrorEx(PGLERRORMSG(i),p1,p2,p3)
*/
#define PGLERRORMSG(i)       _PGL_ERRORARGS_(i,PGL_ERROR_MESSAGE)

#ifndef REMOVE_WARNINGS
/** \brief Warning message

  Generates a warning message (without parameters)
  the second parameter i must be an enum <MODULE>_ERRORLABEL
*/
#define PGLWARNINGMSG(i)     _PGL_ERRORARGS_(i,PGL_WARNING_MESSAGE)
#else
/** \brief Warning message

  Generates a warning message (without parameters)
  the second parameter i must be an enum <MODULE>_ERRORLABEL
*/
#define PGLWARNINGMSG(i)     _PGL_ERRORARGS_(i,PGL_NO_MESSAGE)
#endif

#ifndef REMOVE_DEBUG
/** \brief Debug message

  Generates a warning message (without parameters)
  the second parameter i must be an enum <MODULE>_ERRORLABEL
*/
#define PGLDEBUGMSG(i)     _PGL_ERRORARGS_(i,PGL_DEBUG_MESSAGE)
#else
/** \brief Debug message

  Generates a warning message (without parameters)
  the second parameter i must be an enum <MODULE>_ERRORLABEL
*/
#define PGLDEBUGMSG(i)     _PGL_ERRORARGS_(i,PGL_NO_MESSAGE)
#endif

/** this function creates an ErrorObj with the desired error message

  it takes a variable number of arguments. \n
   It should not be called with all its arguments explicitly. \n
   Rather call it as follows : \n
  \n
   \b pglErrorEx(PGLERRORMSG(i));                  or \n
   \b pglErrorEx(PGLWARNINGMSG(i));                or \n
   \b pglErrorEx(PGLWARNINGMSG(i), \b p1, \b p2, \b p3); \n
*/

/* ----------------------------------------------------------------------- */

#endif
