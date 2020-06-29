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



#include <plantgl/tool/dirnames.h>
#include <plantgl/tool/bfstream.h>
#include <plantgl/tool/util_enviro.h>


#include "binaryprinter.h"

#include <plantgl/pgl_scene.h>
#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>

#include <plantgl/pgl_container.h>

#include <plantgl/algo/base/statisticcomputer.h>

PGL_USING_NAMESPACE

using namespace std;
using namespace STDEXT;

// #define GEOM_DEBUG


/* ----------------------------------------------------------------------- */

TokenCode::TokenCode(const float version ):
  __version(version) // ,
//  __code(44)
{
  __code[uchar_t(1)] = pair<string,uint_t>(string("Reference"),0);
  __code[uchar_t(2)] = pair<string,uint_t>(string("Shape"),0);
  __code[uchar_t(3)] = pair<string,uint_t>(string("Material"),0);
  __code[uchar_t(4)] = pair<string,uint_t>(string("MonoSpectral"),0);
  __code[uchar_t(5)] = pair<string,uint_t>(string("MultiSpectral"),0);
  __code[uchar_t(6)] = pair<string,uint_t>(string("AmapSymbol"),0);
  __code[uchar_t(7)] = pair<string,uint_t>(string("AsymmetricHull"),0);
  __code[uchar_t(8)] = pair<string,uint_t>(string("AxisRotated"),0);
  __code[uchar_t(9)] = pair<string,uint_t>(string("BezierCurve"),0);
  __code[uchar_t(10)] = pair<string,uint_t>(string("BezierPatch"),0);
  __code[uchar_t(11)] = pair<string,uint_t>(string("Box"),0);
  __code[uchar_t(12)] = pair<string,uint_t>(string("Cone"),0);
  __code[uchar_t(13)] = pair<string,uint_t>(string("Cylinder"),0);
  __code[uchar_t(14)] = pair<string,uint_t>(string("ElevationGrid"),0);
  __code[uchar_t(15)] = pair<string,uint_t>(string("EulerRotated"),0);
  __code[uchar_t(16)] = pair<string,uint_t>(string("ExtrudedHull"),0);
  __code[uchar_t(17)] = pair<string,uint_t>(string("FaceSet"),0);
  __code[uchar_t(18)] = pair<string,uint_t>(string("Frustum"),0);
  __code[uchar_t(19)] = pair<string,uint_t>(string("Extrusion"),0);
  __code[uchar_t(20)] = pair<string,uint_t>(string("Group"),0);
  __code[uchar_t(21)] = pair<string,uint_t>(string("NurbsCurve"),0);
  __code[uchar_t(22)] = pair<string,uint_t>(string("NurbsPatch"),0);
  __code[uchar_t(23)] = pair<string,uint_t>(string("Oriented"),0);
  __code[uchar_t(24)] = pair<string,uint_t>(string("Paraboloid"),0);
  __code[uchar_t(25)] = pair<string,uint_t>(string("PointSet"),0);
  __code[uchar_t(26)] = pair<string,uint_t>(string("Polyline"),0);
  __code[uchar_t(27)] = pair<string,uint_t>(string("QuadSet"),0);
  __code[uchar_t(28)] = pair<string,uint_t>(string("Revolution"),0);
  __code[uchar_t(29)] = pair<string,uint_t>(string("Scaled"),0);
  __code[uchar_t(30)] = pair<string,uint_t>(string("Sphere"),0);
  __code[uchar_t(31)] = pair<string,uint_t>(string("Tapered"),0);
  __code[uchar_t(32)] = pair<string,uint_t>(string("Translated"),0);
  __code[uchar_t(33)] = pair<string,uint_t>(string("TriangleSet"),0);
  __code[uchar_t(34)] = pair<string,uint_t>(string("BezierCurve2D"),0);
  __code[uchar_t(35)] = pair<string,uint_t>(string("Disc"),0);
  __code[uchar_t(36)] = pair<string,uint_t>(string("NurbsCurve2D"),0);
  __code[uchar_t(37)] = pair<string,uint_t>(string("PointSet2D"),0);
  __code[uchar_t(38)] = pair<string,uint_t>(string("Polyline2D"),0);
  __code[uchar_t(39)] = pair<string,uint_t>(string("Swung"),0);
  __code[uchar_t(40)] = pair<string,uint_t>(string("IFS"),0);
  __code[uchar_t(41)] = pair<string,uint_t>(string("ImageTexture"),0);
  __code[uchar_t(42)] = pair<string,uint_t>(string("Text"),0);
  __code[uchar_t(43)] = pair<string,uint_t>(string("Font"),0);
  __code[uchar_t(44)] = pair<string,uint_t>(string("Texture2D"),0);
  __code[uchar_t(45)] = pair<string,uint_t>(string("Texture2DTransformation"),0);
  __code[uchar_t(46)] = pair<string,uint_t>(string("ScreenProjected"),0);
}

TokenCode::~TokenCode(){
}

const float
TokenCode::getVersion(){
  return __version;
}

/* ----------------------------------------------------------------------- */

bool TokenCode::setStatistic(const StatisticComputer& a){
    const vector<uint_t>& el = a.getElements();
    for(uchar_t i = 2; i < __code.size() ; i++ ){
        __code[i].second = el[int(i)-2];
    }
    return true;
}


vector<uint_t> TokenCode::getCounts(){
    vector<uint_t> counts((unsigned int)45,0);
    for(pgl_hash_map<uchar_t,pair<string,uint_t> >::iterator _it = __code.begin();
        _it != __code.end();_it++){
        if(_it->second.first == "Shape") counts[0]+=_it->second.second;
        else if(_it->second.first =="Material" ) counts[1]+=_it->second.second;
        else if(_it->second.first =="MonoSpectral" ) counts[2]+=_it->second.second;
        else if(_it->second.first =="MultiSpectral" ) counts[3]+=_it->second.second;
        else if(_it->second.first =="AmapSymbol" ) counts[4]+=_it->second.second;
        else if(_it->second.first =="AsymmetricHull" ) counts[5]+=_it->second.second;
        else if(_it->second.first =="AxisRotated" ) counts[6]+=_it->second.second;
        else if(_it->second.first =="BezierCurve" ) counts[7]+=_it->second.second;
        else if(_it->second.first =="BezierPatch" ) counts[8]+=_it->second.second;
        else if(_it->second.first =="Box" ) counts[9]+=_it->second.second;
        else if(_it->second.first =="Cone" ) counts[10]+=_it->second.second;
        else if(_it->second.first =="Cylinder" ) counts[11]+=_it->second.second;
        else if(_it->second.first =="ElevationGrid" ) counts[12]+=_it->second.second;
        else if(_it->second.first =="EulerRotated" ) counts[13]+=_it->second.second;
        else if(_it->second.first =="ExtrudedHull" ) counts[14]+=_it->second.second;
        else if(_it->second.first =="FaceSet" ) counts[15]+=_it->second.second;
        else if(_it->second.first =="Frustum" ) counts[16]+=_it->second.second;
        else if(_it->second.first =="Extrusion" ) counts[17]+=_it->second.second;
        else if(_it->second.first =="Group" ) counts[18]+=_it->second.second;
        else if(_it->second.first =="NurbsCurve" ) counts[19]+=_it->second.second;
        else if(_it->second.first =="NurbsPatch" ) counts[20]+=_it->second.second;
        else if(_it->second.first =="Oriented" ) counts[21]+=_it->second.second;
        else if(_it->second.first == "Paraboloid") counts[22]+=_it->second.second;
        else if(_it->second.first == "PointSet") counts[23]+=_it->second.second;
        else if(_it->second.first == "Polyline") counts[24]+=_it->second.second;
        else if(_it->second.first == "QuadSet") counts[25]+=_it->second.second;
        else if(_it->second.first == "Revolution") counts[26]+=_it->second.second;
        else if(_it->second.first == "Scaled") counts[27]+=_it->second.second;
        else if(_it->second.first == "Sphere") counts[28]+=_it->second.second;
        else if(_it->second.first == "Tapered") counts[29]+=_it->second.second;
        else if(_it->second.first == "Translated") counts[30]+=_it->second.second;
        else if(_it->second.first == "TriangleSet") counts[31]+=_it->second.second;
        else if(_it->second.first == "BezierCurve2D") counts[32]+=_it->second.second;
        else if(_it->second.first == "Disc") counts[33]+=_it->second.second;
        else if(_it->second.first == "NurbsCurve2D") counts[34]+=_it->second.second;
        else if(_it->second.first == "PointSet2D") counts[35]+=_it->second.second;
        else if(_it->second.first == "Polyline2D") counts[36]+=_it->second.second;
        else if(_it->second.first == "Swung") counts[37]+=_it->second.second;
        else if(_it->second.first == "IFS") counts[38]+=_it->second.second;
        else if(_it->second.first == "ImageTexture" ) counts[39]+=_it->second.second;
        else if(_it->second.first == "Text" ) counts[40]+=_it->second.second;
        else if(_it->second.first == "Font" ) counts[41]+=_it->second.second;
        else if(_it->second.first == "Texture2D" ) counts[42]+=_it->second.second;
        else if(_it->second.first == "Texture2DTransformation" ) counts[43]+=_it->second.second;
        else if(_it->second.first == "ScreenProjected" ) counts[44]+=_it->second.second;
    }
    return counts;
}
/* ----------------------------------------------------------------------- */

fostream& TokenCode::printCurrentToken(fostream& stream,string token){
  pgl_hash_map<uchar_t,pair<string,uint_t> >::iterator _it = __code.begin();
  for(;_it != __code.end() && _it->second.first!=token;_it++);
  if(_it!=__code.end()){
    stream << _it->first;
#ifdef GEOM_DEBUG
    cerr << "Token : " << (int)_it->first  << " for " << token << endl;;
#endif
  }
  else {
#ifdef GEOM_DEBUG
      cerr << "Token : 0 [Error] for " << token << endl;;
#endif
      stream << uchar_t(0);
  }
  return stream;
}


/* ----------------------------------------------------------------------- */

fostream& TokenCode::printAll(fostream& stream){
  stream << '!';
  pgl_hash_map<uchar_t,pair<string,uint_t> >::iterator _it = __code.begin();
  uint_t size(0);
  uchar_t _maxcode = 0;
  for(;_it != __code.end();_it++){
    if(_it->second.second !=0)size++;
    if(_it->first > _maxcode)_maxcode = _it->first;
  }
  stream << size ;
#ifdef GEOM_DEBUG
  cerr << "Print size : " << size << endl;
#endif
  _it = __code.begin();
  for(uchar_t _itVal = 0;_itVal < _maxcode;_itVal++){
      _it = __code.find(_itVal);
      if(_it != __code.end() &&_it->second.second !=0){
          stream << _it->first;
          string label = _it->second.first;
          stream << (uint16_t)(label.length());
          stream << label;
          stream << _it->second.second;
#ifdef GEOM_DEBUG
          cerr << "Token : " << (int)_it->first<< " - " <<_it->second.first  << " (" << _it->second.second << ')' << endl;;
#endif
      }
  }
  stream << '!';
  return stream;
}

/* ----------------------------------------------------------------------- */

string TokenCode::readCurrentToken(fistream& stream){
    if(stream.eof())return string("EOF");
    uchar_t c;
    stream >> c;
    if(stream.eof())return string("EOF");
    pgl_hash_map<uchar_t,pair<string,uint_t> >::iterator _it = __code.find(c);
    if(_it!=__code.end())
        return _it->second.first;
    else {
        cerr << "Found unknown token value : " << (uint16_t)c << endl;
        return string("Error");
    }
}

/* ----------------------------------------------------------------------- */

bool TokenCode::initTokens(fistream& stream,ostream & output){
  uint_t size;
  char c;
  stream >> c;
  if(c != '!'){
      cerr << "Token for Initialisation not Found [ Char=" << int(c) << " ('" << c << "') instead of '!' ]" << endl;
      output << "Token for Initialisation not Found [ Char=" << int(c) << " ('" << c << "') instead of '!' ]" << endl;
      return false;
  }
  else {
      if(stream.eof())return false;
      stream >> size;
#ifdef GEOM_DEBUG
      cerr << "must find " << size << " Tokens " << endl;
#endif
      for(uint_t a = 0; a < size ; a++){
          uchar_t code(0);
          stream >> code;
          uint16_t length(0);
          stream >> length;
          char * label = new char[length+1];
          stream.read(label,length);
          label[length]='\0';
          string _label = label;
          delete [] label;
          uint_t nb(0);
          stream >> nb;
          __code[code] = pair<string,uint_t>(_label,nb);
#ifdef GEOM_DEBUG
         output << "Token : " << (int)code << " - '" << _label << "' [length=" << length << "] (nb=" << nb << ')' << endl;;
         cerr << "Token : " << (int)code << " - '" << _label << "' [length=" << length << "] (nb=" << nb << ')' << endl;;
#endif
      }
  }
  stream >> c;
  if(c != '!'){
      output << "Token for End of Initialisation not Found [ Char=" << int(c) << " ('" << c << "') instead of '!' ]" << endl;
      output << "Found the " << size << " following Tokens : " << endl;
          for(pgl_hash_map<uchar_t,pair<string,uint_t> >::iterator _it = __code.begin();
                        _it != __code.end(); _it++)
                                if(_it->second.second != 0)
                output << "Token : " << (int)_it->first << " - '" << _it->second.first << "' (nb=" << _it->second.second << ')' << endl;;
    return false;
  }
  return true;
}

/* ----------------------------------------------------------------------- */

fostream& operator<<( fostream& stream, TokenCode& c ){
  return c.printAll(stream);
}

/* ----------------------------------------------------------------------- */

#ifdef GEOM_DEBUG
#define DEBUG_INFO(type,name,id) \
  std::cerr << "Print " << #type << " '" << name << "', Id=" << id << " at pos " << __outputStream.getStream().tellp() << std::endl;
#else
#define DEBUG_INFO(type,name,id) ;
#endif


#define GEOM_PRINT_BEGIN(type,obj) \
  if (!obj->unique()) { \
    if (! __cache.insert(obj->getObjectId()).second) { \
      DEBUG_INFO(TokReference,obj->getName(),obj->getObjectId()) \
      printType(TokReference); \
      writeUint32(obj->getObjectId()); \
      return true; \
    } \
    else { \
     DEBUG_INFO(#type,obj->getName(),obj->getObjectId()) \
     printType(#type);    \
     writeString(obj->getName()); \
     writeUint32(obj->getObjectId()); \
    } \
  } \
  else { \
    DEBUG_INFO(#type,"",obj->getObjectId())  \
    printType(#type); writeString("") ; writeUint32(0); \
  }

#define GEOM_PRINT_APPEARANCE(val) \
  val->apply(*this);


#define GEOM_PRINT_ANGLE(val) \
  writeReal(val);


#define GEOM_PRINT_BOOLEAN(val) \
  writeBool(val);


#define GEOM_PRINT_COLOR3(val) write(val);

void BinaryPrinter::write(const Color3& val) {
  writeUchar(val.getRed()); \
  writeUchar(val.getGreen()); \
  writeUchar(val.getBlue());
}

#define GEOM_PRINT_COLOR4(val) write(val);

void BinaryPrinter::write(const Color4& val) {
  writeUchar(val.getRed()); \
  writeUchar(val.getGreen()); \
  writeUchar(val.getBlue()); \
  writeUchar(val.getAlpha());
}

#define GEOM_PRINT_GEOMETRY(val) \
  val->apply(*this);


#define GEOM_PRINT_INDEX3(val) write(val);

void BinaryPrinter::write(const Index3& val) {
  writeUint32(val.getAt(0)); \
  writeUint32(val.getAt(1)); \
  writeUint32(val.getAt(2));
}

#define GEOM_PRINT_INDEX4(val) write(val);

void BinaryPrinter::write(const Index4& val) {
  writeUint32(val.getAt(0)); \
  writeUint32(val.getAt(1)); \
  writeUint32(val.getAt(2)); \
  writeUint32(val.getAt(3));
}

#define GEOM_PRINT_INDEXN(val) write(val);

void BinaryPrinter::write(const Index& val) {
    uint_t _sizej = val.size();
    writeUint32(_sizej);
    for(Index::const_iterator it = val.begin(); it != val.end(); ++it)
        writeUint32(*it);
};


#define GEOM_PRINT_UINT32(val) \
  writeUint32(val);

#define GEOM_PRINT_UCHAR(val) \
  writeUchar(val);


#define GEOM_PRINT_REAL(val) \
  writeReal(val);


#define GEOM_PRINT_STRING(val) \
   writeString(val);

#define GEOM_PRINT_FILE(val) \
  writeFile(val);

#define GEOM_PRINT_VECTOR2(val) write(val);

void BinaryPrinter::write(const Vector2& val) {
    writeReal(val.x()); writeReal(val.y());
}


#define GEOM_PRINT_VECTOR3(val) write(val);

void BinaryPrinter::write(const Vector3& val) {
 writeReal(val.x()); writeReal(val.y()); writeReal(val.z());
}

#define GEOM_PRINT_VECTOR4(val) write(val);

void BinaryPrinter::write(const Vector4& val) {
 writeReal(val.x()); writeReal(val.y()); \
 writeReal(val.z()); writeReal(val.w());
}

void BinaryPrinter::write(const Matrix2& val) {
    for (Matrix2::const_iterator it = val.begin(); it != val.end(); ++it)
        writeReal(*it);
}

void BinaryPrinter::write(const Matrix3& val) {
    for (Matrix3::const_iterator it = val.begin(); it != val.end(); ++it)
        writeReal(*it);
}

void BinaryPrinter::write(const Matrix4& val) {
    for (Matrix4::const_iterator it = val.begin(); it != val.end(); ++it)
        writeReal(*it);
}

#define GEOM_PRINT_TRANSFO4(obj) { \
    Matrix4 m= obj->getMatrix(); \
    for( uchar_t _itVec= 0; _itVec < 4; ++_itVec ){ \
      GEOM_PRINT_VECTOR4( m.getColumn(_itVec) ); \
    }};


#define GEOM_PRINT_FIELD(obj,field,type) { \
    GEOM_PRINT_##type(obj->get##field()); \
  };


#define GEOM_PRINT_FIELD_ARRAY(obj,field,type)  if(obj->get##field()) { writeArray(*obj->get##field()); } else { writeUint32(0); }
#define GEOM_PRINT_FIELD_MATRIX(obj,field,type) if(obj->get##field()) { writeMatrix(*obj->get##field()); } else { writeUint32(0); }

/* { \
    uint_t _sizei = (obj->get##field()?obj->get##field()->size():0); \
    writeUint32(_sizei); \
    for (uint_t _i = 0; _i < _sizei; _i++) { \
      GEOM_PRINT_##type(obj->get##field()->getAt(_i)); \
    }; \
  };


#define GEOM_PRINT_FIELD_MATRIX(obj,field,type) { \
    uint_t _cols =obj->get##field()->getColumnNb(); \
    uint_t _sizei = obj->get##field()->size(); \
    writeUint32( _sizei/ _cols); writeUint32(_cols); \
    for (uint_t _i = 0; _i < _sizei; _i++) { \
      GEOM_PRINT_##type(obj->get##field()->getAt(_i / _cols ,_i % _cols)); \
    }; \
  };
  */

/* ----------------------------------------------------------------------- */

const float BinaryPrinter::BINARY_FORMAT_VERSION(2.5f);

/* ----------------------------------------------------------------------- */


BinaryPrinter::BinaryPrinter( fostream& outputStream  ) :
  Printer(outputStream.getStream(),outputStream.getStream(),outputStream.getStream()),
  __outputStream(outputStream),
  __tokens(BINARY_FORMAT_VERSION){
    outputStream.setByteOrder(PglLittleEndian);
}

BinaryPrinter::~BinaryPrinter( ) {
}

bool
BinaryPrinter::beginProcess(){
    return true;
}

bool
BinaryPrinter::endProcess(){
    return true;
}

/// write an bool value from stream
void BinaryPrinter::writeBool(bool var)
{ __outputStream << var; }


/// write an uint_t value from stream
void BinaryPrinter::writeUint32(uint_t var)
/*#if __WORDSIZE == 64
#ifdef __GNUC__
#warning "Approximate uint64 to uint32 for Binary output"
#endif
{ __outputStream << (uintptr_t)var; }
#else */
{ __outputStream << var; }
// #endif

  /// write an int32_t value from stream
void BinaryPrinter::writeInt32(int_t var)
/*#if __WORDSIZE == 64
{ __outputStream << (intptr_t)var; }
#else */
{ __outputStream << var; }
// #endif

/// write a uint16_t value from stream
void BinaryPrinter::writeUint16(uint16_t var)
{ __outputStream << var; }

  /// write a uchar_t value from stream
void BinaryPrinter::writeUchar(uchar_t var)
{ __outputStream << var; }

/// write a real_t value from stream
void BinaryPrinter::writeReal(real_t var)
{ __outputStream << var; }

/// write a string value from stream
void BinaryPrinter::writeString(const std::string& var)
{ writeUint16(var.length()); if(var.length() != 0)__outputStream << var.c_str(); }

/// write a file name value from stream
void BinaryPrinter::writeFile(const std::string& var)
{ __outputStream << '!' << var.c_str() << '!';  }


/* ----------------------------------------------------------------------- */
bool BinaryPrinter::print(ScenePtr scene,string filename,const char * comment){
    bofstream stream(filename.c_str());
    stream.setByteOrder(PglLittleEndian);
    if(!stream)return false;
    else {
        string cwd = get_cwd();
        chg_dir(get_dirname(filename));
        BinaryPrinter _bp(stream);
        _bp.print(scene,comment);
        chg_dir(cwd);
        return true;
    }
}

#include <sstream>

std::string BinaryPrinter::tobinarystring(ScenePtr scene, const char * comment)
{
    std::ostringstream _mystream;
    fostream _myfstream(_mystream);
    _myfstream.setByteOrder(PglLittleEndian);
    BinaryPrinter _bp(_myfstream);
    _bp.print(scene,comment);
    return _mystream.str();
}


bool BinaryPrinter::print(ScenePtr scene,const char * comment){
    header(comment);
    StatisticComputer _sc;
    scene->apply(_sc);
    __tokens.setStatistic(_sc);
    __tokens.printAll(__outputStream );
#ifdef GEOM_DEBUG
    cerr << "Print scene of " << scene->size() << " objects (" << _sc.getNamed() << " named )." << endl;
#endif
    writeUint32(scene->size());
//    __outputStream << _sc.getNamed();
    return scene->apply(*this);
}



/* ----------------------------------------------------------------------- */

bool BinaryPrinter::header(const char * comment){
    __outputStream << string("!bGEOM");
    __outputStream << float(BINARY_FORMAT_VERSION) << char(13) << char(10);
  if(__tokens.getVersion() >= 1.6f){
#ifdef PGL_USE_DOUBLE
    uchar_t precision = 64 ;
#else
#ifdef __GNUC__
#warning Use simple floating precision
#endif
    uchar_t precision = 32 ;
#endif
    // std::cerr << "Assume "<< (precision==32?"simple":"double")<< " precision." << std::endl;
    writeUchar(precision);
  }
    __outputStream << '#';
    __outputStream << ( comment ? string(comment) : string("a GEOM binary File") ) << '#' ;
    return true;
}

/* ----------------------------------------------------------------------- */

void BinaryPrinter::printType(const string& _string){
    __tokens.printCurrentToken(__outputStream,_string);
}


/* ----------------------------------------------------------------------- */
void
BinaryPrinter::printFile(const std::string& FileName){
    writeFile(getCanonicalFilename(FileName));
}

std::string
BinaryPrinter::getCanonicalFilename(const std::string& FileName){
  if(FileName.empty()){
    return "";
  }
  string plantgl_dir = getPlantGLDir();
  string f = FileName;
  string pref = short_dirname(get_dirname(f));
  if(pref.empty() || pref[0] == '.'){
      return f;
  }
  string pref1 = pref;
  string cwd = short_dirname(get_cwd());
  if(cwd.empty() || cwd == "."){
      return f;
  }
  if(pref1.size() > cwd.size())
    pref1 = string(pref1.begin(),pref1.begin()+cwd.size());
    // cerr << "Compare '" << pref1 << "' to '" << cwd << endl;
  if(similar_dir(pref1,cwd)){
      int count = 0;
      for(string::const_iterator _i = pref1.begin();
          _i != pref1.end(); _i++)
            if(*_i == '\\' || *_i == '/')count++;
      string::iterator _j = f.begin();
      for(;_j != f.end() && count>0; _j++)
        if(*_j == '\\' || *_j == '/')count--;
      if(*(pref1.end()-1) != '\\' && *(pref1.end()-1) != '/'){
        while(*_j != '\\' && *_j != '/')_j++;
      }
      if(*_j == '\\' || *_j == '/')_j++;
      string suffix = string(_j,f.end());
      // cerr << "Print '" << suffix << "' " << endl;
      return suffix;
  }
  else  {
    // printf("compare '%s' '%s'\n",plantgl_dir.c_str(),pref.c_str());
    if(pref.size() > plantgl_dir.size()){
      pref = string(pref.begin(),pref.begin()+plantgl_dir.size());
    }


    if(similar_dir(pref,plantgl_dir)){
      int count = 0;
      for(string::const_iterator _i = pref.begin();
      _i != pref.end(); _i++)if(*_i == '\\' || *_i == '/')count++;
      string::iterator _j = f.begin();
      for(;_j != f.end() && count>0; _j++)if(*_j == '\\' || *_j == '/')count--;
      if(*(pref.end()-1) != '\\' && *(pref.end()-1) != '/'){
        while(*_j != '\\' && *_j != '/')_j++;
      }
      string suffix = string(_j,f.end());
      //      f.reserve(suffix.size()+15);
      f = "PLANTGL_DIR"+suffix;
      return f;
    }
    else  return FileName;
  }
}

/* ----------------------------------------------------------------------- */

bool BinaryPrinter::process(Shape * Shape){
    GEOM_ASSERT(Shape);


    DEBUG_INFO("Shape",Shape->getName(),Shape->getObjectId());
    printType("Shape");
    writeString(Shape->getName());
    writeUint32(Shape->id);
    if(__tokens.getVersion() >= 1.9f)
        writeUint32(Shape->parentId);
    Shape->geometry->apply(*this);
    Shape->appearance->apply(*this);

    return true;
}


bool BinaryPrinter::process(Inline * geomInline){
    GEOM_ASSERT(geomInline);

    Vector3 _trans = geomInline->getTranslation();
    Vector3 _scale = geomInline->getScale();
    if(_trans == Vector3::ORIGIN && _scale == Vector3(1,1,1)){
      for(Scene::iterator _it = geomInline->getScene()->begin();_it != geomInline->getScene()->end();_it++ )
        (*_it)->apply(*this);
    }
    else {
      MatrixTransformedPtr _s;
      GeometryPtr _transf;
      if(_trans == Vector3::ORIGIN){
        _s = MatrixTransformedPtr(new Scaled(_scale,GeometryPtr()));
        _transf = dynamic_pointer_cast<MatrixTransformed>(_s);
      }
      else if (_scale == Vector3(1,1,1)){
        _s = MatrixTransformedPtr(new Translated(_trans,GeometryPtr()));
        _transf = dynamic_pointer_cast<MatrixTransformed>(_s);
      }
      else {
        _s = MatrixTransformedPtr(new Scaled(_scale,GeometryPtr()));
        GeometryPtr _s2;
        if(!(_s2 = dynamic_pointer_cast<Geometry>(_s)))return false;
        _transf = GeometryPtr(new Translated(_trans,_s2));
      }

      for(Scene::iterator _it = geomInline->getScene()->begin();_it != geomInline->getScene()->end();_it++ ){
        ShapePtr shape = dynamic_pointer_cast<Shape>(*_it);
        if(shape){
          DEBUG_INFO("Shape",shape->getName(),shape->getObjectId());
          printType("Shape");
          writeString(shape->getName());
          writeUint32(shape->id);
          _s->getGeometry() = shape->getGeometry();
          _transf->apply(*this);
          shape->appearance->apply(*this);
        }
        else {

        }
      }
    }
    return true;
}

/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Material * material ) {
  GEOM_ASSERT(material);
  GEOM_PRINT_BEGIN(Material,material);

  uchar_t _default(0);
  if (material->isAmbientToDefault())
      _default+=1;
  if (material->isDiffuseToDefault())
      _default+=2;
  if (material->isSpecularToDefault())
      _default+=4;
  if (material->isEmissionToDefault())
      _default+=8;
  if (material->isShininessToDefault())
      _default+=16;
  if (material->isTransparencyToDefault())
      _default+=32;
  __outputStream << _default;

  if (! material->isAmbientToDefault())
    GEOM_PRINT_FIELD(material,Ambient,COLOR3);

  if (! material->isDiffuseToDefault())
    GEOM_PRINT_FIELD(material,Diffuse,REAL);

  if (! material->isSpecularToDefault())
    GEOM_PRINT_FIELD(material,Specular,COLOR3);

  if (! material->isEmissionToDefault())
    GEOM_PRINT_FIELD(material,Emission,COLOR3);

  if (! material->isShininessToDefault())
    GEOM_PRINT_FIELD(material,Shininess,REAL);

  if (! material->isTransparencyToDefault())
    GEOM_PRINT_FIELD(material,Transparency,REAL);

  return true;
}

/* ----------------------------------------------------------------------- */

bool BinaryPrinter::process( ImageTexture * texture ) {
  GEOM_ASSERT(texture);
  GEOM_PRINT_BEGIN(ImageTexture,texture);

  uchar_t _default(0);

  if(__tokens.getVersion() < 2.3f) {
      _default = 63;
  }
  else {
      if (texture->isRepeatSToDefault())
          _default+=1;
      if (texture->isRepeatTToDefault())
          _default+=2;
  }

  if(__tokens.getVersion() >= 1.8f)
    if (texture->isMipmapingToDefault())
        _default+=64;
  writeUchar(_default);

  printFile(texture->getFilename());

  if(__tokens.getVersion() > 2.3f) {
      if (! texture->isRepeatSToDefault())
        GEOM_PRINT_FIELD(texture,RepeatS,BOOLEAN);

      if (! texture->isRepeatTToDefault())
        GEOM_PRINT_FIELD(texture,RepeatT,BOOLEAN);
  }

  if(__tokens.getVersion() >= 1.8f){
      if (! texture->isMipmapingToDefault())
        GEOM_PRINT_FIELD(texture,Mipmaping,BOOLEAN);
  }

  return true;
}

/* ----------------------------------------------------------------------- */

bool BinaryPrinter::process( Texture2D * texture ) {
  GEOM_ASSERT(texture);
  GEOM_PRINT_BEGIN(Texture2D,texture);

  uchar_t _default(0);
  if (texture->isTransformationToDefault())
      _default+=1;
  if (texture->isBaseColorToDefault())
      _default+=2;
  writeUchar(_default);

  texture->getImage()->apply(*this);

  if (!texture->isTransformationToDefault())
    texture->getTransformation()->apply(*this);

  if(__tokens.getVersion() > 2.4f) {
      if (! texture->isBaseColorToDefault())
         GEOM_PRINT_FIELD(texture,BaseColor,COLOR4);
   }

  return true;
}

/* ----------------------------------------------------------------------- */

bool BinaryPrinter::process( Texture2DTransformation * texturetransfo ) {
  GEOM_ASSERT(texturetransfo);
  GEOM_PRINT_BEGIN(Texture2DTransformation,texturetransfo);

  uchar_t _default(0);
  if (texturetransfo->isScaleToDefault())
      _default+=1;

  if (texturetransfo->isTranslationToDefault())
      _default+=2;

  if (texturetransfo->isRotationCenterToDefault())
      _default+=4;

  if (texturetransfo->isRotationAngleToDefault())
      _default+=8;

  writeUchar(_default);

  if (!texturetransfo->isScaleToDefault())
        GEOM_PRINT_FIELD(texturetransfo,Scale,VECTOR2);

  if (!texturetransfo->isTranslationToDefault())
        GEOM_PRINT_FIELD(texturetransfo,Translation,VECTOR2);

  if (!texturetransfo->isRotationCenterToDefault())
        GEOM_PRINT_FIELD(texturetransfo,RotationCenter,VECTOR2);

  if (!texturetransfo->isRotationAngleToDefault())
        GEOM_PRINT_FIELD(texturetransfo,RotationAngle,REAL);

  return true;
}

/* ----------------------------------------------------------------------- */

bool BinaryPrinter::process( MonoSpectral * monoSpectral ) {
  GEOM_ASSERT(monoSpectral);
  GEOM_PRINT_BEGIN(MonoSpectral,monoSpectral);

  uchar_t _default(0);
  if (monoSpectral->isReflectanceToDefault())
      _default+=1;
  if (monoSpectral->isTransmittanceToDefault())
      _default+=2;
  writeUchar(_default);

  if (! monoSpectral->isReflectanceToDefault())
      GEOM_PRINT_FIELD(monoSpectral,Reflectance,REAL);

  if (! monoSpectral->isTransmittanceToDefault())
      GEOM_PRINT_FIELD(monoSpectral,Transmittance,REAL);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( MultiSpectral * multiSpectral ) {
  GEOM_ASSERT(multiSpectral);
  GEOM_PRINT_BEGIN(MultiSpectral,multiSpectral);

  if (! multiSpectral->isFilterToDefault()){
      writeUchar(0);
      GEOM_PRINT_FIELD(multiSpectral,Filter,INDEX3);
  }
  else  writeUchar(1);

  GEOM_PRINT_FIELD_ARRAY(multiSpectral,Reflectance,REAL);

  GEOM_PRINT_FIELD_ARRAY(multiSpectral,Transmittance,REAL);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( AmapSymbol * amapSymbol ) {
  GEOM_ASSERT(amapSymbol);
  GEOM_PRINT_BEGIN(AmapSymbol,amapSymbol);

  printFile(amapSymbol->getFileName());

  GEOM_PRINT_FIELD(amapSymbol,Solid,BOOLEAN);

  if(__tokens.getVersion() >= 1.1f){
        GEOM_PRINT_FIELD_ARRAY(amapSymbol,PointList,VECTOR3);

        GEOM_PRINT_FIELD_ARRAY(amapSymbol,NormalList,VECTOR3);

        GEOM_PRINT_FIELD_ARRAY(amapSymbol,IndexList,INDEXN);
  }
  if(__tokens.getVersion() >= 1.3f){
        GEOM_PRINT_FIELD_ARRAY(amapSymbol,TexCoord3List,VECTOR3);
  }

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( AsymmetricHull * asymmetricHull ) {
  GEOM_ASSERT(asymmetricHull);
  GEOM_PRINT_BEGIN(AsymmetricHull,asymmetricHull);

  uint16_t _default = 0;
  if (asymmetricHull->isNegXRadiusToDefault())
      _default += 1;
  if (asymmetricHull->isPosXRadiusToDefault())
      _default += 2;
  if (asymmetricHull->isNegYRadiusToDefault())
      _default += 4;
  if (asymmetricHull->isPosYRadiusToDefault())
      _default += 8;
  if (asymmetricHull->isNegXHeightToDefault())
      _default += 16;
  if (asymmetricHull->isPosXHeightToDefault())
      _default += 32;
  if (asymmetricHull->isNegYHeightToDefault())
      _default += 64;
  if (asymmetricHull->isPosYHeightToDefault())
      _default += 128;
  if (asymmetricHull->isBottomToDefault())
      _default += 256;
  if (asymmetricHull->isTopToDefault())
      _default += 512;
  if (asymmetricHull->isBottomShapeToDefault())
      _default += 1024;
  if (asymmetricHull->isTopShapeToDefault())
      _default += 2048;
  if (asymmetricHull->isSlicesToDefault())
      _default += 4096;
  if (asymmetricHull->isStacksToDefault())
      _default += 8192;

  writeUint16(_default);
  if (! asymmetricHull->isNegXRadiusToDefault())
    GEOM_PRINT_FIELD(asymmetricHull,NegXRadius,REAL);

  if (! asymmetricHull->isPosXRadiusToDefault())
    GEOM_PRINT_FIELD(asymmetricHull,PosXRadius,REAL);

  if (! asymmetricHull->isNegYRadiusToDefault())
    GEOM_PRINT_FIELD(asymmetricHull,NegYRadius,REAL);

  if (! asymmetricHull->isPosYRadiusToDefault())
    GEOM_PRINT_FIELD(asymmetricHull,PosYRadius,REAL);

  if (! asymmetricHull->isNegXHeightToDefault())
    GEOM_PRINT_FIELD(asymmetricHull,NegXHeight,REAL);

  if (! asymmetricHull->isPosXHeightToDefault())
    GEOM_PRINT_FIELD(asymmetricHull,PosXHeight,REAL);

  if (! asymmetricHull->isNegYHeightToDefault())
    GEOM_PRINT_FIELD(asymmetricHull,NegYHeight,REAL);

  if (! asymmetricHull->isPosYHeightToDefault())
    GEOM_PRINT_FIELD(asymmetricHull,PosYHeight,REAL);

  if (! asymmetricHull->isBottomToDefault())
    GEOM_PRINT_FIELD(asymmetricHull,Bottom,VECTOR3);

  if (! asymmetricHull->isTopToDefault())
    GEOM_PRINT_FIELD(asymmetricHull,Top,VECTOR3);

  if (! asymmetricHull->isBottomShapeToDefault())
    GEOM_PRINT_FIELD(asymmetricHull,BottomShape,REAL);

  if (! asymmetricHull->isTopShapeToDefault())
    GEOM_PRINT_FIELD(asymmetricHull,TopShape,REAL);

  if (! asymmetricHull->isSlicesToDefault())
    GEOM_PRINT_FIELD(asymmetricHull,Slices,UCHAR);

  if (! asymmetricHull->isStacksToDefault())
    GEOM_PRINT_FIELD(asymmetricHull,Stacks,UCHAR);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( AxisRotated * axisRotated ) {
  GEOM_ASSERT(axisRotated);
  GEOM_PRINT_BEGIN(AxisRotated,axisRotated);

  uchar_t _default = 0;
  if (axisRotated->isAxisToDefault())
      _default =1;
  if (axisRotated->isAngleToDefault())
      _default +=2;
  writeUchar(_default);

  if (! axisRotated->isAxisToDefault())
      GEOM_PRINT_FIELD(axisRotated,Axis,VECTOR3);

  if (! axisRotated->isAngleToDefault())
      GEOM_PRINT_FIELD(axisRotated,Angle,ANGLE);

  GEOM_PRINT_FIELD(axisRotated,Geometry,GEOMETRY);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( BezierCurve * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);
  GEOM_PRINT_BEGIN(BezierCurve,bezierCurve);

  uchar_t _default(0);
  if (bezierCurve->isStrideToDefault())
      _default =1;

  if(__tokens.getVersion() >= 2.1f){
    if (bezierCurve->isWidthToDefault()) _default +=2;
  }
  writeUchar(_default);

  if (! bezierCurve->isStrideToDefault()){
      GEOM_PRINT_FIELD(bezierCurve,Stride,UINT32);
  }

  if(__tokens.getVersion() >= 2.1f){
    if (! bezierCurve->isWidthToDefault()){
      GEOM_PRINT_FIELD(bezierCurve,Width,UCHAR);
    }
  }

  GEOM_PRINT_FIELD_ARRAY(bezierCurve,CtrlPointList,VECTOR4);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( BezierPatch * bezierPatch ) {
  GEOM_ASSERT(bezierPatch);
  GEOM_PRINT_BEGIN(BezierPatch,bezierPatch);

  uchar_t _default(0);
  if (bezierPatch->isUStrideToDefault())
      _default =1;
  if (bezierPatch->isVStrideToDefault())
      _default +=2;
  if (bezierPatch->isCCWToDefault())
      _default +=4;
  writeUchar(_default);

  if (! bezierPatch->isUStrideToDefault())
    GEOM_PRINT_FIELD(bezierPatch,UStride,UINT32);

  if (! bezierPatch->isVStrideToDefault())
    GEOM_PRINT_FIELD(bezierPatch,VStride,UINT32);

  if (! bezierPatch->isCCWToDefault())
    GEOM_PRINT_FIELD( bezierPatch,CCW,BOOLEAN);

  GEOM_PRINT_FIELD_MATRIX(bezierPatch,CtrlPointMatrix,VECTOR4);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Box * box ) {
  GEOM_ASSERT(box);
  GEOM_PRINT_BEGIN(Box,box);

  if (! box->isSizeToDefault()){
      writeUchar(0);
      GEOM_PRINT_FIELD(box,Size,VECTOR3);
  }
  else writeUchar(1);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Cone * cone ) {
  GEOM_ASSERT(cone);
  GEOM_PRINT_BEGIN(Cone,cone);

  uchar_t _default(0);
  if (cone->isRadiusToDefault())
      _default = 1;
  if (cone->isHeightToDefault())
      _default += 2;
  if (cone->isSolidToDefault())
      _default += 4;
  if (cone->isSlicesToDefault())
      _default += 8;
  writeUchar(_default);

  if (! cone->isRadiusToDefault())
    GEOM_PRINT_FIELD(cone,Radius,REAL);

  if (! cone->isHeightToDefault())
    GEOM_PRINT_FIELD(cone,Height,REAL);

  if (! cone->isSolidToDefault())
    GEOM_PRINT_FIELD(cone,Solid,BOOLEAN);

  if (! cone->isSlicesToDefault())
    GEOM_PRINT_FIELD(cone,Slices,UCHAR);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Cylinder * cylinder ) {
  GEOM_ASSERT(cylinder);
  GEOM_PRINT_BEGIN(Cylinder,cylinder);

  uchar_t _default(0);
  if (cylinder->isRadiusToDefault())
      _default = 1;
  if (cylinder->isHeightToDefault())
      _default += 2;
  if (cylinder->isSolidToDefault())
      _default += 4;
  if (cylinder->isSlicesToDefault())
      _default += 8;
  writeUchar(_default);


  if (! cylinder->isRadiusToDefault())
    GEOM_PRINT_FIELD(cylinder,Radius,REAL);

  if (! cylinder->isHeightToDefault())
    GEOM_PRINT_FIELD(cylinder,Height,REAL);

  if (! cylinder->isSolidToDefault())
    GEOM_PRINT_FIELD(cylinder,Solid,BOOLEAN);

  if (! cylinder->isSlicesToDefault())
    GEOM_PRINT_FIELD(cylinder,Slices,UCHAR);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( ElevationGrid * elevationGrid ) {
  GEOM_ASSERT(elevationGrid);
  GEOM_PRINT_BEGIN(ElevationGrid,elevationGrid);

  uchar_t _default(0);
  if (elevationGrid->isXSpacingToDefault())
      _default = 1;
  if (elevationGrid->isYSpacingToDefault())
      _default += 2;
  if (elevationGrid->isCCWToDefault())
      _default += 4;
  writeUchar(_default);

  if (! elevationGrid->isXSpacingToDefault())
    GEOM_PRINT_FIELD(elevationGrid,XSpacing,REAL);

  if (! elevationGrid->isYSpacingToDefault())
    GEOM_PRINT_FIELD(elevationGrid,YSpacing,REAL);

  if (! elevationGrid->isCCWToDefault())
    GEOM_PRINT_FIELD( elevationGrid,CCW,BOOLEAN);

  GEOM_PRINT_FIELD_MATRIX(elevationGrid,HeightList,REAL);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( EulerRotated * eulerRotated ) {
  GEOM_ASSERT(eulerRotated);
  GEOM_PRINT_BEGIN(EulerRotated,eulerRotated);

  uchar_t _default(0);
  if (eulerRotated->isAzimuthToDefault())
      _default = 1;
  if (eulerRotated->isElevationToDefault())
      _default += 2;
  if (eulerRotated->isRollToDefault())
      _default += 4;
  writeUchar(_default);

  if (! eulerRotated->isAzimuthToDefault())
    GEOM_PRINT_FIELD(eulerRotated,Azimuth,ANGLE);

  if (! eulerRotated->isElevationToDefault())
    GEOM_PRINT_FIELD(eulerRotated,Elevation,ANGLE);

  if (! eulerRotated->isRollToDefault())
    GEOM_PRINT_FIELD(eulerRotated,Roll,ANGLE);

  GEOM_PRINT_FIELD(eulerRotated,Geometry,GEOMETRY);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( ExtrudedHull * extrudedHull ) {
  GEOM_ASSERT(extrudedHull);
  GEOM_PRINT_BEGIN(ExtrudedHull,extrudedHull);

  uchar_t _default(0);
  if (extrudedHull->isCCWToDefault())
      _default = 1;
  writeUchar(_default);

  if (! extrudedHull->isCCWToDefault())
    GEOM_PRINT_FIELD(extrudedHull,CCW,BOOLEAN);

  GEOM_PRINT_FIELD(extrudedHull,Vertical,GEOMETRY);

  GEOM_PRINT_FIELD(extrudedHull,Horizontal,GEOMETRY);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Extrusion * extrusion ) {
  GEOM_ASSERT(extrusion);
  GEOM_PRINT_BEGIN(Extrusion,extrusion);
  uchar_t _default(0);
  if(extrusion->isSolidToDefault() )
      _default = 1;
  if(extrusion->isCCWToDefault() )
      _default += 2;
  if(extrusion->getProfileTransformation()){
      if( extrusion->isScaleToDefault() )
          _default += 4;
      if( extrusion->isOrientationToDefault() )
          _default += 8;
      if(  extrusion->isKnotListToDefault() )
          _default += 16;
  }
  else _default += 28;
  if(__tokens.getVersion() >= 2.2f){
    if(extrusion->isInitialNormalToDefault() )
         _default += 32;
  }

  writeUchar(_default);

  if(! extrusion->isSolidToDefault() )
      GEOM_PRINT_FIELD(extrusion,Solid,BOOLEAN);

  if(! extrusion->isCCWToDefault() )
      GEOM_PRINT_FIELD(extrusion,CCW,BOOLEAN);

 if(extrusion->getProfileTransformation()){
      ProfileTransformationPtr _p = extrusion->getProfileTransformation();
      if( ! extrusion->isScaleToDefault() ) {
          GEOM_PRINT_FIELD_ARRAY( _p,Scale,VECTOR2);
      }

      if( ! extrusion->isOrientationToDefault() ){
          GEOM_PRINT_FIELD_ARRAY( _p,Orientation,ANGLE);
      }

      if( ! extrusion->isKnotListToDefault() ){
          GEOM_PRINT_FIELD_ARRAY( _p,KnotList,REAL);
      }

  }
  if(__tokens.getVersion() >= 2.2f){
    if(! extrusion->isInitialNormalToDefault() ){
      GEOM_PRINT_FIELD(extrusion,InitialNormal,VECTOR3);
    }
  }

  GEOM_PRINT_FIELD(extrusion,Axis,GEOMETRY);

  GEOM_PRINT_FIELD(extrusion,CrossSection,GEOMETRY);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( FaceSet * faceSet ) {
  GEOM_ASSERT(faceSet);
  GEOM_PRINT_BEGIN(FaceSet,faceSet);

  uint16_t _default(0);
  if (faceSet->isCCWToDefault())
      _default = 1;
  if (faceSet->isSolidToDefault())
      _default += 2;
  if (faceSet->isSkeletonToDefault())
      _default += 4;
  if(__tokens.getVersion() >= 1.2f){
    if (faceSet->isNormalListToDefault())
        _default += 8;
    if(faceSet->isNormalPerVertexToDefault() )
        _default += 16;
    if (__tokens.getVersion() >= 1.5f){
        if (faceSet->isNormalIndexListToDefault())
            _default += 64;
    }
  }
  if (__tokens.getVersion() >= 1.5f){
    if (faceSet->isColorPerVertexToDefault())
        _default += 128;
    if (faceSet->isColorListToDefault())
        _default += 256;
    if (faceSet->isColorIndexListToDefault())
        _default += 512;
  }
  if(__tokens.getVersion() >= 1.3f){
    if (faceSet->isTexCoordListToDefault())
        _default += 32;
    if (__tokens.getVersion() >= 1.5f){
        if (faceSet->isTexCoordIndexListToDefault())
            _default += 1024;
    }
  }

  if (__tokens.getVersion() >= 1.5f)
    writeUint16(_default);
  else writeUchar(_default);

  if (! faceSet->isCCWToDefault())
    GEOM_PRINT_FIELD(faceSet,CCW,BOOLEAN);

  if (! faceSet->isSolidToDefault())
    GEOM_PRINT_FIELD(faceSet,Solid,BOOLEAN);

  if (! faceSet->isSkeletonToDefault())
    GEOM_PRINT_FIELD(faceSet,Skeleton,GEOMETRY);

  GEOM_PRINT_FIELD_ARRAY(faceSet,PointList,VECTOR3);

  GEOM_PRINT_FIELD_ARRAY(faceSet,IndexList,INDEXN);

  if(__tokens.getVersion() >= 1.5f){
    if (!faceSet->isNormalPerVertexToDefault()){
      GEOM_PRINT_FIELD(faceSet,NormalPerVertex,BOOLEAN);
    }
  }
  if(__tokens.getVersion() >= 1.2f){
    if (!faceSet->isNormalListToDefault()){
      GEOM_PRINT_FIELD_ARRAY(faceSet,NormalList,VECTOR3);
    }
  }
  if(__tokens.getVersion() >= 1.5f){
    if (!faceSet->isNormalIndexListToDefault()){
      GEOM_PRINT_FIELD_ARRAY(faceSet,NormalIndexList,INDEXN);
    }

    if (!faceSet->isColorPerVertexToDefault()){
      GEOM_PRINT_FIELD(faceSet,ColorPerVertex,BOOLEAN);
    }

    if (!faceSet->isColorListToDefault()){
      GEOM_PRINT_FIELD_ARRAY(faceSet,ColorList,COLOR4);
    }

    if (!faceSet->isColorIndexListToDefault()){
      GEOM_PRINT_FIELD_ARRAY(faceSet,ColorIndexList,INDEXN);
    }

  }
  if(__tokens.getVersion() >= 1.3f){
    if (!faceSet->isTexCoordListToDefault()){
        GEOM_PRINT_FIELD_ARRAY(faceSet,TexCoordList,VECTOR2);
    }
  }
  if(__tokens.getVersion() >= 1.5f){
    if (!faceSet->isTexCoordIndexListToDefault()){
      GEOM_PRINT_FIELD_ARRAY(faceSet,TexCoordIndexList,INDEXN);
    }
  }

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Frustum * frustum ) {
  GEOM_ASSERT(frustum);
  GEOM_PRINT_BEGIN(Frustum,frustum);

  uchar_t _default(0);
  if (frustum->isRadiusToDefault())
      _default = 1;
  if (frustum->isHeightToDefault())
      _default += 2;
  if (frustum->isTaperToDefault())
      _default += 4;
  if (frustum->isSolidToDefault())
      _default += 8;
  if (frustum->isSlicesToDefault())
      _default += 16;
  writeUchar(_default);


  if (! frustum->isRadiusToDefault())
    GEOM_PRINT_FIELD(frustum,Radius,REAL);

  if (! frustum->isHeightToDefault())
    GEOM_PRINT_FIELD(frustum,Height,REAL);

  if (! frustum->isTaperToDefault())
    GEOM_PRINT_FIELD(frustum,Taper,REAL);

  if (! frustum->isSolidToDefault())
    GEOM_PRINT_FIELD(frustum,Solid,BOOLEAN);

  if (! frustum->isSlicesToDefault())
    GEOM_PRINT_FIELD(frustum,Slices,UCHAR);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Group * group  ) {
  GEOM_ASSERT(group);
  GEOM_PRINT_BEGIN(Group,group);

  if (! group->isSkeletonToDefault()){
      writeUchar(0);
      GEOM_PRINT_FIELD(group,Skeleton,GEOMETRY);}
  else writeUchar(1);

  GEOM_PRINT_FIELD_ARRAY(group,GeometryList,GEOMETRY);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( IFS * ifs )
{
//todo OK
  GEOM_ASSERT(ifs);
GEOM_TRACE("process IFS");
  GEOM_PRINT_BEGIN(IFS,ifs);

  if( !ifs->isDepthToDefault() )
    {
    writeUchar(0);
    GEOM_PRINT_FIELD(ifs,Depth,UCHAR);
    }
  else
    writeUchar(1);

  GEOM_PRINT_FIELD_ARRAY(ifs,TransfoList,TRANSFO4);

  GEOM_PRINT_FIELD(ifs,Geometry,GEOMETRY);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( NurbsCurve * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);
  GEOM_PRINT_BEGIN(NurbsCurve,nurbsCurve);

  uchar_t _default(0);
  if (nurbsCurve->isDegreeToDefault())
      _default = 1;
  if (nurbsCurve->isKnotListToDefault())
      _default += 2;
  if (nurbsCurve->isStrideToDefault())
      _default += 4;
  if(__tokens.getVersion() >= 2.1f){
    if (nurbsCurve->isWidthToDefault()) _default +=8;
  }
  writeUchar(_default);

  if (! nurbsCurve->isDegreeToDefault()) {
    GEOM_PRINT_FIELD(nurbsCurve,Degree,UINT32);
    }

  if (! nurbsCurve->isKnotListToDefault()){
    GEOM_PRINT_FIELD_ARRAY(nurbsCurve,KnotList,REAL);
    }

  if (! nurbsCurve->isStrideToDefault()) {
    GEOM_PRINT_FIELD(nurbsCurve,Stride,UINT32);
    }

  if(__tokens.getVersion() >= 2.1f){
    if (! nurbsCurve->isWidthToDefault()){
      GEOM_PRINT_FIELD(nurbsCurve,Width,UCHAR);
    }
  }
  GEOM_PRINT_FIELD_ARRAY(nurbsCurve,CtrlPointList,VECTOR4);

  return true;
}

/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( NurbsPatch * nurbsPatch ) {
  GEOM_ASSERT(nurbsPatch);
  GEOM_PRINT_BEGIN(NurbsPatch,nurbsPatch);

  uchar_t _default(0);
  if (nurbsPatch->isUDegreeToDefault())
      _default = 1;
  if (nurbsPatch->isVDegreeToDefault())
      _default += 2;
  if (nurbsPatch->isUKnotListToDefault())
      _default += 4;
  if (nurbsPatch->isVKnotListToDefault())
      _default += 8;
  if (nurbsPatch->isUStrideToDefault())
      _default += 16;
  if (nurbsPatch->isVStrideToDefault())
      _default += 32;
  if (nurbsPatch->isCCWToDefault())
      _default += 64;
  writeUchar(_default);


  if (! nurbsPatch->isUDegreeToDefault()){
    GEOM_PRINT_FIELD(nurbsPatch,UDegree,UINT32);
  }

  if (! nurbsPatch->isVDegreeToDefault())
   { GEOM_PRINT_FIELD(nurbsPatch,VDegree,UINT32);}

  if (! nurbsPatch->isUKnotListToDefault())
    {GEOM_PRINT_FIELD_ARRAY(nurbsPatch,UKnotList,REAL);}

  if (! nurbsPatch->isVKnotListToDefault())
    {GEOM_PRINT_FIELD_ARRAY(nurbsPatch,VKnotList,REAL);}

  if (! nurbsPatch->isUStrideToDefault())
    {GEOM_PRINT_FIELD(nurbsPatch,UStride,UINT32);}

  if (! nurbsPatch->isVStrideToDefault())
    {GEOM_PRINT_FIELD(nurbsPatch,VStride,UINT32);}

  if (! nurbsPatch->isCCWToDefault())
    {GEOM_PRINT_FIELD( nurbsPatch,CCW,BOOLEAN);}

  GEOM_PRINT_FIELD_MATRIX(nurbsPatch,CtrlPointMatrix,VECTOR4);

  return true;
}

/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Oriented * oriented ) {
  GEOM_ASSERT(oriented);
  GEOM_PRINT_BEGIN(Oriented,oriented);

  uchar_t _default(0);
  if (oriented->isPrimaryToDefault())
      _default = 1;
  if (oriented->isSecondaryToDefault())
      _default += 2;
  writeUchar(_default);

  if (! oriented->isPrimaryToDefault())
    GEOM_PRINT_FIELD(oriented,Primary,VECTOR3);

  if (! oriented->isSecondaryToDefault())
    GEOM_PRINT_FIELD(oriented,Secondary,VECTOR3);

  GEOM_PRINT_FIELD(oriented,Geometry,GEOMETRY);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Paraboloid * paraboloid ) {
  GEOM_ASSERT(paraboloid);
  GEOM_PRINT_BEGIN(Paraboloid,paraboloid);

  uchar_t _default(0);
  if (paraboloid->isRadiusToDefault())
      _default = 1;
  if (paraboloid->isHeightToDefault())
      _default += 2;
  if (paraboloid->isShapeToDefault())
      _default += 4;
  if (paraboloid->isSolidToDefault())
      _default += 8;
  if (paraboloid->isSlicesToDefault())
      _default += 16;
  if (paraboloid->isStacksToDefault())
      _default += 32;
  writeUchar(_default);

  if (! paraboloid->isRadiusToDefault())
    GEOM_PRINT_FIELD(paraboloid,Radius,REAL);

  if (! paraboloid->isHeightToDefault())
    GEOM_PRINT_FIELD(paraboloid,Height,REAL);

  if (! paraboloid->isShapeToDefault())
    GEOM_PRINT_FIELD(paraboloid,Shape,REAL);

  if (! paraboloid->isSolidToDefault())
    GEOM_PRINT_FIELD(paraboloid,Solid,BOOLEAN);

  if (! paraboloid->isSlicesToDefault())
    GEOM_PRINT_FIELD(paraboloid,Slices,UCHAR);

  if (! paraboloid->isStacksToDefault())
    GEOM_PRINT_FIELD(paraboloid,Stacks,UCHAR);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( PointSet * pointSet ) {
  GEOM_ASSERT(pointSet);
  GEOM_PRINT_BEGIN(PointSet,pointSet);

  GEOM_PRINT_FIELD_ARRAY(pointSet,PointList,VECTOR3);
  GEOM_PRINT_FIELD_ARRAY(pointSet,ColorList,COLOR4);
  if(__tokens.getVersion() >= 2.1f){
      GEOM_PRINT_FIELD(pointSet,Width,UCHAR);
  }

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Polyline * polyline ) {
  GEOM_ASSERT(polyline);
  GEOM_PRINT_BEGIN(Polyline,polyline);

  GEOM_PRINT_FIELD_ARRAY(polyline,PointList,VECTOR3);
  GEOM_PRINT_FIELD_ARRAY(polyline,ColorList,COLOR4);
  if(__tokens.getVersion() >= 2.1f){
      GEOM_PRINT_FIELD(polyline,Width,UCHAR);
  }

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Revolution * revolution ) {
  GEOM_ASSERT(revolution);
  GEOM_PRINT_BEGIN(Revolution,revolution);

  if (! revolution->isSlicesToDefault()){
      writeUchar(0);
      GEOM_PRINT_FIELD(revolution,Slices,UCHAR);
  }
  else writeUchar(1);

  GEOM_PRINT_FIELD(revolution,Profile,GEOMETRY);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Swung * swung )
{
  GEOM_ASSERT(swung);
  GEOM_PRINT_BEGIN(Swung,swung);
#ifdef GEOM_DEBUG
  std::cerr << "Name : '" << swung->getName() << "', Id : " << swung->getObjectId() << std::endl;
  std::cerr << "Pos : " << __outputStream.getStream().tellp() << std::endl;
#endif

  uchar_t _default(0);
  if (swung->isSlicesToDefault())
      _default += 1;
  if (swung->isCCWToDefault())
      _default += 2;
  if (swung->isDegreeToDefault())
      _default += 4;
  if (swung->isStrideToDefault())
      _default += 8;
  writeUchar(_default);
#ifdef GEOM_DEBUG
  std::cerr << "Default : " << int(_default) << std::endl;
#endif

  if( !swung->isSlicesToDefault() ){
    GEOM_PRINT_FIELD(swung,Slices,UCHAR);
#ifdef GEOM_DEBUG
    std::cerr << "Write slices : " << int(swung->getSlices()) << std::endl;
#endif
    }
    if( !swung->isCCWToDefault() ){
    GEOM_PRINT_FIELD(swung,CCW,BOOLEAN);
#ifdef GEOM_DEBUG
    std::cerr << "Write CCW : " << (swung->getCCW()?"True":"False") << std::endl;
#endif
    }
    if( !swung->isDegreeToDefault() ){
    GEOM_PRINT_FIELD(swung,Degree,UINT32);
#ifdef GEOM_DEBUG
    std::cerr << "Write Degree : " << swung->getDegree() << std::endl;
#endif
    }
    if( !swung->isStrideToDefault() ){
    GEOM_PRINT_FIELD(swung,Stride,UINT32);
#ifdef GEOM_DEBUG
    std::cerr << "Write Stride : " << swung->getStride() << std::endl;
#endif
    }

  GEOM_PRINT_FIELD_ARRAY(swung,AngleList,ANGLE);

  GEOM_PRINT_FIELD_ARRAY(swung,ProfileList,GEOMETRY);


  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( QuadSet * quadSet ) {
  GEOM_ASSERT(quadSet);
  GEOM_PRINT_BEGIN(QuadSet,quadSet);

  uint16_t _default(0);
  if (quadSet->isCCWToDefault())
      _default = 1;
  if (quadSet->isSolidToDefault())
      _default += 2;
  if (quadSet->isSkeletonToDefault())
      _default += 4;
  if(__tokens.getVersion() >= 1.2f){
    if(quadSet->getNormalPerVertex())
        _default += 16;
    if (quadSet->isNormalListToDefault())
        _default += 8;
    if(__tokens.getVersion() >= 1.5f){
        if (quadSet->isNormalIndexListToDefault())
            _default += 64;
    }
  }
  if(__tokens.getVersion() >= 1.5f){
    if (quadSet->isColorPerVertexToDefault())
        _default += 128;
    if (quadSet->isColorListToDefault())
        _default += 256;
    if (quadSet->isColorIndexListToDefault())
        _default += 512;
  }
  if(__tokens.getVersion() >= 1.3f){
    if (quadSet->isTexCoordListToDefault())
        _default += 32;
    if(__tokens.getVersion() >= 1.5f){
        if (quadSet->isTexCoordIndexListToDefault())
            _default += 1024;
    }
  }
  if(__tokens.getVersion() >= 1.5f) writeUint16(_default);
  else  writeUchar(_default);

  if (! quadSet->isCCWToDefault())
    GEOM_PRINT_FIELD(quadSet,CCW,BOOLEAN);

  if (! quadSet->isSolidToDefault())
    GEOM_PRINT_FIELD(quadSet,Solid,BOOLEAN);

  if (! quadSet->isSkeletonToDefault())
    GEOM_PRINT_FIELD(quadSet,Skeleton,GEOMETRY);

  GEOM_PRINT_FIELD_ARRAY(quadSet,PointList,VECTOR3);

  GEOM_PRINT_FIELD_ARRAY(quadSet,IndexList,INDEX4);

  if(__tokens.getVersion() >= 1.2f){
    if(__tokens.getVersion() >= 1.5f){
      if (!quadSet->isNormalPerVertexToDefault()){
        GEOM_PRINT_FIELD(quadSet,NormalPerVertex,BOOLEAN);
      }
     }

     if (!quadSet->isNormalListToDefault()){
       GEOM_PRINT_FIELD_ARRAY(quadSet,NormalList,VECTOR3);
     }

    if(__tokens.getVersion() >= 1.5f){
      if (!quadSet->isNormalIndexListToDefault()){
        GEOM_PRINT_FIELD_ARRAY(quadSet,NormalIndexList,INDEX4);
      }
     }
  }
  if(__tokens.getVersion() >= 1.5f){
   if (!quadSet->isColorPerVertexToDefault()){
    GEOM_PRINT_FIELD(quadSet,ColorPerVertex,BOOLEAN);
   }

   if (!quadSet->isColorListToDefault()){
    GEOM_PRINT_FIELD_ARRAY(quadSet,ColorList,COLOR4);
   }

   if (!quadSet->isColorIndexListToDefault()){
    GEOM_PRINT_FIELD_ARRAY(quadSet,ColorIndexList,INDEX4);
   }
  }

  if(__tokens.getVersion() >= 1.3f){
    if (!quadSet->isTexCoordListToDefault()){
        GEOM_PRINT_FIELD_ARRAY(quadSet,TexCoordList,VECTOR2);
    }
    if(__tokens.getVersion() >= 1.5f){
      if (!quadSet->isTexCoordIndexListToDefault()){
        GEOM_PRINT_FIELD_ARRAY(quadSet,TexCoordIndexList,INDEX4);
      }
     }
  }

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Scaled * scaled ) {
  GEOM_ASSERT(scaled);
  GEOM_PRINT_BEGIN(Scaled,scaled);

  GEOM_PRINT_FIELD(scaled,Scale,VECTOR3);

  GEOM_PRINT_FIELD(scaled,Geometry,GEOMETRY);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( ScreenProjected * scp ) {
  GEOM_ASSERT(scp);
  if(__tokens.getVersion() >= 2.4f){

    GEOM_PRINT_BEGIN(ScreenProjected,scp);

    GEOM_PRINT_FIELD(scp,KeepAspectRatio,BOOLEAN);

    GEOM_PRINT_FIELD(scp,Geometry,GEOMETRY);

    return true;
  }
  else return false;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Sphere * sphere ) {
  GEOM_ASSERT(sphere);
  GEOM_PRINT_BEGIN(Sphere,sphere);

  uchar_t _default(0);
  if (sphere->isRadiusToDefault())
      _default = 1;
  if (sphere->isSlicesToDefault())
      _default += 2;
  if (sphere->isStacksToDefault())
      _default += 4;
  writeUchar(_default);

  if (! sphere->isRadiusToDefault())
    GEOM_PRINT_FIELD(sphere,Radius,REAL);

  if (! sphere->isSlicesToDefault())
    GEOM_PRINT_FIELD(sphere,Slices,UCHAR);

  if (! sphere->isStacksToDefault())
    GEOM_PRINT_FIELD(sphere,Stacks,UCHAR);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Tapered * tapered ) {
  GEOM_ASSERT(tapered);
  GEOM_PRINT_BEGIN(Tapered,tapered);

  uchar_t _default(0);
  if (tapered->isBaseRadiusToDefault())
      _default = 1;
  if (tapered->isTopRadiusToDefault())
      _default += 2;
  writeUchar(_default);

  if (! tapered->isBaseRadiusToDefault())
    GEOM_PRINT_FIELD(tapered,BaseRadius,REAL);

  if (! tapered->isTopRadiusToDefault())
    GEOM_PRINT_FIELD(tapered,TopRadius,REAL);

  GEOM_PRINT_FIELD(tapered,Primitive,GEOMETRY);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Translated * translated ) {
  GEOM_ASSERT(translated);
  GEOM_PRINT_BEGIN(Translated,translated);

  GEOM_PRINT_FIELD(translated,Translation,VECTOR3);

  GEOM_PRINT_FIELD(translated,Geometry,GEOMETRY);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( TriangleSet * triangleSet ) {
  GEOM_ASSERT(triangleSet);
  GEOM_PRINT_BEGIN(TriangleSet,triangleSet);

  uint16_t _default(0);
  if (triangleSet->isCCWToDefault())
      _default = 1;
  if (triangleSet->isSolidToDefault())
      _default += 2;
  if (triangleSet->isSkeletonToDefault())
      _default += 4;
  if(__tokens.getVersion() >= 1.2f){
    if(triangleSet->getNormalPerVertex())
        _default += 16;
    if (triangleSet->isNormalListToDefault())
        _default += 8;
    if(__tokens.getVersion() >= 1.5f){
      if (triangleSet->isNormalIndexListToDefault())
        _default += 64;
    }
  }
  if(__tokens.getVersion() >= 1.5f){
   if(triangleSet->getColorPerVertex())
        _default += 128;
   if (triangleSet->isColorListToDefault())
        _default += 256;
   if (triangleSet->isColorIndexListToDefault())
        _default += 512;
  }
  if(__tokens.getVersion() >= 1.3f){
    if (triangleSet->isTexCoordListToDefault()){
        _default += 32;
    }
    if(__tokens.getVersion() >= 1.5f){
      if (triangleSet->isTexCoordIndexListToDefault())
        _default += 1024;
    }
  }

  if(__tokens.getVersion() >= 1.5f) writeUint16(_default);
  else writeUchar(_default);

  if (! triangleSet->isCCWToDefault())
    GEOM_PRINT_FIELD(triangleSet,CCW,BOOLEAN);

  if (! triangleSet->isSolidToDefault())
    GEOM_PRINT_FIELD(triangleSet,Solid,BOOLEAN);

  if (! triangleSet->isSkeletonToDefault())
    GEOM_PRINT_FIELD(triangleSet,Skeleton,GEOMETRY);

  GEOM_PRINT_FIELD_ARRAY(triangleSet,PointList,VECTOR3);

  GEOM_PRINT_FIELD_ARRAY(triangleSet,IndexList,INDEX3);

  if(__tokens.getVersion() >= 1.2f){
    if(__tokens.getVersion() >= 1.5f){
        if (!triangleSet->isNormalPerVertexToDefault()){
          GEOM_PRINT_FIELD(triangleSet,NormalPerVertex,BOOLEAN);
        }
    }
    if (!triangleSet->isNormalListToDefault()){
      GEOM_PRINT_FIELD_ARRAY(triangleSet,NormalList,VECTOR3);
    }
    if(__tokens.getVersion() >= 1.5f){
        if (!triangleSet->isNormalIndexListToDefault()){
          GEOM_PRINT_FIELD_ARRAY(triangleSet,NormalIndexList,INDEX3);
        }
    }
  }
  if(__tokens.getVersion() >= 1.5f){
    if (!triangleSet->isColorPerVertexToDefault()){
      GEOM_PRINT_FIELD(triangleSet,ColorPerVertex,BOOLEAN);
    }
    if (!triangleSet->isColorListToDefault()){
      GEOM_PRINT_FIELD_ARRAY(triangleSet,ColorList,COLOR4);
    }
    if (!triangleSet->isColorIndexListToDefault()){
      GEOM_PRINT_FIELD_ARRAY(triangleSet,ColorIndexList,INDEX3);
    }
  }
  if(__tokens.getVersion() >= 1.3f){
    if (!triangleSet->isTexCoordListToDefault()){
        GEOM_PRINT_FIELD_ARRAY(triangleSet,TexCoordList,VECTOR2);
    }
    if(__tokens.getVersion() >= 1.5f){
        if (!triangleSet->isTexCoordIndexListToDefault()){
          GEOM_PRINT_FIELD_ARRAY(triangleSet,TexCoordIndexList,INDEX3);
        }
    }
  }

  return true;
}


/* ------------------------------------------------------------------------- */


bool BinaryPrinter::process( BezierCurve2D * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);
  GEOM_PRINT_BEGIN(BezierCurve2D,bezierCurve);

  uchar_t _default(0);
  if (bezierCurve->isStrideToDefault())
      _default =1;

  if(__tokens.getVersion() >= 2.1f){
    if (bezierCurve->isWidthToDefault()) _default +=2;
  }
  writeUchar(_default);

  if (! bezierCurve->isStrideToDefault()){
      GEOM_PRINT_FIELD(bezierCurve,Stride,UINT32);
  }

  if(__tokens.getVersion() >= 2.1f){
    if (! bezierCurve->isWidthToDefault()){
      GEOM_PRINT_FIELD(bezierCurve,Width,UCHAR);
    }
  }

  GEOM_PRINT_FIELD_ARRAY(bezierCurve,CtrlPointList,VECTOR3);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Disc * disc ) {
  GEOM_ASSERT(disc);
  GEOM_PRINT_BEGIN(Disc,disc);

  uchar_t _default(0);
  if (disc->isRadiusToDefault())
      _default = 1;
  if (disc->isSlicesToDefault())
      _default += 2;
  writeUchar(_default);

  if (! disc->isRadiusToDefault())
    GEOM_PRINT_FIELD(disc,Radius,REAL);

  if (! disc->isSlicesToDefault())
    GEOM_PRINT_FIELD(disc,Slices,UCHAR);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( NurbsCurve2D * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);
  GEOM_PRINT_BEGIN(NurbsCurve2D,nurbsCurve);

  uchar_t _default(0);
  if (nurbsCurve->isDegreeToDefault())
      _default = 1;
  if (nurbsCurve->isKnotListToDefault())
      _default += 2;
  if (nurbsCurve->isStrideToDefault())
      _default += 4;
  if(__tokens.getVersion() >= 2.1f){
    if (nurbsCurve->isWidthToDefault()) _default +=8;
  }
  writeUchar(_default);

  if (! nurbsCurve->isDegreeToDefault())
    {GEOM_PRINT_FIELD(nurbsCurve,Degree,UINT32);}

  if (! nurbsCurve->isKnotListToDefault())
    {GEOM_PRINT_FIELD_ARRAY(nurbsCurve,KnotList,REAL);}

  if (! nurbsCurve->isStrideToDefault())
    {GEOM_PRINT_FIELD(nurbsCurve,Stride,UINT32);}

  if(__tokens.getVersion() >= 2.1f){
    if (! nurbsCurve->isWidthToDefault()){
      GEOM_PRINT_FIELD(nurbsCurve,Width,UCHAR);
    }
  }
  GEOM_PRINT_FIELD_ARRAY(nurbsCurve,CtrlPointList,VECTOR3);

  return true;
}

/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( PointSet2D * pointSet ) {
  GEOM_ASSERT(pointSet);
  GEOM_PRINT_BEGIN(PointSet2D,pointSet);

  GEOM_PRINT_FIELD_ARRAY(pointSet,PointList,VECTOR2);
  GEOM_PRINT_FIELD(pointSet,Width,UCHAR);

  return true;
}


/* ----------------------------------------------------------------------- */


bool BinaryPrinter::process( Polyline2D * polyline ) {
  GEOM_ASSERT(polyline);
  GEOM_PRINT_BEGIN(Polyline2D,polyline);

  GEOM_PRINT_FIELD_ARRAY(polyline,PointList,VECTOR2);
  GEOM_PRINT_FIELD(polyline,Width,UCHAR);

  return true;
}


/* ----------------------------------------------------------------------- */

bool BinaryPrinter::process( Text * text ) {
  GEOM_ASSERT(text);
  GEOM_PRINT_BEGIN(Text,text);

  uchar_t _default(0);
  if (text->isFontStyleToDefault())
      _default = 1;
  if (text->isPositionToDefault())
      _default += 2;
  if (text->isScreenCoordinatesToDefault())
      _default += 4;
  writeUchar(_default);

  GEOM_PRINT_FIELD(text,String,STRING);

  if (!text->isFontStyleToDefault())
    GEOM_PRINT_FIELD(text,FontStyle,GEOMETRY);

  if (!text->isPositionToDefault())
    GEOM_PRINT_FIELD(text,Position,VECTOR3);

  if (!text->isScreenCoordinatesToDefault())
    GEOM_PRINT_FIELD(text,ScreenCoordinates,BOOLEAN);

  return true;
}

/* ----------------------------------------------------------------------- */

bool BinaryPrinter::process( Font * font ) {
  GEOM_ASSERT(font);
  GEOM_PRINT_BEGIN(Font,font);

  uchar_t _default(0);
  if (font->isSizeToDefault())
      _default += 1;
  if (font->isBoldToDefault())
      _default += 2;
  if (font->isItalicToDefault())
      _default += 4;
  writeUchar(_default);

  GEOM_PRINT_FIELD(font,Family,STRING);

  if(!font->isSizeToDefault())
    GEOM_PRINT_FIELD(font,Size,UINT32);

  if(!font->isBoldToDefault())
    GEOM_PRINT_FIELD(font,Bold,BOOLEAN);

  if(!font->isItalicToDefault())
    GEOM_PRINT_FIELD(font,Italic,BOOLEAN);

  return true;
}

/* ----------------------------------------------------------------------- */
