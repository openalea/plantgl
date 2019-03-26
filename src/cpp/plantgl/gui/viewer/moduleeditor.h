/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
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

#ifndef _GEOM_MODULEEDITOR_H_
#define _GEOM_MODULEEDITOR_H_

#include "../gui_config.h"
#include <plantgl/scenegraph/core/sceneobject.h>
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qwidget.h>
#else
    #include <QtGui/qwidget.h>
#endif

class QMenuBar;
class QToolBar;
class QMainWindow;

/**
    \class GeomModuleEditor
    \brief Abstract Class for Module Edition.
*/

class VIEW_API GeomModuleEditor : public QWidget
{

  Q_OBJECT

public:
  /// Constructor.
    GeomModuleEditor( QWidget* parent = 0, Qt::WindowFlags f = 0 ) :
    QWidget( parent ,  f ){
  }

  /// Destructor.
  virtual ~GeomModuleEditor(){
  }

  /// Add a menu to the main menu bar when show.
  virtual void addMenu(QMenuBar * ){
  }

  /// remove menu to the main menu bar when hide.
  virtual void removeMenu(QMenuBar *){
  }

  /// Create a toolbar witch will be add to the main windows..
  virtual QToolBar * addToolBar(QMainWindow *) {
    return NULL;
  }

  /// Return toolbar of the module for removing
  virtual QToolBar * toolBar() {
    return NULL;
  }

  /// Get the name of the class of objets that the module manage.
  virtual QString geomClassName() const = 0;

  /// Get Optimal size of display.
  virtual const QSize getSize() const = 0;

  /*
     Must return true if \e this can manage the edition, else false.
     Must recopy the object.
  */
  virtual bool setGeomObject(PGL(SceneObjectPtr) obj) = 0;

  /// Edit a new object.
  virtual void newObj() = 0;

  /// Ask object before saving.
  virtual PGL(SceneObjectPtr) getGeomObject() = 0;

public slots:

  virtual void clear() = 0 ;

  signals :

  void valueChanged();

};

#endif

