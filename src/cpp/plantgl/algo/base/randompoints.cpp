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
