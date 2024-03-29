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


/* ----------------------------------------------------------------------- */

#include "cdc_geom.h"

#include "scne_binaryparser.h"
#include "binaryprinter.h"
#include "printer.h"

#include <plantgl/tool/dirnames.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/tool/errormsg.h>
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

#ifdef PGL_WITH_BISONFLEX
#include "scne_parser.h"    /// fonction pour parser les sceneobjects.
#endif

PGL_USING_NAMESPACE

#ifdef PGL_WITH_BISONFLEX
#include "scne_scanner.h"   /// Initialisation des noms des classes de scanner
                            /// [ par redefinition de yyFlexLexer et GENERIC_LEXER ]
#include <plantgl/tool/gparser.h>  /// Instanciation des classes de scans.
                               /// et instanciation de la classe template de parser.

#undef YYLEX_PARAM
#define YYLEX_PARAM param

/// structure de donnees pour parser les sceneobjects.
#ifdef PGL_BISON_HPP
#include "scne_parser.hpp"
#else
#include "scne_parser.cpp.h"
#endif


#endif

#include "scne_binaryparser.h"
#include <plantgl/tool/readline.h>

using namespace std;

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

#ifdef PGL_WITH_BISONFLEX

bool PGL(geom_read)(std::istream& stream, SceneObjectSymbolTable& table, ScenePtr& scene, const std::string& fname)
{
    GenericParser<SceneObjectPtr> _parser(scne_yyparse,&table);
    SceneObjectRecursiveLexer _sceneLexer(&stream,PglErrorStream::error,fname.c_str());
    Timer t;
    t.start();
    bool b =_parser.parse(&_sceneLexer,*PglErrorStream::error,scene.get());
    t.stop();
    if(isParserVerbose())printf("Parse file %s in %.2f sec.\n", fname.c_str(),t.elapsedTime());
    return b;
}

#endif

/* ----------------------------------------------------------------------- */

GeomCodec::GeomCodec() :
    SceneCodec("GEOM", ReadWrite )
    {}

SceneFormatList GeomCodec::formats() const
{
    SceneFormat _format;
    _format.name = "GEOM";
#ifdef PGL_WITH_BISONFLEX
    _format.suffixes.push_back("geom");
#endif
    _format.suffixes.push_back("bgeom");
    _format.comment = "The Standart PlantGL format.";
    SceneFormatList _formats;
    _formats.push_back(_format);
    return _formats;
}

ScenePtr GeomCodec::read(const std::string& fname)
{
  if(BinaryParser::isAGeomBinaryFile(fname)){
      BinaryParser _parser(*PglErrorStream::error);
      _parser.parse(fname);
      return _parser.getScene();
  }
/*  else  if(get_suffix(fname)=="smb"){
    GeometryPtr a(new AmapSymbol(fname));
    ScenePtr sc = new Scene();
    sc->add(Shape(a,Material::DEFAULT_MATERIAL));
    return sc;
  } */
#ifdef PGL_WITH_BISONFLEX
  else {
    ifstream _file(fname.c_str());
    SceneObjectSymbolTable table;
    ScenePtr scene(new Scene());
    bool b = geom_read(_file,table,scene,fname);
    if(!b) return ScenePtr();
    else {
        if(scene && !scene->empty()) return scene;
        else return ScenePtr(new Scene(table));
    }
  }
#endif
}

bool GeomCodec::write(const std::string& fname,const ScenePtr&  scene)
{
    std::string ext = get_suffix(fname);
    ext = toUpper(ext);
    if(ext == "BGEOM"){
        BinaryPrinter::print(scene,fname,"File Generated with PlantGL.");
        return true;
    }
    else{
        std::ofstream stream(fname.c_str());
        if(stream){
            Printer p(stream,stream,stream);
            scene->apply(p);
            return true;
        }
        else return false;
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
      BinaryParser _parser(*PglErrorStream::error);
      _parser.parse(fname);
      return _parser.getScene();
  }
  else return ScenePtr();
}

bool BGeomCodec::write(const std::string& fname,const ScenePtr& scene)
{
    BinaryPrinter::print(scene,fname,"File Generated with PlantGL.");
    return true;
}
/* ----------------------------------------------------------------------- */
