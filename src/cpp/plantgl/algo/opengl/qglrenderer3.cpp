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




#include "qglrenderer3.h"
#include <plantgl/algo/base/discretizer.h>
#include <plantgl/algo/base/tesselator.h>

#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/pgl_scene.h>

#include <plantgl/scenegraph/container/indexarray.h>
#include "util_appegl.h"
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/geometryarray2.h>

#ifndef PGL_WITHOUT_QT

#include <QtOpenGL>
#include <QtGui/QImage>
#include <QtGui/QFont>

#endif

#include <typeinfo>
#include <plantgl/math/util_math.h>


PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE




/* ----------------------------------------------------------------------- */


#ifdef GEOM_TREECALLDEBUG
#define GEOM_ASSERT_OBJ(obj) printf("Look at %sobject %zu of type '%s' in mode %i\n", (!obj->unique()?"shared ":""),obj->getId(), typeid(*obj).name(), __compil);
#else
#define GEOM_ASSERT_OBJ(obj)
#endif

#define GEOM_GLRENDERER_CHECK_APPEARANCE(app) \
  if (__appearance.get() == (Appearance *)app) return true;

#define GEOM_GLRENDERER_UPDATE_APPEARANCE(app) \
  __appearance = AppearancePtr(app);


template<class T>
bool QGLRenderer3::discretize_and_render(T *geom) {
  GEOM_ASSERT_OBJ(geom);
  if (__appearance && __appearance->isTexture())
    __discretizer.computeTexCoord(true);
  else __discretizer.computeTexCoord(false);
  bool b = geom->apply(__discretizer);
  if (b && (b = (__discretizer.getDiscretization()))) {
    b = __discretizer.getDiscretization()->apply(*this);
  }
  return b;
}


#define GEOM_GLRENDERER_DISCRETIZE_RENDER(geom) \
  GEOM_ASSERT_OBJ(geom); \
  return discretize_and_render(geom); \


/*
#define BEGIN_LINE_WIDTH(obj) \
  if(!obj->isWidthToDefault()){ \
    glPushAttrib (GL_LINE_BIT); \
    int globalwidth = 1; \
        glGetIntegerv(GL_LINE_WIDTH,&globalwidth); \
        glLineWidth(float(obj->getWidth()+globalwidth-1)); \
  } \

#define END_LINE_WIDTH(obj) if(!obj->isWidthToDefault()){ glPopAttrib(); }
*/

#define  PUSH_MODELMATRIX __modelmatrix.push();

#define  POP_MODELMATRIX __modelmatrix.pop();

/* ----------------------------------------------------------------------- */


QGLRenderer3::QGLRenderer3(Discretizer& discretizer,
                           Tesselator& tesselator,
                           const QMatrix4x4& projection,
                           const Vector3& lightPosition,
                           const Color3& lightColor,
                           QOpenGLFunctions& ogl) :
        Action(),
        __projection(projection),
        __lightPosition(lightPosition),
        __lightColor(lightColor),
        __ogl(ogl),
        __discretizer(discretizer),
        __tesselator(tesselator),
        __appearance()
{
}


QGLRenderer3::~QGLRenderer3() {
  clear();
}

Discretizer &
QGLRenderer3::getDiscretizer() {
  return __discretizer;
}

void QGLRenderer3::init() {

}

void QGLRenderer3::clear() {

}



/* ----------------------------------------------------------------------- */




bool QGLRenderer3::beginProcess() {
  return true;
}


bool QGLRenderer3::endProcess() {
  __appearance = AppearancePtr();
  return true;
}

/* ----------------------------------------------------------------------- */
bool QGLRenderer3::process(Shape *geomshape) {
  GEOM_ASSERT_OBJ(geomshape);
  processAppereance(geomshape);
  return processGeometry(geomshape);
}

bool QGLRenderer3::processAppereance(Shape *geomshape) {
  GEOM_ASSERT(geomshape);
  if (geomshape->appearance) {
    return geomshape->appearance->apply(*this);
  } else return Material::DEFAULT_MATERIAL->apply(*this);
}

bool QGLRenderer3::processGeometry(Shape *geomshape) 
{
  GEOM_ASSERT(geomshape);
  return geomshape->geometry->apply(*this);

  assert(glGetError() == GL_NO_ERROR);
}

/* ----------------------------------------------------------------------- */

bool QGLRenderer3::process(Inline *geomInline) {
  GEOM_ASSERT_OBJ(geomInline);
  if (geomInline->getScene()) {
    if (!geomInline->isTranslationToDefault() || !geomInline->isScaleToDefault()) {
        PUSH_MODELMATRIX;
        const Vector3 _trans = geomInline->getTranslation();
        __modelmatrix.translate(_trans);
        const Vector3 _scale = geomInline->getScale();
        __modelmatrix.scale(_scale);
    }

    bool _result = true;
    for (Scene::iterator _i = geomInline->getScene()->begin();
         _i != geomInline->getScene()->end();
         _i++) {
      if (!(*_i)->applyAppearanceFirst(*this)) _result = false;
    };

    if (!geomInline->isTranslationToDefault() || !geomInline->isScaleToDefault()) {
      POP_MODELMATRIX;
    }

    assert(glGetError() == GL_NO_ERROR);
    return _result;
  } 
  else 
    return false;
}

/* ----------------------------------------------------------------------- */

bool QGLRenderer3::process(AmapSymbol *amapSymbol) {
  GEOM_ASSERT_OBJ(amapSymbol);

  /*
  const IndexArrayPtr &_indexList = amapSymbol->getIndexList();

  glFrontFace(amapSymbol->getCCW() ? GL_CCW : GL_CW);
  bool tex = __appearance && __appearance->isTexture() && amapSymbol->hasTexCoordList();
  bool color = amapSymbol->hasColorList();
  bool colorV = amapSymbol->getColorPerVertex();

  if (__appearance &&
      __appearance->isTexture() &&
      !amapSymbol->hasTexCoordList()) {
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
  }
  amapSymbol->checkNormalList();

  glEnableClientState(GL_VERTEX_ARRAY);
  real_t * vertices = amapSymbol->getPointList()->data();
  glVertexPointer( 3, GL_GEOM_REAL,0,vertices);

  real_t * normals = amapSymbol->getNormalList()->data();
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer( GL_GEOM_REAL,0,normals);

  real_t * texCoord = NULL;
  if(tex){
    texCoord = amapSymbol->getTexCoordList()->data();
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer( 2, GL_GEOM_REAL, sizeof(real_t) ,&texCoord[1]);
  }

  for(IndexArray::const_iterator it = amapSymbol->getIndexList()->begin();
      it != amapSymbol->getIndexList()->end(); it++)
    {
      glDrawElements( GL_POLYGON, it->size() , GL_UNSIGNED_INT, ( const GLvoid* )( &*( it->begin() ) ));
    }

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  delete [] vertices;
  delete [] normals;
  if(texCoord)delete [] texCoord;

  if (__appearance &&
      __appearance->isTexture() &&
      !amapSymbol->hasTexCoordList()) {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
  }
  */
  return true;
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(AsymmetricHull *asymmetricHull) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(asymmetricHull);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(AxisRotated *axisRotated) {
  GEOM_ASSERT_OBJ(axisRotated);


  const Vector3 &_axis = axisRotated->getAxis();
  const real_t &_angle = axisRotated->getAngle();

  PUSH_MODELMATRIX;
  
  __modelmatrix.axisRotation(_axis, _angle);
  axisRotated->getGeometry()->apply(*this);

  POP_MODELMATRIX;

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(BezierCurve *bezierCurve) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(bezierCurve);
}

/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(BezierPatch *bezierPatch) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(bezierPatch);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(Box *box) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(box);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(Cone *cone) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(cone);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(Cylinder *cylinder) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(cylinder);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(ElevationGrid *elevationGrid) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(elevationGrid);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(EulerRotated *eulerRotated) {
  GEOM_ASSERT_OBJ(eulerRotated);

  PUSH_MODELMATRIX;

  __modelmatrix.eulerRotationZYX(eulerRotated->getAzimuth(),
                            eulerRotated->getElevation(),
                            eulerRotated->getRoll());

  eulerRotated->getGeometry()->apply(*this);
  POP_MODELMATRIX;

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(ExtrudedHull *extrudedHull) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(extrudedHull);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(FaceSet *faceSet) {
  GEOM_ASSERT_OBJ(faceSet);
  __tesselator.process(faceSet);
  __tesselator.getTriangulation()->apply(*this);

  /*
  glFrontFace(faceSet->getCCW() ? GL_CCW : GL_CW);
  const IndexArrayPtr &_indexList = faceSet->getIndexList();
  bool normalV = faceSet->getNormalPerVertex();
  bool tex = __appearance && __appearance->isTexture() && faceSet->hasTexCoordList();
  bool color = faceSet->hasColorList();
  bool colorV = faceSet->getColorPerVertex();

  if (__appearance &&
      __appearance->isTexture() &&
      !faceSet->hasTexCoordList()) {
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
  }
  faceSet->checkNormalList();

  glEnableClientState(GL_VERTEX_ARRAY);
  real_t * vertices = faceSet->getPointList()->data();
  glVertexPointer( 3, GL_GEOM_REAL,0,vertices);

  faceSet->checkNormalList();
  real_t * normals = NULL;
  if (normalV) {
    normals = faceSet->getNormalList()->data();
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer( GL_GEOM_REAL,0,normals);
  }

  real_t * texCoord = NULL;
  if(tex){
    texCoord = faceSet->getTexCoordList()->data();
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer( 2, GL_GEOM_REAL,0,texCoord);
  }

  size_t _i = 0;
  for(IndexArray::const_iterator it = faceSet->getIndexList()->begin();
      it != faceSet->getIndexList()->end(); it++)
    {
      if(!normalV)
    glGeomNormal(faceSet->getNormalAt(_i));
      _i++;
      glDrawElements( GL_POLYGON, it->size() , GL_UNSIGNED_INT, ( const GLvoid* )( &*( it->begin() )) );
    }

  glDisableClientState(GL_VERTEX_ARRAY);

  if (normalV)glDisableClientState(GL_NORMAL_ARRAY);
  if(tex)glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  delete [] vertices;
  if(normals)delete [] normals;
  if(texCoord)delete [] texCoord;

  if (__appearance &&
      __appearance->isTexture() &&
      !faceSet->hasTexCoordList()) {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
  }
    
 */
  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(Frustum *frustum) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(frustum);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(Extrusion *extrusion) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(extrusion);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(Group *group) {
  GEOM_ASSERT_OBJ(group);
    
  group->getGeometryList()->apply(*this);

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(IFS *ifs) {
  GEOM_ASSERT_OBJ(ifs);

  ITPtr transfos;
  transfos = dynamic_pointer_cast<IT>(ifs->getTransformation());
  GEOM_ASSERT(transfos);
  const Matrix4ArrayPtr &matrixList = transfos->getAllTransfo();
  GEOM_ASSERT(matrixList);

  Matrix4Array::const_iterator matrix = matrixList->begin();
  while (matrix != matrixList->end()) {
    PUSH_MODELMATRIX;
    
    __modelmatrix.transform(*matrix);
    ifs->getGeometry()->apply(*this);
    
    POP_MODELMATRIX;
    matrix++;
  }

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(Material *material) {
  GEOM_ASSERT_OBJ(material);
  GEOM_GLRENDERER_CHECK_APPEARANCE(material);

/*
  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  GLfloat _rgba[4];
  const Color3 &_ambient = material->getAmbient();
  _rgba[0] = (GLfloat) _ambient.getRedClamped();
  _rgba[1] = (GLfloat) _ambient.getGreenClamped();
  _rgba[2] = (GLfloat) _ambient.getBlueClamped();
  _rgba[3] = 1.0f - material->getTransparency();
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _rgba);

  const real_t &_diffuse = material->getDiffuse();
  _rgba[0] *= _diffuse;
  _rgba[1] *= _diffuse;
  _rgba[2] *= _diffuse;
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _rgba);

  /// We set the current color in the case of disabling the lighting
  glColor4fv(_rgba);

  const Color3 &_specular = material->getSpecular();
  _rgba[0] = (GLfloat) _specular.getRedClamped();
  _rgba[1] = (GLfloat) _specular.getGreenClamped();
  _rgba[2] = (GLfloat) _specular.getBlueClamped();
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, _rgba);

  const Color3 &_emission = material->getEmission();
  _rgba[0] = (GLfloat) _emission.getRedClamped();
  _rgba[1] = (GLfloat) _emission.getGreenClamped();
  _rgba[2] = (GLfloat) _emission.getBlueClamped();
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, _rgba);

  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->getShininess());
  */

  GEOM_GLRENDERER_UPDATE_APPEARANCE(material);

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */
bool QGLRenderer3::process(ImageTexture *texture) {
  GEOM_ASSERT_OBJ(texture);
  GEOM_GLRENDERER_CHECK_APPEARANCE(texture);

/*

  Cache<GLuint>::Iterator it = __cachetexture.find(texture->getId());
  if (it != __cachetexture.end()) {
    //  printf("bind texture : %i\n", it->second);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, it->second);
  } else {
#ifndef PGL_CORE_WITHOUT_QT
    QImage img;
    if (img.load(texture->getFilename().c_str())) {
      bool notUsingMipmap = (!texture->getMipmaping()) && isPowerOfTwo(img.width()) && isPowerOfTwo(img.height());
      glEnable(GL_TEXTURE_2D);
      img = QGLWidget::convertToGLFormat(img);
      GLuint id;
      glGenTextures(1, &id);
      if (id != 0) {
        glBindTexture(GL_TEXTURE_2D, id);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL  );
//    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND  );
//    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE  );

        glTexParameterf(GL_TEXTURE_2D,
                        GL_TEXTURE_WRAP_S,
                        texture->getRepeatS() ? GL_REPEAT : GL_CLAMP);

        glTexParameterf(GL_TEXTURE_2D,
                        GL_TEXTURE_WRAP_T,
                        texture->getRepeatT() ? GL_REPEAT : GL_CLAMP);

        glTexParameterf(GL_TEXTURE_2D,
                        GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR);

        if (notUsingMipmap) {
          glTexParameterf(GL_TEXTURE_2D,
                          GL_TEXTURE_MIN_FILTER,
                          GL_LINEAR);

          glTexImage2D(GL_TEXTURE_2D, 0, 4, img.width(), img.height(), 0,
                       GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
        } else {
#ifndef PGL_OLD_MIPMAP_STYLE
          if (HasGenerateMipmap) {
            glTexParameterf(GL_TEXTURE_2D,
                            GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR_MIPMAP_NEAREST);

            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

            glTexImage2D(GL_TEXTURE_2D, 0, 4, img.width(), img.height(), 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, img.bits());

            glGenerateMipmap(GL_TEXTURE_2D);
          }
#else
          gluBuild2DMipmaps( GL_TEXTURE_2D, 4, img.width(), img.height(),
                     GL_RGBA, GL_UNSIGNED_BYTE, img.bits() );
#endif
        }
        // printf("gen texture : %i\n",id);
        // registerTexture(texture,id);
        __cachetexture.insert(texture->getId(), id);
      }
    }
#endif
  }
  */

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */

bool QGLRenderer3::process(Texture2D *texture) {
  GEOM_ASSERT_OBJ(texture);
  GEOM_GLRENDERER_CHECK_APPEARANCE(texture);

/*
  GLfloat _rgba[4];

  const Color4 &_color = texture->getBaseColor();
  _rgba[0] = (GLfloat) _color.getRedClamped();
  _rgba[1] = (GLfloat) _color.getGreenClamped();
  _rgba[2] = (GLfloat) _color.getBlueClamped();
  _rgba[3] = 1.0f - _color.getAlphaClamped();

  /// We set the current color in the case of disabling the lighting
  glColor4fv(_rgba);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _rgba);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _rgba);

  if (texture->getImage()) {
    // load image
    texture->getImage()->apply(*this);

    // apply texture transformation
    if (texture->getTransformation())
      texture->getTransformation()->apply(*this);
    else {
      // Set Texture transformation to Id if no transformation is specified
      glMatrixMode(GL_TEXTURE);
      glLoadIdentity();
      glMatrixMode(GL_MODELVIEW);
    }
  }
  */

  GEOM_GLRENDERER_UPDATE_APPEARANCE(texture);

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}

/* ----------------------------------------------------------------------- */

bool QGLRenderer3::process(Texture2DTransformation *texturetransfo) {
  GEOM_ASSERT_OBJ(texturetransfo);

/*
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();

  if (!texturetransfo->isTranslationToDefault()) {
    Vector2 &value = texturetransfo->getTranslation();
    glGeomTranslate(value.x(), value.y(), 0.0);
  }

  if (!texturetransfo->isRotationAngleToDefault()) {
    Vector2 &rotationCenter = texturetransfo->getRotationCenter();
    glGeomTranslate(rotationCenter.x(), rotationCenter.y(), 0.0);

    real_t &value = texturetransfo->getRotationAngle();
    glGeomRadRotate(0, 0, 1, value);

    glGeomTranslate(-rotationCenter.x(), -rotationCenter.y(), 0.0);
  }

  if (!texturetransfo->isScaleToDefault()) {
    Vector2 &value = texturetransfo->getScale();
    glGeomScale(value.x(), value.y(), 1.0);
  }
  */

  glMatrixMode(GL_MODELVIEW);
  return true;
}

/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(MonoSpectral *monoSpectral) {
  GEOM_ASSERT_OBJ(monoSpectral);

  GEOM_GLRENDERER_CHECK_APPEARANCE(monoSpectral);
/*  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  GLfloat _rgba[4];
  _rgba[0] = (GLfloat) monoSpectral->getReflectance();
  _rgba[1] = (GLfloat) monoSpectral->getReflectance();
  _rgba[2] = (GLfloat) monoSpectral->getReflectance();
  _rgba[3] = (GLfloat) monoSpectral->getTransmittance();
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _rgba);

  /// We set the current color in the case of disabling the lighting
  glColor4fv(_rgba);*/

  GEOM_GLRENDERER_UPDATE_APPEARANCE(monoSpectral);

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(MultiSpectral *multiSpectral) {
  GEOM_ASSERT_OBJ(multiSpectral);

  GEOM_GLRENDERER_CHECK_APPEARANCE(multiSpectral);

/*  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  GLfloat _rgba[4];
  const Index3 &_filter = multiSpectral->getFilter();
  _rgba[0] = (GLfloat) multiSpectral->getReflectanceAt(_filter.getAt(0));
  _rgba[1] = (GLfloat) multiSpectral->getReflectanceAt(_filter.getAt(1));
  _rgba[2] = (GLfloat) multiSpectral->getReflectanceAt(_filter.getAt(2));
  _rgba[3] = ((GLfloat) multiSpectral->getTransmittanceAt(_filter.getAt(0)) +
              (GLfloat) multiSpectral->getTransmittanceAt(_filter.getAt(1)) +
              (GLfloat) multiSpectral->getTransmittanceAt(_filter.getAt(2))) / 3;
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _rgba);

  /// We set the current color in the case of disabling the lighting
  glColor4fv(_rgba);
  */

  GEOM_GLRENDERER_UPDATE_APPEARANCE(multiSpectral);

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(NurbsCurve *nurbsCurve) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(nurbsCurve);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(NurbsPatch *nurbsPatch) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(nurbsPatch);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(Oriented *oriented) {
  GEOM_ASSERT_OBJ(oriented);


  PUSH_MODELMATRIX;

  Matrix4TransformationPtr _basis;
  _basis = dynamic_pointer_cast<Matrix4Transformation>(oriented->getTransformation());
  GEOM_ASSERT(_basis);

  __modelmatrix.transform(_basis->getMatrix());

  oriented->getGeometry()->apply(*this);

  POP_MODELMATRIX;

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(Paraboloid *paraboloid) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(paraboloid);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(PointSet *pointSet) {
  GEOM_ASSERT_OBJ(pointSet);

/*
  glPushAttrib(GL_LIGHTING_BIT);
  glDisable(GL_LIGHTING);

  if (!pointSet->isWidthToDefault()) {
    glPushAttrib(GL_POINT_BIT);
    int globalwidth = 1;
    glGetIntegerv(GL_POINT_SIZE, &globalwidth);
    glPointSize(float(pointSet->getWidth() + globalwidth - 1));
  }

  const Point3ArrayPtr &points = pointSet->getPointList();
  if (!points) return false;
  bool color = pointSet->hasColorList() &&
               (pointSet->getColorList()->size() == points->size());


  glEnableClientState(GL_VERTEX_ARRAY);
  real_t * vertices = points->data();
  glVertexPointer( 3, GL_GEOM_REAL, 0 ,vertices);
  uchar_t * colors = NULL;
  if( color ) {
    colors = pointSet->getColorList()->toUcharArray();
    glEnableClientState( GL_COLOR_ARRAY );
    glColorPointer( 4, GL_UNSIGNED_BYTE, 4*sizeof( uchar_t ), colors );
  }
  glDrawArrays(GL_POINTS, 0 , points->size() );

  if( color ) glDisableClientState( GL_COLOR_ARRAY );
  glDisableClientState(GL_VERTEX_ARRAY);
  delete [] vertices;
  if(colors)delete [] colors;

  if (!pointSet->isWidthToDefault()) { glPopAttrib(); }
  glPopAttrib();
*/

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */

bool QGLRenderer3::process(PGL(Polyline) *polyline) {
  GEOM_ASSERT_OBJ(polyline);

/*
  glPushAttrib(GL_LIGHTING_BIT);
  glDisable(GL_LIGHTING);

  BEGIN_LINE_WIDTH(polyline)

  const Point3ArrayPtr &points = polyline->getPointList();
  bool color = polyline->hasColorList() &&
               (polyline->getColorList()->size() == points->size());

  glEnableClientState(GL_VERTEX_ARRAY);
  real_t * vertices = points->data();
  glVertexPointer( 3, GL_GEOM_REAL,0,vertices);

  uchar_t * colors = NULL;
  if( color ) {
    colors = polyline->getColorList()->toUcharArray();
    glEnableClientState( GL_COLOR_ARRAY );
    glColorPointer( 4, GL_UNSIGNED_BYTE, 4*sizeof( uchar_t ), colors );
  }
  glDrawArrays(GL_LINE_STRIP, 0 , points->size() );

  if( color ) glDisableClientState( GL_COLOR_ARRAY );
  glDisableClientState(GL_VERTEX_ARRAY);
  delete [] vertices;
  if(colors)delete [] colors;

  END_LINE_WIDTH(polyline)
  glPopAttrib();
*/

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */




bool QGLRenderer3::process(QuadSet *quadSet) {
  GEOM_ASSERT_OBJ(quadSet);
  __tesselator.process(quadSet);
  __tesselator.getTriangulation()->apply(*this);
/*
  glFrontFace(quadSet->getCCW() ? GL_CCW : GL_CW);
  bool normalV = quadSet->getNormalPerVertex();
  bool tex = __appearance && __appearance->isTexture() && quadSet->hasTexCoordList();
  bool color = quadSet->hasColorList();
  bool colorV = quadSet->getColorPerVertex();

  if (__appearance &&
      __appearance->isTexture() &&
      !quadSet->hasTexCoordList()) {
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
  }

  quadSet->checkNormalList();

  glEnableClientState(GL_VERTEX_ARRAY);
  real_t * vertices = quadSet->getPointList()->data();
  glVertexPointer( 3, GL_GEOM_REAL,0,vertices);

  real_t * normals = NULL;
  if (normalV) {
    normals = quadSet->getNormalList()->data();
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer( GL_GEOM_REAL,0,normals);
  }

  real_t * texCoord = NULL;
  uchar_t * colors = NULL;
  if(tex){
    texCoord = quadSet->getTexCoordList()->data();
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer( 2, GL_GEOM_REAL,0,texCoord);
  }
  else if( color ) {
    colors = quadSet->getColorList()->toUcharArray();
    glEnableClientState( GL_COLOR_ARRAY );
    glColorPointer( 4, GL_UNSIGNED_BYTE, 4*sizeof( uchar_t ),  colors );
  }

  if (normalV){
    size_t si = quadSet->getIndexList()->size();
    uint_t * indices = quadSet->getIndexList()->data();
    glDrawElements( GL_QUADS, 4*si , GL_UNSIGNED_INT, indices);
    delete [] indices;
  }
  else {
    size_t _i = 0;
    for(Index4Array::const_iterator it = quadSet->getIndexList()->begin();
    it != quadSet->getIndexList()->end(); it++){
          glGeomNormal(quadSet->getNormalAt(_i));_i++;
          glDrawElements(   GL_QUADS, 4 , GL_UNSIGNED_INT, it->begin());
     }

  }

  glDisableClientState(GL_VERTEX_ARRAY);
  if (normalV)glDisableClientState(GL_NORMAL_ARRAY);
  if(tex)glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  if( color )glDisableClientState(GL_COLOR_ARRAY);

  delete [] vertices;
  if(normals)  delete [] normals;
  if(texCoord) delete [] texCoord;
  if(colors)   delete [] colors;

  if (__appearance &&
      __appearance->isTexture() &&
      !quadSet->hasTexCoordList()) {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
  }
  */
  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(Revolution *revolution) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(revolution);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(Swung *swung) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(swung);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(Scaled *scaled) {
  GEOM_ASSERT_OBJ(scaled);

  PUSH_MODELMATRIX;

  __modelmatrix.scale(scaled->getScale());

  scaled->getGeometry()->apply(*this);

  POP_MODELMATRIX;

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(ScreenProjected *scp) {
  GEOM_ASSERT_OBJ(scp);
/*
  if (__Mode == Selection) {
    return true;
  }
  GEOM_GLRENDERER_PRECOMPILE_BEG(scp);
    GEOM_GLRENDERER_PRECOMPILE_SUB(scp->getGeometry());
  GEOM_GLRENDERER_PRECOMPILE_END(scp);

  GEOM_GLRENDERER_CHECK_CACHE(scp);
  real_t heigthscale = 1.0;
#ifndef PGL_WITHOUT_QT
  if (__glframe != NULL && scp->getKeepAspectRatio()) {
    heigthscale = float(__glframe->height()) / float(__glframe->width());
  }
#endif

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(-1, 1, -heigthscale, heigthscale, 1, -1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  bool lighting = glIsEnabled(GL_LIGHTING);
  if (lighting) {
    glPushAttrib(GL_LIGHTING_BIT);
    glGeomLightPosition(GL_LIGHT0, Vector3(0, 0, -1));
    glGeomLightDirection(GL_LIGHT0, Vector3(0, 0, 1));
  }

  scp->getGeometry()->apply(*this);

  if (lighting) glPopAttrib();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();


  */
  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(Sphere *sphere) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(sphere);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(Tapered *tapered) {
  GEOM_ASSERT_OBJ(tapered);

  PrimitivePtr _primitive = tapered->getPrimitive();
  if (_primitive->apply(__discretizer)) {
    ExplicitModelPtr _explicit = __discretizer.getDiscretization();

    Transformation3DPtr _taper(tapered->getTransformation());
    ExplicitModelPtr _tExplicit = _explicit->transform(_taper);
    _tExplicit->apply(*this);

    return true;
  } 
  else 
    return false;
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(Translated *translated) {
  GEOM_ASSERT_OBJ(translated);

  PUSH_MODELMATRIX;
  __modelmatrix.translate(translated->getTranslation());
  translated->getGeometry()->apply(*this);
  POP_MODELMATRIX;

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */
using namespace std;


#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

QMatrix4x4 toQMatrix44(const Matrix4& matrix){
    float data[16];
    float * itd = data;
    for(Matrix4::const_iterator it = matrix.begin(); it != matrix.end(); ++it, ++itd){
        *itd = *it;
    }
    return QMatrix4x4(data).transposed();
}

QMatrix3x3 toQMatrix33(const Matrix4& matrix){
    float data[9];
    float * itd = data;
    for(Matrix4::const_iterator it = matrix.begin(); it != matrix.begin()+3; ++it, ++itd){
        *itd = *it;
    }
    for(Matrix4::const_iterator it = matrix.begin()+4; it != matrix.begin()+7; ++it, ++itd){
        *itd = *it;
    }
    for(Matrix4::const_iterator it = matrix.begin()+8; it != matrix.begin()+11; ++it, ++itd){
        *itd = *it;
    }
    return QMatrix3x3(data);
}

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

static const char *vertexShaderSourceCore =
    "#version 150\n"
    "in vec4 vertex;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 modvMatrix;\n"
    "void main() {\n"
    "   gl_Position = projMatrix * modvMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSourceCore =
    "#version 150\n"
    "out highp vec4 fragColor;\n"
    "void main() {\n"
    "   fragColor = vec4(1.0, 0.5, 0.0, 1.0);\n"
    "}\n";

// /////////////////////////////////////////////////////////////////////////////
// main
// /////////////////////////////////////////////////////////////////////////////

bool QGLRenderer3::process(TriangleSet *triangleSet)
{
// /////////////////////////////////////////////////////////////////////////////
// Logging
// /////////////////////////////////////////////////////////////////////////////

  static QOpenGLDebugLogger *logger = nullptr;

    if(!logger) {
      logger = new QOpenGLDebugLogger;
      logger->initialize();

      QObject::connect(logger, &QOpenGLDebugLogger::messageLogged, [=](const QOpenGLDebugMessage &message) -> void {
                                                                     qDebug() << Q_FUNC_INFO << message;
                                                                   });

      logger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
    }

// /////////////////////////////////////////////////////////////////////////////
// Init - program
// /////////////////////////////////////////////////////////////////////////////

  static QOpenGLShaderProgram *program = 0;

  if(!program) {
    program = new QOpenGLShaderProgram;
    program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSourceCore);
    program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSourceCore);
    program->bindAttributeLocation("vertex", 0);
    program->link();
  }

  program->bind();

  GLuint projMatrixID = program->uniformLocation("projMatrix");
  GLuint modvMatrixID = program->uniformLocation("modvMatrix");

// /////////////////////////////////////////////////////////////////////////////
// Init - VAO
// /////////////////////////////////////////////////////////////////////////////

    static QOpenGLVertexArrayObject * m_vao = nullptr;

    if(!m_vao) {
      m_vao = new QOpenGLVertexArrayObject;
      m_vao->create();
    }

    static QOpenGLVertexArrayObject::Binder *vaoBinder = new QOpenGLVertexArrayObject::Binder(m_vao);

// /////////////////////////////////////////////////////////////////////////////
// Init - DS
// /////////////////////////////////////////////////////////////////////////////

    real_t * vertices = triangleSet->getPointList()->data();
    uint_t * indices = triangleSet->getIndexList()->data();

// /////////////////////////////////////////////////////////////////////////////
// Init - VBO
// /////////////////////////////////////////////////////////////////////////////

    static QOpenGLBuffer *vertexBuf = nullptr;

    if(!vertexBuf) {
      vertexBuf = new QOpenGLBuffer( QOpenGLBuffer::VertexBuffer);
      vertexBuf->setUsagePattern(QOpenGLBuffer::StaticDraw);

      vertexBuf->create();
      vertexBuf->bind();
    }

    vertexBuf->allocate(vertices, triangleSet->getPointList()->size() * 3 * sizeof(real_t));
    vertexBuf->write(0, vertices, triangleSet->getPointList()->size() * 3 * sizeof(real_t));

    for(int i = 0; i < triangleSet->getPointList()->size(); i++)
      qDebug() << Q_FUNC_INFO
               << i
               << vertices[3*i+0]
               << vertices[3*i+1]
               << vertices[3*i+2];

    qDebug() << Q_FUNC_INFO << vertices << triangleSet->getPointList()->size();

// /////////////////////////////////////////////////////////////////////////////
// Init - IBO
// /////////////////////////////////////////////////////////////////////////////

    static QOpenGLBuffer *indexBuf = nullptr;

    if(!indexBuf) {
      indexBuf = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
      indexBuf->create();
      indexBuf->bind();
    }

    indexBuf->allocate(indices, triangleSet->getIndexList()->size() * 3* sizeof(uint_t));
    //indexBuf->write(0, indices, triangleSet->getIndexList()->size() * 3 * sizeof(uint_t));

    for(int i = 0; i < triangleSet->getIndexList()->size(); i++)
      qDebug() << Q_FUNC_INFO
               << i
               << indices[3*i+0]
               << indices[3*i+1]
               << indices[3*i+2];

// /////////////////////////////////////////////////////////////////////////////
// Init - attributes
// /////////////////////////////////////////////////////////////////////////////

    __ogl.glEnableVertexAttribArray(0);
    vertexBuf->bind();
    // __ogl.glVertexAttribPointer(0, 3, GL_GEOM_REAL, GL_FALSE, 3 * sizeof(real_t), 0);
    __ogl.glVertexAttribPointer(0, 3, GL_GEOM_REAL, GL_FALSE, 0, 0);

    indexBuf->bind();

// /////////////////////////////////////////////////////////////////////////////

    // program->release();

// /////////////////////////////////////////////////////////////////////////////
// Actual rendering
// /////////////////////////////////////////////////////////////////////////////

    static int frame = 0;

    qDebug() << Q_FUNC_INFO << "Rendering frame" << frame++;

    double m_xRot = 0;
    double m_yRot = 0;
    double m_zRot = 0;

    /*QMatrix4x4 m_camera;
    m_camera.setToIdentity();
    m_camera.translate(0, 0, 0);
    */
    QMatrix4x4 m_world;
    m_world.setToIdentity();
    /*m_world.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
    m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
    m_world.rotate(m_zRot / 16.0f, 0, 0, 1);*/

    // program->bind();
    // QMatrix4x4 proj = toQMatrix44(__projection);
    program->setUniformValue(projMatrixID, __projection);
    program->setUniformValue(modvMatrixID, m_world);

    for(int i = 0; i < 4; i++)
      qDebug() << Q_FUNC_INFO
               << "proj"
               << __projection(i,0)
               << __projection(i,1)
               << __projection(i,2)
               << __projection(i,3);

      qDebug() << Q_FUNC_INFO
               << "nbtr"
               << triangleSet->getIndexList()->size()*3;

    //__ogl.glDrawArrays(GL_TRIANGLES, 0, triangleSet->getIndexList()->size()*3);
    __ogl.glDrawElements(GL_TRIANGLES, triangleSet->getIndexList()->size()*3, GL_UNSIGNED_INT, (void *)0);

    program->release();

    return true;
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(BezierCurve2D *bezierCurve) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(bezierCurve);
}

/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(Disc *disc) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(disc);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(NurbsCurve2D *nurbsCurve) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(nurbsCurve);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(PointSet2D *pointSet) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(pointSet);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer3::process(Polyline2D *polyline) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(polyline);
}


/* ----------------------------------------------------------------------- */

bool QGLRenderer3::process(Text *text) {
  GEOM_ASSERT_OBJ(text);

    /*

    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    if (!text->getString().empty()) {
      QFont f;
      if (text->getFontStyle()) {
        const FontPtr &font = text->getFontStyle();
        if (!font->getFamily().empty())
          f.setFamily(font->getFamily().c_str());
        f.setPointSize(font->getSize());
        f.setBold(font->getBold()),
                f.setItalic(font->getItalic());
      } else {
        f.setPointSize(Font::DEFAULT_SIZE);
      }
      if (__glframe != NULL) {
        if (text->getScreenCoordinates()) {
          glMatrixMode(GL_PROJECTION);
          glPushMatrix();
          glLoadIdentity();
          glOrtho(0, 100, 0, 100, 0, -100);
          glMatrixMode(GL_MODELVIEW);
          glPushMatrix();
          glLoadIdentity();
        }

        const Vector3 &position = text->getPosition();
        __glframe->renderText(position.x(), position.y(), position.z(), QString(text->getString().c_str()), f);

        if (text->getScreenCoordinates()) {
          glMatrixMode(GL_PROJECTION);
          glPopMatrix();
          glMatrixMode(GL_MODELVIEW);
          glPopMatrix();
        }

      }
    }
    glPopAttrib();
    */
  return true;
}



/* ----------------------------------------------------------------------- */

bool QGLRenderer3::process(Font *font) {
  GEOM_ASSERT_OBJ(font);
  return true;
}

/* ----------------------------------------------------------------------- */
