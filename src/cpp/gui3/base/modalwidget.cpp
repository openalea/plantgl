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

#include "modalwidget.h"
#include "modalscenegl.h"
#include "icons.h"

ViewRenderingModeMenu3::ViewRenderingModeMenu3(ViewModalRendererGL3 * renderer, QWidget * parent, const char * name):
  QPopupMenu(parent,name)
{

  if(renderer){
    QPixmap volume(ViewerIcon3::icon_solid);
    QPixmap wire(ViewerIcon3::icon_wire);
    QPixmap skeleton(ViewerIcon3::icon_skeleton);
    QPixmap ctrlpoint(ViewerIcon3::icon_ctrlpoint);
    QPixmap bbox(ViewerIcon3::icon_bbox);
    QPixmap light(ViewerIcon3::icon_light);
    idVolume  = insertItem(volume,  tr("&Volume"),     renderer, SLOT(setRenderVolume()), CTRL+Key_M);
    idWire    = insertItem(wire,    tr("&Wire"),       renderer, SLOT(setRenderWire()), CTRL+Key_W);
    idSkeleton= insertItem(skeleton,tr("S&keleton"),   renderer, SLOT(setRenderSkeleton()), CTRL+Key_K);
    idVolWire=  insertItem(volume,  tr("Volu&me and Wire"),   renderer, SLOT(setRenderVolumenWire()));
    insertSeparator();
    idCtrlPoints = insertItem(ctrlpoint, tr("&Control Points"),   renderer, SLOT(setRenderCtrlPoint()));
    idBBox       = insertItem(bbox,      tr("&Bounding Box"),             renderer, SLOT(setRenderBBox()));
    idLight      = insertItem(light,     tr("&Light"),            renderer, SLOT(setLightEnable()));
    setRenderingMode(renderer->getRenderingMode());
    setRenderCtrlPoint(renderer->isCtrlPointRenderingEnable());
    setLightEnable(renderer->isLightEnable());
    setRenderBBox(renderer->isBBoxRenderingEnable());
    QObject::connect(renderer,SIGNAL(bboxRenderingChanged(bool)),
 		     this,SLOT(setRenderBBox(bool)));
    QObject::connect(renderer,SIGNAL(ctrlPointsRenderingChanged(bool)),
		     this,SLOT(setRenderCtrlPoint(bool)));
    QObject::connect(renderer,SIGNAL(lightEnableChanged(bool)),
		     this,SLOT(setLightEnable(bool)));
    QObject::connect(renderer,SIGNAL(renderingModeChanged(const int)),
		     this,SLOT(setRenderingMode(const int)));
  }
}

ViewRenderingModeMenu3::~ViewRenderingModeMenu3()
{
}


void 
ViewRenderingModeMenu3::setRenderingMode(const int i)
{
  if(i == 2){
    setItemChecked(idVolume,false);
    setItemChecked(idWire,true);
    setItemChecked(idSkeleton,false);
    setItemChecked(idVolWire,false);
  }
  else if(i == 3){
    setItemChecked(idVolume,false);
    setItemChecked(idWire,false);
    setItemChecked(idSkeleton,true);
    setItemChecked(idVolWire,false);
  }
  else if(i == 4){
    setItemChecked(idVolume,false);
    setItemChecked(idWire,false);
    setItemChecked(idSkeleton,false);
    setItemChecked(idVolWire,true);
  }
  else {
    setItemChecked(idVolume,true);
    setItemChecked(idWire,false);
    setItemChecked(idSkeleton,false);
    setItemChecked(idVolWire,false);
  }
}

void
ViewRenderingModeMenu3::setRenderCtrlPoint(const bool b)
{
  setItemChecked(idCtrlPoints,b);
}

void 
ViewRenderingModeMenu3::setLightEnable(const bool b)
{
  setItemChecked(idLight,b);
}

void 
ViewRenderingModeMenu3::setRenderBBox(const bool b)
{
  setItemChecked(idBBox,b);
}
