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

#include "glbboxrenderer.h"
#include <plantgl/algo/base/bboxcomputer.h>

#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/scenegraph/geometry/boundingbox.h>

#include "util_glut.h"

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

#define GEOM_GLBBOXRENDERER_CHECK_APPEARANCE(app) \
  if ((__appearance) && \
      (__appearance->getId() == app->getId())) return true;

#define GEOM_GLBBOXRENDERER_UPDATE_APPEARANCE(app) \
  __appearance = AppearancePtr(app);


#define GEOM_GLBBOXRENDERER_DRAW_BBOX(bbox) \
  GEOM_ASSERT(bbox); \
  glPushMatrix(); \
  Vector3 _bboxCenter = bbox->getCenter(); \
  Vector3 _bboxSize = bbox->getSize(); \
  glGeomTranslate(_bboxCenter); \
  glGeomScale(_bboxSize*2); \
  glutSolidCube(1); \
  glPopMatrix();


#define GEOM_GLBBOXRENDERER(geom)\
   GEOM_ASSERT(geom); \
   if(geom->apply(__bboxComputer)){ \
    BoundingBoxPtr _bbox = __bboxComputer.getBoundingBox(); \
    GEOM_GLBBOXRENDERER_DRAW_BBOX(_bbox); \
    GEOM_ASSERT(glGetError() == GL_NO_ERROR); \
    return true; } \
   else return false;


/* ----------------------------------------------------------------------- */


GLBBoxRenderer::GLBBoxRenderer( BBoxComputer& bboxComputer ) :
  GLRenderer(bboxComputer.getDiscretizer()),
  __bboxComputer(bboxComputer) {
}

GLBBoxRenderer::~GLBBoxRenderer( ) {
}

/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( AmapSymbol * amapSymbol ) {
  GEOM_GLBBOXRENDERER(amapSymbol);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( AsymmetricHull * asymmetricHull ) {
  GEOM_GLBBOXRENDERER(asymmetricHull);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( AxisRotated * axisRotated ) {
  GEOM_GLBBOXRENDERER(axisRotated);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( BezierCurve * bezierCurve ) {
  GEOM_GLBBOXRENDERER(bezierCurve);
}

/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( BezierPatch * bezierPatch ) {
  GEOM_GLBBOXRENDERER(bezierPatch);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( Box * box ) {
  GEOM_GLBBOXRENDERER(box);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( Cone * cone ) {
  GEOM_GLBBOXRENDERER(cone);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( Cylinder * cylinder ) {
  GEOM_GLBBOXRENDERER(cylinder);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( ElevationGrid * elevationGrid ) {
  GEOM_GLBBOXRENDERER(elevationGrid);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( EulerRotated * eulerRotated ) {
  GEOM_GLBBOXRENDERER(eulerRotated);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( ExtrudedHull * extrudedHull ) {
  GEOM_GLBBOXRENDERER(extrudedHull);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( FaceSet * faceSet ) {
  GEOM_GLBBOXRENDERER(faceSet);
}


/* ----------------------------------------------------------------------- */

bool GLBBoxRenderer::process( Frustum * frustum ) {
  GEOM_GLBBOXRENDERER(frustum);
}


/* ----------------------------------------------------------------------- */

bool GLBBoxRenderer::process( Extrusion * extrusion ) {
  GEOM_GLBBOXRENDERER(extrusion);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( Group * group ) {
  GEOM_GLBBOXRENDERER(group);
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( IFS * ifs ) {
  GEOM_GLBBOXRENDERER(ifs);
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( Material * material ) {
  GEOM_ASSERT(material);

  GEOM_GLBBOXRENDERER_CHECK_APPEARANCE(material);

  const real_t& _diffuse = material->getDiffuse();
  const Color3& _ambient = material->getAmbient();
  GLfloat _rgba[4];
  _rgba[0] = _ambient.getRedClamped();
  _rgba[1] = _ambient.getGreenClamped();
  _rgba[2] = _ambient.getBlueClamped();
  _rgba[3] = 0.6f;
  glMaterialfv(GL_FRONT,GL_AMBIENT,_rgba);
  _rgba[0] *= _diffuse;
  _rgba[1] *= _diffuse;
  _rgba[2] *= _diffuse;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,_rgba);

  GEOM_GLBBOXRENDERER_UPDATE_APPEARANCE(material);

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( MonoSpectral * monoSpectral ) {
  GEOM_ASSERT(monoSpectral);

 GEOM_GLBBOXRENDERER_CHECK_APPEARANCE(monoSpectral);

  GLfloat _rgba[4];
  _rgba[0] = (GLfloat)monoSpectral->getReflectance();
  _rgba[1] = (GLfloat)monoSpectral->getReflectance();
  _rgba[2] = (GLfloat)monoSpectral->getReflectance();
  _rgba[3] = 0.6f;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,_rgba);

  GEOM_GLBBOXRENDERER_UPDATE_APPEARANCE(monoSpectral);

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( MultiSpectral * multiSpectral ) {
  GEOM_ASSERT(multiSpectral);

  GEOM_GLBBOXRENDERER_CHECK_APPEARANCE(multiSpectral);

  GLfloat _rgba[4];
  const Index3& _filter = multiSpectral->getFilter();
  _rgba[0] = (GLfloat)multiSpectral->getReflectanceAt(_filter.getAt(0));
  _rgba[1] = (GLfloat)multiSpectral->getReflectanceAt(_filter.getAt(1));
  _rgba[2] = (GLfloat)multiSpectral->getReflectanceAt(_filter.getAt(2));
  _rgba[3] = 0.6f;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,_rgba);

  GEOM_GLBBOXRENDERER_UPDATE_APPEARANCE(multiSpectral);

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}



/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( NurbsCurve * nurbsCurve ) {
  GEOM_GLBBOXRENDERER(nurbsCurve);
}

/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( NurbsPatch * nurbsPatch ) {
  GEOM_GLBBOXRENDERER(nurbsPatch);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( Oriented * oriented ) {
  GEOM_GLBBOXRENDERER(oriented);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( Paraboloid * paraboloid ) {
  GEOM_GLBBOXRENDERER(paraboloid);
}

/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( PointSet * pointSet ) {
  GEOM_GLBBOXRENDERER(pointSet);
}



/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( PGL(Polyline) * polyline ) {
  GEOM_GLBBOXRENDERER(polyline);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( QuadSet * quadSet ) {
  GEOM_GLBBOXRENDERER(quadSet);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( Revolution * revolution ) {
  GEOM_GLBBOXRENDERER(revolution);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( Swung * swung )
{
  GEOM_GLBBOXRENDERER(swung);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( Scaled * scaled ) {
  GEOM_GLBBOXRENDERER(scaled);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( Sphere * sphere ) {
  GEOM_GLBBOXRENDERER(sphere);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( Tapered * tapered ) {
  GEOM_GLBBOXRENDERER(tapered);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( Translated * translated ) {
  GEOM_GLBBOXRENDERER(translated);
}


/* ----------------------------------------------------------------------- */


bool GLBBoxRenderer::process( TriangleSet * triangleSet ) {
  GEOM_GLBBOXRENDERER(triangleSet);
}



/* ----------------------------------------------------------------------- */



bool GLBBoxRenderer::process( BezierCurve2D * bezierCurve ) {
  GEOM_GLBBOXRENDERER(bezierCurve);
}


/* ----------------------------------------------------------------------- */



bool GLBBoxRenderer::process( Disc * disc ) {
  GEOM_GLBBOXRENDERER(disc);
}


/* ----------------------------------------------------------------------- */



bool GLBBoxRenderer::process( NurbsCurve2D * nurbsCurve  ) {
  GEOM_GLBBOXRENDERER(nurbsCurve);
}


/* ----------------------------------------------------------------------- */



bool GLBBoxRenderer::process( PointSet2D * pointSet   ) {
  GEOM_GLBBOXRENDERER(pointSet);
}


/* ----------------------------------------------------------------------- */

bool GLBBoxRenderer::process( Polyline2D * polyline  ) {
  GEOM_GLBBOXRENDERER(polyline);
}

/* ----------------------------------------------------------------------- */

bool GLBBoxRenderer::process( Text * text ) {
  GEOM_ASSERT(text);
  return true;
}

/* ----------------------------------------------------------------------- */

bool GLBBoxRenderer::process( Font * font ) {
  GEOM_ASSERT(font);
  return true;
}

/* ----------------------------------------------------------------------- */
