//#define GEOM_DEBUG
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
  ViewerApplication::_sendAnEvent(new GeomSceneChangeEvent(s,QString(),QString(),false));
}

void
PGLViewerApplication::add(const ScenePtr& s){
  ViewerApplication::_sendAnEvent(new GeomSceneChangeEvent(s,QString(),QString(),true));
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
