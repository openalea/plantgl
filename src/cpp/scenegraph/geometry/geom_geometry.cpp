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






#include "geom_geometry.h"
#include "actn_printer.h"
#include "scne_smbtable.h"
GEOM_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */

Geometry::Builder::Builder( ) {
  // nothing to do
}


Geometry::Builder::~Builder( ) {
  // nothing to do
}


/* ----------------------------------------------------------------------- */

Geometry::Geometry( ) :
  SceneObject() {
}

Geometry::~Geometry( ) {
}

/* ----------------------------------------------------------------------- */

ostream& operator<<( ostream& stream, Geometry& g ){
    Printer p(cout,stream,cout);
    g.apply(p);
    return stream;
}

/* ----------------------------------------------------------------------- */

bool Geometry::parse( istream& input,
                      ostream& output,
                      GeometrySymbolTable& table,
                      int max_errors ) {
/*  GenericParser<GeometryPtr> _parser(geom_yyparse,&table,max_errors);
  GeometryRecursiveLexer _geomLexer(&input,&output);
  return _parser.parse(&_geomLexer,output);*/
  SceneObjectSymbolTable t;
  if(!SceneObject::parse(input,output,t,max_errors ))return false;
  GeometryPtr _geom;
  for(SceneObjectSymbolTable::iterator _it = t.begin();
      _it != t.end() ;
      _it++){
      if(_geom.cast(_it->second))
          table[_it->first] = _geom;
  }
  return true;
}


bool Geometry::parse( const string& filename,
                      ostream& output,
                      GeometrySymbolTable& table,
                      int max_errors ) {
/*  ifstream _file(filename.c_str());
  if (! (_file)) {
    genMessage(ERRORMSG(C_FILE_OPEN_ERR_s),filename.c_str());
    return false;
  };
  GenericParser<GeometryPtr> _parser(geom_yyparse,&table,max_errors);
  GeometryRecursiveLexer _geomLexer(&_file,&output,filename.c_str());
  return _parser.parse(&_geomLexer,output); */

  SceneObjectSymbolTable t;
  if(!SceneObject::parse(filename,output,t,max_errors ))return false;
  GeometryPtr _geom;
  for(SceneObjectSymbolTable::iterator _it = t.begin();
      _it != t.end() ;
      _it++){
      if(_geom.cast(_it->second))
          table[_it->first] = _geom;
  }
  return true;
}



/* ----------------------------------------------------------------------- */
