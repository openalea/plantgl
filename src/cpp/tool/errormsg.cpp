/* -*-c++-*- 
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): Ch. Godin (christophe.godin@cirad.fr) 
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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




#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#ifdef __GNUC__
#include <stream.h>   //for the use of function extern char* form(const char* ...); (printf format)
#endif
#include <assert.h>

#include "errormsg.h"


void genMessage(const char* file, 
		int line, 
		const char* module_name, 
		ErrorType errtype, 
		std::ostream& o, 
		bool b,		
		const char* msg ...) {

  va_list args;			// man vprintf
  va_start(args, msg);

  if (errtype!=NO_MESSAGE) {

    char fullmsg[1000];
    vsprintf(fullmsg,msg,args);

    ErrorObj err_item(module_name, file, line, errtype, fullmsg);
  
    err_item.print(o,b) << std::endl;

  }

  va_end(args);

}

// ------------------
// Class ErrorObj
// ------------------

ErrorObj::ErrorObj() {

  _line_nb = -1;
  _error_type = ERROR_MESSAGE;

}

ErrorObj::ErrorObj(const char* module,
		     const char* file,
		     int line_nb,
		     ErrorType type,
		     const char* message
		     )
{


  if (module) _module = module;
  if (file) _file = file;
  if (message) _message = message;
  _error_type = type;

  _line_nb = line_nb;

}

ErrorObj::ErrorObj(const ErrorObj& err) {

  _module = err._module;
  _file = err._file;
  _message = err._message; 
  _line_nb = err._line_nb;
  _error_type = err._error_type;
}

ErrorObj& ErrorObj::operator=(const ErrorObj& err) {

  if (this == &err) return *this;
  else {

    _module = err._module;
    _file = err._file;
    _message = err._message; 
    _line_nb = err._line_nb;
    _error_type = err._error_type;
    
  }
  return *this;
}

bool ErrorObj::operator==(const ErrorObj& err) {

  if (this == &err) return true;
  else {
    if (_module == err._module  &&
	_file == err._file &&
	_line_nb == err._line_nb &&		
	_message == err._message && 
	_error_type == err._error_type
	)
    return true;
    else return false;
  }

}


std::ostream& operator<<(std::ostream& o, const ErrorObj& i) {return i.print(o);}


/*

// ------------------
// Class ParserErrorObj
// ------------------

ErrorObj::ErrorObj() {

  _file = "";
  _line = -1;
  _token = "";

  _message = ""; 
  _module = "";

  _line_nb = 0;
  _column_nb = 0;
  _input_line = "";
  _err_pos = 0;

}


ErrorObj::ErrorObj(const char* module,
		     const char* file,
		     const char* line,
		     const char* message,
		     const char* token,
		     int srcline,
		     int line_nb,
		     int column_nb,
		     int err_pos
		     )
{


  if (file) _file = file;
  _line = line;
  _token = token;

  _message = message; 
  _module = module;

  _line_nb = line_nb;
  _column_nb = column_nb;
  _input_line = input_line;
  _err_pos = err_pos;

}

ErrorObj::ErrorObj(const ErrorObj& err) {

  _file = err._file;
  _line = err._line;
  _token = err._token;

  _message = err._message; 
  _module = err._module;

  _line_nb = err._line_nb;
  _column_nb = err._column_nb;
  _input_line = err._input_line;
  _err_pos = err._err_pos;
  
}

ErrorObj& ErrorObj::operator=(const ErrorObj& err) {

  if (this == &err) return *this;
  else {

    _file = err._file;
    _line = err._line;
    _token = err._token;

    _message = err._message; // RWCString& operator=(const char*) 
    _module = err._module;

    _line_nb = err._line_nb;
    _column_nb = err._column_nb;
    _input_line = err._input_line;
    _err_pos = err._err_pos;
    
  }
  return *this;
}

bool ErrorObj::operator==(const ErrorObj& err) {

  if (this == &err) return true;
  else {
    if (_message == err._message && 
	_line_nb == err._line_nb &&
	_column_nb == err._column_nb &&
	_input_line == err._input_line)

    return true;
    else return false;
  }

}


ostream& ErrorObj::print(ostream& o) const {

  string locs;

  if (_err_pos >= 0) for(register int i=0; i<_line.length()-1; i++) locs += "-";

  locs += "^";

  o << endl;
  o << "*** ";

  if (_error_type == ERROR_TYPE) o << "ERROR : in ";
  else if (_error_type == WARNING_TYPE) o << "WARNING : in ";
  else o << "UNKNOWN ERROR TYPE : in ";
  o << _module << " module";
#ifdef DEBUGERRORLINE
  if (_file) {
    o << " ... detected in source FILE: " << _file;
    if (_line) o << ", LINE:  " << _line;
    if (_token) o << ", on TOKEN : " << _token << "'"; 
  }
#endif
  o << endl;
  if (_line) {
    o << "*** line  : " << _input_line;
    if (_err_pos) o << "*** at    : " << locs.data() << endl;
  }
  o << "***    ---> " << _message << endl;

return o;

}


ostream& operator<<(ostream& o, const ErrorObj& i) {return i.print(o);}


*/


const char* common_err_msgs_aml[] = {

  "Sorry: message not yet implemented",	        // C_NYI_ERR
  "variable %s: bad value %s: should be %s" ,   // C_VAR_BAD_VALUE_sss
  "file access: cannot access file %s",		// C_FILE_ERR_s
  "file access: cannot open file %s",		// C_FILE_OPEN_ERR_s
  "file access: cannot read file %s",		// C_FILE_READ_ERR_s
  "file access: cannot write file %s",		// C_FILE_WRITE_ERR_s
};

