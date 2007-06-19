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

/*! \file view_modalscenegl.h
    \brief Definition of the viewer class ViewModalRendererGL3.
*/

#ifndef __view_modalscenegl_h__
#define __view_modalscenegl_h__

/* ----------------------------------------------------------------------- */

#include "scenegl.h"

/* ----------------------------------------------------------------------- */

/**   
      \class ViewModalRendererGL3
      \brief A GL Display Manager for Scene with 3 Rendering mode : Normal, Wire, Skeleton
      and 2 options : Bounding Box and Control Points.      
*/

/* ----------------------------------------------------------------------- */

class VIEW3_API ViewModalRendererGL3  : public ViewSceneRendererGL3
{
  Q_OBJECT
  Q_PROPERTY( int RenderingMode READ getRenderingMode)
  Q_PROPERTY( bool BBoxRendering READ isBBoxRenderingEnable)
  Q_PROPERTY( bool CtrlPointRendering READ isCtrlPointRenderingEnable)

    public :
  
  /// Constructor.
    ViewModalRendererGL3(ViewCameraGL3 * camera=0,
			ViewLightGL3 * light=0,
			QGLWidget * parent=0, 
			const char * name=0);
  
  /// Destructor.
  virtual ~ViewModalRendererGL3();

    /// Get the Rendering Mode
  int getRenderingMode() const;

  /// Return whether BBox Rendering is enable.
  bool isBBoxRenderingEnable() const ;

  /// Return whether Control Points Rendering is enable.
  bool isCtrlPointRenderingEnable() const;

  /// Create a Popup menu that reflect the functionality of this.
  virtual QPopupMenu * createToolsMenu(QWidget * parent);

  /// Fill the tool Bar.
  void fillToolBar(QToolBar * toolBar);

  

public slots:

  /// Set the Volume Rendering Mode.
  void setRenderVolume();
  /// Set the Wire Rendering Mode
  void setRenderWire();
  /// Set the Volume and Wire Rendering Mode
  void setRenderVolumenWire();
  /// Set the Skeleton Rendering Mode.
  void setRenderSkeleton();
  /// Set the CtrlPoint Rendering Mode.
  void setRenderCtrlPoint();
  /// Set the  Rendering Mode.
  void setRenderBBox();
  
 signals:

  /// Emit when bbox rendering mode change.
  void bboxRenderingChanged(bool);
  /// Emit when Control Points rendering mode change.
  void ctrlPointsRenderingChanged(bool);
  /// Emit when rendering mode change.
  void renderingModeChanged(const int);

protected :

  /// Current Render mode : 1->volume, 2->wire, 3->Skeleton, 4-> volume n wire
  int __renderingMode; 

  /// Rendering Option : [0] : BoundingBox  -  [1] : Control Point.
  bool __renderingOption[2];


};

/* ----------------------------------------------------------------------- */
#endif

