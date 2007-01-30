/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
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





#include <tool/bfstream.h>
#include "amapsymbol.h"
#include "polyline.h"
#include <scenegraph/core/pgl_messages.h>
#include <scenegraph/container/pointarray.h>
#include <scenegraph/container/indexarray.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
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
  FaceSetDestroy();
  if (FileName) delete FileName;
}


bool AmapSymbol::Builder::isValid( ) const {
    if (! FileName) {
        genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"AmapSymbol","FileName");
        return false;
    };
    beifstream _file(FileName->c_str());
    if (_file) return true;
    string _mess;
    _mess = "Cannot open " + *FileName + ".";
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"AmapSymbol","FileName",_mess.c_str());
    return false;
}


/* ----------------------------------------------------------------------- */

AmapSymbol::AmapSymbol() :
    FaceSet(),
  __fileName(),
  __texCoord3List(0)
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
  __texCoord3List(0)
{
  beifstream _file(__fileName.c_str());
  GEOM_ASSERT(_file);
  if(_file)read(_file);
}

AmapSymbol::~AmapSymbol( ) {
}

bool AmapSymbol::apply( Action& action ) {
  return action.process(this);
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
  uint32_t _pointsCount= 0;
  uint32_t _facesCount= 0;
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
  uint32_t _indicesCount = 0;
  uint32_t _pointColor= 0;
  uint32_t _pointIndex= 0;
  uint16_t _pointAttribute= 0;
  _faceInfo[0]= 0; _faceInfo[1]= 0;
  for (uint32_t _i = 0; _i < _facesCount; _i++) {
    stream >> _indicesCount >> _faceInfo[0] >> _faceInfo[1];
#ifdef GEOM_DEBUG
        gerr << "Face " << _i << " : " << _indicesCount
                 << " Points, Infos = " << _faceInfo[0] << ',' << _faceInfo[1]
                 << " { " << endl;
#endif
    Index _index(_indicesCount);
    for (uint32_t _j = 0; _j < _indicesCount; _j++) {
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

  for (uint32_t _j = 0; _j < _pointsCount; _j++) {
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
  __texCoord3List((faceSet->getTexCoordList())? new Point3Array(faceSet->getTexCoordList()->getSize()):0)
  {
	if(faceSet->getTexCoordList()){
	Point2Array::const_iterator _it = faceSet->getTexCoordList()->getBegin();
	for(Point3Array::iterator _it2 = __texCoord3List->getBegin();
		_it2 != __texCoord3List->getEnd(); _it2++)
		  *_it2 = Vector3(0,_it->x(),_it->y());
	}
}


bool AmapSymbol::isValid( ) const {
  if (! FaceSet::isValid()) return false;
  //if (__fileName.empty()) {
  //  genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"AmapSymbol","FileName");
  //};
  return true;
}


bofstream& AmapSymbol::write( bofstream& stream ) const {
  GEOM_ASSERT(stream);
  uint32_t _facesCount = __indexList->getSize();
  uint32_t _pointsCount = __pointList->getSize();

  stream << _facesCount << _pointsCount;

  // Kind of face: always 1 for polygon - Dimension: always 3 for 3D
  uint16_t _faceInfo[2] = { 1, 3 };
  uint32_t _indicesCount;
  uint32_t _pointColor = 1;
  uint32_t _pointIndex;

  /* The construction attribute is used for the connection between two 
   internodes:
    - 1 means that the point is at the basis of the internode,
    - 2 means that the point is at the top of the internode,
    - 0 for the other position. */
  uint16_t _pointAttribute = 0;

  // Writes the faces
  for (uint32_t _i = 0; _i < _facesCount; _i++) {

    _indicesCount = __indexList->getAt(_i).getSize();
    stream << _indicesCount << _faceInfo[0] << _faceInfo[1];

    for (uint32_t _k = 0; _k < _indicesCount; _k++) {

      _pointIndex = __indexList->getAt(_i).getAt(_k);
      stream << _pointColor << _pointIndex << _pointAttribute;
    };
  };

  // Writes the points
  float _d = 0;

  for (uint32_t _j = 0; _j < _pointsCount; _j++) {
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

const Vector3& AmapSymbol::getTexCoord3At( uint32_t i, uint32_t j ) const {
  GEOM_ASSERT(i < __indexList->getSize());
  GEOM_ASSERT(j < __indexList->getAt(i).getSize());
  return __texCoord3List->getAt(__indexList->getAt(i).getAt(j));
}

Vector3& AmapSymbol::getTexCoord3At( uint32_t i, uint32_t j ){
  GEOM_ASSERT(i < __indexList->getSize());
  GEOM_ASSERT(j < __indexList->getAt(i).getSize());
  return __texCoord3List->getAt(__indexList->getAt(i).getAt(j));
}

/* ----------------------------------------------------------------------- */
