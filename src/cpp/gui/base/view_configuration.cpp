/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon.
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


#include "view_parser.h"
#include "view_scanner.h"
#include "Tools/gparser.h"
#include "view_configuration.h"
#include "view_parser.C.h"

GEOM_USING_NAMESPACE

/*--------------------------------------------------------------*/

WindowConfig::~WindowConfig(){
/*  if(__width) delete __width;
  if(__height) delete __height;
  if(__x) delete __x;
  if(__y) delete __y;*/
}

void 
WindowConfig::write(ostream& _ostream){
  if(__height)_ostream << "Height : " << (*__height) << endl;
  if(__width)_ostream << "Width  : " << (*__width)  << endl;
  if(__x)_ostream << "X  : " << (*__x)  << endl;
  if(__y)_ostream << "Y  : " << (*__y)  << endl;
}

ToolBarConfig::~ToolBarConfig(){
/*  if(__x) delete __x;
  if(__y) delete __y;*/
  if(__Orientation) delete __Orientation;
  if(__hidden) delete __hidden;
}

void 
ToolBarConfig::write(ostream& _ostream){
  if(__x)_ostream << "X  : " << (*__x)  << endl;
  if(__y)_ostream << "Y  : " << (*__y)  << endl;
  if(__hidden)_ostream << "Hidden : " << (*__hidden ? "True" : "False" ) << endl;
  if(__Orientation)_ostream << "Orientation : " << (*__Orientation ? "True" : "False" ) << endl;
}

ViewFileManagerConfig::~ViewFileManagerConfig(){
  if(__directory)delete __directory;
//  if(__history) delete __history;
  if(__fileToolBar) delete __fileToolBar;
}

void 
ViewFileManagerConfig::write(ostream& _ostream){
    if(__directory)_ostream << "Directory : \"" << *__directory << "\"" << endl;
  if(__history){
    _ostream << "History { " << endl;
    for (deque<QString>::iterator _it=__history->begin();
	 _it != __history->end(); _it++){
      _ostream  << "\"" << *_it << "\"";
      if(_it !=__history->end()-1)
	_ostream << ",";
      _ostream << endl;
    }
    _ostream << "}" << endl;
  }
  if(__fileToolBar){
    _ostream << "FileToolBar {" << endl;
    (*__fileToolBar)->write(_ostream);
    _ostream << "}" << endl;
  }
}

RotCenterConfig::~RotCenterConfig(){
  if(__XCenter) delete __XCenter;
  if(__YCenter) delete __YCenter;
  if(__ZCenter) delete __ZCenter;
/*  if(__width) delete __width;
  if(__height) delete __height;
  if(__x) delete __x;
  if(__y) delete __y;*/
}


void 
RotCenterConfig::write(ostream& _ostream){
  WindowConfig::write(_ostream);
  if(__XCenter) _ostream << "XCenter : " << *__XCenter << endl;
  if(__YCenter) _ostream << "YCenter : " << *__YCenter << endl;
  if(__ZCenter) _ostream << "ZCenter : " << *__ZCenter << endl;
}


GridConfig::~GridConfig(){
/*  if(__XYPlane) delete __XYPlane;
  if(__YZPlane) delete __YZPlane;
  if(__XZPlane) delete __XZPlane;
  if(__Axes) delete __Axes;*/
}

void 
GridConfig::write(ostream& _ostream){
  if(__XYPlane) _ostream << "XYPlane : " << (*__XYPlane ? "True" : "False") << endl;
  if(__YZPlane) _ostream << "YZPlane : " << (*__YZPlane ? "True" : "False") << endl;
  if(__XZPlane) _ostream << "XZPlane : " << (*__XZPlane ? "True" : "False") << endl;
  if(__Axes) _ostream << "Axes : " << (*__Axes ? "True" : "False") << endl;
}

ColorConfig::~ColorConfig(){
  if(__Red) delete __Red;
  if(__Green) delete __Green;
  if(__Blue) delete __Blue;
}

void 
ColorConfig::write(ostream& _ostream){
  if(__Red) _ostream << "Red   : " << *__Red << endl;
  if(__Green) _ostream << "Green : " << *__Green << endl;
  if(__Blue) _ostream << "Blue  : " << *__Blue << endl;
}

GLFrameConfig::~GLFrameConfig(){
  if(__Grid) delete __Grid;
  if(__Color) delete __Color;
}

void 
GLFrameConfig::write(ostream& _ostream){
  if(__Grid) {
    _ostream << "Grid {" << endl;
    (*__Grid)->write(_ostream);
    _ostream << "}" << endl << endl;
  }
  if(__Color) {
    _ostream << "BackGroundColor {" << endl;
    (*__Color)->write(_ostream);
    _ostream << "}" << endl;
  }
}

void 
ViewerConfig::write(ostream& _ostream){
  _ostream << "Viewer {" << endl << endl;
  WindowConfig::write(_ostream);
  if(__fullsize)
    _ostream << "FullSize : " << (*__fullsize ? "True" : "False") << endl;
  _ostream << endl << endl;
  if(__ctrlPanelCfg){
    _ostream << "ControlPanel {" << endl;
    (*__ctrlPanelCfg)->write(_ostream);
    _ostream << "}" << endl << endl;
  }
  if(__rotCenterCfg){
    _ostream << "RotatingCenterCoordinate {" << endl;
    (*__rotCenterCfg)->write(_ostream);
    _ostream << "}" << endl << endl;
  }
  if(__errorDialogCfg){
    _ostream << "ErrorDialog {" << endl;
    (*__errorDialogCfg)->write(_ostream);
    _ostream << "}" << endl << endl;
  }
  if(__fileManagerCfg){
    _ostream << "FileManager {" << endl;
    (*__fileManagerCfg)->write(_ostream);
    _ostream << "}" << endl << endl;
  }
  if(__geomBrowserCfg){
    _ostream << "GeomBrowser {" << endl;
    (*__geomBrowserCfg)->write(_ostream);
    _ostream << "}" << endl << endl;
  }
  if(__glFrameCfg){
    _ostream << "GLFrame {" << endl;
    (*__glFrameCfg)->write(_ostream);
    _ostream << "}" << endl << endl;
  }
  _ostream << "}" << endl << endl;
}

ViewerConfig::~ViewerConfig(){
  if(__ctrlPanelCfg) delete __ctrlPanelCfg;
  if(__errorDialogCfg) delete __errorDialogCfg;
  if(__fileManagerCfg) delete __fileManagerCfg;
  if(__geomBrowserCfg) delete __geomBrowserCfg;
  if(__glFrameCfg) delete __glFrameCfg;
/*  if(__fullsize) delete __fullsize;
  if(__width) delete __width;
  if(__height) delete __height;
  if(__x) delete __x;
  if(__y) delete __y;*/
}


bool
ViewerConfig::parse( const string& filename,
		     ostream& outputStream, 
		     ViewerConfigSymbolTable& table,
		     int max_errors = -1 ){
  istream* _file = new ifstream(filename.c_str()); 
  if (!*_file) {
    delete _file;
    cerr << "Viewer Config Error !" << endl;
    cerr << "Cannot open " << filename << endl;
    return false;
  };
  GenericParser<ViewerConfigPtr> _parser(view_yyparse,&table,max_errors);
  ViewerRecursiveLexer _viewerLexer(_file,&outputStream,filename.c_str());
  bool b = _parser.parse(&_viewerLexer,outputStream);
  delete _file;  
  return b;
};


ViewerConfigPtr 
ViewerConfig::read( const string& filename ){
    ViewerConfigSymbolTable table;
    if(ViewerConfig::parse(filename,cerr,table));
    if(table.empty())
	return ViewerConfigPtr();
    else 
	return (table.begin())->second;
       
}
