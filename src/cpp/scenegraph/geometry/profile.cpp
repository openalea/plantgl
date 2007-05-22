//#define DEBUG
/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon
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


#include "profile.h"
#include <scenegraph/core/pgl_messages.h>

#include <scenegraph/transformation/scaled.h>
#include <scenegraph/transformation/mattransformed.h>
#include <scenegraph/transformation/orthotransformed.h>

#include <scenegraph/container/pointarray.h>
#include <scenegraph/container/geometryarray2.h>

#include "interpol.h"
#include "polyline.h"
#include "nurbscurve.h"

#include <math/util_math.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace std;

/* ----------------------------------------------------------------------- */


ProfileTransformation::ProfileTransformation():
    __scalingList(DEFAULT_SCALE_LIST),
    __orientationList(DEFAULT_ORIENTATION_LIST),
    __knotList(0){
}

ProfileTransformation::ProfileTransformation(Point2ArrayPtr _scalingList,
                                             RealArrayPtr _orientationList,
                                             RealArrayPtr _knotList ):
    __scalingList(_scalingList),
    __orientationList(_orientationList),
    __knotList(_knotList){
}

ProfileTransformation::ProfileTransformation(Point2ArrayPtr _scalingList,
                                             RealArrayPtr _orientationList):
    __scalingList(_scalingList),
    __orientationList(_orientationList),
    __knotList(0){
}

ProfileTransformation::~ProfileTransformation(){}

/* ----------------------------------------------------------------------- */

real_t ProfileTransformation::getUMin() const {
    return (__knotList ? __knotList->getAt(0) : 0);
}

real_t ProfileTransformation::getUMax() const {
    return (__knotList ? __knotList->getAt((__knotList->getSize())-1) : 1 );
}

const Point2ArrayPtr& ProfileTransformation::getScale() const {
    return __scalingList;
}

const RealArrayPtr& ProfileTransformation::getOrientation() const {
    return __orientationList;
}

Point2ArrayPtr& ProfileTransformation::getScale() {
    return __scalingList;
}

RealArrayPtr& ProfileTransformation::getOrientation(){
    return __orientationList;
}

const RealArrayPtr ProfileTransformation::getKnotList() const {
    if(__knotList)return __knotList;
    else {
        uint32_t _size = max(__scalingList->getSize(),__orientationList->getSize());
        if(_size<1)_size = 2;
        RealArrayPtr a(new RealArray(_size));
        a->setAt(0,0.0);
        for(uint32_t _i = 1; _i < _size; _i++)
            a->setAt(_i, ((real_t)_i /(real_t)(_size - 1)));
        a->setAt(_size-1,1.0);
        return a;
    }
}

RealArrayPtr& ProfileTransformation::getKnotList(){
    return __knotList;
}

/* ----------------------------------------------------------------------- */

Transformation2DPtr ProfileTransformation::operator() (real_t u) const {
    Matrix3TransformationPtr val(0);
    uint32_t _i = 0;
    real_t _t = 0;

    if((!__scalingList)||__scalingList->isEmpty() ||(__scalingList==DEFAULT_SCALE_LIST));
    else if(__scalingList->getSize() == 1){
        val = Matrix3TransformationPtr(new Scaling2D(__scalingList->getAt(0)));
    }
    else if(__knotList){
        if(u <= __knotList->getAt(0)){
            val = Matrix3TransformationPtr(new Scaling2D(__scalingList->getAt(0)));
        }
        else if (u >= __knotList->getAt((__knotList->getSize())-1)){

            val = Matrix3TransformationPtr(new Scaling2D(__scalingList->getAt((__knotList->getSize())-1)));
        }
        else {
            for(RealArray::iterator _it = __knotList->getBegin();
                ((_it!=__knotList->getEnd())&&(*_it < u )) ;
                _it++)_i++;
            if(__knotList->getAt(_i) == u)val = Matrix3TransformationPtr(new Scaling2D(__scalingList->getAt(_i)));
            else{
                _t = (__knotList->getAt(_i) - u)/(__knotList->getAt(_i) -  __knotList->getAt(_i-1)) ;
                val = Matrix3TransformationPtr(new Scaling2D((__scalingList->getAt(_i-1)*(_t)+(__scalingList->getAt(_i)*(1-_t)))));
            }
        }
    }
    else {
        if(u <= 0 ){
            val = Matrix3TransformationPtr(new Scaling2D(__scalingList->getAt(0)));
        }
        else if (u >= 1 ){
            val = Matrix3TransformationPtr(new Scaling2D(*(__scalingList->getEnd()-1)));
        }
        else {
            real_t _interval = (real_t)(__scalingList->getSize() - 1);
            _i = (int)(u * _interval);
            _t = u*_interval - ((real_t)(_i));
//                      printf("u = %f - i = %i - t = %f - interval = %f \n",u,_i,_t,_interval);
            if(_t <= GEOM_EPSILON)val = Matrix3TransformationPtr(new Scaling2D(__scalingList->getAt(_i)));
            else{
                val = Matrix3TransformationPtr(new Scaling2D((__scalingList->getAt(_i)*(1-_t)+(__scalingList->getAt(_i+1)*(_t)))));
            }
        }
    }

    Matrix3TransformationPtr val2(0);
    if((!__orientationList) ||__orientationList->isEmpty() ||(__orientationList==DEFAULT_ORIENTATION_LIST));
    else if(__orientationList->getSize() == 1){
        real_t _c = cos(__orientationList->getAt(0));
        real_t _s = sin(__orientationList->getAt(0));
        val2 = Matrix3TransformationPtr(new OrthonormalBasis2D(Matrix2(_c,_s,-_s,_c)));
    }
    else
    if(__knotList){
        if(u <= __knotList->getAt(0)){
            real_t _c = cos(__orientationList->getAt(0));
            real_t _s = sin(__orientationList->getAt(0));
            val2 = Matrix3TransformationPtr(new OrthonormalBasis2D(Matrix2(_c,_s,-_s,_c)));
        }
        else if (u >= __knotList->getAt((__knotList->getSize())-1)){
            real_t _c = cos(__orientationList->getAt((__knotList->getSize())-1));
            real_t _s = sin(__orientationList->getAt((__knotList->getSize())-1));
            val2 = Matrix3TransformationPtr(new OrthonormalBasis2D(Matrix2(_c,_s,-_s,_c)));
        }
        else {
            _i = 0;
            for(RealArray::iterator _it = __knotList->getBegin();
                ((_it!=__knotList->getEnd())&&(*_it < u )) ;
                _it++)_i++;
            real_t _angle;
            if(__knotList->getAt(_i) == u){
                _angle = __orientationList->getAt(_i);
            }
            else{
                _t = (__knotList->getAt(_i) - u)/(__knotList->getAt(_i) -  __knotList->getAt(_i-1)) ;
                _angle = (__orientationList->getAt(_i-1)*_t)+(__orientationList->getAt(_i)*(1-_t));
            }
            real_t _c = cos(_angle);
            real_t _s = sin(_angle);
            val2 = Matrix3TransformationPtr(new OrthonormalBasis2D(Matrix2(_c,_s,-_s,_c)));
        }
    }
    else {
        if(u <= 0){
            real_t _c = cos(__orientationList->getAt(0));
            real_t _s = sin(__orientationList->getAt(0));
            val2 = Matrix3TransformationPtr(new OrthonormalBasis2D(Matrix2(_c,_s,-_s,_c)));
        }
        else if (u >= 1){
            real_t _c = cos(*(__orientationList->getEnd()-1));
            real_t _s = sin(*(__orientationList->getEnd()-1));
            val2 = Matrix3TransformationPtr(new OrthonormalBasis2D(Matrix2(_c,_s,-_s,_c)));
        }
        else {

            real_t _interval = (real_t)(__orientationList->getSize() - 1);
            _i = (int)(u / _interval);
            _t = u - (real_t)_i;
            real_t _angle;
            if(_t <= GEOM_EPSILON){
                _angle = __orientationList->getAt(_i);
            }
            else{
                _angle = (__orientationList->getAt(_i)*(1-_t))+(__orientationList->getAt(_i+1)*_t);
            }
            real_t _c = cos(_angle);
            real_t _s = sin(_angle);
            val2 = Matrix3TransformationPtr(new OrthonormalBasis2D(Matrix2(_c,_s,-_s,_c)));
        }
    }

    if (!val && !val2) return Transformation2DPtr(new Scaling2D(Vector2(1.0,1.0)));
    else if (!val){
//      cerr << "Orientation(" << u << ") : "  << val2->getMatrix() << endl;
        return Transformation2DPtr(val2);
    }
    else if (!val2){
//      cerr << "Scale(" << u << ") : "  << val->getMatrix() << endl;
        return Transformation2DPtr(val);
    }
    else{
        Matrix3 mat = val->getMatrix()*val2->getMatrix();
//      cerr << "Transf("<< u << ") : " << mat << endl;
        return Transformation2DPtr(new GeneralMatrix3Transformation(mat));
    }
}

/* ----------------------------------------------------------------------- */

const bool
ProfileTransformation::isKnotListToDefault() const{
    if(!__knotList)return true;
    uint32_t _size = max((__scalingList == NULL || __scalingList->isEmpty()?0:__scalingList->getSize()),
					   (__orientationList == NULL ||__orientationList->isEmpty()?0:__orientationList->getSize()));
    if(__knotList->getAt(0) > GEOM_EPSILON )return false;
    for(uint32_t _i = 1; _i < _size; _i++)
        if(fabs(__knotList->getAt(_i) - ((real_t)_i /(real_t)(_size - 1))) > GEOM_EPSILON ) return false;
    return true;
}

/* ----------------------------------------------------------------------- */


bool ProfileTransformation::isValid( ) const{

    if(__scalingList){
        if(!(__scalingList)->isValid()) {
            genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Extrusion","Scale","Must be a valid Object.");
            return false;
        };
        if((__scalingList)->getSize() == 0 ){
            genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"Extrusion","Scale","Must have more values.");
            return false;
        }
    }
    if(__orientationList)
        if((__orientationList)->getSize() == 0 ){
            genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"Extrusion","Orientation","Must have more values.");
            return false;
        }

    if( (__scalingList.isValid()) && (__orientationList.isValid()) &&
        (__scalingList->getSize() !=1) &&
        (__orientationList->getSize() !=1) &&
        (__scalingList->getSize()!=__orientationList->getSize()) ){
        genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Extrusion","Orientation",
                   "Must specifie Scale and Orientation with the same number of value.");
        return false;
    }

    if((__knotList.isValid()) && ((__scalingList.isValid()) || (__orientationList.isValid()))){
        uint32_t _size = 0;
        if(__scalingList) _size = __scalingList->getSize();
        if(__orientationList) _size = max(_size,__orientationList->getSize());
        if(_size > 1 && __knotList->getSize() != _size){
            genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Extrusion","KnotList",
                       "Must specifie Scale or Orientation with more than one value.");
            return false;
        }
    }
    return true;
}

/* ----------------------------------------------------------------------- */

const
Point2ArrayPtr ProfileTransformation::DEFAULT_SCALE_LIST(new Point2Array(1,Vector2(1,1)));

const
RealArrayPtr ProfileTransformation::DEFAULT_ORIENTATION_LIST(new RealArray((unsigned int)1,0));


/* ----------------------------------------------------------------------- */

const uint32_t ProfileInterpolation::DEFAULT_DEGREE(3);
const uint32_t ProfileInterpolation::DEFAULT_STRIDE(0);


/////////////////////////////////////////////////////////////////////////////

ProfileInterpolation::Builder::Builder() :
                               ProfileList(0),
                               KnotList(0),
                               Degree(0),
                               Stride(0)
{ }

/////////////////////////////////////////////////////////////////////////////

ProfileInterpolation::Builder::~Builder()
{ }

/////////////////////////////////////////////////////////////////////////////

ProfileInterpolationPtr ProfileInterpolation::Builder::build( ) const
{
  ProfileInterpolation* interpolant= 0;

  if( isValid() )
    {
    interpolant= new ProfileInterpolation( *ProfileList,
                                           *KnotList,
                                           (Degree ? *Degree : DEFAULT_DEGREE),
                                           (Stride ? *Stride : DEFAULT_STRIDE) );
    bool diagnosis= interpolant->interpol();
    if( !diagnosis )
      {
      delete interpolant;
      interpolant= 0;
      }
    }

  return ProfileInterpolationPtr(interpolant);
}

/////////////////////////////////////////////////////////////////////////////

void ProfileInterpolation::Builder::destroy()
{
  if( ProfileList )
    {
    delete ProfileList;
    ProfileList= 0;
    }
  if( KnotList )
    {
    delete KnotList;
    KnotList= 0;
    }
  if( Degree )
    {
    delete Degree;
    Degree= 0;
    }
  if( Stride )
    {
    delete Stride;
    Stride= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

bool ProfileInterpolation::Builder::isValid( ) const
{
  if( !ProfileList )
    {
    genMessage( WARNINGMSG(UNINITIALIZED_FIELD_ss),
                "ProfileInterpolation",
                "ProfileList" );
    return false;
    }
  if( !(*ProfileList) )
    {
    genMessage( WARNINGMSG(UNINITIALIZED_FIELD_ss),
                "ProfileInterpolation",
                "*ProfileList" );
    return false;
    }
  if( (*ProfileList)->getSize() == 0 )
    {
    genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),
               "ProfileInterpolation",
               "ProfileList",
               "Must have more values.");
    return false;
    }
  if( ! (*ProfileList)->isValid() )
    {
    genMessage( WARNINGMSG(INVALID_FIELD_VALUE_sss),
                "ProfileInterpolation",
                "ProfileList",
                "Must be a list of valid Geometry Objects." );
    return false;
    }


  if( !KnotList )
    {
    genMessage( WARNINGMSG(UNINITIALIZED_FIELD_ss),
                "ProfileInterpolation",
                "KnotList" );
    return false;
    }
  if( !(*KnotList) )
    {
    genMessage( WARNINGMSG(UNINITIALIZED_FIELD_ss),
                "ProfileInterpolation",
                "*KnotList" );
    return false;
    }

  uint32_t nbKnots= (*KnotList)->getSize();
  uint32_t nbProfiles= (*ProfileList)->getSize();
  if( nbKnots != nbProfiles )
    {
    genMessage( WARNINGMSG(INVALID_FIELD_SIZE_sss),
                "ProfileInterpolation",
                "KnotList",
                "Must have the same number of value than ProfileList." );
    return false;
    }

 return true;
}


/* ----------------------------------------------------------------------- */

/////////////////////////////////////////////////////////////////////////////

ProfileInterpolation::ProfileInterpolation( ) :
    __profileList(0),
    __knotList(0),
    __stride(DEFAULT_STRIDE),
    __degree(DEFAULT_DEGREE),
    __evalPt2D(0),
    __fctList2D(0),
    __evalPt3D(0),
    __fctList3D(0),
    __is2D(true)
{
}

/////////////////////////////////////////////////////////////////////////////

ProfileInterpolation::ProfileInterpolation( Curve2DArrayPtr _profileList,
                                            RealArrayPtr    _knotList,
                                            uint32_t          _degree,
                                            uint32_t          _stride ) :
  __profileList(_profileList),
  __knotList(_knotList),
  __stride(_stride),
  __degree(_degree),
  __evalPt2D(0),
  __fctList2D(0),
  __evalPt3D(0),
  __fctList3D(0),
  __is2D(true)
{
  GEOM_ASSERT( isValid() );
}

ProfileInterpolation::~ProfileInterpolation( ) {}

/////////////////////////////////////////////////////////////////////////////

bool ProfileInterpolation::isValid( ) const
{
  Builder _builder;
  RealArrayPtr _knot(0);

  _builder.ProfileList= const_cast< Curve2DArrayPtr* >(&__profileList);

  if( !isKnotListToDefault() )
    _builder.KnotList= const_cast< RealArrayPtr* >(&__knotList);

  if( !isDegreeToDefault() )
    _builder.Degree= const_cast< uint32_t* >(&__degree);

  if( !isStrideToDefault() )
    _builder.Stride= const_cast< uint32_t* >(&__stride);

  return _builder.isValid();
}

/////////////////////////////////////////////////////////////////////////////

bool ProfileInterpolation::isDegreeToDefault() const
{
  return ( __degree == DEFAULT_DEGREE );
}

/////////////////////////////////////////////////////////////////////////////

bool ProfileInterpolation::isStrideToDefault() const
{
  return ( __stride == DEFAULT_STRIDE );
}

/////////////////////////////////////////////////////////////////////////////

bool ProfileInterpolation::isKnotListToDefault() const
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////

real_t ProfileInterpolation::getUMin() const
{
  return ( (__knotList) ? __knotList->getAt(0) : 0);
}

/////////////////////////////////////////////////////////////////////////////

real_t ProfileInterpolation::getUMax() const
{
  return ( (__knotList) ? __knotList->getAt((__knotList->getSize())-1) : 1 );
}

/////////////////////////////////////////////////////////////////////////////

const Point2ArrayPtr& ProfileInterpolation::getSection2DAt(real_t u) const
{
#ifdef DEBUG
//cout<<"-> getSectionAt "<< u <<endl;
#endif

  if( !__fctList2D )
    return __evalPt2D;

  GEOM_ASSERT( __is2D );
  GEOM_ASSERT( __evalPt2D && __fctList2D );
  GEOM_ASSERT( __evalPt2D->getSize() == __fctList2D->getSize() );

  Curve2DArray::const_iterator itBegin= __fctList2D->getBegin();
  Curve2DArray::const_iterator itEnd=   __fctList2D->getEnd();
  Curve2DArray::const_iterator it= itBegin;

  Point2Array::iterator itPt= __evalPt2D->getBegin();

  for( it= itBegin; it < itEnd; it++, itPt++ )
    {
    Curve2DPtr nurbs= *it;
    *itPt= nurbs->getPointAt(u);
    }
#ifdef DEBUG
//cout<<"<-"<<endl;
#endif
  return __evalPt2D;
}

/////////////////////////////////////////////////////////////////////////////

const Point3ArrayPtr& ProfileInterpolation::getSection3DAt(real_t u) const
{
#ifdef DEBUG
//cout<<"-> getSectionAt "<< u <<endl;
#endif

  if( !__fctList3D )
    return __evalPt3D;

  GEOM_ASSERT( !__is2D );
  GEOM_ASSERT( __evalPt3D && __fctList3D );
  GEOM_ASSERT( __evalPt3D->getSize() == __fctList3D->getSize() );

  CurveArray::const_iterator itBegin= __fctList3D->getBegin();
  CurveArray::const_iterator itEnd=   __fctList3D->getEnd();
  CurveArray::const_iterator it= itBegin;

  Point3Array::iterator itPt= __evalPt3D->getBegin();

  for( it= itBegin; it < itEnd; it++, itPt++ )
    {
    LineicModelPtr nurbs= *it;
    *itPt= nurbs->getPointAt(u);
    }
#ifdef DEBUG
//cout<<"<-"<<endl;
#endif
  return __evalPt3D;
}

/////////////////////////////////////////////////////////////////////////////
const Curve2DArrayPtr&
ProfileInterpolation::getProfileList( ) const
{
  return __profileList;
}

Curve2DArrayPtr&
ProfileInterpolation::getProfileList( )
{
  return __profileList;
}

const RealArrayPtr&
ProfileInterpolation::getKnotList() const
{
  return __knotList;
}

RealArrayPtr&
ProfileInterpolation::getKnotList()
{
  return __knotList;
}

const uint32_t&
ProfileInterpolation::getStride( ) const
{
  return __stride;
}

uint32_t&
ProfileInterpolation::getStride( )
{
  return __stride;
}

const uint32_t&
ProfileInterpolation::getDegree( ) const
{
  return __degree;
}

uint32_t&
ProfileInterpolation::getDegree( )
{
  return __degree;
}

const bool&
ProfileInterpolation::is2DInterpolMode() const
{
  return __is2D;
}



/////////////////////////////////////////////////////////////////////////////

bool ProfileInterpolation::interpol()
{

#ifdef DEBUG
cout<<"-> interpol"<<endl;
#endif

  uint32_t n= __profileList->getSize();

  // uint32_t i= 0;
  if( __stride <= 2 )
    {
    __stride= 0;

    Curve2DArray::const_iterator itBegin= __profileList->getBegin();
    Curve2DArray::const_iterator it= itBegin;
    Curve2DArray::const_iterator itEnd= __profileList->getEnd();

    for( it= itBegin; it != itEnd; it++ )
      {
      Curve2DPtr p= *it;
      uint32_t stride= p->getStride();

      if( stride > __stride )
        __stride= stride;
      }

    if( __stride < 2 ) __stride= 2;
  }

#ifdef DEBUG
cout<<"Stride: "<<__stride<<endl;
#endif

  if( n == 1 )
    {
    __is2D= true;
    Curve2DPtr p = __profileList->getAt(0);
    Polyline2DPtr poly2D= Polyline2DPtr::Cast( p );

    if( poly2D )
      {
      __evalPt2D= poly2D->getPointList();
      }
    else
      {
      real_t u_start= p->getFirstKnot();
      real_t u_end=   p->getLastKnot();

      real_t step= (u_end - u_start) / real_t(__stride-1);

      __evalPt2D= Point2ArrayPtr( new Point2Array( __stride ) );

      real_t u= u_start;
      for(uint32_t i= 0; i < __stride; i++ )
        {
        Vector2 point= p->getPointAt(u);
        __evalPt2D->setAt( i, point );
        u+= step;
        }
      }

    __fctList2D= Curve2DArrayPtr(0);
    __evalPt3D= Point3ArrayPtr(0);
    __fctList3D= CurveArrayPtr(0);

    return true;
    }

  if( n <= 3 )
    __is2D= true;
  else
    __is2D= false;

#ifdef DEBUG
cout<<"is2D? "<<__is2D<<endl;
#endif

  Point2ArrayPtr allPts2D(0);
  Point3ArrayPtr allPts3D(0);

  if( __is2D )
    allPts2D= Point2ArrayPtr ( new Point2Array( n * __stride) );
  else
    allPts3D= Point3ArrayPtr( new Point3Array( n * __stride) );

  real_t cosa= 0., sina= 0.;
  uint32_t j= 0;
  for( j= 0; j < n ; j++ )
    {
    Curve2DPtr p= __profileList->getAt(j);

    real_t u_start= p->getFirstKnot();
    real_t u_end=   p->getLastKnot();
    real_t step= (u_end - u_start) / real_t(__stride-1);
    real_t u= u_start;

    if( !__is2D )
      {
      real_t angle= __knotList->getAt(j);
      cosa= cos(angle);
      sina= sin(angle);
      }
    for(uint32_t i= 0; i < __stride; i++ )
      {
      Vector2 pt= p->getPointAt(u);
      GEOM_ASSERT( j+i*n < n * __stride );
      if( __is2D )
        allPts2D->setAt( j+i*n, pt );
      else
        {
        real_t r= pt.x();
        Vector3 pt3(r*cosa, r*sina, pt.y());
        allPts3D->setAt( j+i*n, pt3 );
        }
      u+= step;
      }
    }

  if(__is2D)
    {
   __fctList2D= Curve2DArrayPtr( new Curve2DArray(__stride) );
    Point2ArrayPtr pts;
    Point2Array::iterator itpBegin= allPts2D->getBegin();
    Point2Array::iterator itpEnd= itpBegin + n;
    for(uint32_t i= 0; i < __stride; i++ )
      {
      pts= Point2ArrayPtr(new Point2Array(itpBegin, itpEnd));
      Point3ArrayPtr pts3D( new Point3Array(*pts, 1.) );
      Interpol local(pts3D, __knotList, __degree, 1 );
#ifdef DEBUG
cout<<"get2DCurve "<<i<<endl;
#endif
      __fctList2D->getAt(i)= local.get2DCurve();
      if(itpEnd != allPts2D->getEnd()){ itpBegin+= n; itpEnd+= n; }
      }
    __evalPt2D= Point2ArrayPtr( new Point2Array( __stride ) );
    __evalPt3D= Point3ArrayPtr(0);
    __fctList3D= CurveArrayPtr(0);

    }
  else
  {
	  __fctList3D= CurveArrayPtr( new CurveArray(__stride) );
	  Point3ArrayPtr pts;
	  Point3Array::iterator itpBegin= allPts3D->getBegin();
	  Point3Array::iterator itpEnd= itpBegin + n;
	  for(uint32_t i= 0; i < __stride; i++ )
	  {
		  pts= Point3ArrayPtr(new Point3Array(itpBegin, itpEnd));
		  Interpol local(pts, __knotList, __degree, 1 );
#ifdef DEBUG
		  cout<<"get3DCurve "<<i<<endl;
#endif
		  __fctList3D->getAt(i)= local.get3DCurve();
		  if(itpEnd != allPts3D->getEnd()){ itpBegin+= n; itpEnd+= n; }
	  }
	  __evalPt3D= Point3ArrayPtr(new Point3Array( __stride ));
	  __fctList2D= Curve2DArrayPtr(0);
	  __evalPt2D= Point2ArrayPtr(0);
  }

#ifdef DEBUG
cout<<"<-"<<endl;
#endif

  return true;
}


/////////////////////////////////////////////////////////////////////////////
