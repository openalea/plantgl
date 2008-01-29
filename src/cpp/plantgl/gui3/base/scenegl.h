/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): F. Boudon
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

/*! \file view_scenegl.h
    \brief Definition of the viewer classes ViewRendererGL3 and ViewSceneRendererGL3.
*/

#ifndef __view_scenegl_h__
#define __view_scenegl_h__

/* ----------------------------------------------------------------------- */

#include "object.h"
#include "glframe.h"
#include <plantgl/tool/util_types.h>
#include <plantgl/scenegraph/geometry/boundingbox.h>
#include <qdatetime.h>

/* ----------------------------------------------------------------------- */

#include <iostream>

/* ----------------------------------------------------------------------- */

class QTabWidget;
class QPopupmenu;
class QMenuBar;
class QMainWindow;
class QListView;
class QListViewItem;
class QGLWidget;
class QEvent;

/* ----------------------------------------------------------------------- */

class ViewCameraGL3;
class ViewLightGL3;
class ViewSceneChangeEvent3;

/* ----------------------------------------------------------------------- */

/**   
   \class ViewRendererGL3
   \brief An abstract class for Display Manager.

*/

/* ----------------------------------------------------------------------- */

class VIEW3_API ViewRendererGL3  : public ViewObjectGL3
{
  Q_OBJECT
  Q_PROPERTY( QString Filename READ getFilename WRITE openFile )

public :
  
  /// Constructor
  ViewRendererGL3(QGLWidget * parent=0, const char * name=0);
  
  /// Destructor.
  virtual ~ViewRendererGL3();

  /// Connect this to a statusbar.
  virtual void connectTo(ViewStatusBar3 *);

  /// Connect this to a GL Widget.
  virtual void connectTo(QGLWidget *);

  /// Connect this to a error dialog.
  virtual void connectTo(ViewErrorDialog3 *);

  /// Called when the qglwidget enter in this mode.
  virtual void changeModeEvent(ViewGLFrame3::Mode m);

  /// Paint scene for Selection.
  virtual void selectGL();

  /// End of Selection. Return whether viewer must stay in selection mode (true) or not (false).
  virtual bool endSelect();

  /// end event.
  virtual void endEvent();

  /// Scene change Event.
  virtual bool sceneChangeEvent(ViewSceneChangeEvent3 *);

  /// Add Open Entries
  virtual bool addOpenEntries(QPopupMenu * menu);
  
  /// Add Import Entries
  virtual bool addImportEntries(QPopupMenu * menu);

  /// Add Save Entries
  virtual bool addSaveEntries(QPopupMenu * menu);

  /// Add Export Entries
  virtual bool addExportEntries(QPopupMenu * menu);
  
  /// Add Edit Entries
  virtual bool addEditEntries(QPopupMenu * menu);
  
  /// Add properties info in the \e tab
  virtual bool addProperties(QTabWidget * tab);

  /// Add other menu.
  virtual bool addOtherMenu(QMenuBar * menu);

  /// Add other toolbar.
  virtual bool addOtherToolBar(QMainWindow * menu);

  /// Fill the list for browsing.
  virtual bool browse(QListView *,bool);

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
  virtual void selectionEvent(QListViewItem *);

  virtual void checkFileModification();

signals:

  /// Signal emit when filename has changed.
  void filenameChanged(const QString&);
  
  /// Signal emit when the scene change.
  void sceneChanged();

  /// Signal emit when the renderer ask to change of mode.
  void changeMode(ViewGLFrame3::Mode);

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
      \class ViewSceneRendererGL3
      \brief An abstract class for Display Manager of GL Scene.
      
*/
class VIEW3_API ViewSceneRendererGL3  : public ViewRendererGL3
{
  Q_OBJECT
  Q_PROPERTY( bool LightEnable READ isLightEnable )

public :
  
  /// Constructor.
  ViewSceneRendererGL3(ViewCameraGL3 * camera=0,
		      ViewLightGL3 * light=0,
		      QGLWidget * parent=0, 
		      const char * name=0);

  /// Destructor.
  virtual ~ViewSceneRendererGL3();

  /// Get the camera.
  ViewCameraGL3 *getCamera() const;

  /// Get the light.
  ViewLightGL3 *getLight() const; 

  /// Set the camera.
  void setCamera(ViewCameraGL3 *);

  /// Set the light.
  void setLight(ViewLightGL3 *); 

  /// Return whether Rendering with light is enable.
  bool isLightEnable() const;

public slots:

  /// Set the Light Enable Rendering Mode.
  void setLightEnable();

  virtual void computeCamera();

  virtual void useDisplayList(bool);

signals:

  /// Emit when light enable status change.
  void lightEnableChanged(bool);

protected :

  /// The camera.
  ViewCameraGL3 * __camera;

  /// The light.
  ViewLightGL3 * __light;

  /// Light Enable.
  bool __lightEnable;
};

/* ----------------------------------------------------------------------- */
#endif

