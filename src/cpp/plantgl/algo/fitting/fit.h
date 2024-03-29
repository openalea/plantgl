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


/*! \file actn_fit.h
    \brief Some algorithms for fitting Geometric Model on points.
*/

/* ----------------------------------------------------------------------- */

#ifndef __actn_fit_h__
#define __actn_fit_h__

/* ----------------------------------------------------------------------- */

#include "../algo_config.h"
#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/geometry/plane.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


/**
   \class Fit
   \brief An action which fit a pointSet with a \e Geometry objects.
*/

class ALGO_API Fit
{

  public:

    /// Constructs an Action.
    Fit();

    /// Constructs an Action with \e _pointstofit.
    Fit(const Point3ArrayPtr& _pointstofit );

    /// Destructor.
    virtual ~Fit( ) {
    }

    /// Set the points to fit to \e self.
    virtual bool setPoints(const Point3ArrayPtr& _pointstofit ){
      __pointstofit=_pointstofit;
      return true;
    }

    /// get the points to fit.
    virtual const Point3ArrayPtr  getPoints(){
      return __pointstofit;
    }


    /// Set the radius to fit to \e self.
    virtual bool setRadius(const Point2ArrayPtr& _radius ){
      __radius=_radius;
      return true;
    }

    /// get the radius to fit.
    virtual const Point2ArrayPtr getRadius(){
      return __radius;
    }

    /// Set the Optimization to fit to \e self.
    virtual bool setOptimization(const unsigned char& _opt ){
      __optimization =_opt;
      return true;
    }

    /// get the Optimization to fit.
    virtual const uchar_t getOptimization(){
      return __optimization;
    }

    /// Fit the points using classname.
    virtual GeometryPtr use(const std::string& classname);

 /* ----------------------------------------------------------------------- */

    /// Fit the points with a sphere using a Gaussian distribution.
    virtual GeometryPtr sphere();

    /// Fit the points with a sphere. radius = average(dist(center,Pi)).
    virtual GeometryPtr asphere();

    /// Fit the points with a bounding sphere.radius = max(dist(center,Pi)).
    virtual GeometryPtr bsphere();

    /*!
      Fit the points with a cylinder using a Gaussian distribution.
    */
    virtual GeometryPtr cylinder();

    /*!
      Fit the points with an average cylinder.
    */
    virtual GeometryPtr acylinder();

    /*!
      Fit the points with a bounding cylinder.
    */
    virtual GeometryPtr bcylinder();

    /*!
      Fit the points with an ellipsoid.
    */
    virtual GeometryPtr ellipsoid();

    /*!
      Fit the points with an average ellipsoid.
    */
    virtual GeometryPtr aellipsoid();

    /*!
      Fit the points with a bounding ellipsoid.
    */
    virtual GeometryPtr bellipsoid();

    /*!
      Fit the points with an ellipsoid.
    */
    virtual GeometryPtr ellipsoid2();


    /*!
      Fit the points with an average aligned box.
    */
    virtual GeometryPtr aalignedbox();

    /*!
      Fit the points with a bounding aligned box.
    */
    virtual GeometryPtr balignedbox();

    /*!
      Fit the points with an box.
    */
    virtual GeometryPtr box();

    /*!
      Fit the points with an average box.
    */
    virtual GeometryPtr abox();

    /*!
      Fit the points with a bounding box.
    */
    virtual GeometryPtr bbox();

    /*!
      Fit 2 3D points with a frustum.
    */
    virtual GeometryPtr frustum();

/* ----------------------------------------------------------------------- */
    /*!
      Fit the points with an extruded hull.
      Compute a vertical and an horizontal profiles.
      */
    virtual GeometryPtr extrudedHull();

    /*!
      Fit the points with an asymmetricHull.
    */
    virtual GeometryPtr asymmetricHull();

    /*!
      Fit the points with a convex hull.
      Use the Quick Hull Algorithm.
      See The Quick Hull Algorithm for Convex Hulls
      from C. B. Barber, D. P. Dobkin, H. Huhdanpaa.
      */
    virtual GeometryPtr convexHull();

    /*!
      Fit the 2D points \e points with a convex Polyline.
    */
    static Point2ArrayPtr convexPolyline(const Point2ArrayPtr& _points);


    virtual GeometryPtr alphashape(real_t alpha = 0);

    /*!
      Fit the 2D points \e points with a bounding circle.
    */
    static bool boundingCircle(const Point2ArrayPtr& _points,
Vector2& center, real_t& radius);

    /*
      Fit the 3D points \e points with a bounding circle.
    */
    static bool plane(const Point3ArrayPtr& _points,
Vector3& center, Plane3& plane, const Index& subset = Index(0));

/* ----------------------------------------------------------------------- */
    /*!
      Fit the points with an extrusion.
    */
    virtual GeometryPtr extrusion();

    /*!
      Fit the points with a nurbs curve.
    */
    virtual LineicModelPtr nurbsCurve();

    /*!
      Fit the points with a nurbs curve with optimization 1.
    */
    static LineicModelPtr nurbsCurve(const Point3ArrayPtr & Q, int degC, int n);


    /*!
      \brief A least squares curve approximation

      \latexonly
      This routine solves the following problem: find the NURBS curve
      $C$ satisfying
      \begin{itemize}
      \item $Q_0 = C(0)$ and $Q_m = C(1)$
      \item the remaining $Q_k$ are approximated in the least squares
      sense, {\em i.e.}
      \[ \sum_{k=1}^{m-1} | Q_k-C(\bar{u}_k)|^2 \]
      in a minimum with respect to the $n$ variable $P_i$; the
      $\bar{u}$ are the parameter values computed with the
      chord length method.
      \end{itemize}

      The resulting curve will generally not pass through $Q_k$ and
      $C(\bar{u}_k)$ is not the closest point on $C(u)$ to $Q_k$.
      \endlatexonly
      \htmlonly
      This routines generates a curve that approrimates the points in the
      least square sense, you can find more details in the LaTeX version.
      \endhtmlonly

      For more details, see section 9.4.1 on page 491 of the NURBS
      book.

      \param Q  the vector of 3D points
      \param degC  the degree of the curve
      \param n  the number of control points in the new curve.

      \warning \a deg must be smaller than Q.n().
      \author Philippe Lavoie
      \date 24 January, 1997
    */
    static LineicModelPtr leastSquares(const Point3ArrayPtr & Q, int degC, int n);


    /*!
      \brief chord length parameterization

      Performs chord length parameterization from a vector of points.
      \latexonly
      The chord length parameterization works as follows:
      \begin{itemize}
      \item The total chord length is defined as
      \begin{equation} d = \sum_{k=1}^{n-1} | Q_k-Q_{k-1}|
      \end{equation}
      \item $\bar{u}_0 = 0$ and $\bar{u}_{n-1} = 1$
      \item \begin{equation}
      \bar{u}_k = \bar{u}_{k-1}+\frac{| Q_k-Q_{k-1}| }{d}
      \hspace{0.5in} k=1,\ldots,n-2
      \end{equation}
      \end{itemize}
      Where $n$ is the size of the vector $Q$.
      \endlatexonly
      \htmlonly
      There are more details about this function in the LaTeX version.
      \endhtmlonly

      \param  Q  a vector of 3D points
      \return the result of chord length parameterization

      \author    Philippe Lavoie
      \date 24 January, 1997
    */
    static RealArrayPtr chordLengthParam(const Point3ArrayPtr &Q,real_t& totalLength);

    /*!
      \brief  A least squares curve approximation

      \latexonly
      This routine solves the following problem: find the NURBS curve
      $C$ satisfying
      \begin{itemize}
      \item $Q_0 = C(0)$ and $Q_m = C(1)$
      \item the remaining $Q_k$ are approximated in the least squares
      sense, {\em i.e.}
      \[ \sum_{k=1}^{m-1} | Q_k-C(\bar{u}_k)|^2 \]
      in a minimum with respect to the $n$ variable $P_i$; the
      $\bar{u}$ are the precomputed parameter values.
      \end{itemize}

      The resulting curve will generally not pass through $Q_k$ and
      $C(\bar{u}_k)$ is not the closest point on $C(u)$ to $Q_k$.
      \endlatexonly
      \htmlonly
      This routines generates a curve that approrimates the points in the
      least square sense, you can find more details in the LaTeX version.
      \endhtmlonly

      For more details, see section 9.4.1 on page 491 of the NURBS
      book.

      \param Q  the vector of 3D points
      \param degC  the degree of the curve
      \param n  the number of control points in the new curve
      \param ub  the knot coefficients

      \warning the variable curve \b must contain a valid knot vector.
      \author Philippe Lavoie
      \date 24 January 1997
    */
    static LineicModelPtr leastSquares(const Point3ArrayPtr & Q,
                   int degC, int n, const RealArrayPtr& ub);

    /*!
      \brief  A least squares curve approximation

      \latexonly
      This routine solves the following problem: find the NURBS curve
      $C$ satisfying
      \begin{itemize}
      \item $Q_0 = C(0)$ and $Q_m = C(1)$
      \item the remaining $Q_k$ are approximated in the least squares
      sense, {\em i.e.}
      \[ \sum_{k=1}^{m-1} | Q_k-C(\bar{u}_k)|^2 \]
      in a minimum with respect to the $n$ variable $P_i$; the
      $\bar{u}$ are the precomputed parameter values.
      \end{itemize}

      The resulting curve will generally not pass through $Q_k$ and
      $C(\bar{u}_k)$ is not the closest point on $C(u)$ to $Q_k$.

      \endlatexonly
      \htmlonly
      This routines generates a curve that approrimates the points in the
      least square sense, you can find more details in the LaTeX version.
      \endhtmlonly

      For more details, see section 9.4.1 on page 491 of the NURBS
      book.

      \param Q  the vector of 3D points
      \param degC  the degree of the curve
      \param n  the number of control points in the new curve
      \param ub  the knot coefficients
      \param knot  the knot vector to use for the curve

      \return 1 if succesfull, 0 it the number of points to approximate
      the curve with is too big compared to the number of points.
      \warning the variable curve \b must contain a valid knot vector.
      \author Philippe Lavoie
      \date 24 January 1997
    */
    static LineicModelPtr leastSquares(const Point3ArrayPtr & Q,
                   int degC, int n, const RealArrayPtr& ub,
                   const RealArrayPtr& knot);

/* ----------------------------------------------------------------------- */

    /// get the class name available.
    static  std::vector<std::string> getClassNames();

    /// get the class name available.
    static  std::vector<std::string> getVolumeClassNames();

    static bool available(const std::string&);

    /// Return whether the radius are needed
    static bool areRadiusNeeded(std::string classname);

    /// compute inertia axis
    static bool inertiaAxis(const Point3ArrayPtr& points,
                            Vector3& u, Vector3& v,
                            Vector3& w, Vector3& s);

    static bool inertiaAxis(const Point2ArrayPtr& points,
                            Vector2& u, Vector2& v, Vector2& s);

    /*!
        Find \e e value for equation x^e / r^e + y^e / h^e +1 = 0.
        Use a second order polynomium approximation of this equation :
        - a = x / r, b = y / h, e = - ln(a.b) / ( ln a + ln b ).
    */
    static double fitShapeFactor(double x, double r, double y, double h);

protected:

  /// The points to fit.
  Point3ArrayPtr __pointstofit;

  /// The radius of the points.
  Point2ArrayPtr __radius;

  /// The default cross section used for the extrusion.
  Curve2DPtr __default_crossSection;

  /// Level of optimisation
  uchar_t __optimization;
};



/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

// __actn_fit_h__
#endif
