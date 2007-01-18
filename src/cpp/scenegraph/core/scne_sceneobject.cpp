/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
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

#include "Tools/dirnames.h"
#include "Tools/timer.h"

#include "scne_sceneobject.h"
#include "scne_smbtable.h"

#include "util_messages.h"

#include "all_scene.h"
#include "all_appearance.h"
#include "all_geometry.h"

#include "geom_pointmatrix.h"
#include <list>
#include <fstream>

#include "scne_parser.h"    /// fonction pour parser les sceneobjects.

GEOM_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/// structure de donnees pour parser les sceneobjects.
#ifdef BISON_HPP
#include "scne_parser.hpp"
#else
#include "scne_parser.cpp.h"
#endif

#include "scne_scanner.h"   /// Initialisation des noms des classes de scanner
                            /// [ par redefinition de yyFlexLexer et GENERIC_LEXER ]

#include "Tools/gparser.h"  /// Instanciation des classes de scans.
                               /// et instanciation de la classe template de parser.

#include "scne_binaryparser.h"
#include "Tools/readline.h"

using namespace std;

/* ----------------------------------------------------------------------- */

ostream * SceneObject::commentStream =  & cerr ;
ostream * SceneObject::warningStream = & cerr ;
ostream * SceneObject::errorStream = & cerr ;

/* ----------------------------------------------------------------------- */


SceneObject::Builder::Builder() {
  // nothing to do
}


SceneObject::Builder::~Builder() {
  // nothing to do
}


/* ----------------------------------------------------------------------- */

/*
SceneObject::SceneObject( ) :
  RefCountObject(),
  __name() {
}

SceneObject::SceneObject(const string& name ) :
  RefCountObject(),
  __name(name) {
}
*/

SceneObject::~SceneObject( ){
#ifdef GEOM_DEBUG
    if(!__name.empty())cerr << __name << " destroyed" << endl;
#endif
}

void SceneObject::setName( const string& name ) {
  __name = name;
}

uint32_t SceneObject::getId( ) const {
  return (uint32_t)this;
}

const string&
SceneObject::getName( ) const {
  return __name;
}

bool
SceneObject::isNamed( ) const {
  return ! __name.empty();
}

/* ------------------------------------------------------------------------- */



bool SceneObject::parse( istream& input,
                      ostream& output,
                      SceneObjectSymbolTable& table,
                      int max_errors ) {
  string p = get_cwd();
  ostream * errlog = errorStream;
  ostream * warlog = warningStream;
  ostream * infolog = commentStream;
  commentStream = &output;
  warningStream = &output;
  errorStream = &output;
  Timer t;
  GenericParser<SceneObjectPtr> _parser(scne_yyparse,&table,max_errors);
  SceneObjectRecursiveLexer _sceneLexer(&input,&output);
  t.start();
  bool b =_parser.parse(&_sceneLexer,output);
  t.stop();
  if(isParserVerbose())(*SceneObject::commentStream) << "Parse in " << t.elapsedTime() << " sec. " << endl;
  commentStream = infolog;
  warningStream = warlog;
  errorStream = errlog;
  chg_dir(p);
  return b;
}

bool SceneObject::parse( SceneObjectSymbolTable& table,
                         int max_errors ) {

#ifdef __GNUC__
  gnu_init_readline();
#endif
  string p = get_cwd();
  ostream * errlog = errorStream;
  ostream * warlog = warningStream;
  ostream * infolog = commentStream;
  commentStream = &cerr;
  warningStream = &cerr;
  errorStream = &cerr;
  Timer t;
  GenericParser<SceneObjectPtr> _parser(scne_yyparse,&table,max_errors);
  SceneObjectRecursiveLexer _sceneLexer(NULL,NULL,NULL,"GML>");
  t.start();
  bool b =_parser.parse(&_sceneLexer,cerr);
  t.stop();
  if(isParserVerbose())(*SceneObject::commentStream) << "Parse in " << t.elapsedTime() << " sec. " << endl;
  commentStream = infolog;
  warningStream = warlog;
  errorStream = errlog;
  chg_dir(p);
  return b;
}


bool SceneObject::parse( const string& filename,
                      ostream& output,
                      SceneObjectSymbolTable& table,
                      int max_errors ) {
  string p = get_cwd();

  ostream * errlog = errorStream;
  ostream * warlog = warningStream;
  ostream * infolog = commentStream;
  commentStream = &output;
  warningStream = &output;
  errorStream = &output;

/*  if(BinaryParser::isAGeomBinaryFile(filename)){
    BinaryParser _parser(&table,output,max_errors);
    return _parser.parse(filename);

  }
  else*/

  if(get_extension(filename)=="smb"){
    SceneObjectPtr a(new AmapSymbol(filename));
    table[filename] = a;
    return true;
  }
  else {
    ifstream _file(filename.c_str());
    if (! (_file)) {
      genMessage(ERRORMSG(C_FILE_OPEN_ERR_s),filename.c_str());
      commentStream = infolog;
      warningStream = warlog;
      errorStream = errlog;
      chg_dir(p);
      return false;
    };
    GenericParser<SceneObjectPtr> _parser(scne_yyparse,&table,max_errors);
    SceneObjectRecursiveLexer _sceneLexer(&_file,&output,filename.c_str());
    Timer t;
    t.start();
    bool b =_parser.parse(&_sceneLexer,output);
    t.stop();
    if(isParserVerbose())cerr << "Parse file " << filename.c_str() << " in " << t.elapsedTime() << " sec. " << endl;
    commentStream = infolog;
    warningStream = warlog;
    errorStream = errlog;
    chg_dir(p);
    return b;
  }
}



/* ----------------------------------------------------------------------- */

