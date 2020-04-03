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


#include "modalscenegl.h"
#include "modalwidget.h"
#include "util_qwidget.h"
#include "icons.h"

#include <QtCore/qvariant.h>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
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

void
ViewModalRendererGL::fillToolBarAdditionnal(QToolBar * toolBar)
{
  if(!__actions) __actions = new ViewRenderingModeActions(this);
  __actions->fillAdditionnal(toolBar);
}
