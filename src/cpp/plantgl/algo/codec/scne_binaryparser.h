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


/*! \file scne_binaryparser.h
    \brief Definition of the parsing class BinaryParser.
*/



#ifndef __scne_binaryparser_h__
#define __scne_binaryparser_h__

/* ----------------------------------------------------------------------- */

#include <string>
#include <vector>
#include <iostream>
#include "codec_config.h"
#include <plantgl/tool/rcobject.h>
#include <plantgl/math/util_math.h>
#include <plantgl/math/util_vector.h>
#include <plantgl/tool/util_cache.h>
#include <plantgl/scenegraph/appearance/color.h>
#include <plantgl/scenegraph/container/indexarray.h>

/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE
class leifstream;
TOOLS_END_NAMESPACE

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class SceneObject;
typedef RCPtr<SceneObject> SceneObjectPtr;
class Scene;
typedef RCPtr<Scene> ScenePtr;
class TokenCode;

/* ----------------------------------------------------------------------- */

/** \fn  bool getBit( T tab , uchar_t num)
    \brief return the \e num bit of tab.
    \pre num must be in [0,8*sizeof(T)].
*/
template<class T>
inline bool getBit( T tab , uchar_t num){
  GEOM_ASSERT(num < 8*sizeof(T));
   return ((tab & (1 << num)) != 0);
}


/* ----------------------------------------------------------------------- */

/**
   \class BinaryParser
   \brief An Class which read object from a file.
*/


class CODEC_API BinaryParser
{
public:

  /** Constructs a Printer with the output streams \e outputStream. */
  BinaryParser(std::ostream& output,int max_errors=5);

  /// Destructor
  virtual ~BinaryParser( );

  /// The parsing function.
  virtual bool parse(const std::string& filename);

  /// return the header comment.
  const std::string& getComment() const;

  /// return the scene.
  const ScenePtr getScene() const;

  /// Test if \e filename is a binary filename.
  static bool isAGeomBinaryFile(const std::string& filename);

  /// A null result.
  static const SceneObjectPtr NULLPTR;

 protected:

  /// read header of GEOM binary File.
  virtual bool readHeader();

  /// read tokens of GEOM binary File.
  virtual bool readNext();



  /// @name Shape
  //@{
  virtual bool readShape();

  //@}

  /// @name Material
  //@{

  /// Read a Material.
  virtual bool readMaterial();

  virtual bool readImageTexture();

  /// Read a MonoSpectral.
  virtual bool readMonoSpectral();

  /// Read a MultiSpectral
  virtual bool readMultiSpectral();

  //@}


  /// @name Geom3D
  //@{
  /// Read a AmapSymbol object
  virtual bool readAmapSymbol();

  /// Read a AsymmetricHull object
  virtual bool readAsymmetricHull();

  /// Read a AxisRotated object
  virtual bool readAxisRotated();

  /// Read a BezierCurve object
  virtual bool readBezierCurve();

  /// Read a BezierPatch object
  virtual bool readBezierPatch();

  /// Read a Box object
  virtual bool readBox();

  /// Read a Cone object
  virtual bool readCone();

  /// Read a Cylinder object
  virtual bool readCylinder();

  /// Read a ElevationGrid object
  virtual bool readElevationGrid();

  /// Read a EulerRotated object
  virtual bool readEulerRotated();

  /// Read a ExtrudedHull object
  virtual bool readExtrudedHull();

  /// Read a FaceSet object
  virtual bool readFaceSet();

  /// Read a Frustum object
  virtual bool readFrustum();

  /// Read a Extrusion object
  virtual bool readExtrusion();

  /// Read a Group object
  virtual bool readGroup();

  /// Read an IFS object
  virtual bool readIFS();

  /// Read a NurbsCurve object
  virtual bool readNurbsCurve();

  /// Read a NurbsPatch object
  virtual bool readNurbsPatch();

  /// Read a Oriented object
  virtual bool readOriented();

  /// Read a Paraboloid object
  virtual bool readParaboloid();

  /// Read a PointSet object
  virtual bool readPointSet();

  /// Read a Polyline object
  virtual bool readPolyline();

  /// Read a QuadSet object
  virtual bool readQuadSet();

  /// Read a Revolution object
  virtual bool readRevolution();

  /// Read a Revolution object
  virtual bool readSwung();

  /// Read a Scaled object
  virtual bool readScaled();

  /// Read a Sphere object
  virtual bool readSphere();

  /// Read a Tapered object
  virtual bool readTapered();

  /// Read a Translated object
  virtual bool readTranslated();

  /// Read a TriangleSet object
  virtual bool readTriangleSet();

  //@}

  /// @name Geom2D
  //@{
  /// Read a BezierCurve2D object
  virtual bool readBezierCurve2D();

  /// Read a Disc object
  virtual bool readDisc();

  /// Read a NurbsCurve2D object
  virtual bool readNurbsCurve2D();

  /// Read a PointSet2D object
  virtual bool readPointSet2D();

  /// Read a Polyline2D object
  virtual bool readPolyline2D();


  //@}

  /// Read a Text object
  virtual bool readText();

  /// Read a Font object
  virtual bool readFont();

  SceneObject * getNext(uint32_t _class);

  protected :

  /// read a char value from stream
  char readChar();

  /// read an bool value from stream
  bool readBool();

  /// read an int32_t value from stream
  uint32_t readUint32();

  /// read an uint32_t value from stream
  int32_t readInt32();

  /// read a uint16_t value from stream
  uint16_t readUint16();

  /// read a uchar_t value from stream
  uchar_t readUchar();

  /// read a real_t value from stream
  real_t readReal();

  /// read a string value from stream
  std::string readString();

  /// read a file name value from stream
  std::string readFile();

  /// read a Color3 value from stream
  PGL(Color3) readColor3();

  /// read a Color4 value from stream
  PGL(Color4) readColor4();

  /// read a Vector2 value from stream
  TOOLS(Vector2) readVector2();

  /// read a Vector3 value from stream
  TOOLS(Vector3) readVector3();

  /// read a Vector4 value from stream
  TOOLS(Vector4) readVector4();

  /// read a Index3 value from stream
  PGL(Index3) readIndex3();

  /// read a Index4 value from stream
  PGL(Index4) readIndex4();

  /// read a Index value from stream
  PGL(Index) readIndex();

  /// The resulting scene.
  ScenePtr __scene;

  /// The number of roots of the scene found.
  uint32_t __roots;

  /// The reference table
  TOOLS(Cache)<SceneObjectPtr > __referencetable;

  /// Output stream.
  std::ostream& __outputStream;

  /// Input binary stream.
  TOOLS(leifstream) * stream;

  /// The tokens codes.
  TokenCode * __tokens;

  /// Max errors.
  int __max_errors;

  /// Current errors count.
  int __errors_count;

  /// Current shape parse.
  int shape_nb;

  /// header comment.
  std::string __comment;

  /// Memory reservation.
  SceneObject * __mem[42];

  /// sizes of memory reservation tabs.
  std::vector<uint32_t> __sizes;

  /// current
  std::vector<uint32_t> __currents;

  /// result of parsing an object.
  SceneObjectPtr __result;

  double __assigntime;

  bool __double_precision;

};


/* ------------------------------------------------------------------------- */

// __actn_binaryparser_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

