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


#include "gltransitionrenderer.h"
#include <plantgl/pgl_appearance.h>
#include <plantgl/scenegraph/scene/scene.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


#define GEOM_GLRENDERER_CHECK_APPEARANCE(app) \
  if ((__appearance) && \
      (__appearance->getObjectId() == app->getObjectId())) return true;


#define GEOM_GLRENDERER_UPDATE_APPEARANCE(app) \
  __appearance = AppearancePtr(app);


#define GEOM_GLRENDERER_COLOR3(color) \
  glColor3ubv(color.begin());

#define GEOM_GLRENDERER_COLOR4(color) \
  glColor4ubv(color.begin());


/* ----------------------------------------------------------------------- */


GLTransitionRenderer::GLTransitionRenderer( Discretizer& discretizer,
#ifndef PGL_CORE_WITHOUT_QT
                                            QGLWidget * widget,
#endif
                                            uint_t step ) :
  GLRenderer( discretizer
#ifndef PGL_CORE_WITHOUT_QT
    , widget
#endif
    ),
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
