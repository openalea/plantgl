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
