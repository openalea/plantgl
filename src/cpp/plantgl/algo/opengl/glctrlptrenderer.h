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

