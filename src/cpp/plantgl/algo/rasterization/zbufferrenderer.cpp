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




#include "zbufferrenderer.h"
#include <plantgl/algo/base/discretizer.h>

#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/pgl_scene.h>


PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE



#define GEOM_ASSERT_OBJ(obj)

/* ----------------------------------------------------------------------- */


template<class T>
bool ZBufferRenderer::discretize_and_render(T *geom) 
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
bool ZBufferRenderer::tesselate_and_render(T *geom) 
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
bool ZBufferRenderer::transform_and_render(T * geom)
{
  GEOM_ASSERT_OBJ(geom);
   __engine.pushModelTransformation(); 
   Matrix4TransformationPtr _basis = dynamic_pointer_cast<Matrix4Transformation>(geom->getTransformation());
   GEOM_ASSERT(_basis);
   __engine.transformModel(_basis->getMatrix());
   bool b = geom->getGeometry()->apply(*this);
   __engine.popModelTransformation();
   return b;
}


/* ----------------------------------------------------------------------- */


ZBufferRenderer::ZBufferRenderer(ZBufferEngine& engine, Tesselator& tesselator, Discretizer &discretizer) :
        Action(),
        __engine(engine),
        __tesselator(tesselator),
        __discretizer(discretizer),
        __appearance(),
        __id(Shape::NOID)
{
}


ZBufferRenderer::~ZBufferRenderer() {
}

Discretizer &
ZBufferRenderer::getDiscretizer() {
  return __discretizer;
}



bool ZBufferRenderer::beginProcess() {
  return true;
}


bool ZBufferRenderer::endProcess() {
  return true;
}

/* ----------------------------------------------------------------------- */
bool ZBufferRenderer::process(Shape * geomshape) {
  GEOM_ASSERT_OBJ(geomshape);
  __engine.setId(geomshape->getId());
  if (geomshape->appearance) 
    __appearance = geomshape->appearance;
  else 
    __appearance = Material::DEFAULT_MATERIAL;
  return geomshape->geometry->apply(*this);
}



/* ----------------------------------------------------------------------- */

bool ZBufferRenderer::process(Inline *geomInline) {
  GEOM_ASSERT_OBJ(geomInline);
  __engine.setId(geomInline->getId());
  if (geomInline->getScene()) {
    if (!geomInline->isTranslationToDefault() || !geomInline->isScaleToDefault()) {
       __engine.pushModelTransformation(); 
      const Vector3 _trans = geomInline->getTranslation();
      __engine.translateModel(_trans);
      const Vector3 _scale = geomInline->getScale();
      __engine.scaleModel(_scale);
    }
    bool _result = true;
    for (Scene::iterator _i = geomInline->getScene()->begin();
         _i != geomInline->getScene()->end();
         _i++) {
        if (!(*_i)->apply(*this)) _result = false;
    };
    if (!geomInline->isTranslationToDefault() || !geomInline->isScaleToDefault()) {
       __engine.popModelTransformation(); 
    }

    return _result;
  } 
  else return false;
}

/* ----------------------------------------------------------------------- */

bool ZBufferRenderer::process(AmapSymbol *amapSymbol) {
  return tesselate_and_render(amapSymbol);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(AsymmetricHull *asymmetricHull) {
  return tesselate_and_render(asymmetricHull);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(AxisRotated *axisRotated) {
  return transform_and_render(axisRotated);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(BezierCurve *bezierCurve) {
  return discretize_and_render(bezierCurve);
}

/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(BezierPatch *bezierPatch) {
  return tesselate_and_render(bezierPatch);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(Box *box) {
  return tesselate_and_render(box);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(Cone *cone) {
  return tesselate_and_render(cone);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(Cylinder *cylinder) {
  return tesselate_and_render(cylinder);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(ElevationGrid *elevationGrid) {
  return tesselate_and_render(elevationGrid);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(EulerRotated *eulerRotated) {
  return transform_and_render(eulerRotated);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(ExtrudedHull *extrudedHull) {
  return tesselate_and_render(extrudedHull);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(FaceSet *faceSet) {
  return tesselate_and_render(faceSet);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(Frustum *frustum) {
  return tesselate_and_render(frustum);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(Extrusion *extrusion) {
  return tesselate_and_render(extrusion);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(Group *group) {
  return group->getGeometryList()->apply(*this);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(IFS *ifs) {
  GEOM_ASSERT_OBJ(ifs);

  ITPtr transfos = dynamic_pointer_cast<IT>(ifs->getTransformation());
  GEOM_ASSERT(transfos);
  const Matrix4ArrayPtr &matrixList = transfos->getAllTransfo();
  GEOM_ASSERT(matrixList);

  bool res = true;
  
  for (Matrix4Array::const_iterator itmatrix = matrixList->begin(); itmatrix != matrixList->end(); ++itmatrix) {
    __engine.pushModelTransformation(); 
    __engine.transformModel(*itmatrix);
    res = res && ifs->getGeometry()->apply(*this);
    __engine.popModelTransformation();   
  }

  return res;
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(Material *material) {
  __appearance = AppearancePtr(material);
  return true;
}


/* ----------------------------------------------------------------------- */
bool ZBufferRenderer::process(ImageTexture *texture) {
  GEOM_ASSERT_OBJ(texture);
  return true;
}


/* ----------------------------------------------------------------------- */

bool ZBufferRenderer::process(Texture2D *texture) {
  GEOM_ASSERT_OBJ(texture);
  __appearance = AppearancePtr(texture);
  return true;
}

/* ----------------------------------------------------------------------- */

bool ZBufferRenderer::process(Texture2DTransformation *texturetransfo) 
{
  GEOM_ASSERT_OBJ(texturetransfo);
  return true;
}

/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(MonoSpectral *monoSpectral) {
  GEOM_ASSERT_OBJ(monoSpectral);
  return true;
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(MultiSpectral *multiSpectral) {
  GEOM_ASSERT_OBJ(multiSpectral);
  return true;
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(NurbsCurve *nurbsCurve) {
  return discretize_and_render(nurbsCurve);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(NurbsPatch *nurbsPatch) {
  return tesselate_and_render(nurbsPatch);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(Oriented *oriented) {
  return transform_and_render(oriented);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(Paraboloid *paraboloid) {
  return tesselate_and_render(paraboloid);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(PointSet *pointSet) {
  GEOM_ASSERT_OBJ(pointSet);
  if (__appearance->isTexture()){
    __engine.render(PointSetPtr(pointSet), dynamic_pointer_cast<Material>(Material::DEFAULT_MATERIAL));    
  }
  else {
    __engine.render(PointSetPtr(pointSet), dynamic_pointer_cast<Material>(__appearance));    
  }
  return true;
}


/* ----------------------------------------------------------------------- */

bool ZBufferRenderer::process(Polyline *polyline) {
  GEOM_ASSERT_OBJ(polyline);
  if (__appearance->isTexture()){
    __engine.render(PolylinePtr(polyline), dynamic_pointer_cast<Material>(Material::DEFAULT_MATERIAL));    
  }
  else {
    __engine.render(PolylinePtr(polyline), dynamic_pointer_cast<Material>(__appearance));
  }
  return true;
}


/* ----------------------------------------------------------------------- */




bool ZBufferRenderer::process(QuadSet *quadSet) {
  return tesselate_and_render(quadSet);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(Revolution *revolution) {
  return tesselate_and_render(revolution);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(Swung *swung) {
  return tesselate_and_render(swung);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(Scaled *scaled) {
  return transform_and_render(scaled);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(ScreenProjected *scp) {
  GEOM_ASSERT_OBJ(scp);
  return true;
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(Sphere *sphere) {
  return tesselate_and_render(sphere);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(Tapered *tapered) {
  return discretize_and_render(tapered);

}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(Translated *translated) {
  return transform_and_render(translated);

}


/* ----------------------------------------------------------------------- */

bool ZBufferRenderer::process(TriangleSet *triangleSet) {
  GEOM_ASSERT_OBJ(triangleSet);
  __engine.render(TriangleSetPtr(triangleSet), __appearance);    
  return true;
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(BezierCurve2D *bezierCurve) {
  return discretize_and_render(bezierCurve);
}

/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(Disc *disc) {
  return tesselate_and_render(disc);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(NurbsCurve2D *nurbsCurve) {
  return discretize_and_render(nurbsCurve);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(PointSet2D *pointSet) {
  return discretize_and_render(pointSet);
}


/* ----------------------------------------------------------------------- */


bool ZBufferRenderer::process(Polyline2D *polyline) {
  return discretize_and_render(polyline);
}


/* ----------------------------------------------------------------------- */

bool ZBufferRenderer::process( Text * text ) {
  GEOM_ASSERT_OBJ(text);
  return true;
}

/* ----------------------------------------------------------------------- */

bool ZBufferRenderer::process(Font *font) {
  GEOM_ASSERT_OBJ(font);
  return true;
}

/* ----------------------------------------------------------------------- */
