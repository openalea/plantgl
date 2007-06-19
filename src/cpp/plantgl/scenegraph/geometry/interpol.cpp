//#define DEBUG
/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Pradal
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


#include "interpol.h"
#include "nurbscurve.h"
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/math/util_polymath.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */

/////////////////////////////////////////////////////////////////////////////

Interpol::Interpol() :
  points(0),
  knots(0),
  degree(1),
  closed(false),
  CP(0),
  params(0),
  T(0)
{}

/////////////////////////////////////////////////////////////////////////////

Interpol::Interpol( const Point3ArrayPtr& _points,
                    const RealArrayPtr& _params,
                    uint32_t _degree,
                    bool _closed ) :
  points(_points),
  knots(_params),
  degree(_degree),
  closed(_closed),
  CP(0),
  params(0),
  T(0)
{}

Interpol::~Interpol()
    { }


const Point3ArrayPtr& Interpol::getPoints( ) const
    { return points; }

Point3ArrayPtr& Interpol::getPoints( )
    { return points; }

const RealArrayPtr& Interpol::getParameters( ) const
    { return knots; }

RealArrayPtr& Interpol::getParameters( )
    { return knots; }

uint32_t Interpol::getDegree( ) const
    { return degree; }

uint32_t& Interpol::getDegree( )
    { return degree; }

bool Interpol::getClosure( ) const
    { return closed; }

bool& Interpol::getClosure( )
    { return closed; }

/////////////////////////////////////////////////////////////////////////////

NurbsCurve2DPtr Interpol::get2DCurve()
{
  NurbsCurve2D* _nurbs= 0;

  bool _ok= run();
  if( !_ok )
    return NurbsCurve2DPtr(_nurbs);

  RealArrayPtr _KV= bezierKV( params );

  Point3Array::iterator _itB= CP->getBegin();
  Point3Array::iterator _itE= CP->getEnd();
  Point3Array::iterator _it= _itB;

  for( _it= _itB; _it < _itE; _it++ )
    _it->z()= 1.;

  _nurbs= new NurbsCurve2D( CP, _KV, degree );

  return NurbsCurve2DPtr(_nurbs);
}

/////////////////////////////////////////////////////////////////////////////

NurbsCurvePtr Interpol::get3DCurve()
{
  NurbsCurve* _nurbs= 0;

  bool _ok= run();
  if( !_ok )
    return NurbsCurvePtr(_nurbs);

  RealArrayPtr _KV= bezierKV( params );

  Point4ArrayPtr _CtrlPts( new Point4Array( *CP, 1. ) );
  _nurbs= new NurbsCurve( _CtrlPts, _KV, degree );

  return NurbsCurvePtr(_nurbs);
}

/////////////////////////////////////////////////////////////////////////////

/// Tangent computation at interpolated points
/// Nurbs Book p 382

bool Interpol::initTangent()
{
  GEOM_ASSERT(points);
  GEOM_ASSERT(knots);

  uint32_t size= points->getSize();
  GEOM_ASSERT( size == knots->getSize() );
  uint32_t n= size-1;

  T= Point3ArrayPtr(new Point3Array(size));

  real_t du= 0., dv= 0., alpha= 0.;
  Vector3 dpu, dpv;
  uint32_t i= 1;
  for( i= 1; i < n; i++ )
    {
    Vector3 P= points->getAt(i-1);
    Vector3 Q= points->getAt(i);
    Vector3 R= points->getAt(i+1);

    du= knots->getAt(i) - knots->getAt(i-1);
    dv= knots->getAt(i+1) - knots->getAt(i);

    GEOM_ASSERT( du > 0. );
    GEOM_ASSERT( dv > 0. );

    dpu= Q - P;
    dpv= R - Q;

    alpha= du / ( du + dv );

    dpu/= du;
    dpv/= dv;

    T->getAt(i)= ( dpu * (1-alpha) ) + ( dpv * alpha );
    }

  // compute initial condition
  du= knots->getAt(n) - knots->getAt(n-1);
  dv= knots->getAt(1) - knots->getAt(0);

  GEOM_ASSERT( du > 0. );
  GEOM_ASSERT( dv > 0. );

  dpu= points->getAt(n) - points->getAt(n-1);
  dpv= points->getAt(1) - points->getAt(0);
  dpu/= du;
  dpv/= dv;

  if( closed )
    {
    alpha= du / ( du + dv );
    Vector3 Tg= ( dpu * (1-alpha) ) + ( dpv * alpha );

    T->getAt(n)= Tg;
    T->getAt(0)= Tg;
    }
  else
    {
    T->getAt(n)= ( dpu * 2 )- T->getAt(n-1);
    T->getAt(0)= ( dpv * 2 ) - T->getAt(1);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// compute one or two arcs ( degree 2) between points
// cf. Nurbs Book p 388

uint32_t Interpol::bezierArc2( uint32_t _arcNum )
{
#ifdef DEBUG
uint32_t methode= 0;
#endif

  GEOM_ASSERT(degree==2);
  GEOM_ASSERT(points);
  GEOM_ASSERT(T);

  uint32_t nbArcs= 1;

  Vector3  P= points->getAt(_arcNum), Q= points->getAt(_arcNum+1);
  Vector3 T1= T->getAt(_arcNum),     T2= T->getAt(_arcNum+1);

  real_t alpha= 0.;
  real_t beta= 0.;

//  real_t detT1T2= T1.x()*T2.y() - T2.x()*T1.y();
  CP->pushBack(P);

  Vector3 PQ= Q-P;
  real_t eps2= GEOM_EPSILON*GEOM_EPSILON;
  real_t n2PQ= normSquared(PQ);

  if( n2PQ < eps2 )
    {
    // P=Q
    Vector3 R= (P + Q)/2.;
    CP->pushBack(R);
    nbArcs= 1;
    }
  else
    {
    bool ok= intersectLine( P,T1,Q,T2,alpha,beta);

    if( ok )
      {
      if( ( alpha > 0 ) && ( beta < 0 ) )
        {
  #ifdef DEBUG
  methode= 1;
  #endif
        Vector3 R= P + ( T1 * alpha );
        CP->pushBack(R);
        nbArcs= 1;
        }
      else
        {
  #ifdef DEBUG
  methode= 2;
  #endif
        // on doit creer un arc supplementaire

        real_t nPQ= norm(PQ);
        real_t nT1= norm(T1);
        real_t nT2= norm(T2);

        real_t cos1= dot( PQ, T1 );
        real_t cos2= dot( PQ, T2 );

        GEOM_ASSERT( nT1 > 0 );
        GEOM_ASSERT( nT2 > 0 );

        cos1/= nPQ * nT1;
        cos2/= nPQ * nT2;

        if( cos1 < 0 ) cos1= - cos1;
        if( cos2 < 0 ) cos2= - cos2;
        if( cos1 > 1. ) cos1= 1.;
        if( cos2 > 1. ) cos2= 1.;

        real_t a= 2./3.;

        real_t D1= a * cos2 + (1. - a) * cos1;
        real_t D2= a * cos1 + (1. - a) * cos2;

        GEOM_ASSERT( D1 > 0. );
        GEOM_ASSERT( D2 > 0. );

        alpha= nPQ / (4 * D1);
        beta=  nPQ / (4 * D2);

        Vector3 R1= P + ( T1 * alpha / nT1 );
        Vector3 R2= Q - ( T2 * beta / nT2 );
        Vector3 P1= ( R1 * beta + R2 * alpha ) / ( alpha +  beta );

        CP->pushBack(R1);
        CP->pushBack(P1);
        CP->pushBack(R2);

        nbArcs= 2;
        }
      }
    else
      {
      // T1 and T2 are colinear or T1 or T2 << 1
      real_t nT1= norm(T1);
      real_t nT2= norm(T2);
      real_t nPQ= norm(PQ);
      Vector3 T= ( nT1 > nT2 ) ? T1 : T2;
      real_t nT= max(nT1,nT2);
      Vector3 PQT= cross( PQ,T);

      if( (nT > GEOM_EPSILON) && (norm(PQT)/(nT*nPQ) < GEOM_EPSILON) )
        {
  #ifdef DEBUG
  methode= 3;
  #endif
        Vector3 R= ( P + Q ) / 2.;
        CP->pushBack(R);
        nbArcs= 1;
        }
      else
        {
  #ifdef DEBUG
  methode= 4;
  #endif
        Vector3 R1= P;
        Vector3 R2= Q;

        alpha= nPQ / 2.;

        if( nT1 >= GEOM_EPSILON )
          R1= P + (T1 * (alpha / nT1));
        if( nT2 >= GEOM_EPSILON )
          R2= Q - (T2 * (alpha / nT2));

        Vector3 P1= ( R1 + R2 ) / 2.;

        CP->pushBack(R1);
        CP->pushBack(P1);
        CP->pushBack(R2);
  #ifdef DEBUG
  cout<<"Arc "<<_arcNum<<": "<<P<<","<<R1<<","<<P1<<","<<R2<<","<<Q<<endl;
  #endif
        nbArcs= 2;
        }
      }
    }
#ifdef DEBUG
cout<<"Arc "<<_arcNum<<" methode="<< methode<<endl;
cout<<"P: "<<P<<" Q: "<<Q<<endl;
#endif

  return nbArcs;
}

/////////////////////////////////////////////////////////////////////////////

// compute one arc ( degree 3) between points
// cf. Nurbs Book p 395

uint32_t Interpol::bezierArc3( uint32_t _arcNum )
{
#ifdef DEBUG
//cout<<" bezierArc 3"<<endl;
#endif

  GEOM_ASSERT(degree==3);
  GEOM_ASSERT(points);
  GEOM_ASSERT(T);

  Vector3 P0= points->getAt(_arcNum);
  Vector3 P3= points->getAt(_arcNum+1);
  Vector3 P1, P2;

  Vector3 P0P3= P3-P0;
  real_t eps2= GEOM_EPSILON*GEOM_EPSILON;
  real_t n2P0P3= normSquared(P0P3);

  if( n2P0P3 < eps2 )
    {
    P1= P0 + P0P3 / 3.;
    P2= P3 - P0P3 / 3.;
    }
  else
    {
    Vector3 T0= T->getAt(_arcNum);
    Vector3 T3= T->getAt(_arcNum+1);
    Vector3 T= T0+T3;

    real_t nT0= norm(T0), nT3= norm(T3);
    real_t nT= max(nT0 , nT3);
    if( nT < GEOM_EPSILON )
      {
      P1= P0; P2= P3;
      }
    else
      {
      if( nT0 < GEOM_EPSILON )
        T= T3/nT3;
      else
      if( nT3 < GEOM_EPSILON )
        T= T0/nT0;
      else
        T= T0/nT0 + T3/nT3;

      real_t alpha= 0.;
      real_t beta= 0.;

      real_t a= 16 - normSquared(T);
      real_t b= 12 * dot(P0P3,T) ;
      real_t c= -36 * normSquared(P0P3);

      bool ok= solve( a, b, c, beta, alpha );
      if( !ok )
        return 0;

      alpha/= 3.;
      beta= (nT3 < GEOM_EPSILON) ? 0. : alpha / nT3;
      alpha=(nT0 < GEOM_EPSILON) ? 0. : alpha / nT0;

      P1= P0 + ( T0 * alpha );
      P2= P3 - ( T3 * beta );
      }
    }
#ifdef DEBUG
cout<<"Arc "<<_arcNum<<": "<<P0<<","<<P1<<","<<P2<<endl;
#endif

  CP->pushBack(P0);
  CP->pushBack(P1);
  CP->pushBack(P2);

  return 1;
}

/////////////////////////////////////////////////////////////////////////////

RealArrayPtr Interpol::bezierKV( const RealArrayPtr& _knots ) const
{
#ifdef DEBUG
cout<<" bezierKV "<<endl;
#endif

  GEOM_ASSERT(_knots);

  uint32_t size= _knots->getSize();
  uint32_t nCP= degree * (size-1) + 1;
  uint32_t nKnots= degree + 1 + nCP;

#ifdef DEBUG
cout<<" size "<<size<<endl;
cout<<" nCP "<<nCP<<endl;
cout<<" nKnots "<<nKnots<<endl;
#endif

  // Knot Vector Computation
  RealArrayPtr KV(new RealArray(nKnots));

  RealArray::const_iterator _itU= _knots->getBegin();
  RealArray::iterator _itKV= KV->getBegin();

  real_t u= *_itU;
  *(_itKV++)= u;

  uint32_t i= 1, j= 0;
  for( i= 1; _itU != _knots->getEnd(); _itU++ )
    {
    u= *_itU;
    for( j= 0; j < degree; j++ )
      *(_itKV++)= u;
    i+= degree;
    }

  *(_itKV++)= u;
  i++;

  GEOM_ASSERT( i == nKnots);

  return KV;
}

/////////////////////////////////////////////////////////////////////////////

uint32_t Interpol::checkContinuity( ) const
{
  if( degree < 2 )
    return 0;

  Vector3 Tprev, Tnext;
  uint32_t i= 0;
  uint32_t size= CP->getSize();

  if( (size-1) % degree )
    return 0;

  uint32_t nArcs= (size-1) / degree;

  for( i= 0; i < nArcs-1; i++ )
    {
    uint32_t index= (i+1) * degree;

    Vector3 P0= CP->getAt(index-1);
    Vector3 P1= CP->getAt(index);
    Vector3 P2= CP->getAt(index+1);
    Tprev= P1-P0;
    Tnext= P2-P1;

    real_t nTp= norm(Tprev);
    real_t nTn= norm(Tnext);

    real_t angle= norm(cross(Tprev,Tnext));
    if( angle > 0.01 * nTp * nTn )
      {
#ifdef DEBUG
cout<<" num Arc KO: "<<i<<endl;
cout<<" Points: "<<P0<<P1<<P2<<endl;
#endif
      return 0;
      }
    }

  if( closed )
    {
    Tprev= CP->getAt(size-1) - CP->getAt(size-2);
    Tnext= CP->getAt(1) - CP->getAt(0);
    real_t nTp= norm(Tprev);
    real_t nTn= norm(Tnext);

    real_t angle= norm(cross(Tprev,Tnext));
    if( angle > 0.01 * nTp * nTn )
      return 0;
    }
  return 1;
}

/////////////////////////////////////////////////////////////////////////////

bool Interpol::intersectLine( Vector3 P, Vector3 T1,
                              Vector3 Q, Vector3 T2,
                              real_t& a, real_t& b  )
{
  Vector3 N= cross(T1,T2);
  Vector3 PQ= Q-P;

  real_t n2= normSquared(N);
  real_t eps2= GEOM_EPSILON * GEOM_EPSILON;
  if( n2 < eps2 )
    return false;

  a= dot( cross( PQ, T2 ), N ) / n2;
  b= dot( cross( PQ, T1 ), N ) / n2;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

bool Interpol::run()
{
  GEOM_ASSERT(points);
  GEOM_ASSERT(knots);

  const uint32_t size= points->getSize();

  if( size != knots->getSize() )
    return false;

  if( size < 2 )
    return false;

  const uint32_t max_degree= 3;

  if( degree < 1)
    degree= 1;
  if( degree > max_degree )
    degree= max_degree;
  if( size < degree + 1 )
    degree= size - 1;

  GEOM_ASSERT( degree > 0 );
  GEOM_ASSERT( degree <= max_degree );
  GEOM_ASSERT( degree <= size - 1 );

  if( degree == 1 )
    {
    CP= points;
    params= knots;
    }
  else
    {
    bool ok= initTangent();

    if( !ok )
      return false;

    CP= Point3ArrayPtr( new Point3Array() );
    if( degree == 2 )
      params= RealArrayPtr( new RealArray() );

    uint32_t n= size-1;
    uint32_t i= 0;
    if( degree == 2 )
      {
      for( i= 0; i < n; i++ )
        {
        uint32_t nbArcs= bezierArc2(i);

        real_t u= knots->getAt(i);
        real_t v= knots->getAt(i+1);

        params->pushBack(u);
        if( nbArcs > 1 )
          {
          GEOM_ASSERT( nbArcs == 2 );
          real_t w= (u + v) / 2.;
          params->pushBack(w);
          }
        }
      }
    else
      {
      for( i= 0; i < n; i++ )
        {
        bool ok= bezierArc3(i);
        if(!ok)
          {
#ifdef DEBUG
cout<<"Pas de solution!!!!!!!!!! "<<endl;
#endif
          return false;
          }
        }
      }

    Vector3 P= points->getAt(n);
    CP->pushBack(P);

    if( degree == 2 )
      {
      real_t u= knots->getAt(n);
      params->pushBack(u);
      }
    else
      params= knots;
    }

#ifdef DEBUG
  uint32_t c= checkContinuity();
  if( c < min( 1, degree-1 ) )
    cout<<"Continuite KO"<<endl;
#endif

  return true;
}
