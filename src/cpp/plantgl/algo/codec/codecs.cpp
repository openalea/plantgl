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

#include "codecs.h"
#include "cdc_geom.h"
#include "cdc_asc.h"
#include "cdc_vgstar.h"
#include "cdc_pov.h"
#include "cdc_vrml.h"
#include "cdc_ply.h"
#include <plantgl/scenegraph/scene/factory.h>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE

class CodecInstaller {
public:
    CodecInstaller() { installCodecs(); }
};

static CodecInstaller MyCodecInstaller;

void installCodecs(){
    static bool installed = false;
    if(!installed){
        installed = true;
        SceneFactory::get().registerCodec(SceneCodecPtr(new GeomCodec()));
        SceneFactory::get().registerCodec(SceneCodecPtr(new AscCodec()));
        SceneFactory::get().registerCodec(SceneCodecPtr(new BGeomCodec()));
        SceneFactory::get().registerCodec(SceneCodecPtr(new VgStarCodec()));
        SceneFactory::get().registerCodec(SceneCodecPtr(new PovCodec()));
        SceneFactory::get().registerCodec(SceneCodecPtr(new VrmlCodec()));
        SceneFactory::get().registerCodec(SceneCodecPtr(new PlyCodec()));
    }
}


