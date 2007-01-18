/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
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

/*! \file actn_vrmlprinter.h
    \brief Definition of the action class VrmlPrinter.
*/

#ifndef __actn_vrmlprinter_h__
#define __actn_vrmlprinter_h__


#include "actn_printer.h"
#include "Tools/rcobject.h"
#include "Tools/util_vector.h"
#include "Tools/util_hashmap.h"

/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Discretizer;
class Appearance;
class Color3;
typedef RCPtr<Appearance> AppearancePtr;


/* ----------------------------------------------------------------------- */

/// struct for testing equality of 2 pairs

#ifndef WIN32_STL_EXTENSION

template <class type>
struct eqpair
{
  bool operator()(std::pair<type,type> a, std::pair<type,type> b) const
  {
    return ((a.first == b.first)&&(a.second == b.second));
  }
};

/// struct for hashing a pair
template <class type>
struct hashpair
{
  size_t operator()(std::pair<type,type> a) const
  {
    return H((a.first+a.second));
  }
  STDEXT::hash<uint32_t> H;
};


/// hash set of pairs
typedef STDEXT::hash_set<std::pair< uint32_t, uint32_t>,hashpair<uint32_t>,eqpair<uint32_t> > SCache;

#else

template <class type>
struct less_pair
{
  bool operator()(std::pair<type,type> a, std::pair<type,type> b) const
  {
    return ((a.first < b.first)||(a.second < b.second));
  }
};


template <class type>
struct hash_comp_pair : STDEXT::hash_compare<std::pair<type,type>,less_pair<type> >
{

  size_t operator()(const std::pair<type,type>& a) const
  {
    return H((a.first+a.second));
  }

  bool operator()(std::pair<type,type> a, std::pair<type,type> b) const
  {
    return ((a.first < b.first)||(a.second < b.second));
  }

  STDEXT::hash_compare<type> H;
};

typedef STDEXT::hash_set<std::pair< uint32_t, uint32_t>, hash_comp_pair<uint32_t> > SCache;

#endif


/**
   \class VrmlPrinter
   \brief An action which prints scene objects to the stream \e vrmlStream
   using Vrml97 file format.

*/

class GEOM_API VrmlPrinter : public Printer
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
  virtual bool setCamera(const TOOLS(Vector3)& position,
                                                 const real_t& az,
                                                 const real_t& el,
                                                 const std::string& name);

  /// Set a light to the scene.
  virtual bool setLight(const TOOLS(Vector3)& location,
                                                const Color3& ambient,
                                                const Color3& diffuse,
                                                const real_t& radius = REAL_MAX);

  /// @name GeomShape
  //@{
  virtual bool process(GeomShape * geomShape);

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

  virtual bool process( GeomPolyline * polyline );

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

  virtual bool process( GeomPolyline2D * polyline );

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

GEOM_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

