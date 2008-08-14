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

#include "modalwidget.h"
#include "modalscenegl.h"
#include "light.h"
#include "icons.h"

#include <qmenu.h>
#include <qtoolbar.h>

ViewRenderingModeActions::ViewRenderingModeActions(ViewModalRendererGL * renderer, const char * name):
  QObject(renderer)
{
  if(name)setObjectName(name);
  if(renderer){
    QPixmap volume(ViewerIcon::getPixmap(ViewerIcon::solid));
    QPixmap wire(ViewerIcon::getPixmap(ViewerIcon::wire));
    QPixmap skeleton(ViewerIcon::getPixmap(ViewerIcon::skeleton));
    QPixmap ctrlpoint(ViewerIcon::getPixmap(ViewerIcon::ctrlpoint));
    QPixmap bbox(ViewerIcon::getPixmap(ViewerIcon::bbox));

	QActionGroup * mActionGroup = new QActionGroup(this);
	mActionGroup->setExclusive(true);

	idVolume   = new QAction(volume,  tr("&Volume"), this);
	idVolume->setShortcut(Qt::CTRL+Qt::Key_M);
	QObject::connect(idVolume,SIGNAL(triggered()), renderer, SLOT(setRenderVolume()));
	idVolume->setCheckable(true);
	idVolume->setWhatsThis(tr("<b>Volume Rendering</b><br><br>"
	"Change the Rendering Mode to <b>Volume</b>.<br><br>"
	"You can also use Menu <br><b>Tools > Renderer > Volume</b><br>"));

    idWire     = new QAction(wire,    tr("&Wire"), this);
	idWire->setShortcut(Qt::CTRL+Qt::Key_W);
	QObject::connect(idWire,SIGNAL(triggered()), renderer, SLOT(setRenderWire()));
	idWire->setCheckable(true);
    idWire->setWhatsThis(tr("<b>Wire Rendering</b><br><br>"
	"Change the Rendering Mode to <b>Wire</b>.<br><br>"
	"You can also use Menu <br><b>Tools > Renderer > Wire</b><br>"));

    idSkeleton = new QAction(skeleton,tr("S&keleton"),  this);
	idWire->setShortcut(Qt::CTRL+Qt::Key_K);
	QObject::connect(idSkeleton,SIGNAL(triggered()), renderer, SLOT(setRenderSkeleton()));
	idSkeleton->setCheckable(true);
    idSkeleton->setWhatsThis(tr("<b>Skeleton Rendering</b><br><br>"
	"Change the Rendering Mode to <b>Skeleton</b>.<br><br>"
	"You can also use Menu <br><b>Tools > Renderer > Skeleton</b><br>"));

	idVolWire =  new QAction(volume,  tr("Volu&me and Wire"),   this);
	QObject::connect(idVolWire,SIGNAL(triggered()), renderer, SLOT(setRenderVolumenWire()));
	idVolWire->setCheckable(true);
    idVolWire->setWhatsThis(tr("<b>Volume and Wire Rendering</b><br><br>"
	"Change the Rendering Mode to <b>Volume and Wire</b>.<br><br>"
	"You can also use Menu <br><b>Tools > Renderer > Volume and Wire</b><br>"));

	mActionGroup->addAction(idVolume);
	mActionGroup->addAction(idWire);
	mActionGroup->addAction(idSkeleton);
	mActionGroup->addAction(idVolWire);

    idCtrlPoints = new QAction(ctrlpoint, tr("&Control Points"),this);
	QObject::connect(idCtrlPoints,SIGNAL(triggered()), renderer, SLOT(setRenderCtrlPoint()));
	idCtrlPoints->setCheckable(true);
	idCtrlPoints->setChecked(renderer->isCtrlPointRenderingEnable());
    idCtrlPoints->setWhatsThis(tr("<b>Control Points Rendering</b><br><br>"
	"Set <b>Control Points Rendering</b> enable/disable.<br><br>"
	"All the shapes define with some control points will display them.<br><br>"
	"You can also use Menu <br><b>Tools > Renderer > Control Points</b><br>"));

	idBBox       = new QAction(bbox,      tr("&Bounding Box"),this);
	QObject::connect(idBBox,SIGNAL(triggered()), renderer, SLOT(setRenderBBox()));
	idBBox->setCheckable(true);
	idBBox->setChecked(renderer->isBBoxRenderingEnable());
    idBBox->setWhatsThis(tr("<b>Bounding Box Rendering</b><br><br>"
	"Set <b>Bounding Box Rendering</b> enable/disable.<br><br>"
	"the Bounding Boxes of all shapes will be displayed.<br><br>"
	"You can also use Menu <br><b>Tools > Renderer > BBox</b><br>"));


	QObject::connect(renderer,SIGNAL(ctrlPointsRenderingChanged(bool)),idCtrlPoints,SLOT(setChecked(bool)));
	QObject::connect(renderer,SIGNAL(bboxRenderingChanged(bool)),idBBox,SLOT(setChecked(bool)));

    setRenderingMode(renderer->getRenderingMode());
    QObject::connect(renderer,SIGNAL(renderingModeChanged(const int)),
		     this,SLOT(setRenderingMode(const int)));
  }
}

ViewRenderingModeActions::~ViewRenderingModeActions()
{
}


void ViewRenderingModeActions::fill(QMenu * menu) const
{
	menu->addAction(idVolume);
	menu->addAction(idWire);
	menu->addAction(idSkeleton);
	menu->addAction(idVolWire);
	menu->addSeparator();
	menu->addAction(idCtrlPoints);
	menu->addAction(idBBox);
}

void ViewRenderingModeActions::fill(QToolBar * bar) const
{
	bar->addAction(idVolume);
	bar->addAction(idWire);
	bar->addAction(idSkeleton);
	bar->addAction(idVolWire);
	bar->addSeparator();
	bar->addAction(idCtrlPoints);
	bar->addAction(idBBox);
}

void 
ViewRenderingModeActions::setRenderingMode(const int i)
{
  idVolume->setChecked(i == 1);
  idWire->setChecked(i == 2);
  idSkeleton->setChecked(i == 3);
  idVolWire->setChecked(i == 4);
}

