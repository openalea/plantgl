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

 /*! \file view_modalwidget.h
    \brief Definition of the viewer class ViewRenderingModeMenu .
*/
			
#ifndef __view_modalwidget_h__
#define __view_modalwidget_h__


/* ----------------------------------------------------------------------- */

#include <qpopupmenu.h>
#include "Tools/config.h"

/* ----------------------------------------------------------------------- */

class ViewModalRendererGL;

/// Menu to control ViewModalRendererGL
class GEOM_API ViewRenderingModeMenu : public QPopupMenu {
   Q_OBJECT
 
public :
  
     ViewRenderingModeMenu(ViewModalRendererGL * renderer, 
		      QWidget * parent=0, 
		      const char * name=0);
   
   ~ViewRenderingModeMenu();
  
public slots:

  /// Set Rendering Mode.
  void setRenderingMode(const int);
  /// Set the CtrlPoint Rendering Mode.
  void setRenderCtrlPoint(bool);
  /// Set the Light Enable Rendering Mode.
  void setLightEnable(bool);
  /// Set the BBox  Rendering Mode.
  void setRenderBBox(bool);

protected :
  int idVolume;
  int idWire;
  int idSkeleton;
  int idVolWire;
  int idCtrlPoints;
  int idBBox;
  int idLight;
};

/* ----------------------------------------------------------------------- */
#endif

