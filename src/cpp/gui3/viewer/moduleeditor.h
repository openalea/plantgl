#ifndef _GEOM_MODULEEDITOR_H_
#define _GEOM_MODULEEDITOR_H_

#include "../gui3_config.h"
#include <scenegraph/core/sceneobject.h>
#include <qwidget.h>

class QMenuBar;
class QToolBar;
class QMainWindow;

/**
    \class GeomModuleEditor3
    \brief Abstract Class for Module Edition.
*/

class VIEW3_API GeomModuleEditor3 : public QWidget
{

  Q_OBJECT

public:
  /// Constructor.
	GeomModuleEditor3( QWidget* parent = 0, WFlags f = 0 ) :
    QWidget( parent , "" , f ){
  }

  /// Destructor.
  virtual ~GeomModuleEditor3(){
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

