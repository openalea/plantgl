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

  QTreeWidgetItem* addItem(const QString& name = QString(), const QString& val = QString());

  QTreeWidgetItem* addItem(const QPixmap& pix, const QString& name = QString(), const QString& val = QString());

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
