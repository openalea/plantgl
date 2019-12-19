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




#include <plantgl/tool/bfstream.h>
#include "amapsymbol.h"
#include "mesh_inline.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>

PGL_USING_NAMESPACE
using namespace std;

/* ----------------------------------------------------------------------- */


AmapSymbol::Builder::Builder( ) :
  FaceSet::Builder(),
  FileName(0) {
}


AmapSymbol::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr AmapSymbol::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new AmapSymbol(*FileName,
                      Solid ? *Solid : FaceSet::DEFAULT_SOLID));
  return SceneObjectPtr();
}


void AmapSymbol::Builder::destroy( ) {
  indexedMeshDestroy();
  if (FileName) delete FileName;
}


bool AmapSymbol::Builder::isValid( ) const {
    if (! FileName) {
        pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"AmapSymbol","FileName");
        return false;
    };
    beifstream _file(FileName->c_str());
    if (_file) return true;
    string _mess;
    _mess = "Cannot open " + *FileName + ".";
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"AmapSymbol","FileName",_mess.c_str());
    return false;
}


/* ----------------------------------------------------------------------- */

AmapSymbol::AmapSymbol() :
    FaceSet(),
  __fileName(),
  __texCoord3List()
{
}

AmapSymbol::AmapSymbol( const string& fileName,
                        bool solid  ) :
  FaceSet(Point3ArrayPtr(),
      IndexArrayPtr(),
      true,
      false,
      solid),
  __fileName(fileName),
  __texCoord3List()
{
  beifstream _file(__fileName.c_str());
  GEOM_ASSERT(_file);
  if(_file)read(_file);
}

AmapSymbol::~AmapSymbol( ) {
}

const string&
AmapSymbol::getFileName( ) const {
  return __fileName;
}

/// Returns \b FileName field.
string&
AmapSymbol::getFileName( ) {
  return __fileName;
}

bool AmapSymbol::readFile(const string& filename){
    beifstream _file(filename.c_str());
    if(_file){
        read(_file);
        __fileName = filename;
        return true;
    }
    else return false;
}

// #define GEOM_DEBUG

beifstream& AmapSymbol::read( beifstream& stream ){
  // reads the header : number of polygons / number of points
  uint_t _pointsCount= 0;
  uint_t _facesCount= 0;
  stream >> _facesCount >> _pointsCount;
#ifdef GEOM_DEBUG
  gerr << _facesCount << " Faces - " << _pointsCount << " Points" << endl;
#endif
  Point3ArrayPtr _pointList(new Point3Array(_pointsCount));
  Point3ArrayPtr _normalList(new Point3Array(_pointsCount));
  Point3ArrayPtr _texCoordList(new Point3Array(_pointsCount));
  IndexArrayPtr _indexList(new IndexArray(_facesCount));

  // Reads the polygons
  uint16_t _faceInfo[2];
  uint_t _indicesCount = 0;
  uint_t _pointColor= 0;
  uint_t _pointIndex= 0;
  uint16_t _pointAttribute= 0;
  _faceInfo[0]= 0; _faceInfo[1]= 0;
  for (uint_t _i = 0; _i < _facesCount; _i++) {
    stream >> _indicesCount >> _faceInfo[0] >> _faceInfo[1];
#ifdef GEOM_DEBUG
        gerr << "Face " << _i << " : " << _indicesCount
                 << " Points, Infos = " << _faceInfo[0] << ',' << _faceInfo[1]
                 << " { " << endl;
#endif
    Index _index(_indicesCount);
    for (uint_t _j = 0; _j < _indicesCount; _j++) {
      stream >> _pointColor >> _pointIndex >> _pointAttribute;
#ifdef GEOM_DEBUG
        gerr << " Point " << _j << " : Index = " << _pointIndex
                 << " Color = " << _pointColor
                 << " Attribut = " << _pointAttribute << endl;
#endif
      _index.setAt(_j,_pointIndex);
    };
#ifdef GEOM_DEBUG
        gerr << "} "<< endl;
#endif
    _indexList->setAt(_i,_index);
  };

  // Reads the points
  float _x= 0., _y= 0., _z= 0.;
  float _nx= 0., _ny= 0., _nz= 0.;
  float _u= 0., _v= 0., _d= 0.;
  real_t _maxZ = 0;
#ifdef GEOM_DEBUG
        gerr << endl;
#endif

  for (uint_t _j = 0; _j < _pointsCount; _j++) {
    stream >> _nx >> _ny >> _nz;
    stream >> _x >> _y >> _z;
    stream >> _u >> _v >> _d;
    /* We flip the coordinates as the symbol is originally oriented
       towards the positive x axis.
       We oriente the symbol on <k,i,j> orthonormal base :
       The primary direction of the symbol is aligned on z-axis.
       The secondary dir on the x-axis and the last on y-axis.
    */
#ifdef GEOM_DEBUG
        gerr << "Point " << _j << " : Position = " << Vector3(_y,_z,_x) << ',' << endl
                 << " Normal = " << Vector3(_ny,_nz,_nx)
                 << ", Texture = " << Vector3(_u,_v,_d) << endl;
#endif


    _pointList->setAt(_j,Vector3(_y,_z,_x));
    _normalList->setAt(_j,Vector3(_ny,_nz,_nx));
    _texCoordList->setAt(_j,Vector3(_u,_v,_d));
    if (_x > _maxZ) _maxZ = _x;
  };

  __pointList = _pointList;
  __normalList = _normalList;
  __indexList = _indexList;
  __texCoord3List = _texCoordList;
  __skeleton = PolylinePtr(new Polyline(Vector3::ORIGIN,Vector3(0,0,_maxZ)));

  GEOM_ASSERT(isValid());
  return stream;
}


AmapSymbol::AmapSymbol( const FaceSetPtr& faceSet ) :
  FaceSet(*faceSet),
  __fileName(),
  __texCoord3List((faceSet->getTexCoordList())? new Point3Array(faceSet->getTexCoordList()->size()):0)
  {
    if(faceSet->getTexCoordList()){
    Point2Array::const_iterator _it = faceSet->getTexCoordList()->begin();
    for(Point3Array::iterator _it2 = __texCoord3List->begin();
        _it2 != __texCoord3List->end(); _it2++)
          *_it2 = Vector3(0,_it->x(),_it->y());
    }
}


bool AmapSymbol::isValid( ) const {
  if (! FaceSet::isValid()) return false;
  //if (__fileName.empty()) {
  //  pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"AmapSymbol","FileName");
  //};
  return true;
}


bofstream& AmapSymbol::write( bofstream& stream ) const {
  GEOM_ASSERT(stream);
  uint_t _facesCount = __indexList->size();
  uint_t _pointsCount = __pointList->size();

  stream << _facesCount << _pointsCount;

  // Kind of face: always 1 for polygon - Dimension: always 3 for 3D
  uint16_t _faceInfo[2] = { 1, 3 };
  uint_t _indicesCount;
  uint_t _pointColor = 1;
  uint_t _pointIndex;

  /* The construction attribute is used for the connection between two
   internodes:
    - 1 means that the point is at the basis of the internode,
    - 2 means that the point is at the top of the internode,
    - 0 for the other position. */
  uint16_t _pointAttribute = 0;

  // Writes the faces
  for (uint_t _i = 0; _i < _facesCount; _i++) {

    _indicesCount = __indexList->getAt(_i).size();
    stream << _indicesCount << _faceInfo[0] << _faceInfo[1];

    for (uint_t _k = 0; _k < _indicesCount; _k++) {

      _pointIndex = __indexList->getAt(_i).getAt(_k);
      stream << _pointColor << _pointIndex << _pointAttribute;
    };
  };

  // Writes the points
  float _d = 0;

  for (uint_t _j = 0; _j < _pointsCount; _j++) {
    const Vector3& _point = __pointList->getAt(_j);
    // Bug: __normalList is null for default cylinder...
    const Vector3& _normal = (__normalList?__normalList->getAt(_j):Vector3::ORIGIN);
    const Vector3& _tex = (hasTexCoordList()?__texCoord3List->getAt(_j):Vector3::ORIGIN);
    stream << _normal.z() << _normal.x() << _normal.y();
    stream << _point.z() << _point.x() << _point.y();
    stream << _tex.x() << _tex.y() << _tex.z();
  };

  return stream;
}


/* ----------------------------------------------------------------------- */

const Vector3& AmapSymbol::getFaceTexCoord3At( uint_t i, uint_t j ) const {
  GEOM_ASSERT(i < __indexList->size());
  GEOM_ASSERT(j < __indexList->getAt(i).size());
  return __texCoord3List->getAt(__indexList->getAt(i).getAt(j));
}

Vector3& AmapSymbol::getFaceTexCoord3At( uint_t i, uint_t j ){
  GEOM_ASSERT(i < __indexList->size());
  GEOM_ASSERT(j < __indexList->getAt(i).size());
  return __texCoord3List->getAt(__indexList->getAt(i).getAt(j));
}


SceneObjectPtr
AmapSymbol::copy(DeepCopier& copier) const{
  AmapSymbolPtr res = mesh_copy<AmapSymbol>(*this,copier);
  copier.copy_attribute(res->__texCoord3List);
  return SceneObjectPtr(res);
}

/* ----------------------------------------------------------------------- */
