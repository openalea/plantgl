

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
    #include <list.h>
#else
    #include <list>
#endif

    #include <plantgl/scenegraph/container/pointarray.h>
    PGL_USING_NAMESPACE


    // Basis
    // -----

#define BSIZE 4

    class Basis {
        private:
            // types

            // data members
            int                 m, s;   // size and number of support points
            Vector3             q0;

            double              z[BSIZE];
            double              f[BSIZE];
            Vector3             v[BSIZE];
            Vector3             a[BSIZE];

            Vector3              c[BSIZE];
            double               sqr_r[BSIZE];

            Vector3*            current_c;      // points to some c[j]
            double              current_sqr_r;

  //          double              sqr (double r) const {return r*r;}

        public:
            Basis();

            // access
            const Vector3&       center() const;
            double              squared_radius() const;
            int                 size() const;
            int                 support_size() const;
            double              excess (const Vector3& p) const;

            // modification
            void                reset(); // generates empty sphere with m=s=0
            bool                push (const Vector3& p);
            void                pop ();

            // checking
            double              slack() const;
    };


    // Miniball
    // --------

    class Miniball {
        public:
            // types
            typedef std::list<Vector3>::iterator         It;
            typedef std::list<Vector3>::const_iterator   Cit;

        private:
            // data members
            std::list<Vector3> L;         // STL list keeping the points
            Basis    B;              // basis keeping the current ball
            It          support_end;    // past-the-end iterator of support set

            // private methods
            void        mtf_mb (It k);
            void        pivot_mb (It k);
            void        move_to_front (It j);
            double      max_excess (It t, It i, It& pivot) const;
            double      abs (double r) const {return (r>0)? r: (-r);}
            double      sqr (double r) const {return r*r;}

        public:
            // construction
            Miniball() {}
            void        check_in (const Vector3& p);
            void        check_in (const Point3ArrayPtr& p);
            void        build (bool pivoting = true);

            // access
            Vector3       center() const;
            double      squared_radius () const;
            int         nr_points () const;
            Cit         points_begin () const;
            Cit         points_end () const;
            int         nr_support_points () const;
            Cit         support_points_begin () const;
            Cit         support_points_end () const;

            // checking
            double      accuracy (double& slack) const;
            bool        is_valid (double tolerance = 1e-15) const;
     };




   //  #include "miniball.C"
