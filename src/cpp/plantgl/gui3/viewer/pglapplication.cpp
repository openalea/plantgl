//#define GEOM_DEBUG
/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): F. Boudon
 *
 *       $Source: /usr/cvsmaster/AMAPmod/src/GEOM/view_application.cpp,v $
 *       $Id: view_application.cpp,v 1.29 2006/08/12 21:44:09 barbier Exp $
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


#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/scene/shape.h>

#include "../base/appbuilder.h"

#include "pglapplication.h"
#include "pglviewer.h"
#include "geomevent.h"


PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace std;

PGLViewerApplication3::PGLViewerApplication3(){}

PGLViewerApplication3::~PGLViewerApplication3(){}


void
PGLViewerApplication3::display(const GeometryPtr& g){
  ScenePtr scene(new Scene());
  scene->add(Shape3DPtr(new Shape(g)));
  PGLViewerApplication3::display(scene);
}

void
PGLViewerApplication3::add(const GeometryPtr& g){
  ScenePtr scene(new Scene());
  scene->add(Shape3DPtr(new Shape(g)));
  PGLViewerApplication3::add(scene);
}

void
PGLViewerApplication3::display(const ScenePtr& s){
  ViewerApplication3::_sendAnEvent(new GeomSceneChangeEvent3(s,QString::null,QString::null,false));
}

void
PGLViewerApplication3::add(const ScenePtr& s){
  ViewerApplication3::_sendAnEvent(new GeomSceneChangeEvent3(s,QString::null,QString::null,true));
}

ScenePtr PGLViewerApplication3::getCurrentScene()
{
	ScenePtr sc;
	ViewerApplication3::_sendAnEvent(new GeomGetSceneEvent3(&sc));
	return sc;
}

std::vector<std::pair<uint_t,double> > 
PGLViewerApplication3::getProjectionSizes(const PGL(ScenePtr)& sc){
  std::vector<std::pair<uint_t,double> > res;
  ViewerApplication3::_sendAnEvent(new GeomProjListEvent3(sc,&res));
  return res;
}

ViewRayPointHitBuffer3 * 
PGLViewerApplication3::castRays2(const PGL(ScenePtr)& sc, bool back_test )
{
	ViewRayPointHitBuffer3 * res = NULL ;
	ViewRayBuff2Event3 * myevent = new ViewRayBuff2Event3(sc,back_test,&res);
	ViewerApplication3::_sendAnEvent(myevent);
	return res;
}

void
PGLViewerApplication3::init()
{ 
	_setViewerBuilder(new ViewerTBuilder3<PGLViewer3>()); 
}
