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

/*! \file actn_glctrlptrenderer.h
    \brief Definition of the action class GLCtrlPointRenderer.
*/



#ifndef __actn_glctrlpointrenderer_h__
#define __actn_glctrlpointrenderer_h__


#include "glrenderer.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class GLCtrlPointRenderer
   \brief An action which display the Control Points of \e Geometry objects.
*/


class ALGO_API GLCtrlPointRenderer : public GLRenderer
{


public:

  static AppearancePtr DEFAULT_APPEARANCE;

  /// Constructs a GLCtrlPointRenderer.
  GLCtrlPointRenderer( Discretizer& discretizer );

  /// Destructor
  virtual ~GLCtrlPointRenderer( ) {
  };

  virtual bool beginProcess();

  virtual bool processAppereance(Shape * Shape);

  /// @name Geom3D
  //@{

  virtual bool process( AmapSymbol * ) {
    return true;
  };

  virtual bool process( AsymmetricHull * ) {
    return true;
  };

  virtual bool process( BezierCurve * bezierCurve );

  virtual bool process( BezierPatch * bezierPatch );

  virtual bool process( Box * ) {
    return true;
  };

  virtual bool process( Cone * ) {
    return true;
  };

  virtual bool process( Cylinder * ){
    return true;
  };

  virtual bool process( ElevationGrid * ){
    return true;
  };


  virtual bool process( ExtrudedHull * );

  virtual bool process( FaceSet * ){
    return true;
  };

  virtual bool process( Frustum * ){
    return true;
  };

  virtual bool process( Extrusion * extrusion );

  virtual bool process( NurbsCurve * nurbsCurve );

  virtual bool process( NurbsPatch * nurbsPatch );

  virtual bool process( Paraboloid * ){
    return true;
  };

    virtual bool process( PointSet * ){
    return true;
  };

  virtual bool process( Polyline * ){
    return true;
  };

  virtual bool process( QuadSet * ){
    return true;
  };

  virtual bool process( Revolution * ){
    return true;
  };

  virtual bool process( Swung *  )
  {
    return true;
  }

  virtual bool process( Sphere * ){
    return true;
  };

  virtual bool process( Tapered * tapered );

  virtual bool process( TriangleSet * ){
    return true;
  };

  //@}
  /// @name Geom2D
  //@{


  virtual bool process( BezierCurve2D * bezierCurve );

  virtual bool process( Disc * ){
    return true;
  };

  virtual bool process( NurbsCurve2D * nurbsCurve );

  virtual bool process( PointSet2D * ){
    return true;
  };

  virtual bool process( Polyline2D * ){
    return true;
  };

  //@}

  virtual bool process( Text * text ){
    return true;
  };

  virtual bool process( Font * font ){
    return true;
  };

};


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ------------------------------------------------------------------------- */

// __actn_glctrlpointrenderer_h__
#endif

