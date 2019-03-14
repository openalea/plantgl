/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2018 - Cirad/Inra/Inria
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://github.com/openalea/plantgl
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

#include "projectionengine.h"
#include <plantgl/math/util_math.h>
#include "projectionrenderer.h"


/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE



ProjectionEngine::ProjectionEngine():
    __camera(0)
{
    setOrthographicCamera(-1, 1, -1, 1, -1, 1);
    lookAt(Vector3(0,1,0),Vector3(0,0,0),Vector3(0,0,1));

}

ProjectionEngine::~ProjectionEngine()
{

}

void ProjectionEngine::process(ScenePtr scene)
{
    Discretizer d;
    Tesselator t;
    ProjectionRenderer r(*this, t, d);
    scene->apply(r);
}
