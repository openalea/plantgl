/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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


PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

#ifndef __cgalwrap__
#define __cgalwrap__

#include <plantgl/math/util_vector.h>

template<class CgalPoint2>
inline CgalPoint2 toPoint2(const TOOLS(Vector2)& v) { return CgalPoint2(v.x(),v.y()); }

template<class CgalPoint2>
inline Vector2 toVector2(const CgalPoint2& v) { return TOOLS(Vector2)(CGAL::to_double(v.x()),CGAL::to_double(v.y())); }

template<class CgalPoint3>
inline CgalPoint3 toPoint3(const Vector3& v) { return CgalPoint3(v.x(),v.y(),v.z()); }

template<class CgalPoint3>
inline TOOLS(Vector3) toVector3(const CgalPoint3& v) { return TOOLS(Vector3)(CGAL::to_double(v.x()),CGAL::to_double(v.y()),CGAL::to_double(v.z())); }

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