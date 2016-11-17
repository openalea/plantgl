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

/*! \file util_qwidget.h
    \brief Some basic specialisations of Qt classes.
*/

#ifndef _util_qwidget_h__
#define _util_qwidget_h__

#include <QtGlobal>
#if QT_VERSION >= 0x050000 
    #include <QtWidgets/QToolBar>
    #include <QtWidgets/QDialog>
    #include <QtWidgets/QStatusbar>
#else
    #include <QtGui/QToolBar>
    #include <QtGui/QDialog>
    #include <QtGui/QStatusbar>
#endif

#include "../gui_config.h"
class QToolButton;
class QMenu;
class QProgressBar;
class QSize;
class QColor;
class QPushButton;


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
  ViewToolBar( const QString & title, 
			   QWidget * mw, 
			   const char* name = NULL );
  

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

   ViewExclusiveButtonSet --> See QActionGroup

*/

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
	      bool modal=false, 
	      Qt::WindowFlags f=0 );
  
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
				  bool modal=false, 
				  Qt::WindowFlags f=0 );
  
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
   ViewPopupButton --> See QAction
*/

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

PGL_BEGIN_NAMESPACE

VIEW_API void drawArrow(QPixmap *,int s=1);

VIEW_API void fillButton(QPushButton* button, const QColor& color,const QSize& defaultsize = QSize(100,40));

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

