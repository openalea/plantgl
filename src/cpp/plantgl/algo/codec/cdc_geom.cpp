/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
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

/* ----------------------------------------------------------------------- */

#include "cdc_geom.h"

#include "scne_binaryparser.h"
#include "binaryprinter.h"
#include "printer.h"

#include <plantgl/tool/dirnames.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/tool/timer.h>

#include <plantgl/scenegraph/core/sceneobject.h>
#include <plantgl/scenegraph/core/smbtable.h>
#include <plantgl/scenegraph/core/pgl_messages.h>

#include <plantgl/pgl_scene.h>
#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>

#include <plantgl/scenegraph/container/pointmatrix.h>

#include <list>
#include <fstream>

#include "scne_parser.h"    /// fonction pour parser les sceneobjects.

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/// structure de donnees pour parser les sceneobjects.
#ifdef BISON_HPP
#include "scne_parser.hpp"
#else
#include "scne_parser.cpp.h"
#endif

#include "scne_scanner.h"   /// Initialisation des noms des classes de scanner
                            /// [ par redefinition de yyFlexLexer et GENERIC_LEXER ]

#include <plantgl/tool/gparser.h>  /// Instanciation des classes de scans.
                               /// et instanciation de la classe template de parser.

#include "scne_binaryparser.h"
#include <plantgl/tool/readline.h>

using namespace std;

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

bool PGL(geom_read)(std::istream& stream, SceneObjectSymbolTable& table, ScenePtr& scene, const std::string& fname)
{
    GenericParser<SceneObjectPtr> _parser(scne_yyparse,&table);
    SceneObjectRecursiveLexer _sceneLexer(&stream,SceneObject::errorStream,fname.c_str());
    Timer t;
    t.start();
    bool b =_parser.parse(&_sceneLexer,*SceneObject::errorStream,scene.toPtr());
    t.stop();
    if(isParserVerbose())cerr << "Parse file " << fname.c_str() << " in " << t.elapsedTime() << " sec. " << endl;
	return b;
}

/* ----------------------------------------------------------------------- */

GeomCodec::GeomCodec() : 
	SceneCodec("GEOM", ReadWrite ) 
	{}

SceneFormatList GeomCodec::formats() const
{
	SceneFormat _format;
	_format.name = "GEOM";
	_format.suffixes.push_back("geom");
	_format.suffixes.push_back("bgeom");
	_format.comment = "The Standart PlantGL format.";
	SceneFormatList _formats;
	_formats.push_back(_format);
	return _formats;
}

ScenePtr GeomCodec::read(const std::string& fname)
{
  if(BinaryParser::isAGeomBinaryFile(fname)){
	  BinaryParser _parser(*SceneObject::errorStream);
	  _parser.parse(fname);
	  return _parser.getScene();
  }
/*  else  if(get_suffix(fname)=="smb"){
    GeometryPtr a(new AmapSymbol(fname));
	ScenePtr sc = new Scene();
	sc->add(Shape(a,Material::DEFAULT_MATERIAL));
    return sc;
  } */
  else {
    ifstream _file(fname.c_str());
	SceneObjectSymbolTable table;
	ScenePtr scene(new Scene());
	bool b = geom_read(_file,table,scene,fname);
    if(!b) return ScenePtr(0);
	else {
		if(scene && !scene->isEmpty()) return scene;
		else return new Scene(table);
	}
  }
  
}

void GeomCodec::write(const std::string& fname,const ScenePtr&	scene)
{
	std::string ext = get_suffix(fname);
	ext = toUpper(ext);
	if(ext == "BGEOM"){
		BinaryPrinter::print(scene,fname,"File Generated with PlantGL.");
	}
	else{
		std::ofstream stream(fname.c_str());
		if(stream){
			Printer p(stream,stream,stream);
			scene->apply(p);
		}
	}
}

/* ----------------------------------------------------------------------- */

BGeomCodec::BGeomCodec() : 
	SceneCodec("BGEOM", ReadWrite ) 
	{}

SceneFormatList BGeomCodec::formats() const
{
	SceneFormat _format;
	_format.name = "BGEOM";
	_format.suffixes.push_back("bgeom");
	_format.comment = "The Standart PlantGL binary format.";
	SceneFormatList _formats;
	_formats.push_back(_format);
	return _formats;
}

ScenePtr BGeomCodec::read(const std::string& fname)
{
  if(BinaryParser::isAGeomBinaryFile(fname)){
	  BinaryParser _parser(*SceneObject::errorStream);
	  _parser.parse(fname);
	  return _parser.getScene();
  }
  else return ScenePtr(0);
}

void BGeomCodec::write(const std::string& fname,const ScenePtr&	scene)
{
    BinaryPrinter::print(scene,fname,"File Generated with PlantGL.");
}
/* ----------------------------------------------------------------------- */
