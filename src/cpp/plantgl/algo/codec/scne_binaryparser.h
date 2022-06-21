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
#include <plantgl/math/util_matrix.h>
#include <plantgl/tool/util_cache.h>
#include <plantgl/scenegraph/appearance/color.h>
#include <plantgl/scenegraph/container/indexarray.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE
class fistream;
PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE


/* ----------------------------------------------------------------------- */

/// function to put parser on verbose mode.
extern void CODEC_API parserVerbose(bool b);

/// function to test parser verbose mode.
extern bool CODEC_API isParserVerbose();

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
  BinaryParser(std::ostream& output, int max_errors=5, fistream * stream = NULL);

  /// Destructor
  virtual ~BinaryParser( );

  /// The parsing function.
  virtual bool parse(const std::string& filename);

  /// The parsing function.
  virtual bool parse();

  static ScenePtr frombinarystring(const std::string& content);

  /// open the file.
  bool open(const std::string& filename);
  bool close();
  bool eof();

  /// return the header comment.
  const std::string& getComment() const;

  /// return the scene.
  const ScenePtr getScene() const;

  /// Test if \e filename is a binary filename.
  static bool isAGeomBinaryFile(const std::string& filename);

  /// A null result.
  static const SceneObjectPtr NULLPTR;

  /// read header of GEOM binary File.
  virtual bool readHeader();

  /// read header of scene description.
  virtual bool readSceneHeader();

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

  /// Read a MonoSpectral.
  virtual bool readMonoSpectral();

  /// Read a MultiSpectral
  virtual bool readMultiSpectral();

  /// Read a Texture2D.
  virtual bool readTexture2D();

  /// Read a ImageTexture.
  virtual bool readImageTexture();

  /// Read a Texture2DTransformation.
  virtual bool readTexture2DTransformation();

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

  /// Read a Scaled object
  virtual bool readScreenProjected();

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

  SceneObject * getNext(uint_t _class);

  /// read a char value from stream
  char readChar();

  /// read an bool value from stream
  bool readBool();

  /// read an uint_t value from stream
  uint32_t readUint32();

  /// read an int_t value from stream
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
  Vector2 readVector2();

  /// read a Vector3 value from stream
  Vector3 readVector3();

  /// read a Vector4 value from stream
  Vector4 readVector4();

  /// read a Matrix2 value from stream
  Matrix2 readMatrix2();

  /// read a Matrix3 value from stream
  Matrix3 readMatrix3();

  /// read a Matrix4 value from stream
  Matrix4 readMatrix4();

  /// read a Index3 value from stream
  PGL(Index3) readIndex3();

  /// read a Index4 value from stream
  PGL(Index4) readIndex4();

  /// read a Index value from stream
  PGL(Index) readIndex();

  template<class T>
  T read();


  template <class Array>
  RCPtr<Array> readArray() {
      uint32_t _sizei = readUint32();
      RCPtr<Array> result(new Array(_sizei));
      uint32_t pid = 0;
      for(typename Array::iterator it = result->begin(); it != result->end(); ++it, ++pid){
          *it = read<typename Array::element_type>();
      }
      return result;
  }

  template <class Array>
  RCPtr<Array> loadArray() {
      std::string classname = read<std::string>();
      if (classname == PglClassInfo<Array>::name()) return readArray<Array>();
      else return RCPtr<Array>();
  }

  template <class Array2>
  RCPtr<Array2> readMatrix() {
      uint32_t _rows = readUint32();
      uint32_t _cols = readUint32();
      RCPtr<Array2> result(new Array2(_rows, _cols));
      for(typename Array2::iterator it = result->begin(); it != result->end(); ++it)
          *it = read<typename Array2::element_type>();
      return result;
  }

  template <class Array2>
  RCPtr<Array2> loadMatrix() {
      std::string classname = read<std::string>();
      if (classname == PglClassInfo<Array2>::name()) return readArray<Array2>();
      else return RCPtr<Array2>();
  }

  protected :
  /// The resulting scene.
  ScenePtr __scene;

  /// The number of roots of the scene found.
  uint_t __roots;

  /// The reference table
  Cache<SceneObjectPtr > __referencetable;

  /// Output stream.
  std::ostream& __outputStream;

  /// Input binary stream.
  fistream * stream;

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
  std::vector<uint_t> __sizes;

  /// current
  std::vector<uint_t> __currents;

  /// result of parsing an object.
  SceneObjectPtr __result;

  double __assigntime;

  bool __double_precision;

};

template<>
inline char BinaryParser::read<char>() { return readChar(); }

template<>
inline uchar_t BinaryParser::read<uchar_t>() { return readUchar(); }

template<>
inline bool BinaryParser::read<bool>() { return readBool(); }

template<>
inline real_t BinaryParser::read<real_t>() { return readReal(); }

template<>
inline uint32_t BinaryParser::read<uint32_t>() { return readUint32(); }

template<>
inline int32_t BinaryParser::read<int32_t>() { return readInt32(); }

template<>
inline uint16_t BinaryParser::read<uint16_t>() { return readUint16(); }

template<>
inline std::string BinaryParser::read<std::string>() { return readString(); }

template<>
inline Vector2 BinaryParser::read<Vector2>() { return readVector2(); }

template<>
inline Vector3 BinaryParser::read<Vector3>() { return readVector3(); }

template<>
inline Vector4 BinaryParser::read<Vector4>() { return readVector4(); }

template<>
inline Matrix2 BinaryParser::read<Matrix2>() { return readMatrix2(); }

template<>
inline Matrix3 BinaryParser::read<Matrix3>() { return readMatrix3(); }

template<>
inline Matrix4 BinaryParser::read<Matrix4>() { return readMatrix4(); }

template<>
inline PGL(Color3) BinaryParser::read<PGL(Color3)>() { return readColor3(); }

template<>
inline PGL(Color4) BinaryParser::read<PGL(Color4)>() { return readColor4(); }

template<>
inline PGL(Index) BinaryParser::read<PGL(Index)>() { return readIndex(); }

template<>
inline PGL(Index4) BinaryParser::read<PGL(Index4)>() { return readIndex4(); }

template<>
inline PGL(Index3) BinaryParser::read<PGL(Index3)>() { return readIndex3(); }

/* ------------------------------------------------------------------------- */

// __actn_binaryparser_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

