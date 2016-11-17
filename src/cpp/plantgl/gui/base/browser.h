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

/*! \file view_browser.h
    \brief Definition of the viewer class ViewBrowser.
*/

#ifndef __view_browser_h__
#define __view_browser_h__

/* ----------------------------------------------------------------------- */

#include "../gui_config.h"
#include <QtGlobal>
#if QT_VERSION >= 0x050000 
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

