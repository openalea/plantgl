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

#include "modalscenegl.h"
#include "modalwidget.h"
#include "util_qwidget.h"
#include "icons.h"

#include <qtoolbutton.h>
#include <qvariant.h>
#include <qwhatsthis.h>

/* ----------------------------------------------------------------------- */

ViewModalRendererGL::ViewModalRendererGL(ViewCameraGL * camera,
					 ViewLightGL * light,
					 QGLWidget * parent, 
					 const char * name) :
  ViewSceneRendererGL(camera,light,parent,name),
  __renderingMode(1)
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

QPopupMenu * 
ViewModalRendererGL::createToolsMenu(QWidget * parent)
{
  return new ViewRenderingModeMenu(this,parent);
}

void 
ViewModalRendererGL::fillToolBar(QToolBar * toolBar)
{
  ViewExclusiveButtonSet * bset = new ViewExclusiveButtonSet(4,toolBar);
  QPixmap volume(ViewerIcon::icon_solid);
  QPixmap wire(ViewerIcon::icon_wire);
  QPixmap skeleton(ViewerIcon::icon_skeleton);
  QPixmap ctrlpoint(ViewerIcon::icon_ctrlpoint);
  QPixmap bbox(ViewerIcon::icon_bbox);
  QPixmap light(ViewerIcon::icon_light);

    
  QToolButton * bt = new QToolButton(volume,  tr("Volume Rendering"), tr("Volume"),
	this, SLOT(setRenderVolume()), toolBar);
  QWhatsThis::add(bt,tr("<b>Volume Rendering</b><br><br>"
	"Change the Rendering Mode to <b>Volume</b>.<br><br>"
	"You can also use Menu <br><b>Tools > Renderer > Volume</b><br>"));
  bset->add(bt);
  bt = new QToolButton(wire,    tr("Wire Rendering"),   tr("Wire"),
		       this, SLOT(setRenderWire()), toolBar);
  QWhatsThis::add(bt,tr("<b>Wire Rendering</b><br><br>"
	"Change the Rendering Mode to <b>Wire</b>.<br><br>"
	"You can also use Menu <br><b>Tools > Renderer > Wire</b><br>"));
  bset->add(bt);
  bt = new QToolButton(skeleton,tr("Skeleton Rendering"),tr("Skeleton"),
		       this, SLOT(setRenderSkeleton()), toolBar);
  QWhatsThis::add(bt,tr("<b>Skeleton Rendering</b><br><br>"
	"Change the Rendering Mode to <b>Skeleton</b>.<br><br>"
	"You can also use Menu <br><b>Tools > Renderer > Skeleton</b><br>"));
  bset->add(bt);
  bt = new QToolButton(volume,    tr("Volume and Wire Rendering"),   tr("Volume and Wire"),
		       this, SLOT(setRenderVolumenWire()), toolBar);
  QWhatsThis::add(bt,tr("<b>Volume and Wire Rendering</b><br><br>"
	"Change the Rendering Mode to <b>Volume and Wire</b>.<br><br>"
	"You can also use Menu <br><b>Tools > Renderer > Volume and Wire</b><br>"));
  bset->add(bt);
  bset->setSelection(getRenderingMode());
  toolBar->addSeparator();

  QToolButton * __CtrlPoints = new QToolButton(ctrlpoint, tr("Control Points Rendering"), tr("Control Points"),
					       this, SLOT(setRenderCtrlPoint()), toolBar);
  QWhatsThis::add(__CtrlPoints,tr("<b>Control Points Rendering</b><br><br>"
	"Set <b>Control Points Rendering</b> enable/disable.<br><br>"
	"All the shapes define with some control points will display them.<br><br>"
	"You can also use Menu <br><b>Tools > Renderer > Control Points</b><br>"));
  __CtrlPoints->setToggleButton(true) ;
  QToolButton * __BBox       = new QToolButton(bbox,      tr("Bounding Box Rendering"),tr("Bounding Box"),
					       this, SLOT(setRenderBBox()),toolBar);
  QWhatsThis::add(__BBox,tr("<b>Bounding Box Rendering</b><br><br>"
	"Set <b>Bounding Box Rendering</b> enable/disable.<br><br>"
	"the Bounding Boxes of all shapes will be displayed.<br><br>"
	"You can also use Menu <br><b>Tools > Renderer > BBox</b><br>"));
  __BBox->setToggleButton(true) ;
  QToolButton * __Light      = new QToolButton(light,     tr("Light Rendering"), tr("Light"),
					       this, SLOT(setLightEnable()),toolBar);
  QWhatsThis::add(__Light,tr("<b>Light Rendering</b><br><br>"
	"Set <b>Light Rendering</b> enable/disable.<br><br>"
	"The Rendering will (not) take into account ligth source.<br><br>"
	"You can also use Menu <br><b>Tools > Renderer > Light</b><br>"));
  __Light->setToggleButton(true) ;
  toolBar->addSeparator();
  __CtrlPoints->setOn(isCtrlPointRenderingEnable());
  __Light->setOn(isLightEnable());
  __BBox->setOn(isBBoxRenderingEnable());
  QObject::connect(this,SIGNAL(bboxRenderingChanged(bool)),
		   __BBox,SLOT(setOn(bool)));
  QObject::connect(this,SIGNAL(ctrlPointsRenderingChanged(bool)),
		   __CtrlPoints,SLOT(setOn(bool)));
  QObject::connect(this,SIGNAL(lightEnableChanged(bool)),
		   __Light,SLOT(setOn(bool)));
  QObject::connect(this,SIGNAL(renderingModeChanged(const int)),
		   bset,SLOT(setSelection(const int)));
  
}
