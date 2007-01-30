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



/*! \file actn_matrixcomputer.h
    \brief Definition of the action class MatrixComputer.
*/


#ifndef __actn_matrixcomputer_h__
#define __actn_matrixcomputer_h__

/* ----------------------------------------------------------------------- */

#include "../algo_config.h"
#include <scenegraph/core/action.h>
#include <math/util_matrix.h>
#include <stack>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Geometry;
class MatrixTransformed;

/* ----------------------------------------------------------------------- */

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


  void pushMatrix() {
		__stack.push(__matrix);
  }

  void popMatrix() {
	  if(__stack.empty()){
		  __matrix = TOOLS(Matrix4::IDENTITY);
	  }
	  else {
	    __matrix = __stack.top();
		__stack.pop();
	  }
  }

  /// Returns the resulting matrix when applying \e self for the last time.
  const TOOLS(Matrix4)& getMatrix() const {
	  return __matrix;
  }

  /// Returns the resulting matrix when applying \e self for the last time.
  TOOLS(Matrix4)& getMatrix() {
	  return __matrix;
  }

  inline void translate(const TOOLS(Vector3)& t){
	  transform(TOOLS(Matrix4)::translation(t));
  }

  inline void scale(const TOOLS(Vector3)& s){
	  transform(TOOLS(Matrix3)::scaling(s));
  }

  inline void axisRotate(const TOOLS(Vector3)& axe,const real_t angle){
	  transform(TOOLS(Matrix3)::axisRotation(axe,angle));
  }

  inline void transform(const TOOLS(Matrix4)& m){
	  __matrix *= m;
  }

  inline void transform(const TOOLS(Matrix3)& m){
	  __matrix *= TOOLS(Matrix4)(m);
  }

  inline void identity(){
	  __matrix = TOOLS(Matrix4)();
  }


  void getTransformation(TOOLS(Vector3)& scale,
                         TOOLS(Vector3)& angle,
                         TOOLS(Vector3)& translate) const;
protected:

  /// The resulting matrix.
  TOOLS(Matrix4) __matrix;

  std::stack<TOOLS(Matrix4)> __stack;

private:


  bool default_process( Geometry* object )
    {
    GEOM_ASSERT(object);
    __matrix= TOOLS(Matrix4::IDENTITY);
    return true;
    }

  bool transfo_process( MatrixTransformed* object );

};


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ------------------------------------------------------------------------- */

// __actn_matrixcomputer_h__
#endif

