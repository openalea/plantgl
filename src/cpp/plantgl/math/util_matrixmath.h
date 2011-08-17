/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 2000-2007 UMR CIRAD/INRIA/INRA DAP 
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

/*! \file util_matrixmath.h
    \brief Definition of the container class LUMatrix and SVDMatrix for linear algebra computation.
        Most of the code of this file is a adaptation of NURBS++ source from Philippe Lavoie (see http://libnurbs.sourceforge.net/)
*/

#ifndef __util_matrixmat_h__
#define __util_matrixmat_h__

#include "math_config.h"
#include "plantgl/tool/util_array.h"
#include "plantgl/tool/util_array2.h"
#include "util_math.h"

/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/*!
  \class LUMatrix
  \brief a class for the LU decomposition of a matrix

  \latexonly
  This class was adapted from a class in the Kalman library
  written by Skip Carter (\verb|skip@taygeta.oc.nps.navy.mil|)
  available from
  \verb|ftp:://usc.edu/pub/C-numanal/kalman.tar.gz|
  \endlatexonly
  \htmlonly
  This class was adapted from a class in the
  <A HREF="ftp:://usc.edu/pub/C-numanal/kalman.tar.gz">Kalman library</A>
  written by Skip Carter
  (<A HREF="mailto:skip@taygeta.oc.nps.navy.mil">skip@taygeta.oc.nps.navy.mil</A>
  \endhtmlonly

  It performs the LU decomposition of a matrix. It can be
  used to find it's inverse and to solve linear systems.

  This class can only be used with floating point values
  (float or double).

  This code was taken from Nurbs++ librairy and adapted for geom
  by fred boudon.

  \author Skip Carter
  \author Philippe Lavoie
  \date 22 October 1997
*/
template <class T>
class LUMatrix : public NumericArray2<T> {

public:

  /// Constructor.
  LUMatrix(int r, int c) :
    NumericArray2<T>(r,c),
    pivot_(r) {
  }

  /// Copy Constructor.
  LUMatrix(const LUMatrix<T>& lu) :
    NumericArray2<T>(lu),
    pivot_(lu.pivot_) {
  }

  /// Constructor.
  LUMatrix(const NumericArray2<T>& a) :
    NumericArray2<T>(a.getRowNb(),a.getColumnNb()){
    decompose(a) ;
  }

  virtual void resize(const uint_t r, const uint_t c) {
    NumericArray2<T>::resize(r,c) ;
    pivot_.resize(r) ;
  }

  // LUMatrix& operator=(const LUMatrix<T>&);

  /*!
    \brief Generetes a LU matrix from a matrix a

    Generetes a LU matrix from a matrix a.

    \param a  the matrix to decompose
    \return a reference to itself

    \author Philippe Lavoie
    \date 22 October 1997
  */
  LUMatrix& decompose(const NumericArray2<T> &a)
  {
    int i, j, k, l, kp1, nm1, n;
    T t, q;
    double den, ten;

    n = a.getRowNb();

    GEOM_ASSERT(a.getRowNb()!=a.getColumnNb());

    //  lu = a;  must do it by copying or LUFACT will be recursively called !

    this->__rowSize = a.getRowSize();
    this->__A = std::vector<T>(a.begin(),a.end());
    pivot_.resize(n);

    /*  for(i=0;i<n;++i)
        for(j=0;j<n;++j)
        elem(i,j) = a(i,j) ;*/

    errval = 0;
    nm1 = n - 1;
    for (k = 0; k < n; k++)
      pivot_[k] = k ;

    sign = 1 ;

    if (nm1 >= 1)       /* non-trivial problem */
      {
        for (k = 0; k < nm1; k++)
          {
            kp1 = k + 1;
            // partial pivoting ROW exchanges
            // -- search over column
#ifdef COMPLEX_COMMENT
            ten = fabs(real_t(a.getAt(k,k)))+
              fabs(imag(a.getAt(k,k)));
#else
            ten = fabs(a.getAt(k,k));
#endif
            l = k;
            for (i = kp1; i < n; i++)
              {
#ifdef COMPLEX_COMMENT
                den = fabs(real_t(a.getAt(i,k)))+
                  fabs(imag(a.getAt(i,k)));
#else
                den = fabs(a.getAt(i,k));
#endif
                if ( den > ten )
                  {
                    ten = den;
                    l = i;
                  }
              }
            pivot_[k] = l;

            if ( this->getAt(l,k) != 0.0 )
              {                 // nonsingular pivot found
                if (l != k ){   // interchange needed
                  for (i = k; i < n; i++)
                    {
                      t = this->getAt(l,i) ;
                      this->setAt(l,i, this->getAt(k,i)) ;
                      this->setAt(k,i,t);
                    }
                  sign = -sign ;
                }
                q =  this->getAt(k,k);        /* scale row */
                for (i = kp1; i < n; i++)
                  {
                    t = - this->getAt(i,k)/q;
                    this->setAt(i,k,t);
                    for (j = kp1; j < n; j++)
                      this->setAt(i,j,this->getAt(i,j) + t * this->getAt(k,j));
                  }
              }
            else                /* pivot singular */
              errval = k;
          }

      }

    pivot_[nm1] = nm1;
    if (this->getAt(nm1,nm1) == 0.0)
      errval = nm1;
    return *this;
  }


  /*!
    \brief Finds the determinant of a LU matrix

    Finds the determinant of a LU matrix.

    \return The value of the determinant

    \author Philippe Lavoie
    \date 22 October 1997
  */
  T determinant(){
    T det = this->getAt(0,0) ;
    for(int i=1;i<this->rows();++i)
      det *= this->getAt(i,i) ;
    return det * (T)sign ;
  }

  /*!
    \brief Finds the inverse of the LU matrix

    Finds the inverse of the LU matrix.

    \return the inverse of the matrix

    \author Philippe Lavoie
    \date 22 October 1997
  */
  NumericArray2<T> inverse(){
    GEOM_ASSERT( getRowNb() != getColumnNb() );

    NumericArray2<T> inverse(this->getRowNb(),this->getColumnNb()) ;
    inverseIn(inverse) ;

    return inverse;
  }

  /*!
    \brief Finds the inverse of the LU matrix

    Finds the inverse of the LU matrix.

    \param inv  the inverse of the matrix

    \author Philippe Lavoie
    \date 22 October 1997
  */
  void inverseIn(NumericArray2<T>& inv){
    T ten;
    int i, j, k, l, kb, kp1, nm1, n, coln;

    GEOM_ASSERT( getRowNb() != getColumnNb() );

    n = coln = this->getRowNb();


    inv = *this ;

    nm1 = n - 1;

    // inverse U
    for (k = 0; k < n; k++)
      {
          assert(inv.getAt(k,k)!=0);
        ten = 1.0 / inv.getAt(k,k) ;
        inv.setAt(k,k,ten);
        ten = -ten;
        for (i = 0; i < k; i++)
          inv.setAt(i,k,inv.getAt(i,k) * ten);
        kp1 = k + 1;
        if (nm1 >= kp1)
          {
            for (j = kp1; j < n; j++)
              {
                ten = inv.getAt(k,j) ;
                inv.setAt(k,j,0.0) ;
                for (i = 0; i < kp1; i++)
                  inv.setAt(i,j, inv.getAt(i,j) + ten * inv.getAt(i,k)) ;
              }
          }
      }

    // INV(U) * INV(L)
    if (nm1 >= 1)
      {
        std::vector<T> work(n) ;
        //error.memory( work.memory() );

        for (kb = 0; kb < nm1; kb++)
          {
            k = nm1 - kb - 1;
            kp1 = k + 1;
            for (i = kp1; i < n; i++)
              {
                work[i] = inv.getAt(i,k) ;
                inv.setAt(i,k, 0.0);
              }
            for (j = kp1; j < n; j++)
              {
                ten = work[j];
                for (i = 0; i < n; i++)
                  inv.setAt(i,k,inv.getAt(i,k)+ten * inv.getAt(i,j)) ;
              }
            l = pivot_[k];
            if (l != k)
              for (i = 0; i < n; i++)
                {
                  ten = inv.getAt(i,k) ;
                  inv.setAt(i,k, inv.getAt(i,l));
                  inv.setAt(i,l, ten);
                }
          }
      }
  };

  /// Get the pivot.
  const std::vector<int>& getPivot() const{
    return pivot_;
  }

private:
  /// Pivot vector.
  std::vector<int> pivot_ ;

protected:
  /// contain error value.
  int errval ;

  /// contain sign.
  int sign ;
};


/*!
  \class SVDMatrix
  \brief A matrix for the SVD decomposition

  \latexonly
  This matrix was adapted from the Numerical Math Class Library
  developed by Oleg Kiselyov and available from
  \verb|http://www.lh.com/oleg/ftp/packages.html|.
  \endlatexonly
  \htmlonly
  This matrix was adapted from the
  <A HREF="http://www.lh.com/oleg/ftp/packages.html">Numerical Math
  Class Library</A>
  developed by Oleg Kiselyov.
  \endhtmlonly

  This class can only be used with floating point values
  (float or double).

  \latexonly
  The following comments are from Oleg Kiselyov.

  Singular Value Decomposition of a rectangular matrix
  $ A = U  Sig  V'$
  where matrices $U$ and $V$ are orthogonal and $Sig$ is a
    diagonal matrix.

    The singular value decomposition is performed by constructing
    an SVD  object from an $M\times N$ matrix $A$ with $M \ge N$
    (that is, at least as many rows as columns). Note, in case
    $M > N,$ matrix $Sig$ has to be a $M \times N$ diagonal
    matrix. However, it has only $N$ diagonal elements, which
    we store in a vector sig.

    {\bf Algorithm:}    Bidiagonalization with Householder
    reflections followed by a modification of a QR-algorithm.
    For more details, see G.E. Forsythe, M.A. Malcolm, C.B. Moler
    Computer methods for mathematical computations, Prentice-Hall,
    1977.  However, in the present implementation, matrices $U$
    and $V$ are computed right away rather than delayed until
    after all Householder reflections.

    This code is based for the most part on a Algol68 code I
    (Oleg Kiselyov) wrote ca. 1987.

    Look at the source code for more information about the
    algorithm.
    \endlatexonly

    \htmlonly
    <P>
    The following comments are from Oleg Kiselyov.
    </P>
    <P>
    Singular Value Decomposition of a rectangular matrix
    <code>A=U Sig V'</code>
  where matrices U and V are orthogonal and Sig is a
  diagonal matrix.
  </p>
  <P>
  The singular value decomposition is performed by constructing
  an SVD  object from an M*N matrix A with M \ge N
  (that is, at least as many rows as columns). Note, in case
  M &gt; N, matrix  Sig has to be a M*N diagonal
    matrix. However, it has only N diagonal elements, which
    we store in a vector sig.
    </p>
    <P>
    <strong>Algorithm:</strong> Bidiagonalization with Householder
    reflections followed by a modification of a QR-algorithm.
    For more details, see G.E. Forsythe, M.A. Malcolm, C.B. Moler
    Computer methods for mathematical computations, Prentice-Hall,
    1977.  However, in the present implementation, matrices  U
    and V are computed right away rather than delayed until
    after all Householder reflections.
    </p>
    <p>
    This code is based for the most part on a Algol68 code I
    (Oleg Kiselyov) wrote ca. 1987.
    </P>
    <p>
    Look at the source code for more information about the
    algorithm.
    </p>
    \endhtmlonly

    This code was taken from Nurbs++ librairy and adapted for geom
    by fred boudon.

    \author Oleg Kiselyov
    \author Philippe Lavoie
    \date 22 Oct. 1997
*/
template <class T>
class SVDMatrix {

public:

  /// Default Constructor.
  SVDMatrix():U_(0,0), V_(0,0),sig_(0) {}

  /// Constructor.
  SVDMatrix(const NumericArray2<T>& A)
    : U_(0,0), V_(0,0),sig_(0)
  {
    decompose(A) ;
  }

  /// Get the \e U Matrix.
  const NumericArray2<T>& getU() const{
    return U_;
  }

  /// Get the \e V Matrix.
  const NumericArray2<T>& getV() const{
    return V_;
  }

  /// Get \e sig .
  const std::vector<T>& getSig() const{
    return sig_;
  }

  /*!
    \brief Performs the SVD decomposition of A

    Performs the SVD decomposition of A.

    \param A  the matrix to decompose
    \return 1 if the decomposition was succesfull, 0 otherwise.

    \warning The matrix A must have a number equal or higher of rows than
    columns.

    \author Philippe Lavoie
    \date 22 October 1997
  */
  int decompose(const NumericArray2<T>& A){
    M = A.getRowNb() ;
    N = A.getColumnNb() ;

    GEOM_ASSERT( M < N );

    U_.resize(M,M) ;
    V_.resize(N,N) ;
    sig_.resize(N) ;

    U_.setDiagonal(1) ;
    V_.setDiagonal(1) ;

    std::vector<T> super_diag(N);
    const double bidiag_norm = bidiagonalize(super_diag,A);

    // Significance threshold
    const double eps = FLT_EPSILON * bidiag_norm;

    diagonalize(super_diag,eps);

    return 1 ;
  }

  /*!
    \brief Finds the minimal and maximal sigma values

    Finds the minimal and maximal sigma values.

    \param min_sig  the minimal sigma value
    \param max_sig  the maximal sigma value

    \warning The SVD matrix must be valid.

    \author Philippe Lavoie
    \date 22 October 1997
  */
  void minMax(T& min_sig, T& max_sig) const{
    max_sig = min_sig = sig_[0];
    typename std::vector<T>::const_iterator _i;
    for ( _i = sig_.begin() + 1; _i != sig_.end(); _i++ )
      if (*_i < min_sig) min_sig = *_i;
      else if (*_i > max_sig) max_sig = *_i;
  }

  /*!
    \brief Finds the condition number

    Finds the condition number which corresponds to the maximal
    sigma value divided by its minimal value.

    \return The condition number

    \warning The SVD matrix must be valid.

    \author Philippe Lavoie
    \date 22 October 1997
  */
  double q_cond_number(void) const{
    T mins,maxs ;
    minMax(mins,maxs) ;
    return (maxs/mins);
  }

  //void cut_off(const double min_sig);

  /*!
    \brief Finds the (pseudo-)inverse of a SVD matrix

    Finds the (pseudo-)inverse of a SVD matrix.

    \param inv  the inverse of the SVD matrix
    \param tau  the minimal singular value accepted

    \warning The SVD matrix must be valid.

    \author Philippe Lavoie
    \date 22 October 1997
  */
  void inverseIn(NumericArray2<T>& inv, double tau=0) {

    T min_sig, max_sig ;
    minMax(min_sig,max_sig) ;

    if(tau==0)
      tau = V_.getRowNb() * max_sig * FLT_EPSILON ;

    GEOM_ASSERT(min_sig<tau);

    NumericArray2<T> S(V_.getColumnNb(),U_.getRowNb()) ;

    for(int i=0;i<sig_.size();++i)
      S.setAt(i,i,(T)1/sig_[i]) ;

    inv = transpose(U_) ;
    inv = (const NumericArray2<T>&)S*(const NumericArray2<T>&)inv ; // transpose(U) ;
    inv = (const NumericArray2<T>&)V_*(const NumericArray2<T>&)inv ;
  }


  /*!
    \brief Finds the (pseudo-)inverse of a SVD matrix

    Finds the (pseudo-)inverse of a SVD matrix.

    \param tau  the minimal singular value accepted

    \warning The SVD matrix must be valid.

    \author Philippe Lavoie
    \date 22 October 1997
  */
  NumericArray2<T> inverse(double tau=0) {
    NumericArray2<T> A ;

    inverseIn(A,tau) ;
    return A ;
  } ;

  /*!
    \brief Solves the linear system \a A \a X = \a B

    Solves the linear system \a A \a X = \a B. Given \a A and \a B it
    finds the value of \a X. \a A is the SVD matrix properly
    initialized and the only other input parameter is \a B.

    \param B  the right hand side of the equation
    \param X  the resulting matrix
    \param tau  the minimal singular value accepted

    \return 1 if the routine was able to solve the problem, 0 otherwise.

    \warning The SVD matrix must be valid and correspond to \a A.

    \author Philippe Lavoie
    \date 22 October 1997
  */
  int solve(const NumericArray2<T>& B, NumericArray2<T>& X, double tau=0) {
    T min_sig, max_sig ;
    minMax(min_sig,max_sig) ;

    GEOM_ASSERT(B.rows() != U.rows());

    X.resize(V_.getRowNb(),B.getColumnNb()) ;

    if(tau==0)
      tau = V_.getRowNb()*max_sig*FLT_EPSILON ;
    int stable = 1 ;

    std::vector<T> S(sig_.size()) ;

    // doing one column from B at a time
    uint_t i,j,k ;
    for(j=0;j<B.getColumnNb();++j){
      for(i=0;i<V_.getColumnNb();++i){
        T s = 0 ;
        if(sig_[i]>tau){
          for(k=0;k<U_.getColumnNb();++k)
            s += U_.getAt(k,i)*B.getAt(k,j);
          s /= sig_[i] ;
        }
        else
          stable = 0 ;
        S[i] = s ;
      }
      for(i=0;i<V_.getRowNb();++i){
        X.setAt(i,j, (T) 0 ) ;
        for(k=0;k<V_.getRowNb();++k)
          X.setAt(i,j,X.getAt(i,j)+ V_.getAt(i,k)*S[k]) ;
      }
    }
    return stable ;
  }

protected:
  /// Dimensions of the problem (M > N)
  uint_t M;

  /// Dimensions of the problem (M > N)
  uint_t N;

  /// M * M orthogonal matrix \a U
  NumericArray2<T> U_;

  /// N * N orthogonal matrix \a V
  NumericArray2<T> V_;

  ///  Vector of \a N unordered singular values
  std::vector<T> sig_;

  // Internal procedures used in SVD

  /*! Left Householder Transformations
   *
   * Zero out an entire subdiagonal of the i-th column of A and compute the
   * modified A[i,i] by multiplication (UP' * A) with a matrix UP'
   *   (1)  UP' = E - UPi * UPi' / beta
   *
   * where a column-vector UPi is as follows
   *   (2)  UPi = [ (i-1) zeros, A[i,i] + Norm, vector A[i+1:M,i] ]
   * where beta = UPi' * A[,i] and Norm is the norm of a vector A[i:M,i]
   * (sub-diag part of the i-th column of A). Note we assign the Norm the
   * same sign as that of A[i,i].
   * By construction, (1) does not affect the first (i-1) rows of A. Since
   * A[*,1:i-1] is bidiagonal (the result of the i-1 previous steps of
   * the bidiag algorithm), transform (1) doesn't affect these i-1 columns
   * either as one can easily verify.
   * The i-th column of A is transformed as
   *   (3)  UP' * A[*,i] = A[*,i] - UPi
   * (since UPi'*A[*,i]/beta = 1 by construction of UPi and beta)
   * This means effectively zeroing out A[i+1:M,i] (the entire subdiagonal
   * of the i-th column of A) and replacing A[i,i] with the -Norm. Thus
   * modified A[i,i] is returned by the present function.
   * The other (i+1:N) columns of A are transformed as
   *    (4)  UP' * A[,j] = A[,j] - UPi * ( UPi' * A[,j] / beta )
   * Note, due to (2), only elements of rows i+1:M actually  participate
   * in above transforms; the upper i-1 rows of A are not affected.
   * As was mentioned earlier,
   * (5)  beta = UPi' * A[,i] = (A[i,i] + Norm)*A[i,i] + A[i+1:M,i]*A[i+1:M,i]
   *    = ||A[i:M,i]||^2 + Norm*A[i,i] = Norm^2 + Norm*A[i,i]
   * (note the sign of the Norm is the same as A[i,i])
   * For extra precision, vector UPi (and so is Norm and beta) are scaled,
   * which would not affect (4) as easy to see.
   *
   * To satisfy the definition
   *   (6)  .SIG = U' A V
   * the result of consecutive transformations (1) over matrix A is accumulated
   * in matrix U' (which is initialized to be a unit matrix). At each step,
   * U' is left-multiplied by UP' = UP (UP is symmetric by construction,
   * see (1)). That is, U is right-multiplied by UP, that is, rows of U are
   * transformed similarly to columns of A, see eq. (4). We also keep in mind
   * that multiplication by UP at the i-th step does not affect the first i-1
   * columns of U.
   * Note that the vector UPi doesn't have to be allocated explicitly: its
   * first i-1 components are zeros (which we can always imply in computations),
   * and the rest of the components (but the UPi[i]) are the same as those
   * of A[i:M,i], the subdiagonal of A[,i]. This column, A[,i] is affected only
   * trivially as explained above, that is, we don't need to carry this
   * transformation explicitly (only A[i,i] is going to be non-trivially
   * affected, that is, replaced by -Norm, but we will use sig[i] to store
   * the result).
   *
       */
  double left_householder(NumericArray2<T>& A, const uint_t i){
    uint_t j,k;
    T scale = 0;                        // Compute the scaling factor
    for(k=i; k<M; k++)
      scale += fabs(A.getAt(k,i));
    if( scale == 0 )                    // If A[,i] is a null vector, no
      return 0;                         // transform is required
    double Norm_sqr = 0;                        // Scale UPi (that is, A[,i])
    for(k=i; k<M; k++){
      A.setAt(k,i, A.getAt(k,i) / scale); // and compute its norm, Norm^2
      Norm_sqr += sq(A.getAt(k,i));
    }
    double new_Aii = sqrt(Norm_sqr);    // new_Aii = -Norm, Norm has the
    if( A.getAt(i,i) > 0 ) new_Aii = -new_Aii;  // same sign as Aii (that is, UPi[i])
    const float beta = - A.getAt(i,i)*new_Aii + Norm_sqr;
    A.setAt(i,i, A.getAt(i,i)- new_Aii);
    // UPi[i] = A[i,i] - (-Norm)

    for(j=i+1; j<N; j++)                // Transform i+1:N columns of A
      {
        T factor = 0;
        for(k=i; k<M; k++)
          factor += A.getAt(k,i) * A.getAt(k,j);
        factor /= beta;
        for(k=i; k<M; k++)
          A.setAt(k,j, A.getAt(k,j)- A.getAt(k,i) * factor);
      }

    for(j=0; j<M; j++)                  // Accumulate the transform in U
      {
        T factor = 0;
        for(k=i; k<M; k++)
          factor += A.getAt(k,i) * U_.getAt(j,k);       // Compute  U[j,] * UPi
        factor /= beta;
        for(k=i; k<M; k++)
          U_.setAt(j,k, U_.getAt(j,k) - A.getAt(k,i) * factor );
      }
    return new_Aii * scale;             // Scale new Aii back (our new Sig[i])
  }

  /*! Right Householder Transformations
       *
       * Zero out i+2:N elements of a row A[i,] of matrix A by right
       * multiplication (A * VP) with a matrix VP
       *   (1)  VP = E - VPi * VPi' / beta
       *
       * where a vector-column .VPi is as follows
       *   (2)  VPi = [ i zeros, A[i,i+1] + Norm, vector A[i,i+2:N] ]
       * where beta = A[i,] * VPi and Norm is the norm of a vector A[i,i+1:N]
       * (right-diag part of the i-th row of A). Note we assign the Norm the
       * same sign as that of A[i,i+1].
       * By construction, (1) does not affect the first i columns of A. Since
       * A[1:i-1,] is bidiagonal (the result of the previous steps of
       * the bidiag algorithm), transform (1) doesn't affect these i-1 rows
       * either as one can easily verify.
       * The i-th row of A is transformed as
       *  (3)  A[i,*] * VP = A[i,*] - VPi'
       * (since A[i,*]*VPi/beta = 1 by construction of VPi and beta)
       * This means effectively zeroing out A[i,i+2:N] (the entire right super-
       * diagonal of the i-th row of A, but ONE superdiag element) and replacing
       * A[i,i+1] with - Norm. Thus modified A[i,i+1] is returned as the result of
       * the present function.
       * The other (i+1:M) rows of A are transformed as
       *    (4)  A[j,] * VP = A[j,] - VPi' * ( A[j,] * VPi / beta )
       * Note, due to (2), only elements of columns i+1:N actually  participate
       * in above transforms; the left i columns of A are not affected.
       * As was mentioned earlier,
       * (5)  beta = A[i,] * VPi = (A[i,i+1] + Norm)*A[i,i+1]
       *                           + A[i,i+2:N]*A[i,i+2:N]
       *        = ||A[i,i+1:N]||^2 + Norm*A[i,i+1] = Norm^2 + Norm*A[i,i+1]
       * (note the sign of the Norm is the same as A[i,i+1])
       * For extra precision, vector VPi (and so is Norm and beta) are scaled,
       * which would not affect (4) as easy to see.
       *
       * The result of consecutive transformations (1) over matrix A is accumulated
       * in matrix V (which is initialized to be a unit matrix). At each step,
       * V is right-multiplied by VP. That is, rows of V are transformed similarly
       * to rows of A, see eq. (4). We also keep in mind that multiplication by
       * VP at the i-th step does not affect the first i rows of V.
       * Note that vector VPi doesn't have to be allocated explicitly: its
       * first i components are zeros (which we can always imply in computations),
       * and the rest of the components (but the VPi[i+1]) are the same as those
       * of A[i,i+1:N], the superdiagonal of A[i,]. This row, A[i,] is affected
       * only trivially as explained above, that is, we don't need to carry this
       * transformation explicitly (only A[i,i+1] is going to be non-trivially
       * affected, that is, replaced by -Norm, but we will use super_diag[i+1] to
       * store the result).
       *
       */
  double right_householder(NumericArray2<T>& A, const uint_t i){
    uint_t j,k;
    T scale = 0;                        // Compute the scaling factor
    for(k=i+1; k<N; k++)
      scale += fabs(A.getAt(i,k));
    if( scale == 0 )                    // If A[i,] is a null vector, no
      return 0;                         // transform is required

    double Norm_sqr = 0;                        // Scale VPi (that is, A[i,])
    for(k=i+1; k<N; k++){               // and compute its norm, Norm^2
      A.setAt(i,k, A.getAt(i,k) / scale);
      Norm_sqr += sq(A.getAt(i,k));
    }
    double new_Aii1 = sqrt(Norm_sqr);   // new_Aii1 = -Norm, Norm has the
    if( A.getAt(i,i+1) > 0 )                    // same sign as
      new_Aii1 = -new_Aii1;             // Aii1 (that is, VPi[i+1])
    const float beta = - A.getAt(i,i+1)*new_Aii1 + Norm_sqr;

    A.setAt(i,i+1, A.getAt(i,i+1) - new_Aii1);
    // VPi[i+1] = A[i,i+1] - (-Norm)

    for(j=i+1; j<M; j++)                // Transform i+1:M rows of A
      {
        T factor = 0;
        for(k=i+1; k<N; k++)
          factor += A.getAt(i,k) * A.getAt(j,k);        // Compute A[j,] * VPi
        factor /= beta;
        for(k=i+1; k<N; k++)
          A.setAt(j,k,A.getAt(j,k) - A.getAt(i,k) * factor);
      }

    for(j=0; j<N; j++)                  // Accumulate the transform in V
      {
        T factor = 0;
        for(k=i+1; k<N; k++)
          factor += A.getAt(i,k) * V_.getAt(j,k);
        // Compute  V[j,] * VPi
        factor /= beta;
        for(k=i+1; k<N; k++)
          V_.setAt(j,k, V_.getAt(j,k) - A.getAt(i,k) * factor);
      }
    return new_Aii1 * scale;            // Scale new Aii1 back
  }

  /*! Bidiagonalization
       * This nethod turns matrix A into a bidiagonal one. Its N diagonal elements
       * are stored in a vector sig, while N-1 superdiagonal elements are stored
       * in a vector super_diag(2:N) (with super_diag(1) being always 0).
       * Matrices U and V store the record of orthogonal Householder
       * reflections that were used to convert A to this form. The method
       * returns the norm of the resulting bidiagonal matrix, that is, the
       * maximal column sum.
       */
  double bidiagonalize(std::vector<T>& super_diag, const NumericArray2<T>& _A){
    double norm_acc = 0;
    super_diag[0] = 0;                  // No superdiag elem above A(1,1)
    NumericArray2<T> A = _A;                   // A being transformed
    for(uint_t i=0; i<N; i++)
      {
        const T& diagi = sig_[i] = left_householder(A,i);
        if( i < (uint_t)N-1 )
          super_diag[i+1] = right_householder(A,i);
        double t2 = fabs(diagi)+fabs(super_diag[i]) ;
        norm_acc = std::max(norm_acc,t2);
      }
    return norm_acc;
  }

  /*! QR-diagonalization of a bidiagonal matrix
       *
       * After bidiagonalization we get a bidiagonal matrix J:
       *    (1)  J = U' * A * V
       * The present method turns J into a matrix JJ by applying a set of
       * orthogonal transforms
       *    (2)  JJ = S' * J * T
       * Orthogonal matrices S and T are chosen so that JJ were also a
       * bidiagonal matrix, but with superdiag elements smaller than those of J.
       * We repeat (2) until non-diag elements of JJ become smaller than EPS
       * and can be disregarded.
       * Matrices S and T are constructed as
       *    (3)  S = S1 * S2 * S3 ... Sn, and similarly T
       * where Sk and Tk are matrices of simple rotations
       *    (4)  Sk[i,j] = i==j ? 1 : 0 for all i>k or i<k-1
       *         Sk[k-1,k-1] = cos(Phk),  Sk[k-1,k] = -sin(Phk),
       *         SK[k,k-1] = sin(Phk),    Sk[k,k] = cos(Phk), k=2..N
       * Matrix Tk is constructed similarly, only with angle Thk rather than Phk.
       *
       * Thus left multiplication of J by SK' can be spelled out as
       *    (5)  (Sk' * J)[i,j] = J[i,j] when i>k or i<k-1,
       *                  [k-1,j] = cos(Phk)*J[k-1,j] + sin(Phk)*J[k,j]
       *                  [k,j] =  -sin(Phk)*J[k-1,j] + cos(Phk)*J[k,j]
       * That is, k-1 and k rows of J are replaced by their linear combinations;
       * the rest of J is unaffected. Right multiplication of J by Tk similarly
       * changes only k-1 and k columns of J.
       * Matrix T2 is chosen the way that T2'J'JT2 were a QR-transform with a
       * shift. Note that multiplying J by T2 gives rise to a J[2,1] element of
       * the product J (which is below the main diagonal). Angle Ph2 is then
       * chosen so that multiplication by S2' (which combines 1 and 2 rows of J)
       * gets rid of that elemnent. But this will create a [1,3] non-zero element.
       * T3 is made to make it disappear, but this leads to [3,2], etc.
       * In the end, Sn removes a [N,N-1] element of J and matrix S'JT becomes
       * bidiagonal again. However, because of a special choice
       * of T2 (QR-algorithm), its non-diag elements are smaller than those of J.
       *
       * All this process in more detail is described in
       *    J.H. Wilkinson, C. Reinsch. Linear algebra - Springer-Verlag, 1971
       *
       * If during transforms (1), JJ[N-1,N] turns 0, then JJ[N,N] is a singular
       * number (possibly with a wrong (that is, negative) sign). This is a
       * consequence of Frantsis' Theorem, see the book above. In that case, we can
       * eliminate the N-th row and column of JJ and carry out further transforms
       * with a smaller matrix. If any other superdiag element of JJ turns 0,
       * then JJ effectively falls into two independent matrices. We will process
       * them independently (the bottom one first).
       *
       * Since matrix J is a bidiagonal, it can be stored efficiently. As a matter
       * of fact, its N diagonal elements are in array Sig, and superdiag elements
       * are stored in array super_diag.
       */
  void rotate(NumericArray2<T>& Mu, const int i, const int j,
              const double cos_ph, const double sin_ph){
    for(uint_t l=0; l<Mu.getRowNb(); l++)
      {
        T Uil = Mu.getAt(l,i);
        T Ujl = Mu.getAt(l,j);
        const T Ujl_was = Ujl;
        Ujl =  cos_ph*Ujl_was + sin_ph*Uil;
        Uil = -sin_ph*Ujl_was + cos_ph*Uil;
        Mu.setAt(l,i,Uil);
        Mu.setAt(l,j,Ujl);
      }
  }

  /*!
       * A diagonal element J[l-1,l-1] turns out 0 at the k-th step of the
       * algorithm. That means that one of the original matrix' singular numbers
       * shall be zero. In that case, we multiply J by specially selected
       * matrices S' of simple rotations to eliminate a superdiag element J[l-1,l].
       * After that, matrix J falls into two pieces, which can be dealt with
       * in a regular way (the bottom piece first).
       *
       * These special S transformations are accumulated into matrix U: since J
       * is left-multiplied by S', U would be right-multiplied by S. Transform
       * formulas for doing these rotations are similar to (5) above. See the
       * book cited above for more details.
       */
  void rip_through(std::vector<T>& super_diag, const int k,
                   const int l, const double eps){
    double cos_ph = 0, sin_ph = 1;      // Accumulate cos,sin of Ph
    // The first step of the loop below
    // (when i==l) would eliminate J[l-1,l],
    // which is stored in super_diag(l)
    // However, it gives rise to J[l-1,l+1]
    // and J[l,l+2]
    // The following steps eliminate these
    // until they fall below
    // significance
    for(int i=l; i<=k; i++)
      {
        const double f = sin_ph * super_diag[i];
        super_diag[i] *= cos_ph;
        if( fabs(f) <= eps )
          break;                        // Current J[l-1,l] became unsignificant
        cos_ph = sig_[i]; sin_ph = -f;  // unnormalized sin/cos
        const double norm = (sig_[i] = hypot(cos_ph,sin_ph)); // sqrt(sin^2+cos^2)
        cos_ph /= norm, sin_ph /= norm; // Normalize sin/cos
        rotate(U_,i,l-1,cos_ph,sin_ph);
      }
  }


  /*!
        We're about to proceed doing QR-transforms
        on a (bidiag) matrix J[1:k,1:k]. It may happen
        though that the matrix splits (or can be
        split) into two independent pieces. This function
        checks for splitting and returns the lowerbound
        index l of the bottom piece, J[l:k,l:k] */
  int get_submatrix_to_work_on(std::vector<T>& super_diag,
                               const int k, const double eps){
    for(register int l=k; l>0; l--)
      if( fabs(super_diag[l]) <= eps )
        return l;                               // The breaking point: zero J[l-1,l]
      else if( fabs(sig_[l-1]) <= eps ) // Diagonal J[l,l] turns out 0
        {                                       // meaning J[l-1,l] _can_ be made
          rip_through(super_diag,k,l,eps);      // zero after some rotations
          return l;
        }
    return 0;                   // Deal with J[1:k,1:k] as a whole
  }

  /// Diagonalize root module
  void diagonalize(std::vector<T>& super_diag, const double eps)    {
    for(register int k=N-1; k>=0; k--)  // QR-iterate upon J[l:k,l:k]
      {
        int l;
        //    while(l=get_submatrix_to_work_on(super_diag,k,eps),
        //        absolute(super_diag[k]) > eps)        // until superdiag J[k-1,k] becomes 0
        // Phil
        while(fabs(super_diag[k]) > eps)// until superdiag J[k-1,k] becomes 0
          {
            l=get_submatrix_to_work_on(super_diag,k,eps) ;
            double shift;                       // Compute a QR-shift from a bottom
            {                                   // corner minor of J[l:k,l:k] order 2
              T Jk2k1 = super_diag[k-1],        // J[k-2,k-1]
                Jk1k  = super_diag[k],
                Jk1k1 = sig_[k-1],              // J[k-1,k-1]
                Jkk   = sig_[k],
                Jll   = sig_[l];                // J[l,l]
              shift = (Jk1k1-Jkk)*(Jk1k1+Jkk) + (Jk2k1-Jk1k)*(Jk2k1+Jk1k);
              shift /= 2*Jk1k*Jk1k1;
              shift += (shift < 0 ? -1 : 1) * sqrt(shift*shift+1);
              shift = ( (Jll-Jkk)*(Jll+Jkk) + Jk1k*(Jk1k1/shift-Jk1k) )/Jll;
            }
            // Carry on multiplications by T2, S2, T3...
            double cos_th = 1, sin_th = 1;
            T Ji1i1 = sig_[l];  // J[i-1,i-1] at i=l+1...k
            for(register int i=l+1; i<=k; i++)
              {
                T Ji1i = super_diag[i], Jii = sig_[i];  // J[i-1,i] and J[i,i]
                sin_th *= Ji1i; Ji1i *= cos_th; cos_th = shift;
                double norm_f = (super_diag[i-1] = hypot(cos_th,sin_th));
                cos_th /= norm_f, sin_th /= norm_f;
                // Rotate J[i-1:i,i-1:i] by Ti
                shift = cos_th*Ji1i1 + sin_th*Ji1i;     // new J[i-1,i-1]
                Ji1i = -sin_th*Ji1i1 + cos_th*Ji1i;     // J[i-1,i] after rotation
                const double Jii1 = Jii*sin_th;         // Emerged J[i,i-1]
                Jii *= cos_th;                          // new J[i,i]
                rotate(V_,i,i-1,cos_th,sin_th); // Accumulate T rotations in V

                double cos_ph = shift, sin_ph = Jii1;// Make Si to get rid of J[i,i-1]
                sig_[i-1] = (norm_f = hypot(cos_ph,sin_ph));    // New J[i-1,i-1]
                if( norm_f == 0 )               // If norm =0, rotation angle
                  cos_ph = cos_th, sin_ph = sin_th; // can be anything now
                else
                  cos_ph /= norm_f, sin_ph /= norm_f;
                // Rotate J[i-1:i,i-1:i] by Si
                shift = cos_ph * Ji1i + sin_ph*Jii;     // New J[i-1,i]
                Ji1i1 = -sin_ph*Ji1i + cos_ph*Jii;      // New Jii, would carry over
                // as J[i-1,i-1] for next i
                rotate(U_,i,i-1,cos_ph,sin_ph);  // Accumulate S rotations in U
                // Jii1 disappears, sin_th would
                cos_th = cos_ph, sin_th = sin_ph; // carry over a (scaled) J[i-1,i+1]
                // to eliminate on the next i, cos_th
                // would carry over a scaled J[i,i+1]
              }
            super_diag[l] = 0;          // Supposed to be eliminated by now
            super_diag[k] = shift;
            sig_[k] = Ji1i1;
          }             // --- end-of-QR-iterations
        if( sig_[k] < 0 )               // Correct the sign of the sing number
          {
            sig_[k] = -sig_[k];
            for(register int j=0; j<(int)V_.getRowNb(); j++)
              V_.setAt(j,k,V_.getAt(j,k) -V_.getAt(j,k));
          }
      }
  }

};

/*!
  \brief Solves the linear system \a A \a X = \a B

  Solves the linear system \a A \a X = \a B. Given \a A and B it
  finds the value of \a X. The routine uses LU decomposition
  if the A matrix is square and it uses SVD decomposition
  otherwise.

  \param   A  the \a A matrix
  \param B  the right hand side of the equation
  \param X  the resulting matrix

  \return 1 if the SVD decomposition worked.

  \author Philippe Lavoie
  \date 22 October 1997
*/
template <class T>
int solve(const NumericArray2<T>& A, const NumericArray2<T>& B, NumericArray2<T>& X){
  if(A.getRowNb()==A.getColumnNb()){
    // use LU decomposition to solve the problem
    LUMatrix<T> lu(A) ;
    X = lu.inverse()*B ;
  }
  else{
    SVDMatrix<T> svd(A) ;
    return svd.solve(B,X) ;
  }
  return 1;
}

/*!
  \brief finds the inverse of a matrix

  Finds the inverse of a matrix. It uses LU decomposition if
  the matrix is square and it uses SVD decomposition otherwise.

  \param A  the matrix to inverse

  \return The inverse of A

  \warning The A matrix must have a number of rows greater or equal to
  its number of columns.

  \author Philippe Lavoie
  \date 22 October 1997
*/
template <class T>
NumericArray2<T> inverse(const NumericArray2<T>&A){
  NumericArray2<T> inv ;
  if(A.getRowNb()==A.getColumnNb()){
    LUMatrix<T> lu(A) ;
    lu.inverseIn(inv) ;
  }
  else{
    SVDMatrix<T> svd(A) ;
    svd.inverseIn(inv) ;
  }
  return inv ;
}



// #ifdef INCLUDE_TEMPLATE_SOURCE
// #include "util_matrixmath.C"


/* ----------------------------------------------------------------------- */

TOOLS_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
 //__util_matrixmat_h__


