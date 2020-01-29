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




/*! \file actn_printer.h
    \brief Definition of the action class Printer.
*/


#ifndef __actn_printer_h__
#define __actn_printer_h__

#include "codec_config.h"
#include <plantgl/tool/util_types.h>
#include <plantgl/tool/rcobject.h>
#include <plantgl/tool/util_hashset.h>
#include <plantgl/scenegraph/core/action.h>

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

  /// Flush \e self.
  void flush( );

  /// Clears \e self.
  void clear( );

  /// Add indentation.
  void addIndent(uint_t i);

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

  virtual bool process( Texture2D * texture );

  virtual bool process( ImageTexture * texture );

  virtual bool process( Texture2DTransformation * texturetransfo );

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

  virtual bool process( ScreenProjected * screenprojected );

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
  pgl_hash_set_uint32 __cache;

  /// The ident used to perform a pretty print.
  std::string __indent;

};


/* ------------------------------------------------------------------------- */

// __actn_printer_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif


