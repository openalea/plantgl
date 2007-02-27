/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): C. Nouguier & F. Boudon
 *                       N. Dones & B. Adam
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

/*! \file view_filemanager.h
    \brief Definition of the viewer class ViewFileManager3.
*/

#ifndef __view_filemanager_h__
#define __view_filemanager_h__

/* ----------------------------------------------------------------------- */

#include <qmainwindow.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qstring.h>
#include <qurloperator.h>

#include <deque>
#include "../gui3_config.h"

/* ----------------------------------------------------------------------- */

class QComboBox;

/* ----------------------------------------------------------------------- */

class ViewGLFrame3;
class ViewLocationBar3;
class ViewHelpMenu3;
class ViewErrorDialog3;
class ViewControlPanel3;
#include "util_qwidget.h"

/* ----------------------------------------------------------------------- */

/**   
   \class ViewFileManager3
   \brief A Class who control Popup Menu and Tool Bar relative to file.
   Allow other class to choose file.
*/

/* ----------------------------------------------------------------------- */

class VIEW3_API ViewFileManager3 : public QPopupMenu 
{
  Q_OBJECT

      public:

  /// The \b size of the open file review.
  static const uint REVIEW_SIZE;

  static QString FILE_CONFIG;
  static float CONFIG_VERSION;

  /// Constructor
  ViewFileManager3( QMainWindow * parent=0, 
	       const char * name=0, 
	       ViewGLFrame3 * frame=0,
		   ViewHelpMenu3 * helpmenu=0,
		   ViewControlPanel3 * controlpanel=0);
    
  /// Destructor
  ~ViewFileManager3();
  
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
  /// Add a preambule to automatically generate file.
//  static void preambule(ostream & _ostream, const char * filename = 0, 
//			const char * begcomment = "(#" , const char * endcomment = "#)" );

  /// Fill the Tool Bar.
  void fillToolBar(QToolBar *);

  /// Get the Location toolBar
  ViewToolBar3 * getLocationBar();

  QString getInitFilename();

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

  /// Find a correct file name and correct extension and ask ViewGLFrame3 to save current scene as bitmap.
  void saveImage();
  /// Find a correct file name and correct extension and ask ViewGLFrame3 to save current scene as bitmap.
  void saveImageWithAlpha();

  /// Find an existant geom or smb file name.
  void openFile();

  /// Open the last file of the review.
  void reOpenFile();

  /// Open the i-th file of the review.
  void reOpenFile( int i );

  /// Find the format of the file \b _filename.
  void openFile(const QString&);

  /// Properties of the last file.
  void properties();
  void configuration();

  /// Refresh.
  void refresh();

  /// load current viewer configuration.
  void loadConfig();

  /// Save current viewer configuration.
  void saveConfig();

  /// Save configuration and review in viewer configuration file.    
  void saveConfigBeforeQuit();

  void downloaded( const QByteArray &, QNetworkOperation * op  );
  void connectionStatus ( int , const QString & );
  void net( QNetworkOperation *  );
  void transferProgress ( int, int, QNetworkOperation * );
//  void crop();

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
  void connectTo(ViewErrorDialog3 *);

  void saveToolbarState(bool);
  bool getToolbarStateSave() const;
  void saveWinPos(bool);
  bool getSaveWinPos() const;
  void saveStyle(bool);
  bool getSaveStyle() const;
  void saveHistory(bool);
  bool getSaveHistory() const;
  void saveBgColor(bool);
  bool getSaveBgColor() const;
  void saveGridVisibility(bool);
  bool getSaveGridVisibility() const;
  void saveCameraReDim(bool b);
  bool getSaveCameraReDim() const;
  void saveUseSpinBox(bool b);
  bool getSaveUseSpinBox() const;
  void showInit(bool b);
  bool getShowInit() const;
  void saveOption(int,bool);
  bool getSaveOption(int) const;


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
  QPopupMenu * __OpenFileMenu;
  
  /// Import File Sub Menu.
  QPopupMenu * __ImportFileMenu;
  
  /// Save File Sub Menu.
  QPopupMenu * __SaveFileMenu;
  
  /// Export File Sub Menu.
  QPopupMenu * __ExportFileMenu;
  
  /// Recent File Sub Menu.
  QPopupMenu * __RecentFilesMenu;
  
  /// last open files Review.
  std::deque<QString> __lastOpenFiles;

  /// is a file as been open in this session.
  bool __hasOpenFile;
    
  /// A pointer to the ViewGLFrame3.
  ViewGLFrame3 * __GLFrame;
  ViewControlPanel3 * __controlPanel;

  ViewLocationBar3 * __locatToolBar;
  ViewHelpMenu3 * __helpmenu;
  QString __pictureDir;
  QUrlOperator __url;
  QString __downloadingfile;

  int __saveoptions;
};


/* ----------------------------------------------------------------------- */

// __view_filemanager_h__
#endif

