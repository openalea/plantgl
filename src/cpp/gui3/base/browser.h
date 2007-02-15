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

/*! \file view_browser.h
    \brief Definition of the viewer class ViewBrowser.
*/

#ifndef __view_browser_h__
#define __view_browser_h__

/* ----------------------------------------------------------------------- */

#include "util_qwidget.h"
class QBrowser;

/* ----------------------------------------------------------------------- */

class ViewRendererGL;

/* ----------------------------------------------------------------------- */


/**   
   \class ViewBrowser
   \brief The Browser of the viewer
*/

class VIEW_API ViewBrowser : public ViewDialog
{
    Q_OBJECT
	public:
    
    /// Constructor
    ViewBrowser(  QWidget * parent=0, 
		  const char * name=0, 
		  bool modal=FALSE);

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
  
  /// Function called when the widget is resized.
  virtual void resizeEvent( QResizeEvent * event); 
  
  /// This event handler is called when a key is pressed for this widget. 
  virtual void keyPressEvent (QKeyEvent * e);
  
private:
  

  /// The GeomListViewBuilder Object
  QBrowser * __browser;
  
  /// A Pointee to the Scene.
  ViewRendererGL  * __scene;
  
  /// Scene empty.
  bool __empty;
   
};


/* ----------------------------------------------------------------------- */

// __view_geombrowser_h__
/* ----------------------------------------------------------------------- */
#endif

