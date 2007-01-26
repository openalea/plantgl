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

/*! \file errormsg.h
    \brief Procedure to generate error messages using parameters:
 *
 * It is assumed here that the error messages must be used in module (file) module.[Ch] \n
 * (in the following each instance of <module> should be replace with a fixed string). \n
 *  \n
 * - FIRST, in module.C, \n
 *  \n
 * include this file: \n
 *  \n
 * \#include "Tools/errormsg.h \n
 *  \n
 * \#define the variable \b ERR_MSG_ARRAY to \n
 * <module>_err_msgs (which should be a const char**), the array of messages \n
 * <module>_err_msgs can be actually defined in an external file (for example \n
 * in <module>_err.C. If so, <module>_err.h should contain a declaration of \n
 * <module>_err_msgs and should be included in module.C. \n
 *  \n
 * \#define the name of the module in \b MODULE_NAME to any symbolic string of \n
 * your choice. \n
 *  \n
 * \#define \b ERROR_VALUE which is a value returned by a function whenever \n
 * a check fails (after having generated an error) \n
 *  \n
 * \#define \b OUTPUT_ERR_STREAM, \b OUTPUT_WARNING_STREAM, \n
 * \b OUTPUT_COMMENT_STREAM which defines the ostream in which errors \n
 * warnings and COMMENTS will be inserted \n
 *  \n
 *  Example: \n
 *  \n
 * \b #include \b "<module>_err.h" \n
 * ... \n
 * \b #define  \b ERR_MSG_ARRAY       <module>_err_msgs \n
 * \b #define  \b MODULE_NAME         "<module_name>" \n
 * \b #define  \b ERROR_VALUE         -1                       // this could also be a function \n
 * \b #define  \b OUTPUT_ERROR_STREAM    getOutputStream()     // or cerr \n
 * \b #define  \b OUTPUT_WARNING_STREAM  std::cerr \n
 * \b #define  \b OUTPUT_COMMENT_STREAM  std::cerr \n
 *  \n
 * - SECOND: \n
 *  \n
 * the array of parametrized error strings <module>_err_msgs should \n
 * be defined (for instance in file <module>_err.C). An enum should be associated \n
 * with this array to give a symbolic reference to the messages in <module>_err_msgs. \n
 *  \n
 *  Example: \n
 *  \n
 *  in file <module>_err.h put the declarations: \n
 *  \n
 *  \b enum  \b <MODULE>_ERRORLABEL {                         // COMMON_ERR_OFFSET is a constant defined \n
 *   \b <MODULE>_NO_OBJECT_LOADED_ERR = COMMON_ERR_OFFSET, // to use an already existing base of messages \n
 *   \b <MODULE>_UNDEF_NAME_ss, \n
 *   \b <MODULE>_OPTION_BADVALUE_sd \n
 *  }; \n
 *  \n
 *  \b extern \b const \b char* \b <module>_err_msgs[]; \n
 *  \n
 *  and in file <module>_err.C put the definition: \n
 *  \n
 * \b const \b char \b *<module>_err_msgs[] = { \n
 *  "Cannot proceed: no object loaded", //<MODULE>_NO_OBJECT_LOADED_ERR \n
 *  "function %s: undefined name %s",   // <MODULE>_UNDEF_NAME_ss, \n
 *  "Option %s: bad value %d "          // <MODULE>_OPTION_BADVALUE_sd \n
 * }; \n
 *  \n
 *
 * - Third, \n
 *  \n
 * - General case: \n
 * ------------ \n
 * when an error has to be generated at a certain point in the program, call: \n
 * \n
 *   => if (FAILS(predicate)) { \n
 *        ... // any free store clean-up needed before returning \n
 *        genMessage(ERRORMSG(i), p1, p2, p3); \n
 *        return error; // some error value \n
 *      } \n
 * \n
 *   where ERRORMSG(i) is the ith message in the array \b ERR_MSG_ARRAY \n
 *   and p1, p2, p3 are % parameters needed by this message \n
 *   (stored in \b ERR_MSG_ARRAY[i] and whose format has the form of a printf() or a form(): \n
 *   cerr << form(" %s %d \\n", stg, 3);  ) \n
 * \n
 * - Simplifications: \n
 * --------------- \n
 * Whenever no parameters are required by the message string, the previous \n
 * call can be simplified as follows: \n
 * \n
 *   => if (FAILS(predicate)) { \n
 *        ... // any free store clean-up needed before returning \n
 *        GENERROR(i); \n
 *        return error; // some error value \n
 *      } \n
 * \n
 *   And even simpler, when in addition no free store clean-up is required, one can use \n
 *   the following macro which automatically returns in case of error (and does not \n
 *   return in case of warning) \n
 * \n
 *   => CHECKERROR(predicate, i); \n
 * \n
 *   which checks if predicate is true, and if not generate an error item containing the \n
 *   ith message of \b ERR_MSG_ARRAY (which must not need parameter) \n
 * \n
 *   If however the ith message contains parameters the following slightly more complex \n
 *   call has to be made: \n
 * \n
 *   => \b CHECKERROR_P(predicate, genError(ERRORMSG(i), p1, p2, p3)); \n
 * \n
 *   Checks can be combined in an if ... else if ... else  manner as follows: \n
 * \n
 *   CHECKERROR(test1, MSG1) \n
 *   ELSE_CHECKWARNING(test2, MSG2) \n
 *   ELSE_CHECKERROR(test3, MSG3) \n
 *   ELSE_COMMENT(MSG4); \n
 * \n
 *   IMPORTANT: Note there is only ONE ';' at the end of the sequence. \n
 * \n
 *   A few options can be controled at compile time if \n
 *   \b REMOVE_COMMENTS, \b REMOVE_WARNINGS, \b PRINT_FILE_LINE_IN_MESSAGES \n
 *   are defined before including "errormsg.h". This can be done as \n
 *   follows: \n
 * \n
 *   \b #if \b 0 \n
 *   \b #define \b REMOVE_COMMENTS \n
 *   \b #endif \n
 * \n
 *   \b #if \b 0 \n
 *   \b #define \b REMOVE_WARNINGS \n
 *   \b #endif \n
 * \n
 *   \b #if \b 1 \n
 *   \b #define \b PRINT_FILE_LINE_IN_MESSAGES \n
 *   \b #endif \n
 * \n
 *   \b #include \b "errormsg.h" \n
 *   ... \n
 */


#ifndef ERROR_MESSAGES_HEADER
#define ERROR_MESSAGES_HEADER

#include <iostream>
#include <string>
#include "tools_config.h"

/// Test of a condition
#define FAILS(x)          !(x)

#ifndef REMOVE_COMMENTS
/// Generate comment message
#define COMMENT(s)        OUTPUT_COMMENT_STREAM << s << std::endl
#else
/// Generate comment message
#define COMMENT(s)
#endif

#ifndef PRINT_LINE_FILE_IN_MESSAGE
/// Print the line number in message
#define PRINT_LINE   false
#else
/// Print the line number in message
#define PRINT_LINE   true
#endif


/// preparation of genError() arguments
#define _ERRORARGS_(i,t,o)   __FILE__,__LINE__,MODULE_NAME,t,o,PRINT_LINE,\
                          (((i)<COMMON_ERR_OFFSET)?\
                            common_err_msgs_aml[i]:\
                            ERR_MSG_ARRAY[(i-COMMON_ERR_OFFSET)<0?0:i-COMMON_ERR_OFFSET])
/** \brief Error message

 To generate an error/warning message with parameters:
 genMessage(ERRORMSG(i),p1,p2,p3)
*/
#define ERRORMSG(i)       _ERRORARGS_(i,ERROR_MESSAGE,OUTPUT_ERROR_STREAM)

#ifndef REMOVE_WARNINGS
/** \brief Warning message

  Generates a warning message (without parameters)
  the second parameter i must be an enum <MODULE>_ERRORLABEL
*/
#define WARNINGMSG(i)     _ERRORARGS_(i,WARNING_MESSAGE,OUTPUT_WARNING_STREAM)
#else
/** \brief Warning message

  Generates a warning message (without parameters)
  the second parameter i must be an enum <MODULE>_ERRORLABEL
*/
#define WARNINGMSG(i)     _ERRORARGS_(i,NO_MESSAGE,OUTPUT_WARNING_STREAM)
#endif



/// Generate Error message
#define GENERROR(i)    genMessage(ERRORMSG(i))
#ifndef REMOVE_WARNINGS
/// Generate Warning message
#define GENWARNING(i)  genMessage(WARNINGMSG(i))
#else
/// Generate Warning message
#define GENWARNING(i)
#endif

/** \brief Checks a condition and generates an error message (without parameters)

 the second parameter i must be an enum <MODULE>_ERRORLABEL
*/
#define CHECKERROR(x,i)      if (FAILS(x)) {GENERROR(i); return ERROR_VALUE;}

#ifndef REMOVE_WARNINGS
/** \brief Checks a condition and generates a warning message (without parameters)

  the second parameter i must be an enum <MODULE>_ERRORLABEL
*/
#define CHECKWARNING(x,i)    if (FAILS(x)) {GENWARNING(i);}
#else
/** \brief Checks a condition and generates a warning message (without parameters)

 the second parameter i must be an enum <MODULE>_ERRORLABEL
*/
#define CHECKWARNING(x,i)
#endif

#define TEST_FAILED(x)  "Test " #x " failed. "

#ifndef REMOVE_COMMENTS
/** \brief Checks a condition and generates a comment message

 the second parameter i must be a string
*/
#define CHECKCOMMENT(x,i)    if (FAILS(x)) {COMMENT(TEST_FAILED(x)<<i);}
#else
/** \brief Checks a condition and generates a comment message

   the second parameter i must be a string
*/
#define CHECKCOMMENT(x,i)
#endif

/** \brief Checks a condition and generates an error message with parameters */
#define CHECKERROR_P(x,i)    if (FAILS(x)) {i; return ERROR_VALUE;} /* gen error msg with parameters */

#ifndef REMOVE_WARNINGS
/** \brief Checks a condition and generates a warning message with parameters */
#define CHECKWARNING_P(x,i)  if (FAILS(x)) {i;}                     /* gen warning msg with parameters */
#else
/** \brief Checks a condition and generates a warning message with parameters */
#define CHECKWARNING_P(x,i)
#endif

/** \brief Generate Error message

   Similar macros than \b GENERROR(i) but on an else \n
   else macros should be used as follows: \n
   \n
   CHECKERROR(test1, MSG1) \n
   ELSE_CHECKWARNING(test2, MSG2) \n
   ELSE_CHECKERROR(test3, MSG3) \n
   ELSE_COMMENT(MSG4); \n
*/
#define ELSE_GENERROR(i)          else genMessage(ERRORMSG(i))

/** \brief Checks a condition and generates an error message (without parameters)

   Similar macros than \b CHECKERROR(x,i) but on an else \n
   else macros should be used as follows: \n
   \n
   CHECKERROR(test1, MSG1) \n
   ELSE_CHECKWARNING(test2, MSG2) \n
   ELSE_CHECKERROR(test3, MSG3) \n
   ELSE_COMMENT(MSG4); \n
*/
#define ELSE_CHECKERROR(x,i)      else if (FAILS(x)) {GENERROR(i); return ERROR_VALUE;}

/** \brief Checks a condition and generates an error message with parameters.

   Similar macros than \b CHECKERROR_P(x,i) but on an else \n
   else macros should be used as follows: \n
   \n
   CHECKERROR(test1, MSG1) \n
   ELSE_CHECKWARNING(test2, MSG2) \n
   ELSE_CHECKERROR(test3, MSG3) \n
   ELSE_COMMENT(MSG4); \n
*/
#define ELSE_CHECKERROR_P(x,i)    else if (FAILS(x)) {i; return ERROR_VALUE;} /* gen error msg with parameters */

#ifndef REMOVE_WARNINGS

/** \brief Generate Warning message

   Similar macros than \b GENWARNING(i) but on an else \n
   else macros should be used as follows: \n
   \n
   CHECKERROR(test1, MSG1) \n
   ELSE_CHECKWARNING(test2, MSG2) \n
   ELSE_CHECKERROR(test3, MSG3) \n
   ELSE_COMMENT(MSG4); \n
*/
#define ELSE_GENWARNING(i)        else genMessage(WARNINGMSG(i))

/** \brief Checks a condition and generates an warning message (without parameters)

   Similar macros than \b CHECKWARNING(x,i) but on an else \n
   else macros should be used as follows: \n
   \n
   CHECKERROR(test1, MSG1) \n
   ELSE_CHECKWARNING(test2, MSG2) \n
   ELSE_CHECKERROR(test3, MSG3) \n
   ELSE_COMMENT(MSG4); \n
*/
#define ELSE_CHECKWARNING(x,i)    else if (FAILS(x)) {GENWARNING(i);}

/** \brief Checks a condition and generates an warning message with parameters

   Similar macros than \b CHECKWARNING_P(x,i) but on an else \n
   else macros should be used as follows: \n
   \n
   CHECKERROR(test1, MSG1) \n
   ELSE_CHECKWARNING(test2, MSG2) \n
   ELSE_CHECKERROR(test3, MSG3) \n
   ELSE_COMMENT(MSG4); \n
*/
#define ELSE_CHECKWARNING_P(x,i)  else if (FAILS(x)) {i;}                     /* gen warning msg with parameters */
#else

/** \brief Generate Warning message

   Similar macros than \b GENWARNING(i) but on an else \n
   else macros should be used as follows: \n
   \n
   CHECKERROR(test1, MSG1) \n
   ELSE_CHECKWARNING(test2, MSG2) \n
   ELSE_CHECKERROR(test3, MSG3) \n
   ELSE_COMMENT(MSG4); \n
*/
#define ELSE_GENWARNING(i)        else if (false) {;}

/** \brief Checks a condition and generates an warning message (without parameters)

   Similar macros than \b CHECKWARNING(x,i) but on an else \n
   else macros should be used as follows: \n
   \n
   CHECKERROR(test1, MSG1) \n
   ELSE_CHECKWARNING(test2, MSG2) \n
   ELSE_CHECKERROR(test3, MSG3) \n
   ELSE_COMMENT(MSG4); \n
*/
#define ELSE_CHECKWARNING(x,i)    else if (false) {;}

/** \brief Checks a condition and generates an warning message with parameters

   Similar macros than \b CHECKWARNING_P(x,i) but on an else \n
   else macros should be used as follows: \n
   \n
   CHECKERROR(test1, MSG1) \n
   ELSE_CHECKWARNING(test2, MSG2) \n
   ELSE_CHECKERROR(test3, MSG3) \n
   ELSE_COMMENT(MSG4); \n
*/
#define ELSE_CHECKWARNING_P(x,i)  else if (false) {;}
#endif

#ifndef REMOVE_COMMENTS

/** \brief Generate Comment message

   Similar macros than \b COMMENT(s) but on an else \n
   else macros should be used as follows: \n
   \n
   CHECKERROR(test1, MSG1) \n
   ELSE_CHECKWARNING(test2, MSG2) \n
   ELSE_CHECKERROR(test3, MSG3) \n
   ELSE_COMMENT(MSG4); \n
*/
#define ELSE_COMMENT(s)           else {COMMENT(s);}

/** \brief Checks a condition and generates a comment message

   Similar macros than \b CHECKCOMMENT(x,i) but on an else \n
   else macros should be used as follows: \n
   \n
   CHECKERROR(test1, MSG1) \n
   ELSE_CHECKWARNING(test2, MSG2) \n
   ELSE_CHECKERROR(test3, MSG3) \n
   ELSE_COMMENT(MSG4); \n
*/
#define ELSE_CHECKCOMMENT(x,i)    else if (FAILS(x)) {COMMENT(TEST_FAILED(x) << i);}
#else

/** \brief Generate Comment message

   Similar macros than \b COMMENT(s) but on an else \n
   else macros should be used as follows: \n
   \n
   CHECKERROR(test1, MSG1) \n
   ELSE_CHECKWARNING(test2, MSG2) \n
   ELSE_CHECKERROR(test3, MSG3) \n
   ELSE_COMMENT(MSG4); \n
*/
#define ELSE_COMMENT(s)           else {;}

/** \brief Checks a condition and generates a comment message

   Similar macros than \b CHECKCOMMENT(x,i) but on an else \n
   else macros should be used as follows: \n
   \n
   CHECKERROR(test1, MSG1) \n
   ELSE_CHECKWARNING(test2, MSG2) \n
   ELSE_CHECKERROR(test3, MSG3) \n
   ELSE_COMMENT(MSG4); \n
*/
#define ELSE_CHECKCOMMENT(x,i)    else {;}
#endif


  /*! \enum ErrorType
  Error Message Type.
  */
  /*!
  \var ErrorType NO_MESSAGE
  Error without message.
  */
  /*!
  \var ErrorType ERROR_MESSAGE
   Error with message.
  */
  /*!
  \var ErrorType WARNING_MESSAGE
    Warning message.
  */
enum ErrorType {NO_MESSAGE, ERROR_MESSAGE, WARNING_MESSAGE};

/// General messages that can be used by different modules
extern const char* common_err_messages[];


/// Obj that contains the error parameters
class TOOLS_API ErrorObj {

        /// print an error message.
        friend TOOLS_API std::ostream& operator<<(std::ostream&, const ErrorObj&);

protected:

  // source file in which the error were detected

                /// name of the module in which the error occurs
        std::string _module;
                /// name of the file where the error is detected
        std::string _file;
        /// line number of the file where the error is detected
        int _line_nb;

                /// enum defining whether the ErrorObj contains an ERROR or a WARNING
        ErrorType _error_type;
                /// error message
        std::string _message;

public:

  /// Constructor
  ErrorObj();
  /// Constructor
  ErrorObj(const char* module,
            const char* file,
            int line_nb,
            ErrorType type,
            const char* message = "Error message not yet implemented"
            );

  /// Copy Constructor
  ErrorObj(const ErrorObj&);
  /// Destructor
  virtual ~ErrorObj(){
  };
  /// Assignement
  virtual ErrorObj& operator=(const ErrorObj&);
  /// Comparaison
  virtual bool operator==(const ErrorObj&);
  /// print on ostream
  virtual std::ostream& print(std::ostream& o, bool file_flag = true) const {

    o << "*** ";

    if (_error_type == ERROR_MESSAGE) o << "ERROR : in ";
    else if (_error_type == WARNING_MESSAGE) o << "WARNING : in ";
    else o << "UNKNOWN ERROR TYPE : in ";
    o << _module;
    o << " module";
    if (file_flag)
      if (_file.length()) {
      o << " ... detected in source FILE: " << _file;
      if (_line_nb >=0) o << ", LINE:  " << _line_nb;
      }
          o << std::endl;
          o << "***   --->  " << _message << std::endl;

    return o;

  }

};

/// print an error message.
std::ostream& operator<<(std::ostream& o, const ErrorObj& i);


/** this function creates an ErrorObj with the desired error message

  it takes a variable number of arguments. \n
   It should not be called with all its arguments explicitly. \n
   Rather call it as follows : \n
  \n
   \b genMessage(ERRORMSG(i));                  or \n
   \b genMessage(WARNINGMSG(i));                or \n
   \b genMessage(WARNINGMSG(i), \b p1, \b p2, \b p3); \n
*/

extern void TOOLS_API genMessage(const char* file,
                       int line,
                       const char* module_name,
                       ErrorType type,
                       std::ostream& o,
                       bool b,  // if true prints the line and the file in the message
                       const char* msg ...);




/*
class ParserErrorObj : public ErrorObj {

protected:

  string _parsed_file;          // name of the parsed file where the error is detected
  string _parsed_line;          // line number of the token which caused the error
  string _token;                // current token which generated the error
  int _parsed_line_nb;          // column number of the token which caused the error
  int _parsed_column_nb;        // column number of the token which caused the error
  int _parsed_pos;              // index of the first caracter of the token in the line

public:

  ParserErrorObj();
  ParserErrorObj(const char* module,
                  const char* file,
                  const char* line,
                  ErrorType type,
                  const char* message = "Error message not yet implemented",
                  int srcline = UNDEF,
                  int line_nb = UNDEF,
                  const char* token = NULL,
                  int column_nb = UNDEF,
                  int err_pos = UNDEF
                  );

  virtual ErrorObj& operator=(const ErrorObj&);
  virtual bool operator==(const ErrorObj&);

  virtual std::ostream& print(std::ostream&) const;
};

typedef slist<ErrorObj*> ErrorObjList;

static const int MAXERROR = 3;

class ErrorList : public ErrorObjList {

  friend std::ostream& operator<<(std::ostream&, const ErrorList&);

  int _MAXERRORS;

public:

  ErrorList(int maxerrors = MAXERROR);
  ~ErrorList() {}

  int& maxErrorNb() {return _MAXERRORS;}

  const ErrorObj* getLastErrorObj() const {
    if (entries()) return &(ErrorObj&)at(entries()-1);
    else return 0;
  }

  void insert(const ErrorObj& item) {if (entries() < _MAXERRORS) ErrorObjList::insert(item);}

  std::ostream& print(std::ostream& o) const;
  void printLastErrorObj(std::ostream& o) const;

};

std::ostream& operator<<(std::ostream& o, const ErrorList& l);

*/

// ******************************************************
// General messages that can be used by different modules
// ******************************************************

/// Max number of error messages in the Kernel
#define  COMMON_ERR_OFFSET      1000

/// General Aml messages that can be used by different modules
extern const char* common_err_msgs_aml[];

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

#endif
