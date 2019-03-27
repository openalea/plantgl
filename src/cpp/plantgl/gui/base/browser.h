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


/*! \file view_browser.h
    \brief Definition of the viewer class ViewBrowser.
*/

#ifndef __view_browser_h__
#define __view_browser_h__

/* ----------------------------------------------------------------------- */

#include "../gui_config.h"
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/QDockWidget>
#else
    #include <QtGui/QDockWidget>
#endif

namespace Ui {  class QBrowser; }

/* ----------------------------------------------------------------------- */

class ViewRendererGL;
class QStandardItemModel;

/* ----------------------------------------------------------------------- */


/**
   \class ViewBrowser
   \brief The Browser of the viewer
*/

class VIEW_API ViewBrowser : public QDockWidget
{
    Q_OBJECT
    public:

    /// Constructor
    ViewBrowser(  QWidget * parent=0, const QString& title = "");

    /// Destructor
    ~ViewBrowser();


  void setRenderer(ViewRendererGL *);

 public slots:

  /// Clear.
  void clear();

  /// Refresh.
  void refresh(bool b = true);

  void setSelection(const QString&);

protected:

  /// Function called when the widget is shown.
  virtual void showEvent ( QShowEvent * event );

  /// This event handler is called when a key is pressed for this widget.
  virtual void keyPressEvent (QKeyEvent * e);

private:


  /// The GeomListViewBuilder Object
  Ui::QBrowser * __browser;

  /// A Pointee to the Scene.
  ViewRendererGL  * __scene;

  /// Scene empty.
  bool __empty;


};


/* ----------------------------------------------------------------------- */

// __view_geombrowser_h__
/* ----------------------------------------------------------------------- */
#endif

