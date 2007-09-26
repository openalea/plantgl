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




#include "elevationgrid.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/math/util_math.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/tool/util_array2.h>
PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace std;

/* ----------------------------------------------------------------------- */


const real_t ElevationGrid::DEFAULT_X_SPACING(1);
const real_t ElevationGrid::DEFAULT_Y_SPACING(1);


/* ----------------------------------------------------------------------- */

ElevationGrid::Builder::Builder( ) :
    Patch::Builder(),
    HeightList(0),
    XSpacing(0),
    YSpacing(0){
}


ElevationGrid::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr ElevationGrid::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr
      (new ElevationGrid(*HeightList,
                         XSpacing ? *XSpacing : DEFAULT_X_SPACING,
                         YSpacing ? *YSpacing : DEFAULT_Y_SPACING,
                         (CCW ? *CCW : DEFAULT_CCW)));
  return SceneObjectPtr();
}


void ElevationGrid::Builder::destroy( ) {
  if (HeightList) delete HeightList;
  if (XSpacing) delete XSpacing;
  if (YSpacing) delete YSpacing;
  if (CCW) delete CCW;
}


bool ElevationGrid::Builder::isValid( ) const {

  // Height Field
 if (! HeightList) {
    genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"Elevation Grid","HeightList");
    return false;
  };
 for (uint32_t _i = 0 ; _i< (*HeightList)->getRowsNb() ; _i++)
     for (uint32_t _j = 0 ; _j< (*HeightList)->getColsNb() ; _j++)
         if (! finite((*HeightList)->getAt(_i,_j))) {
             string _ith = '(' + TOOLS(number)(_i + 1) + '-' + TOOLS(number)(_j + 1) + ')';
             genMessage(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),
                        "Elevation Grid","HeightList",_ith.c_str(),"Must be finite.");
             return false;
         };


  // XSpacing field
  if (XSpacing && (*XSpacing < REAL_EPSILON)) {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Elevation Grid","XSpacing","Must be not null.");
    return false;
  }

  // YSpacing field
  if (YSpacing && (*YSpacing < REAL_EPSILON)) {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Elevation Grid","YSpacing","Must be not null.");
    return false;
  }

  return true;
}


/* ----------------------------------------------------------------------- */

ElevationGrid::ElevationGrid( ) :
  Patch(),
  __heightList(0),
  __xSpacing(DEFAULT_X_SPACING),
  __ySpacing(DEFAULT_Y_SPACING) {
}

ElevationGrid::ElevationGrid( const RealArray2Ptr& heights,
                              real_t xSpacing,
                              real_t ySpacing,
                              bool ccw ) :
  Patch(ccw),
  __heightList(heights),
  __xSpacing(xSpacing),
  __ySpacing(ySpacing) {
  GEOM_ASSERT(isValid());
}

ElevationGrid::~ElevationGrid( ) {
}

bool
ElevationGrid::apply( Action& action ) {
  return action.process(this);
}

/* ----------------------------------------------------------------------- */

const real_t&
ElevationGrid::getHeightAt( const uint32_t i, const uint32_t j ) const {
  return __heightList->getAt(i,j);
}

real_t&
ElevationGrid::getHeightAt( const uint32_t i, const uint32_t j ) {
  return __heightList->getAt(i,j);
}

const RealArray2Ptr&
ElevationGrid::getHeightList( ) const {
  return __heightList;
}

RealArray2Ptr&
ElevationGrid::getHeightList( ) {
  return __heightList;
}

const uint32_t
ElevationGrid::getXDim( ) const {
  return __heightList->getColsNb();
}

const real_t
ElevationGrid::getXSize( ) const {
  return (real_t)((__heightList->getColsNb()-1) * __xSpacing);
}

const real_t&
ElevationGrid::getXSpacing( ) const {
  return __xSpacing;
}

real_t&
ElevationGrid::getXSpacing( ) {
  return __xSpacing;
}

const uint32_t
ElevationGrid::getYDim( ) const {
  return __heightList->getRowsNb();
}

const real_t
ElevationGrid::getYSize( ) const {
  return (real_t)((__heightList->getRowsNb()-1) * __ySpacing);
}

const real_t&
ElevationGrid::getYSpacing( ) const {
  return __ySpacing;
}

real_t&
ElevationGrid::getYSpacing( ) {
  return __ySpacing;
}

bool
ElevationGrid::isXSpacingToDefault( ) const {
  return fabs(__xSpacing - DEFAULT_X_SPACING) < GEOM_TOLERANCE;
}

bool
ElevationGrid::isYSpacingToDefault( ) const {
  return fabs(__ySpacing - DEFAULT_Y_SPACING) < GEOM_TOLERANCE;
}

/* ----------------------------------------------------------------------- */

SceneObjectPtr
ElevationGrid::copy() const {
  ElevationGrid * ptr = new ElevationGrid(*this);
  if(__heightList)ptr->getHeightList( ) = RealArray2Ptr(new RealArray2(*__heightList));
  return SceneObjectPtr(ptr);
}


Vector3
ElevationGrid::getPointAt(uint32_t i, uint32_t j) const{
    GEOM_ASSERT(i<__heightList->getColsNb());
    GEOM_ASSERT(j<__heightList->getRowsNb());
    return Vector3(i*__xSpacing,j*__ySpacing,__heightList->getAt(j,i));
}

Point4MatrixPtr
ElevationGrid::getCtrlPoints( ) const {
    Point4MatrixPtr theCtrlPoint(new Point4Matrix(__heightList->getRowsNb(),__heightList->getColsNb()));
    for(uint32_t _i=0;_i<__heightList->getRowsNb();_i++)
        for(uint32_t _j=0;_j<__heightList->getColsNb();_j++)
            theCtrlPoint->setAt(_i,_j,Vector4(_i*__xSpacing,_j*__ySpacing,__heightList->getAt(_i,_j),1));
    return theCtrlPoint;
}

bool
ElevationGrid::isValid( ) const {
  Builder _builder;
  _builder.HeightList = const_cast<RealArray2Ptr *>(&__heightList);
  _builder.XSpacing = const_cast<real_t *>(&__xSpacing);
  _builder.YSpacing = const_cast<real_t *>(&__ySpacing);
  _builder.CCW= const_cast<bool *>(&__ccw);
  return _builder.isValid();
}


/* ----------------------------------------------------------------------- */

