//#define GEOM_DEBUG
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


#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/scene/shape.h>

#include "../base/appbuilder.h"

#include "pglapplication.h"
#include "pglviewer.h"
#include "geomevent.h"


PGL_USING_NAMESPACE
using namespace std;

PGLViewerApplication::PGLViewerApplication(){}

PGLViewerApplication::~PGLViewerApplication(){}


void
PGLViewerApplication::display(const GeometryPtr& g){
  ScenePtr scene(new Scene());
  scene->add(Shape3DPtr(new Shape(g)));
  PGLViewerApplication::display(scene);
}

void
PGLViewerApplication::add(const GeometryPtr& g){
  ScenePtr scene(new Scene());
  scene->add(Shape3DPtr(new Shape(g)));
  PGLViewerApplication::add(scene);
}

void
PGLViewerApplication::display(const ScenePtr& s){
  ViewerApplication::_sendAnEvent(new GeomSceneChangeEvent(s,QString::null,QString::null,false));
}

void
PGLViewerApplication::add(const ScenePtr& s){
  ViewerApplication::_sendAnEvent(new GeomSceneChangeEvent(s,QString::null,QString::null,true));
}

ScenePtr PGLViewerApplication::getCurrentScene()
{
    ScenePtr sc;
    ViewerApplication::_sendAnEvent(new GeomGetSceneEvent(&sc));
    return sc;
}

std::vector<std::pair<uint_t,double> >
PGLViewerApplication::getProjectionSizes(const PGL(ScenePtr)& sc){
  std::vector<std::pair<uint_t,double> > res;
  ViewerApplication::_sendAnEvent(new GeomProjListEvent(&res,sc));
  return res;
}

std::vector<std::pair<uint_t,uint_t> >
PGLViewerApplication::getProjectionPerShape(double& pixelsize){
  std::vector<std::pair<uint_t,uint_t> > res;
  ViewerApplication::_sendAnEvent(new ViewIntegratedProjListEvent(&res,&pixelsize));
  return res;
}

ViewRayPointHitBuffer *
PGLViewerApplication::castRays2(const PGL(ScenePtr)& sc, bool back_test )
{
    ViewRayPointHitBuffer * res = NULL ;
    ViewRayBuff2Event * myevent = new ViewRayBuff2Event(&res,sc,back_test);
    ViewerApplication::_sendAnEvent(myevent);
    return res;
}

void
PGLViewerApplication::init()
{
    _setViewerBuilder(new ViewerTBuilder<PGLViewer>());
}
