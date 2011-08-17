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





#include "nurbspatch.h"
#include "nurbscurve.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/math/util_math.h>
#include <plantgl/tool/util_array.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/scenegraph/container/pointarray.h>
//#include <iostream>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */

const uint_t NurbsPatch::DEFAULT_NURBS_DEGREE(NurbsCurve::DEFAULT_NURBS_DEGREE);

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

        uint_t _udegree=0;
        RealArrayPtr _uknots ;
        uint_t _vdegree=0;
        RealArrayPtr _vknots ;

        if( ! UDegree ){
            if( ! UKnotList )_udegree = min ( NurbsCurve::DEFAULT_NURBS_DEGREE,(*CtrlPointMatrix)->getRowSize() - 1) ;
            else _udegree = (*UKnotList)->size() - (*CtrlPointMatrix)->getRowSize() - 1;
#ifdef GEOM_DEBUG
            //cout << "UDegree value assign to " <<  _udegree << endl;
#endif
        }
        else _udegree = (*UDegree);

        if( ! VDegree ){
            if( ! VKnotList ) _vdegree = min ( NurbsCurve::DEFAULT_NURBS_DEGREE, (*CtrlPointMatrix)->getColumnSize() - 1);
            else _vdegree = (*VKnotList)->size() - (*CtrlPointMatrix)->getColumnSize() - 1;
#ifdef GEOM_DEBUG
            //cout << "VDegree value assign to " <<  _vdegree  << endl;
#endif
        }
        else _vdegree = (*VDegree);

        if( UKnotList ){ _uknots = *UKnotList; }
        if( VKnotList ){ _vknots = *VKnotList; }

        return SceneObjectPtr(new NurbsPatch(*CtrlPointMatrix,_uknots,_vknots, _udegree,
                                                     _vdegree, (UStride ? *UStride : DEFAULT_STRIDE),
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
        pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"Nurbs Patch","CtrlPointMatrix");
        return false;
    }
    uint_t _usize = (*CtrlPointMatrix)->getColumnSize();
    uint_t _vsize = (*CtrlPointMatrix)->getRowSize();

    if (_usize < 2 ) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Nurbs Patch","CtrlPointMatrix","Size of Columnsmust be greater than 1.");
        return false;
    }

    if (_vsize < 2 ) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Nurbs Patch","CtrlPointMatrix","Size of Rows  must be greater than 1.");
        return false;
    }

    for(uint_t i=0; i < _usize;i++){
        for(uint_t j=0; j < _vsize;j++)
            if(fabs((*CtrlPointMatrix)->getAt(i,j).w()) < GEOM_TOLERANCE) {
                string _ith = '(' + number(i) + " , " + number(j) + ')';
                pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"Nurbs Patch","CtrlPointMatrix",_ith.c_str(),"Weight must be not null");
                return false;
            }
        }

    // UDegree field
    if (UDegree!=NULL && (*UDegree < 1 || *UDegree >= _usize)) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Nurbs Patch","UDegree","Must be greater than 0 and smaller than the number of control points in a column.");
        return false;
    };

    // uknot list
    if( UKnotList != NULL && (*UKnotList)->size() < _usize + 2 ){
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Nurbs Patch","UKnotList","Number of knots must be greater than the number of control points in a column.");
        return false;
    };

    // vknot list
    if( VKnotList != NULL && (*VKnotList)->size() < _vsize + 2 ){
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Nurbs Patch","VKnotList","Number of knots must be greater than the number of control points in a row.");
        return false;
    };

    // VDegree field
    if (VDegree && (*VDegree < 1 || *VDegree >= _vsize) ) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Nurbs Patch","VDegree","Must be greater than 0 and smaller than the number of control points in a row.");
        return false;
    };

    // UStride field
    if ( UStride && *UStride < 1 ) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Nurbs Patch","UStride","Must be greater than 0.");
        return false;
    };

    // VStride field
    if ( VStride && *VStride < 1 ) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Nurbs Patch","VStride","Must be greater than 0.");
        return false;
    };

    // Clamped knot vector test
    if( UKnotList ){
        uint_t _deg;
        if (UDegree) _deg = (*UDegree);
        else _deg =  ((*UKnotList)->size()) - _usize -1;
        real_t _val = (*UKnotList)->getAt(0);
        for (uint_t i1 = 1 ; i1 < _deg+1 ; i1++ )
            if( !(fabs ((*UKnotList)->getAt(i1) - _val ) <GEOM_TOLERANCE )){
                pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VECTOR_TYPE_ssss),"Nurbs Patch","UKnotList",number(i1+1).c_str(),"Must be a clamped vector.");
                return false;
            }
        uint_t _knsize = (*UKnotList)->size();
        _val = (*UKnotList)->getAt(_knsize -1  );
        for (uint_t j1 = _knsize - _deg - 1 ; j1 < _knsize ; j1++ )
            if( !(fabs ((*UKnotList)->getAt(j1) -_val ) <GEOM_TOLERANCE )){
                pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VECTOR_TYPE_ssss),"Nurbs Patch","UKnotList",number(j1+1).c_str(),"Must be a clamped vector.");
                return false;
            }

    };

    // Clamped knot vector test
    if( VKnotList ){
        uint_t _deg;
        if (VDegree ) _deg = (*VDegree);
        else _deg =  ((*VKnotList)->size()) - _vsize -1;
        real_t _val = (*VKnotList)->getAt(0);
        for (uint_t i = 1 ; i < _deg+1 ; i++ )
            if( !(fabs ((*VKnotList)->getAt(i) - _val ) <GEOM_TOLERANCE )){
                pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VECTOR_TYPE_ssss),"Nurbs Patch","VKnotList",number(i+1).c_str(),"Must be a clamped vector.");
                return false;
            }
        uint_t _knsize = (*VKnotList)->size();
        _val = (*VKnotList)->getAt(_knsize -1  );
        for (uint_t j = _knsize - _deg - 1 ; j < _knsize ; j++ )
            if( !(fabs ((*VKnotList)->getAt(j) -_val ) <GEOM_TOLERANCE )){
                pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VECTOR_TYPE_ssss),"Nurbs Patch","VKnotList",number(j+1).c_str(),"Must be a clamped vector.");
                return false;
            }

    };


    // Fields dependency
    if (  UDegree != NULL &&
          UKnotList != NULL  &&
           _usize != ( (*UKnotList)->size() -  *UDegree - 1)) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Nurbs Patch","UKnotList","Number of knots must be coherent with degree and number of control points in a column.");
        return false;
    };
    if (  VDegree != NULL &&
          VKnotList != NULL  &&
          _vsize != ( (*VKnotList)->size() -  *VDegree - 1)) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Nurbs Patch","VKnotList","Number of knots must be coherent with degree and number of control points in a row.");
        return false;
    };

    return true;
}


/* ----------------------------------------------------------------------- */

NurbsPatch::NurbsPatch( ) :
    BezierPatch(),
    __uKnotList(),
    __vKnotList(),
    __udegree(DEFAULT_NURBS_DEGREE),
    __vdegree(DEFAULT_NURBS_DEGREE){
}

NurbsPatch::NurbsPatch( const Point4MatrixPtr& ctrlPoints,
                        const RealArrayPtr& uKnotList,
                        const RealArrayPtr& vKnotList,
                        uint_t uDegree, uint_t vDegree,
                        uint_t ustride , uint_t vstride,  bool ccw ) :
    BezierPatch(ctrlPoints,ustride,vstride,ccw),
    __uKnotList(uKnotList),
    __vKnotList(vKnotList),
    __udegree(uDegree),
    __vdegree(vDegree){
    if (__udegree >= ctrlPoints->getRowSize()) __udegree = ctrlPoints->getColumnSize() -1;
    if (__vdegree >= ctrlPoints->getColumnSize()) __vdegree = ctrlPoints->getRowSize() -1;
    if (!uKnotList) setUKnotListToDefault();
    if (!vKnotList) setVKnotListToDefault();
    GEOM_ASSERT(isValid());
}

NurbsPatch::NurbsPatch( const Point4MatrixPtr& ctrlPoints,
                        uint_t uDegree, uint_t vDegree,
                        const RealArrayPtr& uKnotList,
                        const RealArrayPtr& vKnotList,
                        uint_t ustride , uint_t vstride,  bool ccw ) :
    BezierPatch(ctrlPoints,ustride,vstride,ccw),
    __uKnotList(uKnotList),
    __vKnotList(vKnotList),
    __udegree(uDegree),
    __vdegree(vDegree){
    if (__udegree >= ctrlPoints->getRowSize()) __udegree = ctrlPoints->getColumnSize() -1;
    if (__vdegree >= ctrlPoints->getColumnSize()) __vdegree = ctrlPoints->getRowSize() -1;
    if (!uKnotList) setUKnotListToDefault();
    if (!vKnotList) setVKnotListToDefault();
    GEOM_ASSERT(isValid());
}

NurbsPatch::NurbsPatch( const Point3MatrixPtr& ctrlPoints,
                        uint_t uDegree, uint_t vDegree,
                        const RealArrayPtr& uKnotList,
                        const RealArrayPtr& vKnotList,
                        uint_t ustride , uint_t vstride,  bool ccw ) :
    BezierPatch(ctrlPoints,ustride,vstride,ccw),
    __uKnotList(uKnotList),
    __vKnotList(vKnotList),
    __udegree(uDegree),
    __vdegree(vDegree){
    if (__udegree >= ctrlPoints->getRowSize()) __udegree = ctrlPoints->getColumnSize() -1;
    if (__vdegree >= ctrlPoints->getColumnSize()) __vdegree = ctrlPoints->getRowSize() -1;
    if (!uKnotList) setUKnotListToDefault();
    if (!vKnotList) setVKnotListToDefault();
    GEOM_ASSERT(isValid());
}

NurbsPatch::~NurbsPatch( ) {
}


SceneObjectPtr NurbsPatch::copy(DeepCopier& copier) const {
  NurbsPatch * ptr = new NurbsPatch(*this);
  copier.copy_attribute(ptr->getCtrlPointMatrix( ));
  copier.copy_attribute(ptr->getUKnotList( ));
  copier.copy_attribute(ptr->getVKnotList( ));
  return SceneObjectPtr(ptr);
}


/* ----------------------------------------------------------------------- */
const uint_t
NurbsPatch::getUDegree( ) const {
  return __udegree;
}

uint_t&
NurbsPatch::getUDegree( ) {
  return __udegree;
}

const uint_t
NurbsPatch::getVDegree( ) const {
  return __vdegree;
}

uint_t&
NurbsPatch::getVDegree( )  {
  return __vdegree;
}

bool
NurbsPatch::isUDegreeToDefault( ) const {
  return (__udegree == NurbsCurve::DEFAULT_NURBS_DEGREE);
}

uint_t
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
  return __uKnotList->getAt((__uKnotList->size())-1);
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
  return __vKnotList->getAt((__vKnotList->size())-1);
}

/* ----------------------------------------------------------------------- */

bool NurbsPatch::setUKnotListToDefault( ){
    if(!__ctrlPointMatrix) return false;
    __uKnotList = NurbsCurve::defaultKnotList(__ctrlPointMatrix->getColumnSize(),__udegree);
    return true;
}

bool NurbsPatch::isUKnotListToDefault( ) const{
    return NurbsCurve::defaultKnotListTest(getUKnotList(),__ctrlPointMatrix->getColumnSize(),__udegree);
}


bool NurbsPatch::setVKnotListToDefault( ){
    if(!__ctrlPointMatrix) return false;
    __vKnotList = NurbsCurve::defaultKnotList(__ctrlPointMatrix->getRowSize(),__vdegree);
    return true;
}

bool NurbsPatch::isVKnotListToDefault( ) const{
    return NurbsCurve::defaultKnotListTest(getVKnotList(),__ctrlPointMatrix->getRowSize(),__vdegree);
}


Point4MatrixPtr  NurbsPatch::deriveAtH(real_t u, real_t v, int d, int uspan, int vspan ) const {
    int du = ( d < (int)__udegree ? d : __udegree);
    int dv = ( d < (int)__vdegree ? d : __vdegree);
    Point4MatrixPtr patchders(new Point4Matrix(d+1,d+1));
    Point4Array temp(__vdegree+1, 0.0) ;
    RealArray2Ptr UderF = derivatesBasisFunctions(du,u,uspan,__udegree,__uKnotList);
    RealArray2Ptr VderF = derivatesBasisFunctions(dv,v,vspan,__vdegree,__vKnotList);
    int k, j = 0;
    for(k=__udegree;k<=d;++k){
        for(j=0;j<=d-k;++j){
            //cout<<"set patchders"<<k<<j<<d<<endl;
            patchders->setAt(k,j, Vector4::ORIGIN) ;
        }
    }
    for(j=__vdegree;j>=d;++j){
        for(k=0;k<=d-j;++k){
            //cout<<"set patchders"<<k<<j<<endl;
            patchders->setAt(k,j, Vector4::ORIGIN) ;
        }
    }
    for(int k=0;k<=du;++k){
        for(int s=0;s<=__vdegree;++s){
            for(int r=0;r<=__udegree;++r){
 	        temp[s] +=  UderF->getAt(k,r)*__ctrlPointMatrix->getAt(uspan-__udegree+r,vspan-__vdegree+s) ;
            }
        }
        int dd = ( (d-k) < dv ? (d-k) : dv); //min(d-k,dv) ;
        for(int r=0;r<=dd;++r){
            patchders->setAt(k,r, Vector4::ORIGIN) ;
            for(int s=0;s<=__vdegree;++s){
 	        patchders->getAt(k,r) += VderF->getAt(r,s)*temp[s] ;	//
            }
        }
    }
    return patchders;
}

Point4MatrixPtr NurbsPatch::deriveAt(real_t  u, real_t  v, int d, int uspan, int vspan ) const{
    //cout<<"NurbsPatch.deriveAt"<<endl;
    Point4MatrixPtr patchders(new Point4Matrix(d+1,d+1));
    Point4MatrixPtr dersW = deriveAtH(u,v,d,uspan,vspan) ;

    Vector4 vec;
    int k,i ;

    RealArray2 Bin(d+1,d+1);
    int n,l ;
    // Setup the first line
    Bin.setAt(0,0,1.0) ;
    for( l = d ; l > 0 ; --l )
        Bin.setAt( 0 , l , 0.0 ) ;
    // Setup the other lines
    for( n = 0 ; n < d ; n++ ){
        Bin.setAt( n+1 , 0 , 1.0 );
        for( l = 1 ; l < d+1 ; l++ )
            if( n+1 < l )
                Bin.setAt( n , l , 0.0 ) ;
            else
                Bin.setAt( n+1 , l , Bin.getAt( n , l ) + Bin.getAt( n , l-1 ) ) ;
    }
    for( k = 0 ; k <= d ; k++ ){
        for( l = 0 ; l <= d-k ; l++){
            vec.x() = dersW->getAt(k,l).x() ;
            vec.y() = dersW->getAt(k,l).y() ;
            vec.z() = dersW->getAt(k,l).z() ;
            for(i=1 ; i<=l ;i++)
                vec -= patchders->getAt(k,l-i)*Bin.getAt(l,i)*dersW->getAt(0,i).w() ;
            for (int j = 1 ; j <= k ; j++){
                vec += patchders->getAt(k-j,l)*Bin.getAt(k,j)*dersW->getAt(j,0).w() ;
                Vector4 v2 = Vector4(0,0,0,0) ;
                for ( i = 1 ; i <= l ; i++ )
                    v2 += patchders->getAt(k-j,l-i)*Bin.getAt(l,i)*dersW->getAt(j,i).w() ;
                vec -= Bin.getAt(k,j)*v2 ;
            }
            patchders->getAt(k,l) = vec/dersW->getAt(0.0,0.0).w();
        }
    }
    return patchders;
}


Vector4 NurbsPatch::getDerivativeAt(real_t u, real_t v, int du, int dv) const {
    //cout<<"nurbsPatch.getDerivativeAt"<<endl;
    int d = max(du,dv) ;
    if (d > min(__udegree,__vdegree)) return Vector4(0,0,0,0);
    int uspan = findSpan(u,__udegree,__uKnotList) ;
    int vspan = findSpan(v,__vdegree,__vKnotList) ;
    Point4MatrixPtr ders = deriveAt(u,v,d,uspan,vspan) ;
    return ders->getAt(du,dv) ;
}


Point4MatrixPtr NurbsPatch::getDerivativesAt(real_t u,real_t v) const {
    int uspan = findSpan(u,__udegree,__uKnotList) ;
    int vspan = findSpan(v,__vdegree,__vKnotList) ;
    int degree = min(__udegree,__vdegree) ;
    return deriveAt(u,v,degree,uspan,vspan) ;
}



Vector3 NurbsPatch::getPointAt(real_t u, real_t v) const{
  GEOM_ASSERT( u >= getFirstUKnot() && u <= getLastUKnot() && v>= getFirstVKnot() && v<= getLastVKnot());

  uint_t uspan = findSpan(u,__udegree,__uKnotList);
  RealArrayPtr Nu = basisFunctions(uspan, u, __udegree, __uKnotList);
  uint_t vspan = findSpan(v,__vdegree,__vKnotList);
  RealArrayPtr Nv = basisFunctions(vspan, v, __vdegree, __vKnotList);
  Vector4 Sw( 0 , 0 , 0 ,0 );

  uint_t uind = uspan - __udegree;
  for (uint_t l = 0 ; l <= __vdegree ; l++ ){
	  Vector4 temp( 0 , 0 , 0 ,0 );
      uint_t vind = vspan - __vdegree +l;
      for (uint_t k = 0 ; k <= __udegree ; k++ ) {
           /*
             Note that ctrlPointMatrix access is inverted:
             Row of ctrlpoint represent variation in v
             Thus, in getAt(row,col), constant row should be used to make v vary.
             Indices are similar between ctrlPointMatrix.getAt and
             NurbsPatch.getPointAt which is  coherent.
           */
          temp += (__ctrlPointMatrix->getAt(uind+k,vind) *  (Nu->getAt(k))) ;
      }
      Sw += temp * Nv->getAt(l);
  }



  if (fabs(Sw.w()) < GEOM_TOLERANCE)
    return Vector3(Sw.x(),Sw.y(),Sw.z());

  return Sw.project();
}

Point4MatrixPtr NurbsPatch::getMetric(real_t u, real_t v) const{
    GEOM_ASSERT( u >= 0.0 && u <= 1.0 && v>= 0.0 && v<=1.0);
    Point4MatrixPtr _metric(new Point4Matrix(2,2));
    Vector3 _utangent = getUTangentAt(u,v);
    Vector3 _vtangent = getVTangentAt(u,v);
    for (uint_t l = 0 ; l <= 1 ; l++ ){
      for (uint_t k = 0 ; k <= 1 ; k++ ){
        // CPL: This line do not compile
        //_metric->setAt(l,k,_utangent(l) * _vtangent(v))
        #warning implement the function with a valid code.
      }
    }
    return _metric;
}

real_t NurbsPatch::getDetMetric(real_t u, real_t v) const{
    GEOM_ASSERT( u >= 0.0 && u <= 1.0 && v>= 0.0 && v<=1.0);
    Vector3 _utangent = getUTangentAt(u,v);
    Vector3 _vtangent = getVTangentAt(u,v);
    real_t _det = _utangent[0] * _vtangent[1] - _utangent[1] * _vtangent[0];
    return _det;
}

Point4MatrixPtr NurbsPatch::getInvMetric(real_t u, real_t v) const{
    GEOM_ASSERT( u >= 0.0 && u <= 1.0 && v>= 0.0 && v<=1.0);
    Point4MatrixPtr _invmetric(new Point4Matrix(2,2));
    Vector3 _utangent = getUTangentAt(u,v);
    Vector3 _vtangent = getVTangentAt(u,v);
    for (uint_t l = 0 ; l <= 1 ; l++ ){
      for (uint_t k = 0 ; k <= 1 ; k++ ){
        //_invmetric->setAt(l,k,_utangent(l)*_vtangent(v)/(_utangent[0]*_vtangent[1] - _utangent[1]*_vtangent[0]))
        #warning implement the function with a valid code.
      }
    }
    return _invmetric;
}

// This function is not finished yet...
/*
Point4MatrixPtr NurbsPatch::getConnectCoeffs(real_t  u, real_t  v, int d, int uspan, int vspan ) const{
    //cout<<"NurbsPatch.deriveAt"<<endl;
    Point4MatrixPtr _drvecoord = nurbsPatch::getDerivativesAt(u,v) ;
    Point4MatrixPtr _christoffel(new Point4Matrix(d+1,d+1));
    Vector3 _chvec = Vector3::ORIGIN;
    _coord_contra = coord_contra();
    for (uint_t l = 0 ; l <= 1 ; l++ ){
      for (uint_t k = 0 ; k <= 1 ; k++ ){
        _chvec = _drve_coord->getAt(l,k);
        for (uint_t j = 0 ; j <= 1 ; j++ ){
          //_connect_coeffs[(l,k,j)] = _coord_contra[j] * 
                pass
    return _connect_coeffs
*/



Vector3 NurbsPatch::getUTangentAt(real_t u, real_t v) const {
    //cout<<"NurbsPatch.getUtangent"<<endl;
    GEOM_ASSERT( u >= 0.0 && u <= 1.0 && v>= 0.0 && v<=1.0);
    Vector4 _derivate = getDerivativeAt( u, v, 1, 0);
    //cout<<"return"<<endl;
    if(!_derivate.w())
        return Vector3(_derivate.x(),_derivate.y(),_derivate.z());
    else return _derivate.project();
}

Vector3 NurbsPatch::getVTangentAt(real_t u, real_t v) const {
    GEOM_ASSERT( u >= 0.0 && u <= 1.0 && v>= 0.0 && v<=1.0);
    Vector4 _derivate = getDerivativeAt( u, v, 0, 1);
    if(!_derivate.w())
        return Vector3(_derivate.x(),_derivate.y(),_derivate.z());
    else return _derivate.project();
}

Vector3 NurbsPatch::getNormalAt(real_t u, real_t v) const{
    GEOM_ASSERT( u >= 0.0 && u <= 1.0 && v>= 0.0 && v<=1.0);
    Vector3 _utangent = getUTangentAt(u,v);
    _utangent.normalize();
    Vector3 _vtangent = getVTangentAt(u,v);
    _vtangent.normalize();
    return cross(_utangent,_vtangent);
}




/* ----------------------------------------------------------------------- */

LineicModelPtr NurbsPatch::getIsoUSectionAt(real_t u) const
{
  GEOM_ASSERT( u >= 0.0 && u <= 1.0 );

  uint_t uspan = findSpan(u,__udegree,__uKnotList);
  RealArrayPtr Nu = basisFunctions(uspan, u, __udegree, __uKnotList);
  uint_t vdim = __ctrlPointMatrix->getColumnNb();
  Point4ArrayPtr temp(new Point4Array(vdim));
  for (uint_t l = 0 ; l < vdim ; l++ ){
      Vector4 vec;
      for (uint_t k = 0 ; k <= __udegree ; k++ )
          vec += (__ctrlPointMatrix->getAt(uspan - __udegree +k,l) *  (Nu->getAt(k))) ;
      temp->setAt(l,vec);
  }
  return LineicModelPtr(new NurbsCurve(temp,__vKnotList,__vdegree,__vstride));
}

LineicModelPtr NurbsPatch::getIsoVSectionAt(real_t v) const
{
  GEOM_ASSERT(  v>= 0.0 && v<=1.0 );

  uint_t vspan = findSpan(v,__vdegree,__vKnotList);
  RealArrayPtr Nv = basisFunctions(vspan, v, __vdegree, __vKnotList);
  uint_t udim = __ctrlPointMatrix->getRowNb();
  Point4ArrayPtr temp(new Point4Array(udim));
  for (uint_t l = 0 ; l < udim ; l++ ){
	  Vector4 vec;
      for (uint_t k = 0 ; k <= __vdegree ; k++ )
          vec += (__ctrlPointMatrix->getAt(l,vspan - __vdegree +k) *  (Nv->getAt(k))) ;
	  temp->setAt(l,vec);
  }
  return LineicModelPtr(new NurbsCurve(temp,__uKnotList,__udegree,__ustride));
}

/* ----------------------------------------------------------------------- */

bool NurbsPatch::isValid( ) const {
    Builder _builder;
  _builder.CtrlPointMatrix = const_cast<Point4MatrixPtr *>(&__ctrlPointMatrix);
  _builder.UStride = const_cast<uint_t *>(&__ustride);
  _builder.VStride = const_cast<uint_t *>(&__vstride);
  _builder.UDegree = const_cast<uint_t *>(&__udegree);
  _builder.VDegree = const_cast<uint_t *>(&__vdegree);
  _builder.UKnotList = const_cast<RealArrayPtr *>(&__uKnotList);
  _builder.VKnotList= const_cast<RealArrayPtr *>(&__vKnotList);
  _builder.CCW= const_cast<bool *>(&__ccw);
  return _builder.isValid();
}


/* ----------------------------------------------------------------------- */




