/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2009 - Cirad/Inria/Inra - Virtual Plant Team
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

#ifndef __randompoints_h__
#define __randompoints_h__

#include "../algo_config.h"
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/math/util_vector.h>


PGL_BEGIN_NAMESPACE

ALGO_API TOOLS(Vector3) random_point_in_box(const TOOLS(Vector3)& minpt, const TOOLS(Vector3)& maxpt);
ALGO_API TOOLS(Vector3) random_point_in_tetrahedron(const TOOLS(Vector3)& p0, const TOOLS(Vector3)& p1, const TOOLS(Vector3)& p2, const TOOLS(Vector3)& p3);
ALGO_API Point3ArrayPtr random_points_in_tetrahedron(size_t nb, const TOOLS(Vector3)& p0, const TOOLS(Vector3)& p1, const TOOLS(Vector3)& p2, const TOOLS(Vector3)& p3);
ALGO_API Point3ArrayPtr random_points_in_tetrahedra(size_t nb_per_tetra, const Point3ArrayPtr points, const Index4ArrayPtr tetraindices);



PGL_END_NAMESPACE

#endif
