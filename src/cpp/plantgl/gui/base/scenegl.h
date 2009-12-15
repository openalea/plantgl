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
class Q3ListView;
class Q3ListViewItem;
class QGLWidget;
class QEvent;

/* ----------------------------------------------------------------------- */

class ViewCameraGL;
class ViewLightGL;
class ViewSceneChangeEvent;

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
  ViewRendererGL(QGLWidget * parent=0, const char * name=0);
  
  /// Destructor.
  virtual ~ViewRendererGL();

  /// Connect this to a statusbar.
  virtual void connectTo(ViewStatusBar *);

  /// Connect this to a GL Widget.
  virtual void connectTo(QGLWidget *);

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
  virtual bool browse(Q3ListView *,bool);

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
  virtual const PGL::BoundingBoxPtr getGlobalBoundingBox() const;

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
  virtual void selectionEvent(Q3ListViewItem *);

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
		      QGLWidget * parent=0, 
		      const char * name=0);

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

public slots:

  virtual void computeCamera();

  virtual void useDisplayList(bool);


protected :

  /// The camera.
  ViewCameraGL * __camera;

  /// The light.
  ViewLightGL * __light;

};

/* ----------------------------------------------------------------------- */
#endif

