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


/*! \file view_scenegl.h
    \brief Definition of the viewer classes ViewRendererGL and ViewSceneRendererGL.
*/

#ifndef __view_scenegl_h__
#define __view_scenegl_h__

/* ----------------------------------------------------------------------- */

#include "object.h"
#include "glframe.h"
#include <plantgl/tool/util_types.h>
#include <plantgl/scenegraph/geometry/boundingbox.h>
#include <QtCore/qdatetime.h>

/* ----------------------------------------------------------------------- */

#include <iostream>

/* ----------------------------------------------------------------------- */

class QTabWidget;
class QPopupmenu;
class QMenuBar;
class QMainWindow;
class QTreeWidget;
class QTreeWidgetItem;
class QOpenGLWidget;
class QEvent;

/* ----------------------------------------------------------------------- */

class ViewCameraGL;
class ViewLightGL;
class ViewSceneChangeEvent;

/* ----------------------------------------------------------------------- */

#include "flags.h"

/* ----------------------------------------------------------------------- */

/**
   \class ViewRendererGL
   \brief An abstract class for Display Manager.

*/

/* ----------------------------------------------------------------------- */

class VIEW_API ViewRendererGL  : public ViewObjectGL
{
  Q_OBJECT
  Q_PROPERTY( QString Filename READ getFilename WRITE openFile )

public :

  /// Constructor
  ViewRendererGL(QOpenGLBaseWidget * parent=0, const char * name=0, PGLOpenGLFunctionsPtr ogl = NULL);

  /// Destructor.
  virtual ~ViewRendererGL();

  /// Connect this to a statusbar.
  virtual void connectTo(ViewStatusBar *);

  /// Connect this to a GL Widget.
  virtual void connectTo(QOpenGLBaseWidget *);

  /// Connect this to a error dialog.
  virtual void connectTo(ViewErrorDialog *);

  /// Called when the qglwidget enter in this mode.
  virtual void changeModeEvent(ViewGLFrame::Mode m);

  /// Paint scene for Selection.
  virtual void selectGL();

  /// End of Selection. Return whether viewer must stay in selection mode (true) or not (false).
  virtual bool endSelect();

  /// end event.
  virtual void endEvent();

  /// Scene change Event.
  virtual bool sceneChangeEvent(ViewSceneChangeEvent *);

  /// Add Open Entries
  virtual bool addOpenEntries(QMenu * menu);

  /// Add Import Entries
  virtual bool addImportEntries(QMenu * menu);

  /// Add Save Entries
  virtual bool addSaveEntries(QMenu * menu);

  /// Add Export Entries
  virtual bool addExportEntries(QMenu * menu);

  /// Add Edit Entries
  virtual bool addEditEntries(QMenu * menu);

  /// Add properties info in the \e tab
  virtual bool addProperties(QTabWidget * tab);

  /// Add other menu.
  virtual bool addOtherMenu(QMenuBar * menu);

  /// Add other toolbar.
  virtual bool addOtherToolBar(QMainWindow * menu);

  /// Fill the list for browsing.
  virtual bool browse(QTreeWidget *,bool);

  /// Try to open file. Return true if ok.
  virtual bool open(const QString& filename);

  /// Try to open stream. Return true if ok.
  virtual bool openStream(std::istream& stream);

  /// Save scene in \e filename.
  virtual bool saveAs(const QString& filename);

  /// Clear.
  virtual void clear();

  /// Get current filename.
  const QString& getFilename() const;

  /// Set filename.
  void setFilename(const QString&);

  /// Function to tell the qglwidget to enter in this mode.
  void setRotationMode();

  /// Function to tell the qglwidget to enter in this mode.
  void setSelectionMode();

  virtual std::vector<uint_t> getSelectionIds() const;

  virtual uint_t translateId(uint_t) const;

  /// Get the global Bounding Box.
  virtual const PGL(BoundingBoxPtr) getGlobalBoundingBox() const;

  virtual bool isEmpty() const;

public slots:

  /// Try to open filename. call open(filename).
  void openFile(const QString& filename);

  virtual void addFile(const QString& filename);

  /// Save.
  virtual void save();

  /// Save As
  virtual void saveAs();

  /// Refresh.
  virtual void refresh();
  virtual void refreshDisplay();

  /// Close. Actually call clear function.
  virtual void close();

  /// Clear Selection Event.
  virtual void clearSelectionEvent();

  /// Selection Event.
  virtual void selectionEvent(uint_t);
  virtual void selectionEvent(const std::vector<uint_t>&);
  virtual void selectionIdEvent(const std::vector<uint_t>&);
  virtual void selectionEvent(QTreeWidgetItem *);

  virtual void checkFileModification();

signals:

  /// Signal emit when filename has changed.
  void filenameChanged(const QString&);

  /// Signal emit when the scene change.
  void sceneChanged();

  /// Signal emit when the renderer ask to change of mode.
  void changeMode(ViewGLFrame::Mode);

  void selectionChanged(const QString&);

protected:

  /// Event processing.
  virtual bool event(QEvent *);


private:

  /// Filename.
  QString __filename;
  QDateTime __readTime;

};

/**
      \class ViewSceneRendererGL
      \brief An abstract class for Display Manager of GL Scene.

*/
class VIEW_API ViewSceneRendererGL  : public ViewRendererGL
{
  Q_OBJECT

public :

  /// Constructor.
  ViewSceneRendererGL(ViewCameraGL * camera=0,
              ViewLightGL * light=0,
              QOpenGLBaseWidget * parent=0,
              const char * name=0, 
              PGLOpenGLFunctionsPtr ogl = NULL);

  /// Destructor.
  virtual ~ViewSceneRendererGL();

  /// Get the camera.
  ViewCameraGL *getCamera() const;

  /// Get the light.
  ViewLightGL *getLight() const;

  /// Set the camera.
  void setCamera(ViewCameraGL *);

  /// Set the light.
  void setLight(ViewLightGL *);

  inline eAnimationFlag isAnimated() const { return __animated; }

public slots:

  virtual void computeCamera();

  virtual void useDisplayList(bool);

  void setAnimation(eAnimationFlag flag) { __animated = flag;  animationChangedEvent(flag); }


protected :

  virtual void animationChangedEvent(eAnimationFlag flag) { }

  /// The camera.
  ViewCameraGL * __camera;

  /// The light.
  ViewLightGL * __light;

  eAnimationFlag __animated;
};

/* ----------------------------------------------------------------------- */
#endif

