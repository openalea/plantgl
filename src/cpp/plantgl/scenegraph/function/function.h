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

class QuantisedFunction;
typedef RCPtr<QuantisedFunction> QuantisedFunctionPtr;


/**
    \class QuantisedFunction
    \brief A 2D quantised injective function (homomorphism) defined on [firstx,lastx].
*/

class SG_API QuantisedFunction : public RefCountObject {
public:
    static uint_t DEFAULT_SAMPLING;
    static bool DEFAULT_CLAMPED;

    QuantisedFunction(const Curve2DPtr& curve, uint_t sampling = DEFAULT_SAMPLING, bool clamped = DEFAULT_CLAMPED);
    QuantisedFunction(const Point2ArrayPtr& points, uint_t sampling = DEFAULT_SAMPLING, bool clamped = DEFAULT_CLAMPED);
    QuantisedFunction(const std::vector<real_t>& values, real_t firstx = 0.0,real_t lastx = 1.0, bool clamped = DEFAULT_CLAMPED);
    ~QuantisedFunction();

    /// Return the y-value for x.
    real_t getValue(real_t x) const ;
    inline real_t operator()(real_t x) const { return getValue(x); }

    /// Return the first x value corresponding to y starting at startingX. found is set to false if it cannot be found.
    inline real_t findX(real_t y, bool& found) const { return findX(y,found,__firstx); }
    real_t findX(real_t y, bool& found, real_t startingX) const ;

    inline bool build(const Curve2DPtr& curve) { return build(curve,__sampling); }
    bool build(const Curve2DPtr& curve, uint_t sampling);

    bool isMonotonous(bool strictly = false) const ;
    bool isIncreasing(bool strictly = false) const ;
    bool isDecreasing(bool strictly = false) const ;
    inline bool isValid() const { return __values.size() != 0 && __values.size() == __sampling; }

    /** Give the inverse function f-1
        For this, the function f should be isomorphic, else return 0.
     */
    QuantisedFunctionPtr inverse() const;

    /// Return sampling
    inline uint_t getSampling() { return __sampling; }

    inline real_t getFirstX() const { return __firstx; }
    inline real_t getLastX() const { return __lastx; }

    static real_t computeValue(const Curve2DPtr& curve, real_t x, real_t maxerror = GEOM_EPSILON);

    inline const bool getClamped() const { return __clamped; }
    inline bool& getClamped() { return __clamped; }
    inline bool isClampedToDefault() { return __clamped == DEFAULT_CLAMPED; }

    inline const std::vector<real_t>& getSamples() const { return  __values; }

    static bool check(const Curve2DPtr& curve);
protected:
    static real_t _computeValue(const Curve2DPtr& curve, real_t x, real_t maxerror = GEOM_EPSILON);

    real_t getIndex(real_t x) const;
    real_t getX(real_t i) const;

    void computeCache(const Curve2DPtr& curve);

    std::vector<real_t> __values;
    uint_t __sampling;
    real_t __firstx,__lastx;
    bool __clamped;
};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

#endif
