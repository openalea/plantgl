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

 /*! \file view_modalwidget.h
    \brief Definition of the viewer class ViewRenderingModeMenu .
*/
			
#ifndef __view_modalwidget_h__
#define __view_modalwidget_h__


/* ----------------------------------------------------------------------- */

#include <qaction.h>
#include "../gui_config.h"

/* ----------------------------------------------------------------------- */

class QMenu;
class QToolBar;
class ViewModalRendererGL;

/// Menu to control ViewModalRendererGL
class VIEW_API ViewRenderingModeActions : public QObject {
   Q_OBJECT
 
public :
  
   ViewRenderingModeActions(ViewModalRendererGL * renderer, const char * name=0);
   
   ~ViewRenderingModeActions();

   void fill(QMenu * menu) const;
   void fill(QToolBar * menu) const;

public slots:

  /// Set Rendering Mode.
  void setRenderingMode(const int);

protected :
  QAction * idVolume;
  QAction * idWire;
  QAction * idSkeleton;
  QAction * idVolWire;

  QAction * idCtrlPoints;
  QAction * idBBox;
};

/* ----------------------------------------------------------------------- */
#endif

