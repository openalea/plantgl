/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2012-2015 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
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




#include <plantgl/math/util_matrix.h>

#include "planeclipping.h"
#include "tesselator.h"
#include "intersection.h"

#include <plantgl/pgl_scene.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>

#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/scenegraph/container/geometryarray2.h>

#include <plantgl/math/util_math.h>

TOOLS_USING_NAMESPACE
PGL_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */
Point3ArrayPtr PGL(plane_segment_clip)(const Plane3& plane, 
                                  const TOOLS(Vector3)& p1, 
                                  const TOOLS(Vector3)& p2)
{
    real_t d1 = dot(plane.getNormal(),p1);
    real_t d2 = dot(plane.getNormal(),p2);
    if (d1 <  plane.getD() && d2 <  plane.getD()) return Point3ArrayPtr();
    else if (d1 <  plane.getD() || d2 <  plane.getD()) {
        Point3ArrayPtr result(new Point3Array(2));
        uint32_t pid = 0;
        if (d1 >  plane.getD()) result->setAt(pid++,p1);
        result->setAt(pid++,_plane_segment_intersection(plane, d1, p1, p2));
        if (d2 >  plane.getD()) result->setAt(pid++,p2);
        return result;
    }
    else {
        Point3ArrayPtr result(new Point3Array(2));
        result->setAt(0,p1);
        result->setAt(1,p2);
        return result;
    }
}

Point3ArrayPtr PGL(plane_triangle_clip)(const Plane3& plane, 
                                   const TOOLS(Vector3)& p1, 
                                   const TOOLS(Vector3)& p2, 
                                   const TOOLS(Vector3)& p3)
{
    real_t d1 = dot(plane.getNormal(),p1);
    real_t d2 = dot(plane.getNormal(),p2);
    real_t d3 = dot(plane.getNormal(),p3 );
    bool side1 = (d1 > plane.getD());
    bool side2 = (d2 > plane.getD());
    bool side3 = (d3 > plane.getD());
    if ( side1 == side2 && side1 == side3) {
        if (side1) { 
            Point3ArrayPtr result(new Point3Array(3));
            result->setAt(0,p1); result->setAt(1,p2); result->setAt(2,p3);
            return result;
        }
        else return Point3ArrayPtr();
    }
    else {
        Point3ArrayPtr result(new Point3Array());
        if (side1) {
            result->push_back(p1);
            if (side2) {
                result->push_back(p2);
                assert (!side3);
                result->push_back(_plane_segment_intersection(plane,d2,p2,p3));
                result->push_back(_plane_segment_intersection(plane,d1,p1,p3));
            }
            else {
                result->push_back(_plane_segment_intersection(plane,d1,p1,p2));
                if (side3){
                    result->push_back(_plane_segment_intersection(plane,d3,p3,p2));
                    result->push_back(p3);
                }
                else {
                    result->push_back(_plane_segment_intersection(plane,d1,p1,p3));
                }
            }

        }
        else {
            if(side2) {
                result->push_back(_plane_segment_intersection(plane,d2,p2,p1));
                result->push_back(p2);
                if (side3) {
                    result->push_back(p3);
                    result->push_back(_plane_segment_intersection(plane,d3,p3,p1));
                }
                else {
                    result->push_back(_plane_segment_intersection(plane,d2,p2,p3));
                }
            }
            else {
                assert(side3);
                result->push_back(_plane_segment_intersection(plane,d3,p3,p1));
                result->push_back(_plane_segment_intersection(plane,d3,p3,p2));
                result->push_back(p3);
            }
        }
        return result;
    }

}

Index PGL::plane_pointset_clip(const Plane3& plane, 
                                   const Point3ArrayPtr points, 
                                   const Index& group)
{
   size_t nbpoints = group.size();
   Index result;
   if(nbpoints != 0) {        
        for(Index::const_iterator itg = group.begin(); itg != group.end(); ++itg){
            if (plane.getDistance(points->getAt(*itg)) >= 0) result.push_back(*itg);
        }
        
    }
    else {
        size_t pid = 0;
        for(Point3Array::const_iterator itp = points->begin(); itp != points->end(); ++itp, ++pid)
            if (plane.getDistance(*itp) >= 0) result.push_back(pid);
   }
   return result;

}

#ifdef false

/* ----------------------------------------------------------------------- */
#define GEOM_TRANSLATE(geom) \
   GEOM_ASSERT(geom); \
    if(geom->apply(__discretizer)){ \
        ExplicitModelPtr _discret = __discretizer.getDiscretization(); \
        if (_discret) return _discret->apply(*this); \
        else { \
            __result = FaceSetPtr(); \
            return false; \
        }; \
    } \
    else { \
        __result = FaceSetPtr(); \
        return false; \
    }; \

/* ----------------------------------------------------------------------- */

PlaneClipper::PlaneClipper(const Plane3& plane, Tesselator& _discretizer) :
    Action(),
    __plane(plane),
    __discretizer(_discretizer){
    }


PlaneClipper::~PlaneClipper() {
}

/* ----------------------------------------------------------------------- */


bool PlaneClipper::process(Shape * Shape) {
  GEOM_ASSERT(Shape);
  return Shape->geometry->apply(*this);
}


bool PlaneClipper::process( Inline * geominline )
{
    return false;
}

/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Material * material ) {
  GEOM_ASSERT(material);
  // nothing to do
  __result = FaceSetPtr();
  return false;
}


/* ----------------------------------------------------------------------- */

bool PlaneClipper::process( ImageTexture * texture ) {
  GEOM_ASSERT(texture);
  // nothing to do
  __result = FaceSetPtr();
  return false;
}


/* ----------------------------------------------------------------------- */

bool PlaneClipper::process( Texture2D * texture ) {
  GEOM_ASSERT(texture);
  // nothing to do
  __result = FaceSetPtr();
  return false;
}


/* ----------------------------------------------------------------------- */

bool PlaneClipper::process( Texture2DTransformation * texturetransformation ) {
  GEOM_ASSERT(texturetransformation);
  // nothing to do
  __result = FaceSetPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( MonoSpectral * monoSpectral ) {
  GEOM_ASSERT(monoSpectral);
  // nothing to do
  __result = FaceSetPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( MultiSpectral * multiSpectral ) {
  GEOM_ASSERT(multiSpectral);
  // nothing to do
  __result = FaceSetPtr();
  return false;
}



/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( AmapSymbol * amapSymbol ) {
    GEOM_TRANSLATE(amapSymbol);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( AsymmetricHull * asymmetricHull ) {
    GEOM_TRANSLATE(asymmetricHull);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( AxisRotated * axisRotated ) {
  GEOM_TRANSLATE(axisRotated);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( BezierCurve * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);
  // nothing to do
  __result = FaceSetPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( BezierPatch * bezierPatch ) {
  GEOM_TRANSLATE(bezierPatch);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Box * box ) {
  GEOM_TRANSLATE(box);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Cone * cone ) {
  GEOM_TRANSLATE(cone);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Cylinder * cylinder ) {
  GEOM_TRANSLATE(cylinder);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( ElevationGrid * elevationGrid ) {
  GEOM_TRANSLATE(elevationGrid);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( EulerRotated * eulerRotated ) {
  GEOM_TRANSLATE(eulerRotated);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( ExtrudedHull * extrudedHull ) {
  GEOM_TRANSLATE(extrudedHull);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( FaceSet * faceSet ) {
   GEOM_TRANSLATE(faceSet);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Frustum * frustum ) {
  GEOM_TRANSLATE(frustum);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Extrusion * extrusion   ) {
  GEOM_TRANSLATE(extrusion);
}

/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Group * group ) {
  GEOM_TRANSLATE(group);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( IFS * ifs ) {
  GEOM_TRANSLATE(ifs);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( NurbsCurve * nurbsCurve ) {
    GEOM_ASSERT(nurbsCurve);
    // nothing to do
    __result = FaceSetPtr();
    return false;
}

/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( NurbsPatch * nurbsPatch ) {
  GEOM_TRANSLATE(nurbsPatch);
}

/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Oriented * oriented ) {
  GEOM_TRANSLATE(oriented);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Paraboloid * paraboloid ) {
  GEOM_TRANSLATE(paraboloid);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( PointSet * pointSet ) {
  GEOM_ASSERT(pointSet);
  // nothing to do
  __result = FaceSetPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Polyline * polyline ) {
  GEOM_ASSERT(polyline);
  // nothing to do
  __result = FaceSetPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Revolution * revolution ) {
  GEOM_TRANSLATE(revolution);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Swung * swung )
{
  GEOM_TRANSLATE(swung);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( QuadSet * quadSet ) {
  GEOM_TRANSLATE(quadSet);
}

/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Scaled * scaled ) {
  GEOM_TRANSLATE(scaled);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Sphere * sphere ) {
  GEOM_TRANSLATE(sphere);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Tapered * tapered ) {
  GEOM_TRANSLATE(tapered);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Translated * translated ) {
  GEOM_TRANSLATE(translated);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( TriangleSet * triangleSet ) {
  GEOM_ASSERT(triangleSet);
#ifdef __GNUC__
#warning to be made
#endif
/*  Index3ArrayPtr _ind = triangleSet->getIndexList();
  IndexArrayPtr indices(new IndexArray(_ind->size()));
  IndexArray::iterator _it2 = indices->begin();
  for(Index3Array::const_iterator _it = _ind->begin();_it!=_ind->end();_it++){
      (*_it2) = Index(0);
      _it2->insert(_it2->begin(),_it->begin(),_it->end());_it2++;
  }
  FaceSetPtr f( new FaceSet( triangleSet->getPointList(), indices, triangleSet->getNormalPerVertex(),
             triangleSet->getCCW(), triangleSet->getSolid(), triangleSet->getSkeleton()));
  return f->apply(*this); */
  return false;
}


/* ------------------------------------------------------------------------- */


bool PlaneClipper::process( BezierCurve2D * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);
  // nothing to do
  __result = FaceSetPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Disc * disc ) {
  GEOM_TRANSLATE(disc);
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( NurbsCurve2D * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);
  // nothing to do
  __result = FaceSetPtr();
  return false;
}

/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( PointSet2D * pointSet ) {
  GEOM_ASSERT(pointSet);
  // nothing to do
  __result = FaceSetPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Polyline2D * polyline ) {
  GEOM_ASSERT(polyline);
  // nothing to do
  __result = FaceSetPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Text * text ) {
  GEOM_ASSERT(text);
  // nothing to do
  __result = FaceSetPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool PlaneClipper::process( Font * font ) {
  GEOM_ASSERT(font);
  // nothing to do
  __result = FaceSetPtr();
  return false;
}


/* ----------------------------------------------------------------------- */

#endif