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





#include "explicitmodel.h"
#include <scenegraph/core/pgl_messages.h>
#include <scenegraph/container/pointarray.h>
#include <tool/util_string.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

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
    genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"ExplicitModel","PointList");
    return false;
  };
  if ((*PointList)->getSize() < 1) {
    genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"ExplicitModel","PointList","Number of points must be greater than 1.");
    return false;
  };

  uint32_t _pointListSize = (*PointList)->getSize();
  for (uint32_t _i = 0; _i < _pointListSize; _i++)
    if (! (*PointList)->getAt(_i).isValid()) {
      const Vector3& p = (*PointList)->getAt(_i);
      string mess = "Must be a valid point <" + number(p.x()) + "," + number(p.y()) + "," + number(p.z()) + ">.";
      genMessage
        (ERRORMSG(INVALID_FIELD_ITH_VALUE_ssss),"ExplicitModel","PointList",number(_i+1).c_str(),mess.c_str());
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

uint32_t
ExplicitModel::getPointListSize( ) const {
  return __pointList->getSize();
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
  return __colorList.isValid();
}


/* ----------------------------------------------------------------------- */
