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




#include "mattransformed.h"
#include "orthotransformed.h"

#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

Matrix4Transformation::Matrix4Transformation( ) :
  Transformation3D() {
}

Matrix4Transformation::~Matrix4Transformation( ) {
}


/* ----------------------------------------------------------------------- */

Matrix3Transformation::Matrix3Transformation( ) :
  Transformation2D() {
}

Matrix3Transformation::~Matrix3Transformation( ) {
}

/* ----------------------------------------------------------------------- */



MatrixTransformed::Builder::Builder() :
  Transformed::Builder(),
  Geometry(0) {
}


MatrixTransformed::Builder::~Builder() {
}


void MatrixTransformed::Builder::destroy() {
        MTDestroy() ;
}
void MatrixTransformed::Builder::MTDestroy() {
  if (Geometry) delete Geometry;
}


bool MatrixTransformed::Builder::isValid( ) const {
        return MTValid( ) ;
}
bool MatrixTransformed::Builder::MTValid( ) const {
  if (! Geometry) {
    genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"Matrix Transformed","Geometry");
    return false;
  };
  if (! (*Geometry) ) {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Matrix Transformed","Geometry","Must be a valid Geometry Object.");
    return false;
  };
  return true;
}


/* ----------------------------------------------------------------------- */


MatrixTransformed::MatrixTransformed( const GeometryPtr& geometry ) :
  Transformed(),
  __geometry(geometry) {
}

MatrixTransformed::MatrixTransformed() :
  Transformed(),
  __geometry(0) {
}

MatrixTransformed::~MatrixTransformed( ) {
}

const GeometryPtr
MatrixTransformed::getGeometry( ) const {
  return __geometry;
}

GeometryPtr&
MatrixTransformed::getGeometry( ){
  return __geometry;
}

bool
MatrixTransformed::isACurve( ) const {
  return __geometry->isACurve();
}

bool
MatrixTransformed::isASurface( ) const {
  return __geometry->isASurface();
}

bool
MatrixTransformed::isAVolume( ) const {
  return __geometry->isAVolume();
}

bool
MatrixTransformed::isExplicit( ) const {
  return __geometry->isExplicit();
}

/* ----------------------------------------------------------------------- */
GeneralMatrix3Transformation::GeneralMatrix3Transformation( const Matrix3& mat ) :
  Matrix3Transformation(),
  __matrix(mat){
}

GeneralMatrix3Transformation::~GeneralMatrix3Transformation( ) {
}

Matrix3 GeneralMatrix3Transformation::getMatrix( ) const{
  return __matrix;
}

bool GeneralMatrix3Transformation::isValid() const {
    return __matrix.isValid();
}

Point2ArrayPtr
GeneralMatrix3Transformation::transform( const Point2ArrayPtr& points ) const{
  GEOM_ASSERT(points);
  Point2ArrayPtr _tPoints(new Point2Array(points->getSize()));
  Point2Array::iterator _ti = _tPoints->getBegin();
  for (Point2Array::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = (__matrix * Vector3(*_i,1.0)).project();
  return _tPoints;
}

Point3ArrayPtr
GeneralMatrix3Transformation::transform( const Point3ArrayPtr& points ) const{
  GEOM_ASSERT(points);
  Point3ArrayPtr _tPoints(new Point3Array(points->getSize()));
  Point3Array::iterator _ti = _tPoints->getBegin();
  for (Point3Array::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = __matrix * (*_i);
  return _tPoints;
}

Point2MatrixPtr
GeneralMatrix3Transformation::transform( const Point2MatrixPtr& points ) const{
  GEOM_ASSERT(points);
  Point2MatrixPtr _tPoints(new Point2Matrix(points->getSize()));
  Point2Matrix::iterator _ti = _tPoints->getBegin();
  for (Point2Matrix::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = (__matrix * Vector3(*_i,1.0)).project();
  return _tPoints;
}

Point3MatrixPtr
GeneralMatrix3Transformation::transform( const Point3MatrixPtr& points ) const{
  GEOM_ASSERT(points);
  Point3MatrixPtr _tPoints(new Point3Matrix(points->getSize()));
  Point3Matrix::iterator _ti = _tPoints->getBegin();
  for (Point3Matrix::const_iterator _i = points->getBegin();
       _i != points->getEnd();
         _i++)
    *_ti++ = __matrix * (*_i);
  return _tPoints;
}


/* ----------------------------------------------------------------------- */


Transform4::Builder::Builder() :
  Translation(0),
  Scale(0),
  Rotation(0)
{
  GEOM_TRACE("Constructor Transform4::Builder");
}

Transform4::Builder::~Builder(){
}

Transform4Ptr Transform4::Builder::build( ) const
{
  GEOM_TRACE("build Transform4::Builder");
  if( isValid() )
    {
    Transform4* T= new Transform4();
    if( Scale )
      T->scale(*Scale);
    if( Rotation )
      T->rotate((*Rotation)->getMatrix3());
    if( Translation )
      T->translate(*Translation);

    return Transform4Ptr(T);
    }

  return Transform4Ptr();
}

void Transform4::Builder::destroy( )
{
  GEOM_TRACE("destroy Transform4::Builder");
  if( Translation ) delete Translation; Translation= 0;
  if( Scale ) delete Scale; Scale= 0;
  if( Rotation ) delete Rotation; Rotation= 0;
}

bool Transform4::Builder::isValid( ) const
{
  GEOM_TRACE("validate Transform4::Builder");
  if( (!Translation) && (!Scale) && (!Rotation) )
    {
    genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"Transform4","Translation, Scale and Rotation");
    return false;
    }

  if( Translation && (!Translation->isValid()) )
    {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Transform4","Translation","Must be a valid Translation.");
    return false;
    }

  if( Scale && (!Scale->isValid()) )
    {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Transform4","Scale","Must be a valid Scale.");
    return false;
    }

  if( Rotation && (!(*Rotation)->isValid()) )
    {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Transform4","Rotation","Must be a valid Rotation.");
    return false;
    }

  return true;
}

/* ----------------------------------------------------------------------- */

Transform4::Transform4() :
  __matrix(Matrix4::IDENTITY) {
}

Transform4::Transform4( const Matrix4& mat ) :
  __matrix(mat){
}

Transform4::~Transform4( ) {
}

Matrix4 Transform4::getMatrix( ) const{
  return __matrix;
}

Matrix4& Transform4::getMatrix( ){
  return __matrix;
}

/* ----------------------------------------------------------------------- */

/////////////////////////////////////////////////////////////////////////////
bool Transform4::isValid() const
/////////////////////////////////////////////////////////////////////////////
{
  return __matrix.isValid();
}

/////////////////////////////////////////////////////////////////////////////
Point3ArrayPtr Transform4::transform( const Point3ArrayPtr& points ) const
/////////////////////////////////////////////////////////////////////////////
{
  GEOM_ASSERT(points);
  Point3ArrayPtr _tPoints(new Point3Array(points->getSize()));
  Point3Array::iterator _ti = _tPoints->getBegin();
  Point3Array::const_iterator _i = points->getBegin();
  for ( _i = points->getBegin(); _i != points->getEnd(); _i++ )
    *_ti++ = __matrix * (*_i);
  return _tPoints;
}

/////////////////////////////////////////////////////////////////////////////
Point4ArrayPtr Transform4::transform( const Point4ArrayPtr& points ) const
/////////////////////////////////////////////////////////////////////////////
{
  GEOM_ASSERT(points);
  Point4ArrayPtr _tPoints(new Point4Array(points->getSize()));
  Point4Array::iterator _ti = _tPoints->getBegin();
  Point4Array::const_iterator _i = points->getBegin();
  for ( _i = points->getBegin(); _i != points->getEnd(); _i++ )
    *_ti++ = __matrix * (*_i);
  return _tPoints;
}

/////////////////////////////////////////////////////////////////////////////
Point3MatrixPtr Transform4::transform( const Point3MatrixPtr& points ) const
/////////////////////////////////////////////////////////////////////////////
{
  GEOM_ASSERT(points);
  Point3MatrixPtr _tPoints(new Point3Matrix(points->getSize()));
  Point3Matrix::iterator _ti = _tPoints->getBegin();
  Point3Matrix::const_iterator _i = points->getBegin();
  for ( _i = points->getBegin(); _i != points->getEnd(); _i++ )
    *_ti++ = __matrix * (*_i);
  return _tPoints;
}

/////////////////////////////////////////////////////////////////////////////
Point4MatrixPtr Transform4::transform( const Point4MatrixPtr& points ) const
/////////////////////////////////////////////////////////////////////////////
{
  GEOM_ASSERT(points);
  Point4MatrixPtr _tPoints(new Point4Matrix(points->getSize()));
  Point4Matrix::iterator _ti = _tPoints->getBegin();
  Point4Matrix::const_iterator _i = points->getBegin();
  for ( _i = points->getBegin(); _i != points->getEnd(); _i++ )
    *_ti++ = __matrix * (*_i);
  return _tPoints;
}

/////////////////////////////////////////////////////////////////////////////
Transform4& Transform4::translate( const Vector3& t )
/////////////////////////////////////////////////////////////////////////////
{
  Matrix4 tm= Matrix4::translation(t);
  __matrix= tm * __matrix;
//cout<<"translation :"<<tm<<endl;
//cout<<"result :"<<__matrix<<endl<<endl;;
  return *this;
}

/////////////////////////////////////////////////////////////////////////////
Transform4& Transform4::scale( const Vector3& s )
/////////////////////////////////////////////////////////////////////////////
{
  Matrix4 sm(Matrix3::scaling(s));
  __matrix= sm * __matrix;
//cout<<"scaling :"<<sm<<endl;
//cout<<"result :"<<__matrix<<endl<<endl;;
  return *this;
}

/////////////////////////////////////////////////////////////////////////////
Transform4& Transform4::rotate( const Matrix3& m )
/////////////////////////////////////////////////////////////////////////////
{
  Matrix4 rm(m);
  __matrix= rm * __matrix;
//cout<<"rotated :"<<rm<<endl;
//cout<<"result :"<<__matrix<<endl<<endl;;
  return *this;
}

/////////////////////////////////////////////////////////////////////////////
void Transform4::getTransformation( Vector3& scale,
                                    Vector3& rotate,
                                    Vector3& translate )
/////////////////////////////////////////////////////////////////////////////
{
  __matrix.getTransformation(scale,rotate,translate);
}

/////////////////////////////////////////////////////////////////////////////
real_t Transform4::getVolume() const
/////////////////////////////////////////////////////////////////////////////
{
  return det( Matrix3(__matrix) );
}
