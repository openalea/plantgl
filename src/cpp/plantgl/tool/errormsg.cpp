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




#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <cassert>


#include "errormsg.h"
#include "util_string.h"

#define STREAM_BASED_ERRORS

/* ----------------------------------------------------------------------- */

std::ostream * PglErrorStream::debug =  & std::cerr ;
std::ostream * PglErrorStream::warning = & std::cerr ;
std::ostream * PglErrorStream::error = & std::cerr ;

/* ----------------------------------------------------------------------- */

static error_msg_handler_func ERROR_PRINTER = NULL;
static error_msg_handler_func WARNING_PRINTER = NULL;
static error_msg_handler_func DEBUG_PRINTER = NULL;

/* ----------------------------------------------------------------------- */


PglErrorStream::Binder::Binder(std::ostream& _error, 
								 std::ostream& _warning, 
								 std::ostream& _debug):
	previousDebug(PglErrorStream::debug), 
	previoudWarning(PglErrorStream::warning),
	previousError(PglErrorStream::error),
	previousDebugPrinter(DEBUG_PRINTER),
	previousWarningPrinter(WARNING_PRINTER),
	previousErrorPrinter(ERROR_PRINTER)
{
	PglErrorStream::debug = &_debug;
	PglErrorStream::warning = &_warning;
	PglErrorStream::error = &_error;
	DEBUG_PRINTER = NULL;
	WARNING_PRINTER = NULL;
	ERROR_PRINTER = NULL;
}

  
PglErrorStream::Binder::Binder(std::ostream& stream):
	previousDebug(PglErrorStream::debug), 
	previoudWarning(PglErrorStream::warning),
	previousError(PglErrorStream::error),
	previousDebugPrinter(DEBUG_PRINTER),
	previousWarningPrinter(WARNING_PRINTER),
	previousErrorPrinter(ERROR_PRINTER)
{
	PglErrorStream::debug = &stream;
	PglErrorStream::warning = &stream;
	PglErrorStream::error = &stream;
	DEBUG_PRINTER = NULL;
	WARNING_PRINTER = NULL;
	ERROR_PRINTER = NULL;
}

PglErrorStream::Binder::~Binder()
{
	PglErrorStream::debug = previousDebug;
	PglErrorStream::warning = previoudWarning;
	PglErrorStream::error = previousError;
	DEBUG_PRINTER = previousDebugPrinter;
	WARNING_PRINTER = previousWarningPrinter;
	ERROR_PRINTER = previousErrorPrinter;
}


/* ----------------------------------------------------------------------- */

void register_error_handler(error_msg_handler_func f)
{ ERROR_PRINTER = f; }

void register_warning_handler(error_msg_handler_func f)
{ WARNING_PRINTER = f; }

void register_debug_handler(error_msg_handler_func f)
{ DEBUG_PRINTER = f; }

/* ----------------------------------------------------------------------- */

void pglErrorEx(const char* file, int line, const std::string& msg){
    if (ERROR_PRINTER != NULL) ERROR_PRINTER(msg,file,line);
	else 
#ifdef STREAM_BASED_ERRORS
		if(PglErrorStream::error) *PglErrorStream::error << "*** ERROR : " << msg << std::endl;
#else
		printf("*** ERROR : %s\n",msg.c_str());
#endif
	    
}

void pglWarningEx(const char* file, int line, const std::string& msg){
    if (WARNING_PRINTER != NULL) WARNING_PRINTER(msg,file,line);
	else
#ifdef STREAM_BASED_ERRORS
		if(PglErrorStream::warning) *PglErrorStream::warning << "*** WARNING : " << msg << std::endl;
#else
         printf("*** WARNING : %s\n",msg.c_str());
#endif
}

void pglDebugEx(const char* file, int line, const std::string& msg){
    if (DEBUG_PRINTER != NULL) DEBUG_PRINTER(msg,file,line);
	else
#ifdef STREAM_BASED_ERRORS
		if(PglErrorStream::debug) *PglErrorStream::debug << "*** DEBUG : " << msg << std::endl;
#else
		 printf("*** DEBUG : %s\n",msg.c_str());
#endif
}

void pglErrorEx(PglErrorType errtype, const char* file, int line, const std::string& msg){
  switch(errtype){
	case PGL_ERROR_MESSAGE : pglErrorEx(file,line,msg); break;
	case PGL_WARNING_MESSAGE : pglWarningEx(file,line,msg); break;
	case PGL_DEBUG_MESSAGE : pglErrorEx(file,line,msg); break;
  }
}


/* ----------------------------------------------------------------------- */

#ifdef _MSC_VER
#define pglvsprintf vsprintf_s
#else
#define pglvsprintf vsprintf
#endif

void pglDebug(const char* msg ...){
  va_list args;         // man vprintf
  va_start(args, msg);

  char fullmsg[1000];
  pglvsprintf(fullmsg,msg,args);

  pglDebugEx("",-1,std::string(fullmsg));

  va_end(args);
}

void pglWarning(const char* msg ...){
  va_list args;         // man vprintf
  va_start(args, msg);

  char fullmsg[1000];
  pglvsprintf(fullmsg,msg,args);

  pglWarningEx("",-1,std::string(fullmsg));

  va_end(args);
}

void pglError(const char* msg ...){
  va_list args;         // man vprintf
  va_start(args, msg);

  char fullmsg[1000];
  pglvsprintf(fullmsg,msg,args);

  pglErrorEx("",-1,std::string(fullmsg));

  va_end(args);
}

void pglError(PglErrorType errtype, const char* msg ...){
  va_list args;         // man vprintf
  va_start(args, msg);

  char fullmsg[1000];
  pglvsprintf(fullmsg,msg,args);

  pglErrorEx(errtype,"",-1,std::string(fullmsg));

  va_end(args);
}

/* ----------------------------------------------------------------------- */

void pglDebugEx(const char* file, int line,const char* msg ...){
  va_list args;         // man vprintf
  va_start(args, msg);

  char fullmsg[1000];
  pglvsprintf(fullmsg,msg,args);

  pglDebugEx(file,line,std::string(fullmsg));

  va_end(args);
}

void pglWarningEx(const char* file, int line,const char* msg ...){
  va_list args;         // man vprintf
  va_start(args, msg);

  char fullmsg[1000];
  pglvsprintf(fullmsg,msg,args);

  pglWarningEx(file,line,std::string(fullmsg));

  va_end(args);
}

void pglErrorEx(const char* file, int line,const char* msg ...){
  va_list args;         // man vprintf
  va_start(args, msg);

  char fullmsg[1000];
  pglvsprintf(fullmsg,msg,args);

  pglErrorEx(file,line,std::string(fullmsg));

  va_end(args);
}

void pglErrorEx(PglErrorType errtype, const char* file, int line,const char* msg ...){
  va_list args;         // man vprintf
  va_start(args, msg);

  char fullmsg[1000];
  pglvsprintf(fullmsg,msg,args);

  pglErrorEx(errtype, file,line, std::string(fullmsg));

  va_end(args);
}


/* ----------------------------------------------------------------------- */

const char* common_err_msgs_aml[] = {

  "Sorry: message not yet implemented",         // C_NYI_ERR
  "variable %s: bad value %s: should be %s" ,   // C_VAR_BAD_VALUE_sss
  "file access: cannot access file '%s'.",     // C_FILE_ERR_s
  "file access: cannot open file '%s'.",       // C_FILE_OPEN_ERR_s
  "file access: cannot read file '%s'.",       // C_FILE_READ_ERR_s
  "file access: cannot write file '%s'.",      // C_FILE_WRITE_ERR_s
};

/* ----------------------------------------------------------------------- */
