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
/*! \file view_info.h
    \brief Definition of the viewer class ViewSysInfo.
*/

#ifndef __view_info_h__
#define __view_info_h__

/* ----------------------------------------------------------------------- */

#include "../gui_config.h"

#include <QtGlobal>
#include <QtCore/qvariant.h>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/QDialog>
#else
    #include <QtGui/QDialog>
#endif

/* ----------------------------------------------------------------------- */

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QTreeWidget;
class QTreeWidgetItem;
class QPushButton;
class QGLWidget;

/* ----------------------------------------------------------------------- */

/**
    \class ViewSysInfo
    \brief A Dialog to display information of the system such as driver use by openGL.
    Must be build with a QGLWidget to obtain all the information.
*/

/* ----------------------------------------------------------------------- */

class VIEW_API ViewSysInfo : public QDialog
{
  Q_OBJECT

public:

  /*! Constructor
   *  Constructs a ViewSysInfo which is a child of 'parent', with the
   *  name 'name' and widget flags set to 'f'
   *
   *  The dialog will by default be modeless, unless you set 'modal' to
   *  TRUE to construct a modal dialog.
   */
  ViewSysInfo( QWidget* parent = 0,
             QGLWidget * FrameGL = 0,
             const char* name = 0,
             bool modal = false,
             Qt::WindowFlags fl = 0 );

  /*!  Destructor.
   *  Destroys the object and frees any allocated resources
   */
  ~ViewSysInfo();

  QTreeWidgetItem* addItem(const QString& name = QString::null, const QString& val = QString::null);

  QTreeWidgetItem* addItem(const QPixmap& pix, const QString& name = QString::null, const QString& val = QString::null);

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
  QTreeWidget * AttView;

  /// Set of Attributes of the system.
  QTreeWidgetItem* RootItem;

  /// Last Attribut of the system.
  QTreeWidgetItem* LastItem;

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
