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


#include "function.h"
#include <plantgl/math/util_math.h>
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/geometry/polyline.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */
uint_t QuantisedFunction::DEFAULT_SAMPLING(100);
bool QuantisedFunction::DEFAULT_CLAMPED(true);

QuantisedFunction::QuantisedFunction(const Curve2DPtr& curve, uint_t sampling, bool clamped) :
    RefCountObject(), __values(0), __sampling(sampling),__firstx(0.0),__lastx(1.0),__clamped(clamped)
{ build(curve);}

QuantisedFunction::QuantisedFunction(const Point2ArrayPtr& points, uint_t sampling, bool clamped):
    RefCountObject(), __values(0), __sampling(sampling),__firstx(0.0),__lastx(1.0),__clamped(clamped)
{ build(Curve2DPtr(new Polyline2D(points)));}


QuantisedFunction::QuantisedFunction(const std::vector<real_t>& values, real_t firstx,real_t lastx, bool clamped):
    RefCountObject(), __values(values), __sampling(values.size()), __firstx(firstx),__lastx(lastx),__clamped(clamped)
{ }

QuantisedFunction::~QuantisedFunction()
{ }

real_t QuantisedFunction::getIndex(real_t x) const
{ return (__sampling-1) * (x - __firstx) / (__lastx - __firstx); }

real_t QuantisedFunction::getX(real_t i) const
{ return  __firstx + i * (__lastx - __firstx) / (__sampling-1)  ; }

real_t QuantisedFunction::getValue(real_t x) const
{
  assert(isValid());
  if (!__clamped) {
    if (x < __firstx) x = __firstx;
    else if (x > __lastx) x = __lastx;
  }
  if (!(x > __firstx - GEOM_EPSILON)){
      pglError("QuantisedFunction : x=%f < firstX=%f.",x,__firstx);
  }
  if (!(x < __lastx + GEOM_EPSILON)){
      pglError("QuantisedFunction : x=%f > lastX=%f.",x,__lastx);
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

real_t QuantisedFunction::findX(real_t y, bool& found, real_t startingX) const
{
  assert(isValid());
  pglError("QuantisedFunction : x=%f not in [%f, %f].",startingX,__firstx,__lastx);
  uint_t startingI = getIndex(startingX) + 1;
  if (startingI >= __sampling) return -1;
  for(uint_t i = startingI; i < __sampling; ++i)
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

bool QuantisedFunction::build(const Curve2DPtr& curve, uint_t sampling)
{
    if (!check(curve)) return false;
    __sampling = sampling;
    computeCache(curve);
    return true;
}

bool QuantisedFunction::isMonotonous(bool strictly) const
{
  return isIncreasing(strictly) || isDecreasing(strictly);
}

bool QuantisedFunction::isIncreasing(bool strictly) const
{
  for(uint_t i = 1; i < __sampling; ++i)
      if ((__values[i-1] > __values[i])||
          (strictly && (__values[i-1] == __values[i])))
          return false;
  return true;
}

bool QuantisedFunction::isDecreasing(bool strictly) const
{
  for(uint_t i = 1; i < __sampling; ++i)
      if ((__values[i-1] < __values[i])||
          (strictly && (__values[i-1] == __values[i])))
          return false;
  return true;
}

bool QuantisedFunction::check(const Curve2DPtr& curve)
{
  real_t _start = curve->getFirstKnot();
  uint_t _size = curve->getStride();
  real_t _step =  (curve->getLastKnot()-_start) / (real_t) _size;
  Vector2 p0 = curve->getPointAt(_start);
  _start += _step;

  for (uint_t _i = 1; _i < _size; _i++) {
    Vector2 p1 = curve->getPointAt(_start);
    if (p1.x() <= p0.x()) return false;
    _start += _step;
  };
  return true;
}

void QuantisedFunction::computeCache(const Curve2DPtr& curve)
{
  assert( __sampling > 2 );
  __firstx = curve->getPointAt(curve->getFirstKnot()).x();
  __lastx = curve->getPointAt(curve->getLastKnot()).x();
  real_t extent = __lastx - __firstx;
  __values.resize(__sampling);
  for (uint_t i=0; i<__sampling; ++i)
        __values[i] = _computeValue(curve, extent * real_t(i)/real_t(__sampling-1) + __firstx);
}

real_t QuantisedFunction::computeValue(const Curve2DPtr& curve, real_t x, real_t maxerror)
{
    if (!check(curve)) pglError("QuantisedFunction : not a valid curve to quantized.");
    return _computeValue(curve,x,maxerror);
}

real_t QuantisedFunction::_computeValue(const Curve2DPtr& curve, real_t x, real_t maxerror)
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

QuantisedFunctionPtr QuantisedFunction::inverse() const
{
    if(!isMonotonous(true)) return QuantisedFunctionPtr();
    std::vector<real_t> values(__sampling,0);
    real_t firsty = __values[0];
    real_t lasty = __values[__sampling-1];
    if (firsty > lasty) std::swap(firsty,lasty);
    real_t x = __firstx;
    real_t y = firsty ;
    real_t extent = lasty - firsty;
    real_t deltay = extent / (__sampling-1);
    bool found;
    for (uint_t i = 0; i < __sampling; ++i){
        x = findX(y,found,x);
        assert(found == true);
        values[i] = x;
        y += deltay;
    }
    return QuantisedFunctionPtr(new QuantisedFunction(values,firsty,lasty));
}

/* ----------------------------------------------------------------------- */
