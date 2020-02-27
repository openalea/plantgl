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




#include "qglrenderer.h"
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
    #define GEOM_ASSERT_OBJ(obj) printf("Look at %sobject %zu of type '%s'\n", (!obj->unique()?"shared ":""),obj->getObjectId(), typeid(*obj).name());
#else
    #define GEOM_ASSERT_OBJ(obj)
#endif

#define GEOM_GLRENDERER_CHECK_APPEARANCE(app) \
  if (__appearance.get() == (Appearance *)app) return true;

#define GEOM_GLRENDERER_UPDATE_APPEARANCE(app) \
  __appearance = AppearancePtr(app);


template<class T>
bool QGLRenderer::discretize_and_render(T *geom) {
  GEOM_ASSERT_OBJ(geom);
  bool b = false;
  GeometryCache::Iterator it = __geometrycache.find((uint_t)geom->getObjectId());
  if (it != __geometrycache.end()) {
     b = it->second->apply(*this);
  }
  else {
      if (__appearance && __appearance->isTexture())
        __discretizer.computeTexCoord(true);
      else __discretizer.computeTexCoord(false);
      b = geom->apply(__discretizer);
      if (b && (b = (__discretizer.getDiscretization()))) {
        __geometrycache.insert((uint_t)geom->getObjectId(), __discretizer.getDiscretization());
        b = __discretizer.getDiscretization()->apply(*this);
      }
  }
  return b;
}


#define QGLRENDERER_DISCRETIZE_N_RENDER(geom) \
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


QGLRenderer::QGLRenderer(Discretizer& discretizer,
                           Tesselator& tesselator) :
        Action(),
        __projectionmatrix(),
        __viewmatrix(),
        __lightPosition(0,0,10),
        __lightAmbient(255,255,255),
        __lightDiffuse(255,255,255),
        __lightSpecular(255,255,255),
        __lightEnabled(false),
        __discretizer(discretizer),
        __tesselator(tesselator),
        __appearance(),
        __materialprogram(0),
        __colorprogram(0),
        __textureprogram(0),
        __currentprogram(0){
}


void QGLRenderer::setViewProjection(const QMatrix4x4& view, const QMatrix4x4& projection )
{
        __projectionmatrix = projection;
        __viewmatrix = view;
}


QGLRenderer::~QGLRenderer() {
  clear();
}

Discretizer &
QGLRenderer::getDiscretizer() {
  return __discretizer;
}


static const char *vertexShaderSourceCore =
    "#version 150\n"
    "in vec3 vertex;\n"
    "in vec3 normal;\n"
    "\n"
    "out vec3 fragPos;\n"
    "out vec3 fragNormal;\n"
    "out vec3 viewPos;\n"
    "\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "\n"
    "vec3 origin = vec3(0.0);\n"
    "\n"
    "void main() {\n"
    "   viewPos = vec3(inverse(view) * vec4(origin, 1.0));\n"
    "   fragPos = vec3(model * vec4(vertex, 1.0));\n"
    "   fragNormal = mat3(transpose(inverse(model))) * normal;\n"
    "   gl_Position = projection * view * vec4(fragPos, 1.0);\n"
    "}\n";



static const char *fragmentShaderSourceCore =
    "#version 150\n"
    "struct Material {\n"
    "    vec3 ambient;\n"
    "    vec3 diffuse;\n"
    "    vec3 specular;\n"    
    "    float shininess;\n"
    "    float transparency;\n"
    "};\n" 
    "\n"
    "struct Light {\n"
    "    vec3 position;\n"
    "\n"
    "    vec3 ambient;\n"
    "    vec3 diffuse;\n"
    "    vec3 specular;\n"
    "};\n"
    "\n"
    "in vec3 fragPos;\n"
    "in vec3 fragNormal;\n" 
    "in vec3 viewPos;\n"
    "\n"
    "out vec4 fragColor;\n"
    "\n"
    "uniform Material material;\n"
    "uniform Light light;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    // ambient\n"
    "    vec3 ambient = light.ambient * material.ambient;\n"
    "\n"    
    "    // diffuse \n"
    "    vec3 norm = normalize(fragNormal);\n"
    "    vec3 lightDir = normalize(light.position - fragPos);\n"
    "    float diff = dot(norm, lightDir); //, 0.0);\n"
    "    vec3 diffuse = light.diffuse * (diff * material.diffuse);\n"
    "\n"    
    "    // specular\n"
    "    vec3 viewDir = normalize(viewPos - fragPos);\n"
    "    vec3 reflectDir = reflect(-lightDir, norm);\n"  
    "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
    "    vec3 specular = light.specular * (spec * material.specular);  \n"
    "\n"        
    "    vec3 result = ambient + diffuse + specular;\n"
    "    fragColor = vec4(result, 1.0 - material.transparency);\n"
    "} \n";


void QGLRenderer::init() {

    __ogl.initializeOpenGLFunctions();

    /*
  static QOpenGLDebugLogger *logger = nullptr;

    if(!logger) {
      logger = new QOpenGLDebugLogger;
      logger->initialize();

      QObject::connect(logger, &QOpenGLDebugLogger::messageLogged, [=](const QOpenGLDebugMessage &message) -> void {
                                                                     qDebug() << Q_FUNC_INFO << message;
                                                                   });

      logger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
    }
    */

// /////////////////////////////////////////////////////////////////////////////
// Init - program
// /////////////////////////////////////////////////////////////////////////////

  if(!__materialprogram) {
    __materialprogram = new QOpenGLShaderProgram;
    __materialprogram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSourceCore);
    __materialprogram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSourceCore);
    __materialprogram->bindAttributeLocation("vertex", 0);
    __materialprogram->bindAttributeLocation("normal", 1);
    __materialprogram->link();
   } 




}

void QGLRenderer::clear() {

    __geometrycache.clear();
    for(BufferCache::Iterator it = __buffercache.begin(); it != __buffercache.end(); ++it){
        if(it->second.vertexBuf) delete it->second.vertexBuf;
        if(it->second.normalBuf) delete it->second.normalBuf;
        if(it->second.colorBuf) delete it->second.colorBuf;
        if(it->second.indexBuf) delete it->second.indexBuf;
    }
    __buffercache.clear();

}



/* ----------------------------------------------------------------------- */




bool QGLRenderer::beginProcess() {

    __materialprogram->bind();

    GLuint viewMatrixID = __materialprogram->uniformLocation("view");
    __materialprogram->setUniformValue(viewMatrixID, __viewmatrix);

    GLuint projMatrixID = __materialprogram->uniformLocation("projection");
    __materialprogram->setUniformValue(projMatrixID, __projectionmatrix);

    GLuint lightPositionID = __materialprogram->uniformLocation("light.position");
    __materialprogram->setUniformValue(lightPositionID, QVector3D(__lightPosition.x(),__lightPosition.y(),__lightPosition.z() ));

    GLuint lightAmbientID = __materialprogram->uniformLocation("light.ambient");
    __materialprogram->setUniformValue(lightAmbientID, QVector3D(__lightAmbient.getRedClamped(),__lightAmbient.getGreenClamped(),__lightAmbient.getBlueClamped() ));

    GLuint lightDiffuseID = __materialprogram->uniformLocation("light.diffuse");
    __materialprogram->setUniformValue(lightDiffuseID, QVector3D(__lightDiffuse.getRedClamped(),__lightDiffuse.getGreenClamped(),__lightDiffuse.getBlueClamped() ));

    GLuint lightSpecularID = __materialprogram->uniformLocation("light.specular");
    __materialprogram->setUniformValue(lightSpecularID, QVector3D(__lightSpecular.getRedClamped(),__lightSpecular.getGreenClamped(),__lightSpecular.getBlueClamped() ));

  return true;
}


bool QGLRenderer::endProcess() {
  __appearance = AppearancePtr();
  return true;
}

/* ----------------------------------------------------------------------- */
bool QGLRenderer::process(Shape *geomshape) {
  GEOM_ASSERT_OBJ(geomshape);
  processAppereance(geomshape);
  return processGeometry(geomshape);
}

bool QGLRenderer::processAppereance(Shape *geomshape) {
  GEOM_ASSERT(geomshape);
  if (geomshape->appearance) {
    return geomshape->appearance->apply(*this);
  } else return Material::DEFAULT_MATERIAL->apply(*this);
}

bool QGLRenderer::processGeometry(Shape *geomshape) 
{
  GEOM_ASSERT(geomshape);
  return geomshape->geometry->apply(*this);

  assert(glGetError() == GL_NO_ERROR);
}

/* ----------------------------------------------------------------------- */

bool QGLRenderer::process(Inline *geomInline) {
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

bool QGLRenderer::process(AmapSymbol *amapSymbol) {
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


bool QGLRenderer::process(AsymmetricHull *asymmetricHull) {
  QGLRENDERER_DISCRETIZE_N_RENDER(asymmetricHull);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(AxisRotated *axisRotated) {
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


bool QGLRenderer::process(BezierCurve *bezierCurve) {
  QGLRENDERER_DISCRETIZE_N_RENDER(bezierCurve);
}

/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(BezierPatch *bezierPatch) {
  QGLRENDERER_DISCRETIZE_N_RENDER(bezierPatch);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(Box *box) {
  QGLRENDERER_DISCRETIZE_N_RENDER(box);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(Cone *cone) {
  QGLRENDERER_DISCRETIZE_N_RENDER(cone);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(Cylinder *cylinder) {
  QGLRENDERER_DISCRETIZE_N_RENDER(cylinder);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(ElevationGrid *elevationGrid) {
  QGLRENDERER_DISCRETIZE_N_RENDER(elevationGrid);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(EulerRotated *eulerRotated) {
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


bool QGLRenderer::process(ExtrudedHull *extrudedHull) {
  QGLRENDERER_DISCRETIZE_N_RENDER(extrudedHull);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(FaceSet *faceSet) {
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


bool QGLRenderer::process(Frustum *frustum) {
  QGLRENDERER_DISCRETIZE_N_RENDER(frustum);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(Extrusion *extrusion) {
  QGLRENDERER_DISCRETIZE_N_RENDER(extrusion);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(Group *group) {
  GEOM_ASSERT_OBJ(group);
    
  group->getGeometryList()->apply(*this);

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(IFS *ifs) {
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


bool QGLRenderer::process(Material *material) {
  GEOM_ASSERT_OBJ(material);


  GEOM_GLRENDERER_CHECK_APPEARANCE(material);

    __materialprogram->bind();

    GLuint ambientId = __materialprogram->uniformLocation("material.ambient");
    __materialprogram->setUniformValue(ambientId, QVector3D((float)material->getAmbient().getRedClamped(),(float)material->getAmbient().getGreenClamped(),(float)material->getAmbient().getBlueClamped() ));

    GLuint diffuseId = __materialprogram->uniformLocation("material.diffuse");
    __materialprogram->setUniformValue(diffuseId, QVector3D((float)material->getAmbient().getRedClamped()*material->getDiffuse(),
                                                            (float)material->getAmbient().getGreenClamped()*material->getDiffuse(),
                                                            (float)material->getAmbient().getBlueClamped()*material->getDiffuse() ));

    GLuint specularId = __materialprogram->uniformLocation("material.specular");
    __materialprogram->setUniformValue(specularId, QVector3D((float)material->getSpecular().getRedClamped(),(float)material->getSpecular().getGreenClamped(),(float)material->getSpecular().getBlueClamped() ));

    GLuint shininessId = __materialprogram->uniformLocation("material.shininess");
    __materialprogram->setUniformValue(shininessId, (float)material->getShininess() );

    GLuint transparencyId = __materialprogram->uniformLocation("material.transparency");
    __materialprogram->setUniformValue(transparencyId, (float)material->getTransparency() );

    __currentprogram = __materialprogram;


    /// What about emission ???

  GEOM_GLRENDERER_UPDATE_APPEARANCE(material);

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */
bool QGLRenderer::process(ImageTexture *texture) {
  GEOM_ASSERT_OBJ(texture);
  GEOM_GLRENDERER_CHECK_APPEARANCE(texture);

/*

  Cache<GLuint>::Iterator it = __cachetexture.find(texture->getObjectId());
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
        __cachetexture.insert(texture->getObjectId(), id);
      }
    }
#endif
  }
  */

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */

bool QGLRenderer::process(Texture2D *texture) {
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

bool QGLRenderer::process(Texture2DTransformation *texturetransfo) {
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


bool QGLRenderer::process(MonoSpectral *monoSpectral) {
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


bool QGLRenderer::process(MultiSpectral *multiSpectral) {
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


bool QGLRenderer::process(NurbsCurve *nurbsCurve) {
  QGLRENDERER_DISCRETIZE_N_RENDER(nurbsCurve);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(NurbsPatch *nurbsPatch) {
  QGLRENDERER_DISCRETIZE_N_RENDER(nurbsPatch);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(Oriented *oriented) {
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


bool QGLRenderer::process(Paraboloid *paraboloid) {
  QGLRENDERER_DISCRETIZE_N_RENDER(paraboloid);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(PointSet *pointSet) {
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

bool QGLRenderer::process(PGL(Polyline) *polyline) {
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




bool QGLRenderer::process(QuadSet *quadSet) {
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


bool QGLRenderer::process(Revolution *revolution) {
  QGLRENDERER_DISCRETIZE_N_RENDER(revolution);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(Swung *swung) {
  QGLRENDERER_DISCRETIZE_N_RENDER(swung);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(Scaled *scaled) {
  GEOM_ASSERT_OBJ(scaled);

  PUSH_MODELMATRIX;

  __modelmatrix.scale(scaled->getScale());

  scaled->getGeometry()->apply(*this);

  POP_MODELMATRIX;

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(ScreenProjected *scp) {
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


bool QGLRenderer::process(Sphere *sphere) {
  QGLRENDERER_DISCRETIZE_N_RENDER(sphere);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(Tapered *tapered) {
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


bool QGLRenderer::process(Translated *translated) {
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
    return QMatrix4x4(data); // .transposed();
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


// /////////////////////////////////////////////////////////////////////////////
// main
// /////////////////////////////////////////////////////////////////////////////


bool QGLRenderer::process(TriangleSet *triangleSet)
{
    qDebug() << Q_FUNC_INFO << triangleSet << triangleSet->getPointList()->size();

    QOpenGLVertexArrayObject * m_vao = nullptr;
    QOpenGLBuffer *vertexBuf = nullptr;
    QOpenGLBuffer *normalBuf = nullptr;
    QOpenGLBuffer *colorBuf = nullptr;
    QOpenGLBuffer *indexBuf = nullptr;

    // QMatrix4x4 model = toQMatrix44(__modelmatrix.getMatrix());
    // for(int i = 0; i < 4; i++)
    //    qDebug() << Q_FUNC_INFO
    //             << "m" << model(i,0) << model(i,1) << model(i,2) << model(i,3);
    
     __currentprogram->bind();
    GLuint modelMatrixID = __currentprogram->uniformLocation("model");
    __currentprogram->setUniformValue(modelMatrixID, toQMatrix44(__modelmatrix.getMatrix()));


    BufferCache::Iterator _it = __buffercache.find((uint_t)triangleSet->getObjectId()); 
    bool tocreate = _it == __buffercache.end();
    if (tocreate) {
      m_vao = new QOpenGLVertexArrayObject;
      m_vao->create();
    }
    else {
      m_vao = _it->second.vao;
    }

    QOpenGLVertexArrayObject::Binder vaoBinder (m_vao);

    if (tocreate) {
        printf("Creating buffers for %u\n", (uint_t)triangleSet->getObjectId());


      triangleSet->checkNormalList();

      real_t * vertices = triangleSet->getPointList()->data();
      real_t * normals = triangleSet->getNormalList()->data();
      uint_t * indices = triangleSet->getIndexList()->data();

      real_t * itvert =  vertices;
      for(uint_t i = 0; i< triangleSet->getPointList()->size(); ++i){
            for(uint_t j = 0; j<3; ++j) {
                printf("%f ",*itvert); ++itvert;
            }
            printf("\n");
        }
      printf("\n");
      uint_t * itindex =  indices;
      for(uint_t i = 0; i< triangleSet->getIndexList()->size(); ++i){
            for(uint_t j = 0; j<3; ++j) {
                printf("%u ",*itindex); ++itindex;
            }
            printf("\n");
        }


      vertexBuf = new QOpenGLBuffer( QOpenGLBuffer::VertexBuffer);

      vertexBuf->create();
      vertexBuf->bind();
      vertexBuf->setUsagePattern(QOpenGLBuffer::StaticDraw);
      vertexBuf->allocate(vertices, triangleSet->getPointList()->size() * 3 * sizeof(real_t));
      vertexBuf->release();

      normalBuf = new QOpenGLBuffer( QOpenGLBuffer::VertexBuffer);

      normalBuf->create();
      normalBuf->bind();
      normalBuf->setUsagePattern(QOpenGLBuffer::StaticDraw);
      normalBuf->allocate(normals, triangleSet->getNormalList()->size() * 3 * sizeof(real_t));
      vertexBuf->release();

      indexBuf = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
      indexBuf->create();
      indexBuf->bind();
      indexBuf->allocate(indices, triangleSet->getIndexList()->size() * 3 * sizeof(uint_t));
      indexBuf->release();
      
      printf("Storing buffers in cache\n");

      BufferInfo mcache;
      mcache.vao = m_vao;
      mcache.vertexBuf = vertexBuf;
      mcache.normalBuf = normalBuf;
      mcache.colorBuf = colorBuf;
      mcache.indexBuf = indexBuf;

      __buffercache.insert((uint_t) triangleSet->getObjectId(), mcache);

      if (vertices) delete [] vertices;
      if (normals) delete [] normals;
      if (indices) delete [] indices;

      printf("done\n");
    }
    else {
      // printf("Reusing buffers\n");
      BufferInfo mcache = _it->second;

      m_vao = mcache.vao;
      vertexBuf = mcache.vertexBuf;
      normalBuf = mcache.normalBuf;
      indexBuf = mcache.indexBuf;

    }

    // printf("Enabling vertex attributes\n");
    vertexBuf->bind();
    normalBuf->bind();
    __ogl.glEnableVertexAttribArray(0);
    __ogl.glEnableVertexAttribArray(1);

    // printf("Vertex attributes types\n");
    // __ogl.glVertexAttribPointer(0, 3, GL_GEOM_REAL, GL_FALSE, 3 * sizeof(real_t), 0);
    __ogl.glVertexAttribPointer(0, 3, GL_GEOM_REAL, GL_FALSE, 0, 0);
    __ogl.glVertexAttribPointer(1, 3, GL_GEOM_REAL, GL_TRUE, 0, 0);


    // printf("Draw elements\n");
    indexBuf->bind();
    //__ogl.glDrawArrays(GL_TRIANGLES, 0, triangleSet->getIndexList()->size()*3);
    __ogl.glDrawElements(GL_TRIANGLES, triangleSet->getIndexList()->size()*3, GL_UNSIGNED_INT, (void *)0);

    // printf("end tr\n");

    return true;
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(BezierCurve2D *bezierCurve) {
  QGLRENDERER_DISCRETIZE_N_RENDER(bezierCurve);
}

/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(Disc *disc) {
  QGLRENDERER_DISCRETIZE_N_RENDER(disc);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(NurbsCurve2D *nurbsCurve) {
  QGLRENDERER_DISCRETIZE_N_RENDER(nurbsCurve);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(PointSet2D *pointSet) {
  QGLRENDERER_DISCRETIZE_N_RENDER(pointSet);
}


/* ----------------------------------------------------------------------- */


bool QGLRenderer::process(Polyline2D *polyline) {
  QGLRENDERER_DISCRETIZE_N_RENDER(polyline);
}


/* ----------------------------------------------------------------------- */

bool QGLRenderer::process(Text *text) {
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

bool QGLRenderer::process(Font *font) {
  GEOM_ASSERT_OBJ(font);
  return true;
}

/* ----------------------------------------------------------------------- */
