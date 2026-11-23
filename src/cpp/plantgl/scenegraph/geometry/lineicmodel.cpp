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



#include "lineicmodel.h"
#include "../function/function.h"

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

const uchar_t LineicModel::DEFAULT_WIDTH = 1;

/* ----------------------------------------------------------------------- */

LineicModel::LineicModel(uchar_t width) :
  Primitive(),__width(width) {
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
LineicModel::getLength(real_t begin, real_t end) const
{
  real_t fk = getFirstKnot();
  real_t lk = getLastKnot();

  if (begin < getFirstKnot()) begin = fk;
  if (end > getLastKnot()) end = lk;

  real_t deltau = (lk - fk)/getStride();
  // We use the same u sequence to compute the length
  // For this, we compute the closer smaller u value from begin and end
  real_t beginI = int((begin-fk)/deltau) * deltau + fk;
  real_t endI = int((end-fk)/deltau) * deltau + fk;

  Vector3 p1 = getPointAt(beginI);
  Vector3 p2;

  real_t length = 0;

  // Eventually we do some adjustement according to the real begin and end values
  // here and just after the loop
  if (begin-beginI > GEOM_EPSILON){
    p2 = getPointAt(begin);
    length -= norm(p2 - p1);
  }

  for(real_t u = beginI + fk + deltau ; u <= endI ; u += deltau){
    p2 = getPointAt(u);
    length += norm(p2 - p1);
    p1 = p2;
  }

  if (end-endI > GEOM_EPSILON){
    p2 = getPointAt(end);
    length += norm(p2 - p1);
  }

  return length;
}

/* ----------------------------------------------------------------------- */

QuantisedFunctionPtr LineicModel::getArcLengthToUMapping() const
{
  real_t totlength = getLength();

  real_t fk = getFirstKnot();
  real_t lk = getLastKnot();

  if (totlength == 0)
      return QuantisedFunctionPtr(new QuantisedFunction(Point2ArrayPtr(new Point2Array(2,Vector2(0, fk),Vector2(1.0,lk))),10));


  uint_t stride = getStride();
  real_t deltau = (lk - fk)/stride;

  Vector3 p1 = getPointAt(fk);
  Vector3 p2;

  real_t length = 0;
  real_t n = 0;

  Point2ArrayPtr points(new Point2Array(stride+1));
  points->setAt(0,Vector2(0,fk));
  real_t u = fk + deltau;
  uint_t j = 1;
  for(uint_t i = 1 ; i <= stride; ++i, u += deltau){
    p2 = getPointAt(u);
    n = norm(p2 - p1);
    if (n > 0){
        length += n;
        p1 = p2;
        points->setAt(j,Vector2(length/totlength,u));
    }
    else {
        points->setAt(j,Vector2((length/totlength)+GEOM_EPSILON,u));
    }
    ++j;
  }
  points->setAt(j-1,Vector2(1.0,lk));
  if (j != stride+1){
      points = Point2ArrayPtr(new Point2Array(points->begin(),points->begin()+j));
  }
  return QuantisedFunctionPtr(new QuantisedFunction(points,std::max(QuantisedFunction::DEFAULT_SAMPLING,5*stride)));
}

QuantisedFunctionPtr LineicModel::getUToArcLengthMapping() const
{
  real_t totlength = getLength();

  real_t fk = getFirstKnot();
  real_t lk = getLastKnot();

  if (totlength < GEOM_EPSILON)
      return QuantisedFunctionPtr(new QuantisedFunction(Point2ArrayPtr(new Point2Array(2,Vector2(fk, 0),Vector2(lk, 0.0))),2));

  uint_t stride = getStride();
  real_t deltau = (lk - fk)/stride;

  Vector3 p1 = getPointAt(fk);
  Vector3 p2;

  real_t length = 0;
  real_t n = 0;

  Point2ArrayPtr points(new Point2Array(stride+1));
  points->setAt(0,Vector2(fk,0));
  real_t u = fk + deltau;
  for(uint_t i = 1 ; i <= stride; ++i, u += deltau){
    p2 = getPointAt(u);
    n = norm(p2 - p1);
    length += n;
    p1 = p2;
    points->setAt(i,Vector2(u,length/totlength));
  }
  points->setAt(stride,Vector2(lk,1.0));
  return QuantisedFunctionPtr(new QuantisedFunction(points,std::max(QuantisedFunction::DEFAULT_SAMPLING,5*stride)));
}

/* ----------------------------------------------------------------------- */

inline real_t
closestPointToSegment_sq(const Vector3& p,
                         const Vector3& A,
                         const Vector3& B,
                         real_t*        u       = nullptr,
                         Vector3*       closest = nullptr)
{
    const Vector3 AB   = B - A;
    const Vector3 AP   = p - A;
    const real_t  dotABAB = dot(AB, AB);

    real_t t;
    if (dotABAB < real_t(1e-12)) {
        // Segment dégénéré (A ≈ B) → distance au point A
        t = real_t(0);
    } else {
        t = std::clamp(dot(AB, AP) / dotABAB, real_t(0), real_t(1));
    }

    const Vector3 proj = A + AB * t;
    const Vector3 diff = p - proj;

    if (closest) *closest = proj;
    if (u)       *u       = t;

    return dot(diff, diff);
}

/// Retourne la distance (non élevée au carré) entre `p` et le segment [A, B].
real_t
PGL(closestPointToSegment)(const Vector3& p,
                      const Vector3& A,
                      const Vector3& B,
                      real_t*        u,
                      Vector3*       closest )
{
    return std::sqrt(closestPointToSegment_sq(p, A, B, u, closest));
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
    Vector3 pres;
    real_t d = closestPointToSegment_sq(pt, p1, p2, &lu, &pres);
    if(d < dist){
      dist = d;
      res = pres;
      if (ui != NULL) *ui = u + deltau * (lu -1);
    }
    p1 = p2;
  }
  return res;
}

/* ----------------------------------------------------------------------- */

// ─────────────────────────────────────────────────────────────────────────────
// Segment → Segment
// ─────────────────────────────────────────────────────────────────────────────
//
// Algorithme de Dan Sunday (2001) — "Distance between 3D Lines and Segments"
// http://geomalgorithms.com/a07-_distance.html
//
// Gère tous les cas dégénérés :
//   - Les deux segments sont des points
//   - Un seul segment est un point
//   - Segments parallèles (ou quasi-parallèles)
//   - Cas général (segments gauches dans R³)
//
// Paramètres :
//   A0, A1   extrémités du segment A
//   B0, B1   extrémités du segment B
//   closestA (optionnel) point le plus proche sur A
//   closestB (optionnel) point le plus proche sur B
//   uA, uB   (optionnel) paramètres normalisés ∈ [0,1]
//
// Retourne dist²(segment A, segment B).

inline real_t
closestSegmentToSegment_sq(const Vector3& A0,
                           const Vector3& A1,
                           const Vector3& B0,
                           const Vector3& B1,
                           real_t*        uA       = nullptr,
                           real_t*        uB       = nullptr,
                           Vector3*       closestA = nullptr,
                           Vector3*       closestB = nullptr)
{
    constexpr real_t EPS = real_t(1e-10);

    const Vector3 dA = A1 - A0;   // direction segment A
    const Vector3 dB = B1 - B0;   // direction segment B
    const Vector3 r  = A0 - B0;

    const real_t a = dot(dA, dA);  // ||dA||²
    const real_t e = dot(dB, dB);  // ||dB||²
    const real_t f = dot(dB, r);

    real_t tA, tB;

    if (a < EPS && e < EPS) {
        // ── Les deux segments sont des points ─────────────────────────────
        tA = tB = real_t(0);
    }
    else if (a < EPS) {
        // ── Segment A est un point ────────────────────────────────────────
        tA = real_t(0);
        tB = std::clamp(f / e, real_t(0), real_t(1));
    }
    else {
        const real_t c = dot(dA, r);

        if (e < EPS) {
            // ── Segment B est un point ────────────────────────────────────
            tB = real_t(0);
            tA = std::clamp(-c / a, real_t(0), real_t(1));
        }
        else {
            // ── Cas général ───────────────────────────────────────────────
            // Résolution du système linéaire :
            //   (a  -b) (tA)   (−c)
            //   (b  −e) (tB) = (−f)
            // où b = dot(dA, dB)
            const real_t b     = dot(dA, dB);
            const real_t denom = a * e - b * b;   // ||dA×dB||²

            if (denom > EPS) {
                // Segments non parallèles : solution unique sur les droites
                tA = std::clamp((b * f - c * e) / denom, real_t(0), real_t(1));
            } else {
                // Segments parallèles : tA arbitraire (on prend 0),
                // tB sera recalculé ci-dessous pour coller au segment B
                tA = real_t(0);
            }

            // tB calculé depuis tA, puis reclamped + correction de tA
            tB = (b * tA + f) / e;

            if (tB < real_t(0)) {
                tB = real_t(0);
                tA = std::clamp(-c / a, real_t(0), real_t(1));
            } else if (tB > real_t(1)) {
                tB = real_t(1);
                tA = std::clamp((b - c) / a, real_t(0), real_t(1));
            }
        }
    }

    const Vector3 pA   = A0 + dA * tA;
    const Vector3 pB   = B0 + dB * tB;
    const Vector3 diff = pA - pB;

    if (closestA) *closestA = pA;
    if (closestB) *closestB = pB;
    if (uA)       *uA       = tA;
    if (uB)       *uB       = tB;

    return dot(diff, diff);
}

/// Retourne la distance (non élevée au carré) entre les segments [A0,A1] et [B0,B1].
real_t
PGL(closestSegmentToSegment)(const Vector3& A0,
                        const Vector3& A1,
                        const Vector3& B0,
                        const Vector3& B1,
                        real_t*        uA       ,
                        real_t*        uB       ,
                        Vector3*       closestA ,
                        Vector3*       closestB )
{
    return std::sqrt(closestSegmentToSegment_sq(A0, A1, B0, B1, uA, uB,
                                                closestA, closestB));
}
