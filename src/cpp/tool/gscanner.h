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

/*! \file gscanner.h
    \brief File for generic scanner.
*/

#ifndef _GSCANNER_HEADER
#define _GSCANNER_HEADER

#ifndef GENERIC_LEXER // Default name of the class implementing the generic lexer
#define GENERIC_LEXER RecursiveLexer
#endif

#include <stack>
#include <iostream>
#include <string>

#include "util_types.h"


// the name of the class GENERIC_LEXER can be changed here whenever flex must be used
// to generate several different lexers within one application.

typedef struct yy_buffer_state* LexBufferType;

#ifndef LEXDEBUG
#define Trace(x)
#endif

#define VAL ((YYSTYPE*)val)
#define TRACE Trace(YYText())


/// Maximum depth of nested include.
static uint32_t MAX_INCLUDE_DEPTH = 10;
/// Default prompt for default shell.
static char * DEFAULT_PROMPT = "> ";
/// Readline input buffer size.
static int READLINE_INPUT_BUF_SIZE = 10000;

/*!
  \class GENERIC_LEXER
  \brief Class based on yyFlexLexer to be able to recursively analyse new stream.
  the name of the class RecursiveLexer can be changed here whenever flex must be used
  to generate several different lexers within one application.
*/
class GENERIC_LEXER : public yyFlexLexer {

protected:

  /// A Struct that contains the state of the stream.
  struct StreamState {
    std::string file;          /// < may be not used ...
    std::string cwd;           /// < may be not used ...
    int lineno;           /// < line # in the current stream
    int columno;          /// < column # in the current stream
    LexBufferType lexbuf;
    std::istream* input_stream;/// < corresponding input stream of the lexer

    StreamState(std::string f, std::string w, LexBufferType fb, int l, int c, std::istream* is)
      : file(f), cwd(w), lineno(l), columno(c), lexbuf(fb), input_stream(is) {}
  };

  /// stack of buffers corresponding to the nested include streams
  std::stack<StreamState> bstack;
  /// filename of the current file being analyzed
  std::string _current_file_name;
  /// current work directory
  std::string _cwd;
  /// output stream of the lexer
  std::ostream* _lo;
  /// current input stream of the lexer
  std::istream* _li;
  /// true is the lexer uses readline
  bool _uses_readline;
  /// sets the prompt whenever readline is used
  const char* _prompt;
  /// column # in the current stream
  int _columno;

public:

        /*! Constructor.
          @param filename name of the file to lex (NULL corresponds to cin/readline).
          @param is input stream (NULL corresponds to cin/readline).
          @param os output stream for error.
          @param prompt displayed prompt of the shell if using readline.
        */
  GENERIC_LEXER(std::istream* is = NULL,
                std::ostream* os = NULL,
                const char* filename = NULL, // NULL corresponds to cin/readline
                const char* prompt = DEFAULT_PROMPT);

        /*! Destructor
          At the top level, the stream was allocated by the user.
          it must be then deallocated by him (do not delete _li)
          */

  ~GENERIC_LEXER() ;

  /*! At any moment the current state of the lexer can analyse
    a new stream and then get back to the old one when the new
    has been completely read
    returns false if MAX_INCLUDE_DEPTH reached.
  */
  bool pushStream(std::istream& is, const char* f=NULL);

  bool pushMacroStream(std::istream& is, const char* m=NULL);

  /// restore the old stream (returns false if the lstack is empty)
  bool popStream();

  /// test if the stack is empty
  bool isEmpty() const ;

  /// test if the stack is empty
  int includeDepth() const ;

  /// return the current line number.
  int lineno() const ;
  /// return the current columns number .
  int columno() const ;
  /// return the current File name.
  const char* currentFile() ;

  /// return whether lexer use readline or not.
  bool useReadline() const ;

  /*! Function used by yyFlexLexer to fill its buffer when the scan of the previous
   buffer has been completed.

   LexerInput reads up to max_size characters into buf and returns the
   number of characters read.  To indicate end-of-input, return 0
   characters

   This function is redefined on GLexer to allow buffering via readline.

   by default max_size is 8192
  */
  int LexerInput(char* buf, int max_size ) ;

public:

  /*! buffer used to pass values (tokens) to the parser
   On genere un "pure" parser avec bison, ce qui veut dire
   que le parseur appelle la fonction lex avec un argument qui
   est l'endroit ou stocker la valeur de retour.
   Mais flex++ lui genere une methode lex qui ne prend pas d'argument.
   On fait donc l'interface necessaire.
   pointer (of actual type YYSTYPE*) to the lex/bison buffer */

  void* val;

  /// function generated by flex++ in class yyFlexLexer
  virtual int yylex();

  /// function called by the parser (void* should be YYSTYPE*)
  virtual int yylex(void* pval) ;

#ifdef LEXDEBUG

  /// Debug function : print current character to output stream.
  std::ostream& printChar(char c, std::ostream& os=std::cerr) const ;

  /// Debug function : print current token value to output stream.
  void Trace(const char* s) const ;

#endif

};

/*! function yylex.
 returns 0 if reaches end-of-file.
 The TRUE type of lvalp is YYSTYPE.
*/
inline int yylex(void* lvalp, void* _lexer) {

  int token = ((GENERIC_LEXER*)_lexer)->yylex(lvalp); // returns 0 if reaches end-of-file

  return token;

}

/// Macro for lexer output
#define lostream (*_lo)

#endif
