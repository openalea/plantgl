/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon & C.godin
 *       (frederic.boudon@cirad.fr) nouguier
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



/*! \file actn_linetreeprinter.h
    \brief Definition of the action class LinetreePrinter.
*/


#ifndef __actn_linetreeprinter_h__
#define __actn_linetreeprinter_h__

#include "actn_matrixcomputer.h"
#include "actn_amaptranslator.h"
#include "appe_material.h"
#include "geom_amapsymbol.h"
#include "Tools/bfstream.h"

#include "Tools/util_hashmap.h"
#include <vector>

/* ----------------------------------------------------------------------- */

GEOM_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class LinetreePrinter
   \brief An action which prints scene objects to the stream \e __stream
   using Linetree file format.

*/

class Geometry;
typedef RCPtr<Geometry> GeometryPtr;


/* ----------------------------------------------------------------------- */


class GEOM_API LinetreePrinter : public MatrixComputer
{

public:

        static std::string PROTECTION;

        static GeometryPtr UNIT_CYLINDER;
        static GeometryPtr UNIT_SOLID_CYLINDER;
        static GeometryPtr UNIT_SPHERE;
        static GeometryPtr UNIT_DISC;
        static GeometryPtr UNIT_BOX;
        static void initUnitShape();

 /** Constructs a VgstarPrinter with the output streams \e vgstarStream and
      the Tesselator \e tesselator. */
  LinetreePrinter( TOOLS(bofstream)& ligstream,
          std::ostream& dtastream,
          const std::string& smbpath,
      AmapTranslator& translator);

  /// Destructor
  virtual ~LinetreePrinter( ) {
  }

  virtual bool endProcess();

  virtual bool header(const std::string& key = NULL);

  void printSymbol();
  void printTransformation();
  void printMaterial(const MaterialPtr& mat);

  void printInf( std::ostream& stream,
                             const TOOLS(Vector3)& _max = TOOLS(Vector3)(1,1,1),
                                 const TOOLS(Vector3)& _min = TOOLS(Vector3)(-1,-1,-1)) const;

  bool printCfg( const std::string& path,
                             const std::string& ligne ,
                                 const std::string& smb ,
                                 std::string& cfg) const;
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

  virtual bool process( GeomPolyline * polyline );

  virtual bool process( QuadSet * quadSet );

  virtual bool process( Revolution * revolution );

  virtual bool process( Swung * swung );

  virtual bool process( Sphere * sphere );

  virtual bool process( Tapered * tapered );

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

protected:

  TOOLS(bofstream)& __ligstream;
  std::ostream& __dtastream;
  std::string __smbpath;

  float __base_radius;
  float __top_radius;
  long __entity_number;

  /// The tesselator used to triangulate objects when needed
  AmapTranslator& __translator;

  /// The name of the current texture is cached using a string.
  MaterialPtr __mat;

  STDEXT::hash_map<uint32_t, std::pair<AmapSymbolPtr,std::pair<TOOLS(Vector3),TOOLS(Vector3)> > > __smbcache;
  STDEXT::hash_map<uint32_t, std::string > __smbfilecache;
  STDEXT::hash_map<uint32_t, STDEXT::hash_map<uint32_t,long> > __cache;
  std::vector<std::pair<long, std::pair<AmapSymbolPtr,MaterialPtr> > > __dta;

  long __smbNumber;

  bool symbolProcess( Geometry* object );
};


/* ------------------------------------------------------------------------- */

GEOM_END_NAMESPACE

/* ----------------------------------------------------------------------- */
// __actn_vgstarprinter_h__
#endif

