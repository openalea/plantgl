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


/*! \file util_qwidget.h
    \brief Some basic specialisations of Qt classes.
*/

#ifndef _util_qwidget_h__
#define _util_qwidget_h__

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/QToolBar>
    #include <QtWidgets/QDialog>
    #include <QtWidgets/QStatusBar>
#else
    #include <QtGui/QToolBar>
    #include <QtGui/QDialog>
    #include <QtGui/QStatusBar>
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
          Qt::WindowFlags f=Qt::WindowFlags() );

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
                  Qt::WindowFlags f=Qt::WindowFlags() );

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

