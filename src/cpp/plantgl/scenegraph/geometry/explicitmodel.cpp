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






#include "explicitmodel.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/tool/util_string.h>

PGL_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */


ExplicitModel::Builder::Builder( ) :
  Primitive::Builder(),
  PointList(0) ,
  ColorList(0) {
}


ExplicitModel::Builder::~Builder( ) {
  // nothing to do
}

void ExplicitModel::Builder::destroy( )
{
  EMDestroy( );
}

void ExplicitModel::Builder::EMDestroy( )
{
  if (PointList) delete PointList;
  if (ColorList) delete ColorList;
}

bool ExplicitModel::Builder::isValid( ) const
{
  return EMValid( );
}

bool ExplicitModel::Builder::EMValid( ) const
{
  // PointList
  if (! PointList) {
    pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"ExplicitModel","PointList");
    return false;
  };
  if ((*PointList)->size() < 1) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"ExplicitModel","PointList","Number of points must be greater than 1.");
    return false;
  };

  uint_t _pointListSize = (*PointList)->size();
  for (uint_t _i = 0; _i < _pointListSize; _i++)
    if (! (*PointList)->getAt(_i).isValid()) {
      const Vector3& p = (*PointList)->getAt(_i);
      string mess = "Must be a valid point <" + number(p.x()) + "," + number(p.y()) + "," + number(p.z()) + ">.";
      pglErrorEx
        (PGLERRORMSG(INVALID_FIELD_ITH_VALUE_ssss),"ExplicitModel","PointList",number(_i+1).c_str(),mess.c_str());
      return false;
    };

  return true;
}
/* ----------------------------------------------------------------------- */


ExplicitModel::ExplicitModel() :
  Primitive(){
}

ExplicitModel::ExplicitModel(const Point3ArrayPtr& points, const Color4ArrayPtr& colors ) :
  Primitive(),
  __pointList(points),
  __colorList(colors){
}

ExplicitModel::~ExplicitModel( ) {
}

bool
ExplicitModel::isExplicit( ) const {
  return true;
}

const Point3ArrayPtr&
ExplicitModel::getPointList( ) const{
  return __pointList;
}

Point3ArrayPtr&
ExplicitModel::getPointList( ) {
  return __pointList;
}

uint_t
ExplicitModel::getPointListSize( ) const {
  return __pointList->size();
}

Color4ArrayPtr&
ExplicitModel::getColorList( ) {
  return __colorList;
}

const Color4ArrayPtr&
ExplicitModel::getColorList( ) const {
  return __colorList;
}

bool
ExplicitModel::isColorListToDefault() const {
  return (!__colorList);
}

bool
ExplicitModel::hasColorList( ) const {
  return (__colorList);
}


/* ----------------------------------------------------------------------- */
