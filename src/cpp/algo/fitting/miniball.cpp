

//    Copright (C) 1999
//    $Revision$
//    $Date$
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA,
//    or download the License terms from prep.ai.mit.edu/pub/gnu/COPYING-2.0.
//
//    Contact:
//    --------
//    Bernd Gaertner
//    Institut f. Informatik
//    ETH Zuerich
//    ETH-Zentrum
//    CH-8092 Zuerich, Switzerland
//    http://www.inf.ethz.ch/personal/gaertner
//


#ifdef MINIBALL_NO_STD_NAMESPACE
   #include<assert.h>
#else
   #include<cassert>
#endif

	#include "miniball.h"

   // Miniball
   // --------
   
   
   void Miniball::check_in (const Vector3& p)
   {
       L.push_back(p);
   }
   
   void Miniball::check_in (const Point3ArrayPtr& p)
   {
       L.insert(L.end(),p->getBegin(),p->getEnd());
   }
   
   
   void Miniball::build (bool pivoting)
   {
       B.reset();
       support_end = L.begin();
       if (pivoting)
           pivot_mb (L.end());
       else
           mtf_mb (L.end());
   }
   
   
   
   void Miniball::mtf_mb (It i)
   {
       support_end = L.begin();
       if ((B.size())==BSIZE) return;
       for (It k=L.begin(); k!=i;) {
           It j=k++;
           if (B.excess(*j) > 0) {
               if (B.push(*j)) {
                   mtf_mb (j);
                   B.pop();
                   move_to_front(j);
               }
           }
       }
   }
   
   
   void Miniball::move_to_front (It j)
   {
       if (support_end == j)
           support_end++;
       L.splice (L.begin(), L, j);
   }
   
   
   
   void Miniball::pivot_mb (It i)
   {
       It t = ++L.begin();
       mtf_mb (t);
       double max_e, old_sqr_r;
       do {
           It pivot;
           max_e = max_excess (t, i, pivot);
           if (max_e > 0) {
               t = support_end;
               if (t==pivot) ++t;
               old_sqr_r = B.squared_radius();
               B.push (*pivot);
               mtf_mb (support_end);
               B.pop();
               move_to_front (pivot);
           }
       } while ((max_e > 0) && (B.squared_radius() > old_sqr_r));
   }
   
   
   
   double Miniball::max_excess (It t, It i, It& pivot) const
   {
       const Vector3 c = B.center();
	   const double sqr_r = B.squared_radius();
       double e, max_e = 0;
       for (It k=t; k!=i; ++k) {
           e = -sqr_r;
		   e += normSquared(*k-c);
           if (e > max_e) {
               max_e = e;
               pivot = k;
           }
       }
       return max_e;
    }
   
   
   
   
   Vector3 Miniball::center () const
   {
       return Vector3(B.center());
   }
   
   
   double Miniball::squared_radius () const
   {
       return B.squared_radius();
   }
   
   
   
   int Miniball::nr_points () const
   {
       return L.size();
   }
   
   
   Miniball::Cit Miniball::points_begin () const
   {
       return L.begin();
   }
   
   
   Miniball::Cit Miniball::points_end () const
   {
       return L.end();
   }
   
   
   
   int Miniball::nr_support_points () const
   {
       return B.support_size();
   }
   
   
   Miniball::Cit Miniball::support_points_begin () const
   {
       return L.begin();
   }
   
   
   Miniball::Cit Miniball::support_points_end () const
   {
       return support_end;
   }
   
   
   
   
   double Miniball::accuracy (double& slack) const
   {
       double e, max_e = 0;
       int n_supp=0;
       Cit i;
       for (i=L.begin(); i!=support_end; ++i,++n_supp)
           if ((e = abs (B.excess (*i))) > max_e)
               max_e = e;
   
       // you've found a non-numerical problem if the following ever fails
       assert (n_supp == nr_support_points());
   
       for (i=support_end; i!=L.end(); ++i)
          if ((e = B.excess (*i)) > max_e)
               max_e = e;
   
       slack = B.slack();
       return (max_e/squared_radius());
   }
   
   
   
   bool Miniball::is_valid (double tolerance) const
   {
       double slack;
       return ( (accuracy (slack) < tolerance) && (slack == 0) );
   }
   
   

   // Basis
   // -----
   
   
   const Vector3& Basis::center () const
   {
       return *current_c;
   }
   
   
   double Basis::squared_radius() const
   {
       return current_sqr_r;
   }
   
   
   int Basis::size() const
   {
       return m;
   }
   
   
   int Basis::support_size() const
   {
       return s;
   }
   
   
   double Basis::excess (const Vector3& p) const
   {
       double e = -current_sqr_r;
       e += normSquared(p-*current_c);
       return e;
   }
   
   
   
   
   void Basis::reset ()
   {
       m = s = 0;
       // we misuse c[0] for the center of the empty sphere
       for (Vector3::iterator j=c[0].getBegin(); j !=c[0].getEnd(); ++j)
           *j=0;
       current_c = &c[0];
       current_sqr_r = -1;
   }
   
   
   
   Basis::Basis ()
   {
       reset();
   }
   
   
   
   void Basis::pop ()
   {
       --m;
   }
   
   
   
   bool Basis::push (const Vector3& p)
   {
       int i, j;
       double eps = 1e-32;
       if (m==0) {
		   q0 = p;
           c[0] = q0;
           sqr_r[0] = 0;
       } 
	   else {
          // set v_m to Q_m
          v[m] = p-q0;
   
          // compute the a_{m,i}, i< m
          for (i=1; i<m; ++i) {
               a[m].getAt(i) = 0;
               for (j=0; j<3; ++j)
                   a[m].getAt(i) += v[i].getAt(j) * v[m].getAt(j);
               a[m].getAt(i)*=(2/z[i]);
          }
   
          // update v_m to Q_m-\bar{Q}_m
          for (i=1; i<m; ++i) {
               for (j=0; j<3; ++j)
                   v[m].getAt(j) -= a[m].getAt(i)*v[i].getAt(j);
          }
   
          // compute z_m
          z[m]=0;
          z[m]+= normSquared(v[m]);
          z[m]*=2;
   
          // reject push if z_m too small
          if (z[m]<eps*current_sqr_r) {
               return false;
          }
   
          // update c, sqr_r
          double e = -sqr_r[m-1];
          e += normSquared(p-c[m-1]);
          f[m]=e/z[m];
   
          c[m] = c[m-1]+v[m]*f[m];
          sqr_r[m] = sqr_r[m-1] + e*f[m]/2;
       }
       current_c = &(c[m]);
       current_sqr_r = sqr_r[m];
       s = ++m;
       return true;
   }
   
   
   
   
   double Basis::slack () const
   {
       double l[4], min_l=0;
       l[0] = 1;
       for (int i=s-1; i>0; --i) {
           l[i] = f[i];
           for (int k=s-1; k>i; --k)
               l[i]-=a[k].getAt(i)*l[k];
           if (l[i] < min_l) min_l = l[i];
           l[0] -= l[i];
       }
       if (l[0] < min_l) min_l = l[0];
       return ( (min_l < 0) ? -min_l : 0);
   }
   
   

