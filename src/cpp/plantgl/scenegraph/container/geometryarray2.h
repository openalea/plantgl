/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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

/*! \file geom_geometryarray2.h
    \brief Definition of the container class GeometryArray, Curve2DArray and CurveArray.
*/

#ifndef __geom_geometryarray2_h__
#define __geom_geometryarray2_h__

/* ----------------------------------------------------------------------- */

#include "geometryarray.h"
#include <plantgl/scenegraph/geometry/lineicmodel.h>
#include <plantgl/scenegraph/geometry/curve.h>

/* ----------------------------------------------------------------------- */


PGL_BEGIN_NAMESPACE


/* ----------------------------------------------------------------------- */

/**
   \class GeometryArray
   \brief An array of geometry object
*/

class SG_API GeometryArray : public GeomArray<GeometryPtr> {
public :
  GeometryArray (uint_t size = 0 );

  template <class InIterator>
  GeometryArray( InIterator first, InIterator last ) :
     GeomArray<GeometryPtr>(first,last){ }

  virtual ~GeometryArray();
};

/**
   \class Curve2DArray
   \brief An array of curve 2D object
*/

class SG_API Curve2DArray : public GeomArray<Curve2DPtr> {
public :
  Curve2DArray (uint_t size = 0 );
  template <class InIterator>
  Curve2DArray( InIterator first, InIterator last ) :
    GeomArray<Curve2DPtr>(first,last){ }
  virtual ~Curve2DArray();
};

/**
   \class CurveArray 
   \brief An array of curve object
*/
class SG_API CurveArray : public GeomArray<LineicModelPtr> {
public :
  CurveArray (uint_t size = 0 );
  template <class InIterator>
  CurveArray( InIterator first, InIterator last ) :
    GeomArray<LineicModelPtr>(first,last){ }

  virtual ~CurveArray();
};

// typedef GeomArray<GeometryPtr> GeometryArray;
// typedef GeomArray<Curve2DPtr> Curve2DArray;
// typedef GeomArray<LineicModelPtr> CurveArray;

/// GeometryArrayPtr Pointer
typedef RCPtr<GeometryArray> GeometryArrayPtr;
PGL_DECLARE_TYPE(GeometryArray)
/// Curve2DArrayPtr Pointer
typedef RCPtr<Curve2DArray> Curve2DArrayPtr;
PGL_DECLARE_TYPE(Curve2DArray)
/// CurveArrayPtr Pointer
typedef RCPtr<CurveArray> CurveArrayPtr;
PGL_DECLARE_TYPE(CurveArray)


/* ----------------------------------------------------------------------- */

// __geom_geometryarray_h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif



