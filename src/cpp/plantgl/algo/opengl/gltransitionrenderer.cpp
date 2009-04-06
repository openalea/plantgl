/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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




#include "gltransitionrenderer.h"
#include <plantgl/pgl_appearance.h>
#include <plantgl/scenegraph/scene/scene.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


#define GEOM_GLRENDERER_CHECK_APPEARANCE(app) \
  if ((__appearance) && \
      (__appearance->getId() == app->getId())) return true;


#define GEOM_GLRENDERER_UPDATE_APPEARANCE(app) \
  __appearance = AppearancePtr(app); 


#define GEOM_GLRENDERER_COLOR3(color) \
  glColor3ubv(color.begin());

#define GEOM_GLRENDERER_COLOR4(color) \
  glColor4ubv(color.begin());


/* ----------------------------------------------------------------------- */


GLTransitionRenderer::GLTransitionRenderer( Discretizer& discretizer, 
											QGLWidget * widget, 
											uint_t step ) :
  GLRenderer( discretizer, widget ),
  __totalstep(step),
  __laststep(step+1),
  __transparency(0)
{
}


GLTransitionRenderer::~GLTransitionRenderer( ) {
  clear();
}


void 
GLTransitionRenderer::clear( ) {
  GLRenderer::clear();
  __scene1 = ScenePtr();
  __scene2 = ScenePtr();
}

const uint_t 
GLTransitionRenderer::getTotalStep() const {
  return __totalstep;
}

void
GLTransitionRenderer::setTotalStep(const uint_t s){
  __totalstep = s;
  __laststep = s+1;
}


void GLTransitionRenderer::setScene(ScenePtr scene1, ScenePtr scene2){
    clear();
      __scene1 = scene1;
      __scene2 = scene2;
}

/* ----------------------------------------------------------------------- */

bool 
GLTransitionRenderer::endProcess(){
  return true;
}

/* ----------------------------------------------------------------------- */

bool GLTransitionRenderer::process( Material * material ) {
  GEOM_ASSERT(material);

  GEOM_GLRENDERER_CHECK_APPEARANCE(material);

  GLfloat _rgba[4];
  const Color3& _ambient = material->getAmbient();
  _rgba[0] = (GLfloat)_ambient.getRedClamped();
  _rgba[1] = (GLfloat)_ambient.getGreenClamped();
  _rgba[2] = (GLfloat)_ambient.getBlueClamped();
  _rgba[3] = __transparency;
  glMaterialfv(GL_FRONT,GL_AMBIENT,_rgba);
  
  const real_t& _diffuse = material->getDiffuse();
  _rgba[0] *= _diffuse;
  _rgba[1] *= _diffuse;
  _rgba[2] *= _diffuse;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,_rgba);

 /// We set the current color in the case of disabling the lighting
  glColor4fv(_rgba);

  const Color3& _specular = material->getSpecular();
  _rgba[0] = (GLfloat)_specular.getRedClamped();
  _rgba[1] = (GLfloat)_specular.getGreenClamped();
  _rgba[2] = (GLfloat)_specular.getBlueClamped();
  glMaterialfv(GL_FRONT,GL_SPECULAR,_rgba);

  const Color3& _emission = material->getEmission();
  _rgba[0] = (GLfloat)_emission.getRedClamped();
  _rgba[1] = (GLfloat)_emission.getGreenClamped();
  _rgba[2] = (GLfloat)_emission.getBlueClamped();
  glMaterialfv(GL_FRONT,GL_EMISSION,_rgba);

  glMaterialf(GL_FRONT,GL_SHININESS,material->getShininess());

  GEOM_GLRENDERER_UPDATE_APPEARANCE(material);

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true; 
}


/* ----------------------------------------------------------------------- */


bool GLTransitionRenderer::process( MonoSpectral * monoSpectral ) {
  GEOM_ASSERT(monoSpectral);

 GEOM_GLRENDERER_CHECK_APPEARANCE(monoSpectral);

  GLfloat _rgba[4];
  _rgba[0] = (GLfloat)monoSpectral->getReflectance();
  _rgba[1] = (GLfloat)monoSpectral->getReflectance();
  _rgba[2] = (GLfloat)monoSpectral->getReflectance();
  _rgba[3] = __transparency;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,_rgba);

 /// We set the current color in the case of disabling the lighting
  glColor4fv(_rgba);

  GEOM_GLRENDERER_UPDATE_APPEARANCE(monoSpectral);

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true; 
}


/* ----------------------------------------------------------------------- */


bool GLTransitionRenderer::process( MultiSpectral * multiSpectral ) {
  GEOM_ASSERT(multiSpectral);

  GEOM_GLRENDERER_CHECK_APPEARANCE(multiSpectral);

  GLfloat _rgba[4];
  const Index3& _filter = multiSpectral->getFilter();
  _rgba[0] = (GLfloat)multiSpectral->getReflectanceAt(_filter.getAt(0));
  _rgba[1] = (GLfloat)multiSpectral->getReflectanceAt(_filter.getAt(1));
  _rgba[2] = (GLfloat)multiSpectral->getReflectanceAt(_filter.getAt(2));
  _rgba[3] = __transparency;

  glMaterialfv(GL_FRONT,GL_DIFFUSE,_rgba);

 /// We set the current color in the case of disabling the lighting
  glColor4fv(_rgba);

  GEOM_GLRENDERER_UPDATE_APPEARANCE(multiSpectral);

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true; 
}


/* ----------------------------------------------------------------------- */

bool GLTransitionRenderer::rend(uint_t step){
  if(step >= __totalstep)step = __totalstep - 1;
  
  if(__laststep != step){
    clearSceneList();
    __laststep = step;
  }
  if(beginSceneList()){
    if(step <= 0){
      if(__scene1){
	__transparency = (GLfloat)1;
	__scene1->apply(*this);
      }
    }
    else if(step >= __totalstep){
      if(__scene2){
	__transparency = (GLfloat)1;
	__scene2->apply(*this);
      }	
    }
    else {
      if(step < __totalstep/2){
	if(__scene1){
	  __transparency = (GLfloat)((float)(__totalstep - step -1) 
				     / (float)(__totalstep - 1));
	  __scene1->apply(*this);
	}
	if(__scene2){
	__transparency = (GLfloat)((float)step /(float) (__totalstep - 1));
	__scene2->apply(*this);
	}
      }
      else {
	if(__scene2){
	  __transparency = (GLfloat)((float)step /(float) (__totalstep - 1));
	  __scene2->apply(*this);
	}
	if(__scene1){
	  __transparency = (GLfloat)((float)(__totalstep - step -1) 
				     / (float)(__totalstep - 1));
	  __scene1->apply(*this);
	}
      }
    }    
    __appearance = AppearancePtr();
    endSceneList();
  }
  return true;
}
