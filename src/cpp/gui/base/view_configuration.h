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
#ifndef __view_configuration_h__
#define __view_configuration_h__


#include "Tools/rcobject.h"
#include "Tools/gsmbtable.h"
#include <qstring.h>
#include "Tools/config.h"

/*--------------------------------------------------------------*/

/**
    \class ViewComponentConfig
    \brief Abstract base class for all Viewer component config object.
*/
/* ----------------------------------------------------------------------- */


class ViewComponentConfig : public RefCountObject
{
public :
  /// A constructor.
  ViewComponentConfig(){
  };

  /// A destructor.
  virtual ~ViewComponentConfig(){
  };

  /// Print self to ostream \e _ostream.
  virtual void write(std::ostream& _ostream) = 0;

};

/*--------------------------------------------------------------*/

/**
    \class WindowConfig
    \brief class for all viewer window component config object.
*/
class WindowConfig : public ViewComponentConfig
{
public :

  /// A constructor.
  WindowConfig():
    ViewComponentConfig(),
    __width(0),
    __height(0),
    __x(0),
    __y(0){
  }

  /// A destructor.
  virtual ~WindowConfig();

  virtual void write(std::ostream& _ostream);

  /// Width field
  uint32 * __width;

  /// Height field
  uint32 * __height;

  /// X coordinate field
  uint32 * __x;

  /// Y coordinate field
  uint32 * __y;

};

typedef RCPtr<WindowConfig> WindowConfigPtr;

/*--------------------------------------------------------------*/

/**
    \class ToolBarConfig
    \brief class for all viewer toolbar component config object.
*/
class ToolBarConfig : public ViewComponentConfig
{
public :

  /// A constructor.
  ToolBarConfig() :
    ViewComponentConfig(),
    __x(0),
    __y(0),
    __Orientation(0),
    __hidden(0){
  };

  /// A destructor.
  virtual ~ToolBarConfig();

  virtual void write(std::ostream& _ostream);

  /// X coordinate field
  uint32 * __x;

  /// Y coordinate field
  uint32 * __y;

  /// orientation field
  bool * __Orientation;

  /// hidden field
  bool * __hidden;

};

typedef RCPtr<ToolBarConfig> ToolBarConfigPtr;

/*--------------------------------------------------------------*/

/**
    \class ViewFileManagerConfig
    \brief class for filemanager config.
*/
class ViewFileManagerConfig : public ViewComponentConfig
{

public :

  /// Constructor
  ViewFileManagerConfig() :
  ViewComponentConfig(),
  __fileToolBar(0),
  __history(0),
  __directory(0){
  }

  /// Destructor
  virtual ~ViewFileManagerConfig();

  virtual void write(std::ostream& _ostream);

  /// Toolbar Field.
  ToolBarConfigPtr * __fileToolBar;

  /// History Field.
  deque<QString> * __history;

  /// Directory Field.
  QString * __directory;
};

typedef RCPtr<ViewFileManagerConfig> ViewFileManagerConfigPtr;

/*--------------------------------------------------------------*/

/**
    \class RotCenterConfig
    \brief class for Rotating Center config.
*/
class RotCenterConfig : public WindowConfig
{
public :
  /// Constructor
  RotCenterConfig():
  WindowConfig(),
  __XCenter(0),
  __YCenter(0),
  __ZCenter(0){
  }

  /// Destructor
  virtual ~RotCenterConfig();

  virtual void write(std::ostream& _ostream);

  /// X coordinate field
  real_t * __XCenter;

  /// Y coordinate field
  real_t * __YCenter;

  /// Z coordinate field
  real_t * __ZCenter;

};

typedef RCPtr<RotCenterConfig> RotCenterConfigPtr;

/*--------------------------------------------------------------*/

/**
    \class GridConfig
    \brief class for ViewGLFrame Grid config.
*/
class GridConfig : public ViewComponentConfig
{
public :

  /// Constructor
  GridConfig():
    ViewComponentConfig(),
    __XYPlane(0),
    __YZPlane(0),
    __XZPlane(0),
    __Axes(0){
  }

  /// Destructor
  virtual ~GridConfig();

  virtual void write(std::ostream& _ostream);

  /// XYPlane Visibility Field.
  bool * __XYPlane;

  /// YZPlane Visibility Field.
  bool * __YZPlane;

  /// XZPlane Visibility Field.
  bool * __XZPlane;

  /// Axes Visibility Field.
  bool * __Axes;

};

typedef RCPtr<GridConfig> GridConfigPtr;

/*--------------------------------------------------------------*/

/**
    \class ColorConfig
    \brief class for ViewGLFrame Background Color config.
*/
class ColorConfig : public ViewComponentConfig
{
public :

  /// Constructor
  ColorConfig() :
    __Red(0),
    __Green(0),
    __Blue(0){
  }

  /// Destructor
  virtual ~ColorConfig();

  virtual void write(std::ostream& _ostream);


  /// Red value of the background color
  uint32  * __Red;

  /// Green value of the background color
  uint32  * __Green;

  /// Blue value of the background color
  uint32  * __Blue;

};

typedef RCPtr<ColorConfig> ColorConfigPtr;

/*--------------------------------------------------------------*/

/**
    \class ViewGLFrameConfig
    \brief class for ViewGLFrame config.
*/
class ViewGLFrameConfig : public ViewComponentConfig
{
public :

  /// Constructor
  ViewGLFrameConfig() :
    ViewComponentConfig(),
    __Grid(0),
    __Color(0){
  }

  /// Destructor
  virtual ~ViewGLFrameConfig();

  virtual void write(std::ostream& _ostream);

  /// Grid Field;
  GridConfigPtr * __Grid;

  /// Background Color Field.
  ColorConfigPtr * __Color;
};

typedef RCPtr<ViewGLFrameConfig> ViewGLFrameConfigPtr;

/*--------------------------------------------------------------*/

class ViewerConfig;
typedef RCPtr<ViewerConfig> ViewerConfigPtr;
typedef SymbolTable<ViewerConfigPtr> ViewerConfigSymbolTable;

/**
    \class ViewerConfig
    \brief main class for viewer config.
*/
class ViewerConfig : public WindowConfig
{

public :

  /// Constructor
  ViewerConfig() :
    WindowConfig(),
    __ctrlPanelCfg(0),
    __errorDialogCfg(0),
    __fileManagerCfg(0),
    __geomBrowserCfg(0),
    __glFrameCfg(0),
    __rotCenterCfg(0),
    __fullsize(0){
  }

  /// Destructor
  virtual ~ViewerConfig();

  virtual void write(std::ostream& _ostream);

  /// Control Panel config Field.
  ToolBarConfigPtr * __ctrlPanelCfg;

  /// Error Dialog config Field.
  WindowConfigPtr * __errorDialogCfg;

  ///  File Manager config Field.
  ViewFileManagerConfigPtr * __fileManagerCfg;

  /// Geom Browser config Field.
  WindowConfigPtr * __geomBrowserCfg;

  /// ViewGLFrame config Field.
  ViewGLFrameConfigPtr * __glFrameCfg;

  /// Rotating Center Coordinates Field.
  RotCenterConfigPtr * __rotCenterCfg;

  /// Full Size Field
  bool * __fullsize;

  /** Parse the file \e fileName and returns the corresponding symbol table
      \e table.
      \pre
      - \e fileName must exist;
      - \e outputStream must be valid. */
  static bool parse( const std::string& filename,
                     std::ostream& outputStream,
                     ViewerConfigSymbolTable& table,
                     int max_errors = -1 );


  /** Parse the file \e fileName and returns the corresponding Viewer Config
      \pre
      - \e fileName must exist; */

  static ViewerConfigPtr read ( const std::string& filename );
};

/*--------------------------------------------------------------*/

#endif // __view_configuration_h__
