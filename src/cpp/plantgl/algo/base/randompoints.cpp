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


#include "randompoints.h"
#include <stdlib.h>

PGL_USING_NAMESPACE

Vector3 PGL(random_point_in_box)(const Vector3& minpt, const Vector3& maxpt)
{
    return minpt + Vector3((maxpt.x()-minpt.x())*rand()/RAND_MAX, (maxpt.y()-minpt.y())*rand()/RAND_MAX, (maxpt.z()-minpt.z())*rand()/RAND_MAX);
}

Vector3 PGL(random_point_in_tetrahedron)(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
    // from http://vcg.isti.cnr.it/activities/geometryegraphics/pointintetraedro.html    
    float s = rand()/RAND_MAX;
    float t = rand()/RAND_MAX;
    float u = rand()/RAND_MAX;

    if (s+t > 1.0)
    { // cut and fold the cube into a prism
      s = 1.0 - s;
      t = 1.0 - t;
    }
    if (t+u > 1.0)
    { // cut and fold the cube into a tetrahedron
      float tmp = u;
      u = 1.0 - s -t;
      t = 1.0 - tmp;
    }
    else if (s+t+u > 1.0) {
      float tmp = u;
      u = s + t + u - 1.0;
      s = 1 - t - tmp;
    }
    float a = 1 - s - t - u;
    
    return (p0 * a + p1 * s + p2 * t + p3 * u);
}

Point3ArrayPtr PGL(random_points_in_tetrahedron)(size_t nb, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
    Point3ArrayPtr res(new Point3Array(nb));
    for (Point3Array::iterator it = res->begin(); it != res->end(); ++it)
        *it = random_point_in_tetrahedron(p0, p1, p2, p3);
    return res;
}

Point3ArrayPtr PGL(random_points_in_tetrahedra)(size_t nb_per_tetra, const Point3ArrayPtr points, const Index4ArrayPtr tetraindices)
{
    Point3ArrayPtr res(new Point3Array(nb_per_tetra* tetraindices->size()));
    Point3Array::iterator it = res->begin();
    for (Index4Array::iterator ittetra = tetraindices->begin(); ittetra != tetraindices->end(); ++it, ++ittetra){
        const Vector3& p0 = points->getAt(ittetra->getAt(0));
        const Vector3& p1 = points->getAt(ittetra->getAt(1)); 
        const Vector3& p2 = points->getAt(ittetra->getAt(2));
        const Vector3& p3 = points->getAt(ittetra->getAt(3));
        for (int i = 0; i < nb_per_tetra; ++i)
            *it = random_point_in_tetrahedron(p0, p1, p2, p3);
    }
    return res;

}




