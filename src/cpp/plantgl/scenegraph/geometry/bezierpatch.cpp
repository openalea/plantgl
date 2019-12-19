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


#include "bezierpatch.h"
#include "beziercurve.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/math/util_math.h>
#include <plantgl/scenegraph/container/pointmatrix.h>
#include <plantgl/scenegraph/container/pointarray.h>

PGL_USING_NAMESPACE
using namespace std;

/* ----------------------------------------------------------------------- */


const uint_t BezierPatch::DEFAULT_STRIDE(30);


/* ----------------------------------------------------------------------- */

BezierPatch::Builder::Builder( ) :
  Patch::Builder(),
         CtrlPointMatrix(0),
         UDegree(0),
         UStride(0),
         VDegree(0),
         VStride(0){
}


BezierPatch::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr BezierPatch::Builder::build( ) const {
    if (isValid()){
#ifdef GEOM_DEBUG
        if(!UDegree)cout << "UDegree value assign to " << ((*CtrlPointMatrix)->getColumnSize() - 1) << endl;
        if(!VDegree)cout << "VDegree value assign to " << ((*CtrlPointMatrix)->getRowSize() - 1) << endl;
#endif
        return SceneObjectPtr(
            new BezierPatch(*CtrlPointMatrix,(UStride ? *UStride : DEFAULT_STRIDE),
                            (VStride ? *VStride : DEFAULT_STRIDE),(CCW ? *CCW : DEFAULT_CCW)));
    }
    return SceneObjectPtr();
}


void BezierPatch::Builder::destroy( ) {
  if (CtrlPointMatrix) delete CtrlPointMatrix;
  if (UDegree) delete UDegree;
  if (VDegree) delete VDegree;
  if (UStride) delete UStride;
  if (VStride) delete VStride;
  if (CCW) delete CCW;
}


bool BezierPatch::Builder::isValid( ) const {

  // CtrlPointList field
    if ( !CtrlPointMatrix ) {
        pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"Bezier Patch","CtrlPointMatrix");
        return false;
    }
    uint_t _usize = (*CtrlPointMatrix)->getRowSize();
    uint_t _vsize = (*CtrlPointMatrix)->getColumnSize();

    if (_usize < 2 ) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Bezier Patch","CtrlPointMatrix","Size of rows must be greater than 1.");
        return false;

    }    if (_vsize < 2 ) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Bezier Patch","CtrlPointMatrix","Size of columns must be greater than 1.");
        return false;
    }

    for(uint_t i=0; i < _usize;i++)
      for(uint_t j=0; j < _vsize;j++)
        {
        if(fabs((*CtrlPointMatrix)->getAt(j,i).w()) < GEOM_TOLERANCE) {
            string _ith =  "( " + number(i + 1) + " , " + number(j+1) + ")";
            pglErrorEx
                (PGLWARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"Bezier Patch","CtrlPointMatrix",_ith.c_str(),"Weight must be not null.");
          return false;
        }
        }

    // UDegree field
    uint_t _udegree = (UDegree ? *UDegree : _usize - 1);
    if (_udegree < 1 ) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Bezier Patch","UDegree","Must be greater than 0.");
        return false;
    };

    // VDegree field
    uint_t _vdegree = (VDegree ? *VDegree : _vsize - 1);
    if (_vdegree < 1 ) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Bezier Patch","VDegree","Must be greater than 0.");
        return false;
    };

    // UStride field
    if ( UStride && *UStride < 1 ) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Bezier Patch","UStride","Must be greater than 0.");
        return false;
    };

    // VStride field
    if ( VStride && *VStride < 1 ) {
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"Bezier Patch","VStride","Must be greater than 0.");
        return false;
    };

    // Fields dependency
    if ( (_usize != (_udegree + 1)) || (_vsize !=(_vdegree +1)) ) {
        string _mess = "Must be coherent with UDegree and VDegree : wait for " + number(_udegree + 1) + "x" + number(_vdegree+1) + " Matrix";
        pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"Bezier Patch","CtrlPointMatrix",_mess.c_str());
        return false;
    };

    return true;
}


/* ----------------------------------------------------------------------- */

BezierPatch::BezierPatch( const Point4MatrixPtr& ctrlPoints,
                          uint_t ustride ,
                          uint_t vstride ,
                          bool ccw) :
    Patch(ccw),
    __ctrlPointMatrix(ctrlPoints),
    __ustride(ustride),
    __vstride(vstride){
    GEOM_ASSERT(isValid());
}

BezierPatch::BezierPatch( const Point3MatrixPtr& ctrlPoints,
                          uint_t ustride ,
                          uint_t vstride ,
                          bool ccw) :
    Patch(ccw),
    __ctrlPointMatrix(),
    __ustride(ustride),
    __vstride(vstride){
    if(ctrlPoints) __ctrlPointMatrix = Point4MatrixPtr(new Point4Matrix(*ctrlPoints,1));
    GEOM_ASSERT(isValid());
}

BezierPatch::BezierPatch() :
    Patch(),
    __ctrlPointMatrix(),
    __ustride(DEFAULT_STRIDE),
    __vstride(DEFAULT_STRIDE){
}

BezierPatch::~BezierPatch( ) {
}


/* ----------------------------------------------------------------------- */


Point4MatrixPtr
BezierPatch::getCtrlPoints( ) const {
  return __ctrlPointMatrix;
}

const Point4MatrixPtr&
BezierPatch::getCtrlPointMatrix( ) const {
  return __ctrlPointMatrix;
}

Point4MatrixPtr&
BezierPatch::getCtrlPointMatrix( ) {
  return __ctrlPointMatrix;
}

const uint_t&
BezierPatch::getUStride( ) const {
  return __ustride;
}

uint_t&
BezierPatch::getUStride( ){
  return __ustride;
}

const uint_t&
BezierPatch::getVStride( ) const {
  return __vstride;
}

uint_t&
BezierPatch::getVStride( ) {
  return __vstride;
}

bool
BezierPatch::isUStrideToDefault( ) const{
  return ( __ustride == DEFAULT_STRIDE );
}

bool
BezierPatch::isVStrideToDefault( ) const{
  return ( __vstride == DEFAULT_STRIDE );
}

const uint_t
BezierPatch::getUDegree( ) const {
  return __ctrlPointMatrix->getRowSize() - 1;
}

const uint_t
BezierPatch::getVDegree( ) const {
  return __ctrlPointMatrix->getColumnSize() - 1;
}



/* ----------------------------------------------------------------------- */





Vector3 BezierPatch::getPointAt(real_t u, real_t v) const{
    GEOM_ASSERT( u >= 0.0 && u <= 1.0 && v>= 0.0 && v<=1.0);

    uint_t _udeg = getUDegree();
    uint_t _vdeg = getVDegree();
    Vector3 myPoint( 0 , 0 , 0 );

    real_t u1 = real_t(1.0) - u;
    real_t v1 = real_t(1.0) - v;


    if( _udeg <= _vdeg ){


        vector<Vector4> Q(_vdeg+1);
        for (uint_t j =0 ; j <= _vdeg ; ++j){
          vector<Vector4> T=__ctrlPointMatrix->getRow(j);
          for (uint_t k = 1 ; k <= _udeg ; ++k)
              for (uint_t i = 0 ; i <= ( _udeg - k ) ; ++i)
                  T[i] = (T[i] * u1) + (T[i+1] * u);
          Q[j]=T[0];
        }
        for (uint_t k = 1 ; k <= _vdeg ; ++k)
            for (uint_t j = 0 ; j <= ( _vdeg - k ) ; ++j)
                Q[j] = (Q[j] * v1) + (Q[j+1] * v);

        if (fabs(Q[0].w()) < GEOM_TOLERANCE)
            return Vector3(Q[0].x(),Q[0].y(),Q[0].z());
        return Q[0].project();
    }
    else{
        vector<Vector4> Q(_udeg+1);
        for (uint_t i =0 ; i <= _udeg ; ++i){
            vector<Vector4> T=__ctrlPointMatrix->getColumn(i);
            for (uint_t k = 1 ; k <= _vdeg ; ++k)
                for (uint_t j = 0 ; j <= ( _vdeg - k ) ; ++j)
                    T[j] = (T[j] * v1) + (T[j+1] * v);
            Q[i]=T[0];
        }
        for (uint_t k = 1 ; k <= _udeg ; k++)
            for (uint_t i = 0 ; i <= ( _udeg - k ) ; ++i)
                Q[i] = (Q[i] * u1) + (Q[i+1] * u);

        if (fabs(Q[0].w()) < GEOM_TOLERANCE)
            return Vector3(Q[0].x(),Q[0].y(),Q[0].z());
        return Q[0].project();
    }
}


LineicModelPtr BezierPatch::getIsoUSectionAt(real_t u) const
{
    GEOM_ASSERT( u >= 0.0 && u <= 1.0);

    uint_t _udeg = getUDegree();
    uint_t _vdeg = getVDegree();
    Vector3 myPoint( 0 , 0 , 0 );

    real_t u1 = real_t(1.0) - u;

    vector<Vector4> Q(_vdeg+1);
    for (uint_t j =0 ; j <= _vdeg ; j ++){
          vector<Vector4> T=__ctrlPointMatrix->getRow(j);
          for (uint_t k = 1 ; k <= _udeg ; k++)
              for (uint_t i = 0 ; i <= ( _udeg - k ) ; i++)
                  T[i] = (T[i] * u1) + (T[i+1] * u);
          Q[j]=T[0];
    }
    return LineicModelPtr(new BezierCurve(Point4ArrayPtr(new Point4Array(Q.begin(),Q.end())),_vdeg));
}


LineicModelPtr BezierPatch::getIsoVSectionAt(real_t v) const
{
    GEOM_ASSERT( u >= 0.0 && u <= 1.0);

    uint_t _udeg = getUDegree();
    uint_t _vdeg = getVDegree();
    Vector3 myPoint( 0 , 0 , 0 );

    real_t v1 = real_t(1.0) - v;

    vector<Vector4> Q(_udeg+1);
    for (uint_t i =0 ; i <= _udeg ; i ++){
        vector<Vector4> T=__ctrlPointMatrix->getColumn(i);

        for (uint_t k = 1 ; k <= _vdeg ; k++)
                for (uint_t j = 0 ; j <= ( _vdeg - k ) ; j++)
                    T[j] = (T[j] * v1) + (T[j+1] * v);
        Q[i]=T[0];
    }
    return LineicModelPtr(new BezierCurve(Point4ArrayPtr(new Point4Array(Q.begin(),Q.end())),_udeg));
}


bool BezierPatch::isValid( ) const {
  Builder _builder;
  _builder.CtrlPointMatrix = const_cast<Point4MatrixPtr *>(&__ctrlPointMatrix);
  _builder.UStride = const_cast<uint_t *>(&__ustride);
  _builder.VStride = const_cast<uint_t *>(&__vstride);
  _builder.CCW= const_cast<bool *>(&__ccw);
  return _builder.isValid();
}

SceneObjectPtr BezierPatch::copy(DeepCopier& copier) const {
  BezierPatch * ptr = new BezierPatch(*this);
  copier.copy_attribute(ptr->getCtrlPointMatrix());
  return SceneObjectPtr(ptr);
}




/* ----------------------------------------------------------------------- */


