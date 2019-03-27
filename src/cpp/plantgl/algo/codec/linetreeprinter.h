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




/*! \file actn_linetreeprinter.h
    \brief Definition of the action class LinetreePrinter.
*/


#ifndef __actn_linetreeprinter_h__
#define __actn_linetreeprinter_h__

#include "codec_config.h"
#include <plantgl/algo/base/matrixcomputer.h>
#include <plantgl/algo/base/amaptranslator.h>
#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/geometry/amapsymbol.h>
#include <plantgl/tool/bfstream.h>

#include <plantgl/tool/util_hashmap.h>
#include <vector>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class LinetreePrinter
   \brief An action which prints scene objects to the stream \e __stream
   using Linetree file format.

*/

class Geometry;
typedef RCPtr<Geometry> GeometryPtr;


/* ----------------------------------------------------------------------- */


class CODEC_API LinetreePrinter : public MatrixComputer
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
  LinetreePrinter( bofstream& ligstream,
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
                             const Vector3& _max = Vector3(1,1,1),
                                 const Vector3& _min = Vector3(-1,-1,-1)) const;

  bool printCfg( const std::string& path,
                             const std::string& ligne ,
                                 const std::string& smb ,
                                 std::string& cfg) const;
  /// @name Shape
  //@{
  virtual bool process(Shape * Shape);

  //@}

  /// @name Material
  //@{

  virtual bool process( Material * material );

  virtual bool process( MonoSpectral * monoSpectral );

  virtual bool process( MultiSpectral * multiSpectral );

  virtual bool process( ImageTexture * texture );

  virtual bool process( Texture2D * texture );

  virtual bool process( Texture2DTransformation * texturetransformation );

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

  virtual bool process( ScreenProjected * scp);

  //@}

  /// @name Geom2D
  //@{
  virtual bool process( BezierCurve2D * bezierCurve );

  virtual bool process( Disc * disc );

  virtual bool process( NurbsCurve2D * nurbsCurve );

  virtual bool process( PointSet2D * pointSet );

  virtual bool process( Polyline2D * polyline );


  //@}

protected:

  bofstream& __ligstream;
  std::ostream& __dtastream;
  std::string __smbpath;

  float __base_radius;
  float __top_radius;
  long __entity_number;

  /// The tesselator used to triangulate objects when needed
  AmapTranslator& __translator;

  /// The name of the current texture is cached using a string.
  MaterialPtr __mat;

  typedef pgl_hash_map<uint_t, std::pair<AmapSymbolPtr,std::pair<Vector3,Vector3> > > SmbMap;
  typedef pgl_hash_map<uint_t, std::string > SmbFileMap;
  typedef pgl_hash_map<uint_t,long> CacheUnit;
  typedef pgl_hash_map<uint_t, CacheUnit > Cache;
  typedef std::vector<std::pair<long, std::pair<AmapSymbolPtr,MaterialPtr> > > DtaList;

  SmbMap __smbcache;
  SmbFileMap __smbfilecache;
  Cache __cache;
  DtaList __dta;

  long __smbNumber;

  bool symbolProcess( Geometry* object );
};


/* ------------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
// __actn_vgstarprinter_h__
#endif

