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



#include "triangulation3D.h"

PGL_USING_NAMESPACE

#ifdef PGL_WITH_CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
// #include <CGAL/Triangulation_3.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_3<uint32_t, K>    Vb;
typedef CGAL::Triangulation_data_structure_3<Vb>                    Tds;

typedef CGAL::Delaunay_triangulation_3<K, Tds>                      Triangulation;
// typedef CGAL::Triangulation_3<K,Tds>      Triangulation;


typedef Triangulation::Cell_handle    Cell_handle;
typedef Triangulation::Vertex_handle  Vertex_handle;
typedef Triangulation::Locate_type    Locate_type;
typedef Triangulation::Point          Point;
typedef Triangulation::Segment        Segment;



inline Point toPoint(const Vector3& v) { return Point(v.x(),v.y(),v.z()); }
inline Vector3 toVector3(const Point& v) { return Vector3(v.x(),v.y(),v.z()); }

#endif

Index3ArrayPtr
PGL(delaunay_triangulation3D)(const Point3ArrayPtr points)
{
#ifdef PGL_WITH_CGAL
    Triangulation triangulation;
    uint32_t pointCount = 0;
    for (Point3Array::const_iterator it = points->begin(); it != points->end(); ++it)
        triangulation.insert(toPoint(*it))->info() = pointCount++;

    Index3ArrayPtr result(new Index3Array(points->size()));
    for(Triangulation::Finite_facets_iterator it = triangulation.finite_facets_begin();
        it != triangulation.finite_facets_end(); ++it){
            const Cell_handle cell = it->first;
            const int& index = it->second;
            const int index1 = cell->vertex(triangulation.vertex_triple_index(index, 0))->info();
            const int index2 = cell->vertex(triangulation.vertex_triple_index(index, 1))->info();
            const int index3 = cell->vertex(triangulation.vertex_triple_index(index, 2))->info();
            result->push_back(Index3(index1,index2,index3));
    }
#else
    #ifdef _MSC_VER
    #pragma message("function 'delaunay_triangulation3D' disabled. CGAL needed.")
    #else
    #warning "function 'delaunay_triangulation3D' disabled. CGAL needed"
    #endif

    Index3ArrayPtr result;
#endif
    return result;
}
