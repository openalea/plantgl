/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
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

#include "cdc_asc.h"

#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/colorarray.h>
#include <plantgl/scenegraph/geometry/pointset.h>
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/transformation/translated.h>
#include <plantgl/tool/dirnames.h>
#include <plantgl/math/util_vector.h>
#include <plantgl/algo/base/discretizer.h>
#include <fstream>
#include <iterator>
#include <sstream>
#include <algorithm>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

AscCodec::AscCodec() : SceneCodec("ASC", ReadWrite ) {
}

SceneFormatList AscCodec::formats() const
{
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

std::vector<std::string> AscCodec::split(const std::string &str, const std::string &delim) const
{
	std::vector<std::string> vec;
	std::size_t start = 0;
	std::size_t end = 0;

	do
	{
		end = str.find(delim, start);
		vec.push_back(str.substr(start, end - start));
		start = end + delim.length();
	} while (end != std::string::npos);
	//vec.push_back(str.substr(start, end));

	std::vector<std::string>::iterator it = vec.begin()+1;
	while (it != vec.end())
	{
		if (it->empty())
			it = vec.erase(it);
		else
			it++;
	}
	return vec;
}

ScenePtr AscCodec::read(const std::string& fname)
{
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

	int i = 0;
	std::string sep = " ";
	std::string line;
	bool firstline = true;
	while (std::getline(file, line))
	{
		if (firstline)
		{
			firstline = false;
			if ((isptsfile || ispwnfile))
				continue; // skip line number for .pts and .pwn
			else if (!isxyzfile)
			{
				if (line.find(";") != std::string::npos) sep = ";";
				if (line.find(",") != std::string::npos) sep = ",";
				if (line.find("\t") != std::string::npos) sep = "\t";
			}
		}
		else if (line[0] == '#')
			continue; // skip comment

        if (sep != ",")
        	std::replace(line.begin(), line.end(), ',', '.');

		std::vector<std::string> values = this->split(line, sep);
		try
		{
			float x, y, z;
			std::stringstream(values.at(0)) >> x;
			std::stringstream(values.at(1)) >> y;
			std::stringstream(values.at(2)) >> z;
			pts->push_back(Vector3(x, y, z));
			if (values.size() > 3)
			{
				int r, g, b;
				if (!isptsfile && !istxtfile)
				{
					std::stringstream(values.at(3)) >> r;
					std::stringstream(values.at(4)) >> g;
					std::stringstream(values.at(5)) >> b;
					col->push_back(Color4(r, g, b, 0));
				}
				else
				{
					std::stringstream(values.at(4)) >> r;
					std::stringstream(values.at(5)) >> g;
					std::stringstream(values.at(6)) >> b;
					col->push_back(Color4(r, g, b, 0));
				}
			}
		}
		catch (std::exception& e)
		{
			if (!(isptsfile and values.size() == 4))
			{
				 //warnings.warn("Error in file '"+fname+"' at line "+str(i+isptsfile))
				std::cerr << "Error in file " << fname << " at line " << (i + (int)isptsfile) << " : " << e.what() << std::endl;
				throw e;
			}
		}
		i++;
	}
	file.close();
	Vector3 center = pts->getCenter();
	GeometryPtr pointset( new PointSet(pts, (col->size() == pts->size()) ? col : Color4ArrayPtr()));
	ScenePtr scene(new Scene());
	scene->add(ShapePtr(new Shape(GeometryPtr(new Translated(-center, pointset)))));
	return scene;
}

bool AscCodec::write(const std::string& fname,const ScenePtr& scene)
{
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

	if (isptsfile || ispwnfile)
	{
		int nbpoints = 0;
		for (Scene::iterator it = scene->begin(); it != scene->end(); ++it)
		{
			if ((*it)->apply(d))
			{
				ExplicitModelPtr e = d.getDiscretization();
				PointSet *pointList = dynamic_cast<PointSet *>(e.get());
				if (pointList)
					nbpoints += pointList->getPointList()->size();
			}
		}
		file << nbpoints << std::endl;
	}
	for (Scene::iterator it = scene->begin(); it != scene->end(); ++it)
	{
		if ((*it)->apply(d))
		{
			ExplicitModelPtr e = d.getDiscretization();
			PointSet *pointList = dynamic_cast<PointSet *>(e.get());
			if (pointList)
			{
				bool hasColor = pointList->hasColorList();
				// col = i.appearance.ambient
				for (Point3Array::iterator pt = pointList->getPointList()->begin(); pt != pointList->getPointList()->end(); ++pt)
				{
					file << pt->x() << " " << pt->y() << " " << pt->z() << " ";
					if (!isxyzfile && !ispwnfile)
					{
/*						if hasColor:
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