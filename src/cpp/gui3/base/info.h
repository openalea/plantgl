/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
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
/*! \file view_info.h
    \brief Definition of the viewer class ViewSysInfo3.
*/

#ifndef __view_info_h__
#define __view_info_h__

/* ----------------------------------------------------------------------- */

#include "../gui3_config.h"

#include <qvariant.h>
#include <qdialog.h>

/* ----------------------------------------------------------------------- */

class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;
class QListView;
class QListViewItem;
class QPushButton;
class QGLWidget;
class QListViewItem;

/* ----------------------------------------------------------------------- */

/** 
    \class ViewSysInfo3
    \brief A Dialog to display information of the system such as driver use by openGL.
    Must be build with a QGLWidget to obtain all the information.
*/

/* ----------------------------------------------------------------------- */

class VIEW3_API ViewSysInfo3 : public QDialog
{ 
  Q_OBJECT

public:

  /*! Constructor 
   *  Constructs a ViewSysInfo3 which is a child of 'parent', with the 
   *  name 'name' and widget flags set to 'f' 
   *
   *  The dialog will by default be modeless, unless you set 'modal' to
   *  TRUE to construct a modal dialog.
   */
  ViewSysInfo3( QWidget* parent = 0,
		     QGLWidget * FrameGL = 0,
		     const char* name = 0,	      
		     bool modal = FALSE, 
		     WFlags fl = 0 );

  /*!  Destructor.
   *  Destroys the object and frees any allocated resources
   */
  ~ViewSysInfo3();

  QListViewItem* addItem(const QString& name = QString::null, const QString& val = QString::null);

  QListViewItem* addItem(const QPixmap& pix, const QString& name = QString::null, const QString& val = QString::null);

  static const char * tools_logo[34];

  static const char * qt_logo[24];

  static const char * info_logo[24];

protected slots:

	void saveAsFile() ;

protected:

	void saveAsFile(const QString&) const;

	/// Cancel Button.
  QPushButton* CancelButton;

  /// Ok Button.
  QPushButton* OkButton;

  /// List of Attributes of the system.
  QListView* AttView;

  /// Set of Attributes of the system.
  QListViewItem* RootItem;

  /// Last Attribut of the system.
  QListViewItem* LastItem;

  /// Label of the Dialog.
  QLabel* Title;

  /// Icon of the Dialog.
  QLabel* Icon;

  /*! Main event handler.
   *  Reimplemented to handle application font changes.
   */
  bool event( QEvent* );

    
  /*! Resize event handler.
   *  Reimplemented to move objects of the dialog when resizing.
   */
  virtual void resizeEvent(QResizeEvent*);
};

#endif // _GENERAL_INFORMATION_H_
