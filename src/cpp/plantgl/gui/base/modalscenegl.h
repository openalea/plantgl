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


/*! \file view_modalscenegl.h
    \brief Definition of the viewer class ViewModalRendererGL.
*/

#ifndef __view_modalscenegl_h__
#define __view_modalscenegl_h__

/* ----------------------------------------------------------------------- */

#include "scenegl.h"

class ViewRenderingModeActions;

/* ----------------------------------------------------------------------- */

/**
      \class ViewModalRendererGL
      \brief A GL Display Manager for Scene with 3 Rendering mode : Normal, Wire, Skeleton
      and 2 options : Bounding Box and Control Points.
*/

/* ----------------------------------------------------------------------- */

class VIEW_API ViewModalRendererGL  : public ViewSceneRendererGL
{
  Q_OBJECT
  Q_PROPERTY( int RenderingMode READ getRenderingMode)
  Q_PROPERTY( bool BBoxRendering READ isBBoxRenderingEnable)
  Q_PROPERTY( bool CtrlPointRendering READ isCtrlPointRenderingEnable)

    public :

  /// Constructor.
    ViewModalRendererGL(ViewCameraGL * camera=0,
            ViewLightGL * light=0,
            QGLWidget * parent=0,
            const char * name=0);

  /// Destructor.
  virtual ~ViewModalRendererGL();

    /// Get the Rendering Mode
  int getRenderingMode() const;

  /// Return whether BBox Rendering is enable.
  bool isBBoxRenderingEnable() const ;

  /// Return whether Control Points Rendering is enable.
  bool isCtrlPointRenderingEnable() const;

  /// Create a Popup menu that reflect the functionality of this.
  virtual QMenu * createToolsMenu(QWidget * parent);

  /// Fill the tool Bar.
  void fillToolBar(QToolBar * toolBar);

  /// Fill the tool Bar.
  void fillToolBarAdditionnal(QToolBar * toolBar);

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

  ViewRenderingModeActions * __actions;

};

/* ----------------------------------------------------------------------- */
#endif

