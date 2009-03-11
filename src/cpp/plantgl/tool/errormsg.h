/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): Ch. Godin (christophe.godin@cirad.fr)
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

/*! \file errormsg.h
    \brief Procedure to generate error messages using parameters:
*/

#ifndef ERROR_MESSAGES_HEADER
#define ERROR_MESSAGES_HEADER

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
#define _ERRORARGS_(i,t)   t,__FILE__,__LINE__,\
                          (((i)<COMMON_ERR_OFFSET)?\
                            common_err_msgs_aml[i]:\
                            ERR_MSG_ARRAY[(i-COMMON_ERR_OFFSET)<0?0:i-COMMON_ERR_OFFSET])
/** \brief Error message

 To generate an error/warning message with parameters:
 pglErrorEx(ERRORMSG(i),p1,p2,p3)
*/
#define ERRORMSG(i)       _ERRORARGS_(i,PGL_ERROR_MESSAGE)

#ifndef REMOVE_WARNINGS
/** \brief Warning message

  Generates a warning message (without parameters)
  the second parameter i must be an enum <MODULE>_ERRORLABEL
*/
#define WARNINGMSG(i)     _ERRORARGS_(i,PGL_WARNING_MESSAGE)
#else
/** \brief Warning message

  Generates a warning message (without parameters)
  the second parameter i must be an enum <MODULE>_ERRORLABEL
*/
#define WARNINGMSG(i)     _ERRORARGS_(i,PGL_NO_MESSAGE)
#endif

#ifndef REMOVE_DEBUG
/** \brief Debug message

  Generates a warning message (without parameters)
  the second parameter i must be an enum <MODULE>_ERRORLABEL
*/
#define DEBUGMSG(i)     _ERRORARGS_(i,PGL_DEBUG_MESSAGE)
#else
/** \brief Debug message

  Generates a warning message (without parameters)
  the second parameter i must be an enum <MODULE>_ERRORLABEL
*/
#define DEBUGMSG(i)     _ERRORARGS_(i,PGL_NO_MESSAGE)
#endif

/** this function creates an ErrorObj with the desired error message

  it takes a variable number of arguments. \n
   It should not be called with all its arguments explicitly. \n
   Rather call it as follows : \n
  \n
   \b pglErrorEx(ERRORMSG(i));                  or \n
   \b pglErrorEx(WARNINGMSG(i));                or \n
   \b pglErrorEx(WARNINGMSG(i), \b p1, \b p2, \b p3); \n
*/

/* ----------------------------------------------------------------------- */

#endif
