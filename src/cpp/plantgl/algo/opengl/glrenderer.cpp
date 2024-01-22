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





#include "glrenderer.h"
#include <plantgl/algo/base/discretizer.h>

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
#include <plantgl/gui/pglqopenglwidget.h>
#include <QWidget>

#include <QtGui/QImage>
#include <QtGui/QFont>

#endif

#include <typeinfo>
#include <plantgl/math/util_math.h>


#ifndef GL_GENERATE_MIPMAP
#define GL_GENERATE_MIPMAP 0x8191
#endif

PGL_USING_NAMESPACE

// #define GEOM_DLDEBUG
// #define GEOM_TREECALLDEBUG

// #define WITH_PRECOMPILING

#define WITH_VERTEXARRAY
// #define PGL_OLD_MIPMAP_STYLE

#ifdef GEOM_TREECALLDEBUG
#define GEOM_ASSERT_OBJ(obj) printf("Look at %sobject %zu of type '%s' in mode %i\n", (!obj->unique()?"shared ":""),obj->getObjectId(), typeid(*obj).name(), __compil);
#else
#define GEOM_ASSERT_OBJ(obj) if (__ogltoinit) { init(); }
#endif

/* ----------------------------------------------------------------------- */

#define ePreCompileMode 1024
#define MAXPRECOMPILDEPTH 100

#ifdef WITH_PRECOMPILING

#define GEOM_GLRENDERER_PRECOMPILE_INIT(sh) \
    int prevmode = __compil; \
    __compil = ePreCompileMode; \
    __executionmode = GL_COMPILE; \
    __precompildepth = 0; \

#define GEOM_GLRENDERER_PRECOMPILE_INITEND \
    __compil = prevmode; \
    __executionmode = GL_COMPILE_AND_EXECUTE; \

#define GEOM_GLRENDERER_PRECOMPILE_BEG(geom) \
    if (__compil == ePreCompileMode) { \
        if (!geom->unique()) { \
            Cache<GLuint>::Iterator _it = __cache.find((uint_t)geom->getObjectId()); \
            if (_it != __cache.end()) return true; \
            ++__precompildepth; \
        } \

#define GEOM_GLRENDERER_PRECOMPILE_END(geom) \
        if (geom->unique()) return true; \
        else __compil = 0; \
    } \

#define GEOM_GLRENDERER_PRECOMPILE_SUB(subgeom) \
    if (__precompildepth < __maxprecompildepth) { \
        subgeom->apply(*this); \
        __compil = ePreCompileMode; \
    } \

#else

#define GEOM_GLRENDERER_PRECOMPILE_INIT(sh)
#define GEOM_GLRENDERER_PRECOMPILE_INITEND
#define GEOM_GLRENDERER_PRECOMPILE_BEG(geom)
#define GEOM_GLRENDERER_PRECOMPILE_END(geom)
#define GEOM_GLRENDERER_PRECOMPILE_SUB(subgeom)

#endif

#define GEOM_GLRENDERER_PRECOMPILE(geom) \
        GEOM_GLRENDERER_PRECOMPILE_BEG(geom) \
        GEOM_GLRENDERER_PRECOMPILE_END(geom) \


#define GEOM_GLRENDERER_CHECK_CACHE(geom) \
  GLuint _displaylist = 0; \
  if (!geom->unique()) { \
        if (__compil == 0) { \
            if (check(geom->getObjectId(),_displaylist)) return true; \
        }  \
        else if (call(geom->getObjectId()))return true; \
  } \


#define GEOM_GLRENDERER_UPDATE_CACHE(geom) \
  if (__compil == 0 && !geom->unique()) update(geom->getObjectId(),_displaylist); \

#define GEOM_GLRENDERER_CHECK_APPEARANCE(app) \
  if (__appearance.get() == app) return true;

#define GEOM_GLRENDERER_UPDATE_APPEARANCE(app) \
  __appearance = AppearancePtr(app);

template<class T>
bool GLRenderer::discretize_and_render(T *geom) {
  GEOM_ASSERT_OBJ(geom);
  GEOM_GLRENDERER_CHECK_CACHE(geom);
  if (__appearance && __appearance->isTexture())
       __discretizer.computeTexCoord(true);
  else __discretizer.computeTexCoord(false);
  bool b = geom->apply(__discretizer);
  if ( b && (b = (__discretizer.getDiscretization())) ) {
      b = __discretizer.getDiscretization()->apply(*this);
  }
  GEOM_GLRENDERER_UPDATE_CACHE(geom);
  return b;
}

#define GEOM_GLRENDERER_DISCRETIZE_RENDER(geom) \
  GEOM_ASSERT_OBJ(geom); \
  GEOM_GLRENDERER_PRECOMPILE(geom); \
  return discretize_and_render(geom); \


#define BEGIN_LINE_WIDTH(obj) \
  if (!obj->isWidthToDefault()) { \
    __ogl->glPushAttrib (GL_LINE_BIT); \
    int globalwidth = 1; \
    __ogl->glGetIntegerv(GL_LINE_WIDTH,&globalwidth); \
    __ogl->glLineWidth(float(obj->getWidth()+globalwidth-1)); \
  } \

#define END_LINE_WIDTH(obj) if (!obj->isWidthToDefault()) { __ogl->glPopAttrib(); }

#define  GL_PUSH_MATRIX(geom) \
   bool mypushpop = __dopushpop; \
   if (mypushpop) __ogl->glPushMatrix(); \
   __dopushpop = is_null_ptr(dynamic_pointer_cast<Transformed>(geom));

#define  GL_POP_MATRIX(geom) \
   if (mypushpop) __ogl->glPopMatrix(); \
   __dopushpop = mypushpop;

/* ----------------------------------------------------------------------- */

bool glError(const char * file = NULL, int line = 0)
{
  GLenum _glerror;

  if((_glerror = glGetError()) != GL_NO_ERROR){
      int i = 0;
      while(_glerror != GL_NO_ERROR && i < 10){
          printf("[GLRenderer] GL Error [%i] : '%s' ; File : '%s' ; Line : %i;\n", _glerror, (const char *)gluErrorString(_glerror), file, line);
          _glerror = glGetError();
          i++;
      }
      return true;
  }
  return false;
}

#define GL_ERROR_CHECK  glError(__FILE__,__LINE__)


GLRenderer::GLRenderer(Discretizer &discretizer,
#ifndef PGL_WITHOUT_QT
    QOpenGLBaseWidget *glframe,
#endif
  PGLOpenGLFunctionsPtr ogl
) :
    Action(),
    __scenecache(0),
    __discretizer(discretizer),
    __appearance(),
    __Mode(Normal),
    __selectMode(ShapeId),
    __compil(0),
#ifndef PGL_WITHOUT_QT
    __glframe(glframe),
#endif
    __currentdisplaylist(false),
    __dopushpop(true),
    __executionmode(GL_COMPILE_AND_EXECUTE),
    __maxprecompildepth(MAXPRECOMPILDEPTH),
    __withvertexarray(false),
    __ogl(!ogl?PGLOpenGLFunctionsPtr(new PGLOpenGLFunctions()):ogl),__ogltoinit(!ogl) {
}


GLRenderer::~GLRenderer() {
    clear();
}

Discretizer &
GLRenderer::getDiscretizer() {
    return __discretizer;
}

void
GLRenderer::setSelectionMode(SelectionId m) {
    __selectMode = m;
}

const
GLRenderer::SelectionId GLRenderer::getSelectionMode() const {
    return __selectMode;
}

#ifndef PGL_WITHOUT_QT

bool GLRenderer::setGLFrameFromId(WId wid) {
  QWidget *widget = QWidget::find(WId(wid));
  if (!widget) return false;
#ifdef Q_CC_MSVC
  // By default qmake do not compile project with rtti information when
  // using msvc. So do a static cast
  QOpenGLBaseWidget * glwidget = static_cast<QOpenGLBaseWidget *>(widget);
#else
  QOpenGLBaseWidget * glwidget = dynamic_cast<QOpenGLBaseWidget *>(widget);
#endif
  if (!glwidget) return false;
  setGLFrame(glwidget);
  return true;
}

#endif

void GLRenderer::init() {
  if (__ogltoinit) {
    __ogl->initializeOpenGLFunctions();
    __ogltoinit = false;
  }
}

void GLRenderer::clear() {
#ifdef GEOM_DLDEBUG
  printf("clear cache\n");
#endif
  for (Cache<GLuint>::Iterator _it = __cache.begin();
       _it != __cache.end();
       _it++) {
    if (_it->second) __ogl->glDeleteLists(_it->second, 1);
  }
  __cache.clear();
  if (__scenecache != 0) {
    __ogl->glDeleteLists(__scenecache, 1);
    __scenecache = 0;
  }
  for (TextureCache::Iterator _it2 = __cachetexture.begin();
       _it2 != __cachetexture.end();
       _it2++) {
    if (_it2->second) {
      _it2->second->destroy();
      delete _it2->second;
    }
    // __ogl->glDeleteTextures(1, &(_it2->second));
  }
  __cachetexture.clear();
  __currentdisplaylist = false;
  if (__compil != -1)__compil = 0;
}


bool GLRenderer::check(size_t id, GLuint &displaylist) {
#ifdef GEOM_DLDEBUG
      printf("Check display list \n");
#endif
  if (__Mode != DynamicPrimitive) {
    Cache<GLuint>::Iterator _it = __cache.find((uint_t) id);
    if (_it != __cache.end()) {
      displaylist = _it->second;
      __ogl->glCallList(displaylist);
#ifdef GEOM_DLDEBUG
      printf("Call Display List %i\n",displaylist);
#endif
      GL_ERROR_CHECK;
      return true;
    }
    else {
      displaylist = 0;
      if (!__currentdisplaylist) {
        displaylist = glGenLists(1);
        if (displaylist != 0) {
          __ogl->glNewList(displaylist, __executionmode);
#ifdef GEOM_DLDEBUG
          printf("Create Display List %i for id=%zu\n",displaylist, id);
#endif
          __currentdisplaylist = true;
        }
        GL_ERROR_CHECK;
      }
    }
  }
  return false;
}

bool GLRenderer::call(size_t id) {
  if (__Mode != DynamicPrimitive) {
    Cache<GLuint>::Iterator _it = __cache.find((uint_t) id);
    if (_it != __cache.end()) {
     __ogl-> glCallList(_it->second);
#ifdef GEOM_DLDEBUG
      printf("Call Display List %i\n",_it->second);
#endif
      GL_ERROR_CHECK;
      return true;
    }
  }
  return false;
}

void GLRenderer::update(size_t id, GLuint displaylist) {
  if (__Mode != DynamicPrimitive && displaylist != 0 && __currentdisplaylist) {
#ifdef GEOM_DLDEBUG
    printf("End Display List %i for obj %zu\n",displaylist,id);
#endif
    __ogl->glEndList();
    __cache.insert((uint_t) id, displaylist);
    GL_ERROR_CHECK;
    __currentdisplaylist = false;
  }
}

void GLRenderer::registerTexture(ImageTexture *texture, QOpenGLTexture * gltexture, bool erasePreviousIfExists) {
  TextureCache::Iterator it = __cachetexture.find(texture->getObjectId());
  if (it != __cachetexture.end()) {
    QOpenGLTexture * oldtexture = it->second;
    if (erasePreviousIfExists){
      oldtexture->destroy();
      delete oldtexture;
   
    } 
    it->second = gltexture;
  }
  else {
    __cachetexture.insert(texture->getObjectId(), gltexture);
  }
}


QOpenGLTexture * GLRenderer::getTextureId(ImageTexture *texture) {
  TextureCache::Iterator it = __cachetexture.find(texture->getObjectId());
  if (it != __cachetexture.end()) return it->second;
  else return 0;
}

/* ----------------------------------------------------------------------- */
void
GLRenderer::setRenderingMode(RenderingMode mode) {
  if (__Mode != mode) {
    if (!(__Mode & DynamicPrimitive) && (mode & DynamicPrimitive)) {
      clear();
    }
    if (!(__Mode & Dynamic) && (mode & DynamicScene)) {
      clearSceneList();
    }
    if (mode & DynamicPrimitive)__compil = -1;
    else if (__compil == -1)__compil = 0;
    __Mode = mode;

#ifdef GEOM_DLDEBUG
    if (mode == DynamicPrimitive) printf("Mode: DynamicPrimitive\n");
    else if (mode == DynamicScene) printf("Mode: DynamicScene\n");
    else if (mode == Dynamic) printf("Mode: Dynamic\n");
    else if (mode == Normal) printf("Mode: Normal\n");
    else if (mode == Selection) printf("Mode: Selection\n");
#endif

  }
}

const GLRenderer::RenderingMode &GLRenderer::getRenderingMode() const {
  return __Mode;
}

bool
GLRenderer::beginSceneList() {
  if (__Mode == Normal && !__withvertexarray) {
    if (__compil < 2) {
#ifdef GEOM_DLDEBUG
      printf("No Scene DisplayList yet : %i\n", __scenecache);
#endif
      return true;
    }
    else {
      if (__scenecache == 0) {
         __scenecache = __ogl->glGenLists(1);
        if (__scenecache != 0) {
          __ogl->glNewList(__scenecache, GL_COMPILE_AND_EXECUTE);
#ifdef GEOM_DLDEBUG
          printf("Begin Scene DisplayList : %i\n", __scenecache);
#endif
          GL_ERROR_CHECK;
        }
#ifdef GEOM_DLDEBUG
        else printf("Failed to create global display list");
#endif
        return true;
      }
      else {
#ifdef GEOM_DLDEBUG
        printf("Call Scene DisplayList : %i \n", __scenecache);
#endif
        __ogl->glCallList(__scenecache);
        GL_ERROR_CHECK;
        return false;
      }
    }
  }
  else return true;
}

void
GLRenderer::endSceneList() {
  if (__compil >= 2 &&
      __scenecache != 0 &&
      __Mode == Normal) {
#ifdef GEOM_DLDEBUG
    printf("End Scene DisplayList\n");
#endif
    __ogl->glEndList();
    if(GL_ERROR_CHECK){
        clearSceneList();
        __compil = 2;
    }
  }
}

void
GLRenderer::clearSceneList() {
  if (__scenecache) {
    __ogl->glDeleteLists(__scenecache, 1);
    __scenecache = 0;
    __compil = 2;
  }
}

#ifndef PGL_MIN_MAX
#define PGL_MIN_MAX

template <typename T>
inline const T &pglMin(const T &a, const T &b) { return (a < b) ? a : b; }

template <typename T>
inline const T &pglMax(const T &a, const T &b) { return (a < b) ? b : a; }


#endif


bool GLRenderer::beginProcess() {
#ifdef GEOM_DLDEBUG
  printf("begin process\n");
#endif
  if (__ogltoinit) {
    __ogl->initializeOpenGLFunctions();
    __ogltoinit = false;
  }
  if (__Mode == Selection) {
   __ogl-> glInitNames();
  }
  else {

    GLint maxgllistnesting = 0;
    __ogl->glGetIntegerv(GL_MAX_LIST_NESTING, &maxgllistnesting);
    __maxprecompildepth = pglMin(maxgllistnesting, MAXPRECOMPILDEPTH);

#ifdef GEOM_DLDEBUG
    if (__compil == 0)printf("** Compile Geometry\n");
    else if (__compil == 1)printf("** Compile Shape\n");
    else if (__compil == 2)printf("** Compile Scene\n");
#endif

  }
  return true;
}


bool GLRenderer::endProcess() {
#ifdef GEOM_DLDEBUG
  printf("end process\n");
#endif

  if (__compil != -1 && __compil < 3) {
#ifdef GEOM_DLDEBUG
    if (__compil == 0)printf("** Compiled Geometry\n");
    else if (__compil == 1)printf("** Compiled Shape\n");
    else if (__compil == 2)printf("** Compiled Scene : %i\n",__scenecache);
#endif
    __compil += 2;
  }
  if(is_valid_ptr(__appearance) && __appearance->isTexture()) {
    __ogl->glDisable(GL_TEXTURE_2D);
    __ogl->glBindTexture(GL_TEXTURE_2D, 0);
  }
  __appearance = AppearancePtr();

  return true;
}

/* ----------------------------------------------------------------------- */
bool GLRenderer::process(Shape *geomshape) {
  GEOM_ASSERT_OBJ(geomshape);
  processAppereance(geomshape);
  return processGeometry(geomshape);
}

bool GLRenderer::processAppereance(Shape *geomshape) {
  GEOM_ASSERT(geomshape);
  if (geomshape->appearance) {
    return geomshape->appearance->apply(*this);
  }
  else return Material::DEFAULT_MATERIAL->apply(*this);
}

bool GLRenderer::processGeometry(Shape *geomshape) {
  GEOM_ASSERT(geomshape);
  if (__Mode & Dynamic) {
    if (__Mode == DynamicScene && __compil == 0) {
      GEOM_GLRENDERER_PRECOMPILE_INIT(geomshape);
      GEOM_GLRENDERER_PRECOMPILE_SUB(geomshape->geometry);
      GEOM_GLRENDERER_PRECOMPILE_INITEND;
    }
    return geomshape->geometry->apply(*this);
  }

  if (__Mode == Selection) {
    if ((__selectMode & ShapeId) == ShapeId && geomshape->getId() != Shape::NOID) {
      __ogl->glPushName(GLuint(geomshape->getId()));
    }
    else if ((__selectMode & SceneObjectId) == SceneObjectId) {
      __ogl->glPushName(GLuint(geomshape->getObjectId()));
    }
  }

  // if (__compil == 0) return Shape->geometry->apply(*this);

  GL_ERROR_CHECK;

/*  GLuint _displayList = 0;
  if (__compil == 1) {
    if (check(Shape->getObjectId(),_displayList)) {
      if (__Mode == Selection)glPopName();
      return true;
    }
    else {*/

  /*if (__compil == 0) {
    GEOM_GLRENDERER_PRECOMPILE_INIT(geomshape);
    GEOM_GLRENDERER_PRECOMPILE_SUB(geomshape->geometry);
    GEOM_GLRENDERER_PRECOMPILE_INITEND;
  }*/

  bool res = geomshape->geometry->apply(*this);

  //update(Shape->getObjectId(),_displayList);
  if (__Mode == Selection && (__selectMode != PrimitiveId)) __ogl->glPopName();
  return res;
/*  }
  }
  else {
    if (!call(Shape->getObjectId()))Shape->geometry->apply(*this);
    if (__Mode == Selection)glPopName();
    return true;
  }*/
}

/* ----------------------------------------------------------------------- */

bool GLRenderer::process(Inline *geomInline) {
  GEOM_ASSERT_OBJ(geomInline);
  if (geomInline->getScene()) {
    if (__Mode == Selection) {
      __ogl->glPushName(GLuint(geomInline->getObjectId()));
    }
    if (!geomInline->isTranslationToDefault() || !geomInline->isScaleToDefault()) {
      __ogl->glPushMatrix();
      const Vector3 _trans = geomInline->getTranslation();
      __ogl->glGeomTranslate(_trans);
      const Vector3 _scale = geomInline->getScale();
      __ogl->glGeomScale(_scale);
    }
    bool _result = true;
    for (Scene::iterator _i = geomInline->getScene()->begin();
         _i != geomInline->getScene()->end();
         _i++) {
      if (!(*_i)->applyAppearanceFirst(*this)) _result = false;
    };
    if (!geomInline->isTranslationToDefault() || !geomInline->isScaleToDefault()) {
      __ogl->glPopMatrix();
    }
    if (__Mode == Selection)__ogl->glPopName();

    GL_ERROR_CHECK;
    return _result;
  }
  else return false;
}

/* ----------------------------------------------------------------------- */

bool GLRenderer::process(AmapSymbol *amapSymbol) {
  GEOM_ASSERT_OBJ(amapSymbol);
  GEOM_GLRENDERER_PRECOMPILE(amapSymbol);
  GEOM_GLRENDERER_CHECK_CACHE(amapSymbol);

  const IndexArrayPtr &_indexList = amapSymbol->getIndexList();

  __ogl->glFrontFace(amapSymbol->getCCW() ? GL_CCW : GL_CW);
  bool tex = __appearance && __appearance->isTexture() && amapSymbol->hasTexCoordList();
  bool color = amapSymbol->hasColorList();
  bool colorV = amapSymbol->getColorPerVertex();

  if (__appearance &&
      __appearance->isTexture() &&
      !amapSymbol->hasTexCoordList()) {
    __ogl->glEnable(GL_TEXTURE_GEN_S);
    __ogl->glEnable(GL_TEXTURE_GEN_T);
  }
  amapSymbol->checkNormalList();

  if(!__withvertexarray){
    uint_t _sizei = _indexList->size();
    GLfloat _rgba[4];
    for (uint_t _i = 0; _i < _sizei; _i++) {
      __ogl->glBegin(GL_POLYGON);
      if (color && !colorV) {
        const Color4 &_ambient = amapSymbol->getColorAt(_i);
        _rgba[0] = (GLfloat) _ambient.getRedClamped();
        _rgba[1] = (GLfloat) _ambient.getGreenClamped();
        _rgba[2] = (GLfloat) _ambient.getBlueClamped();
        _rgba[3] = 1.0f - (GLfloat) _ambient.getAlphaClamped();
        __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _rgba);
        __ogl->glColor4fv(_rgba);
      }

      const Index &_index = _indexList->getAt(_i);
      uint_t _sizej = _index.size();
      for (uint_t _j = 0; _j < _sizej; _j++) {
        __ogl->glGeomNormal(amapSymbol->getFaceNormalAt(_i, _j));
        if (tex) {
          Vector3 &tex = amapSymbol->getFaceTexCoord3At(_i, _j);
          __ogl->glTexCoord2d(tex.y(), tex.z());
        } else if (color && colorV) {
          const Color4 &_ambient = amapSymbol->getFaceColorAt(_i, _j);
          _rgba[0] = (GLfloat) _ambient.getRedClamped();
          _rgba[1] = (GLfloat) _ambient.getGreenClamped();
          _rgba[2] = (GLfloat) _ambient.getBlueClamped();
          _rgba[3] = 1.0f - (GLfloat) _ambient.getAlphaClamped();
          __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _rgba);
          __ogl->glColor4fv(_rgba);
        }
        __ogl->glGeomVertex(amapSymbol->getFacePointAt(_i, _j));
      }
      __ogl->glEnd();
    };
  }
  else {
    __ogl->glEnableClientState(GL_VERTEX_ARRAY);
    real_t * vertices = amapSymbol->getPointList()->data();
    __ogl->glVertexPointer( 3, GL_GEOM_REAL,0,vertices);

    real_t * normals = amapSymbol->getNormalList()->data();
    __ogl->glEnableClientState(GL_NORMAL_ARRAY);
    __ogl->glNormalPointer( GL_GEOM_REAL,0,normals);

    real_t * texCoord = NULL;
    if (tex) {
      texCoord = amapSymbol->getTexCoordList()->data();
      __ogl->glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      __ogl->glTexCoordPointer( 2, GL_GEOM_REAL, sizeof(real_t) ,&texCoord[1]);
    }

    for(IndexArray::const_iterator it = amapSymbol->getIndexList()->begin();
        it != amapSymbol->getIndexList()->end(); it++)
      {
        __ogl->glDrawElements( GL_POLYGON, it->size() , GL_UNSIGNED_INT, ( const GLvoid* )( &*( it->begin() ) ));
      }

    __ogl->glDisableClientState(GL_VERTEX_ARRAY);
    __ogl->glDisableClientState(GL_NORMAL_ARRAY);
    __ogl->glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    delete [] vertices;
    delete [] normals;
    if (texCoord) delete [] texCoord;

  }
  if (__appearance &&
      __appearance->isTexture() &&
      !amapSymbol->hasTexCoordList()) {
    __ogl->glDisable(GL_TEXTURE_GEN_S);
    __ogl->glDisable(GL_TEXTURE_GEN_T);
  }

  GEOM_GLRENDERER_UPDATE_CACHE(amapSymbol);
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(AsymmetricHull * asymmetricHull) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(asymmetricHull);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(AxisRotated *axisRotated) {
  GEOM_ASSERT_OBJ(axisRotated);

  GEOM_GLRENDERER_PRECOMPILE_BEG(axisRotated);
    GEOM_GLRENDERER_PRECOMPILE_SUB(axisRotated->getGeometry());
  GEOM_GLRENDERER_PRECOMPILE_END(axisRotated);

  GEOM_GLRENDERER_CHECK_CACHE(axisRotated);

  const Vector3 &_axis = axisRotated->getAxis();
  const real_t &_angle = axisRotated->getAngle();

  GL_PUSH_MATRIX(axisRotated->getGeometry());
  __ogl->glGeomRadRotate(_axis, _angle);
  axisRotated->getGeometry()->apply(*this);
  GL_POP_MATRIX(axisRotated->getGeometry());

  GL_ERROR_CHECK;
  GEOM_GLRENDERER_UPDATE_CACHE(axisRotated);
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(BezierCurve * bezierCurve) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(bezierCurve);
}

/* ----------------------------------------------------------------------- */


bool GLRenderer::process(BezierPatch * bezierPatch) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(bezierPatch);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(Box * box) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(box);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(Cone * cone) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(cone);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(Cylinder * cylinder) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(cylinder);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(ElevationGrid * elevationGrid) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(elevationGrid);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(EulerRotated * eulerRotated) {
  GEOM_ASSERT_OBJ(eulerRotated);
  GEOM_GLRENDERER_PRECOMPILE_BEG(eulerRotated);
    GEOM_GLRENDERER_PRECOMPILE_SUB(eulerRotated->getGeometry());
  GEOM_GLRENDERER_PRECOMPILE_END(eulerRotated);
  GEOM_GLRENDERER_CHECK_CACHE(eulerRotated);

  GL_PUSH_MATRIX(eulerRotated->getGeometry());
  __ogl->glGeomRadEulerRotateZYX(eulerRotated->getAzimuth(),
                          eulerRotated->getElevation(),
                          eulerRotated->getRoll());
  eulerRotated->getGeometry()->apply(*this);
  GL_POP_MATRIX(eulerRotated->getGeometry());

  GEOM_GLRENDERER_UPDATE_CACHE(eulerRotated);
  GL_ERROR_CHECK;
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(ExtrudedHull * extrudedHull) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(extrudedHull);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(FaceSet * faceSet) {
  GEOM_ASSERT_OBJ(faceSet);
  GEOM_GLRENDERER_PRECOMPILE(faceSet);
  GEOM_GLRENDERER_CHECK_CACHE(faceSet);

  __ogl->glFrontFace(faceSet->getCCW() ? GL_CCW : GL_CW);
  const IndexArrayPtr &_indexList = faceSet->getIndexList();
  bool normalV = faceSet->getNormalPerVertex();
  bool tex = __appearance && __appearance->isTexture() && faceSet->hasTexCoordList();
  bool color = faceSet->hasColorList();
  bool colorV = faceSet->getColorPerVertex();

  if (__appearance &&
      __appearance->isTexture() &&
      !faceSet->hasTexCoordList()) {
    __ogl->glEnable(GL_TEXTURE_GEN_S);
    __ogl->glEnable(GL_TEXTURE_GEN_T);
  }
  faceSet->checkNormalList();

  if(!__withvertexarray){
    uint_t _sizei = _indexList->size();
    GLfloat _rgba[4];
    for (uint_t _i = 0; _i < _sizei; _i++) {
      __ogl->glBegin(GL_POLYGON);
      if (!normalV)__ogl->glGeomNormal(faceSet->getNormalAt(_i));
      if (color && !colorV) {
        const Color4 &_ambient = faceSet->getColorAt(_i);
        _rgba[0] = (GLfloat) _ambient.getRedClamped();
        _rgba[1] = (GLfloat) _ambient.getGreenClamped();
        _rgba[2] = (GLfloat) _ambient.getBlueClamped();
        _rgba[3] = 1.0f - (GLfloat) _ambient.getAlphaClamped();
        __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _rgba);
        __ogl->glColor4fv(_rgba);
      }

      const Index &_index = _indexList->getAt(_i);
      uint_t _sizej = _index.size();
      for (uint_t _j = 0; _j < _sizej; _j++) {
        if (normalV)__ogl->glGeomNormal(faceSet->getFaceNormalAt(_i, _j));
        if (tex)__ogl->glGeomTexCoord(faceSet->getFaceTexCoordAt(_i, _j));
        else if (color && colorV) {
          const Color4 &_ambient = faceSet->getFaceColorAt(_i, _j);
          _rgba[0] = (GLfloat) _ambient.getRedClamped();
          _rgba[1] = (GLfloat) _ambient.getGreenClamped();
          _rgba[2] = (GLfloat) _ambient.getBlueClamped();
          _rgba[3] = 1.0f - (GLfloat) _ambient.getAlphaClamped();
          __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _rgba);
          __ogl->glColor4fv(_rgba);
        }
        __ogl->glGeomVertex(faceSet->getFacePointAt(_i, _j));
      }
      __ogl->glEnd();
    };
  }
  else {

    __ogl->glEnableClientState(GL_VERTEX_ARRAY);
    real_t * vertices = faceSet->getPointList()->data();
    __ogl->glVertexPointer( 3, GL_GEOM_REAL,0,vertices);

    faceSet->checkNormalList();
    real_t * normals = NULL;
    if (normalV) {
      normals = faceSet->getNormalList()->data();
      __ogl->glEnableClientState(GL_NORMAL_ARRAY);
      __ogl->glNormalPointer( GL_GEOM_REAL,0,normals);
    }

    real_t * texCoord = NULL;
    if (tex) {
      texCoord = faceSet->getTexCoordList()->data();
      __ogl->glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      __ogl->glTexCoordPointer( 2, GL_GEOM_REAL,0,texCoord);
    }

    size_t _i = 0;
    for(IndexArray::const_iterator it = faceSet->getIndexList()->begin();
        it != faceSet->getIndexList()->end(); it++)
      {
        if (!normalV)
      __ogl->glGeomNormal(faceSet->getNormalAt(_i));
        _i++;
        __ogl->glDrawElements( GL_POLYGON, it->size() , GL_UNSIGNED_INT, ( const GLvoid* )( &*( it->begin() )) );
      }

    __ogl->glDisableClientState(GL_VERTEX_ARRAY);

    if (normalV)__ogl->glDisableClientState(GL_NORMAL_ARRAY);
    if (tex)__ogl->glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    delete [] vertices;
    if (normals)delete [] normals;
    if (texCoord)delete [] texCoord;

  }
  if (__appearance &&
      __appearance->isTexture() &&
      !faceSet->hasTexCoordList()) {
    __ogl->glDisable(GL_TEXTURE_GEN_S);
    __ogl->glDisable(GL_TEXTURE_GEN_T);
  }

  GEOM_GLRENDERER_UPDATE_CACHE(faceSet);
  GL_ERROR_CHECK;
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(Frustum * frustum) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(frustum);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(Extrusion * extrusion) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(extrusion);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(Group * group) {
  GEOM_ASSERT_OBJ(group);
  GEOM_GLRENDERER_PRECOMPILE_BEG(group);
  for (GeometryArray::const_iterator it = group->getGeometryList()->begin();
      it != group->getGeometryList()->end(); ++it) {
#ifdef GEOM_TREECALLDEBUG
      printf("Look at child of group %zu in mode %i\n", group->getObjectId(),  __compil);
#endif
      GEOM_GLRENDERER_PRECOMPILE_SUB((*it));
  }
  GEOM_GLRENDERER_PRECOMPILE_END(group);
  GEOM_GLRENDERER_CHECK_CACHE(group);

  group->getGeometryList()->apply(*this);

  GEOM_GLRENDERER_UPDATE_CACHE(group);

  GL_ERROR_CHECK;
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(IFS * ifs) {
  GEOM_ASSERT_OBJ(ifs);
  GEOM_GLRENDERER_PRECOMPILE_BEG(ifs);
    GEOM_GLRENDERER_PRECOMPILE_SUB(ifs->getGeometry());
  GEOM_GLRENDERER_PRECOMPILE_END(ifs);
  GEOM_GLRENDERER_CHECK_CACHE(ifs);

  ITPtr transfos;
  transfos = dynamic_pointer_cast<IT>(ifs->getTransformation());
  GEOM_ASSERT(transfos);
  const Matrix4ArrayPtr& matrixList = transfos->getAllTransfo();
  GEOM_ASSERT(matrixList);

  __dopushpop = true;

  Matrix4Array::const_iterator matrix = matrixList->begin();
  while (matrix != matrixList->end())
  {
    GL_PUSH_MATRIX(ifs->getGeometry());
    __ogl->glGeomMultMatrix(*matrix);
    ifs->getGeometry()->apply(*this);
    GL_POP_MATRIX(ifs->getGeometry());
    matrix++;
  }

  GEOM_GLRENDERER_UPDATE_CACHE(ifs);

  GL_ERROR_CHECK;
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(Material * material) {
  GEOM_ASSERT_OBJ(material);
  GEOM_GLRENDERER_CHECK_APPEARANCE(material);

  __ogl->glDisable(GL_TEXTURE_2D);
  __ogl->glBindTexture(GL_TEXTURE_2D, 0);

  GLfloat _rgba[4];
  const Color3& _ambient = material->getAmbient();
  _rgba[0] = (GLfloat) _ambient.getRedClamped();
  _rgba[1] = (GLfloat) _ambient.getGreenClamped();
  _rgba[2] = (GLfloat) _ambient.getBlueClamped();
  _rgba[3] = 1.0f - material->getTransparency();
  __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _rgba);

  const real_t& _diffuse = material->getDiffuse();
  _rgba[0] *= _diffuse;
  _rgba[1] *= _diffuse;
  _rgba[2] *= _diffuse;
  __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _rgba);

  /// We set the current color in the case of disabling the lighting
  __ogl->glColor4fv(_rgba);

  const Color3& _specular = material->getSpecular();
  _rgba[0] = (GLfloat) _specular.getRedClamped();
  _rgba[1] = (GLfloat) _specular.getGreenClamped();
  _rgba[2] = (GLfloat) _specular.getBlueClamped();
  __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, _rgba);

  const Color3& _emission = material->getEmission();
  _rgba[0] = (GLfloat) _emission.getRedClamped();
  _rgba[1] = (GLfloat) _emission.getGreenClamped();
  _rgba[2] = (GLfloat) _emission.getBlueClamped();
  __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, _rgba);

  __ogl->glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->getShininess());

  GEOM_GLRENDERER_UPDATE_APPEARANCE(material);

  GL_ERROR_CHECK;
  return true;
}


/* ----------------------------------------------------------------------- */
bool GLRenderer::process(ImageTexture * texture) {
  GEOM_ASSERT_OBJ(texture);


  TextureCache::Iterator it = __cachetexture.find(texture->getObjectId());
  if (it != __cachetexture.end()) {
    __ogl->glEnable(GL_TEXTURE_2D);
    it->second->bind();
  } 
  else {
#ifndef PGL_CORE_WITHOUT_QT
    QImage img;
    if (img.load(texture->getFilename().c_str())) {
      bool notUsingMipmap = (!texture->getMipmaping()) && isPowerOfTwo(img.width()) && isPowerOfTwo(img.height());
      __ogl->glEnable(GL_TEXTURE_2D);

      QOpenGLTexture * qgl_texture = new QOpenGLTexture(img);
      qgl_texture->setWrapMode(QOpenGLTexture::DirectionS, texture->getRepeatS() ? QOpenGLTexture::Repeat : QOpenGLTexture::ClampToEdge);
      qgl_texture->setWrapMode(QOpenGLTexture::DirectionT, texture->getRepeatT() ? QOpenGLTexture::Repeat : QOpenGLTexture::ClampToEdge);
      qgl_texture->setMinMagFilters(QOpenGLTexture::LinearMipMapNearest, QOpenGLTexture::LinearMipMapNearest);
      qgl_texture->generateMipMaps();
      qgl_texture->create();
      qgl_texture->bind();
      __cachetexture.insert(texture->getObjectId(), qgl_texture);
    }
#endif
  }

  GL_ERROR_CHECK;
  return true;
}


/* ----------------------------------------------------------------------- */

bool GLRenderer::process(Texture2D * texture) {
  GEOM_ASSERT_OBJ(texture);
  GEOM_GLRENDERER_CHECK_APPEARANCE(texture);

  GLfloat _rgba[4];

  const Color4 &_color = texture->getBaseColor();
  _rgba[0] = (GLfloat) _color.getRedClamped();
  _rgba[1] = (GLfloat) _color.getGreenClamped();
  _rgba[2] = (GLfloat) _color.getBlueClamped();
  _rgba[3] = 1.0f - _color.getAlphaClamped();

  /// We set the current color in the case of disabling the lighting
  __ogl->glColor4fv(_rgba);
  __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _rgba);
  __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _rgba);

  if (texture->getImage()) {
    // load image
    texture->getImage()->apply(*this);

    // apply texture transformation
    if (texture->getTransformation())
      texture->getTransformation()->apply(*this);
    else {
      // Set Texture transformation to Id if no transformation is specified
      __ogl->glMatrixMode(GL_TEXTURE);
      __ogl->glLoadIdentity();
      __ogl->glMatrixMode(GL_MODELVIEW);
    }
  }
  GEOM_GLRENDERER_UPDATE_APPEARANCE(texture);

  GL_ERROR_CHECK;
  return true;
}

/* ----------------------------------------------------------------------- */

bool GLRenderer::process(Texture2DTransformation * texturetransfo) {
  GEOM_ASSERT_OBJ(texturetransfo);

  __ogl->glMatrixMode(GL_TEXTURE);
  __ogl->glLoadIdentity();

  if (!texturetransfo->isTranslationToDefault()) {
    Vector2 &value = texturetransfo->getTranslation();
    __ogl->glGeomTranslate(value.x(), value.y(), 0.0);
  }

  if (!texturetransfo->isRotationAngleToDefault()) {
    Vector2 &rotationCenter = texturetransfo->getRotationCenter();
    __ogl->glGeomTranslate(rotationCenter.x(), rotationCenter.y(), 0.0);

    real_t &value = texturetransfo->getRotationAngle();
    __ogl->glGeomRadRotate(0, 0, 1, value);

    __ogl->glGeomTranslate(-rotationCenter.x(), -rotationCenter.y(), 0.0);
  }

  if (!texturetransfo->isScaleToDefault()) {
    Vector2 &value = texturetransfo->getScale();
    __ogl->glGeomScale(value.x(), value.y(), 1.0);
  }

  __ogl->glMatrixMode(GL_MODELVIEW);
  return true;
}

/* ----------------------------------------------------------------------- */


bool GLRenderer::process(MonoSpectral * monoSpectral) {
  GEOM_ASSERT_OBJ(monoSpectral);

  GEOM_GLRENDERER_CHECK_APPEARANCE(monoSpectral);

  __ogl->glDisable(GL_TEXTURE_2D);
  __ogl->glBindTexture(GL_TEXTURE_2D, 0);

  GLfloat _rgba[4];
  _rgba[0] = (GLfloat) monoSpectral->getReflectance();
  _rgba[1] = (GLfloat) monoSpectral->getReflectance();
  _rgba[2] = (GLfloat) monoSpectral->getReflectance();
  _rgba[3] = (GLfloat) monoSpectral->getTransmittance();
  __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _rgba);

  /// We set the current color in the case of disabling the lighting
  __ogl->glColor4fv(_rgba);

  GEOM_GLRENDERER_UPDATE_APPEARANCE(monoSpectral);

  GL_ERROR_CHECK;
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(MultiSpectral * multiSpectral) {
  GEOM_ASSERT_OBJ(multiSpectral);

  GEOM_GLRENDERER_CHECK_APPEARANCE(multiSpectral);

  __ogl->glDisable(GL_TEXTURE_2D);
  __ogl->glBindTexture(GL_TEXTURE_2D, 0);

  GLfloat _rgba[4];
  const Index3 &_filter = multiSpectral->getFilter();
  _rgba[0] = (GLfloat) multiSpectral->getReflectanceAt(_filter.getAt(0));
  _rgba[1] = (GLfloat) multiSpectral->getReflectanceAt(_filter.getAt(1));
  _rgba[2] = (GLfloat) multiSpectral->getReflectanceAt(_filter.getAt(2));
  _rgba[3] = ((GLfloat) multiSpectral->getTransmittanceAt(_filter.getAt(0)) +
              (GLfloat) multiSpectral->getTransmittanceAt(_filter.getAt(1)) +
              (GLfloat) multiSpectral->getTransmittanceAt(_filter.getAt(2))) / 3;
  __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, _rgba);

  /// We set the current color in the case of disabling the lighting
  __ogl->glColor4fv(_rgba);

  GEOM_GLRENDERER_UPDATE_APPEARANCE(multiSpectral);

  GL_ERROR_CHECK;
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(NurbsCurve * nurbsCurve) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(nurbsCurve);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(NurbsPatch * nurbsPatch) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(nurbsPatch);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(Oriented * oriented) {
  GEOM_ASSERT_OBJ(oriented);

  GEOM_GLRENDERER_PRECOMPILE_BEG(oriented);
    GEOM_GLRENDERER_PRECOMPILE_SUB(oriented->getGeometry());
  GEOM_GLRENDERER_PRECOMPILE_END(oriented);

  GEOM_GLRENDERER_CHECK_CACHE(oriented);

  GL_PUSH_MATRIX(oriented->getGeometry());

  Matrix4TransformationPtr _basis;
  _basis = dynamic_pointer_cast<Matrix4Transformation>(oriented->getTransformation());
  GEOM_ASSERT(_basis);
  __ogl->glGeomMultMatrix(_basis->getMatrix());

  oriented->getGeometry()->apply(*this);
  GL_POP_MATRIX(oriented->getGeometry());

  GEOM_GLRENDERER_UPDATE_CACHE(oriented);
  GL_ERROR_CHECK;
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(Paraboloid * paraboloid) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(paraboloid);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(PointSet * pointSet) {
  GEOM_ASSERT_OBJ(pointSet);
  GEOM_GLRENDERER_PRECOMPILE(pointSet);
  GEOM_GLRENDERER_CHECK_CACHE(pointSet);

  __ogl->glPushAttrib(GL_LIGHTING_BIT);
  __ogl->glDisable(GL_LIGHTING);

  if (!pointSet->isWidthToDefault()) {
    __ogl->glPushAttrib(GL_POINT_BIT);
    int globalwidth = 1;
    __ogl->glGetIntegerv(GL_POINT_SIZE, &globalwidth);
    __ogl->glPointSize(float(pointSet->getWidth() + globalwidth - 1));
  }

  const Point3ArrayPtr &points = pointSet->getPointList();
  if (!points) return false;
  bool color = pointSet->hasColorList() &&
               (pointSet->getColorList()->size() == points->size());

  if(!__withvertexarray){
    Color4Array::const_iterator _itCol;
    if (color) _itCol = pointSet->getColorList()->begin();
    GLfloat _rgba[4];
    GLuint index = 0;

    bool primitiveselection = (__Mode == Selection) && ((__selectMode & PrimitiveId) == PrimitiveId);
    if (primitiveselection) __ogl->glPushName(0);
    else __ogl->glBegin(GL_POINTS);

    for (Point3Array::const_iterator _i = points->begin(); _i != points->end(); _i++, index++) {
      if (primitiveselection) {
        __ogl->glLoadName(index);
        __ogl->glBegin(GL_POINTS);
      }
      if (color) {
        const Color4 &_ambient = *_itCol;
        _rgba[0] = (GLfloat) _ambient.getRedClamped();
        _rgba[1] = (GLfloat) _ambient.getGreenClamped();
        _rgba[2] = (GLfloat) _ambient.getBlueClamped();
        _rgba[3] = 1.0f - (GLfloat) _ambient.getAlphaClamped();
        __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _rgba);
        __ogl->glColor4fv(_rgba);
        ++_itCol;
      }
      __ogl->glGeomVertex(*_i);
      if (primitiveselection)
       __ogl->glEnd();
    }
    if (primitiveselection) __ogl->glPopName();
    else __ogl->glEnd();
  }
  else {

    __ogl->glEnableClientState(GL_VERTEX_ARRAY);
    real_t * vertices = points->data();
    __ogl->glVertexPointer( 3, GL_GEOM_REAL, 0 ,vertices);
    uchar_t * colors = NULL;
    if ( color ) {
      colors = pointSet->getColorList()->toUcharArray();
      __ogl->glEnableClientState( GL_COLOR_ARRAY );
      __ogl->glColorPointer( 4, GL_UNSIGNED_BYTE, 4*sizeof( uchar_t ), colors );
    }
    __ogl->glDrawArrays(GL_POINTS, 0 , points->size() );

    if ( color ) __ogl->glDisableClientState( GL_COLOR_ARRAY );
    __ogl->glDisableClientState(GL_VERTEX_ARRAY);
    delete [] vertices;
    if (colors)delete [] colors;
  }

  if (!pointSet->isWidthToDefault()) { __ogl->glPopAttrib(); }
  __ogl->glPopAttrib();

  GEOM_GLRENDERER_UPDATE_CACHE(pointSet);
  GL_ERROR_CHECK;
  return true;
}


/* ----------------------------------------------------------------------- */

bool GLRenderer::process(PGL(Polyline) * polyline) {
  GEOM_ASSERT_OBJ(polyline);
  GEOM_GLRENDERER_PRECOMPILE(polyline);
  GEOM_GLRENDERER_CHECK_CACHE(polyline);

  __ogl->glPushAttrib(GL_LIGHTING_BIT);
  __ogl->glDisable(GL_LIGHTING);

  BEGIN_LINE_WIDTH(polyline)

  const Point3ArrayPtr &points = polyline->getPointList();
  bool color = polyline->hasColorList() &&
               (polyline->getColorList()->size() == points->size());

  if(!__withvertexarray){
    Color4Array::const_iterator _itCol;
    if (color) _itCol = polyline->getColorList()->begin();
    GLfloat _rgba[4];
    __ogl->glBegin(GL_LINE_STRIP);
    for (Point3Array::const_iterator _i = points->begin();
        _i != points->end();
        _i++) {
      if (color) {
        const Color4 &_ambient = *_itCol;
        _rgba[0] = (GLfloat) _ambient.getRedClamped();
        _rgba[1] = (GLfloat) _ambient.getGreenClamped();
        _rgba[2] = (GLfloat) _ambient.getBlueClamped();
        _rgba[3] = 1.0f - (GLfloat) _ambient.getAlphaClamped();
        __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _rgba);
        __ogl->glColor4fv(_rgba);
        ++_itCol;
      }
      __ogl->glGeomVertex((*_i));
    }
    __ogl->glEnd();
  }
  else {

    __ogl->glEnableClientState(GL_VERTEX_ARRAY);
    real_t * vertices = points->data();
    __ogl->glVertexPointer( 3, GL_GEOM_REAL,0,vertices);

    uchar_t * colors = NULL;
    if ( color ) {
      colors = polyline->getColorList()->toUcharArray();
      __ogl->glEnableClientState( GL_COLOR_ARRAY );
      __ogl->glColorPointer( 4, GL_UNSIGNED_BYTE, 4*sizeof( uchar_t ), colors );
    }
    __ogl->glDrawArrays(GL_LINE_STRIP, 0 , points->size() );

    if ( color ) __ogl->glDisableClientState( GL_COLOR_ARRAY );
    __ogl->glDisableClientState(GL_VERTEX_ARRAY);
    delete [] vertices;
    if (colors)delete [] colors;
  }

  END_LINE_WIDTH(polyline)
  __ogl->glPopAttrib();

  GEOM_GLRENDERER_UPDATE_CACHE(polyline);
  GL_ERROR_CHECK;
  return true;
}


/* ----------------------------------------------------------------------- */




bool GLRenderer::process(QuadSet * quadSet) {
  GEOM_ASSERT_OBJ(quadSet);
  GEOM_GLRENDERER_PRECOMPILE(quadSet);

  GEOM_GLRENDERER_CHECK_CACHE(quadSet);

  __ogl->glFrontFace(quadSet->getCCW() ? GL_CCW : GL_CW);
  bool normalV = quadSet->getNormalPerVertex();
  bool tex = __appearance && __appearance->isTexture() && quadSet->hasTexCoordList();
  bool color = quadSet->hasColorList();
  bool colorV = quadSet->getColorPerVertex();

  if (__appearance &&
      __appearance->isTexture() &&
      !quadSet->hasTexCoordList()) {
    __ogl->glEnable(GL_TEXTURE_GEN_S);
    __ogl->glEnable(GL_TEXTURE_GEN_T);
  }

  quadSet->checkNormalList();
  if(!__withvertexarray){
    __ogl->glBegin(GL_QUADS);
    uint_t _size = quadSet->getIndexListSize();
    GLfloat _rgba[4];
    for (uint_t _i = 0; _i < _size; _i++) {
      if (!normalV)__ogl->glGeomNormal(quadSet->getNormalAt(_i));
      if (color && !colorV) {
        const Color4 &_ambient = quadSet->getColorAt(_i);
        _rgba[0] = (GLfloat) _ambient.getRedClamped();
        _rgba[1] = (GLfloat) _ambient.getGreenClamped();
        _rgba[2] = (GLfloat) _ambient.getBlueClamped();
        _rgba[3] = 1.0f - (GLfloat) _ambient.getAlphaClamped();
        __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _rgba);
        __ogl->glColor4fv(_rgba);
      }
      for (uint_t _j = 0; _j < 4; _j++) {
        if (normalV) __ogl->glGeomNormal(quadSet->getFaceNormalAt(_i, _j));
        if (tex)__ogl->glGeomTexCoord(quadSet->getFaceTexCoordAt(_i, _j));
        else if (color && colorV) {
          const Color4 &_ambient = quadSet->getFaceColorAt(_i, _j);
          _rgba[0] = (GLfloat) _ambient.getRedClamped();
          _rgba[1] = (GLfloat) _ambient.getGreenClamped();
          _rgba[2] = (GLfloat) _ambient.getBlueClamped();
          _rgba[3] = 1.0f - (GLfloat) _ambient.getAlphaClamped();
          __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _rgba);
          __ogl->glColor4fv(_rgba);
        }
        __ogl->glGeomVertex(quadSet->getFacePointAt(_i, _j));
      }
    };
    __ogl->glEnd();
  }
  else {

    __ogl->glEnableClientState(GL_VERTEX_ARRAY);
    real_t * vertices = quadSet->getPointList()->data();
    __ogl->glVertexPointer( 3, GL_GEOM_REAL,0,vertices);

    real_t * normals = NULL;
    if (normalV) {
      normals = quadSet->getNormalList()->data();
      __ogl->glEnableClientState(GL_NORMAL_ARRAY);
      __ogl->glNormalPointer( GL_GEOM_REAL,0,normals);
    }

    real_t * texCoord = NULL;
    uchar_t * colors = NULL;
    if (tex) {
      texCoord = quadSet->getTexCoordList()->data();
      __ogl->glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      __ogl->glTexCoordPointer( 2, GL_GEOM_REAL,0,texCoord);
    }
    else if ( color ) {
      colors = quadSet->getColorList()->toUcharArray();
      __ogl->glEnableClientState( GL_COLOR_ARRAY );
      __ogl->glColorPointer( 4, GL_UNSIGNED_BYTE, 4*sizeof( uchar_t ),  colors );
    }

    if (normalV) {
      size_t si = quadSet->getIndexList()->size();
      uint_t * indices = quadSet->getIndexList()->data();
      __ogl->glDrawElements( GL_QUADS, 4*si , GL_UNSIGNED_INT, indices);
      delete [] indices;
    }
    else {
      size_t _i = 0;
      for(Index4Array::const_iterator it = quadSet->getIndexList()->begin();
      it != quadSet->getIndexList()->end(); it++) {
            __ogl->glGeomNormal(quadSet->getNormalAt(_i));_i++;
            __ogl->glDrawElements(   GL_QUADS, 4 , GL_UNSIGNED_INT, it->begin());
      }

    }

    __ogl->glDisableClientState(GL_VERTEX_ARRAY);
    if (normalV)__ogl->glDisableClientState(GL_NORMAL_ARRAY);
    if (tex)__ogl->glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if ( color )__ogl->glDisableClientState(GL_COLOR_ARRAY);

    delete [] vertices;
    if (normals)  delete [] normals;
    if (texCoord) delete [] texCoord;
    if (colors)   delete [] colors;

  }
  if (__appearance &&
      __appearance->isTexture() &&
      !quadSet->hasTexCoordList()) {
    __ogl->glDisable(GL_TEXTURE_GEN_S);
    __ogl->glDisable(GL_TEXTURE_GEN_T);
  }

  GEOM_GLRENDERER_UPDATE_CACHE(quadSet);
  GL_ERROR_CHECK;
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(Revolution * revolution) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(revolution);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(Swung * swung) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(swung);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(Scaled * scaled) {
  GEOM_ASSERT_OBJ(scaled);
  GEOM_GLRENDERER_PRECOMPILE_BEG(scaled);
    GEOM_GLRENDERER_PRECOMPILE_SUB(scaled->getGeometry());
  GEOM_GLRENDERER_PRECOMPILE_END(scaled);
  GEOM_GLRENDERER_CHECK_CACHE(scaled);

  GL_PUSH_MATRIX(scaled->getGeometry());

  __ogl->glGeomScale(scaled->getScale());

  scaled->getGeometry()->apply(*this);
  GL_POP_MATRIX(scaled->getGeometry());

  GEOM_GLRENDERER_UPDATE_CACHE(scaled);
  GL_ERROR_CHECK;
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(ScreenProjected * scp) {
  GEOM_ASSERT_OBJ(scp);
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

  __ogl->glMatrixMode(GL_PROJECTION);
  __ogl->glPushMatrix();
  __ogl->glLoadIdentity();
  __ogl->glOrtho(-1, 1, -heigthscale, heigthscale, 1, -1);
  __ogl->glMatrixMode(GL_MODELVIEW);
  __ogl->glPushMatrix();
  __ogl->glLoadIdentity();

  bool lighting = __ogl->glIsEnabled(GL_LIGHTING);
  if (lighting) {
    __ogl->glPushAttrib(GL_LIGHTING_BIT);
    __ogl->glGeomLightPosition(GL_LIGHT0, Vector3(0, 0, -1));
    __ogl->glGeomLightDirection(GL_LIGHT0, Vector3(0, 0, 1));
  }

  scp->getGeometry()->apply(*this);

  if (lighting) __ogl->glPopAttrib();
  __ogl->glMatrixMode(GL_PROJECTION);
  __ogl->glPopMatrix();
  __ogl->glMatrixMode(GL_MODELVIEW);
  __ogl->glPopMatrix();


  GEOM_GLRENDERER_UPDATE_CACHE(scp);
  GL_ERROR_CHECK;
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(Sphere * sphere) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(sphere);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(Tapered * tapered) {
  GEOM_ASSERT_OBJ(tapered);
  GEOM_GLRENDERER_PRECOMPILE_BEG(tapered);
    GEOM_GLRENDERER_PRECOMPILE_SUB(tapered->getPrimitive());
  GEOM_GLRENDERER_PRECOMPILE_END(tapered);

  GEOM_GLRENDERER_CHECK_CACHE(tapered);

  PrimitivePtr _primitive = tapered->getPrimitive();
  if (_primitive->apply(__discretizer)) {
    ExplicitModelPtr _explicit = __discretizer.getDiscretization();

    Transformation3DPtr _taper(tapered->getTransformation());
    ExplicitModelPtr _tExplicit = _explicit->transform(_taper);
    _tExplicit->apply(*this);

    GEOM_GLRENDERER_UPDATE_CACHE(tapered);
    return true;
  } else return false;
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(Translated * translated) {
  GEOM_ASSERT_OBJ(translated);
  GEOM_GLRENDERER_PRECOMPILE_BEG(translated);
    GEOM_GLRENDERER_PRECOMPILE_SUB(translated->getGeometry());
  GEOM_GLRENDERER_PRECOMPILE_END(translated);

  GEOM_GLRENDERER_CHECK_CACHE(translated);

  GL_PUSH_MATRIX(translated->getGeometry());
  __ogl->glGeomTranslate(translated->getTranslation());
  translated->getGeometry()->apply(*this);
  GL_POP_MATRIX(translated->getGeometry());

  GEOM_GLRENDERER_UPDATE_CACHE(translated);
  GL_ERROR_CHECK;
  return true;
}


/* ----------------------------------------------------------------------- */
#include <iostream>

using namespace std;

bool GLRenderer::process(TriangleSet * triangleSet) {
  GEOM_ASSERT_OBJ(triangleSet);
  GEOM_GLRENDERER_PRECOMPILE(triangleSet);

  GEOM_GLRENDERER_CHECK_CACHE(triangleSet);

  bool normalV = triangleSet->getNormalPerVertex();
  bool tex = __appearance && __appearance->isTexture() && triangleSet->hasTexCoordList();
  bool color = triangleSet->hasColorList();
  bool colorV = triangleSet->getColorPerVertex();

  __ogl->glFrontFace(triangleSet->getCCW() ? GL_CCW : GL_CW);
  if (__appearance && __appearance->isTexture() && !triangleSet->hasTexCoordList()) {
    __ogl->glEnable(GL_TEXTURE_GEN_S);
    __ogl->glEnable(GL_TEXTURE_GEN_T);
  } else if (color) {
    __ogl->glDisable(GL_TEXTURE_2D);
    __ogl->glBindTexture(GL_TEXTURE_2D, 0);
  }

  triangleSet->checkNormalList();
  GL_ERROR_CHECK;

  if(!__withvertexarray){
    __ogl->glBegin(GL_TRIANGLES);
    uint_t _size = triangleSet->getIndexListSize();
    GLfloat _rgba[4];
    for (uint_t _i = 0; _i < _size; _i++) {
      if (!normalV){
        __ogl->glGeomNormal(triangleSet->getNormalAt(_i));
      }
      if (color && !colorV) {
        const Color4 &_ambient = triangleSet->getColorAt(_i);
        _rgba[0] = (GLfloat) _ambient.getRedClamped();
        _rgba[1] = (GLfloat) _ambient.getGreenClamped();
        _rgba[2] = (GLfloat) _ambient.getBlueClamped();
        _rgba[3] = 1.0f - (GLfloat) _ambient.getAlphaClamped();
        __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _rgba);
        __ogl->glColor4fv(_rgba);
      }
      for (uint_t _j = 0; _j < 3; _j++) {
        if (normalV)
          __ogl->glGeomNormal(triangleSet->getFaceNormalAt(_i, _j));
        if (tex)
          __ogl->glGeomTexCoord(triangleSet->getFaceTexCoordAt(_i, _j));
        else if (color && colorV) {
          const Color4 &_ambient = triangleSet->getFaceColorAt(_i, _j);
          _rgba[0] = (GLfloat) _ambient.getRedClamped();
          _rgba[1] = (GLfloat) _ambient.getGreenClamped();
          _rgba[2] = (GLfloat) _ambient.getBlueClamped();
          _rgba[3] = 1.0f - (GLfloat) _ambient.getAlphaClamped();
          __ogl->glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, _rgba);
          __ogl->glColor4fv(_rgba);
        }
        __ogl->glGeomVertex(triangleSet->getFacePointAt(_i, _j));
      }
    };
    __ogl->glEnd();
    GL_ERROR_CHECK;
  }
  else {

    TriangleSetPtr cTriangleSet = triangleSet->canonicalMesh();

    __ogl->glEnableClientState(GL_VERTEX_ARRAY);
    real_t * vertices = cTriangleSet->getPointList()->data();
    __ogl->glVertexPointer( 3, GL_GEOM_REAL,0,vertices);
    size_t si = cTriangleSet->getIndexList()->size();

    real_t * normals = NULL;
    if (normalV) {
      normals = cTriangleSet->getNormalList()->data();
      __ogl->glEnableClientState(GL_NORMAL_ARRAY);
      __ogl->glNormalPointer( GL_GEOM_REAL,0,normals);
    }

    real_t * texCoord = NULL;
    real_t * colors = NULL;

    if (tex) {
      texCoord = cTriangleSet->getTexCoordList()->data();
      __ogl->glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      __ogl->glTexCoordPointer( 2, GL_GEOM_REAL,0,texCoord);
    }
    else if ( color ) {
      printf("set color buffer\n");

      colors = cTriangleSet->getColorList()->toRealArray();
      __ogl->glEnableClientState( GL_COLOR_ARRAY );
      __ogl->glColorPointer( 4, GL_GEOM_REAL, 0, colors );
    }

    // if (normalV) {
      uint_t * indices = triangleSet->getIndexList()->data();
      __ogl->glDrawElements( GL_TRIANGLES, 3*si , GL_UNSIGNED_INT, indices);
      delete [] indices;
    // }
    /*else {
      size_t _i = 0;
      for(Index3Array::const_iterator it = triangleSet->getIndexList()->begin();
      it != triangleSet->getIndexList()->end(); it++, _i++) {
            glGeomNormal(triangleSet->getNormalAt(_i));
            glDrawElements(   GL_TRIANGLES, 3 , GL_UNSIGNED_INT, it->begin());
            }

    }*/

    __ogl->glDisableClientState(GL_VERTEX_ARRAY);
    if (normalV)__ogl->glDisableClientState(GL_NORMAL_ARRAY);
    if ( color )__ogl->glDisableClientState(GL_NORMAL_ARRAY);
    if (tex)__ogl->glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if (normalV)__ogl->glDisableClientState(GL_COLOR_ARRAY);

    delete [] vertices;
    if (normals)  delete [] normals;
    if (texCoord) delete [] texCoord;
    if (colors)   delete [] colors;
  }

  if (__appearance && __appearance->isTexture() && !triangleSet->hasTexCoordList()) {
    __ogl->glDisable(GL_TEXTURE_GEN_S);
    __ogl->glDisable(GL_TEXTURE_GEN_T);
  }

  GEOM_GLRENDERER_UPDATE_CACHE(triangleSet);
  GL_ERROR_CHECK;
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(BezierCurve2D * bezierCurve) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(bezierCurve);
}

/* ----------------------------------------------------------------------- */


bool GLRenderer::process(Disc * disc) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(disc);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(NurbsCurve2D * nurbsCurve) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(nurbsCurve);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(PointSet2D * pointSet) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(pointSet);
}


/* ----------------------------------------------------------------------- */


bool GLRenderer::process(Polyline2D * polyline) {
  GEOM_GLRENDERER_DISCRETIZE_RENDER(polyline);
}


/* ----------------------------------------------------------------------- */

#ifndef PGL_CORE_WITHOUT_QT

bool GLRenderer::process(Text * text) {
  GEOM_ASSERT_OBJ(text);
  if (__Mode == Selection) {
    return true;
  } else {
    __ogl->glPushAttrib(GL_LIGHTING_BIT | GL_DEPTH_BUFFER_BIT);
    __ogl->glDisable(GL_LIGHTING);
    __ogl->glDisable(GL_DEPTH_TEST);
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
          __ogl->glMatrixMode(GL_PROJECTION);
          __ogl->glPushMatrix();
          __ogl->glLoadIdentity();
          __ogl->glOrtho(0, 100, 0, 100, 1, -100);
          __ogl->glMatrixMode(GL_MODELVIEW);
          __ogl->glPushMatrix();
          __ogl->glLoadIdentity();
        }

        const Vector3 &position = text->getPosition();

        // Retrieve last OpenGL color to use as a font color

#ifdef PGL_USE_QOPENGLWIDGET
        GLdouble glColor[4];
        __ogl->glGetDoublev(GL_CURRENT_COLOR, glColor);
        QColor fontColor = QColor(glColor[0]*255, glColor[1]*255, glColor[2]*255, 255);

        GLint viewport[4];
        GLdouble modelMatrix[16];
        GLdouble projMatrix[16];
        __ogl->glGetIntegerv(GL_VIEWPORT, viewport);
        __ogl->glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
        __ogl->glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
        GLdouble winX, winY, winZ;
        gluProject(position.x(), position.y(),position.z(),modelMatrix, projMatrix, viewport, &winX, &winY, &winZ);
        if (winZ > 0) {

          // Render text
          double _scalingfactor = __glframe->window()->devicePixelRatio();
          QPainter painter(__glframe);
          painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
          painter.setPen(fontColor);
          painter.setFont(f);
          painter.drawText( int(winX)/_scalingfactor, __glframe->height()-int(winY)/_scalingfactor, QString(text->getString().c_str()));
          painter.end();
        }
#else
          __glframe->renderText(position.x(), position.y(), position.z(), QString(text->getString().c_str()), f);
#endif

        if (text->getScreenCoordinates()) {
          __ogl->glMatrixMode(GL_PROJECTION);
          __ogl->glPopMatrix();
          __ogl->glMatrixMode(GL_MODELVIEW);
          __ogl->glPopMatrix();
        }

      }
    }
    __ogl->glPopAttrib();
    GL_ERROR_CHECK;    
  }
  return true;
}

#else

bool GLRenderer::process( Text * text ) {
  GEOM_ASSERT_OBJ(text);
  return true;
}

#endif


/* ----------------------------------------------------------------------- */

bool GLRenderer::process(Font * font) {
  GEOM_ASSERT_OBJ(font);
  return true;
}

/* ----------------------------------------------------------------------- */
