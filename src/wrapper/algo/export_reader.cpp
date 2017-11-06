/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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


#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_list.h>
#include "export_printer.h"
#include <plantgl/scenegraph/scene/scene.h>

#ifdef WITH_BISONFLEX
#include <plantgl/algo/codec/cdc_geom.h>
#include <plantgl/algo/codec/scne_parser.h>
#include <plantgl/scenegraph/core/smbtable.h>
#endif
#include <plantgl/algo/codec/scne_binaryparser.h>
#include <sstream>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python

#ifdef WITH_BISONFLEX

object pgl_read(const std::string& txt, const std::string& fname)
{
	SceneObjectSymbolTable table;
	ScenePtr scene;
	std::stringstream input(txt, std::ios_base::in);
        bool res = geom_read(input,table,scene);
	return make_tuple(scene,make_dict(table)());
}

#endif

void export_PglReader()
{
#ifdef WITH_BISONFLEX
	def("pgl_read",&pgl_read, (bp::arg("input"),bp::arg("fname")="string"));
#endif
	def("pglParserVerbose",&parserVerbose, (bp::arg("verbose")=true));
	def("isPglParserVerbose",&isParserVerbose);
}
