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


/*! \file actn_vrmlprinter.h
    \brief Definition of the action class VrmlPrinter.
*/

#ifndef __actn_vrmlprinter_h__
#define __actn_vrmlprinter_h__


#include "printer.h"
#include <plantgl/tool/rcobject.h>
#include <plantgl/math/util_vector.h>
#include <plantgl/tool/util_hashset.h>
#include <functional>  // for std::hash

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Discretizer;
class Appearance;
class Color3;
typedef RCPtr<Appearance> AppearancePtr;


/* ----------------------------------------------------------------------- */

/// struct for testing equality of 2 pairs
template <class type>
struct eqpair {
  bool operator()(const std::pair<type,type>& a,
                  const std::pair<type,type>& b) const {
    return (a.first == b.first && a.second == b.second);
  }
};

/// struct for hashing a pair
template <class type>
struct hashpair {
  size_t operator()(const std::pair<type,type>& a) const {
    return std::hash<type>()(a.first + a.second);
  }
};

/// hash set of pairs
typedef pgl_hash_set<std::pair<uint_t,uint_t>, hashpair<uint_t>, eqpair<uint_t>> SCache;



/**
   \class VrmlPrinter
   \brief An action which prints scene objects to the stream \e vrmlStream
   using Vrml97 file format.

*/

class CODEC_API VrmlPrinter : public Printer
{

public:

  /** Constructs a VrmlPrinter with the output streams \e vrmlStream and
      the Discretizer \e discretizer. */
  VrmlPrinter( std::ostream& vrmlStream,
               Discretizer& discretizer );

  /// Destructor
  virtual ~VrmlPrinter( );

  /// header of the VRML File.
  virtual bool header(const char * comment = NULL);

  /// Set the background color.
  virtual bool setBackGround(const Color3& sky);

  /// Set a camera to the scene. All vector must be given in the GEOM basis and will be transformed in VRML basis.
  virtual bool setCamera(const Vector3& position,
                                                 const real_t& az,
                                                 const real_t& el,
                                                 const std::string& name);

  /// Set a light to the scene.
  virtual bool setLight(const Vector3& location,
                                                const Color3& ambient,
                                                const Color3& diffuse,
                                                const real_t& radius = REAL_MAX);

  /// @name Shape
  //@{
  virtual bool process(Shape * Shape);

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

  virtual bool process( ScreenProjected * scp)
  { return false; }

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

  /// The discretizer used to discretize objects when needed.
  Discretizer& __discretizer;

  /// The current apperance.
  AppearancePtr __app;

  /// The shape cache
  SCache __shapecache;

};


/* ------------------------------------------------------------------------- */

// __actn_vrmlprinter_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

