/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       GeomPy: Python wrapper for the Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR AMAP 
 *
 *       File author(s): C. Pradal (christophe.pradal@cirad.fr)
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

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

#include <algo/base/bboxcomputer.h>
#include <algo/base/discretizer.h>
#include <scenegraph/geometry/boundingbox.h>
#include <scenegraph/scene/scene.h>

#include "../util/export_property.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;


BoundingBoxPtr d_getBBox( BBoxComputer* b )
{ return b->getBoundingBox(); }

bool p_scene( BBoxComputer * b, ScenePtr s){
	return b->process(s);
}

void class_BBoxComputer()
{
  class_< BBoxComputer,bases< Action >,boost::noncopyable >
    ("BBoxComputer", init<Discretizer&>("BBoxComputer() -> Compute the objects bounding box" ))
    .def("clear",&BBoxComputer::clear)
    .add_property("boundingbox",d_getBBox,"Return the last computed Bounding Box.")
    .def("process",&p_scene)
	 
    ;
}
