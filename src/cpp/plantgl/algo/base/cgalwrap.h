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



PGL_USING_NAMESPACE

#ifndef __cgalwrap__
#define __cgalwrap__

#include <plantgl/math/util_vector.h>

template<class CgalPoint2>
inline CgalPoint2 toPoint2(const Vector2& v) { return CgalPoint2(v.x(),v.y()); }

template<class CgalPoint2>
inline Vector2 toVector2(const CgalPoint2& v) { return Vector2(CGAL::to_double(v.x()),CGAL::to_double(v.y())); }

template<class CgalPoint3>
inline CgalPoint3 toPoint3(const Vector3& v) { return CgalPoint3(v.x(),v.y(),v.z()); }

template<class CgalPoint3>
inline Vector3 toVector3(const CgalPoint3& v) { return Vector3(CGAL::to_double(v.x()),CGAL::to_double(v.y()),CGAL::to_double(v.z())); }

template<class CgalDirection3>
inline Vector3 dir2Vector3(const CgalDirection3& v) { return Vector3(CGAL::to_double(v.dx()),CGAL::to_double(v.dy()),CGAL::to_double(v.dz())); }

template<class CgalPoint3>
inline std::list<CgalPoint3> toPoint3List(const Point3ArrayPtr v) {
    std::list<CgalPoint3> res;
    for(Point3Array::const_iterator it = v->begin(); it != v->end(); ++it)
        res.push_back(toPoint3<CgalPoint3>(*it));
    return res;
}

template<class CgalPoint3>
inline std::list<CgalPoint3> toPoint3List(const Point3ArrayPtr v, const Index& subset) {
    std::list<CgalPoint3> res;
    for(Index::const_iterator it = subset.begin(); it != subset.end(); ++it)
        res.push_back(toPoint3<CgalPoint3>(v->getAt(*it)));
    return res;
}


#endif
