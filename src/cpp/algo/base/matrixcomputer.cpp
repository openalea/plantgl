#define CPL
/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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




#include "matrixcomputer.h"
#include <pgl_geometry.h>
#include <pgl_transformation.h>
#include <math/util_math.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


MatrixComputer::MatrixComputer( ) :
  Action(),
  __matrix(Matrix4::IDENTITY) {
}


void MatrixComputer::clear( ) {
  __matrix = Matrix4::IDENTITY;
}

/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( AmapSymbol * amapSymbol ) {
  return default_process(amapSymbol);
}


/* ----------------------------------------------------------------------- */


/*bool MatrixComputer::process( Appearance * appearance ) {
  GEOM_ASSERT(appearance);

   __matrix = Matrix4::IDENTITY;

  return false;
}*/


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( AsymmetricHull * asymmetricHull ) {
  return default_process(asymmetricHull);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( AxisRotated * axisRotated ) {
  return transfo_process(axisRotated);
}



/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( BezierCurve * bezierCurve ) {
  return default_process(bezierCurve);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( BezierPatch * bezierPatch ) {
  return default_process(bezierPatch);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( Box * box ) {
  return default_process(box);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( Cone * cone ) {
  return default_process(cone);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( Cylinder * cylinder ) {
  return default_process(cylinder);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( ElevationGrid * elevationGrid ) {
  return default_process(elevationGrid);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( EulerRotated * eulerRotated ) {
  return transfo_process(eulerRotated);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( ExtrudedHull * extrudedHull ) {
  return default_process(extrudedHull);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( FaceSet * faceSet ) {
  return default_process(faceSet);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( Frustum * frustum ) {
  return default_process(frustum);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process(Extrusion * extrusion) {
  return default_process(extrusion);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( Group * group ) {
  return default_process(group);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( IFS * ifs ) {
  ITPtr transfos = ITPtr::Cast( ifs->getTransformation() );
  GEOM_ASSERT(transfos);
  const Matrix4ArrayPtr& matrixList= transfos->getAllTransfo();
  GEOM_ASSERT(matrixList);
  Matrix4Array::const_iterator matrix= matrixList->getBegin();

  while( matrix != matrixList->getEnd() )
    {
	  pushMatrix();
	  transform(*matrix);
	  ifs->getGeometry()->apply(*this);
	  popMatrix();
	  matrix++;
    }
	return true;
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( NurbsCurve * nurbsCurve ) {
  return default_process(nurbsCurve);
}

/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( NurbsPatch * nurbsPatch ) {
  return default_process(nurbsPatch);
}

/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( Oriented * oriented ) {
  return transfo_process(oriented);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( Paraboloid * paraboloid ) {
  return default_process(paraboloid);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( PointSet * pointSet ) {
  return default_process(pointSet);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( Polyline * polyline ) {
  return default_process(polyline);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( Revolution * revolution ) {
  return default_process(revolution);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( Swung * swung )
{
  return default_process(swung);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( QuadSet * quadSet ) {
  return default_process(quadSet);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( Scaled * scaled ) {
  return transfo_process( scaled );
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( Sphere * sphere ) {
  return default_process(sphere);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( Tapered * tapered ) {
  return default_process(tapered);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( Translated * translated ) {
  return transfo_process( translated );
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( TriangleSet * triangleSet ) {
  return default_process(triangleSet);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( BezierCurve2D * bezierCurve ) {
  return default_process(bezierCurve);
}


/* ----------------------------------------------------------------------- */



bool MatrixComputer::process( Disc * disc ) {
  return default_process(disc);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( NurbsCurve2D * nurbsCurve ) {
  return default_process(nurbsCurve);
}

/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( PointSet2D * pointSet ) {
  return default_process(pointSet);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( Polyline2D * polyline ) {
  return default_process(polyline);
}

/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( Text * text ) {
  return default_process(text);
}


/* ----------------------------------------------------------------------- */


bool MatrixComputer::process( Font * font ) {
  return false;
}

/* ----------------------------------------------------------------------- */

bool MatrixComputer::transfo_process( MatrixTransformed* object )
{
  GEOM_ASSERT(object);


  Matrix4TransformationPtr _transform =
    Matrix4TransformationPtr::Cast(object->getTransformation());

  GEOM_ASSERT(_transform);

  pushMatrix();
  transform(_transform->getMatrix());

  GeometryPtr geom= object->getGeometry();
  geom->apply(*this); // recursive call

  popMatrix();

  return true;
}


/* ----------------------------------------------------------------------- */

void 
MatrixComputer::getTransformation(Vector3& scale,
                         Vector3& angle,
                         Vector3& translate) const{
//	__matrix.getTransformation(scale,angle,translate);
  translate.x()= __matrix(0,3);
  translate.y()= __matrix(1,3);
  translate.z()= __matrix(2,3);

  Vector3 a(__matrix(0,0),__matrix(1,0),__matrix(2,0));
  Vector3 b(__matrix(0,1),__matrix(1,1),__matrix(2,1));
  Vector3 c(__matrix(0,2),__matrix(1,2),__matrix(2,2));
  scale.x()= a.normalize();
  scale.y()= b.normalize();
  scale.z()= c.normalize();

  real_t thetaX,thetaY,thetaZ;
  thetaY = asin(-a.z());
  if(thetaY < GEOM_HALF_PI-GEOM_EPSILON){
	if(thetaY > -GEOM_HALF_PI+GEOM_EPSILON){
		thetaZ = atan2(a.y(),a.x());
		thetaX = atan2(b.z(),c.z());
	}
	else {
		// Not a unique solution
		thetaZ = -atan2(-b.x(),c.x());
		thetaX = 0;
	}
  }
  else {
	  // Not a unique solution
	  thetaZ = atan2(-b.x(),c.x());
	  thetaX = 0;
  }
  angle.x() = thetaZ;
  angle.y() = thetaY;
  angle.z() = thetaX;

}

/* ----------------------------------------------------------------------- */
