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





#include "glctrlptrenderer.h"
#include <plantgl/algo/base/discretizer.h>

#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/transformation/axisrotated.h>
#include <plantgl/scenegraph/transformation/eulerrotated.h>
#include <plantgl/scenegraph/transformation/oriented.h>
#include <plantgl/scenegraph/transformation/scaled.h>
#include <plantgl/scenegraph/transformation/tapered.h>
#include <plantgl/scenegraph/transformation/translated.h>
#include <plantgl/scenegraph/geometry/beziercurve.h>
#include <plantgl/scenegraph/geometry/bezierpatch.h>
#include <plantgl/scenegraph/geometry/elevationgrid.h>
#include <plantgl/scenegraph/geometry/extrudedhull.h>
#include <plantgl/scenegraph/geometry/extrusion.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/geometry/nurbscurve.h>
#include <plantgl/scenegraph/geometry/nurbspatch.h>
#include <plantgl/scenegraph/geometry/revolution.h>
#include <plantgl/scenegraph/geometry/sphere.h>
#include <plantgl/scenegraph/scene/shape.h>

#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/pointmatrix.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


#define GEOM_GLCTRLPOINTRENDERER_DRAW_LINE(Point4A) { \
    glBegin(GL_LINE_STRIP); \
    for (Point4Array::const_iterator _i = Point4A->begin(); \
         _i != Point4A->end(); \
         _i++) \
      glGeomVertex(_i->x(), \
                   _i->y(), \
                   _i->z()); \
    glEnd(); \
  };

#define GEOM_GLCTRLPOINTRENDERER_DRAW_LINE3(Point3A) { \
    glBegin(GL_LINE_STRIP); \
    for (Point3Array::const_iterator _i = Point3A->begin(); \
         _i != Point3A->end(); \
         _i++) \
      glGeomVertex(_i->x(), \
                   _i->y(), \
                   _i->z()); \
    glEnd(); \
  };

#define GEOM_GLCTRLPOINTRENDERER_DRAW_LINE2D(Point3A) { \
    glBegin(GL_LINE_STRIP); \
    for (Point3Array::const_iterator _i = Point3A->begin(); \
         _i != Point3A->end(); \
         _i++) \
      glGeomVertex(_i->x(), \
                   _i->y(), \
                   0); \
    glEnd(); \
  };

#define GEOM_GLCTRLPOINTRENDERER_DRAW_HORLINE2D(Point3A) { \
    glBegin(GL_LINE_STRIP); \
    for (Point3Array::const_iterator _i = Point3A->begin(); \
         _i != Point3A->end(); \
         _i++) \
      glGeomVertex(_i->x(), \
                   _i->y(), \
                   0); \
    glGeomVertex(Point3A->begin()->x(),Point3A->begin()->y(),0); \
    glEnd(); \
  };

#define GEOM_GLCTRLPOINTRENDERER_DRAW_VERTLINE2D(Point3A) { \
    glBegin(GL_LINE_STRIP); \
    for (Point3Array::const_iterator _i = Point3A->begin(); \
         _i != Point3A->end(); \
         _i++) \
      glGeomVertex(_i->x(), \
                   0, \
                   _i->y()); \
    glGeomVertex(Point3A->begin()->x(),0,Point3A->begin()->y()); \
    glEnd(); \
  };

#define GEOM_GLCTRLPOINTRENDERER_DRAW_LINES(Point4Matrix) { \
    for (uint_t _usize = 0 ; _usize < Point4Matrix->getRowNb() ; _usize++){ \
      glBegin(GL_LINE_STRIP); \
      for (uint_t _vsize = 0; \
         _vsize <  Point4Matrix->getRowSize() ; _vsize++){ \
         Vector4 mypoint=Point4Matrix->getAt(_usize,_vsize); \
         glGeomVertex(mypoint.x(), \
                      mypoint.y(), \
                      mypoint.z()); \
         } \
      glEnd(); \
    }; \
    for (uint_t _vsize = 0 ; _vsize < Point4Matrix->getRowSize(); _vsize++){ \
      glBegin(GL_LINE_STRIP); \
      for (uint_t _usize = 0; \
         _usize <  Point4Matrix->getRowNb() ; _usize++){ \
         Vector4 mypoint=Point4Matrix->getAt(_usize,_vsize); \
         glGeomVertex(mypoint.x(), \
                      mypoint.y(), \
                      mypoint.z()); \
         } \
      glEnd(); \
    }; \
  };



AppearancePtr GLCtrlPointRenderer::DEFAULT_APPEARANCE(new Material(Color3::RED));

/* ----------------------------------------------------------------------- */


GLCtrlPointRenderer::GLCtrlPointRenderer( Discretizer& discretizer ) :
  GLRenderer(discretizer) {
}

bool GLCtrlPointRenderer::beginProcess(){
  DEFAULT_APPEARANCE->apply(*this);
  return GLRenderer::beginProcess();
}

/* ----------------------------------------------------------------------- */
bool GLCtrlPointRenderer::processAppereance(Shape * Shape){
    GEOM_ASSERT(Shape);
    return true;
}


bool GLCtrlPointRenderer::process( BezierCurve * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);

  GEOM_GLCTRLPOINTRENDERER_DRAW_LINE(bezierCurve->getCtrlPointList());

  return true;
}

/* ----------------------------------------------------------------------- */


bool GLCtrlPointRenderer::process( BezierPatch * bezierPatch ) {
  GEOM_ASSERT(bezierPatch);

  GEOM_GLCTRLPOINTRENDERER_DRAW_LINES(bezierPatch->getCtrlPointMatrix());

  return true;
}


/* ----------------------------------------------------------------------- */


bool GLCtrlPointRenderer::process( ExtrudedHull * extrudedHull ) {
  GEOM_ASSERT(extrudedHull);

  if(extrudedHull->getHorizontal()->apply(__discretizer)){
    PolylinePtr p = dynamic_pointer_cast<Polyline>(__discretizer.getDiscretization());
    if(p){
      GEOM_GLCTRLPOINTRENDERER_DRAW_HORLINE2D(p->getPointList());
    }
  }

  if(extrudedHull->getVertical()->apply(__discretizer)){
    PolylinePtr p = dynamic_pointer_cast<Polyline>(__discretizer.getDiscretization());
    if(p){
      GEOM_GLCTRLPOINTRENDERER_DRAW_VERTLINE2D(p->getPointList());
    }
  }


  return true;
}



/* ----------------------------------------------------------------------- */


bool GLCtrlPointRenderer::process( Extrusion * extrusion ) {
  GEOM_ASSERT(extrusion);

  extrusion->getAxis()->apply(*this);

  return true;
}



/* ----------------------------------------------------------------------- */


bool GLCtrlPointRenderer::process( NurbsCurve * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);

  GEOM_GLCTRLPOINTRENDERER_DRAW_LINE(nurbsCurve->getCtrlPointList());

  return true;
}

/* ----------------------------------------------------------------------- */


bool GLCtrlPointRenderer::process( NurbsPatch * nurbsPatch ) {
  GEOM_ASSERT(nurbsPatch);

  GEOM_GLCTRLPOINTRENDERER_DRAW_LINES(nurbsPatch->getCtrlPointMatrix());

  return true;
}


/* ----------------------------------------------------------------------- */


bool GLCtrlPointRenderer::process( Tapered * tapered ) {
  GEOM_ASSERT(tapered);
  BezierCurvePtr _curve = dynamic_pointer_cast<BezierCurve>(tapered->getPrimitive());
  if(_curve){
      Transformation3DPtr _taper=tapered->getTransformation();
      Point4ArrayPtr mypoints =  _taper->transform(_curve->getCtrlPoints());
      GEOM_GLCTRLPOINTRENDERER_DRAW_LINE(mypoints);
  }
  else{
      BezierPatchPtr _patch = dynamic_pointer_cast<BezierPatch>(tapered->getPrimitive());
      if(_patch){
          Transformation3DPtr _taper=tapered->getTransformation();
          Point4MatrixPtr mypoints =  _taper->transform(_patch->getCtrlPoints());
          GEOM_GLCTRLPOINTRENDERER_DRAW_LINES(mypoints);
      }
      else {
        ExtrudedHullPtr extrudedHull = dynamic_pointer_cast<ExtrudedHull>(tapered->getPrimitive());
        if(extrudedHull){
          if(extrudedHull->getHorizontal()->apply(__discretizer)){
            PolylinePtr p  = dynamic_pointer_cast<Polyline>(__discretizer.getDiscretization());
            if(p){
              GEOM_GLCTRLPOINTRENDERER_DRAW_HORLINE2D(p->getPointList());
            }
          }

          if(extrudedHull->getVertical()->apply(__discretizer)){
            PolylinePtr p = dynamic_pointer_cast<Polyline>(__discretizer.getDiscretization());
            Point3ArrayPtr points(new Point3Array(*(p->getPointList())));
            points->push_back(points->getAt(0));
            for(Point3Array::iterator _it = points->begin();
                _it != points->end();
                _it++){ _it->z() = _it->y();_it->y() = 0;}
            Transformation3DPtr _taper=tapered->getTransformation();
            _taper->transform(points);
            if(p){
              GEOM_GLCTRLPOINTRENDERER_DRAW_LINE3(points);
            }
          }
        }
        else return false;
      }
  }

  return true;
}


/* ----------------------------------------------------------------------- */


bool GLCtrlPointRenderer::process( BezierCurve2D * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);

  GEOM_GLCTRLPOINTRENDERER_DRAW_LINE2D(bezierCurve->getCtrlPointList());

  return true;
}

/* ----------------------------------------------------------------------- */


bool GLCtrlPointRenderer::process( NurbsCurve2D * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);

  GEOM_GLCTRLPOINTRENDERER_DRAW_LINE2D(nurbsCurve->getCtrlPointList());

  return true;
}

/* ----------------------------------------------------------------------- */
