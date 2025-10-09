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


#include "vgsfile.h"
#include <plantgl/math/util_math.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/tool/dirnames.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/appearance/material.h>


#include <fstream>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
using namespace std;

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */
 
const char * VegeStarFile::VgstarItemName[32] = { "obj", "EchX", "EchY", "EchZ", "TransX", "TransY", "TransZ", "RotX", "RotY", "RotZ", "R", "G", "B", "X1", "Y1", "Z1", "X2", "Y2", "Z2", "X3", "Y3", "Z3", "Mask", "Grp1", "Grp2", "Pmax", "Alpha",  "Teta",  "Resp", "VCmax", "Jmax", "Error"};

ShapePtr 
VegeStarLine::build(VGStarColorMap& colormap) const {
    if(!isValid())return ShapePtr();
    uint_t c = color.getRed() + 256*color.getGreen() + 256*256*color.getBlue();
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
        points->push_back(point1);
        points->push_back(point2);
        points->push_back(point3);
        Index3ArrayPtr indices(new Index3Array);
        indices->push_back(Index3(0,1,2));
        GeometryPtr t(new TriangleSet(points,indices,true,true,false,PolylinePtr()));
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
        points->push_back(point1);
        GeometryPtr t(new PointSet(points));
        return ShapePtr(new Shape(t,mat));
            }
        break;
    default:
        return ShapePtr();
    }
    return ShapePtr();
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
GeometryPtr VegeStarFile::SHAPE_11;
GeometryPtr VegeStarFile::SHAPE_12;
GeometryPtr VegeStarFile::SHAPE_13;

// Surface
GeometryPtr VegeStarFile::SHAPE_21;
GeometryPtr VegeStarFile::SHAPE_22;
GeometryPtr VegeStarFile::SHAPE_23;
GeometryPtr VegeStarFile::SHAPE_24;

// Volume
GeometryPtr VegeStarFile::SHAPE_31;
GeometryPtr VegeStarFile::SHAPE_32;

/* ----------------------------------------------------------------------- */

bool 
VegeStarFile::parse(const std::string& filename)
{
    ifstream stream(filename.c_str());
    if(!stream){
        pglError(PGLERRORMSG(FILE_OPEN_ERR_s),filename.c_str());
        return false;
    }
    else return parse(stream);
}

bool 
VegeStarFile::parse(std::istream& stream)
{
    if(!parseHeader(stream))return false;
    else {
        uint32_t linenb = 2;
        while(!stream.eof())parseLine(stream, linenb++);
        return true;
    }
}

#define MAXLINELENGTH 500

size_t find_next(const std::string s, size_t pos) {
    std::string separators = "\t\r\n;,";
    size_t next = std::string::npos;
    for (std::string::const_iterator it = separators.begin(); it != separators.end(); ++it){
        size_t npos = s.find(*it, pos);
        if (npos != std::string::npos) {
            if (next != std::string::npos)  next = npos;
            else if(next > npos) next = npos;
        }
    }
    return next;

}

std::string get_line(std::istream& stream)
{
    char cline[MAXLINELENGTH]; 
    stream.getline(cline,MAXLINELENGTH);
    std::string line(cline);
    if (*(line.end()-1) == '\r'){
        line.erase(line.end()-1);
    }
    return line;
}

std::vector<std::string> vgs_split(const std::string& s)
{
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;
    while((pos = find_next(s, pos)) != std::string::npos)
    {
        std::string substring( s.substr(prev_pos, pos-prev_pos) );
        output.push_back(substring);
        prev_pos = ++pos;
    }

    output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word
    return output;
}

bool 
VegeStarFile::parseHeader(std::istream& stream)
{
    std::string line = get_line(stream);
    if (line.size() == 0) return false;
    
    std::vector<std::string> tokens = vgs_split(line);
    for(int it = 0; it < tokens.size(); ++it){
        std::string token = tokens[it];
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
            pglError("Unrecognized header token : '%s'.\n", token.c_str() );
        }
    }
    return true;
}

bool 
VegeStarFile::parseLine(std::istream& stream, uint32_t linenb)
{
    if(stream.eof())return false;
    VegeStarLine l;
    std::string line = get_line(stream);
    if (line.size() == 0) return false;

    std::vector<std::string> tokens = vgs_split(line);
    if(tokens.size() != attlist.size()){
        pglError("Invalid token number in line %u: '%s'.\n", linenb, line.c_str() );
        return false;
    }

    for(size_t i = 0; i < attlist.size() && i < tokens.size(); i++){
        real_t val = 0;
        setlocale(LC_ALL,"C");
        val = atof(tokens[i].c_str());
        VgstarItem item = attlist[i];
        // printf("'%s'=%f (%s)\n", VgstarItemName[item], val, tokens[i].c_str());

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
        if(shape && shape->isValid()){
            shape->getId() = i;
            shape->setName("SHAPE_"+number(i));
            i++;
            scene->add(ShapePtr(new Shape(*shape)));
        }
    }
    return scene;
}

/* ----------------------------------------------------------------------- */

ScenePtr VegeStarFile::read(const std::string& filename,
                          ostream& error){

  ScenePtr result;
  string p = get_cwd();
  PglErrorStream::Binder psb(error);
  VegeStarFile f;
  if(f.parse(filename))
    result = f.build();
  chg_dir(p);
  return result;
}


void 
VegeStarFile::initShape(){
    
    if(!SHAPE_11||!SHAPE_12||!SHAPE_13){
        Point3ArrayPtr points(new Point3Array);
        points->push_back(Vector3(0.5,0,0));
        points->push_back(Vector3(0,0,0));
        points->push_back(Vector3(1./3.,-0.5,0));
        points->push_back(Vector3(2./3.,-0.5,0));
        points->push_back(Vector3(1,0,0));
        points->push_back(Vector3((real_t)2./3.,(real_t)0.5,0));
        points->push_back(Vector3((real_t)1./3.,(real_t)0.5,0));
        Index3ArrayPtr indices(new Index3Array);
        indices->push_back(Index3(0,2,1));
        indices->push_back(Index3(0,3,2));
        indices->push_back(Index3(0,4,3));
        indices->push_back(Index3(0,5,4));
        indices->push_back(Index3(0,6,5));
        indices->push_back(Index3(0,1,6));
        if(!SHAPE_11){
            SHAPE_11 = GeometryPtr(new TriangleSet(points,indices,true,true,false,PolylinePtr()));
            SHAPE_11->setName("VGX_SHAPE_11");
        }
        if(!SHAPE_12){
            SHAPE_12 = GeometryPtr(new TriangleSet(points,indices,true,true,false,PolylinePtr()));
            SHAPE_12->setName("VGX_SHAPE_12");
        }
        if(!SHAPE_13){
            SHAPE_13 = GeometryPtr(new TriangleSet(points,indices,true,true,false,PolylinePtr()));
            SHAPE_13->setName("VGX_SHAPE_13");
        }
    }
    if(!SHAPE_21){
        SHAPE_21 = GeometryPtr(new Translated(Vector3(0.5,0,0),GeometryPtr(new Disc(0.5))));
        SHAPE_21->setName("VGX_SHAPE_21");
    }

    if(!SHAPE_22){
        Point3ArrayPtr points(new Point3Array);
        points->push_back(Vector3(0,-0.5,0));
        points->push_back(Vector3(0,0.5,0));
        points->push_back(Vector3(1,0.5,0));
        points->push_back(Vector3(1,-0.5,0));
        Index4ArrayPtr indices4(new Index4Array);
        indices4->push_back(Index4(0,1,2,3));
        SHAPE_22 = GeometryPtr(new Translated(Vector3(0.5,0,0),
            GeometryPtr(new QuadSet(points,indices4,true,true,false,PolylinePtr()))));
        SHAPE_22->setName("VGX_SHAPE_22");
    }
    
    if(!SHAPE_23){
        Point3ArrayPtr points(new Point3Array);
        points->push_back(Vector3(1,0,0));
        int slices = 1 + SOR::DEFAULT_SLICES / 4;
        for(int i = 1; i <= slices; i++)
            points->push_back(Vector3(Vector3::Cylindrical(1,(i*GEOM_PI)/(2*slices),0)));
        points->push_back(Vector3(0,0,0));
        Index3ArrayPtr indices(new Index3Array);
        for(int j = 0; j < slices; j++)
            indices->push_back(Index3(j,j+1,slices+1));
        SHAPE_23 = GeometryPtr(new Translated(Vector3(0.5,0,0),
            GeometryPtr(new TriangleSet(points,indices,true,true,false,PolylinePtr()))));
        SHAPE_23->setName("VGX_SHAPE_23");
    }
    
    if(!SHAPE_24){
        Point3ArrayPtr points(new Point3Array);
        points->push_back(Vector3::ORIGIN);
        points->push_back(Vector3::OY);
        points->push_back(Vector3::OX);
        Index3ArrayPtr indices(new Index3Array);
        indices->push_back(Index3(0,1,2));
        SHAPE_24 = GeometryPtr(new TriangleSet(points,indices,true,true,false,PolylinePtr()));
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
  GeometryPtr result;
  string p = get_cwd();
  PglErrorStream::Binder psb(error);
  ifstream stream(filename.c_str());
  if(!stream){
      pglError(PGLERRORMSG(FILE_OPEN_ERR_s),filename.c_str());
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
                points->push_back(Vector3(x,y,0));
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
              result = GeometryPtr(new FaceSet(points,indices,true,true,false,PolylinePtr()));
              string n = get_filename(filename);
              size_t pos = n.find_last_of('.');
              if(pos != string::npos){
                  n.erase(n.begin()+pos,n.end());
              }
              result->setName(n);
          }
      }
  }
  
  chg_dir(p);
  return result;

}
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE
