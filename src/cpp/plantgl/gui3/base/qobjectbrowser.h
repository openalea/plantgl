/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): F. Boudon
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
		
/*! \file view_qobjectbrowser.h
    \brief Definition of the viewer class ViewQObjectBrowser3.
*/


#ifndef __view_qobjectbrowser_h__
#define __view_qobjectbrowser_h__

/* ----------------------------------------------------------------------- */

#include "util_qwidget.h"
class QBrowser;

/* ----------------------------------------------------------------------- */



/**   
   \class ViewQObjectBrowser3
   \brief The Browser of the QObject of the viewer
*/

class VIEW3_API ViewQObjectBrowser3 : public ViewDialog3
{
    Q_OBJECT
	public:
    
    /// Constructor
    ViewQObjectBrowser3(  QWidget * parent=0, 
		  const char * name=0, 
		  bool modal=FALSE);

    /// Destructor
    ~ViewQObjectBrowser3();



protected:
  
  /// Function called when the widget is resized.
  virtual void resizeEvent( QResizeEvent * event); 
  
  /// This event handler is called when a key is pressed for this widget. 
  virtual void keyPressEvent (QKeyEvent * e);
  
private:
  

  /// The GeomListViewBuilder3 Object
  QBrowser * __browser;
  
   
};


/* ----------------------------------------------------------------------- */

// __viewqobjectbrowser_h__
/* ----------------------------------------------------------------------- */
#endif

