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

#include "export_viewer.h"
#include <boost/python.hpp>

#include <plantgl/gui/base/icons.h>
#include <plantgl/tool/errormsg.h>

using namespace boost::python;


boost::python::list py_get_pixmap(ViewerIcon::PredefinedIcon id){
	boost::python::list res;
	const char * const * icon = ViewerIcon::getPixmap(id);
	size_t len = ViewerIcon::getPixmapNbLines(id);
	for (size_t i = 0; i < len; ++i)
		res.append(boost::python::str(icon[i]));
	return res;
}

void export_icon()
{
	scope icon = class_< ViewerIcon, boost::noncopyable >("Icon", no_init )
	 .def("get", &py_get_pixmap,"get(id).",args("id"))
     .staticmethod("get")
    ;

	enum_<ViewerIcon::PredefinedIcon>("PredefinedIcon")
		.value("exit",ViewerIcon::exit)
	    .value("fileopen",ViewerIcon::fileopen)
		.value("filefloppy",ViewerIcon::filefloppy)
		.value("fileprint",ViewerIcon::fileprint)
		.value("fileclose",ViewerIcon::fileclose)
		.value("reload",ViewerIcon::reload)
		.value("document",ViewerIcon::document)
		.value("camera",ViewerIcon::camera)
		.value("camerafile",ViewerIcon::camerafile)
		.value("perspective",ViewerIcon::perspective)
		.value("orthographic",ViewerIcon::orthographic)
		.value("fullscreen",ViewerIcon::fullscreen)
		.value("memory",ViewerIcon::memory)
		.value("wheel",ViewerIcon::wheel)
		.value("wizard",ViewerIcon::wizard)
		.value("wizardmenu",ViewerIcon::wizardmenu)
		.value("wizardrect",ViewerIcon::wizardrect)
		.value("notwizard",ViewerIcon::notwizard)
		.value("home",ViewerIcon::home)
		.value("locerase",ViewerIcon::locerase)
		.value("geometry",ViewerIcon::geometry)
		.value("transformed",ViewerIcon::transformed)
		.value("appearance",ViewerIcon::appearance)
		.value("shape",ViewerIcon::shape)
		.value("attribut",ViewerIcon::attribut)
		.value("attributptr",ViewerIcon::attributptr)
		.value("color",ViewerIcon::color)
		.value("plantlogo",ViewerIcon::plantlogo)
		.value("flower",ViewerIcon::flower)
		.value("bbox",ViewerIcon::bbox)
		.value("cross",ViewerIcon::cross)
		.value("line_width",ViewerIcon::line_width)
		.value("ctrlpoint",ViewerIcon::ctrlpoint)
		.value("rcactive",ViewerIcon::rcactive)
		.value("rccentered",ViewerIcon::rccentered)
		.value("rcvisible",ViewerIcon::rcvisible)
		.value("skeleton",ViewerIcon::skeleton)
		.value("solid",ViewerIcon::solid)
		.value("wire",ViewerIcon::wire)
		.value("light",ViewerIcon::light)
		.value("eye",ViewerIcon::eye)
		.value("eyef",ViewerIcon::eyef)
		.value("linked",ViewerIcon::linked)
		.value("unlinked",ViewerIcon::unlinked)
		.value("grid",ViewerIcon::grid)
		.value("gridXY",ViewerIcon::gridXY)
		.value("gridXZ",ViewerIcon::gridXZ)
		.value("gridYZ",ViewerIcon::gridYZ)
		.value("axis",ViewerIcon::axis)
		.value("logobar",ViewerIcon::logobar)
		.value("pov",ViewerIcon::pov)
		.value("vrml",ViewerIcon::vrml)
		.export_values()
	;

}
