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

/*! \file view_filemanager.h
    \brief Definition of the viewer class ViewFileManager.
*/

#ifndef __view_filemanager_h__
#define __view_filemanager_h__

/* ----------------------------------------------------------------------- */
#include <QtGlobal>
#if QT_VERSION >= 0x050000 
    #include <QtWidgets/qmainwindow.h>
    #include <QtWidgets/qmenu.h>
    #include <QtWidgets/qtoolbar.h>
#else
    #include <QtGui/qmainwindow.h>
    #include <QtGui/qmenu.h>
    #include <QtGui/qtoolbar.h>
#endif

#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>

#include "../gui_config.h"

/* ----------------------------------------------------------------------- */

class QComboBox;

/* ----------------------------------------------------------------------- */

class ViewGLFrame;
class ViewLocationBar;
class ViewHelpMenu;
class ViewErrorDialog;
class ViewControlPanel;
#include "util_qwidget.h"

/* ----------------------------------------------------------------------- */

/**   
   \class ViewFileManager
   \brief A Class who control Popup Menu and Tool Bar relative to file.
   Allow other class to choose file.
*/

/* ----------------------------------------------------------------------- */

class VIEW_API ViewFileManager : public QMenu 
{
  Q_OBJECT
  Q_PROPERTY(QStringList RecentFiles READ getRecentFiles WRITE setRecentFiles );


      public:

  /// The \b size of the open file review.
  static const uint REVIEW_SIZE;

  /// Constructor
  ViewFileManager( QMainWindow * parent=0, 
	       const char * name=0, 
	       ViewGLFrame * frame=0,
		   ViewHelpMenu * helpmenu=0,
		   ViewControlPanel * controlpanel=0);
    
  /// Destructor
  ~ViewFileManager();
  
  static QString getSaveFileName ( const QString& initial, 
				   const QString& ext, 
				   const QString& filter, 
				   QWidget* parent,
				   const QString& caption = "Save");

  /// return the review of open file.
  const QString& getLastOpenFiles(int i) const;

  /// return the name of the last open file.
  const QString getLastOpenFile() const;
  
  /// return whether a file has been open or not in this session.
  const bool hasOpenFile() const;

  /// set/get the history of open files.
  const QStringList& getRecentFiles() const;
  void setRecentFiles(const QStringList&);


  /// Fill the Tool Bar.
  void fillToolBar(QToolBar *);

  /// Get the Location toolBar
  ViewToolBar * getLocationBar();

  void saveImage(bool withAlpha);


public slots:

  void initialize();
      

  /// add file in the review.
  void addOpenFile(const QString&);

  /// remove file in the review.
  void removeOpenFile(const QString&);

  /// clear the open file review
  bool clearReview();
  bool clearTmpFiles();

  /// Draw The Open File Menu.
  void drawRecentFilesMenu();

  /// Draw The Location Bar.
  void drawLocationBar(bool b = false);

  /// Find a correct file name and correct extension and ask ViewGLFrame to save current scene as bitmap.
  void saveImage();
  /// Find a correct file name and correct extension and ask ViewGLFrame to save current scene as bitmap.
  void saveImageWithAlpha();

  /// Find an existant geom or smb file name.
  void openFile();

  /// Open the last file of the review.
  void reOpenFile();

  /// Open the i-th file of the review.
  void reOpenFile( QAction * action );

  /// Open the i-th file of the review.
  void reOpenFile( int i );

  /// Find the format of the file \b _filename.
  void openFile(const QString&);

  /// Properties of the last file.
  void properties();
  void configuration();

  /// Refresh.
  void refresh();


  /// Display error message.
  void error(const QString&);
  /// Display warning message.
  void warning(const QString&);
  /// Display info message.
  void info(const QString&);
  /// Display status message.
  void status(const QString&);
  /// Display status message for int millisecond.
  void status(const QString&,int);

  void connectTo(QStatusBar *);
  void connectTo(ViewErrorDialog *);

signals:
  /// Signal emit when some error where found.
  void errorMessage(const QString&);

  /// Signal emit when some warning where found.
  void warningMessage(const QString&);

  /// Signal emit when some info where found.
  void infoMessage(const QString&);

  /// Status message.
  void statusMessage(const QString&);

  /// Status message.
  void statusMessage(const QString&,int);

  private:

  /// Open File Sub Menu.
  QMenu * __OpenFileMenu;
  
  /// Import File Sub Menu.
  QMenu * __ImportFileMenu;
  
  /// Save File Sub Menu.
  QMenu * __SaveFileMenu;
  
  /// Export File Sub Menu.
  QMenu * __ExportFileMenu;
  
  /// Recent File Sub Menu.
  QMenu * __RecentFilesMenu;
  
  /// last open files Review.
  QStringList __lastOpenFiles;

  /// is a file as been open in this session.
  bool __hasOpenFile;
    
  /// A pointer to the ViewGLFrame.
  ViewGLFrame * __GLFrame;
  ViewControlPanel * __controlPanel;

  ViewLocationBar * __locatToolBar;
  ViewHelpMenu * __helpmenu;
  QString __pictureDir;

};


/* ----------------------------------------------------------------------- */

// __view_filemanager_h__
#endif

