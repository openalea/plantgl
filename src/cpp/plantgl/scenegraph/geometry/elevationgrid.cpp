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





#include "elevationgrid.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/math/util_math.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/tool/util_array2.h>
PGL_USING_NAMESPACE
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
    pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"Elevation Grid","HeightList");
    return false;
  };
 for (uint_t _i = 0 ; _i< (*HeightList)->getRowNb() ; _i++)
     for (uint_t _j = 0 ; _j< (*HeightList)->getColumnNb() ; _j++)
         if (! pglfinite((*HeightList)->getAt(_i,_j))) {
             string _ith = '(' + number(_i + 1) + '-' + number(_j + 1) + ')';
             pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),
                        "Elevation Grid","HeightList",_ith.c_str(),"Must be pglfinite.");
             return false;
         };


  // XSpacing field
  if (XSpacing && (*XSpacing < REAL_EPSILON)) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Elevation Grid","XSpacing","Must be not null.");
    return false;
  }

  // YSpacing field
  if (YSpacing && (*YSpacing < REAL_EPSILON)) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Elevation Grid","YSpacing","Must be not null.");
    return false;
  }

  return true;
}


/* ----------------------------------------------------------------------- */

ElevationGrid::ElevationGrid( ) :
  Patch(),
  __heightList(),
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

/* ----------------------------------------------------------------------- */

const real_t&
ElevationGrid::getHeightAt( const uint_t i, const uint_t j ) const {
  return __heightList->getAt(i,j);
}

real_t&
ElevationGrid::getHeightAt( const uint_t i, const uint_t j ) {
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

const uint_t
ElevationGrid::getXDim( ) const {
  return __heightList->getColumnNb();
}

const real_t
ElevationGrid::getXSize( ) const {
  return (real_t)((__heightList->getColumnNb()-1) * __xSpacing);
}

const real_t&
ElevationGrid::getXSpacing( ) const {
  return __xSpacing;
}

real_t&
ElevationGrid::getXSpacing( ) {
  return __xSpacing;
}

const uint_t
ElevationGrid::getYDim( ) const {
  return __heightList->getRowNb();
}

const real_t
ElevationGrid::getYSize( ) const {
  return (real_t)((__heightList->getRowNb()-1) * __ySpacing);
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
ElevationGrid::copy(DeepCopier& copier) const {
  ElevationGrid * ptr = new ElevationGrid(*this);
  copier.copy_attribute(ptr->getHeightList());
  return SceneObjectPtr(ptr);
}


Vector3
ElevationGrid::getPointAt(uint_t i, uint_t j) const{
    GEOM_ASSERT(i<__heightList->getColumnNb());
    GEOM_ASSERT(j<__heightList->getRowNb());
    return Vector3(i*__xSpacing,j*__ySpacing,__heightList->getAt(j,i));
}

Point4MatrixPtr
ElevationGrid::getCtrlPoints( ) const {
    Point4MatrixPtr theCtrlPoint(new Point4Matrix(__heightList->getRowNb(),__heightList->getColumnNb()));
    for(uint_t _i=0;_i<__heightList->getRowNb();_i++)
        for(uint_t _j=0;_j<__heightList->getColumnNb();_j++)
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


real_t ElevationGrid::getHeightAt(const Vector2 pos) const
{
    int xind = int(pos.x() / __xSpacing);
    float xres = pos.x() -  (xind * __xSpacing);
    if (xind < 0 || xind >= __heightList->getColumnNb()) return 0;
    int yind = int(pos.y() / __ySpacing);
    float yres = pos.y() -  (yind * __ySpacing);
    if (yind < 0 || yind >= __heightList->getRowNb()) return 0;
    float refelevation, refelev1, refelev2;
    if (xres+yres > 1.0){
        refelevation = getPointAt(xind+1,yind+1).z();
        refelev1 = getPointAt(xind,yind+1).z();
        refelev2 = getPointAt(xind+1,yind).z();
        xres = 1 - xres;
        yres = 1 - yres;
    }
    else {
        refelevation = getPointAt(xind,yind).z();
        refelev1 = getPointAt(xind+1,yind).z();
        refelev2 = getPointAt(xind,yind+1).z();
    }
    refelev1 = refelev1-refelevation;
    refelev2 = refelev2-refelevation;
    return refelevation + (refelev1* xres) + (refelev2 * yres);
}


/* ----------------------------------------------------------------------- */

