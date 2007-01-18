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
 *                      GNU General Public license
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

/*! \file view_helpmenu.h
    \brief Definition of the viewer class ViewHelpMenu.
*/

#ifndef __view_helpmenu_h__
#define __view_helpmenu_h__

/* ----------------------------------------------------------------------- */

#include "Tools/config.h"
#include <qpopupmenu.h>
#include <qdialog.h>
#include <qtimer.h>
#include <vector>
class QGLWidget;
class ViewAboutDialog;
class QLabel;

/* ----------------------------------------------------------------------- */

/**   
   \class ViewHelpMenu
   \brief A Class who control Popup Menu relative to Help.
   Display Help Information.
*/

/* ----------------------------------------------------------------------- */

class GEOM_API ViewHelpMenu : public QPopupMenu 
{
  Q_OBJECT
  Q_PROPERTY(int AppStyle READ getStyle WRITE setStyle );
      public:



  /// Constructor
  ViewHelpMenu(QWidget * parent=0,
	   QGLWidget * glwidget = 0,
	   const char * name=0);
  
  /// Destructor
  ~ViewHelpMenu();
  
  int getStyle() const;

  void setGLWidget(QGLWidget * glwidget);

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

  void setStyle(int);

  void setInitText(const QString&);
  void setInitText(const QString&,int timeout);


protected :
  
  void checkItem(int i);
  
  QPopupMenu * __style;
  std::vector<int>  __ids;

  QGLWidget * __glwidget;
  ViewAboutDialog * __about;

};

class ViewAboutDialog : public QDialog {
  Q_OBJECT
public :
  ViewAboutDialog ( QWidget * parent=0, 
					const char * name=0, 
					int timeout=-1,
					bool modal=FALSE);

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

