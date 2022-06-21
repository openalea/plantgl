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


/*! \file view_helpmenu.h
    \brief Definition of the viewer class ViewHelpMenu.
*/

#ifndef __view_helpmenu_h__
#define __view_helpmenu_h__

/* ----------------------------------------------------------------------- */

#include "../gui_config.h"
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qmenu.h>
    #include <QtWidgets/qdialog.h>
#else
    #include <QtGui/qmenu.h>
    #include <QtGui/qdialog.h>
#endif
#include <QtCore/qtimer.h>
#include <vector>
class QOpenGLWidget;
class ViewAboutDialog;
class QLabel;

/* ----------------------------------------------------------------------- */

/**
   \class ViewHelpMenu
   \brief A Class who control Popup Menu relative to Help.
   Display Help Information.
*/

/* ----------------------------------------------------------------------- */

class VIEW_API ViewHelpMenu : public QMenu
{
  Q_OBJECT
  Q_PROPERTY(int AppStyle READ getStyle WRITE setStyle );
      public:



  /// Constructor
  ViewHelpMenu(QWidget * parent=0,
       QOpenGLWidget * glwidget = 0,
       const char * name=0);

  /// Destructor
  ~ViewHelpMenu();

  int getStyle() const;
  int getStyleId(const QString& name) const;
  QString getStyleName() const;

  void setGLWidget(QOpenGLWidget * glwidget);

public slots:

  /// Display About Viewer Information
  void showAbout();

  /// Display About Viewer Information
  void showInit();

  /// Display About Viewer Information
  void showLicense();

  /// Display  Help on use of the Viewer
  void showHelp();

  /// Display About Qt Information
  void aboutQt();

  /// Display Informations about Librairies used.
  void generalInfo();

  /// Display Informations about qt objects used.
  void qtbrowse();

  void setStyle(const QString& name);
  void setStyle(int);

  void setInitText(const QString&);
  void setInitText(const QString&,int timeout);

  void endEvent();

  protected slots:
  void setStyleCorrespondingTo(QAction *);

protected :

  void checkItem(int i);

  QMenu * __style;
  std::vector<QAction *>  __ids;

  QOpenGLWidget * __glwidget;
  ViewAboutDialog * __about;

};

class ViewAboutDialog : public QDialog {
  Q_OBJECT
public :
  ViewAboutDialog ( QWidget * parent=0,
                    const char * name=0,
                    int timeout=-1,
                    bool modal=false);

     ~ViewAboutDialog();

     void display(int timeout);
     bool useNewStyle() const { return __style; }
signals:
     void licenseView();
public slots:
     void setText(const QString&);
     void setText(const QString&,int timeout);
protected :

     void clear();

     void setIconPixmap( const QPixmap & icon);

     void setInfo(const QString& text, QRect d, int size);

     virtual void mousePressEvent ( QMouseEvent * ) ;
     virtual void mouseMoveEvent ( QMouseEvent * );
     virtual void showEvent ( QShowEvent * );
     virtual void hideEvent ( QHideEvent * );
     virtual bool eventFilter( QObject *o, QEvent *e );
     void changeLogo(const QPoint& p);

     QLabel * __text;
     QTimer __timer;

     bool __style;
     QPixmap __logo;
     QPixmap __logo2;
     bool __license;
     QRect __licenseRect;

};
/* ----------------------------------------------------------------------- */

// __view_helpmenu_h__
#endif

