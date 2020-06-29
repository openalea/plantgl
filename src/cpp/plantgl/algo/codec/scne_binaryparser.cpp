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



#include "scne_binaryparser.h"
#include "binaryprinter.h"
#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/pgl_container.h>
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/scene/scene.h>

#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/tool/timer.h>
#include <plantgl/tool/bfstream.h>
#include <plantgl/tool/dirnames.h>
#include <plantgl/tool/util_enviro.h>

#include "scne_parser.h"

#include <iostream>

#include <typeinfo>

PGL_USING_NAMESPACE

using namespace std;
using namespace STDEXT;

// #define GEOM_DEBUG


/* ----------------------------------------------------------------------- */

static bool __verbose = true;

void PGL(parserVerbose)(bool b)
{
  __verbose = b;
}

bool PGL(isParserVerbose)()
{
  return (__verbose);
}

/* ----------------------------------------------------------------------- */

#define GEOM_READ_DEFAULT(_default) \
   uchar_t _default; \
   *stream >> _default; \

#define IF_GEOM_NOTDEFAULT(_default,num) \
   if(!getBit(_default,uchar_t(num))) \

#define IF_GEOM_DEFAULT(_default,num) \
   if(getBit(_default,uchar_t(num))) \

#define GEOM_NOTDEFAULT_COND(_default,num) \
   (!getBit(_default,uchar_t(num))) \

#define GEOM_DEFAULT_COND(_default,num) \
   (getBit(_default,uchar_t(num))) \


/* ----------------------------------------------------------------------- */



#define GEOM_PARSER_SETNAME( _name,_ident,_shape,type) \
  if (_shape) { \
    __result = SceneObjectPtr(_shape); \
    if (!_name.empty()) { \
        __result->setName(_name); \
        count_name++; \
    } \
    if (_ident != 0) \
        __referencetable.insert(_ident, __result); \
    return  true; \
  } \
  else { \
    __errors_count++; \
    __result = NULLPTR; \
    return false; \
  } \


/* ----------------------------------------------------------------------- */

#define GEOM_READ_FIELD(obj,field,type)  obj->get##field() = read##type();

#define GEOM_READ_ARRAY(obj,type,primitive) { \
    uint_t _sizej = readUint32(); \
    if (_sizej > 0){ \
      obj = type##Ptr (new type(_sizej)); \
      for (type::iterator _it = obj->begin();_it != obj->end() && !stream->eof(); _it++) { \
      *_it = read##primitive(); \
      }; \
    }; \
  };

#define GEOM_READ_INDEXARRAY(obj) { \
    uint_t _sizej  = readUint32(); \
    if (_sizej > 0){ \
      obj = IndexArrayPtr(new IndexArray(_sizej)); \
      for (IndexArray::iterator _it = obj->begin();_it != obj->end() && !stream->eof(); _it++) { \
        *_it = readIndex(); \
      }; \
    }; \
  };

#define GEOM_READ_MATRIX(obj,type,primitive) { \
    uint_t _rows  = readUint32(); \
    uint_t _cols  = readUint32(); \
    obj = type##Ptr (new type(_rows,_cols)); \
    for (type::iterator _it = obj->begin();_it != obj->end() && !stream->eof(); _it++) { \
    *_it = read##primitive(); \
    }; \
  };


/* ----------------------------------------------------------------------- */
//#define MEMORY_MANAGEMENT

#ifndef MEMORY_MANAGEMENT

#define GEOM_INIT_MEM(num,type,size);
#define GEOM_INIT_OBJ(obj, num,type)type * obj = new type;
#define GEOM_DEL_OBJ(obj,num)delete obj; __result = NULLPTR;
#define GEOM_CLEAN_MEM(num,type,size);

#else

#define GEOM_INIT_MEM(num,type,size) \
  if(!__sizes[num])__mem[num] = NULL; \
  else if(__sizes[num] == 1)__mem[num] = new type; \
  else __mem[num] = new type[__sizes[num]]; \
  if(__sizes[num])size += __sizes[num]*sizeof(type); \



#define GEOM_INIT_OBJ(obj, num,type) \
     type * obj; \
     if(__currents[num] >=  __sizes[num]){ \
       obj = new type; \
         __outputStream << "Warning. Memory not allocated. [Class="  \
        << num << " , obj=" <<__currents[num] << " , reserved="  << __sizes[num] << ']' << endl; \
     } \
     else { \
      SceneObject * b(getNext(num)); \
      if(!b){ \
         obj = new type; \
         __outputStream << "Warning. Memory not allocated. [Class="  \
        << num << " , obj=" <<__currents[num] << " , reserved="  << __sizes[num] << ']' << endl; \
      } \
      else{ \
         obj = (type *)b; \
         if(!obj) { \
             __outputStream << "Warning Construction error. Memory allocated cannot be reached. [Class="  \
             << num << " , obj=" <<__currents[num] << '/' << __sizes[num] << ']' << endl; \
             obj = new type; \
         } \
         obj+=__currents[num]; \
       } \
     } \
     __currents[num]++; \

#define GEOM_DEL_OBJ(obj,num)__currents[num]--;__result = NULLPTR;


#define GEOM_CLEAN_MEM(num,type,size) \
  if(__currents[num] <  __sizes[num]){ \
     SceneObject * b(getNext(num)); \
     type * obj; \
     if(b){ \
        obj = dynamic_cast<type *>(b); \
        if(obj) { \
         if(!__currents[num]){ \
          __outputStream << "Unused object(s) (" << num << " - " <<__sizes[num]  << ") : adr=" \
               << (uint_t)obj << " - size=" << __sizes[num]*sizeof(*obj)<< endl; \
          if(__sizes[num] == 1) delete obj; \
          else delete [] obj; \
         } \
         else {\
          obj+=__currents[num]; \
          for(uint_t j = __currents[num]; j < __sizes[num]; j++){ \
           __outputStream << "Unused object (" << num << " , " << j << ") : adr=" << (uint_t)obj << " - size=" << sizeof(*obj)<< endl; \
           obj->~type(); obj++;\
          } \
         } \
        } \
        else { \
            __outputStream << "Cannot delete unused reserved memory (" << num << " - " << __sizes[num]-__currents[num] << ')' << endl; \
        } \
      } \
      size += (__sizes[num]-__currents[num])*sizeof(type); \
  } \

#endif

/* ----------------------------------------------------------------------- */

#ifdef GEOM_DEBUG

#define GEOM_BEGIN(_name,_ident) \
    string  _name = readString(); \
    uint_t _ident(0); \
    if(!_name.empty()){ \
     _ident = readUint32(); \
     cerr << "Name : '" << _name << "', Id : " << _ident << endl; \
    } \
    else if( __tokens->getVersion() >= 2.0f) \
     _ident = readUint32(); \

#else

#define GEOM_BEGIN(_name,_ident) \
    string  _name = readString(); \
    uint_t _ident(0); \
    if(!_name.empty()){ \
     _ident = readUint32(); \
    } \
    else if( __tokens->getVersion() >= 2.0f) \
     _ident = readUint32(); \

#endif

/* ----------------------------------------------------------------------- */

static uint_t count_name(0);

const SceneObjectPtr BinaryParser::NULLPTR;

/* ----------------------------------------------------------------------- */
BinaryParser::BinaryParser(ostream& output,int max_errors, fistream * _stream) :
    __scene(new Scene()),
    __roots(0),
    __referencetable(),
    __outputStream(output),
    stream(_stream),
    __tokens(NULL),
    __max_errors(max_errors),
    __errors_count(0),
    shape_nb(0),
    __comment(),
    __sizes(45,uint_t(0)),
    __currents(45,uint_t(0)),
    __result(),
    __assigntime(0),
    __double_precision(false){
    for(uint_t i=0;i<45;i++)__mem[i]=NULL;
}

/* ----------------------------------------------------------------------- */

BinaryParser::~BinaryParser( ) {
  if(__tokens)delete __tokens;
#ifdef MEMORY_MANAGEMENT
  uint_t _reservedsize(0);
#endif
  GEOM_CLEAN_MEM(0,Shape,_reservedsize);
  GEOM_CLEAN_MEM(1,Material,_reservedsize);  GEOM_CLEAN_MEM(2,MonoSpectral,_reservedsize);
  GEOM_CLEAN_MEM(3,MultiSpectral,_reservedsize);  GEOM_CLEAN_MEM(4,AmapSymbol,_reservedsize);
  GEOM_CLEAN_MEM(5,AsymmetricHull,_reservedsize);  GEOM_CLEAN_MEM(6,AxisRotated,_reservedsize);
  GEOM_CLEAN_MEM(7,BezierCurve,_reservedsize);  GEOM_CLEAN_MEM(8,BezierPatch,_reservedsize);
  GEOM_CLEAN_MEM(9,Box,_reservedsize);  GEOM_CLEAN_MEM(10,Cone,_reservedsize);
  GEOM_CLEAN_MEM(11,Cylinder,_reservedsize);  GEOM_CLEAN_MEM(12,ElevationGrid,_reservedsize);
  GEOM_CLEAN_MEM(13,EulerRotated,_reservedsize);  GEOM_CLEAN_MEM(14,ExtrudedHull,_reservedsize);
  GEOM_CLEAN_MEM(15,FaceSet,_reservedsize);  GEOM_CLEAN_MEM(16,Frustum,_reservedsize);
  GEOM_CLEAN_MEM(17,Extrusion,_reservedsize);  GEOM_CLEAN_MEM(18,Group,_reservedsize);
  GEOM_CLEAN_MEM(19,NurbsCurve,_reservedsize);  GEOM_CLEAN_MEM(20,NurbsPatch,_reservedsize);
  GEOM_CLEAN_MEM(21,Oriented,_reservedsize);  GEOM_CLEAN_MEM(22,Paraboloid,_reservedsize);
  GEOM_CLEAN_MEM(23,PointSet,_reservedsize);  GEOM_CLEAN_MEM(24,Polyline,_reservedsize);
  GEOM_CLEAN_MEM(25,QuadSet,_reservedsize);  GEOM_CLEAN_MEM(26,Revolution,_reservedsize);
  GEOM_CLEAN_MEM(27,Scaled,_reservedsize);  GEOM_CLEAN_MEM(28,Sphere,_reservedsize);
  GEOM_CLEAN_MEM(29,Tapered,_reservedsize);  GEOM_CLEAN_MEM(30,Translated,_reservedsize);
  GEOM_CLEAN_MEM(31,TriangleSet,_reservedsize);  GEOM_CLEAN_MEM(32,BezierCurve2D,_reservedsize);
  GEOM_CLEAN_MEM(33,Disc,_reservedsize);  GEOM_CLEAN_MEM(34,NurbsCurve2D,_reservedsize);
  GEOM_CLEAN_MEM(35,PointSet2D,_reservedsize);  GEOM_CLEAN_MEM(36,Polyline2D,_reservedsize);
  GEOM_CLEAN_MEM(37,Swung,_reservedsize); GEOM_CLEAN_MEM(38,IFS,_reservedsize);
  GEOM_CLEAN_MEM(39,TextureImage,_reservedsize);GEOM_CLEAN_MEM(40,Text,_reservedsize);
  GEOM_CLEAN_MEM(41,Font,_reservedsize);GEOM_CLEAN_MEM(42,Texture2D,_reservedsize);
  GEOM_CLEAN_MEM(43,Texture2DTransformation,_reservedsize);GEOM_CLEAN_MEM(44,ScreenProjected,_reservedsize);  
#ifdef MEMORY_MANAGEMENT
  if(_reservedsize > 0)
      __outputStream << "Delete unused reserved memory ... done (" << (_reservedsize > 1024 ? _reservedsize/1024 : _reservedsize )
           << (_reservedsize > 1024 ? " K" : " " ) << "bytes)." << endl;
#endif
}

/* ----------------------------------------------------------------------- */

/// read an bool value from stream
bool BinaryParser::readBool()
{ bool val;  *stream >> val; return val; }

/// read an int32_t value from stream
uint32_t BinaryParser::readUint32()
{ uint32_t val;  *stream >> val; return val; }

/// read an uint_t value from stream
int32_t BinaryParser::readInt32()
{ int32_t val;  *stream >> val; return val; }

/// read a uint16_t value from stream
uint16_t BinaryParser::readUint16()
{ uint16_t val;  *stream >> val; return val; }

/// read a uchar_t value from stream
uchar_t BinaryParser::readUchar()
{ uchar_t val;  *stream >> val; return val; }

char BinaryParser::readChar()
{ char val;  *stream >> val; return val; }

/// read a real_t value from stream
real_t BinaryParser::readReal()
{
 if (__double_precision)
 { double val;  *stream >> val; return val; }
 else
 { float val;  *stream >> val; return val; }
}

/// read a string value from stream
std::string BinaryParser::readString()
{
  std::string val;
  uint16_t _sizes = readUint16();
  if( _sizes!=0){
   char * label = new char[_sizes+1];
   stream->read(label,_sizes);
   label[_sizes] = '\0';
   val = std::string(label);
   delete [] label;
  }
  return val;
}

#define MAXFILELENGTH 1000

/// read a file name value from stream
std::string BinaryParser::readFile()
{
  std::string val;
  char c[MAXFILELENGTH+1];
  uint_t i=0;
  *stream >> c[i];
  if( c[i] =='!'){
   *stream >> c[i];
   while(c[i]!='!' && !stream->eof() && i< MAXFILELENGTH){
    i++;
    *stream >> c[i];
   }
   if(i==MAXFILELENGTH){
      char r = ' ';
      while(r!='!'  && !stream->eof()) *stream >> r;
   } \
   c[i] = '\0';
   if(i!=0) val = c;
   if(val.size() > 11 &&
      (string(val.begin(),val.begin()+11) == "AMAPMOD_DIR" ||
       string(val.begin(),val.begin()+11) == "PLANTGL_DIR")){
        string suffix(val.begin()+11,val.end());
        val = getPlantGLDir();
        if(suffix[0] != '\\' && suffix[0] != '/')val += '/';
        val += suffix;
   }
  }
  else __outputStream << "Filename wrong format." << endl;
  return val;
}

/// read a Color3 value from stream
Color3 BinaryParser::readColor3()
{ Color3 val; val.getRed() = readUchar(); val.getGreen() = readUchar();
  val.getBlue() = readUchar(); return val; }

/// read a Color4 value from stream
Color4 BinaryParser::readColor4()
{ Color4 val; val.getRed() = readUchar(); val.getGreen() = readUchar();
  val.getBlue() = readUchar(); val.getAlpha() = readUchar(); return val; }

/// read a Vector2 value from stream
Vector2 BinaryParser::readVector2()
{ Vector2 val; val.x() = readReal(); val.y() = readReal(); return val; }

/// read a Vector3 value from stream
Vector3 BinaryParser::readVector3()
{ Vector3 val; val.x() = readReal(); val.y() = readReal();
  val.z() = readReal(); return val; }

/// read a Vector4 value from stream
Vector4 BinaryParser::readVector4()
{ Vector4 val; val.x() = readReal(); val.y() = readReal();
  val.z() = readReal(); val.w() = readReal(); return val; }

/// read a Vector2 value from stream
Matrix2 BinaryParser::readMatrix2()
{ Matrix2 val; for(Matrix2::iterator it = val.begin(); it != val.end(); ++it) *it = readReal(); return val; }

/// read a Vector2 value from stream
Matrix3 BinaryParser::readMatrix3()
{ Matrix3 val; for(Matrix3::iterator it = val.begin(); it != val.end(); ++it) *it = readReal(); return val; }

/// read a Vector2 value from stream
Matrix4 BinaryParser::readMatrix4()
{ Matrix4 val; for(Matrix4::iterator it = val.begin(); it != val.end(); ++it) *it = readReal(); return val; }

/// read a Index3 value from stream
Index3 BinaryParser::readIndex3()
{ Index3 val; for(uchar_t it = 0; it < 3; ++it) val.setAt(it,readUint32());  return val; }

/// read a Index4 value from stream
Index4 BinaryParser::readIndex4()
{ Index4 val; for(uchar_t it = 0; it < 4; ++it) val.setAt(it,readUint32());  return val; }

/// read a Index value from stream
Index BinaryParser::readIndex()
{
  uint_t size = readUint32();
  Index val(size);
  for(Index::iterator it = val.begin(); it != val.end(); ++it)
      *it = readUint32();
  return val;
}

/* ----------------------------------------------------------------------- */

const string& BinaryParser::getComment() const {
  return __comment;
}

const ScenePtr BinaryParser::getScene() const {
  return __scene;
}

/* ----------------------------------------------------------------------- */

SceneObject * BinaryParser::getNext(uint_t _class){
    if(__currents[_class]<  __sizes[_class]){
        return (__mem[_class]);
    }
    else return NULL;
}

/* ----------------------------------------------------------------------- */

bool BinaryParser::isAGeomBinaryFile(const string& filename){
    bifstream stream(filename.c_str());
    stream.setByteOrder(PglLittleEndian);
    if(!stream)return false;
    char tokBegin[7];
    stream.read(tokBegin,6);
    tokBegin[6]='\0';
    string _tokBegin(tokBegin);
    if(_tokBegin == "!bGEOM")return true;
    return false;
}

/* ----------------------------------------------------------------------- */

bool BinaryParser::readHeader(){
  char tokBegin[7];
  stream->read(tokBegin,6);
  tokBegin[6]='\0';
  string _tokBegin(tokBegin);
  if(_tokBegin != "!bGEOM"){
      __outputStream << "Header : " << _tokBegin << " not valid" << endl;
      return false;
  }
  float _version;
  *stream >> _version;
#ifdef GEOM_DEBUG
  printf("File version : %f\n",_version);
#endif
  if(_version > BinaryPrinter::BINARY_FORMAT_VERSION){
      __outputStream << "*** ERROR: Binary Format Version invalid  (File=" << _version << ";Current=" << BinaryPrinter::BINARY_FORMAT_VERSION << "). Upgrade."<< endl;
      return false;
  }
  if(__tokens)delete __tokens;
  __tokens = new TokenCode(_version);
  char c;
  if(_version >= 1.1f){
          c = readChar();
          if(c != char(13)){
                  __outputStream << "*** ERROR: Binary integrity check failed ! [ Char=" << int(c) << " instead of 13 ]" <<  endl;
                  __outputStream << "*** ERROR: Transfert of this file may have corrupt its integrity." <<  endl;
                  __outputStream << "*** ERROR: Abort." <<  endl;
                  return false;
          }
          c = readChar();
          if(c != char(10)){
                  __outputStream << "*** ERROR: Binary integrity check failed ! [ Char=" << int(c) << " instead of 10 ]" <<  endl;
                  __outputStream << "*** ERROR: Transfert of this file may have corrupt its integrity." <<  endl;
                  __outputStream << "*** ERROR: Abort." <<  endl;
                  return false;
          }
  }
  if(_version >= 1.6f){
      uchar_t precision = readUchar();
      if (precision == 32){
        __double_precision = false;
      }
      else if (precision == 64){
        __double_precision = true;
      }
      else  {
        __outputStream << "*** ERROR: Real precision not valid ! [ Value=" << int(precision) << " ]" <<  endl;
      }
  }
  else {
    __double_precision = false;
  }
#ifdef GEOM_DEBUG
  std::cerr << "Assume "<< (!__double_precision?"simple":"double")<< " precision." << std::endl;
#endif

  c = readChar();
  if(c != '#'){
      __outputStream << "No Valid Version found : " << _version << " ('" << c << "')" <<  endl;
      return false;
  }
  c = readChar();
  while(c != '#' && !stream->eof()){
    __comment+=c;
#ifdef GEOM_DEBUG
    cerr << c;
#endif
    c = readChar();
  }
#ifdef GEOM_DEBUG
  cerr << std::endl;
#endif
  return true;
}

bool BinaryParser::readSceneHeader(){
  if(!__tokens->initTokens(*stream,__outputStream)){
          __outputStream << "*** ERROR: Token initialization failed !" <<  endl;
          __outputStream << "*** ERROR: Abort." <<  endl;
          return false;
  }
  uint_t _size = readUint32();
#ifdef GEOM_DEBUG
  cerr << "Must find a scene of " << _size << " objects." << endl;
  __outputStream << "Must find a scene of " << _size << " objects." << endl;
#endif
  __scene->resize(_size);
  // __sizes = __tokens->getCounts();
#ifdef MEMORY_MANAGEMENT
  uint_t _reservedsize(0);
  __outputStream << "Initialisation of memory management ... " << flush;
#endif
  GEOM_INIT_MEM(0,Shape,_reservedsize);
  GEOM_INIT_MEM(1,Material,_reservedsize);
  GEOM_INIT_MEM(2,MonoSpectral,_reservedsize);
  GEOM_INIT_MEM(3,MultiSpectral,_reservedsize);  GEOM_INIT_MEM(4,AmapSymbol,_reservedsize);
  GEOM_INIT_MEM(5,AsymmetricHull,_reservedsize);  GEOM_INIT_MEM(6,AxisRotated,_reservedsize);
  GEOM_INIT_MEM(7,BezierCurve,_reservedsize);  GEOM_INIT_MEM(8,BezierPatch,_reservedsize);
  GEOM_INIT_MEM(9,Box,_reservedsize);  GEOM_INIT_MEM(10,Cone,_reservedsize);
  GEOM_INIT_MEM(11,Cylinder,_reservedsize);  GEOM_INIT_MEM(12,ElevationGrid,_reservedsize);
  GEOM_INIT_MEM(13,EulerRotated,_reservedsize);  GEOM_INIT_MEM(14,ExtrudedHull,_reservedsize);
  GEOM_INIT_MEM(15,FaceSet,_reservedsize);  GEOM_INIT_MEM(16,Frustum,_reservedsize);
  GEOM_INIT_MEM(17,Extrusion,_reservedsize);  GEOM_INIT_MEM(18,Group,_reservedsize);
  GEOM_INIT_MEM(19,NurbsCurve,_reservedsize);  GEOM_INIT_MEM(20,NurbsPatch,_reservedsize);
  GEOM_INIT_MEM(21,Oriented,_reservedsize);  GEOM_INIT_MEM(22,Paraboloid,_reservedsize);
  GEOM_INIT_MEM(23,PointSet,_reservedsize);  GEOM_INIT_MEM(24,Polyline,_reservedsize);
  GEOM_INIT_MEM(25,QuadSet,_reservedsize);  GEOM_INIT_MEM(26,Revolution,_reservedsize);
  GEOM_INIT_MEM(27,Scaled,_reservedsize);  GEOM_INIT_MEM(28,Sphere,_reservedsize);
  GEOM_INIT_MEM(29,Tapered,_reservedsize);  GEOM_INIT_MEM(30,Translated,_reservedsize);
  GEOM_INIT_MEM(31,TriangleSet,_reservedsize);  GEOM_INIT_MEM(32,BezierCurve2D,_reservedsize);
  GEOM_INIT_MEM(33,Disc,_reservedsize);  GEOM_INIT_MEM(34,NurbsCurve2D,_reservedsize);
  GEOM_INIT_MEM(35,PointSet2D,_reservedsize);  GEOM_INIT_MEM(36,Polyline2D,_reservedsize);
  GEOM_INIT_MEM(37,Swung,_reservedsize); GEOM_INIT_MEM(38,IFS,_reservedsize);
  GEOM_INIT_MEM(39,TextureImage,_reservedsize);GEOM_INIT_MEM(40,Text,_reservedsize);
  GEOM_INIT_MEM(41,Font,_reservedsize);GEOM_INIT_MEM(42,Texture2D,_reservedsize);
  GEOM_INIT_MEM(43,Texture2DTransformation,_reservedsize);GEOM_INIT_MEM(44,ScreenProjected,_reservedsize);
#ifdef MEMORY_MANAGEMENT
  __outputStream  << "done ("<< (_reservedsize > 1024 ? _reservedsize/1024 : _reservedsize )
                  << (_reservedsize > 1024 ? " K" : " " ) << "bytes)." << endl;
#endif
  return true;
}

/* ----------------------------------------------------------------------- */
bool BinaryParser::open(const std::string& filename)
{
    stream = new bifstream(filename.c_str());
    stream->setByteOrder(PglLittleEndian);

    if(!*stream){
        pglErrorEx(PGLERRORMSG(C_FILE_OPEN_ERR_s),filename.c_str());
        delete stream;
        stream = 0;
        return false;
    }
    return true;
}

bool BinaryParser::close()
{
    if(stream) {
        delete stream;
        stream = 0;
        return true;
    }
    else return false;
}

bool BinaryParser::eof()
{

    if(stream) {
        return stream->eof();
    }
    else return true;
}

/// The parsing function.
bool BinaryParser::parse(const string& filename){
    if(!open(filename)) return false;
    string p = get_cwd();
    chg_dir(get_dirname(filename));
    bool res = parse();
    chg_dir(p);
    return res;
}

bool BinaryParser::parse(){
    if(!readHeader())return false;
    if(!readSceneHeader())return false;
    PglErrorStream::Binder psb(__outputStream);
    Timer t;
    __errors_count=0;
    shape_nb=0;
    t.start();
    while(!stream->eof()&& __errors_count!=__max_errors)readNext();
    t.stop();
    if(__roots > 0)
       __scene->resize(__roots);
    else __scene = ScenePtr(new Scene);
#ifdef GEOM_DEBUG
    if(isParserVerbose()){
      __outputStream << "Parse file in " << t.elapsedTime() << " sec (" << __assigntime << ")." << endl;
      cerr << "Parse file in " << t.elapsedTime() << " sec (" << __assigntime << ")." << endl;
    }
#endif
    close();
    return true;
}

#include <sstream>

ScenePtr BinaryParser::frombinarystring(const std::string& content)
{
    std::ostringstream ostream;
    std::istringstream sstream(content);
    fistream * stream = new fistream(sstream);
    stream->setByteOrder(PglLittleEndian);
    BinaryParser parser(ostream, 5, stream);
    parser.parse();
    return parser.getScene();

}

/* ----------------------------------------------------------------------- */

bool BinaryParser::readNext(){
  string _classname = __tokens->readCurrentToken(*stream);
#ifdef GEOM_DEBUG
  uint_t pos =  stream->getStream().tellg();
  --pos;
  cerr << "Found " << _classname << " at pos : " << pos << std::endl;
  __outputStream << "Found " << _classname << " at pos : " << pos << std::endl;
#endif
  if(_classname == "Shape")                   return readShape ();
  else if(_classname == "Material")           return readMaterial  ();
  else if(_classname == "Texture2D")          return readTexture2D ();
  else if(_classname == "ImageTexture")       return readImageTexture ();
  else if(_classname == "Texture2DTransformation")return readTexture2DTransformation();
  else if(_classname == "MonoSpectral")       return readMonoSpectral ();
  else if(_classname == "MultiSpectral")      return readMultiSpectral();
  else if(_classname == "AmapSymbol")         return readAmapSymbol ();
  else if(_classname == "AxisRotated")        return readAxisRotated();
  else if(_classname == "AsymmetricHull")     return readAsymmetricHull();
  else if(_classname == "BezierCurve")        return readBezierCurve();
  else if(_classname == "BezierPatch")        return readBezierPatch();
  else if(_classname == "Box")                return readBox ();
  else if(_classname == "Cone")               return readCone();
  else if(_classname == "Cylinder")           return readCylinder();
  else if(_classname == "ElevationGrid")      return readElevationGrid();
  else if(_classname == "EulerRotated")       return readEulerRotated();
  else if(_classname == "ExtrudedHull")       return readExtrudedHull();
  else if(_classname == "Extrusion")          return readExtrusion();
  else if(_classname == "FaceSet")            return readFaceSet();
  else if(_classname == "Frustum")            return readFrustum();
  else if(_classname == "Group")              return readGroup();
  else if(_classname == "IFS")                return readIFS();
  else if(_classname == "NurbsCurve")         return readNurbsCurve();
  else if(_classname == "NurbsPatch")         return readNurbsPatch();
  else if(_classname == "Oriented")           return readOriented();
  else if(_classname == "Paraboloid")         return readParaboloid();
  else if(_classname == "PointSet")           return readPointSet();
  else if(_classname == "Polyline")           return readPolyline();
  else if(_classname == "QuadSet")            return readQuadSet();
  else if(_classname == "Revolution")         return readRevolution();
  else if(_classname == "Swung")              return readSwung();
  else if(_classname == "Scaled")             return readScaled();
  else if(_classname == "ScreenProjected")    return readScreenProjected();   
  else if(_classname == "Sphere")             return readSphere();
  else if(_classname == "Tapered")            return readTapered();
  else if(_classname == "Translated")         return readTranslated();
  else if(_classname == "TriangleSet")        return readTriangleSet();
  else if(_classname == "BezierCurve2D")      return readBezierCurve2D();
  else if(_classname == "Disc")               return readDisc();
  else if(_classname == "NurbsCurve2D")       return readNurbsCurve2D();
  else if(_classname == "PointSet2D")         return readPointSet2D();
  else if(_classname == "Polyline2D")         return readPolyline2D();
  else if(_classname == "Text")               return readText();
  else if(_classname == "Font")               return readFont();
  else if(_classname == "Reference"){
    uint_t _ref = readUint32();
#ifdef GEOM_DEBUG
    cerr << "Ref : " << _ref << endl;
    __outputStream << "Ref : " << _ref << endl;
#endif
    Timer _timer;
    _timer.start();
    Cache<SceneObjectPtr >::Iterator _it = __referencetable.find(_ref);
    __assigntime += _timer.stop();
    if(_it !=__referencetable.end()){
        __result =  _it->second;
        if(__result)return true;
        else {
            __errors_count++;
            __result = NULLPTR;
            return false;
        }
    }
    else{
        __errors_count++;
        __result = NULLPTR;
        return false;
    }
  }
#ifdef GEOM_DEBUG
  else if(_classname == "Error"){
      __outputStream << "*** PARSER: Error when write file." << endl; \
      __errors_count++;
      __result = NULLPTR;
      return false;
  }
#endif
  else{
      __errors_count++;
      __result = NULLPTR;
      return false;
  }
}

/* ----------------------------------------------------------------------- */


bool BinaryParser::readShape(){
    GEOM_INIT_OBJ(a, 0, Shape);

    string _name = readString();
    GEOM_READ_FIELD(a,Id,Uint32);
    if( __tokens->getVersion() >= 1.9f){
        GEOM_READ_FIELD(a,ParentId,Uint32);
    }

    if(readNext())
       a->getGeometry() = dynamic_pointer_cast<Geometry>(__result);
    if(readNext()){
        a->getAppearance() = dynamic_pointer_cast<Appearance>(__result);
        if (is_null_ptr(a->getAppearance()) && __tokens->getVersion() <= 2.3f && is_valid_ptr(__result)){
            ImageTexturePtr imgtex = dynamic_pointer_cast<ImageTexture>(__result);
            if(imgtex) {
                a->getAppearance() = AppearancePtr(new Texture2D(imgtex));
            }
        }
    }

    if(a->getGeometry()){
        if (!a->getAppearance() || !a->getAppearance()->isValid()){
            __outputStream << "*** PARSER: <Shape : " << (_name.empty() ? "(unamed)" : _name ) << "> Appearance not valid. Setting it to default." << endl;
            a->getAppearance() = AppearancePtr(new Material());

        }
        if(!_name.empty())a->setName(_name);
        __result = SceneObjectPtr(a);
        Shape3DPtr sh = Shape3DPtr(a);
        if(!sh) cerr << "Shape not valid" << endl;
        else {
            if(__roots < __scene->size())
                 __scene->setAt(__roots, sh);
            else
                 __scene->add(sh);
            __roots++;
        }
        if(isParserVerbose())
          if(__roots % 50 == 0 || __roots == __scene->size())
             printf("\x0d Already parsed : %i %% shapes.", 100*__roots / __scene->size());
        return true;
    }
    else{
        __outputStream << "*** PARSER: <Shape : " << (_name.empty() ? "(unamed)" : _name ) << ">"
                       << (!a->getGeometry() || !a->getGeometry()->isValid()? " Geometry ":" ")
                       << (!a->getAppearance() || !a->getAppearance()->isValid() ? "Appearance " : "" ) << "not valid." << endl;
        __currents[0]--;
        a->getGeometry()=GeometryPtr();
        a->getAppearance() = AppearancePtr();
        a->getId() = Shape::NOID;
        __result = NULLPTR;
        return false;
    }
}

/* ----------------------------------------------------------------------- */


bool BinaryParser::readMaterial() {
    GEOM_BEGIN(_name,_ident);

    GEOM_READ_DEFAULT(_default);

    GEOM_INIT_OBJ(mat, 1, Material);

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(mat,Ambient,Color3) ;

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(mat,Diffuse,Real);

    IF_GEOM_NOTDEFAULT(_default,2)
        GEOM_READ_FIELD(mat,Specular,Color3);

    IF_GEOM_NOTDEFAULT(_default,3)
        GEOM_READ_FIELD(mat,Emission,Color3);

    IF_GEOM_NOTDEFAULT(_default,4)
        GEOM_READ_FIELD(mat,Shininess,Real);

    IF_GEOM_NOTDEFAULT(_default,5)
        GEOM_READ_FIELD(mat,Transparency,Real);

    GEOM_PARSER_SETNAME(_name,_ident,mat,Material);
    return true;

}

/* ----------------------------------------------------------------------- */

bool BinaryParser::readTexture2D() {
    GEOM_BEGIN(_name,_ident);

    GEOM_READ_DEFAULT(_default);

    GEOM_INIT_OBJ(texture, 42, Texture2D);

    bool invalid = false;

    if(readNext())
        texture->getImage() = dynamic_pointer_cast<ImageTexture>(__result);

    if(!texture->getImage()){
        __outputStream << "*** PARSER: <Texture2D : " << (_name.empty() ? "(unamed)" : _name ) << "> Image not valid." << endl;
        invalid = true;
    }

    IF_GEOM_NOTDEFAULT(_default,0)
    {
        if(readNext())
            texture->getTransformation() = dynamic_pointer_cast<Texture2DTransformation>(__result);
        if(!texture->getTransformation()){
            __outputStream << "*** PARSER: <Texture2D : " << (_name.empty() ? "(unamed)" : _name ) << "> Transformation not valid." << endl;
            invalid = true;
        }
    }

    if( __tokens->getVersion() >= 2.3f){
        IF_GEOM_NOTDEFAULT(_default,1)
             GEOM_READ_FIELD(texture,BaseColor,Color4);
    }


    if (invalid) {
        GEOM_DEL_OBJ(texture,42) ;
        return false;
    }

    GEOM_PARSER_SETNAME(_name,_ident,texture,Texture2D);
    return true;

}

/* ----------------------------------------------------------------------- */

bool BinaryParser::readImageTexture() {
    GEOM_BEGIN(_name,_ident);

    GEOM_READ_DEFAULT(_default);

    GEOM_INIT_OBJ(mat, 39, ImageTexture);

    string FileName = readFile();
    cerr << "Filename : " << FileName << endl;
    if(!FileName.empty()&&exists(FileName.c_str())) {
            FileName = absolute_filename(FileName);
            mat->getFilename() = FileName;
    }
    float version =  __tokens->getVersion();

    if( version >= 2.3f){
        IF_GEOM_NOTDEFAULT(_default,0)
            GEOM_READ_FIELD(mat,RepeatS,Bool) ;

        IF_GEOM_NOTDEFAULT(_default,1)
            GEOM_READ_FIELD(mat,RepeatT,Bool);

        if( version < 2.4f){
            IF_GEOM_NOTDEFAULT(_default,2)
                real_t transparency = readReal();
        }
    }
    else {
        MaterialPtr oldmat(new Material());
        IF_GEOM_NOTDEFAULT(_default,0)
            GEOM_READ_FIELD(oldmat,Ambient,Color3) ;

        IF_GEOM_NOTDEFAULT(_default,1)
            GEOM_READ_FIELD(oldmat,Diffuse,Real);

        IF_GEOM_NOTDEFAULT(_default,2)
            GEOM_READ_FIELD(oldmat,Specular,Color3);

        IF_GEOM_NOTDEFAULT(_default,3)
            GEOM_READ_FIELD(oldmat,Emission,Color3);

        IF_GEOM_NOTDEFAULT(_default,4)
            GEOM_READ_FIELD(oldmat,Shininess,Real);

        IF_GEOM_NOTDEFAULT(_default,5)
            GEOM_READ_FIELD(oldmat,Transparency,Real);
    }

    if( version >= 1.8f){
        IF_GEOM_NOTDEFAULT(_default,6)
            GEOM_READ_FIELD(mat,Mipmaping,Bool);
    }

    if (FileName.empty()) {
        string label = "ImageTexture : " + string((_name.empty() ? "(unamed)" : _name));
        pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),label.c_str(),"FileName");
        MaterialPtr defmat(new Material());
        GEOM_PARSER_SETNAME(_name,_ident,defmat,Material);
    }
    else if (!exists(FileName.c_str())) {
        string label = "ImageTexture : " + string((_name.empty() ? "(unamed)" : _name));
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),label.c_str(),"FileName",FileName.c_str());
        MaterialPtr defmat(new Material());
        GEOM_PARSER_SETNAME(_name,_ident,defmat,Material);
    }
    else {
      GEOM_PARSER_SETNAME(_name,_ident,mat,ImageTexture);
    }

    return true;

}

/* ----------------------------------------------------------------------- */

bool BinaryParser::readTexture2DTransformation() {
    GEOM_BEGIN(_name,_ident);

    GEOM_READ_DEFAULT(_default);

    GEOM_INIT_OBJ(obj, 43, Texture2DTransformation);

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Scale,Vector2) ;

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,Translation,Vector2);

    IF_GEOM_NOTDEFAULT(_default,2)
        GEOM_READ_FIELD(obj,RotationCenter,Vector2);

    IF_GEOM_NOTDEFAULT(_default,3)
        GEOM_READ_FIELD(obj,RotationAngle,Real);

    GEOM_PARSER_SETNAME(_name,_ident,obj,ImageTexture);

    return true;

}

/* ----------------------------------------------------------------------- */


bool BinaryParser::readMonoSpectral() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(mat, 2, MonoSpectral);

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(mat,Reflectance,Real);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(mat,Transmittance,Real);

    GEOM_PARSER_SETNAME(_name,_ident,mat,MonoSpectral);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readMultiSpectral() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(mat, 3, MultiSpectral);

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(mat,Filter,Index3);

    GEOM_READ_ARRAY(mat->getReflectance(),RealArray, Real);

    GEOM_READ_ARRAY(mat->getTransmittance(),RealArray, Real);

    GEOM_PARSER_SETNAME(_name,_ident,mat,MultiSpectral);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readAmapSymbol() {
    GEOM_BEGIN(_name,_ident);
    GEOM_INIT_OBJ(obj, 4, AmapSymbol);

    string FileName = readFile();
    if(!FileName.empty()&&exists(FileName.c_str())) {
            FileName = absolute_filename(FileName);
            obj->readFile(FileName);
    }
    Point3ArrayPtr points;
    Point3ArrayPtr normals;
    Point3ArrayPtr texCoords;
    IndexArrayPtr indices;

    GEOM_READ_FIELD(obj,Solid,Bool);

    float version =  __tokens->getVersion();
    if( version >= 1.1f){
        GEOM_READ_ARRAY(points,Point3Array,Vector3);

        GEOM_READ_ARRAY(normals,Point3Array,Vector3);

        GEOM_READ_INDEXARRAY(indices);

        if( version >= 1.3f){
          GEOM_READ_ARRAY(texCoords,Point3Array,Vector3);
        }
    }

    if( version < 1.1f){
      if (FileName.empty() || !exists(FileName.c_str())) {
    string label = "AmapSymbol : " + string((_name.empty() ? "(unamed)" : _name));
    pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),label.c_str(),"FileName");
    GEOM_DEL_OBJ(obj,4) ;
    return false;
      }
      else {
      FileName = absolute_filename(FileName);
      obj->readFile(FileName);
      obj->getFileName() = FileName;
      }
    }
    else {
      obj->getPointList() = points;
      obj->getNormalList() = normals;
      obj->getTexCoord3List() = texCoords;
      obj->getIndexList() = indices;
      obj->getCCW() = false;
      real_t _maxZ = points->getZMax()->z();
      obj->getSkeleton() = PolylinePtr(new Polyline(Vector3::ORIGIN,Vector3(0,0,_maxZ)));
      if (!FileName.empty() && exists(FileName.c_str())) {
      obj->getFileName() = FileName;
      }
    }


    GEOM_PARSER_SETNAME(_name,_ident,obj,AmapSymbol);
    return true;
}

/* ----------------------------------------------------------------------- */


bool BinaryParser::readAsymmetricHull() {
    GEOM_BEGIN(_name,_ident);
    uint16_t _default;
    *stream >> _default;
    GEOM_INIT_OBJ(obj, 5, AsymmetricHull);

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,NegXRadius,Real);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,PosXRadius,Real);

    IF_GEOM_NOTDEFAULT(_default,2)
        GEOM_READ_FIELD(obj,NegYRadius,Real);

    IF_GEOM_NOTDEFAULT(_default,3)
        GEOM_READ_FIELD(obj,PosYRadius,Real);

    IF_GEOM_NOTDEFAULT(_default,4)
        GEOM_READ_FIELD(obj,NegXHeight,Real);

    IF_GEOM_NOTDEFAULT(_default,5)
        GEOM_READ_FIELD(obj,PosXHeight,Real);

    IF_GEOM_NOTDEFAULT(_default,6)
        GEOM_READ_FIELD(obj,NegYHeight,Real);

    IF_GEOM_NOTDEFAULT(_default,7)
        GEOM_READ_FIELD(obj,PosYHeight,Real);

    IF_GEOM_NOTDEFAULT(_default,8)
        GEOM_READ_FIELD(obj,Bottom,Vector3);

    IF_GEOM_NOTDEFAULT(_default,9)
        GEOM_READ_FIELD(obj,Top,Vector3);

    IF_GEOM_NOTDEFAULT(_default,10)
        GEOM_READ_FIELD(obj,BottomShape,Real);

    IF_GEOM_NOTDEFAULT(_default,11)
        GEOM_READ_FIELD(obj,TopShape,Real);

    IF_GEOM_NOTDEFAULT(_default,12)
        GEOM_READ_FIELD(obj,Slices,Uchar);

    IF_GEOM_NOTDEFAULT(_default,13)
        GEOM_READ_FIELD(obj,Stacks,Uchar);

    GEOM_PARSER_SETNAME(_name,_ident,obj,AsymmetricHull);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readAxisRotated() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 6, AxisRotated );


    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Axis,Vector3);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,Angle,Real);
#ifdef GEOM_DEBUG
    std::cerr << "Angle=" << obj->getAngle() << std::endl;
    std::cerr << "Axis="  << obj->getAxis() << std::endl;
#endif


    if(readNext())
        obj->getGeometry() = dynamic_pointer_cast<Geometry>(__result);
    if(!obj->getGeometry()){
        __outputStream << "*** PARSER: <AxisRotated : " << (_name.empty() ? "(unamed)" : _name ) << "> Geometry not valid." << endl;
        obj->getAngle() = AxisRotated::DEFAULT_ANGLE;
        obj->getAxis() = AxisRotated::DEFAULT_AXIS;
        GEOM_DEL_OBJ(obj,6) ;
        return false;
    }


    GEOM_PARSER_SETNAME(_name,_ident,obj,AxisRotated);
    return true;

}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readBezierCurve() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 7, BezierCurve);

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Stride,Uint32);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,Width,Uchar);

    GEOM_READ_ARRAY(obj->getCtrlPointList(),Point4Array,Vector4);

    GEOM_PARSER_SETNAME(_name,_ident,obj,BezierCurve);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readBezierPatch() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 8, BezierPatch);


    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,UStride,Uint32);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,VStride,Uint32);

    IF_GEOM_NOTDEFAULT(_default,2)
        GEOM_READ_FIELD(obj,CCW,Bool);

    GEOM_READ_MATRIX(obj->getCtrlPointMatrix(),Point4Matrix,Vector4);

    GEOM_PARSER_SETNAME(_name,_ident,obj,BezierPatch);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readBox() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 9, Box);

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Size,Vector3);

    GEOM_PARSER_SETNAME(_name,_ident,obj,Box);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readCone() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 10, Cone);

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Radius,Real);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,Height,Real);

    IF_GEOM_NOTDEFAULT(_default,2)
        GEOM_READ_FIELD(obj,Solid,Bool);

    IF_GEOM_NOTDEFAULT(_default,3)
        GEOM_READ_FIELD(obj,Slices,Uchar);

    GEOM_PARSER_SETNAME(_name,_ident,obj,Cone);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readCylinder() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 11, Cylinder);

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Radius,Real);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,Height,Real);

    IF_GEOM_NOTDEFAULT(_default,2)
        GEOM_READ_FIELD(obj,Solid,Bool);

    IF_GEOM_NOTDEFAULT(_default,3)
        GEOM_READ_FIELD(obj,Slices,Uchar);

    GEOM_PARSER_SETNAME(_name,_ident,obj,Cylinder);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readElevationGrid() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 12,ElevationGrid );

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,XSpacing,Real);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,YSpacing,Real);

    IF_GEOM_NOTDEFAULT(_default,2)
        GEOM_READ_FIELD(obj,CCW,Bool);

    GEOM_READ_MATRIX(obj->getHeightList(),RealArray2,Real);

    GEOM_PARSER_SETNAME(_name,_ident,obj,ElevationGrid);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readEulerRotated() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 13,EulerRotated );

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Azimuth,Real);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,Elevation,Real);

    IF_GEOM_NOTDEFAULT(_default,2)
        GEOM_READ_FIELD(obj,Roll,Real);

    if(readNext())
        obj->getGeometry() = dynamic_pointer_cast<Geometry>(__result);
    if(!obj->getGeometry()){
        __outputStream << "*** PARSER: <EulerRotated : " << (_name.empty() ? "(unamed)" : _name ) << "> Geometry not valid." << endl;
        obj->getAzimuth() = EulerRotated::DEFAULT_AZIMUTH;
        obj->getElevation() = EulerRotated::DEFAULT_ELEVATION;
        obj->getRoll() = EulerRotated::DEFAULT_ROLL;
        GEOM_DEL_OBJ(obj,13) ;
        return false;
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,EulerRotated);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readExtrudedHull() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 14,ExtrudedHull );

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,CCW,Bool);

    if(readNext())
        obj->getVertical() = dynamic_pointer_cast<Curve2D>(__result);
    if(readNext())
        obj->getHorizontal() = dynamic_pointer_cast<Curve2D>(__result);

    if((!obj->getVertical()) || (!obj->getHorizontal())){
        __outputStream << "*** PARSER: <ExtrudedHull : " << (_name.empty() ? "(unamed)" : _name ) << "> Vertical or Horizontal profile not valid." << endl;
        obj->getCCW() =  ExtrudedHull::DEFAULT_CCW;
        obj->getVertical() = Curve2DPtr();
        obj->getHorizontal() = Curve2DPtr();
        GEOM_DEL_OBJ(obj,14) ;
        return false;
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,ExtrudedHull);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readExtrusion() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 17,Extrusion );


    IF_GEOM_NOTDEFAULT(_default,0){
        GEOM_READ_FIELD(obj,Solid,Bool);
    }

    IF_GEOM_NOTDEFAULT(_default,1){
        GEOM_READ_FIELD(obj,CCW,Bool);
    }

    if(GEOM_NOTDEFAULT_COND(_default,2)||GEOM_NOTDEFAULT_COND(_default,3)||GEOM_NOTDEFAULT_COND(_default,4)){
      obj->getProfileTransformation() = ProfileTransformationPtr(new ProfileTransformation);

      IF_GEOM_NOTDEFAULT(_default,2){
        GEOM_READ_ARRAY(obj->getProfileTransformation()->getScale(),Point2Array,Vector2);
      }

      IF_GEOM_NOTDEFAULT(_default,3){
        GEOM_READ_ARRAY(obj->getProfileTransformation()->getOrientation(),RealArray,Real);
      }

      IF_GEOM_NOTDEFAULT(_default,4){
        GEOM_READ_ARRAY(obj->getProfileTransformation()->getKnotList(),RealArray,Real);
      }

    }
    if( __tokens->getVersion() >= 2.2f){
        IF_GEOM_NOTDEFAULT(_default,5){
            GEOM_READ_FIELD(obj,InitialNormal,Vector3);
        }
    }

    if(readNext()){
      obj->getAxis() = dynamic_pointer_cast<LineicModel>(__result);
    }
    else {
      __outputStream << "*** PARSER: <Extrusion : " << (_name.empty() ? "(unamed)" : _name ) << "> Axis not find." << endl;
    }

    if(readNext()){
      obj->getCrossSection() = dynamic_pointer_cast<Curve2D>(__result);
    }
    else {
      __outputStream << "*** PARSER: <Extrusion : " << (_name.empty() ? "(unamed)" : _name ) << "> CrossSection not find." << endl;
    }

    if(!(obj->getAxis()) && !(obj->getCrossSection())){
      __outputStream << "*** PARSER: <Extrusion : " << (_name.empty() ? "(unamed)" : _name ) << "> Axis and CrossSection not valid." << endl;
      obj->getSolid() = Mesh::DEFAULT_SOLID;
      obj->getCCW() = Mesh::DEFAULT_CCW;
      obj->getProfileTransformation() = ProfileTransformationPtr();
      GEOM_DEL_OBJ(obj,17) ;
      return false;
    }

    if(!(obj->getAxis())){
      __outputStream << "*** PARSER: <Extrusion : " << (_name.empty() ? "(unamed)" : _name ) << "> Axis not valid." << endl;
      obj->getSolid() = Mesh::DEFAULT_SOLID;
      obj->getCCW() = Mesh::DEFAULT_CCW;
      obj->getProfileTransformation() = ProfileTransformationPtr();
      GEOM_DEL_OBJ(obj,17) ;
      return false;
    }

    if(!(obj->getCrossSection())){
      __outputStream << "*** PARSER: <Extrusion : " << (_name.empty() ? "(unamed)" : _name ) << "> CrossSection not valid." << endl;
      obj->getSolid() = Mesh::DEFAULT_SOLID;
      obj->getCCW() = Mesh::DEFAULT_CCW;
      obj->getProfileTransformation() = ProfileTransformationPtr();
      GEOM_DEL_OBJ(obj,17) ;
      return false;
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,Extrusion);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readFaceSet() {
    GEOM_BEGIN(_name,_ident);

    float version =  __tokens->getVersion();
    uint16_t _default;
    if( version >= 1.5f)
        *stream >> _default;
    else {
        uchar_t _ldefault;
        *stream >> _ldefault;
        _default = _ldefault;
    }

    GEOM_INIT_OBJ(obj, 15,FaceSet );

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,CCW,Bool);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,Solid,Bool);

    IF_GEOM_NOTDEFAULT(_default,2)
      if(readNext()){
        obj->getSkeleton() = dynamic_pointer_cast<Polyline>(__result);
        if(!obj->getSkeleton()){
          __outputStream << "*** PARSER: <FaceSet : " << (_name.empty() ? "(unamed)" : _name ) << "> Skeleton not valid." << endl;
        }
      }

    GEOM_READ_ARRAY(obj->getPointList(),Point3Array,Vector3);

    GEOM_READ_INDEXARRAY(obj->getIndexList());

    if( version >= 1.2f){
        if( version >= 1.5f){
            IF_GEOM_NOTDEFAULT(_default,4){
                 GEOM_READ_FIELD(obj,NormalPerVertex,Bool);
            }
            else obj->getNormalPerVertex() = (GEOM_DEFAULT_COND(_default,4)?Mesh::DEFAULT_NORMALPERVERTEX:!Mesh::DEFAULT_NORMALPERVERTEX);
        }
        else obj->getNormalPerVertex() = (GEOM_DEFAULT_COND(_default,4)?Mesh::DEFAULT_NORMALPERVERTEX:!Mesh::DEFAULT_NORMALPERVERTEX);

        IF_GEOM_NOTDEFAULT(_default,3)
            GEOM_READ_ARRAY(obj->getNormalList(),Point3Array,Vector3);

        if( version >= 1.5f){
            IF_GEOM_NOTDEFAULT(_default,6)
                GEOM_READ_INDEXARRAY(obj->getNormalIndexList());
        }
    }
    if( version >= 1.5f){
        IF_GEOM_NOTDEFAULT(_default,7)
            GEOM_READ_FIELD(obj,ColorPerVertex,Bool);

        IF_GEOM_NOTDEFAULT(_default,8)
            GEOM_READ_ARRAY(obj->getColorList(),Color4Array,Color4);

        IF_GEOM_NOTDEFAULT(_default,9)
            GEOM_READ_INDEXARRAY(obj->getColorIndexList());
    }

    if( version >= 1.3f){
        IF_GEOM_NOTDEFAULT(_default,5)
            GEOM_READ_ARRAY(obj->getTexCoordList(),Point2Array,Vector2);

        if( version >= 1.5f){
            IF_GEOM_NOTDEFAULT(_default,10)
                GEOM_READ_INDEXARRAY(obj->getTexCoordIndexList());
        }
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,FaceSet);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readFrustum() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 16,Frustum );


    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Radius,Real);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,Height,Real);

    IF_GEOM_NOTDEFAULT(_default,2)
        GEOM_READ_FIELD(obj,Taper,Real);

    IF_GEOM_NOTDEFAULT(_default,3)
        GEOM_READ_FIELD(obj,Solid,Bool);

    IF_GEOM_NOTDEFAULT(_default,4)
        GEOM_READ_FIELD(obj,Slices,Uchar);

    GEOM_PARSER_SETNAME(_name,_ident,obj,Frustum);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readGroup() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 18,Group );

    GeometryArrayPtr GeometryList;

    IF_GEOM_NOTDEFAULT(_default,0)
      if(readNext()){
        obj->getSkeleton() = dynamic_pointer_cast<Polyline>(__result);
        if(!obj->getSkeleton()){
          __outputStream << "*** PARSER: <Group : " << (_name.empty() ? "(unamed)" : _name ) << "> Skeleton not valid." << endl;
        }
      }


    uint_t _sizej;
    *stream >> _sizej;
    obj->getGeometryList()= GeometryArrayPtr(new GeometryArray(_sizej));
    uint_t err = 0;
    GeometryArray::iterator _it = obj->getGeometryList()->begin();

    for (uint_t num = 0;
         num < _sizej &&
         _it != obj->getGeometryList()->end() &&
         !stream->eof(); num++) {
        if(readNext())
            *_it  = dynamic_pointer_cast<Geometry>(__result);
        if((*_it))_it++;
        else {
            err++;
            __outputStream << "*** PARSER: <Group : " << (_name.empty() ? "(unamed)" : _name ) << "> A Geometry component is not valid." << endl;
        }
    }
    if(err > 0 && err >= _sizej-1){
      obj->getSkeleton() = PolylinePtr();
      obj->getGeometryList()= GeometryArrayPtr();
      GEOM_DEL_OBJ(obj,18) ;
      return false;
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,Group);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readIFS() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ( obj, 38, IFS );

//todo OK
    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Depth,Uchar);

    uint_t size;
    *stream >> size;
    obj->getTransfoList()= Transform4ArrayPtr(new Transform4Array(size));

    uint_t err= 0;
    Transform4Array::iterator _ti= obj->getTransfoList()->begin();
    Transform4Array::iterator _tend= obj->getTransfoList()->end();
    for( uint_t num = 0;
         num < size && _ti != _tend && !stream->eof();
         num++ )
      {
      Vector4 col[4];
      for( uchar_t i= 0; i < 4; i++ )
        *stream >> col[i];
      Matrix4 m(col[0],col[1],col[2],col[3]);
      if( m.isValid() )
        {
        (*_ti)= Transform4Ptr( new Transform4( m ));
        _ti++;
        }
      else
        {
        err++;
        __outputStream << "*** PARSER: <IFS : " << (_name.empty() ? "(unamed)" : _name ) << "> A Transfo component is not valid." << endl;
        }
      }

    if(readNext())
        obj->getGeometry() = dynamic_pointer_cast<Geometry>(__result);

    if( err >= size-1 || (!obj->getGeometry()) )
      {
      obj->getDepth()= IFS::DEFAULT_DEPTH;
      obj->getTransfoList()= Transform4ArrayPtr();
      GEOM_DEL_OBJ(obj,38) ;
      }

    GEOM_PARSER_SETNAME(_name,_ident,obj,IFS);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readNurbsCurve() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 19,NurbsCurve );

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Degree,Uint32);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_ARRAY(obj->getKnotList(),RealArray,Real);

    IF_GEOM_NOTDEFAULT(_default,2)
        GEOM_READ_FIELD(obj,Stride,Uint32);

    IF_GEOM_NOTDEFAULT(_default,3)
        GEOM_READ_FIELD(obj,Width,Uchar);

    GEOM_READ_ARRAY(obj->getCtrlPointList(),Point4Array,Vector4);

    if(!GEOM_NOTDEFAULT_COND(_default,1))
        obj->setKnotListToDefault();

    GEOM_PARSER_SETNAME(_name,_ident,obj,NurbsCurve);
    return true;
}

/* ----------------------------------------------------------------------- */


bool BinaryParser::readNurbsPatch() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 20,NurbsPatch );


    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,UDegree,Uint32);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,VDegree,Uint32);

    IF_GEOM_NOTDEFAULT(_default,2)
        GEOM_READ_ARRAY(obj->getUKnotList(),RealArray,Real);

    IF_GEOM_NOTDEFAULT(_default,3)
        GEOM_READ_ARRAY(obj->getVKnotList(),RealArray,Real);

    IF_GEOM_NOTDEFAULT(_default,4)
        GEOM_READ_FIELD(obj,UStride,Uint32);

    IF_GEOM_NOTDEFAULT(_default,5)
        GEOM_READ_FIELD(obj,VStride,Uint32);

    IF_GEOM_NOTDEFAULT(_default,6)
        GEOM_READ_FIELD(obj,CCW,Bool);

    GEOM_READ_MATRIX(obj->getCtrlPointMatrix(),Point4Matrix,Vector4);

    if(!GEOM_NOTDEFAULT_COND(_default,2))
        obj->setUKnotListToDefault();

    if(!GEOM_NOTDEFAULT_COND(_default,3))
        obj->setVKnotListToDefault();

    GEOM_PARSER_SETNAME(_name,_ident,obj,NurbsPatch);
    return true;
}

/* ----------------------------------------------------------------------- */


bool BinaryParser::readOriented() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 21,Oriented );

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Primary, Vector3);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,Secondary, Vector3);

    if(readNext())
        obj->getGeometry() = dynamic_pointer_cast<Geometry>(__result);
    if(!obj->getGeometry()){
      __outputStream << "*** PARSER: <Oriented : " << (_name.empty() ? "(unamed)" : _name ) << "> Geometry not valid." << endl;
      obj->getPrimary() = Oriented::DEFAULT_PRIMARY;
      obj->getSecondary() = Oriented::DEFAULT_SECONDARY;
      GEOM_DEL_OBJ(obj,21) ;
      return false;
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,Oriented);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readParaboloid() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 22,Paraboloid );


    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Radius, Real);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,Height, Real);

    IF_GEOM_NOTDEFAULT(_default,2)
        GEOM_READ_FIELD(obj,Shape, Real);

    IF_GEOM_NOTDEFAULT(_default,3)
        GEOM_READ_FIELD(obj,Solid, Bool);

    IF_GEOM_NOTDEFAULT(_default,4)
        GEOM_READ_FIELD(obj,Slices, Uchar);

    float version =  __tokens->getVersion();
    if( version >= 1.6f){
        IF_GEOM_NOTDEFAULT(_default,5)
            GEOM_READ_FIELD(obj,Stacks, Uchar);
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,Paraboloid);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readPointSet() {
    GEOM_BEGIN(_name,_ident);
    GEOM_INIT_OBJ(obj, 23,PointSet );

    GEOM_READ_ARRAY(obj->getPointList(),Point3Array,Vector3);

    float version =  __tokens->getVersion();
    if( version >= 1.7f){
        GEOM_READ_ARRAY(obj->getColorList(),Color4Array,Color4);
    }
    if( version >= 2.1f){
        GEOM_READ_FIELD(obj,Width,Uchar);
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,PointSet);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readPolyline() {
    GEOM_BEGIN(_name,_ident);
    GEOM_INIT_OBJ(obj, 24,Polyline );

    GEOM_READ_ARRAY(obj->getPointList(),Point3Array,Vector3);

    float version =  __tokens->getVersion();
    if( version >= 1.7f){
        GEOM_READ_ARRAY(obj->getColorList(),Color4Array,Color4);
    }

    if( version >= 2.1f){
        GEOM_READ_FIELD(obj,Width,Uchar);
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,Polyline);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readQuadSet() {
    GEOM_BEGIN(_name,_ident);
    float version =  __tokens->getVersion();
    uint16_t _default;
    if( version >= 1.5f)
        *stream >> _default;
    else {
        uchar_t _ldefault;
        *stream >> _ldefault;
        _default = _ldefault;
    }

    GEOM_INIT_OBJ(obj, 25,QuadSet);

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,CCW,Bool);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,Solid,Bool);

    IF_GEOM_NOTDEFAULT(_default,2)
      if(readNext()){
        obj->getSkeleton() = dynamic_pointer_cast<Polyline>(__result);
        if(!obj->getSkeleton()){
          __outputStream << "*** PARSER: <QuadSet : " << (_name.empty() ? "(unamed)" : _name ) << "> Skeleton not valid." << endl;
        }
      }

    GEOM_READ_ARRAY(obj->getPointList(),Point3Array,Vector3);

    GEOM_READ_ARRAY(obj->getIndexList(),Index4Array,Index4);

    if( version >= 1.2f){
        if( version >= 1.5f){
            IF_GEOM_NOTDEFAULT(_default,4){
                GEOM_READ_FIELD(obj,NormalPerVertex,Bool);
            }
            else obj->getNormalPerVertex() = (GEOM_DEFAULT_COND(_default,4)?!Mesh::DEFAULT_NORMALPERVERTEX:Mesh::DEFAULT_NORMALPERVERTEX);
        }
        else obj->getNormalPerVertex() = (GEOM_DEFAULT_COND(_default,4)?!Mesh::DEFAULT_NORMALPERVERTEX:Mesh::DEFAULT_NORMALPERVERTEX);

        IF_GEOM_NOTDEFAULT(_default,3)
            GEOM_READ_ARRAY(obj->getNormalList(),Point3Array,Vector3);

        if( version >= 1.5f){
            IF_GEOM_NOTDEFAULT(_default,6)
                GEOM_READ_ARRAY(obj->getNormalIndexList(),Index4Array,Index4);
        }
    }
    if( version >= 1.5f){
        IF_GEOM_NOTDEFAULT(_default,7)
            GEOM_READ_FIELD(obj,ColorPerVertex,Bool);

        IF_GEOM_NOTDEFAULT(_default,8)
            GEOM_READ_ARRAY(obj->getColorList(),Color4Array,Color4);

        IF_GEOM_NOTDEFAULT(_default,9)
            GEOM_READ_ARRAY(obj->getColorIndexList(),Index4Array,Index4);
    }

    if( version >= 1.3f){
        IF_GEOM_NOTDEFAULT(_default,5)
            GEOM_READ_ARRAY(obj->getTexCoordList(),Point2Array,Vector2);

        if( version >= 1.5f){
            IF_GEOM_NOTDEFAULT(_default,10)
                GEOM_READ_ARRAY(obj->getTexCoordIndexList(),Index4Array,Index4);
        }
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,QuadSet);
    return true;

}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readRevolution() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 26,Revolution);

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Slices,Uchar);

    if(readNext())
        obj->getProfile() = dynamic_pointer_cast<Curve2D>(__result);
    if(!obj->getProfile()){
      __outputStream << "*** PARSER: <Revolution : " << (_name.empty() ? "(unamed)" : _name ) << "> Profile not valid." << endl;
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,Revolution);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readSwung()
{
  GEOM_BEGIN(_name,_ident);
  // std::cerr << "Pos : " << stream->getStream().tellg() << std::endl;

  GEOM_READ_DEFAULT(_default);
#ifdef GEOM_DEBUG
  std::cerr << "Default : " << int(_default) << std::endl;
#endif
  // GEOM_INIT_OBJ(obj, 37,Swung);

  uchar_t slices = Swung::DEFAULT_SLICES;
  IF_GEOM_NOTDEFAULT(_default,0){
    slices = readUchar();
#ifdef GEOM_DEBUG
    std::cerr << "Read slices : " << int(slices) << std::endl;
#endif
  }

  bool ccw = Swung::DEFAULT_CCW;
  IF_GEOM_NOTDEFAULT(_default,1){
      ccw = readBool();
#ifdef GEOM_DEBUG
      std::cerr << "Read CCW : " << (ccw?"True":"False") << std::endl;
#endif
  }

  uint_t degree = Swung::DEFAULT_DEGREE;
  IF_GEOM_NOTDEFAULT(_default,2){
    degree = readUint32();
#ifdef GEOM_DEBUG
    std::cerr << "Read Degree : " << degree << std::endl;
#endif
  }

  uint_t stride = Swung::DEFAULT_STRIDE;
  IF_GEOM_NOTDEFAULT(_default,3){
    stride = readUint32();
#ifdef GEOM_DEBUG
    std::cerr << "Read Stride : " << stride << std::endl;
#endif
  }

  RealArrayPtr angles;
  GEOM_READ_ARRAY(angles,RealArray,Real);

  // we try to read the Curve2D List
  uint_t size = readUint32();
#ifdef GEOM_DEBUG
  cerr << "must find " << size << " curves" << endl;
#endif
  Curve2DArrayPtr curves(new Curve2DArray(size));

  Curve2DArray::iterator _it = curves->begin();
  Curve2DArray::iterator _itEnd = curves->end();
  uint_t i= 0;
  uint_t err = 0;
  for( i= 0 ; i < size && _it != _itEnd && !stream->eof(); i++ )
    {
    if( readNext() )
      *_it  = dynamic_pointer_cast<Curve2D>(__result);
    if( (*_it) )
      _it++;
    else
      {
          err++;
          __outputStream << "*** PARSER: <Swung : "
                         << ( _name.empty() ? "(unamed)" : _name )
                         << "> A Curve2D component is not valid." << endl;
          if(__result){
            SceneObject& res = *__result;
            __outputStream << "*** PARSER: <Swung : "
                           << ( _name.empty() ? "(unamed)" : _name )
                           << "> Found " << typeid(res).name() << " instead." << endl;
          }
      }
    }

  if( err >= size ) {
    __result = NULLPTR;
    return false;
  }
  else if( err > 0 ){
    curves = Curve2DArrayPtr(new Curve2DArray(curves->begin(),_it));
  }

  Swung * obj = new Swung(curves,angles,slices,ccw,degree,stride);

  GEOM_PARSER_SETNAME(_name,_ident,obj,Swung);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readScaled() {
    GEOM_BEGIN(_name,_ident);
    GEOM_INIT_OBJ(obj, 27,Scaled);

    GEOM_READ_FIELD(obj,Scale,Vector3);

    if(readNext())
        obj->getGeometry() = dynamic_pointer_cast<Geometry>(__result);
    if(!obj->getGeometry()){
        __outputStream << "*** PARSER: <Scaled : " << (_name.empty() ? "(unamed)" : _name ) << "> Geometry not valid." << endl;
        GEOM_DEL_OBJ(obj,27) ;
        return false;
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,Scaled);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readScreenProjected() {
    GEOM_BEGIN(_name,_ident);
    GEOM_INIT_OBJ(obj, 44,ScreenProjected);

    GEOM_READ_FIELD(obj,KeepAspectRatio,Bool);

    if(readNext())
        obj->getGeometry() = dynamic_pointer_cast<Geometry>(__result);
    if(!obj->getGeometry()){
        __outputStream << "*** PARSER: <ScreenProjected : " << (_name.empty() ? "(unamed)" : _name ) << "> Geometry not valid." << endl;
        GEOM_DEL_OBJ(obj,44) ;
        return false;
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,ScreenProjected);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readSphere() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 28,Sphere);

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Radius,Real);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,Slices,Uchar);

    IF_GEOM_NOTDEFAULT(_default,2)
        GEOM_READ_FIELD(obj,Stacks,Uchar);

    GEOM_PARSER_SETNAME(_name,_ident,obj,Sphere);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readTapered() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 29,Tapered);

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,BaseRadius,Real);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,TopRadius,Real);

    if(readNext())
        obj->getPrimitive() = dynamic_pointer_cast<Primitive>(__result);
    if(!obj->getPrimitive()){
        __outputStream << "*** PARSER: <Tapered : " << (_name.empty() ? "(unamed)" : _name ) << "> Geometry not valid." << endl;
        obj->getBaseRadius() = Tapered::DEFAULT_BASE_RADIUS;
        obj->getTopRadius() = Tapered::DEFAULT_TOP_RADIUS;
        GEOM_DEL_OBJ(obj,29) ;
        return false;
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,Tapered);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readTranslated() {
    GEOM_BEGIN(_name,_ident);
    GEOM_INIT_OBJ(obj, 30,Translated);

    GEOM_READ_FIELD(obj,Translation,Vector3);



    if(readNext()){
        obj->getGeometry() = dynamic_pointer_cast<Geometry>(__result);
    }
    if(!obj->getGeometry()){
        __outputStream << "*** PARSER: <Translated : " << (_name.empty() ? "(unamed)" : _name ) << "> Geometry not valid." << endl;
        GEOM_DEL_OBJ(obj,30) ;
        return false;
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,Translated);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readTriangleSet() {
    GEOM_BEGIN(_name,_ident);

    float version =  __tokens->getVersion();
    uint16_t _default;
    if( version >= 1.5f)
        *stream >> _default;
    else {
        uchar_t _ldefault;
        *stream >> _ldefault;
        _default = _ldefault;
    }

    GEOM_INIT_OBJ(obj, 31,TriangleSet);

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,CCW,Bool);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,Solid,Bool);

    IF_GEOM_NOTDEFAULT(_default,2)
      if(readNext()){
        obj->getSkeleton() = dynamic_pointer_cast<Polyline>(__result);
        if(!obj->getSkeleton())
          __outputStream << "*** PARSER: <TriangleSet : " << (_name.empty() ? "(unamed)" : _name ) << "> Skeleton not valid." << endl;
      }

    GEOM_READ_ARRAY(obj->getPointList(),Point3Array,Vector3);

    GEOM_READ_ARRAY(obj->getIndexList(),Index3Array,Index3);

    if( version >= 1.2f){
        if( version >= 1.5f){
            IF_GEOM_NOTDEFAULT(_default,4){
                 GEOM_READ_FIELD(obj,NormalPerVertex,Bool);
            }
            else obj->getNormalPerVertex() = (GEOM_DEFAULT_COND(_default,4)?Mesh::DEFAULT_NORMALPERVERTEX:!Mesh::DEFAULT_NORMALPERVERTEX);
        }
        else obj->getNormalPerVertex() = (GEOM_DEFAULT_COND(_default,4)?Mesh::DEFAULT_NORMALPERVERTEX:!Mesh::DEFAULT_NORMALPERVERTEX);

        IF_GEOM_NOTDEFAULT(_default,3)
            GEOM_READ_ARRAY(obj->getNormalList(),Point3Array,Vector3);

        if( version >= 1.5f){
            IF_GEOM_NOTDEFAULT(_default,6)
                GEOM_READ_ARRAY(obj->getNormalIndexList(),Index3Array,Index3);
        }
    }

    if( version >= 1.5f){
        IF_GEOM_NOTDEFAULT(_default,7)
            GEOM_READ_FIELD(obj,ColorPerVertex,Bool);

        IF_GEOM_NOTDEFAULT(_default,8)
            GEOM_READ_ARRAY(obj->getColorList(),Color4Array,Color4);

        IF_GEOM_NOTDEFAULT(_default,9)
            GEOM_READ_ARRAY(obj->getColorIndexList(),Index3Array,Index3);
    }

    if( version >= 1.3f){
        IF_GEOM_NOTDEFAULT(_default,5)
            GEOM_READ_ARRAY(obj->getTexCoordList(),Point2Array,Vector2);

        if( version >= 1.5f){
            IF_GEOM_NOTDEFAULT(_default,10)
                GEOM_READ_ARRAY(obj->getTexCoordIndexList(),Index3Array,Index3);
        }
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,TriangleSet);
    return true;
}


/* ------------------------------------------------------------------------- */


bool BinaryParser::readBezierCurve2D() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 32,BezierCurve2D);

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Stride,Uint32);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,Width,Uchar);

    GEOM_READ_ARRAY(obj->getCtrlPointList(),Point3Array,Vector3);

    GEOM_PARSER_SETNAME(_name,_ident,obj,BezierCurve2D);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readDisc() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 33,Disc);

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Radius,Real);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,Slices,Uchar);

    GEOM_PARSER_SETNAME(_name,_ident,obj,Disc);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readNurbsCurve2D() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 34,NurbsCurve2D );

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Degree,Uint32);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_ARRAY(obj->getKnotList(),RealArray,Real);

    IF_GEOM_NOTDEFAULT(_default,2)
        GEOM_READ_FIELD(obj,Stride,Uint32);

    IF_GEOM_NOTDEFAULT(_default,3)
        GEOM_READ_FIELD(obj,Width,Uchar);

    GEOM_READ_ARRAY(obj->getCtrlPointList(),Point3Array,Vector3);

    if(!GEOM_NOTDEFAULT_COND(_default,1))
        obj->setKnotListToDefault();

    GEOM_PARSER_SETNAME(_name,_ident,obj,NurbsCurve2D);
    return true;
}

/* ----------------------------------------------------------------------- */


bool BinaryParser::readPointSet2D() {
    GEOM_BEGIN(_name,_ident);
    GEOM_INIT_OBJ(obj, 35,PointSet2D);

    GEOM_READ_ARRAY(obj->getPointList(),Point2Array,Vector2);

    float version =  __tokens->getVersion();
    if( version >= 2.1f){
        GEOM_READ_FIELD(obj,Width,Uchar);
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,PointSet2D);
    return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryParser::readPolyline2D() {
    GEOM_BEGIN(_name,_ident);
    GEOM_INIT_OBJ(obj, 36,Polyline2D);

    GEOM_READ_ARRAY(obj->getPointList(),Point2Array,Vector2);

    float version =  __tokens->getVersion();
    if( version >= 2.1f){
        GEOM_READ_FIELD(obj,Width,Uchar);
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,Polyline2D);
    return true;
}


/* ----------------------------------------------------------------------- */

bool BinaryParser::readText() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 40, Text);

    GEOM_READ_FIELD(obj,String,String);
    std::cerr << "String :\"" << obj->getString() << '"' << std::endl;

    IF_GEOM_NOTDEFAULT(_default,0){
      if(readNext()){
        obj->getFontStyle() = dynamic_pointer_cast<Font>(__result);
        if(!obj->getFontStyle())
          __outputStream << "*** PARSER: <Text : " << (_name.empty() ? "(unamed)" : _name ) << "> font not valid." << endl;
      }
    }

    float version =  __tokens->getVersion();
    if( version >= 1.7f){
        IF_GEOM_NOTDEFAULT(_default,1){
            GEOM_READ_FIELD(obj,Position,Vector3);
        }
        IF_GEOM_NOTDEFAULT(_default,2){
            GEOM_READ_FIELD(obj,ScreenCoordinates,Bool);
        }
    }

    GEOM_PARSER_SETNAME(_name,_ident,obj,Text);
    return true;
}

/* ----------------------------------------------------------------------- */

bool BinaryParser::readFont() {
    GEOM_BEGIN(_name,_ident);
    GEOM_READ_DEFAULT(_default);
    GEOM_INIT_OBJ(obj, 40, Font);

    GEOM_READ_FIELD(obj,Family,String);

    IF_GEOM_NOTDEFAULT(_default,0)
        GEOM_READ_FIELD(obj,Size,Uint32);

    IF_GEOM_NOTDEFAULT(_default,1)
        GEOM_READ_FIELD(obj,Bold,Bool);

    IF_GEOM_NOTDEFAULT(_default,2)
        GEOM_READ_FIELD(obj,Italic,Bool);

    GEOM_PARSER_SETNAME(_name,_ident,obj,Font);
    return true;
}

/* ----------------------------------------------------------------------- */
