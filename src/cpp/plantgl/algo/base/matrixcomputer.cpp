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





#include "matrixcomputer.h"
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

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
  ITPtr transfos = dynamic_pointer_cast<IT>( ifs->getTransformation() );
  GEOM_ASSERT(transfos);
  const Matrix4ArrayPtr& matrixList= transfos->getAllTransfo();
  GEOM_ASSERT(matrixList);
  Matrix4Array::const_iterator matrix= matrixList->begin();

  while( matrix != matrixList->end() )
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


bool MatrixComputer::process( ScreenProjected * scp) {
  return default_process(scp);
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
    dynamic_pointer_cast<Matrix4Transformation>(object->getTransformation());

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
// __matrix.getTransformation(scale,angle,translate);
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
