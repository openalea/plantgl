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

/* ----------------------------------------------------------------------- */

#include "cdc_vgstar.h"

#include "vgsfile.h"
#include <plantgl/algo/base/tesselator.h>
#include <plantgl/tool/errormsg.h>
#include "vgstarprinter.h"
#include <fstream>

PGL_USING_NAMESPACE


using namespace std;

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

VgStarCodec::VgStarCodec() : 
    SceneCodec("VGS", ReadWrite ) 
    {}

SceneFormatList VgStarCodec::formats() const
{
    SceneFormat _format;
    _format.name = "VGS";
    _format.suffixes.push_back("vgs");
    _format.suffixes.push_back("vgx");
    _format.comment = "The VegeStar format.";
    SceneFormatList _formats;
    _formats.push_back(_format);
    return _formats;
}

ScenePtr VgStarCodec::read(const std::string& fname)
{
  return VegeStarFile::read(fname,*PglErrorStream::error);
}

bool VgStarCodec::write(const std::string& fname,const ScenePtr& scene)
{
    std::ofstream stream(fname.c_str());
    if(stream){
        Tesselator t;
        VgstarPrinter p(stream,t);
        p.header();
        scene->apply(p);
        return true;
    }
    return false;
}
