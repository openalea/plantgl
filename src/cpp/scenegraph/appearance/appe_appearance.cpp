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





#include "appe_appearance.h"
#include "scne_smbtable.h"
#include "actn_printer.h"

GEOM_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */


Appearance::Builder::Builder( ) :
  SceneObject::Builder() {
}


Appearance::Builder::~Builder( ) {
}

/* ----------------------------------------------------------------------- */

Appearance::Appearance( ) :
  SceneObject() {
}

Appearance::~Appearance( ) {
}

/* ----------------------------------------------------------------------- */
ostream& operator<<( ostream& stream, Appearance& a ){
    Printer p(cout,cout,stream);
    a.apply(p);
    return stream;
}

/* ----------------------------------------------------------------------- */


bool Appearance::parse( istream& input,
                        ostream& output,
                        AppearanceSymbolTable& table,
                        int max_errors ) {
/*  GenericParser<AppearancePtr> _parser(appe_yyparse,&table,max_errors);
  AppearanceRecursiveLexer _appeLexer(&input,&output);
  return _parser.parse(&_appeLexer,output);*/

  SceneObjectSymbolTable t;
  if(!SceneObject::parse(input,output,t,max_errors ))return false;
  AppearancePtr _app;
  for(SceneObjectSymbolTable::iterator _it = t.begin();
      _it != t.end() ;
      _it++){
    if(_app.cast(_it->second))
      table[_it->first] = _app;
  }
  return true;
}


bool Appearance::parse( const string& filename,
                        ostream& output,
                        AppearanceSymbolTable& table,
                        int max_errors ) {
/*  ifstream _file(filename.c_str());
  if (! (_file)) {
    genMessage(ERRORMSG(C_FILE_OPEN_ERR_s),filename.c_str());
    return false;
  };
  GenericParser<AppearancePtr> _parser(appe_yyparse,&table,max_errors);
  AppearanceRecursiveLexer _appeLexer(&_file,&output,filename.c_str());
  return _parser.parse(&_appeLexer,output);*/


    SceneObjectSymbolTable t;
    if(!SceneObject::parse(filename,output,t,max_errors ))return false;
    AppearancePtr _app;
    for(SceneObjectSymbolTable::iterator _it = t.begin();
        _it != t.end() ;
        _it++){
        if(_app.cast(_it->second))
            table[_it->first] = _app;
    }
    return true;
}

bool Appearance::isTexture() const {
  return false;
}


/* ----------------------------------------------------------------------- */

