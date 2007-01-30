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
#include "../gui_config.h"
class QToolButton;
class QPopupMenu;
class QProgressBar;


/* ----------------------------------------------------------------------- */

/**   
   \class ViewToolBar
   \brief A QToolBar that emit a signal when its visibility changed.
*/
/* ----------------------------------------------------------------------- */



class VIEW_API ViewToolBar : public QToolBar {
  Q_OBJECT
  
  public :
    
  /// Constructor
  ViewToolBar( const QString & label, 
		QMainWindow * mw, 
#if QT_VERSION >= 300
		ToolBarDock = DockTop,
#else
		QMainWindow::ToolBarDock tbd = QMainWindow::Top,
#endif
		bool newLine = FALSE, 
		const char * name = 0 );
  
  /// Constructor
  ViewToolBar ( const QString & label, 
		QMainWindow *mw, 
		QWidget * w, 
		bool newLine = FALSE, 
		const char * name = 0, 
		WFlags f = 0 );
  
  /// Constructor
  ViewToolBar ( QMainWindow * parent = 0, const char * name = 0 );
  virtual ~ViewToolBar();

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
   \class ViewExclusiveButtonSet
   \brief A Set of Exclusive QToolButton.
*/
class VIEW_API ViewExclusiveButtonSet :  public QObject {
  Q_OBJECT

public :

  /// Constructor.
  ViewExclusiveButtonSet(unsigned int size,
		      QToolBar * toolbar,
		      const char * name=0);

  /// Destructor.
  ~ViewExclusiveButtonSet();
  
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
   \class ViewDialog
   \brief A QDialog that emit a signal when its visibility changed.
*/
class VIEW_API ViewDialog : public QDialog {
  Q_OBJECT

public:

  /// Constructor.
  ViewDialog( QWidget * parent=0, 
	      const char * name=0, 
	      bool modal=FALSE, 
	      WFlags f=0 );
  
  /// Destructor.
  ~ViewDialog();

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

class ViewMainDialog : public ViewDialog {

public:

  /// Constructor.
  ViewMainDialog( QWidget * parent=0,			  
				  const char * name=0, 
				  bool modal=FALSE, 
				  WFlags f=0 );
  
  /// Destructor.
  ~ViewMainDialog();

  void setMainWidget(QWidget * mainwidget);
  
protected :

  virtual void resizeEvent(QResizeEvent * e);

  QWidget * __mainwidget;
};
/* ----------------------------------------------------------------------- */

/**   
   \class ViewPopupButton
   \brief A PopupButton that can be checked using slots check.
*/
class VIEW_API ViewPopupButton : public QObject {
  Q_OBJECT

public :

  /// Constructor
  ViewPopupButton(QPopupMenu * parent, int id, char * name = NULL);

  /// Destructor
  ~ViewPopupButton();

 public slots:

 /// (un)check the button
 void check(bool);

protected :
  
  int __id;

  QPopupMenu * __menu;

};

/* ----------------------------------------------------------------------- */

class ViewStatusBar : public QStatusBar {
	Q_OBJECT
public:
	ViewStatusBar(QWidget * parent = 0, const char * name = 0 );
	virtual ~ViewStatusBar();
	QProgressBar * progressBar();
public slots:
	void setProgress(int progress,int total);
	void setProgress(int progress);
	void setTotalSteps(int total);
protected:
	QProgressBar * __progress;
};


/* ----------------------------------------------------------------------- */

VIEW_API void drawArrow(const QPixmap *,int s=1);

/* ----------------------------------------------------------------------- */
#endif

