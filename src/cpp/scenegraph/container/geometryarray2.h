/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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

#include "geom_geometryarray.h"

/* ----------------------------------------------------------------------- */

#ifdef _WIN32

#include "geom_curve.h"
#include "geom_lineicmodel.h"
GEOM_BEGIN_NAMESPACE

#else

GEOM_BEGIN_NAMESPACE
class Geometry;
class Curve2D;
class LineicModel;

#endif

/* ----------------------------------------------------------------------- */

typedef RCPtr<Geometry> GeometryPtr;
typedef RCPtr<Curve2D> Curve2DPtr;
typedef RCPtr<LineicModel> LineicModelPtr;

/* ----------------------------------------------------------------------- */

/**
   \class GeometryArray
   \brief An array of geometry object
*/

class GEOM_API GeometryArray : public GeomArray<GeometryPtr> {
public :
  GeometryArray (uint32_t size = 0 );

  template <class InIterator>
  GeometryArray( InIterator first, InIterator last ) :
     GeomArray<GeometryPtr>(first,last){ }

  virtual ~GeometryArray();
};

/**
   \class Curve2DArray
   \brief An array of curve 2D object
*/

class GEOM_API Curve2DArray : public GeomArray<Curve2DPtr> {
public :
  Curve2DArray (uint32_t size = 0 );
  template <class InIterator>
  Curve2DArray( InIterator first, InIterator last ) :
    GeomArray<Curve2DPtr>(first,last){ }
  virtual ~Curve2DArray();
};

/**
   \class CurveArray 
   \brief An array of curve object
*/
class GEOM_API CurveArray : public GeomArray<LineicModelPtr> {
public :
  CurveArray (uint32_t size = 0 );
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
/// Curve2DArrayPtr Pointer
typedef RCPtr<Curve2DArray> Curve2DArrayPtr;
/// CurveArrayPtr Pointer
typedef RCPtr<CurveArray> CurveArrayPtr;


/* ----------------------------------------------------------------------- */

// __geom_geometryarray_h__
/* ----------------------------------------------------------------------- */

GEOM_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif



