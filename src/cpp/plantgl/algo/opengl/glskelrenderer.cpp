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


#include "glskelrenderer.h"

#include <plantgl/algo/base/skelcomputer.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/scenegraph/container/pointarray.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */




#define GEOM_GLSKELRENDERER_DRAW_SKEL(skel) { \
    glBegin(GL_LINE_STRIP); \
    const Point3ArrayPtr& points = skel->getPointList(); \
    for (Point3Array::const_iterator _i = points->getBegin(); \
         _i != points->getEnd(); \
         _i++) \
      glGeomVertex(*_i); \
    glEnd(); \
  };

#define GEOM_GLSKELRENDERER_GEOM(geom){ \
  GEOM_ASSERT(geom); \
  if(!(geom->apply(__skelComputer)))return false; \
  PolylinePtr _skel = __skelComputer.getSkeleton(); \
  if (_skel){ \
      GEOM_GLSKELRENDERER_DRAW_SKEL(_skel); \
  } \
  else return false; \
  GEOM_ASSERT(glGetError() == GL_NO_ERROR); \
  return true; \
 };


/* ----------------------------------------------------------------------- */


GLSkelRenderer::GLSkelRenderer( SkelComputer& skelComputer ) :
  GLRenderer(skelComputer.getDiscretizer()),
  __skelComputer(skelComputer) {
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( AmapSymbol * amapSymbol ) {
  GEOM_GLSKELRENDERER_GEOM(amapSymbol);

  PolylinePtr _skel = amapSymbol->getSkeleton();
  if (_skel){
      GEOM_GLSKELRENDERER_DRAW_SKEL(_skel);
  }
  else return false;

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( AsymmetricHull * asymmetricHull ) {
  GEOM_GLSKELRENDERER_GEOM(asymmetricHull);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( AxisRotated * axisRotated ) {
  GEOM_GLSKELRENDERER_GEOM(axisRotated);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( BezierCurve * bezierCurve ) {
  GEOM_GLSKELRENDERER_GEOM(bezierCurve);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( BezierPatch * bezierPatch ) {
  GEOM_GLSKELRENDERER_GEOM(bezierPatch);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( Box * box ) {
  GEOM_GLSKELRENDERER_GEOM(box);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( Cone * cone ) {
  GEOM_GLSKELRENDERER_GEOM(cone);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( Cylinder * cylinder ) {
  GEOM_GLSKELRENDERER_GEOM(cylinder);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( ElevationGrid * elevationGrid ) {
  GEOM_GLSKELRENDERER_GEOM(elevationGrid);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( EulerRotated * eulerRotated ) {
  GEOM_GLSKELRENDERER_GEOM(eulerRotated);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( ExtrudedHull * extrudedHull ) {
  GEOM_GLSKELRENDERER_GEOM(extrudedHull);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( FaceSet * faceSet ) {
  GEOM_GLSKELRENDERER_GEOM(faceSet);

  PolylinePtr _skel = faceSet->getSkeleton();
  if (_skel){
      GEOM_GLSKELRENDERER_DRAW_SKEL(_skel);
  }
  else return false;

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( Frustum * frustum ) {
  GEOM_GLSKELRENDERER_GEOM(frustum);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( Group * group ) {
  GEOM_ASSERT(group);

  PolylinePtr _skel = group->getSkeleton();
  if (_skel){
      GEOM_GLSKELRENDERER_DRAW_SKEL(_skel);
  }
  else return false;

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( IFS * ifs ) {
  GEOM_GLSKELRENDERER_GEOM(ifs);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process(Extrusion * extrusion ) {
  GEOM_GLSKELRENDERER_GEOM(extrusion);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( NurbsCurve * nurbsCurve ) {
  GEOM_GLSKELRENDERER_GEOM(nurbsCurve);
}

/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( NurbsPatch * nurbsPatch ) {
  GEOM_GLSKELRENDERER_GEOM(nurbsPatch);
}

/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( Oriented * oriented ) {
  GEOM_GLSKELRENDERER_GEOM(oriented);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( Paraboloid * paraboloid ) {
  GEOM_GLSKELRENDERER_GEOM(paraboloid);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( PointSet * pointSet ) {
  GEOM_GLSKELRENDERER_GEOM(pointSet);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( PGL(Polyline) * polyline ) {
  GEOM_ASSERT(polyline);

  GEOM_GLSKELRENDERER_DRAW_SKEL(polyline);

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( QuadSet * quadSet ) {
  GEOM_ASSERT(quadSet);

  PolylinePtr _skel = quadSet->getSkeleton();
  if (_skel){
      GEOM_GLSKELRENDERER_DRAW_SKEL(_skel);
  }
  else return false;

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( Revolution * revolution ) {
  GEOM_GLSKELRENDERER_GEOM(revolution);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( Swung * swung )
{
  GEOM_GLSKELRENDERER_GEOM(swung);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( Scaled * scaled ) {
  GEOM_GLSKELRENDERER_GEOM(scaled);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( Sphere * sphere ) {
  GEOM_GLSKELRENDERER_GEOM(sphere);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( Tapered * tapered ) {
  GEOM_GLSKELRENDERER_GEOM(tapered);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( Translated * translated ) {
  GEOM_GLSKELRENDERER_GEOM(translated);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( TriangleSet * triangleSet ) {
  GEOM_ASSERT(triangleSet);

  PolylinePtr _skel = triangleSet->getSkeleton();
  if (_skel){
      GEOM_GLSKELRENDERER_DRAW_SKEL(_skel);
  }
  else return false;

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */



bool GLSkelRenderer::process( BezierCurve2D * bezierCurve ) {
  GEOM_GLSKELRENDERER_GEOM(bezierCurve);
}


/* ----------------------------------------------------------------------- */



bool GLSkelRenderer::process( Disc * disc ) {
  GEOM_GLSKELRENDERER_GEOM(disc);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( NurbsCurve2D * nurbsCurve ) {
  GEOM_GLSKELRENDERER_GEOM(nurbsCurve);
}

/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( PointSet2D * pointSet ) {
  GEOM_GLSKELRENDERER_GEOM(pointSet);
}


/* ----------------------------------------------------------------------- */


bool GLSkelRenderer::process( Polyline2D * polyline ) {
  GEOM_ASSERT(polyline);

  glBegin(GL_LINE_STRIP);
  const Point2ArrayPtr& points = polyline->getPointList();
  for (Point2Array::const_iterator _i = points->getBegin();
       _i != points->getEnd();
       _i++){
     glGeomVertex(_i->x(),_i->y(),0);
  }
  glEnd();

  GEOM_ASSERT(glGetError() == GL_NO_ERROR);
  return true;
}


/* ----------------------------------------------------------------------- */

bool GLSkelRenderer::process( Text * text ) {
  GEOM_ASSERT(text);
  return true;
}

/* ----------------------------------------------------------------------- */

bool GLSkelRenderer::process( Font * font ) {
  GEOM_ASSERT(font);
  return true;
}

/* ----------------------------------------------------------------------- */
