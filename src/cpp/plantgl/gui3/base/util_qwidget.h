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

/*! \file util_qwidget.h
    \brief Some basic specialisations of Qt classes.
*/

#ifndef _util_qwidget_h__
#define _util_qwidget_h__

#include <qtoolbar.h>
#include <qdialog.h>
#include <qstatusbar.h>
#include "../gui3_config.h"
class QToolButton;
class QPopupMenu;
class QProgressBar;


/* ----------------------------------------------------------------------- */

/**   
   \class ViewToolBar3
   \brief A QToolBar that emit a signal when its visibility changed.
*/
/* ----------------------------------------------------------------------- */



class VIEW3_API ViewToolBar3 : public QToolBar {
  Q_OBJECT
  
  public :
    
  /// Constructor
  ViewToolBar3( const QString & label, 
		QMainWindow * mw, 
#if QT_VERSION >= 300
		ToolBarDock = DockTop,
#else
		QMainWindow::ToolBarDock tbd = QMainWindow::Top,
#endif
		bool newLine = FALSE, 
		const char * name = 0 );
  
  /// Constructor
  ViewToolBar3 ( const QString & label, 
		QMainWindow *mw, 
		QWidget * w, 
		bool newLine = FALSE, 
		const char * name = 0, 
		WFlags f = 0 );
  
  /// Constructor
  ViewToolBar3 ( QMainWindow * parent = 0, const char * name = 0 );
  virtual ~ViewToolBar3();

 signals: 
  /// Signal emit when visibility changed.
  void __visibilityChanged(bool);
  
 public slots:
  
  /// Change the visibilty of \e this.
  void changeVisibility();
  
  
 protected:
  
  virtual void showEvent ( QShowEvent *);
  
  virtual void hideEvent ( QHideEvent *);
};


/* ----------------------------------------------------------------------- */

/**   
   \class ViewExclusiveButtonSet3
   \brief A Set of Exclusive QToolButton.
*/
class VIEW3_API ViewExclusiveButtonSet3 :  public QObject {
  Q_OBJECT

public :

  /// Constructor.
  ViewExclusiveButtonSet3(unsigned int size,
		      QToolBar * toolbar,
		      const char * name=0);

  /// Destructor.
  ~ViewExclusiveButtonSet3();
  
  /// Add a ToolButton.
  void add(QToolButton *);
  
public slots:

  /// Selection Button i
  void setSelection(const int i);
  
protected:
  
  QToolButton ** __buttonSet;
  unsigned int __i;
  unsigned int __size;

};

/* ----------------------------------------------------------------------- */

/**   
   \class ViewDialog3
   \brief A QDialog that emit a signal when its visibility changed.
*/
class VIEW3_API ViewDialog3 : public QDialog {
  Q_OBJECT

public:

  /// Constructor.
  ViewDialog3( QWidget * parent=0, 
	      const char * name=0, 
	      bool modal=FALSE, 
	      WFlags f=0 );
  
  /// Destructor.
  ~ViewDialog3();

public slots:

  /// Show dialog.
  virtual void show();
  
  signals:

  /// Emit when visibility of this changed.
  void visibilityChanged(bool);

protected:
  
  /// By default when Key Escape is pressed this is hidden.
  virtual void keyPressEvent (QKeyEvent * e);
  
  virtual void hideEvent (QHideEvent * event);

  virtual void showEvent (QShowEvent * event);
  
};

class ViewMainDialog3 : public ViewDialog3 {

public:

  /// Constructor.
  ViewMainDialog3( QWidget * parent=0,			  
				  const char * name=0, 
				  bool modal=FALSE, 
				  WFlags f=0 );
  
  /// Destructor.
  ~ViewMainDialog3();

  void setMainWidget(QWidget * mainwidget);
  
protected :

  virtual void resizeEvent(QResizeEvent * e);

  QWidget * __mainwidget;
};
/* ----------------------------------------------------------------------- */

/**   
   \class ViewPopupButton3
   \brief A PopupButton that can be checked using slots check.
*/
class VIEW3_API ViewPopupButton3 : public QObject {
  Q_OBJECT

public :

  /// Constructor
  ViewPopupButton3(QPopupMenu * parent, int id, char * name = NULL);

  /// Destructor
  ~ViewPopupButton3();

 public slots:

 /// (un)check the button
 void check(bool);

protected :
  
  int __id;

  QPopupMenu * __menu;

};

/* ----------------------------------------------------------------------- */

class ViewStatusBar3 : public QStatusBar {
	Q_OBJECT
public:
	ViewStatusBar3(QWidget * parent = 0, const char * name = 0 );
	virtual ~ViewStatusBar3();
	QProgressBar * progressBar();
public slots:
	void setProgress(int progress,int total);
	void setProgress(int progress);
	void setTotalSteps(int total);
protected:
	QProgressBar * __progress;
};


/* ----------------------------------------------------------------------- */

VIEW3_API void drawArrow3(const QPixmap *,int s=1);

/* ----------------------------------------------------------------------- */
#endif

