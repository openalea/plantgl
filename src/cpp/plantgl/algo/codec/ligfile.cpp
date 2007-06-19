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

#include "ligfile.h"
#include "dtafile.h"

#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/geometry/pointset.h>
#include <plantgl/scenegraph/geometry/frustum.h>
#include <plantgl/scenegraph/geometry/cylinder.h>
#include <plantgl/scenegraph/geometry/disc.h>
#include <plantgl/scenegraph/transformation/oriented.h>
#include <plantgl/scenegraph/transformation/translated.h>
#include <plantgl/scenegraph/transformation/axisrotated.h>
#include <plantgl/scenegraph/transformation/scaled.h>
#include <plantgl/scenegraph/transformation/tapered.h>
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/scene/scene.h>
#include <plantgl/algo/base/discretizer.h>
#include <plantgl/tool/bfstream.h>
#include <plantgl/tool/dirnames.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace std;

LigRecord::LigRecord() :
    __symbol(0),
    __val1(0),
    __val2(0),
    __range(0),
    __base_dia(0),
    __sommit_dia(0),
    __entity_number(0){
}

LigRecord::~LigRecord(){
}

void LigRecord::setValues(const long symbol, const long val1, const long val2,
                          const long range,  const Vector3 dirp,const Vector3 dirs,
                          const Vector3 dirt,const Vector3 origin,
                          const float base_dia,const float sommit_dia,const long entity_number){
    __symbol=symbol;
    __val1=val1;
    __val2=val2;
    __range=range;
    __matrix[0][0] = dirp.x(); __matrix[0][1] = dirs.x();__matrix[0][2] = dirt.x(); __matrix[0][3] = origin.x();
    __matrix[1][0] = dirp.y(); __matrix[1][1] = dirs.y();__matrix[1][2] = dirt.y(); __matrix[1][3] = origin.y();
    __matrix[2][0] = dirp.z(); __matrix[2][1] = dirs.z();__matrix[2][2] = dirt.z(); __matrix[2][3] = origin.z();
    __base_dia=base_dia;
    __sommit_dia=sommit_dia;
    __entity_number=entity_number;
}
    
void LigRecord::setValues(const Vector3 dirp,const Vector3 dirs,
						  const Vector3 dirt,const Vector3 origin,
						  const float base_dia, 
						  const float sommit_dia){
    __matrix[0][0] = dirp.x(); __matrix[0][1] = dirs.x();__matrix[0][2] = dirt.x(); __matrix[0][3] = origin.x();
    __matrix[1][0] = dirp.y(); __matrix[1][1] = dirs.y();__matrix[1][2] = dirt.y(); __matrix[1][3] = origin.y();
    __matrix[2][0] = dirp.z(); __matrix[2][1] = dirs.z();__matrix[2][2] = dirt.z(); __matrix[2][3] = origin.z();
    __base_dia=base_dia;
    __sommit_dia=sommit_dia;
}
    
#define SCALE_F(x) (fabs(x)>GEOM_EPSILON?x:1.0)

GeometryPtr 
LigRecord::getTransformed(GeometryPtr primitive) const{
  	Discretizer dis;
	return getTransformed(primitive,dis);
}

GeometryPtr 
LigRecord::getTransformed(GeometryPtr primitive, 
						  Discretizer& dis) const {
  
  Vector3 dirp(__matrix[0][0],__matrix[1][0], __matrix[2][0]);
  Vector3 dirs(__matrix[0][1],__matrix[1][1], __matrix[2][1]);
  Vector3 dirt(__matrix[0][2],__matrix[1][2], __matrix[2][2]);
  Vector3 origin(__matrix[0][3],__matrix[1][3], __matrix[2][3]);
  real_t length = dirp.normalize();
  real_t s1 = dirs.normalize();
  real_t s2 = dirt.normalize();
  real_t base_dia = __base_dia ;// 2;
  real_t sommit_dia = __sommit_dia; // 2;

  if(!dirs.isOrthogonalTo(dirt)){
	dirt = cross(dirp,dirs);
	dirt.normalize();
  }
  if(dot(cross(dirs,dirt),dirp) < 0) dirs = - dirs;
  
  if (!primitive) {

	 base_dia /= 2;
     sommit_dia /= 2;
	
  /*
  The geometry of the component was not defined : a default 
  geometry will be used: a frustum from the geom library
	*/
	bool open = false; // false for open cylinder
	real_t ratio;
	if (base_dia > REAL_EPSILON) ratio = sommit_dia/base_dia;
	else ratio = REAL_MAX;
	
	GeometryPtr shape(0);
	
	// if the radius is non-zero, define a "default" geometric object:
	//		if(fabs(base_dia) > REAL_EPSILON){
	
	if(length > REAL_EPSILON){
	  if(!(fabs(s1) > GEOM_EPSILON && fabs(s2) > GEOM_EPSILON)){
		Point3ArrayPtr pts = new Point3Array(2,origin);
		pts->setAt(1,origin+dirp*length);
		return GeometryPtr(new Polyline(pts));
	  }
	  else if(fabs(sommit_dia) < GEOM_EPSILON){
		if(fabs(base_dia) > GEOM_EPSILON){
		  if(s1!=s2 || fabs(base_dia*s1) > GEOM_EPSILON)
			shape = GeometryPtr(new Cone(base_dia*(s1==s2?s1:1),
			length, open, 6));
		  else {
			Point3ArrayPtr pts = new Point3Array(2,origin);
			pts->setAt(1,origin+dirp*length);
			return GeometryPtr(new Polyline(pts));
		  }
		}
		else {
		  Point3ArrayPtr pts = new Point3Array(2,origin);
		  pts->setAt(1,origin+dirp*length);
		  return GeometryPtr(new Polyline(pts));
		}
	  }
	  else if(fabs(base_dia) < GEOM_EPSILON){
		if(s1!=s2 || fabs(sommit_dia*s1) > GEOM_EPSILON)
		  shape = GeometryPtr(
		  new Translated(Vector3(0,0,length),GeometryPtr(
		  new AxisRotated(Vector3::OY,GEOM_PI,GeometryPtr(
		  new Cone(sommit_dia*(s1==s2?s1:1),
		  length, open, 6))))));
		else {
		  Point3ArrayPtr pts = new Point3Array(2,origin);
		  pts->setAt(1,origin+dirp*length);
		  return GeometryPtr(new Polyline(pts));
		}
	  }
	  else if(fabs( ratio - 1 ) > GEOM_EPSILON)
		shape = GeometryPtr(new Frustum(base_dia*(s1==s2 && s1 > GEOM_EPSILON?s1:1),
		length, ratio, open, 6));
	  else
		shape = GeometryPtr(new Cylinder(base_dia*(s1==s2 && s1 > GEOM_EPSILON?s1:1),
		length, open, 6));// six faces
	}
	else {
	  if(!(fabs(s1) > GEOM_EPSILON && fabs(s2) > GEOM_EPSILON)){
		return GeometryPtr(new PointSet(Point3ArrayPtr(new Point3Array(1,origin))));
	  }
	  if(fabs(base_dia) > GEOM_EPSILON){
		if(s1!=s2 || fabs(base_dia*s1) > GEOM_EPSILON)
		  shape = GeometryPtr(new Disc(base_dia*(s1==s2?s1:1),6));
		else return GeometryPtr(new PointSet(Point3ArrayPtr(new Point3Array(1,origin))));
	  }
	  else if(fabs(sommit_dia) > GEOM_EPSILON) {
		if(s1!=s2 || fabs(sommit_dia*s1) > GEOM_EPSILON)
		  shape = GeometryPtr(new Disc(sommit_dia*(s1==s2?s1:1),6));
		else return GeometryPtr(new PointSet(Point3ArrayPtr(new Point3Array(1,origin))));
	  }
	  else return GeometryPtr(new PointSet(Point3ArrayPtr(new Point3Array(1,origin))));
	}
	
	if(s1 != s2)shape = GeometryPtr(new Scaled(Vector3(s1,s2,1),shape));
	
	if((dirt != Vector3::ORIGIN)&&
	  (dirs != Vector3::ORIGIN)&&
	  (dirs != Oriented::DEFAULT_PRIMARY|| 
	  dirt != Oriented::DEFAULT_SECONDARY)){
	  shape = GeometryPtr(new Oriented(dirs,dirt,shape)); // see comment below
	}
	
	if(fabs(origin.x()) > GEOM_EPSILON || 
	  fabs(origin.y()) > GEOM_EPSILON || 
	  fabs(origin.z()) > GEOM_EPSILON)
	  return GeometryPtr(new Translated(origin,shape));
	else return shape;
	
  }
  
  if(!(fabs(base_dia) > GEOM_EPSILON &&
	fabs(s1) > GEOM_EPSILON &&
	fabs(s2) > GEOM_EPSILON &&
	fabs(length) > GEOM_EPSILON ))return GeometryPtr(0);
		
  
  PrimitivePtr _primitive;
  
  if(!_primitive.cast(primitive)){
	
	if (fabs(base_dia - sommit_dia) > GEOM_EPSILON) {
	  GeometryPtr psmb(0);
	  if (!(fabs(length - 1.0) > GEOM_EPSILON || 
		fabs(s1 - 1.0) > GEOM_EPSILON || 
		fabs(s2 - 1.0) > GEOM_EPSILON || 
		fabs(base_dia  - 1.0) > GEOM_EPSILON )) {
		psmb = GeometryPtr(_primitive);
	  }
	  else {
		psmb = GeometryPtr(
		  (new Scaled
		  (Vector3(base_dia * s1,
		  base_dia * s2,
		  length),
		  primitive)));
	  }
	  
	  if((dirt != Vector3::ORIGIN)&&
		(dirs != Vector3::ORIGIN)&&
		(dirs != Oriented::DEFAULT_PRIMARY|| 
		dirt != Oriented::DEFAULT_SECONDARY)){
		psmb =  GeometryPtr(new Oriented(dirs,dirt,psmb)); // see comment below
	  }
	  
	  if(fabs(origin.x()) > GEOM_EPSILON || 
		fabs(origin.y()) > GEOM_EPSILON || 
		fabs(origin.z()) > GEOM_EPSILON)
		psmb = GeometryPtr(new Translated(origin, psmb));
	  return psmb;
	}
	else if(primitive->apply(dis)){
	  _primitive = dis.getDiscretization();
	  if(primitive->isNamed()) 
		_primitive->setName(primitive->getName());
	}
  }
  
  if (primitive) {
	GeometryPtr psmb(0);
	
	// Creates a tapered geometry
	// The geometric model is defined in a reference system Rl = (x,y,z)
	// AMAPmod defines the coordinates of a reference system R = (P,S,T)
	// with respect to a global reference system R0
	// Since z is the primary direction P, x is the secundary direction S
	// and y is the ternary direction, the passage matrix M(R/RL) from RL 
	// to R is: [[0,1,0]t, [0,0,1]t, [1,0,0]t]
	// We then need to express how to pass from Rl to R0 :
	// X/R0 = M(R/R0) . M(R/RL) . X/Rl
	//      = [P,S,T] . [[0,1,0]t, [0,0,1]t, [1,0,0]t] . X/Rl
	//      = [S,T,P] . X/Rl
	
	
	// Creates a tapered geometry
	if (!(fabs(base_dia - sommit_dia) > GEOM_EPSILON)) {
	  
	  if( fabs(base_dia - 1) > GEOM_EPSILON ||
		fabs(s1 - 1) > GEOM_EPSILON ||
		fabs(s2 - 1) > GEOM_EPSILON ||
		fabs(length-1) > GEOM_EPSILON ){
		
		psmb = GeometryPtr(
		  (new Scaled
		  (Vector3( base_dia * s1 , base_dia * s2 , length ),
		  primitive)));
	  }
	  else {
		psmb = GeometryPtr(primitive);
	  }
	}
	else  {
	  
	  psmb = GeometryPtr
		(new Tapered
		(base_dia, 
		sommit_dia,
		_primitive));
	  
	  if(fabs(s1 - 1) > GEOM_EPSILON  ||
		fabs(s2 - 1) > GEOM_EPSILON ||
		fabs(length-1) > GEOM_EPSILON )
		
		psmb = GeometryPtr
		(new Scaled(Vector3(s1,s2,length),psmb));
	}
	
	if((dirt != Vector3::ORIGIN) &&
	  (dirs != Vector3::ORIGIN) &&
	  (dirs != Oriented::DEFAULT_PRIMARY|| 
	  dirt != Oriented::DEFAULT_SECONDARY))
	  psmb =  GeometryPtr(new Oriented(dirs,dirt,psmb)); // see comment above
	
	if(fabs(origin.x()) > GEOM_EPSILON || 
	  fabs(origin.y()) > GEOM_EPSILON || 
	  fabs(origin.z()) > GEOM_EPSILON)
	  psmb = GeometryPtr(new Translated(origin, psmb));
	
	return psmb;
  }
  return GeometryPtr(0); // the geometric symbol was not defined: default is used
}

const long& LigRecord::getSymbolNumber() const{
  return __symbol;
}

const long& LigRecord::getEntityNumber() const{
  return __entity_number;
}


bifstream& operator>>( bifstream& stream, LigRecord& l){
  l.read(stream);
  return stream;
}

bool LigRecord::read( bifstream& stream){
  if(!stream.eof())stream >> __symbol;
  else return false;
  if(!stream.eof())stream >> __val1;
  else return false;
  if(!stream.eof())stream >> __val2;
  else return false;
  if(!stream.eof())stream >> __range;
  else return false;
  for(int i=0;i<3;i++)
    for(int j=0;j<4;j++)
      if(!stream.eof())stream >>  __matrix[i][j];
      else return false;
  if(!stream.eof())stream >>  __base_dia;
  else return false;
  if(!stream.eof())stream >>  __sommit_dia;
  else return false;
  if(!stream.eof())stream >>  __entity_number;
  else return false;
  return true;
}

bofstream& operator<<( bofstream& stream, const LigRecord& l ){
    stream << l.__symbol << l.__val1 << l.__val2 << l.__range;
    for(int i=0;i<3;i++)
                for(int j=0;j<4;j++)
                        stream <<  l.__matrix[i][j];
    stream <<  l.__base_dia <<  l.__sommit_dia <<  l.__entity_number;
    return stream;
}


/* ----------------------------------------------------------------------- */

Ligfile::Ligfile( const string& fileName, bool bigendian):
    _fileName(fileName),
    recordTable(0){
    bifstream  * stream;
	if(bigendian) stream = new beifstream(_fileName.c_str());
	else stream = new leifstream(_fileName.c_str());
    if(*stream){
        char protection[81];
        stream->read(protection,80);
        recordTable = new vector< LigRecord >;
        while(!stream->eof()){
            LigRecord a;
            if(a.read(*stream))
                 recordTable->push_back(a);
        }
    }
	delete stream;

}

Ligfile::~Ligfile( ) {
    if(recordTable)delete recordTable;
}

const string&
Ligfile::getFileName( ) const {
  return _fileName;
}

const unsigned int Ligfile::getSize( ) const{
    if(!recordTable) return 0;
    else return recordTable->size();
}

ScenePtr Ligfile::computeScene(const Dtafile& _dtafile) const{
    AppearancePtr _default( Material::DEFAULT_MATERIAL);
    if(!isValid() && _dtafile.isValid())return ScenePtr(0);
    ScenePtr result(new Scene());
    uint32_t id = 1;
    for(vector< LigRecord >::iterator _it=recordTable->begin();
        _it!=recordTable->end();
        _it++){
        ShapePtr a = _dtafile.getdtainfo((unsigned int)_it->getSymbolNumber());
        if(a){
            GeometryPtr b = _it->getTransformed(a->getGeometry());
            if(!a->getAppearance())(*SceneObject::errorStream) << "The Dta object " << _it->getSymbolNumber() << " don't have valid Appearance" << endl;
            if(b)result->add(Shape(b,AppearancePtr(a->getAppearance()),_it->getEntityNumber()));
            else  (*SceneObject::errorStream) << "*** Error : The object " << distance(recordTable->begin(),_it) << " cannot be computed" << endl;
        }
        else {
                (*SceneObject::warningStream) << "*** Warning : Cannot find symbol in Dta file for object " << distance(recordTable->begin(),_it) << ". Default geometry used." << endl;
            GeometryPtr b = _it->getTransformed(GeometryPtr(0));
            if(b)result->add(Shape(b,_default,_it->getEntityNumber()));
            else  (*SceneObject::errorStream) << "*** Error : The object " << distance(recordTable->begin(),_it) << " cannot be computed" << endl;
        }
        id++;
    }
    return result;
}


bool Ligfile::isValid( ) const {
    return (recordTable!= NULL);
}

/* ----------------------------------------------------------------------- */

bool Ligfile::isBigEndian( const std::string& fileName ){
	beifstream stream(fileName.c_str());
    if(stream){
        char protection[81];
        stream.read(protection,80);
        if(!stream.eof()){
            LigRecord a;
            if(a.read(stream)){
				if (a.getSymbolNumber() >= 0x1000) return false;
			}
		}
	}
	return true;
}


/* ----------------------------------------------------------------------- */

ScenePtr PGL(readLineTree)(string ligFile, 
							string dtaFile, 
							string smbpath,
							bool bigendian,
							ostream& output){

    string p = get_cwd();
    ScenePtr result(0);
    ostream * errlog = SceneObject::errorStream;
    ostream * warlog = SceneObject::warningStream;
    ostream * comlog = SceneObject::commentStream;

    SceneObject::commentStream = &output;
    SceneObject::warningStream = &output;
    SceneObject::errorStream = &output;
    // (*SceneObject::commentStream) << "Read Line Tree on " << ligFile << " , " << dtaFile << " and on " << smbpath << endl;
    Dtafile d(dtaFile,smbpath);
    if(d.isValid()){
      // (*SceneObject::commentStream) << "Dta File read (" << d.getSize() << ") - Valid"  << endl;
      Ligfile l(ligFile,bigendian);
      // (*SceneObject::commentStream) << "Lig File read (" << l.getSize() << ") - " << (l.isValid()?"Valid":"Invalid") << endl;
      if(!l.isValid()){
                  (*SceneObject::errorStream) << "Error : Invalid Lig File !"  << endl;
                  result = d.getScene();
          }
      else result =l.computeScene(d);
    }
        else {
      (*SceneObject::errorStream) << "Error : Invalid Dta File !"  << endl;
          result = ScenePtr(new Scene());
        }
    chg_dir(p);
    if(!result->isValid()){
                (*SceneObject::errorStream) << "Error : Invalid Scene !" << endl;
        }
    // else (*SceneObject::commentStream) << "Scene of " << result->getSize() << " element(s)" << endl;
    SceneObject::commentStream = comlog;
        SceneObject::warningStream = warlog;
        SceneObject::errorStream = errlog;
        return result;
}


