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



/*! \file actn_binaryprinter.h
    \brief Definition of the action class BinaryPrinter.
*/



#ifndef __actn_binaryprinter_h__
#define __actn_binaryprinter_h__

#include "printer.h"
#include <plantgl/tool/rcobject.h>
#include <plantgl/tool/bfstream.h>

#include <plantgl/tool/util_hashmap.h>
#include <plantgl/tool/util_types.h>
#include <plantgl/pgl_container.h>
#include <vector>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */
class fistream;
class fostream;


class StatisticComputer;
class Scene;
typedef RCPtr<Scene> ScenePtr;


/* ----------------------------------------------------------------------- */

#define TokReference std::string("Reference")


/**
   \class TokenCode
   \brief A class which print and read the binary token.
*/
class CODEC_API TokenCode  {

public:


  /// Constructor.
  TokenCode(const float version);

  /// Destructor.
  ~TokenCode();

  /// Add statistic for faster parsing.
  bool setStatistic(const StatisticComputer& a);

  /// print the token \e token  on \e stream. ex :  printToken(stream,Token(Reference));.
  fostream& printCurrentToken(fostream& stream,std::string token);

  /// read \e stream and return current token.
  std::string readCurrentToken(fistream& stream);

  /// print all the token on \e stream.
  fostream& printAll(fostream& stream);

  /// read token on \e stream. return if it works.
  bool initTokens(fistream& stream,std::ostream & output);

  /// print the local Token.
  friend CODEC_API fostream& operator<<( fostream& stream, TokenCode& c );

  /// get the number of element of each class.
  std::vector<uint_t> getCounts();

  /// print the
  const float getVersion();

  private:

  /// the version of the token code.
  float __version;

  pgl_hash_map<uchar_t,std::pair<std::string,uint_t> > __code;

};

CODEC_API fostream& operator<<( fostream& stream, TokenCode& c );




/* ----------------------------------------------------------------------- */

/**
   \class BinaryPrinter
   \brief An action which prints objects of type of GEOMobject to the
   specified output stream.
*/


class CODEC_API BinaryPrinter : public Printer
{
public:

  /// The Version Number of Geom Binary Format.
  static const float BINARY_FORMAT_VERSION;

  /** Constructs a Printer with the output streams \e outputStream. */
  BinaryPrinter( fostream& outputStream );

  /// Destructor
  virtual ~BinaryPrinter( );

  /// Print \e scene in \e filename in binary format.
  virtual bool print(ScenePtr scene, const char * comment = NULL);

  /// Print header of GEOM binary File.
  virtual bool header(const char * comment = NULL);

  /// @name Pre and Post Processing
  //@{

  /// Begining of the Action.
  virtual bool beginProcess();

  /// End of the Action.
  virtual bool endProcess();
  //@}

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

  virtual bool process( ImageTexture * textureimg );

  virtual bool process( Texture2D * texture );

  virtual bool process( Texture2DTransformation * texturetransformation );

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

  void printFile(const std::string&);

  static std::string getCanonicalFilename(const std::string&);

  /// Print the scene \e scene in the file \e filename in binary format.
  static bool print(ScenePtr scene,std::string filename,const char * comment = NULL);

  static std::string tobinarystring(ScenePtr scene, const char * comment = NULL);

//private :



  /// write an bool value from stream
  void writeBool(bool var);
  inline void write(bool var) { writeBool(var); }

  /// write an int32_t value from stream
  void writeUint32(uint_t var);
  inline void write(uint_t var) { writeUint32(var); }

  /// write an uint_t value from stream
  void writeInt32(int_t var);
  inline void write(int_t var) { writeInt32(var); }

  /// write a uint16_t value from stream
  void writeUint16(uint16_t var);
  inline void write(uint16_t var) { writeUint16(var); }

  /// write a uchar_t value from stream
  void writeUchar(uchar_t var);
  inline void write(uchar_t var) { writeUchar(var); }

  /// write a real_t value from stream
  void writeReal(real_t var);
  inline void write(real_t var) { writeReal(var); }

  /// write a string value from stream
  void writeString(const std::string& var);
  inline void write(const std::string& var) { writeString(var); }

  /// write a file name value from stream
  void writeFile(const std::string& var);

  void write(const Color3& var);
  void write(const Color4& var);
  void write(const Index3& var);
  void write(const Index4& var);
  void write(const Index& var);
  void write(const Vector2& var);
  void write(const Vector3& var);
  void write(const Vector4& var);
  void write(const Matrix2& var);
  void write(const Matrix3& var);
  void write(const Matrix4& var);

  template<class Array>
  void writeArray(const Array& array){
    uint_t _sizei = array.size();
    writeUint32(_sizei);
    for (typename Array::const_iterator it = array.begin(); it != array.end(); ++it) {
      write(*it);
    };
  }

  template<class Array>
  void dumpArray(const Array& array){
      writeString(PglClassInfo<Array>::name());
      writeArray(array);
  }


  template<class Array2>
  void writeMatrix(const Array2& array){
    uint_t _rows = array.getRowNb();
    uint_t _cols = array.getColumnNb();
    writeUint32( _rows );
    writeUint32( _cols );
    for (typename Array2::const_iterator it = array.begin(); it != array.end(); ++it) {
        write(*it);
    };
  }

  template<class Array2>
  void dumpMatrix(const Array2& array){
      writeString(PglClassInfo<Array2>::name());
      writeMatrix(array);
  }

protected:
  /// Return a Token Number for the string \e _string.
  void printType(const std::string& _string);

  /// Binary output stream.
  fostream& __outputStream;

  /// The tokens codes.
  TokenCode __tokens;

};


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ------------------------------------------------------------------------- */

// __actn_binaryprinter_h__
#endif
