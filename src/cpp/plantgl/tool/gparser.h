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




/*! \file gparser.h
    \brief File for generic parser.
*/


#ifndef __GPARSER_HEADER
#define __GPARSER_HEADER

#include <iostream>
#include <utility>

// File FlexLexer.h is automatically included by flex is the .C file
// generated and cannot be added in "gscanner.h" otherwise class
// yyFlexLexer is redefined -> bug of flex++. Therefore, the include
// has been removed from gscanner.h to compile correctly the file
// generated by flex++ and, consequently, it must be added here
#include <FlexLexer.h>

#include "gscanner.h"
#include <assert.h>

/// function generated by bison and implementing the syntax
extern int yyparse(void*);

const int MAX_SYNTAX_ERRORS = 10;

/* The role of a parser is to scan some input stream and to build up
a semantic interpretation of it. Here the semantic interpretation
consists of a symbol table, i.e. a table associating a name and a
value (symbol).

 */

#include "gsmbtable.h"

/**
   \class GenericParser
   \brief Class used for parsing a stream.

   A parser is built by giving at least two arguments: a syntax and a
   lexer.

   - The syntax is simply the function that analyses the input
   stream and that returns 0 if the parsing completed ok. This function
   is most often generated by yacc/bison (see yyparse).

   - The lexer must be built before calling the parser. The lexer
   determines the istream and the ostream considered by the parser.
*/
template <class T>
class GenericParser {

protected:

  /// type of syntax functions generated by bison/yacc yyparse().
  typedef int (*Syntax)(void*);

  friend int yyparse(void*);

  /** if the symbol table is allocated by the parser, it should be
      deallocated by the destructor. */
  bool _symbol_table_allocated_here;

  /// parser output stream
  std::ostream* _po;

  /// in order to access to the symboltable: _pst
  SymbolTable<T>* _pst;

  /// the syntax is defined as a function
  Syntax _syntax;

  /** the lexer is an object which was defined at the construction of the
      parser */
  GENERIC_LEXER* _plexer;

  /// count the number of errors encountered up to the current parsing point.
  int _current_error_nb;

  /// defines a maximum number of errors before method parse() returns
  int _max_errors;

public:

  ///@name Constructors
  //@{

  /** The constructor takes at least one argument consisting of the syntax function.
      @param syntax is mandatory.
      @param pst is a pointer to a symbol table. By default, it is NULL. In this
      case the parser allocates a symbol table internally and deletes it when
      the parser is deleted.
      @param max_errors defines the maximum number of errors that are allowed before
      method parse() returns. By default the number of errors is not bounded.
  */

   GenericParser(Syntax syntax,             // function like yyparse()
                 SymbolTable<T>* pst=NULL,  // if Null the symbol table is created by the parser
                 int max_errors=-1          // nb of errors not bounded
                 ): _po(&std::cout),
       _syntax(syntax),
       _max_errors(max_errors){

    if (!pst) {
      _pst = new SymbolTable<T>;
      _symbol_table_allocated_here = true;
    }
    else {
      _pst = pst;
      _symbol_table_allocated_here = false;
    }

    _max_errors = max_errors;
    _current_error_nb = 0;
    _plexer = NULL;

  };

  /// the destructor deletes the symbol table only if it was created internally.
  ~GenericParser() {

    if (_symbol_table_allocated_here) {
      delete _pst;
    }

  }

  //@}

  /// Information may be passed on to the syntax function which may be used during parsing
  void* _data_for_parsing;

  /** This function is called to parse an input stream which is embedded within
      the lexer. It returns true is the parsing completed ok.
  */
  bool parse(GENERIC_LEXER* plexer, std::ostream& output_stream = std::cout, void* data = NULL);

  /// current token in the current stream
  const char* currentTokenString() {return _plexer->YYText();}

  /// the symbol table is a hash map associating strings and values of type T.
  SymbolTable<T>* smbTable() {return _pst;}

  /// value corresponding to the current token read by the lexer.
  void* lexVal() const {return _plexer->val;}

  /// return the output stream.
  std::ostream* outputStream() const {return _po;}

  /** This function generates an error message.
      It is usually called on an error detected by the parser by the macro yyerror().
  */
  bool handleError(std::string msg, int yychar, const char*);

};

typedef std::pair<void*, GENERIC_LEXER*> ParserPair;

template <class T>
bool GenericParser<T>::parse(GENERIC_LEXER* plexer,
                             std::ostream& output_stream,
                             void* data) {

  _po = &output_stream;
  _data_for_parsing = data;

  _plexer = plexer; // The lexer is always built outside the parser and should be deleted outside !

  if (_plexer) {

    ParserPair pp(this, plexer);

    // the pair parser/lexer is passed as a parameter to yyparse(void*).
    // function yyparse returns 0 if parsing completed sucessfully 1 otherwise.
    return !(*_syntax)(&pp);
  }
  else {
    *_po << "No lexer was defined ..." << std::endl;
    return false;
  }

}

template <class T>
bool GenericParser<T>::handleError(std::string msg, int yychar, const char* token_name)
{
  _current_error_nb += 1;

  assert(_plexer);

  *_po << "*** PARSER: " << msg;
    if (_plexer->currentFile()) {
      *_po << " in file "
           << _plexer->currentFile()
           << ", line "
           << _plexer->lineno()
           <<", column "
           << _plexer->columno();
    }
    else {
      *_po << " at top level ";
    }

    *_po << ", near `"
         << _plexer->YYText()
         << "' (current token is "
         << token_name
         << " -- "
         << yychar
         << ")"
         << std::endl;

  if ( _max_errors>= 0)
  {
    if (_current_error_nb >= _max_errors)
    {
      *_po << std::endl << "-- Too many errors, abort --" << std::endl;
      return false;
    }
  }

  return true;
  /*

  // On relance le parseur au top-level
  if (parser_error_occured_in_current_top_level
      > max_parser_error_before_restarting)
  {
    PARSER::parser_error_occured_in_current_top_level = 0;
    ::longjmp(start_parsing_at_top_level, 1);
  }
  */

}



/**
    If you use a reentrant parser, you can optionally pass additional
    parameter information to it in a reentrant way.  To do so, define the
    macro `YYPARSE_PARAM' as a variable name.  This modifies the `yyparse'
    function to accept one argument, of type `void *', with that name.

    When you call `yyparse', pass the address of an object, casting the
    address to `void *'.  The grammar actions can refer to the contents of
    the object by casting the pointer value back to its proper type and
    then dereferencing it.  Here's an example.  Write this in the parser:

    {
       struct parser_control foo;
       ...  // Store proper data in `foo'.
       value = yyparse ((void *) &foo);
       ...
    }

    YYPARSE_PARAM makes it possible to make references to this
    additional information from within the parser rules. In our case
    the additional information is the parser object (of type Generic
    parser) itself. Information/methods of this parser object can be
    referenced to from within the body of the parser rules as
    follows:

    ((GenericParser*)parser)->handleError()

*/

#define YYPARSE_PARAM parser

/*
   Similarly, if you wish to pass additional parameter data to `yylex', define
   the macro `YYLEX_PARAM' just like `YYPARSE_PARAM', as shown here:

   Function yyparse(void*) in turn calls yylex(void*, void*) with YYLEX_PARAM
   as a second argument


 */

#define YYLEX_PARAM   ((ParserPair*)parser)->second

  // to call the parser an its symbol table from within
  // the action part of the grammar rules, either :
  // - use directly PARSER(p, T) where T must be the template parameter
  //   of class GenericParser<T> (similarly use SMBTABLE(p, T, t)
  // - or make a #define as below.
  // these macros define variables p of type GenericParser<T> (corresponds to
  // the parser, variable t of type SymbolTbale<T> (corresponds to the
  // symbol table contained in the parser p.

#define PARSER(p, type) GenericParser<type>& p = \
                          *(GenericParser<type> *) (((ParserPair*)parser)->first)

#define SMBTABLE(p, type, t) SymbolTable<type>& t = *(p.smbTable())

#define getparser(p)      PARSER(p, SMB_TABLE_TYPE)
#define smbtable(p, t) SMBTABLE(p, SMB_TABLE_TYPE, t)
#define lexer(l)       GENERIC_LEXER& l = *(GENERIC_LEXER*)(((ParserPair*)parser)->second)

// parser output stream
#define postream(p) (*(p.outputStream()))

/* if defined, this means that we are in a multiple parser and it is
   up to the user to define its corresponding function prefix_yyerror */
#ifndef yyerror

/* yyerror must be a macro since yyerrok; yyclearin; yychar have only a meaning
   in the yyparse() function generated by bison
*/
// %token_table in the .y file generates a table yytname[] containing the names
// of the tokens.

#ifdef YYTNAME
#define yyerror(_msg) {				\
        yyerrok;\
        yyclearin;\
        parser(p);\
        if (!(p.handleError(std::string(_msg), yychar, yytname[YYTRANSLATE((yychar>=0?yychar:-yychar))]))) YYABORT;\
        postream(p) << std::endl; }
#else
#define yyerror(_msg){ 				\
        yyerrok;\
        yyclearin;\
        parser(p);\
        if (!(p.handleError(std::string(_msg), yychar, ""))) YYABORT;\
        postream(p) << std::endl; }

#endif
#endif

#endif
