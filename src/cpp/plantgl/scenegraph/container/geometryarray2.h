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



