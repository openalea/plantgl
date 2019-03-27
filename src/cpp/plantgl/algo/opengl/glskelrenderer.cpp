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



#include "glskelrenderer.h"

#include <plantgl/algo/base/skelcomputer.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/scenegraph/container/pointarray.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */




#define GEOM_GLSKELRENDERER_DRAW_SKEL(skel) { \
    glBegin(GL_LINE_STRIP); \
    const Point3ArrayPtr& points = skel->getPointList(); \
    for (Point3Array::const_iterator _i = points->begin(); \
         _i != points->end(); \
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
  for (Point2Array::const_iterator _i = points->begin();
       _i != points->end();
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
