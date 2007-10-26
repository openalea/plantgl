/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon
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
/*! \file function.h
    \brief 2D discrete injective function (monomorphism).
*/

/* ----------------------------------------------------------------------- */


#ifndef __function_h__
#define __function_h__

#include "../geometry/curve.h"
#include "../container/pointarray.h"
#include <vector>

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Function;
typedef RCPtr<Function> FunctionPtr; 


/**
    \class Function
    \brief AD quantised injective function (monomorphism) defined on [firstx,lastx].
*/

class SG_API Function : public TOOLS(RefCountObject) {
public:
    static uint32_t DEFAULT_SAMPLING;
    static bool DEFAULT_CLAMPED;

    Function(const Curve2DPtr& curve, uint32_t sampling = DEFAULT_SAMPLING, bool clamped = DEFAULT_CLAMPED);
    Function(const Point2ArrayPtr& points, uint32_t sampling = DEFAULT_SAMPLING, bool clamped = DEFAULT_CLAMPED);
    Function(const std::vector<real_t>& values, real_t firstx = 0.0,real_t lastx = 1.0, bool clamped = DEFAULT_CLAMPED);
    ~Function();

    /// Return the y-value for x.
    real_t getValue(real_t x) const ;
    inline real_t operator()(real_t x) const { return getValue(x); }

    /// Return the first x value corresponding to y starting at startingX. found is set to false if it cannot be found.
    inline real_t findX(real_t y, bool& found) const { return findX(y,found,__firstx); }
    real_t findX(real_t y, bool& found, real_t startingX) const ;

    inline bool build(const Curve2DPtr& curve) { return build(curve,__sampling); }
    bool build(const Curve2DPtr& curve, uint32_t sampling);

    bool isMonotonous(bool strictly = false) const ;
    bool isIncreasing(bool strictly = false) const ;
    bool isDecreasing(bool strictly = false) const ;
    inline bool isValid() const { return __values.size() != 0 && __values.size() == __sampling; }

    /** Give the inverse function f-1
        For this, the function f should be isomorphic, else return 0.
     */
    FunctionPtr inverse() const;

    /// Return sampling
    inline uint32_t getSampling() { return __sampling; }

    inline real_t getFirstX() const { return __firstx; }
    inline real_t getLastX() const { return __lastx; }

    static real_t computeValue(const Curve2DPtr& curve, real_t x, real_t maxerror = GEOM_EPSILON);

    inline const bool getClamped() const { return __clamped; }
    inline bool& getClamped() { return __clamped; }
    inline bool isClampedToDefault() { return __clamped == DEFAULT_CLAMPED; }

    inline const std::vector<real_t>& getSamples() const { return  __values; }
protected:
    static real_t _computeValue(const Curve2DPtr& curve, real_t x, real_t maxerror = GEOM_EPSILON);
    static bool check(const Curve2DPtr& curve);

    real_t getIndex(real_t x) const;
    real_t getX(real_t i) const;

    void computeCache(const Curve2DPtr& curve);

    std::vector<real_t> __values;
    uint32_t __sampling;
    real_t __firstx,__lastx;
    bool __clamped;
};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

#endif