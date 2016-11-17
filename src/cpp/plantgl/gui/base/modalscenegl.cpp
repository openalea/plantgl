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

#include "modalscenegl.h"
#include "modalwidget.h"
#include "util_qwidget.h"
#include "icons.h"

#include <QtCore/qvariant.h>
#if QT_VERSION >= 0x050000 
    #include <QtWidgets/qmenu.h>
    #include <QtWidgets/qtoolbutton.h>
    #include <QtWidgets/qwhatsthis.h>
#else
    #include <QtGui/qmenu.h>
    #include <QtGui/qtoolbutton.h>
    #include <QtGui/qwhatsthis.h>
#endif

/* ----------------------------------------------------------------------- */

ViewModalRendererGL::ViewModalRendererGL(ViewCameraGL * camera,
					 ViewLightGL * light,
					 QGLWidget * parent, 
					 const char * name) :
  ViewSceneRendererGL(camera,light,parent,name),
  __renderingMode(1),
  __actions(0)
{
  __renderingOption[0] = false;
  __renderingOption[1] = false;
}


ViewModalRendererGL::~ViewModalRendererGL()
{
}

/* ----------------------------------------------------------------------- */

int 
ViewModalRendererGL::getRenderingMode() const 
{
  return __renderingMode;
}

bool 
ViewModalRendererGL::isBBoxRenderingEnable() const 
{
  return __renderingOption[0];
}

bool 
ViewModalRendererGL::isCtrlPointRenderingEnable() const
{
  return __renderingOption[1];
}

/* ----------------------------------------------------------------------- */

void 
ViewModalRendererGL::setRenderVolume()
{
  __renderingMode=1;
  emit renderingModeChanged(__renderingMode);
  emit valueChanged();
  status(tr("Volume Rendering"),5000);
}

void 
ViewModalRendererGL::setRenderVolumenWire()
{
  __renderingMode=4;
  emit renderingModeChanged(__renderingMode);
  emit valueChanged();
  status(tr("Volume and Wire Rendering"),5000);
}

void 
ViewModalRendererGL::setRenderWire()
{
  __renderingMode=2;
  emit renderingModeChanged(__renderingMode);
  emit valueChanged();
  status(tr("Wire Rendering"),5000);
}

void 
ViewModalRendererGL::setRenderSkeleton()
{
  __renderingMode=3;
  emit renderingModeChanged(__renderingMode);
  emit valueChanged();
  status(tr("Skeleton Rendering"),5000);
}

void 
ViewModalRendererGL::setRenderCtrlPoint()
{
  __renderingOption[1]=!__renderingOption[1];
  emit ctrlPointsRenderingChanged(__renderingOption[1]);
  emit valueChanged();
  if(__renderingOption[1])status(tr("Control Points Rendering Enable"),5000);
  else status(tr("Control Points Rendering Disable"),5000);
}

void 
ViewModalRendererGL::setRenderBBox()
{
  __renderingOption[0]=!__renderingOption[0];
  emit bboxRenderingChanged(__renderingOption[0]);
  emit valueChanged();
  if(__renderingOption[0])status(tr("Bounding Box Rendering Enable"),5000);
  else status(tr("Bounding Box Rendering Disable"),5000);
}

/* ----------------------------------------------------------------------- */

QMenu * 
ViewModalRendererGL::createToolsMenu(QWidget * parent)
{
  if(!__actions) __actions = new ViewRenderingModeActions(this);
  QMenu * menu = new QMenu(parent);
  __actions->fill(menu);
  return menu;
}

void 
ViewModalRendererGL::fillToolBar(QToolBar * toolBar)
{
  if(!__actions) __actions = new ViewRenderingModeActions(this);
  __actions->fill(toolBar);  
}
