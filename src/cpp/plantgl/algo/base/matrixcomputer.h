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




/*! \file actn_matrixcomputer.h
    \brief Definition of the action class MatrixComputer.
*/


#ifndef __actn_matrixcomputer_h__
#define __actn_matrixcomputer_h__

/* ----------------------------------------------------------------------- */

#include "../algo_config.h"
#include <plantgl/scenegraph/core/action.h>
#include <plantgl/math/util_matrix.h>
#include <stack>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Geometry;
class MatrixTransformed;

/* ----------------------------------------------------------------------- */


class MatrixStack {
public:
  typedef std::stack<Matrix4> Matrix4StackInternalType;
  MatrixStack();
  ~MatrixStack();

  void push() {
        __stack.push(__matrix);
  }

  void pop() {
      if(__stack.empty()){
          __matrix = TOOLS(Matrix4::IDENTITY);
      }
      else {
        __matrix = __stack.top();
        __stack.pop();
      }
  }

  /// Returns the resulting matrix when applying \e self for the last time.
  const Matrix4& getMatrix() const {
      return __matrix;
  }

  /// Returns the resulting matrix when applying \e self for the last time.
  Matrix4& getMatrix() {
      return __matrix;
  }

  inline void translate(const Vector3& t){
      transform(Matrix4::translation(t));
  }

  inline void scale(const Vector3& s){
      transform(Matrix3::scaling(s));
  }

  inline void axisRotation(const Vector3& axe,const real_t angle){
      transform(Matrix3::axisRotation(axe,angle));
  }

  inline void eulerRotationZYX(const real_t azimuth, const real_t elevation, const real_t roll){
      transform(Matrix3::eulerRotationZYX(Vector3(azimuth,elevation,roll)));
  }

  inline void transform(const Matrix4& m){
      __matrix *= m;
  }

  inline void transform(const Matrix3& m){
      __matrix *= Matrix4(m);
  }

  inline void identity(){
      __matrix = Matrix4();
  }


  void getTransformation(Vector3& scale,
                         Vector3& angle,
                         Vector3& translate) const;

  void clear();

protected:

  /// The resulting matrix.
  Matrix4 __matrix;

  Matrix4StackInternalType __stack;
};

/**
   \class MatrixComputer
   \brief An action to compute the matrix4 representing the affine transformations in a branch of the scene.
*/

class ALGO_API MatrixComputer : public Action
{

public:

  /// Constructs a MatrixComputer.
  MatrixComputer( );

  /// Destructor
  virtual ~MatrixComputer( ) {
  }

  /// Clears \e self.
  void clear( );

  /// @name Material
  //@{

  virtual bool process( Material * material ){
      return false;
  }

  virtual bool process( MonoSpectral * monoSpectral ){
      return false;
  }

  virtual bool process( MultiSpectral * multiSpectral ){
      return false;
  }

  virtual bool process( ImageTexture * texture ){
      return false;
  }

  virtual bool process( Texture2D * texture ){
      return false;
  }

  virtual bool process( Texture2DTransformation * texturetransformation ){
      return false;
  }

  //@}


  /// @name Geom3D
  //@{

  virtual bool process( AmapSymbol * amapSymbol );

  virtual bool process( AsymmetricHull * asymmetricHull );

  virtual bool process( BezierCurve * bezierCurve );

  virtual bool process( BezierPatch * bezierPatch );

  virtual bool process( Box * box );

  virtual bool process( Cone * cone );

  virtual bool process( Cylinder * cylinder );

  virtual bool process( ElevationGrid * elevationGrid );

  virtual bool process( ExtrudedHull * extrudedHull );

  virtual bool process( FaceSet * faceSet );

  virtual bool process( Frustum * frustum );

  virtual bool process( Extrusion * extrusion );

  virtual bool process( Group * group );

  virtual bool process( NurbsCurve * nurbsCurve );

  virtual bool process( NurbsPatch * nurbsPatch );

  virtual bool process( Paraboloid * paraboloid );

  virtual bool process( PointSet * pointSet );

  virtual bool process( Polyline * polyline );

  virtual bool process( QuadSet * quadSet );

  virtual bool process( Revolution * revolution );

  virtual bool process( Swung * swung );

  virtual bool process( Sphere * sphere );

  virtual bool process( Tapered * tapered );

  virtual bool process( TriangleSet * triangleSet );

  bool process( AxisRotated * axisRotated );

  bool process( EulerRotated * eulerRotated );

  bool process( IFS * ifs );

  bool process( Oriented * oriented );

  bool process( Scaled * scaled );

  bool process( ScreenProjected * screenprojected );

  bool process( Translated * translated );

  //@}

  /// @name Geom2D
  //@{
  virtual bool process( BezierCurve2D * bezierCurve );

  virtual bool process( Disc * disc );

  virtual bool process( NurbsCurve2D * nurbsCurve );

  virtual bool process( PointSet2D * pointSet );

  virtual bool process( Polyline2D * polyline );


  //@}

  virtual bool process( Text * text );

  virtual bool process( Font * font );


  const Matrix4& getMatrix() const { return __matrix.getMatrix(); }

protected:

  MatrixStack __matrix;

private:

  bool default_process( Geometry* object )
    {
    GEOM_ASSERT(object);
    __matrix.identity();
    return true;
    }

  bool transfo_process( MatrixTransformed* object );

};


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ------------------------------------------------------------------------- */

// __actn_matrixcomputer_h__
#endif
