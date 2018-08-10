/*
** cdc_ply.cpp for plantgl in
**
** Made by julien1.benoit@epitech.eu
** Login   <julien1.benoit@epitech.eu>
**
** Started on  jeu. juil. 13:23 2018 Julien Benoit
** Last update jeu. juil. 13:23 2018 Julien Benoit
*/

#include "cdc_ply.h"
#include <plantgl/tool/bfstream.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/colorarray.h>
#include <plantgl/scenegraph/geometry/pointset.h>
#include <plantgl/scenegraph/geometry/faceset.h>
#include <plantgl/tool/util_progress.h>
#include "plyprinter.h"
#include <fstream>
#include <boost/bind.hpp>
#include <boost/function.hpp>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

PlyCodec::PlyCodec() : SceneCodec("PLY", ReadWrite) {
  this->fcodingTypes.push_back("binary_little_endian");
  this->fcodingTypes.push_back("binary_big_endian");
  this->fcodingTypes.push_back("ascii");

  propertiesTypes["float"] = float();
  propertiesTypes["float32"] = float();
  propertiesTypes["int"] = int();
  propertiesTypes["int8"] = int8_t();
  propertiesTypes["int16"] = int16_t();
  propertiesTypes["int32"] = int32_t();
  propertiesTypes["uint"] = uint_t();
  propertiesTypes["uint8"] = uint8_t();
  propertiesTypes["uint16"] = uint16_t();
  propertiesTypes["uint32"] = uint32_t();
  propertiesTypes["uchar"] = uchar_t();
  propertiesTypes["char"] = char();
  propertiesTypes["short"] = short();
  propertiesTypes["ushort"] = u_short();
  propertiesTypes["double"] = double();

  knowncolortypes.push_back("red");
  knowncolortypes.push_back("green");
  knowncolortypes.push_back("blue");
  knowncolortypes.push_back("diffuse_red");
  knowncolortypes.push_back("diffuse_green");
  knowncolortypes.push_back("diffuse_blue");
  knowncolortypes.push_back("alpha");
}

SceneFormatList PlyCodec::formats() const {
  SceneFormat _format;
  _format.name = "Ply Codec";
  _format.suffixes.push_back("ply");
  _format.comment = "The ply format.";
  SceneFormatList _formats;
  _formats.push_back(_format);
  return _formats;
}

ScenePtr PlyCodec::read(const std::string &fname) {
  std::ifstream file(fname.c_str());
  if (!file)
    return ScenePtr();

  std::string header = this->nextline(file);
  assert(TOOLS(strip)(header) == "ply");

  std::string format = this->nextline(file);
  std::string fkeyword, fcoding;
  float fversion;
  {
    std::vector<std::string> sformat = TOOLS(split)(format);
    fkeyword = sformat[0];
    fcoding = sformat[1];
    fversion = TOOLS(toNumber)<float>(sformat[2]);
  }

  assert(std::find(this->fcodingTypes, fcoding) != this->fcodingTypes.end());
  bool reversebytes = false;
  if (fcoding != "ascii") {
    std::string byteorder = TOOLS(split)(fcoding, "_")[1];
    if (byteorder == "little" && __BYTE_ORDER == __BIG_ENDIAN)
      reversebytes = true;
    else if (byteorder == "big" && __BYTE_ORDER == __LITTLE_ENDIAN)
      reversebytes = true;
  }
  std::map<std::string, SpecElement> spec;
  std::string nline = this->nextline(file);
  std::string lastSpecName;
  while (nline != "end_header") {
    if (nline.find("element") == 0) {
      std::string elementkwd, elementname, elementnb;
      std::vector<std::string> elements = split(nline);
      elementkwd = elements[0];
      elementname = elements[1];
      elementnb = elements[2];
      spec[elementname] = SpecElement(toNumber<std::size_t>(elementnb));
      lastSpecName = elementname;
    } else if (nline.find("property") == 0) {
      std::vector<std::string> properties = split(nline);
      if (properties.size() == 3) {
        std::string propkwd, proptype, propname;
        propkwd = properties[0];
        proptype = properties[1];
        propname = properties[2];
        assert(this->propertiesTypes.find(proptype) != this->propertiesTypes.end());
        spec[lastSpecName].properties.push_back(PropertyElement(propname, proptype));
      } else if (properties.size() == 5) {
        std::string propkwd, proplist, propsizetype, proptype, propname;
        propkwd = properties[0];
        proplist = properties[1];
        propsizetype = properties[2];
        proptype = properties[3];
        propname = properties[4];
        assert(this->propertiesTypes.find(propsizetype) != this->propertiesTypes.end());
        assert(this->propertiesTypes.find(proptype) != this->propertiesTypes.end());
        spec[lastSpecName].properties.push_back(PropertyElement(propname, propsizetype, proptype));
      }
    }
    nline = this->nextline(file);
  }

  std::vector<std::string> colorprops;
  if (spec.find("vertex") != spec.end()) {
    SpecElement &specVertex = spec["vertex"];
    for (size_t i = 2; i < specVertex.properties.size(); i++) {
      if (std::find(this->knowncolortypes.begin(), this->knowncolortypes.end(), specVertex.properties[i].name) != this->knowncolortypes.end())
        colorprops.push_back(specVertex.properties[i].name);
    }
  }
  if (!colorprops.empty()) {
    std::sort(colorprops.begin(), colorprops.end(), boost::bind(&PlyCodec::colorPropsSortFunction, this, _1, _2));
    assert(colorprops.size() == 3 || colorprops.size() == 4);
  }

  std::size_t vertexSize = (spec.find("vertex") != spec.end()) ? spec["vertex"].number : 0;
  std::size_t colorSize = (!colorprops.empty()) ? vertexSize : 0;
  std::size_t faceSize = (spec.find("face") != spec.end()) ? spec["face"].number : 0;
  Point3ArrayPtr points(new Point3Array(vertexSize));
  Color4ArrayPtr colors(new Color4Array(colorSize));
  IndexArrayPtr faces(new IndexArray(faceSize));

  std::cout << "vertexSize = " << spec["vertex"].number << std::endl;
  std::cout << "colorSize = " << colorSize << std::endl;
  std::cout << "faceSize = " << faceSize << std::endl;

  for (std::map<std::string, SpecElement>::iterator s = spec.begin(); s != spec.end(); ++s) {
    ProgressStatus st(s->second.number, "PLY: Reading " + s->first + " part : %.2f%%");
    for (size_t i = 0; i < s->second.number; i++, ++st) {
      std::map<std::string, std::vector<propertyType> > ielement;
      if (fcoding == "ascii") {
        std::vector<std::string> linevalues = split(this->nextline(file));
        size_t itv = 0;
        for (std::vector<PropertyElement>::iterator p = s->second.properties.begin(); p != s->second.properties.end(); ++p) {
          std::vector<propertyType> values;
          propertyType type = this->propertiesTypes[p->type];
          if (p->isList) {
            size_t nbpropv = TOOLS(toNumber)<size_t>(linevalues[itv]); itv++;

            for (size_t j = 0; j < nbpropv; j++) {
              ReadVisitor v(linevalues[itv]); itv++;
              boost::apply_visitor(v)(type);
              values.push_back(v.get_value());
            }
          } else {
            ReadVisitor visitor(linevalues[itv]); itv++;
            boost::apply_visitor(visitor)(type);
            values.push_back(visitor.get_value());
          }
          ielement[p->name] = values;
        }
      } else {
        for (std::vector<PropertyElement>::iterator p = s->second.properties.begin(); p != s->second.properties.end(); ++p) {
          std::vector<propertyType> values;
          propertyType type = this->propertiesTypes[p->type];
          if (p->isList) {
            propertyType sizetype = this->propertiesTypes[p->sizetype];
            GetVisitor<size_t> visitor;
            propertyType tmp = this->readnextval(file, sizetype, reversebytes);
            tmp.apply_visitor(visitor);
            size_t nbpropv = visitor.get_value();

            for (size_t j = 0; j < nbpropv; j++)
              values.push_back(this->readnextval(file, type, reversebytes));
          } else
            values.push_back(this->readnextval(file, type, reversebytes));
          ielement[p->name] = values;
        }
      }
      if (s->first == "vertex") {
        GetVisitor<float> visitor;
        ielement["x"][0].apply_visitor(visitor);
        float x = visitor.get_value();
        ielement["y"][0].apply_visitor(visitor);
        float y = visitor.get_value();
        ielement["z"][0].apply_visitor(visitor);
        float z = visitor.get_value();
        points->setAt(i, TOOLS(Vector3)(x, y, z));
        if (!colorprops.empty()) {
          uchar_t rgba[4];
          int index = 0;

          for (std::vector<std::string>::iterator cni = colorprops.begin(); cni != colorprops.end(); ++cni, index++) {
            GetVisitor<uchar_t> v;
            ielement[*cni][0].apply_visitor(v);
            rgba[index] = v.get_value();
          }
          if (colorprops[3] == "alpha")
            rgba[3] = 255 - rgba[3];
          else
            rgba[3] = 0;
          colors->setAt(i, Color4(rgba));
        }
      } else if (s->first == "face") {
        std::vector<propertyType> indices = ielement["vertex_indices"];
        GetVisitor<uint_t> visitor;
        Index index;
        for (std::vector<propertyType>::iterator v = indices.begin(); v != indices.end(); ++v) {
          v->apply_visitor(visitor);
          index.push_back(visitor.get_value());
        }
        faces->setAt(i, index);
      }
    }
  }
  file.close();

  ScenePtr scene(new Scene());
  ShapePtr shape;
  if (faces->empty())
    shape = new Shape(GeometryPtr(new PointSet(points, colors)));
  else
    shape = new Shape(GeometryPtr(new FaceSet(points, faces)));
  scene->add(shape);
  return scene;
}

bool PlyCodec::write(const std::string &fname, const PGL::ScenePtr &scene) {
  return PlyPrinter::print(scene, fname, NULL, PlyPrinter::ply_binary_little_endian);
}

std::string PlyCodec::nextline(std::ifstream &file) {
  std::string line;
  do {
    std::getline(file, line);
  } while (line.find("comment") == 0);
  return TOOLS(strip)(line);
}

PlyCodec::propertyType PlyCodec::readnextval(std::ifstream &file, const PGL::PlyCodec::propertyType &type, const bool &reversebytes) {
  boost::apply_visitor(this->sizeVisitor)(type);
  size_t size = this->sizeVisitor.get_size();

  char *tmp = new char[size];
  char *value = new char[size];
  file.read(tmp, size);
  if (reversebytes) TOOLS(flipBytes)(tmp, value, size);
  else memcpy(value, tmp, size);

  ReinterpretVisitor visitor(value);
  boost::apply_visitor(visitor)(type);
  delete [] tmp;
  delete [] value;
  return visitor.get_res();
}

bool PlyCodec::colorPropsSortFunction(const std::string &c1, const std::string &c2) {
  size_t indexc1 = std::distance(this->knowncolortypes.begin(), std::find(this->knowncolortypes.begin(), this->knowncolortypes.end(), c1));
  size_t indexc2 = std::distance(this->knowncolortypes.begin(), std::find(this->knowncolortypes.begin(), this->knowncolortypes.end(), c2));

  return indexc1 < indexc2;
}