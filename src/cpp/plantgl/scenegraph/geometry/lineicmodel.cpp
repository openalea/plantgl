/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr)
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


#include "lineicmodel.h"
PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

LineicModel::LineicModel() :
  Primitive(){
}

LineicModel::~LineicModel(){
}

bool LineicModel::isACurve( ) const {
  return true;
}

bool LineicModel::isASurface( ) const {
  return false;
} 

bool LineicModel::isAVolume( ) const {
  return false;
} 

/* ----------------------------------------------------------------------- */

real_t
LineicModel::getLength(){
  real_t length(0);
  real_t u0 = getFirstKnot();
  real_t u1 = getLastKnot();
  real_t deltau = (u1 - u0)/getStride();
  Vector3 p1 = getPointAt(u0);
  Vector3 p2;
  for(real_t u = u0 + deltau ; u <= u1 ; u += deltau){
    p2 = getPointAt(u);
    length += norm(p2 - p1);
    p1 = p2;
  }
  return length;
}

Vector3 
LineicModel::findClosest(const Vector3& p, real_t* ui) const{
  real_t u0 = getFirstKnot();
  real_t u1 = getLastKnot();
  real_t deltau = (u1 - u0)/getStride();
  Vector3 p1 = getPointAt(u0);
  Vector3 res = p1;
  real_t dist = normSquared(p-res);
  Vector3 p2, pt;
  real_t lu;
  for(real_t u = u0 + deltau ; u <= u1 ; u += deltau){
    p2 = getPointAt(u);
	pt = p;
	real_t d = closestPointToSegment(pt,p1,p2,&lu);
	if(d < dist){
	  dist = d;
	  res = pt;
      if (ui != NULL) *ui = u + deltau * (lu -1);
	}
    p1 = p2;
  }
  return res;
}

real_t 
PGL(closestPointToSegment)(Vector3& p, 
					       const Vector3& segA,
					       const Vector3& segB,
                           real_t* u)
{
  Vector3 diff = p - segA;
  Vector3 M = segB - segA;
  real_t t = dot(M,diff);
  if(t > 0){
	real_t dotMM = dot(M,M);
	if(t < dotMM){
	  t = t / dotMM;
	  diff -= M*t;
	  p = segA + M*t;
	}
	else {
	  t = 1;
	  diff -= M;
	  p = segB;
	}
  }
  else {
	t = 0;
	p = segA;
  }
  if (u != NULL) *u = t;
  return dot(diff,diff);
}

/* ----------------------------------------------------------------------- */
