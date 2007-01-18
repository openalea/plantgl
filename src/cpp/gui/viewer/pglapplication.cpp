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


#include <SceneGraph/Appearance/appe_material.h>
#include <SceneGraph/Scene/scne_shape.h>

#include <Viewer/view_appbuilder.h>

#include "view_pglapplication.h"
#include "view_pglviewer.h"
#include "view_geomevent.h"


PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace std;

PGLViewerApplication::PGLViewerApplication(){}

PGLViewerApplication::~PGLViewerApplication(){}


void
PGLViewerApplication::display(const GeometryPtr& g){
  ScenePtr scene(new Scene());
  scene->add(new Shape(g));
  PGLViewerApplication::display(scene);
}

void
PGLViewerApplication::add(const GeometryPtr& g){
  ScenePtr scene(new Scene());
  scene->add(new Shape(g));
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

std::vector<std::pair<uint32_t,double> > 
PGLViewerApplication::getProjectionSizes(const PGL(ScenePtr)& sc){
  std::vector<std::pair<uint32_t,double> > res;
  ViewerApplication::_sendAnEvent(new GeomProjListEvent(sc,&res));
  return res;
}

void
PGLViewerApplication::init()
{ 
	_setViewerBuilder(new ViewerTBuilder<PGLViewer>()); 
}
