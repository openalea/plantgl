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

#include "function.h"
#include <plantgl/math/util_math.h>
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/geometry/polyline.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */
uint32_t Function::DEFAULT_SAMPLING(100);
bool Function::DEFAULT_CLAMPED(true);

Function::Function(const Curve2DPtr& curve, uint32_t sampling, bool clamped) : 
    RefCountObject(), __values(0), __sampling(sampling),__firstx(0.0),__lastx(1.0),__clamped(clamped)
{ build(curve);}

Function::Function(const Point2ArrayPtr& points, uint32_t sampling, bool clamped):
    RefCountObject(), __values(0), __sampling(sampling),__firstx(0.0),__lastx(1.0),__clamped(clamped)
{ build(Curve2DPtr(new Polyline2D(points)));}


Function::Function(const std::vector<real_t>& values, real_t firstx,real_t lastx, bool clamped):
    RefCountObject(), __values(values), __sampling(values.size()), __firstx(firstx),__lastx(lastx),__clamped(clamped)
{ }

Function::~Function() 
{ }

real_t Function::getIndex(real_t x) const
{ return (__sampling-1) * (x - __firstx) / (__lastx - __firstx); }

real_t Function::getX(real_t i) const
{ return  __firstx + i * (__lastx - __firstx) / (__sampling-1)  ; }

real_t Function::getValue(real_t x) const
{
  assert(isValid());
  if (!__clamped)
    if (x < __firstx) x = __firstx;
    else if (x > __lastx) x = __lastx;
  assert(x>=__firstx);
  if (!(x<=__lastx)){
      std::cerr << x << ' ' << __lastx << std::endl;
      assert(x<=__lastx);
  }


  real_t index = getIndex(x);
  int floorindex = int(floor(index));
  real_t diff = index-floorindex;
  real_t w1 = 1-diff;
  assert(floorindex >= 0);
  assert(floorindex < __sampling);
  if ((floorindex == __sampling-1) || (fabs(diff) < GEOM_EPSILON))
        return __values[floorindex];
  else
        return w1*__values[floorindex] + diff*__values[floorindex+1];
}

real_t Function::findX(real_t y, bool& found, real_t startingX) const 
{
  assert(isValid());
  assert(__firstx <= startingX && startingX < __lastx);
  uint32_t startingI = getIndex(startingX) + 1;
  if (startingI >= __sampling) return -1;
  for(uint32_t i = startingI; i < __sampling; ++i)
  {
      if ((__values[i-1] <= y &&  y < __values[i])||
          (__values[i-1] >= y &&  y > __values[i]))
      {
          real_t d  = __values[i] - __values[i-1];
          real_t dy = y - __values[i-1];
          real_t w1 = dy/d;
          found = true;
          return getX(i - 1 + w1);
      }
  }
  found = false;
  return -1;
}

bool Function::build(const Curve2DPtr& curve, uint32_t sampling)
{
    if (!check(curve)) return false;
    __sampling = sampling;
    computeCache(curve);
    return true;
}

bool Function::isMonotonous(bool strictly) const 
{
  return isIncreasing(strictly) || isDecreasing(strictly);
}

bool Function::isIncreasing(bool strictly) const
{
  for(uint32_t i = 1; i < __sampling; ++i)
      if ((__values[i-1] > __values[i])||
          (strictly && (__values[i-1] == __values[i])))
          return false;
  return true;
}

bool Function::isDecreasing(bool strictly) const
{
  for(uint32_t i = 1; i < __sampling; ++i)
      if ((__values[i-1] < __values[i])||
          (strictly && (__values[i-1] == __values[i])))
          return false;
  return true;
}

bool Function::check(const Curve2DPtr& curve)
{
  real_t _start = curve->getFirstKnot();
  uint32_t _size = curve->getStride();
  real_t _step =  (curve->getLastKnot()-_start) / (real_t) _size;
  Vector2 p0 = curve->getPointAt(_start);
  _start += _step;

  for (uint32_t _i = 1; _i < _size; _i++) {
    Vector2 p1 = curve->getPointAt(_start);
    if (p1.x() <= p0.x()) return false;
    _start += _step;
  };
  return true;
}

void Function::computeCache(const Curve2DPtr& curve)
{
  assert( __sampling > 2 );
  __firstx = curve->getPointAt(curve->getFirstKnot()).x();
  __lastx = curve->getPointAt(curve->getLastKnot()).x();
  real_t extent = __lastx - __firstx;
  __values.resize(__sampling);
  for (uint32_t i=0; i<__sampling; ++i)
        __values[i] = _computeValue(curve, extent * real_t(i)/real_t(__sampling-1) + __firstx);
}

real_t Function::computeValue(const Curve2DPtr& curve, real_t x, real_t maxerror)
{
    assert(check(curve));
    return _computeValue(curve,x,maxerror);
}

real_t Function::_computeValue(const Curve2DPtr& curve, real_t x, real_t maxerror)
{
   Vector2 firstpoint = curve->getPointAt(curve->getFirstKnot());
   if (x == firstpoint.x()) return firstpoint.y();
   Vector2 lastpoint = curve->getPointAt(curve->getLastKnot());
   if (x == lastpoint.x()) return lastpoint.y();
   real_t low = curve->getFirstKnot();
   real_t high = curve->getLastKnot();
   real_t check = 0.5f;
   Vector2 tst;
   do
   {
       check = (low+high)/2.0f;
       tst = curve->getPointAt(check);
       if (tst.x()> x) high = check;
       else low = check;
   } while (fabs(tst.x()-x) > maxerror);
   return tst.y();
}

FunctionPtr Function::inverse() const
{
    if(!isMonotonous(true)) return FunctionPtr(0);
    std::vector<real_t> values(__sampling,0);
    real_t firsty = __values[0];
    real_t lasty = __values[__sampling-1];
    if (firsty > lasty) std::swap(firsty,lasty);
    real_t x = __firstx;
    real_t y = firsty ;
    real_t extent = lasty - firsty;
    real_t deltay = extent / (__sampling-1);
    bool found;
    for (uint32_t i = 0; i < __sampling; ++i){
        x = findX(y,found,x);
        assert(found == true);
        values[i] = x;
        y += deltay;
    }
    return FunctionPtr(new Function(values,firsty,lasty));
}

/* ----------------------------------------------------------------------- */
