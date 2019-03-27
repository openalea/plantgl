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


#include "cdc_asc.h"

#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/colorarray.h>
#include <plantgl/scenegraph/geometry/pointset.h>
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/transformation/translated.h>
#include <plantgl/tool/dirnames.h>
#include <plantgl/math/util_vector.h>
#include <plantgl/tool/util_progress.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/algo/base/discretizer.h>
#include <fstream>
#include <iterator>
#include <sstream>
#include <algorithm>

PGL_USING_NAMESPACE

AscCodec::AscCodec() : SceneCodec("ASC", ReadWrite) {
}

SceneFormatList AscCodec::formats() const {
  SceneFormat _format;
  _format.name = "ASC";
  _format.suffixes.push_back("asc");
  _format.suffixes.push_back("pts");
  _format.suffixes.push_back("xyz");
  _format.suffixes.push_back("txt");
  _format.comment = "The Ascii point file format.";
  SceneFormatList _formats;
  _formats.push_back(_format);
  return _formats;
}

ScenePtr AscCodec::read(const std::string &fname) {
  Point3ArrayPtr pts(new Point3Array());
  Color4ArrayPtr col(new Color4Array());

  std::string suffix = get_suffix(fname);

  bool isascfile = (suffix == "asc");
  bool isptsfile = (suffix == "pts");
  bool istxtfile = (suffix == "txt");
  bool ispwnfile = (suffix == "pnw");
  bool isxyzfile = (suffix == "xyz");

  std::ifstream file(fname.c_str());
  if (!file)
    return ScenePtr();

  std::vector<std::string> lines;
  std::string l;
  while (std::getline(file, l)) lines.push_back(l);

  int i = 0;
  std::string sep = " ";
  bool firstline = true;
  ProgressStatus st(lines.size(), "Reading ascii file : %.2f%%");
  for (std::vector<std::string>::iterator line = lines.begin(); line != lines.end(); ++line, ++st) {
    if (firstline) {
      firstline = false;
      if ((isptsfile || ispwnfile))
        continue; // skip line number for .pts and .pwn
      else if (!isxyzfile) {
        if (line->find(";") != std::string::npos) sep = ";";
        if (line->find(",") != std::string::npos) sep = ",";
        if (line->find("\t") != std::string::npos) sep = "\t";
      }
    } else if (line->at(0) == '#')
      continue; // skip comment

    if (sep != ",")
      std::replace(line->begin(), line->end(), ',', '.');

    std::vector<std::string> values = split(*line, sep);
    try {
      float x, y, z;
      std::stringstream(values.at(0)) >> x;
      std::stringstream(values.at(1)) >> y;
      std::stringstream(values.at(2)) >> z;
      pts->push_back(Vector3(x, y, z));
      if (values.size() > 3) {
        int r, g, b;
        if (!isptsfile && !istxtfile) {
          std::stringstream(values.at(3)) >> r;
          std::stringstream(values.at(4)) >> g;
          std::stringstream(values.at(5)) >> b;
          col->push_back(Color4(r, g, b, 0));
        } else {
          std::stringstream(values.at(4)) >> r;
          std::stringstream(values.at(5)) >> g;
          std::stringstream(values.at(6)) >> b;
          col->push_back(Color4(r, g, b, 0));
        }
      }
    }
    catch (std::exception &e) {
      if (!(isptsfile && values.size() == 4)) {
        //warnings.warn("Error in file '"+fname+"' at line "+str(i+isptsfile))
        std::cerr << "Error in file " << fname << " at line " << (i + (int) isptsfile) << " : " << e.what() << std::endl;
        throw e;
      }
    }
    i++;
  }
  file.close();
  Vector3 center = pts->getCenter();
  GeometryPtr pointset(new PointSet(pts, (col->size() == pts->size()) ? col : Color4ArrayPtr()));
  ScenePtr scene(new Scene());
  scene->add(ShapePtr(new Shape(GeometryPtr(new Translated(-center, pointset)))));
  return scene;
}

bool AscCodec::write(const std::string &fname, const ScenePtr &scene) {
  std::cout << "Write " << fname << std::endl;
  std::ofstream file(fname.c_str());

  if (!file)
    return false;

  Discretizer d;
  std::string suffix = get_suffix(fname);

  bool isascfile = (suffix == "asc");
  bool isptsfile = (suffix == "pts");
  bool istxtfile = (suffix == "txt");
  bool ispwnfile = (suffix == "pnw");
  bool isxyzfile = (suffix == "xyz");

  if (isptsfile || ispwnfile) {
    int nbpoints = 0;
    for (Scene::iterator it = scene->begin(); it != scene->end(); ++it) {
      if ((*it)->apply(d)) {
        ExplicitModelPtr e = d.getDiscretization();
        PointSet *pointList = dynamic_cast<PointSet *>(e.get());
        if (pointList)
          nbpoints += pointList->getPointList()->size();
      }
    }
    file << nbpoints << std::endl;
  }
  for (Scene::iterator it = scene->begin(); it != scene->end(); ++it) {
    if ((*it)->apply(d)) {
      ExplicitModelPtr e = d.getDiscretization();
      PointSet *pointList = dynamic_cast<PointSet *>(e.get());
      if (pointList) {
        bool hasColor = pointList->hasColorList();
        // col = i.appearance.ambient
        ProgressStatus st(pointList->getPointList()->size(), "Writing ascii file : %.2f%%");
        for (Point3Array::iterator pt = pointList->getPointList()->begin(); pt != pointList->getPointList()->end(); ++pt, ++st) {
          file << pt->x() << " " << pt->y() << " " << pt->z() << " ";
          if (!isxyzfile && !ispwnfile) {
/*                      if hasColor:
                            col = p.colorList[i]
                        if isptsfile or istxtfile:
                            f.write(str(rgb2intensity(col)))
                        f.write(str(col.red)+' '+str(col.green)+' '+str(col.blue)+'\n')
*/
          }
          file << std::endl;
        }
      }
    }
  }

  file.close();
  return true;
}
