/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2002 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): Frederic Boudon (frederic.boudon@cirad.fr)
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

#include "vgsfile.h"
#include <math/util_math.h>
#include <tool/util_string.h>
#include <tool/dirnames.h>
#include <pgl_geometry.h>
#include <pgl_transformation.h>
#include <scenegraph/container/pointarray.h>
#include <scenegraph/container/indexarray.h>
#include <scenegraph/appearance/material.h>


#include <fstream>
#include <qstring.h>
#include <qstringlist.h>
#include <qregexp.h>

/* ----------------------------------------------------------------------- */

TOOLS_USING_NAMESPACE
using namespace std;

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

ShapePtr 
VegeStarLine::build(VGStarColorMap& colormap) const {
	if(!isValid())return ShapePtr(0);
	uint32_t c = color.getRed() + 256*color.getGreen() + 256*256*color.getBlue();
	AppearancePtr mat;
	VGStarColorMap::const_iterator _it = colormap.find(c);
	if(_it == colormap.end()){
		mat = AppearancePtr(new Material(color));
		colormap[c] = mat;
	}
	else mat = _it->second;
	switch(shapeid) {
	case 0: {
		Point3ArrayPtr points(new Point3Array);
		points->pushBack(point1);
		points->pushBack(point2);
		points->pushBack(point3);
		Index3ArrayPtr indices(new Index3Array);
		indices->pushBack(Index3(0,1,2));
		GeometryPtr t(new TriangleSet(points,indices,true,true,false,PolylinePtr(0)));
		return ShapePtr(new Shape(t,mat));
			}
		break;
	case 11:{
		return ShapePtr(new Shape(transform(VegeStarFile::SHAPE_11),mat ));
			}
		break;
	case 12:{
		return ShapePtr(new Shape(transform(VegeStarFile::SHAPE_12),mat ));
			}
		break;
	case 13:{
		return ShapePtr(new Shape(transform(VegeStarFile::SHAPE_13),mat ));
			}
		break;
	case 21: {
		return ShapePtr(new Shape(transform(VegeStarFile::SHAPE_21),mat ));
			}
		break;
	case 22: {
		return ShapePtr(new Shape(transform(VegeStarFile::SHAPE_22),mat ));
			}
		break;
	case 23: {
		return ShapePtr(new Shape(transform(VegeStarFile::SHAPE_23),mat ));
			}
		break;
	case 24: {
		return ShapePtr(new Shape(transform(VegeStarFile::SHAPE_24),mat ));
			}
		break;
	case 31:{
		return ShapePtr(new Shape(transform(VegeStarFile::SHAPE_31),mat ));
			}
		break;
	case 32:{
		if(scaling.y() == scaling.z()){
			GeometryPtr t(new Cylinder(scaling.y()/2,scaling.x(),false));
			t = GeometryPtr(new AxisRotated(Vector3::OY,(real_t)GEOM_HALF_PI,t));
			if(rotation != Vector3::ORIGIN)
				t = GeometryPtr(new EulerRotated(rotation.z()*GEOM_RAD,
												 rotation.y()*GEOM_RAD,
												 rotation.x()*GEOM_RAD,
												 t));
			if(translation != Vector3::ORIGIN)
				t = GeometryPtr(new Translated(translation,t));
			return ShapePtr(new Shape(t,mat ));
		}
		else {
			return ShapePtr(new Shape(transform(VegeStarFile::SHAPE_32),mat ));
		}
			}
		break;
	case 33:{
		GeometryPtr t(new Frustum(scaling.y()/2,scaling.x(),scaling.z()/scaling.y(),false));
		t = GeometryPtr(new AxisRotated(Vector3::OY,(real_t)GEOM_HALF_PI,t));
		if(rotation != Vector3::ORIGIN)
			t = GeometryPtr(new EulerRotated(rotation.z()*GEOM_RAD,
											 rotation.y()*GEOM_RAD,
											 rotation.x()*GEOM_RAD,
											 t));
		if(translation != Vector3::ORIGIN)
			t = GeometryPtr(new Translated(translation,t));
		return ShapePtr(new Shape(t,mat));
			}
		break;
	case 40: {
		Point3ArrayPtr points(new Point3Array);
		points->pushBack(point1);
		GeometryPtr t(new PointSet(points));
		return ShapePtr(new Shape(t,mat));
			}
		break;
	default:
		return ShapePtr(0);
	}
	return ShapePtr(0);
}

GeometryPtr VegeStarLine::transform(GeometryPtr geom) const {
	GeometryPtr t = geom;
	if(scaling != Vector3::ORIGIN && scaling != Vector3(1.,1.,1.))
		t = GeometryPtr(new Scaled(scaling,t));
	if(rotation != Vector3::ORIGIN)
		t = GeometryPtr(new EulerRotated(rotation.z()*GEOM_RAD,
										 rotation.y()*GEOM_RAD,
										 rotation.x()*GEOM_RAD,
										 t));
	if(translation != Vector3::ORIGIN)
		t = GeometryPtr(new Translated(translation,t));
	return t;
}

/* ----------------------------------------------------------------------- */

// Predefined Polygonal Shape
GeometryPtr VegeStarFile::SHAPE_11(0);
GeometryPtr VegeStarFile::SHAPE_12(0);
GeometryPtr VegeStarFile::SHAPE_13(0);

// Surface
GeometryPtr VegeStarFile::SHAPE_21(0);
GeometryPtr VegeStarFile::SHAPE_22(0);
GeometryPtr VegeStarFile::SHAPE_23(0);
GeometryPtr VegeStarFile::SHAPE_24(0);

// Volume
GeometryPtr VegeStarFile::SHAPE_31(0);
GeometryPtr VegeStarFile::SHAPE_32(0);

/* ----------------------------------------------------------------------- */

bool 
VegeStarFile::parse(const std::string& filename)
{
	ifstream stream(filename.c_str());
	if(!stream){
		*SceneObject::errorStream << "Cannot open file :'" 
			<< filename << '\'' << endl;
		return false;
	}
	else return parse(stream);
}

bool 
VegeStarFile::parse(std::istream& stream)
{
	if(!parseHeader(stream))return false;
	else {
		while(!stream.eof())parseLine(stream);
		return true;
	}
}

#define MAXLINELENGTH 500

inline QStringList splitString(const QString& t){
#if QT_VERSION >= 0x040000
	return t.split(QRegExp("[ \t\n;,]"));
#else
	return QStringList::split(QRegExp("[ \t\n;,]"),t);
#endif
}

bool 
VegeStarFile::parseHeader(std::istream& stream)
{
	char line[MAXLINELENGTH]; 
	stream.getline(line,MAXLINELENGTH);
	QString t(line);
	QStringList tokens = splitString(t);
	for(int it = 0; it < tokens.size(); ++it){
		QString token = tokens[it];
		if(token == "Obj")attlist.push_back(obj);
		else if(token == "EchX")attlist.push_back(EchX);
		else if(token == "EchY")attlist.push_back(EchY);
		else if(token == "EchZ")attlist.push_back(EchZ);
		else if(token == "TransX")attlist.push_back(TransX);
		else if(token == "TransY")attlist.push_back(TransY);
		else if(token == "TransZ")attlist.push_back(TransZ);
		else if(token == "RotX")attlist.push_back(RotX);
		else if(token == "RotY")attlist.push_back(RotY);
		else if(token == "RotZ")attlist.push_back(RotZ);
		else if(token == "R")attlist.push_back(R);
		else if(token == "G")attlist.push_back(G);
		else if(token == "B")attlist.push_back(B);
		else if(token == "X1")attlist.push_back(X1);
		else if(token == "Y1")attlist.push_back(Y1);
		else if(token == "Z1")attlist.push_back(Z1);
		else if(token == "X2")attlist.push_back(X2);
		else if(token == "Y2")attlist.push_back(Y2);
		else if(token == "Z2")attlist.push_back(Z2);
		else if(token == "X3")attlist.push_back(X3);
		else if(token == "Y3")attlist.push_back(Y3);
		else if(token == "Z3")attlist.push_back(Z3);
		else if(token == "Mask")attlist.push_back(Mask);
		else if(token == "Grp1")attlist.push_back(Grp1);
		else if(token == "Grp2")attlist.push_back(Grp2);
		else if(token == "Pmax")attlist.push_back(Pmax);
		else if(token == "Alpha")attlist.push_back(Alpha);
		else if(token == "Teta")attlist.push_back(Teta);
		else if(token == "Resp")attlist.push_back(Resp);
		else if(token == "VCmax")attlist.push_back(VCmax);
		else if(token == "Jmax")attlist.push_back(Jmax);
		else {
			attlist.push_back(Error);
			*SceneObject::errorStream << "Unrecognized header token : '" << token << '\'' << endl;
		}
	}
	return true;
}

bool 
VegeStarFile::parseLine(std::istream& stream)
{
	VegeStarLine l;
	real_t val = 0;
	char line[MAXLINELENGTH]; 
	stream.getline(line,MAXLINELENGTH);

	QString t(line);
	QStringList tokens = splitString(t);

	for(size_t i = 0; i < attlist.size() && i < tokens.size(); i++){
		val = tokens[i].toDouble();
		VgstarItem item = attlist[i];
		if(stream.eof())return false;
		switch(item){
		case obj :  {l.shapeid = (int)val;}break; 
		case EchX : {l.scaling.x() = val;}break; 
		case EchY : {l.scaling.y() = val;}break; 
		case EchZ : {l.scaling.z() = val;}break; 
		case TransX : {l.translation.x() = val;}break;
		case TransY : {l.translation.y() = val;}break;
		case TransZ : {l.translation.z() = val;}break;
		case RotX : {l.rotation.x() = val;}break; 
		case RotY : {l.rotation.y() = val;}break; 
		case RotZ : {l.rotation.z() = val;}break; 
		case R : {l.color.getRed() = (uchar_t)val;}break; 
		case G : {l.color.getGreen() = (uchar_t)val;}break; 
		case B : {l.color.getBlue() = (uchar_t)val;}break; 
		case X1 : {l.point1.x() = val;}break; 
		case Y1 : {l.point1.y() = val;}break; 
		case Z1 : {l.point1.z() = val;}break; 
		case X2 : {l.point2.x() = val;}break; 
		case Y2 : {l.point2.y() = val;}break; 
		case Z2 : {l.point2.z() = val;}break; 
		case X3 : {l.point3.x() = val;}break; 
		case Y3 : {l.point3.y() = val;}break; 
		case Z3 : {l.point3.z() = val;}break;
		default : break;
		}
	}
	if(l.isValid())objlist.push_back(l);
	return true;
}

ScenePtr 
VegeStarFile::build() const
{
	ScenePtr scene(new Scene);
	VGStarColorMap colormap;
	int i = 2;
	for(std::vector<VegeStarLine>::const_iterator it = objlist.begin();
		it != objlist.end(); it++){
		ShapePtr shape = it->build(colormap);
		if(shape && shape.isValid()){
			shape->getId() = i;
			shape->setName("SHAPE_"+number(i));
			i++;
			scene->add(*shape);
		}
	}
	return scene;
}

/* ----------------------------------------------------------------------- */

ScenePtr VegeStarFile::read(const std::string& filename,
						  ostream& error){

  ScenePtr result;
  string p = get_cwd();
  ostream * errlog = SceneObject::errorStream;
  ostream * warlog = SceneObject::warningStream;
  ostream * infolog = SceneObject::commentStream;
  SceneObject::commentStream = &error;
  SceneObject::warningStream = &error;
  SceneObject::errorStream = &error;
  VegeStarFile f;
  if(f.parse(filename))
	result = f.build();

  SceneObject::commentStream = infolog;
  SceneObject::warningStream = warlog;
  SceneObject::errorStream = errlog;
  chg_dir(p);
  return result;
}


void 
VegeStarFile::initShape(){
	
	if(SHAPE_11.isNull()||SHAPE_12.isNull()||SHAPE_13.isNull()){
		Point3ArrayPtr points(new Point3Array);
		points->pushBack(Vector3(0.5,0,0));
		points->pushBack(Vector3(0,0,0));
		points->pushBack(Vector3(1./3.,-0.5,0));
		points->pushBack(Vector3(2./3.,-0.5,0));
		points->pushBack(Vector3(1,0,0));
		points->pushBack(Vector3((real_t)2./3.,(real_t)0.5,0));
		points->pushBack(Vector3((real_t)1./3.,(real_t)0.5,0));
		Index3ArrayPtr indices(new Index3Array);
		indices->pushBack(Index3(0,2,1));
		indices->pushBack(Index3(0,3,2));
		indices->pushBack(Index3(0,4,3));
		indices->pushBack(Index3(0,5,4));
		indices->pushBack(Index3(0,6,5));
		indices->pushBack(Index3(0,1,6));
		if(!SHAPE_11){
			SHAPE_11 = GeometryPtr(new TriangleSet(points,indices,true,true,false,PolylinePtr(0)));
			SHAPE_11->setName("VGX_SHAPE_11");
		}
		if(!SHAPE_12){
			SHAPE_12 = GeometryPtr(new TriangleSet(points,indices,true,true,false,PolylinePtr(0)));
			SHAPE_12->setName("VGX_SHAPE_12");
		}
		if(!SHAPE_13){
			SHAPE_13 = GeometryPtr(new TriangleSet(points,indices,true,true,false,PolylinePtr(0)));
			SHAPE_13->setName("VGX_SHAPE_13");
		}
	}
	if(!SHAPE_21){
		SHAPE_21 = GeometryPtr(new Translated(Vector3(0.5,0,0),GeometryPtr(new Disc(0.5))));
		SHAPE_21->setName("VGX_SHAPE_21");
	}

	if(!SHAPE_22){
		Point3ArrayPtr points(new Point3Array);
		points->pushBack(Vector3(0,-0.5,0));
		points->pushBack(Vector3(0,0.5,0));
		points->pushBack(Vector3(1,0.5,0));
		points->pushBack(Vector3(1,-0.5,0));
		Index4ArrayPtr indices4(new Index4Array);
		indices4->pushBack(Index4(0,1,2,3));
		SHAPE_22 = GeometryPtr(new Translated(Vector3(0.5,0,0),
			GeometryPtr(new QuadSet(points,indices4,true,true,false,PolylinePtr(0)))));
		SHAPE_22->setName("VGX_SHAPE_22");
	}
	
	if(!SHAPE_23){
		Point3ArrayPtr points(new Point3Array);
		points->pushBack(Vector3(1,0,0));
		int slices = 1 + SOR::DEFAULT_SLICES / 4;
		for(int i = 1; i <= slices; i++)
			points->pushBack(Vector3(Vector3::Cylindrical(1,(i*GEOM_PI)/(2*slices),0)));
		points->pushBack(Vector3(0,0,0));
		Index3ArrayPtr indices(new Index3Array);
		for(int j = 0; j < slices; j++)
			indices->pushBack(Index3(j,j+1,slices+1));
		SHAPE_23 = GeometryPtr(new Translated(Vector3(0.5,0,0),
			GeometryPtr(new TriangleSet(points,indices,true,true,false,PolylinePtr(0)))));
		SHAPE_23->setName("VGX_SHAPE_23");
	}
	
	if(!SHAPE_24){
		Point3ArrayPtr points(new Point3Array);
		points->pushBack(Vector3::ORIGIN);
		points->pushBack(Vector3::OY);
		points->pushBack(Vector3::OX);
		Index3ArrayPtr indices(new Index3Array);
		indices->pushBack(Index3(0,1,2));
		SHAPE_24 = GeometryPtr(new TriangleSet(points,indices,true,true,false,PolylinePtr(0)));
		SHAPE_24->setName("VGX_SHAPE_24");
	}

	if(!SHAPE_31){
		SHAPE_31 = GeometryPtr(new Sphere);
		SHAPE_31->setName("VGX_SHAPE_31");
	}

	if(!SHAPE_32){
		SHAPE_32 = GeometryPtr(new AxisRotated(Vector3::OY,GEOM_HALF_PI,
								GeometryPtr(new Cylinder((real_t)0.5,1,false))));
		SHAPE_32->setName("VGX_SHAPE_32");
	}

}

void 
VegeStarFile::setShape11(GeometryPtr geom){
	SHAPE_11 = geom;
}

void 
VegeStarFile::setShape12(GeometryPtr geom){
	SHAPE_12 = geom;
}

void 
VegeStarFile::setShape13(GeometryPtr geom){
	SHAPE_13 = geom;
}

GeometryPtr
VegeStarFile::getShape11(){
  initShape();
  return SHAPE_11;
}

GeometryPtr
VegeStarFile::getShape12(){
  initShape();
  return SHAPE_12;
}

GeometryPtr 
VegeStarFile::getShape13(){
  initShape();
  return SHAPE_13;
}

GeometryPtr
VegeStarFile::importPolygonFile(const string& filename, ostream& error){
  GeometryPtr result(0);
  string p = get_cwd();
  ostream * errlog = SceneObject::errorStream;
  ostream * warlog = SceneObject::warningStream;
  ostream * infolog = SceneObject::commentStream;
  SceneObject::commentStream = &error;
  SceneObject::warningStream = &error;
  SceneObject::errorStream = &error;
  ifstream stream(filename.c_str());
  if(!stream){
	  *SceneObject::errorStream << "Cannot open file :'" 
		  << filename << '\'' << endl;
  }
  else {
	  int vnb = 0;
	  real_t surf = 0;
	  real_t x,y;
	  stream >> vnb;
	  stream >> surf;
	  Point3ArrayPtr points(new Point3Array(vnb));
	  IndexArrayPtr indices(new IndexArray(1));
	  if(vnb != 0 && !stream.eof()){
		  bool err = false;
		  for(int i = 0; i < vnb && !(err = stream.eof()); i++){
			stream >> x >> y;
			points->setAt(i,Vector3(x,y,0));
		  }
		  if(!stream.eof()){
			stream >> x >> y;
			if(points->getAt(0) != Vector3(x,y,0)){
				points->pushBack(Vector3(x,y,0));
				Index index(vnb+1);
				for(int j = 0; j < vnb+1 ; j++)index.setAt(j,j);
				indices->setAt(0,index);
			}
			else{
				Index index(vnb);
				for(int j = 0; j < vnb ; j++)index.setAt(j,j);
				indices->setAt(0,index);
			}
		  }
		  if(!err){
			  result = GeometryPtr(new FaceSet(points,indices,true,true,false,PolylinePtr(0)));
			  string n = get_filename(filename);
			  size_t pos = n.find_last_of('.');
			  if(pos != string::npos){
				  n.erase(n.begin()+pos,n.end());
			  }
			  result->setName(n);
		  }
	  }
  }
  
  SceneObject::commentStream = infolog;
  SceneObject::warningStream = warlog;
  SceneObject::errorStream = errlog;
  chg_dir(p);
  return result;

}
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE
