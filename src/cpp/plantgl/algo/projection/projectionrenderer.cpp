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





#include "projectionrenderer.h"
#include <plantgl/algo/base/discretizer.h>

#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/pgl_scene.h>


PGL_USING_NAMESPACE



#define GEOM_ASSERT_OBJ(obj)

/* ----------------------------------------------------------------------- */


template<class T>
bool ProjectionRenderer::discretize_and_process(T *geom) 
{
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

template<class T>
bool ProjectionRenderer::tesselate_and_process(T *geom) 
{
  GEOM_ASSERT_OBJ(geom);
  if (__appearance && __appearance->isTexture())
    __tesselator.computeTexCoord(true);
  else __tesselator.computeTexCoord(false);
  bool b = geom->apply(__tesselator);
  if (b && (b = (__tesselator.getDiscretization()))) {
    b = __tesselator.getDiscretization()->apply(*this);
  }
  return b;
}

template<class T> 
bool ProjectionRenderer::transform_and_process(T * geom)
{
  GEOM_ASSERT_OBJ(geom);
   __camera->pushModelTransformation(); 
   Matrix4TransformationPtr _basis = dynamic_pointer_cast<Matrix4Transformation>(geom->getTransformation());
   GEOM_ASSERT(_basis);
   __camera->transformModel(_basis->getMatrix());
   bool b = geom->getGeometry()->apply(*this);
   __camera->popModelTransformation();
   return b;
}


/* ----------------------------------------------------------------------- */


ProjectionRenderer::ProjectionRenderer(ProjectionEngine& engine, Tesselator& tesselator, Discretizer &discretizer, uint32_t threadid) :
        Action(),
        __engine(engine),
        __camera(engine.camera()),
        __tesselator(tesselator),
        __discretizer(discretizer),
        __appearance(),
        __id(Shape::NOID),
        __threadid(threadid)
{
}



ProjectionRenderer::ProjectionRenderer(ProjectionEngine& engine, ProjectionCameraPtr camera, Tesselator& tesselator, Discretizer &discretizer, uint32_t threadid) :
        Action(),
        __engine(engine),
        __camera(camera),
        __tesselator(tesselator),
        __discretizer(discretizer),
        __appearance(),
        __id(Shape::NOID),
        __threadid(threadid)
{
}


ProjectionRenderer::~ProjectionRenderer() {
}

Discretizer &
ProjectionRenderer::getDiscretizer() {
  return __discretizer;
}



bool ProjectionRenderer::beginProcess() {
  return true;
}


bool ProjectionRenderer::endProcess() {
  return true;
}

/* ----------------------------------------------------------------------- */
bool ProjectionRenderer::process(Shape * geomshape) {
  GEOM_ASSERT_OBJ(geomshape);
  __id = geomshape->getId();
  if (geomshape->appearance) 
    __appearance = geomshape->appearance;
  else 
    __appearance = Material::DEFAULT_MATERIAL;
  return geomshape->geometry->apply(*this);
}



/* ----------------------------------------------------------------------- */

bool ProjectionRenderer::process(Inline *geomInline) {
  GEOM_ASSERT_OBJ(geomInline);
  __id = geomInline->getObjectId();
  if (geomInline->getScene()) {
    if (!geomInline->isTranslationToDefault() || !geomInline->isScaleToDefault()) {
       __camera->pushModelTransformation(); 
      const Vector3 _trans = geomInline->getTranslation();
      __camera->translateModel(_trans);
      const Vector3 _scale = geomInline->getScale();
      __camera->scaleModel(_scale);
    }
    bool _result = true;
    for (Scene::iterator _i = geomInline->getScene()->begin();
         _i != geomInline->getScene()->end();
         _i++) {
        if (!(*_i)->apply(*this)) _result = false;
    };
    if (!geomInline->isTranslationToDefault() || !geomInline->isScaleToDefault()) {
       __camera->popModelTransformation(); 
    }

    return _result;
  } 
  else return false;
}

/* ----------------------------------------------------------------------- */

bool ProjectionRenderer::process(AmapSymbol *amapSymbol) {
  return tesselate_and_process(amapSymbol);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(AsymmetricHull *asymmetricHull) {
  return tesselate_and_process(asymmetricHull);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(AxisRotated *axisRotated) {
  return transform_and_process(axisRotated);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(BezierCurve *bezierCurve) {
  return discretize_and_process(bezierCurve);
}

/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(BezierPatch *bezierPatch) {
  return tesselate_and_process(bezierPatch);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(Box *box) {
  return tesselate_and_process(box);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(Cone *cone) {
  return tesselate_and_process(cone);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(Cylinder *cylinder) {
  return tesselate_and_process(cylinder);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(ElevationGrid *elevationGrid) {
  return tesselate_and_process(elevationGrid);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(EulerRotated *eulerRotated) {
  return transform_and_process(eulerRotated);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(ExtrudedHull *extrudedHull) {
  return tesselate_and_process(extrudedHull);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(FaceSet *faceSet) {
  return tesselate_and_process(faceSet);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(Frustum *frustum) {
  return tesselate_and_process(frustum);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(Extrusion *extrusion) {
  return tesselate_and_process(extrusion);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(Group *group) {
  return group->getGeometryList()->apply(*this);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(IFS *ifs) {
  GEOM_ASSERT_OBJ(ifs);

  ITPtr transfos = dynamic_pointer_cast<IT>(ifs->getTransformation());
  GEOM_ASSERT(transfos);
  const Matrix4ArrayPtr &matrixList = transfos->getAllTransfo();
  GEOM_ASSERT(matrixList);

  bool res = true;
  
  for (Matrix4Array::const_iterator itmatrix = matrixList->begin(); itmatrix != matrixList->end(); ++itmatrix) {
    __camera->pushModelTransformation(); 
    __camera->transformModel(*itmatrix);
    res = res && ifs->getGeometry()->apply(*this);
    __camera->popModelTransformation();   
  }

  return res;
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(Material *material) {
  __appearance = AppearancePtr(material);
  return true;
}


/* ----------------------------------------------------------------------- */
bool ProjectionRenderer::process(ImageTexture *texture) {
  GEOM_ASSERT_OBJ(texture);
  return true;
}


/* ----------------------------------------------------------------------- */

bool ProjectionRenderer::process(Texture2D *texture) {
  GEOM_ASSERT_OBJ(texture);
  __appearance = AppearancePtr(texture);
  return true;
}

/* ----------------------------------------------------------------------- */

bool ProjectionRenderer::process(Texture2DTransformation *texturetransfo) 
{
  GEOM_ASSERT_OBJ(texturetransfo);
  return true;
}

/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(MonoSpectral *monoSpectral) {
  GEOM_ASSERT_OBJ(monoSpectral);
  return true;
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(MultiSpectral *multiSpectral) {
  GEOM_ASSERT_OBJ(multiSpectral);
  return true;
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(NurbsCurve *nurbsCurve) {
  return discretize_and_process(nurbsCurve);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(NurbsPatch *nurbsPatch) {
  return tesselate_and_process(nurbsPatch);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(Oriented *oriented) {
  return transform_and_process(oriented);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(Paraboloid *paraboloid) {
  return tesselate_and_process(paraboloid);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(PointSet *pointSet) {
  GEOM_ASSERT_OBJ(pointSet);
  if (__appearance->isTexture()){
    __engine.iprocess(PointSetPtr(pointSet), dynamic_pointer_cast<Material>(Material::DEFAULT_MATERIAL),__id, __camera, __threadid);    
  }
  else {
    __engine.iprocess(PointSetPtr(pointSet), dynamic_pointer_cast<Material>(__appearance),__id, __camera, __threadid);    
  }
  return true;
}


/* ----------------------------------------------------------------------- */

bool ProjectionRenderer::process(Polyline *polyline) {
  GEOM_ASSERT_OBJ(polyline);
  if (__appearance->isTexture()){
    __engine.iprocess(PolylinePtr(polyline), dynamic_pointer_cast<Material>(Material::DEFAULT_MATERIAL),__id, __camera, __threadid);    
  }
  else {
    __engine.iprocess(PolylinePtr(polyline), dynamic_pointer_cast<Material>(__appearance), __id, __camera, __threadid);
  }
  return true;
}


/* ----------------------------------------------------------------------- */




bool ProjectionRenderer::process(QuadSet *quadSet) {
  return tesselate_and_process(quadSet);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(Revolution *revolution) {
  return tesselate_and_process(revolution);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(Swung *swung) {
  return tesselate_and_process(swung);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(Scaled *scaled) {
  return transform_and_process(scaled);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(ScreenProjected *scp) {
  GEOM_ASSERT_OBJ(scp);
  real_t heigthscale = 1;
  ImageProjectionEngine * mengine = dynamic_cast<ImageProjectionEngine *>(&__engine);
  if(mengine != NULL) { 
    heigthscale = float(mengine->getImageHeight()) / float(mengine->getImageWidth());
  }
  ProjectionCameraPtr camera = __camera;
  __camera = ProjectionCamera::orthographicCamera(-1, 1, -heigthscale, heigthscale, 1, 3);
  __camera->lookAt(Vector3(0,-2,0), Vector3(0,0,0), Vector3(0,0,1));
  printf("Apply it with new camera\n" );
  bool b = scp->getGeometry()->apply(*this);
  __camera = camera;
  return b;
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(Sphere *sphere) {
  return tesselate_and_process(sphere);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(Tapered *tapered) {
  return discretize_and_process(tapered);

}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(Translated *translated) {
  return transform_and_process(translated);

}


/* ----------------------------------------------------------------------- */

bool ProjectionRenderer::process(TriangleSet *triangleSet) {
  GEOM_ASSERT_OBJ(triangleSet);
  __engine.iprocess(TriangleSetPtr(triangleSet), __appearance, __id, __camera, __threadid);    
  return true;
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(BezierCurve2D *bezierCurve) {
  return discretize_and_process(bezierCurve);
}

/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(Disc *disc) {
  return tesselate_and_process(disc);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(NurbsCurve2D *nurbsCurve) {
  return discretize_and_process(nurbsCurve);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(PointSet2D *pointSet) {
  return discretize_and_process(pointSet);
}


/* ----------------------------------------------------------------------- */


bool ProjectionRenderer::process(Polyline2D *polyline) {
  return discretize_and_process(polyline);
}


/* ----------------------------------------------------------------------- */

bool ProjectionRenderer::process( Text * text ) {
  GEOM_ASSERT_OBJ(text);
  return true;
}

/* ----------------------------------------------------------------------- */

bool ProjectionRenderer::process(Font *font) {
  GEOM_ASSERT_OBJ(font);
  return true;
}

/* ----------------------------------------------------------------------- */
