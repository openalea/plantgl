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



/*! \file actn_printer.h
    \brief Definition of the action class Printer.
*/


#ifndef __actn_printer_h__
#define __actn_printer_h__

#include "codec_config.h"
#include <tool/util_types.h>
#include <tool/rcobject.h>
#include <tool/util_hashset.h>
#include <scenegraph/core/action.h>

#include <string>
#include <iostream>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

class SceneObject;
typedef RCPtr<SceneObject> SceneObjectPtr;

/* ----------------------------------------------------------------------- */


/**
   \class Printer
   \brief An action which prints objects of type of GEOM object to the
   specified output streams.
*/


class CODEC_API Printer : public Action
{

public:

  /** Constructs a Printer on cout. */
  Printer( );

  /** Constructs a Printer with the output streams \e stream . */
  Printer( std::ostream& stream );

  /** Constructs a Printer with the output streams \e shapeStream, \e geomStream and
      \e matStream. */
  Printer( std::ostream& shapeStream , std::ostream& geomStream , std::ostream& matStream  );

  /// Destructor
  virtual ~Printer( );

  /// Clears \e self.
  void clear( );

  /// Add indentation.
  void addIndent(uint32_t i);

  /// test if an object is already printed.
  virtual bool isPrinted(SceneObjectPtr);

  /// Add comment in the header of the output file.
  virtual bool header(const char * comment = NULL);

  /// Add comment in the header of the output file.
  virtual bool header(std::ostream & _ostream,const char * filename = NULL,const char * comment = NULL);


  /// @name Shape
  //@{
  virtual bool process(Shape * Shape);

  virtual bool process(Inline * geomInline);

  //@}

  /// @name Material
  //@{

  virtual bool process( Material * material );

  virtual bool process( MonoSpectral * monoSpectral );

  virtual bool process( MultiSpectral * multiSpectral );

  virtual bool process( ImageTexture * texture );

  //@}


  /// @name Geom3D
  //@{
  virtual bool process( AmapSymbol * amapSymbol );

  virtual bool process( AsymmetricHull * asymmetricHull );

  virtual bool process( AxisRotated * axisRotated );

  virtual bool process( BezierCurve * bezierCurve );

  virtual bool process( BezierPatch * bezierPatch );

  virtual bool process( Box * box );

  virtual bool process( Cone * cone );

  virtual bool process( Cylinder * cylinder );

  virtual bool process( ElevationGrid * elevationGrid );

  virtual bool process( EulerRotated * eulerRotated );

  virtual bool process( ExtrudedHull * extrudedHull );

  virtual bool process( FaceSet * faceSet );

  virtual bool process( Frustum * frustum );

  virtual bool process( Extrusion * extrusion );

  virtual bool process( Group * group );

  virtual bool process( IFS * ifs );

  virtual bool process( NurbsCurve * nurbsCurve );

  virtual bool process( NurbsPatch * nurbsPatch );

  virtual bool process( Oriented * oriented );

  virtual bool process( Paraboloid * paraboloid );

  virtual bool process( PointSet * pointSet );

  virtual bool process( Polyline * polyline );

  virtual bool process( QuadSet * quadSet );

  virtual bool process( Revolution * revolution );

  virtual bool process( Swung * swung );

  virtual bool process( Scaled * scaled );

  virtual bool process( Sphere * sphere );

  virtual bool process( Tapered * tapered );

  virtual bool process( Translated * translated );

  virtual bool process( TriangleSet * triangleSet );

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

protected:

  /// The output stream for the objects of type of Shape.
  std::ostream& __shapeStream;

  /// The output stream for the objects of type of Geometry.
  std::ostream& __geomStream;

  /// The output stream for the objects of type of Material.
  std::ostream& __matStream;

  /// The cache where to store the already printed objects
  hash_set_uint32 __cache;

  /// The ident used to perform a pretty print.
  std::string __indent;

};


/* ------------------------------------------------------------------------- */

// __actn_printer_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif


