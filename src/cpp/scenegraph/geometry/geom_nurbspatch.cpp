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





#include "geom_nurbspatch.h"
#include "geom_nurbscurve.h"
#include "util_messages.h"
#include "Tools/util_string.h"
#include "Tools/util_math.h"
#include "geom_pointmatrix.h"
#include "geom_pointarray.h"
#include "util_array.h"

GEOM_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */

const uint32_t
NurbsPatch::DEFAULT_NURBS_DEGREE(NurbsCurve::DEFAULT_NURBS_DEGREE);

/* ----------------------------------------------------------------------- */

NurbsPatch::Builder::Builder( ) :
  BezierPatch::Builder(),
         UKnotList(0),
         VKnotList(0){
}


NurbsPatch::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr NurbsPatch::Builder::build( ) const {
    if (isValid()){

        uint32_t _udegree=0;
        RealArrayPtr * _uknots = NULL;
        uint32_t _vdegree=0;
        RealArrayPtr *_vknots = NULL;

        if( ! UDegree ){
            if( ! UKnotList )_udegree = min ( NurbsCurve::DEFAULT_NURBS_DEGREE,(*CtrlPointMatrix)->getRowsNb() - 1) ;
            else _udegree = (*UKnotList)->getSize() - (*CtrlPointMatrix)->getRowsNb() - 1;
#ifdef GEOM_DEBUG
            cout << "UDegree value assign to " <<  _udegree << endl;
#endif
        }
        else _udegree = (*UDegree);

        if( ! VDegree ){
            if( ! VKnotList ) _vdegree = min ( NurbsCurve::DEFAULT_NURBS_DEGREE, (*CtrlPointMatrix)->getColsNb() - 1);
            else _vdegree = (*VKnotList)->getSize() - (*CtrlPointMatrix)->getColsNb() - 1;
#ifdef GEOM_DEBUG
            cout << "VDegree value assign to " <<  _vdegree  << endl;
#endif
        }
        else _vdegree = (*VDegree);

        if( ! UKnotList ){
            uint32_t _usize = ((*CtrlPointMatrix)->getRowsNb()) + _udegree + 1;
            _uknots = new RealArrayPtr(new RealArray(_usize));
            for (uint32_t i = 0 ; i < _udegree+1 ; i++ )
                (*_uknots)->setAt(i , 0.0);

            for( uint32_t j =   _udegree+1 ; j<_usize - _udegree - 1; j++ )
                (*_uknots)->setAt(j ,(real_t) ( j - _udegree )  /(real_t)(_usize - (2 * _udegree)  - 1 ));

            for (uint32_t k = _usize - _udegree - 1 ; k < _usize ; k++ )
                (*_uknots)->setAt(k , 1.0);

#ifdef GEOM_DEBUG
            cout << "UKnotList assign to a vector of size " << (*_uknots)->getSize() <<endl;
            cout << "UKnotList values is : "<<endl;
            for( int l = 0 ; l < (*_uknots)->getSize() ; l++ )
                cout << " " << (*_uknots)->getAt(l);
            cout << endl;
#endif
        }

        if( ! VKnotList ){
            uint32_t _vsize = ((*CtrlPointMatrix)->getColsNb()) + _vdegree + 1;
            _vknots = new RealArrayPtr(new RealArray(_vsize));
            for (uint32_t i = 0 ; i < _vdegree+1 ; i++ )
                (*_vknots)->setAt(i , 0.0);

            for( uint32_t j =   _vdegree+1 ; j<_vsize - _vdegree - 1; j++ )
                (*_vknots)->setAt(j ,(real_t) ( j - _vdegree )  /(real_t)(_vsize - (2 * _vdegree)  - 1 ));

            for (uint32_t k = _vsize - _vdegree - 1 ; k < _vsize ; k++ )
                (*_vknots)->setAt(k , 1.0);

#ifdef GEOM_DEBUG
            cout << "VKnotList assign to a vector of size " << (*_vknots)->getSize() <<endl;
            cout << "VKnotList values is : "<<endl;
            for( int l = 0 ; l < (*_vknots)->getSize() ; l++ )
                cout << " " << (*_vknots)->getAt(l);
            cout << endl;
#endif
        }
        if(! UKnotList && !  VKnotList ){
            SceneObjectPtr myNurbsPtr(new NurbsPatch(*CtrlPointMatrix,*_uknots,*_vknots, _udegree,
                                                     _vdegree, (UStride ? *UStride : DEFAULT_STRIDE),
                                                     (VStride ? *VStride : DEFAULT_STRIDE),
                                                     (CCW ? *CCW : DEFAULT_CCW)));
            delete _uknots;
            delete _vknots;
            return myNurbsPtr;
        }
        else if ( ! UKnotList ){
            SceneObjectPtr myNurbsPtr(new NurbsPatch(*CtrlPointMatrix, *_uknots,*VKnotList,_udegree,
                                                     _vdegree, (UStride ? *UStride : DEFAULT_STRIDE),
                                                     (VStride ? *VStride : DEFAULT_STRIDE),
                                                     (CCW ? *CCW : DEFAULT_CCW)));
            delete _uknots;
            return myNurbsPtr;
        }
        else if ( ! VKnotList ){
            SceneObjectPtr myNurbsPtr(new NurbsPatch(*CtrlPointMatrix,*UKnotList,*_vknots,
                                                     _udegree, _vdegree,
                                                     (UStride ? *UStride : DEFAULT_STRIDE),
                                                     (VStride ? *VStride : DEFAULT_STRIDE),
                                                     (CCW ? *CCW : DEFAULT_CCW)));
            delete _vknots;
            return myNurbsPtr;
        }
        else return SceneObjectPtr(new NurbsPatch(*CtrlPointMatrix,*UKnotList,*VKnotList,
                                                  _udegree, _vdegree,
                                                  (UStride ? *UStride : DEFAULT_STRIDE),
                                                  (VStride ? *VStride : DEFAULT_STRIDE),
                                                  (CCW ? *CCW : DEFAULT_CCW)));
    }
    return SceneObjectPtr();
}


void NurbsPatch::Builder::destroy( ) {
  if (CtrlPointMatrix) delete CtrlPointMatrix;
  if (UDegree) delete UDegree;
  if (VDegree) delete VDegree;
  if (UStride) delete UStride;
  if (VStride) delete VStride;
  if (UKnotList) delete UKnotList;
  if (VKnotList) delete VKnotList;
  if (CCW) delete CCW;
}


bool NurbsPatch::Builder::isValid( ) const {

  // CtrlPointList field
    if ( !CtrlPointMatrix ) {
        genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"Nurbs Patch","CtrlPointMatrix");
        return false;
    }
    uint32_t _usize = (*CtrlPointMatrix)->getRowsNb();
    uint32_t _vsize = (*CtrlPointMatrix)->getColsNb();

    if (_usize < 2 ) {
        genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"Nurbs Patch","CtrlPointMatrix","Rows must be greater than 1.");
        return false;
    }

    if (_vsize < 2 ) {
        genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"Nurbs Patch","CtrlPointMatrix","Columns must be greater than 1.");
        return false;
    }

    for(uint32_t i=0; i < _usize;i++)
        for(uint32_t j=0; j < _vsize;j++){
            if(fabs((*CtrlPointMatrix)->getAt(i,j).w()) < GEOM_TOLERANCE) {
                string _ith = '(' + number(i + 1) + " , " + number(j+1) + ')';
                genMessage(WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"Nurbs Patch","CtrlPointMatrix",_ith.c_str(),"Weight must be not null");
                return false;
            }
        }

    // UDegree field
    if (UDegree!=NULL && (*UDegree < 1 || *UDegree >= (*CtrlPointMatrix)->getRowsNb())) {
        genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Nurbs Patch","UDegree","Must be greater than 0 and smaller than the number of control points in a row.");
        return false;
    };

    // uknot list
    if( UKnotList != NULL && (*UKnotList)->getSize() < _usize + 2 ){
        genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"Nurbs Patch","UKnotList","Number of knots must be greater than the number of control points in a row.");
        return false;
    };

    // vknot list
    if( VKnotList != NULL && (*VKnotList)->getSize() < _vsize + 2 ){
        genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"Nurbs Patch","VKnotList","Number of knots must be greater than the number of control points in a column.");
        return false;
    };

    // VDegree field
    if (VDegree && (*VDegree < 1 || *VDegree >= (*CtrlPointMatrix)->getColsNb()) ) {
        genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Nurbs Patch","VDegree","Must be greater than 0 and smaller than the number of control points in a column.");
        return false;
    };

    // UStride field
    if ( UStride && *UStride < 1 ) {
        genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Nurbs Patch","UStride","Must be greater than 0.");
        return false;
    };

    // VStride field
    if ( VStride && *VStride < 1 ) {
        genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Nurbs Patch","VStride","Must be greater than 0.");
        return false;
    };

    // Clamped knot vector test
    if( UKnotList ){
        uint32_t _deg;
        if (UDegree) _deg = (*UDegree);
        else _deg =  ((*UKnotList)->getSize()) - _usize -1;
        real_t _val = (*UKnotList)->getAt(0);
        for (uint32_t i1 = 1 ; i1 < _deg+1 ; i1++ )
            if( !(fabs ((*UKnotList)->getAt(i1) - _val ) <GEOM_TOLERANCE )){
                genMessage(WARNINGMSG(INVALID_FIELD_VECTOR_TYPE_ssss),"Nurbs Patch","UKnotList",number(i1+1).c_str(),"Must be a clamped vector.");
                return false;
            }
        uint32_t _knsize = (*UKnotList)->getSize();
        _val = (*UKnotList)->getAt(_knsize -1  );
        for (uint32_t j1 = _knsize - _deg - 1 ; j1 < _knsize ; j1++ )
            if( !(fabs ((*UKnotList)->getAt(j1) -_val ) <GEOM_TOLERANCE )){
                genMessage(WARNINGMSG(INVALID_FIELD_VECTOR_TYPE_ssss),"Nurbs Patch","UKnotList",number(j1+1).c_str(),"Must be a clamped vector.");
                return false;
            }

    };

    // Clamped knot vector test
    if( VKnotList ){
        uint32_t _deg;
        if (VDegree ) _deg = (*VDegree);
        else _deg =  ((*VKnotList)->getSize()) - _vsize -1;
        real_t _val = (*VKnotList)->getAt(0);
        for (uint32_t i = 1 ; i < _deg+1 ; i++ )
            if( !(fabs ((*VKnotList)->getAt(i) - _val ) <GEOM_TOLERANCE )){
                genMessage(WARNINGMSG(INVALID_FIELD_VECTOR_TYPE_ssss),"Nurbs Patch","VKnotList",number(i+1).c_str(),"Must be a clamped vector.");
                return false;
            }
        uint32_t _knsize = (*VKnotList)->getSize();
        _val = (*VKnotList)->getAt(_knsize -1  );
        for (uint32_t j = _knsize - _deg - 1 ; j < _knsize ; j++ )
            if( !(fabs ((*VKnotList)->getAt(j) -_val ) <GEOM_TOLERANCE )){
                genMessage(WARNINGMSG(INVALID_FIELD_VECTOR_TYPE_ssss),"Nurbs Patch","VKnotList",number(j+1).c_str(),"Must be a clamped vector.");
                return false;
            }

    };


    // Fields dependency
    if (  UDegree != NULL &&
          UKnotList != NULL  &&
           _usize != ( (*UKnotList)->getSize() -  *UDegree - 1)) {
        genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"Nurbs Patch","UKnotList","Number of knots must be coherent with degree and number of control points in a row.");
        return false;
    };
    if (  VDegree != NULL &&
          VKnotList != NULL  &&
          _vsize != ( (*VKnotList)->getSize() -  *VDegree - 1)) {
        genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"Nurbs Patch","VKnotList","Number of knots must be coherent with degree and number of control points in a columns.");
        return false;
    };

    return true;
}


/* ----------------------------------------------------------------------- */

NurbsPatch::NurbsPatch( ) :
    BezierPatch(),
    __uKnotList(0),
    __vKnotList(0),
    __udegree(DEFAULT_NURBS_DEGREE),
    __vdegree(DEFAULT_NURBS_DEGREE){
}
NurbsPatch::NurbsPatch( const Point4MatrixPtr& ctrlPoints,
                        const RealArrayPtr& uKnotList,
                        const RealArrayPtr& vKnotList,
                        uint32_t uDegree, uint32_t vDegree,
                        uint32_t ustride , uint32_t vstride,  bool ccw ) :
    BezierPatch(ctrlPoints,ustride,vstride,ccw),
    __uKnotList(uKnotList),
    __vKnotList(vKnotList),
    __udegree(uDegree),
    __vdegree(vDegree){
    GEOM_ASSERT(isValid());
}

NurbsPatch::~NurbsPatch( ) {
}

bool
NurbsPatch::apply( Action& action ) {
  return action.process(this);
}

SceneObjectPtr NurbsPatch::copy() const {
  NurbsPatch * ptr = new NurbsPatch(*this);
  if(__ctrlPointMatrix)ptr->getCtrlPointMatrix( ) = Point4MatrixPtr(new Point4Matrix(*__ctrlPointMatrix));
  if(__uKnotList)ptr->getUKnotList( ) = RealArrayPtr(new RealArray(*__uKnotList));
  if(__vKnotList)ptr->getVKnotList( ) = RealArrayPtr(new RealArray(*__vKnotList));
  return SceneObjectPtr(ptr);
}


/* ----------------------------------------------------------------------- */
const uint32_t
NurbsPatch::getUDegree( ) const {
  return __udegree;
}

uint32_t&
NurbsPatch::getUDegree( ) {
  return __udegree;
}

const uint32_t
NurbsPatch::getVDegree( ) const {
  return __vdegree;
}

uint32_t&
NurbsPatch::getVDegree( )  {
  return __vdegree;
}

bool
NurbsPatch::isUDegreeToDefault( ) const {
  return (__udegree == NurbsCurve::DEFAULT_NURBS_DEGREE);
}

uint32_t
NurbsPatch::isVDegreeToDefault( ) const {
  return (__vdegree == NurbsCurve::DEFAULT_NURBS_DEGREE);
}

const RealArrayPtr&
NurbsPatch::getUKnotList( ) const {
  return __uKnotList;
}

RealArrayPtr&
NurbsPatch::getUKnotList( ) {
  return __uKnotList;
}

const real_t
NurbsPatch::getFirstUKnot( ) const {
  return __uKnotList->getAt(0);
}

const real_t
NurbsPatch::getLastUKnot( ) const {
  return __uKnotList->getAt((__uKnotList->getSize())-1);
}


RealArrayPtr&
NurbsPatch::getVKnotList( ) {
  return __vKnotList;
}

const RealArrayPtr&
NurbsPatch::getVKnotList( )const {
  return __vKnotList;
}

real_t
NurbsPatch::getFirstVKnot( ) const {
  return __vKnotList->getAt(0);
}

real_t
NurbsPatch::getLastVKnot( ) const {
  return __vKnotList->getAt((__vKnotList->getSize())-1);
}

/* ----------------------------------------------------------------------- */

bool NurbsPatch::setUKnotListToDefault( ){
    if(!__ctrlPointMatrix) return false;
    uint32_t _size = (__ctrlPointMatrix->getRowsNb()) + __udegree + 1;
    __uKnotList = RealArrayPtr(new RealArray(_size));
    for (uint32_t i = 0 ; i < __udegree+1 ; i++ )
        (__uKnotList)->setAt(i , 0.0);

    for( uint32_t j =   __udegree+1 ; j<_size - __udegree - 1; j++ )
        (__uKnotList)->setAt(j ,(real_t) ( j - __udegree )  /(real_t)(_size - (2 * __udegree)  - 1 ));

    for (uint32_t k = _size - __udegree - 1 ; k < _size ; k++ )
        (__uKnotList)->setAt(k , 1.0);
    return true;
}

bool NurbsPatch::isUKnotListToDefault( ) const{
    uint32_t _size=getUKnotList()->getSize();
    real_t _val = __uKnotList->getAt(0);
    for (uint32_t i = 1 ; i < __udegree+1 ; i++ )
        if( !(fabs (__uKnotList->getAt(i) -_val ) <GEOM_TOLERANCE) ) return false;

    _val = real_t(1.0) / (real_t)(_size - (2 * __udegree)  - 1 );
    for(uint32_t j= __udegree+1;j<_size - __udegree -1;j++)
        if(!(fabs(fabs(__uKnotList->getAt(j+1)-__uKnotList->getAt(j))-_val)<GEOM_EPSILON))
            return false;

    _val = __uKnotList->getAt(_size -1  );
    for (uint32_t k = _size - __udegree - 1 ; k < _size ; k++ )
        if( !(fabs (__uKnotList->getAt(k) -_val ) <GEOM_TOLERANCE) ) return false;
    return true;
}


bool NurbsPatch::setVKnotListToDefault( ){
    if(!__ctrlPointMatrix) return false;
    uint32_t _size = (__ctrlPointMatrix->getColsNb()) + __vdegree + 1;
    __vKnotList = RealArrayPtr(new RealArray(_size));
    for (uint32_t i = 0 ; i < __vdegree+1 ; i++ )
        (__vKnotList)->setAt(i , 0.0);

    for( uint32_t j =   __vdegree+1 ; j<_size - __vdegree - 1; j++ )
        (__vKnotList)->setAt(j ,(real_t) ( j - __vdegree )  /(real_t)(_size - (2 * __vdegree)  - 1 ));

    for (uint32_t k = _size - __vdegree - 1 ; k < _size ; k++ )
        (__vKnotList)->setAt(k , 1.0);
    return true;
}

bool NurbsPatch::isVKnotListToDefault( ) const{
    uint32_t _size=getVKnotList()->getSize();
    real_t _val = __vKnotList->getAt(0);
    for (uint32_t i = 1 ; i < __vdegree+1 ; i++ )
        if( !(fabs (__vKnotList->getAt(i) -_val ) <GEOM_TOLERANCE) ) return false;

    _val = real_t(1.0) / (real_t)(_size - (2 * __vdegree)  - 1 );
    for(uint32_t j= __vdegree+1;j<_size - __vdegree -1;j++)
        if(!(fabs(fabs(__vKnotList->getAt(j+1)-__vKnotList->getAt(j))-_val)<GEOM_EPSILON))
            return false;

    _val = __vKnotList->getAt(_size -1  );
    for (uint32_t k = _size - __vdegree - 1 ; k < _size ; k++ )
        if( !(fabs (__vKnotList->getAt(k) -_val ) <GEOM_TOLERANCE) ) return false;
    return true;
}



Vector3 NurbsPatch::getPointAt(real_t u, real_t v) const{
  GEOM_ASSERT( u >= 0.0 && u <= 1.0 && v>= 0.0 && v<=1.0);

  uint32_t uspan = findSpan(__udegree,__uKnotList,u);
  RealArrayPtr Nu = basisFunctions(uspan, u, __udegree, __uKnotList);
  uint32_t vspan = findSpan(__vdegree,__vKnotList,v);
  RealArrayPtr Nv = basisFunctions(vspan, v, __vdegree, __vKnotList);
  Point4Array temp(__vdegree+1);
  for (uint32_t l = 0 ; l <= __vdegree ; l++ ){
      temp.setAt(l,Vector4(0,0,0,0));
      for (uint32_t k = 0 ; k <= __udegree ; k++ )
          temp.setAt(l, temp.getAt(l) + (__ctrlPointMatrix->getAt(uspan - __udegree +k,vspan - __vdegree +l) *  (Nu->getAt(k)))) ;
  }


  Vector4 Sw( 0 , 0 , 0 ,0 );

  for (uint32_t l2 = 0; l2 <= __vdegree ; l2++)
      Sw = Sw + (temp.getAt(l2) * Nv->getAt(l2));

  if (fabs(Sw.w()) < GEOM_TOLERANCE)
    return Vector3(Sw.x(),Sw.y(),Sw.z());

  return Sw.project();
}


bool NurbsPatch::isValid( ) const {
    Builder _builder;
  _builder.CtrlPointMatrix = const_cast<Point4MatrixPtr *>(&__ctrlPointMatrix);
  _builder.UStride = const_cast<uint32_t *>(&__ustride);
  _builder.VStride = const_cast<uint32_t *>(&__vstride);
  _builder.UDegree = const_cast<uint32_t *>(&__udegree);
  _builder.VDegree = const_cast<uint32_t *>(&__vdegree);
  _builder.UKnotList = const_cast<RealArrayPtr *>(&__uKnotList);
  _builder.VKnotList= const_cast<RealArrayPtr *>(&__vKnotList);
  _builder.CCW= const_cast<bool *>(&__ccw);
  return _builder.isValid();
}


/*Vector3 NurbsPatch::getPointAt2(real_t u) const{
}*/

/*Vector3 NurbsPatch::getTangentAt(real_t u) const{
}*/


/* ----------------------------------------------------------------------- */


  /* Computational Algorithms */

uint32_t NurbsPatch::findSpan(uint32_t deg, const RealArrayPtr& U,  real_t u) const{
    uint32_t n = U->getSize() - deg - 1 ;
    if( u >= U->getAt( n ) )return ( n - 1 );
    if( u <= U->getAt( deg )  )return ( deg );

    uint32_t low = deg;
    uint32_t high = n ;
    uint32_t mid = ( low + high ) / 2;
    real_t _knot =  U->getAt(mid);

    while (u < _knot  || u >= U->getAt(mid+1) ){
        if ( u < _knot ) high = mid;
        else low = mid;
        mid = ( low + high ) / 2;
        _knot = U->getAt(mid);
    }

    return mid;
}
/*
RealArrayPtr NurbsPatch::computeBasisFunctions(uint32_t span, real_t u, uint32_t deg,const RealArrayPtr& U ) const{

  RealArrayPtr BasisFunctions(new RealArray( deg + 1));
  real_t left[ deg+1 ];
  real_t right[ deg+1 ];
  real_t saved;

  BasisFunctions->setAt(0,1.0);

  for( uint32_t j = 1 ; j <= deg ; j++ ){
    left[j] = u - U->getAt(span + 1 -j) ;
    right[j] = U->getAt(span + j) - u ;
    saved = 0.0;
    for ( uint32_t r = 0; r < j; r++){
      real_t temp = BasisFunctions->getAt(r) / ( right[r+1] + left[j-r] );
      BasisFunctions->setAt(r, saved + ( right[r+1] * temp ));
      saved = left[j-r] * temp;
    }
    BasisFunctions->setAt(j,saved);
  }
  return BasisFunctions;
}
*/
