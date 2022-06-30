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
    GeomModuleEditor( QWidget* parent = 0, Qt::WindowFlags f = Qt::WindowFlags() ) :
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

