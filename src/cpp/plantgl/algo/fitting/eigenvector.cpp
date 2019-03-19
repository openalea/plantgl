/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
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







#include "eigenvector.h"
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

void PGL(Laxi_VectNorm) (double valprop,double vectprop[3])
   {
   double vno, vp;

   vno = vectprop[0]*vectprop[0] + vectprop[1]*vectprop[1] + vectprop[2]*vectprop[2];

   if (vno > 1.0e-5  &&  valprop > 1.0e-5)
      {
      vno = sqrt (vno);
      vp = 2.0 * sqrt (valprop);
      vectprop[0] *= vp/vno;
      vectprop[1] *= vp/vno;
      vectprop[2] *= vp/vno;
      }
     else
      {
      vectprop[0] = 0.0;
      vectprop[1] = 0.0;
      vectprop[2] = 0.0;
      }
   }


/*
polynome caracteristique :
    P(t) = determinant(I*t-mat3x3)
    P(t) = pc[3]*t3 + pc[2]*t2 + pc[1]*t + pc[0]
*/

void PGL(Laxi_Polynom) (const double mat3x3[3][3],double pc[4])
   {
   double   a,b,c,d,e,f,g,h,i ;
//   double  pc[4];
   /* coefficients operateur lineaire */
   a = mat3x3[0][0] ;
   b = mat3x3[0][1] ;
   c = mat3x3[0][2] ;
   d = mat3x3[1][0] ;
   e = mat3x3[1][1] ;
   f = mat3x3[1][2] ;
   g = mat3x3[2][0] ;
   h = mat3x3[2][1] ;
   i = mat3x3[2][2] ;

   /* coefficients polynome caracteristique */
   pc[0] = a*f*h + b*d*i + c*e*g - a*e*i - b*g*f - c*d*h ;
   pc[1] = a*e + a*i + e*i - b*d -c*g - f*h ;
   pc[2] = - a - e - i ;
   pc[3] = 1. ;

//   return pc;
   }



/*
estimation P(x) = pc[3]*x3 + pc[2]*x2 + pc[1]*x + pc[0]
*/
double PGL(Laxi_PolyValue)(double pc[4],double x)
   {
   double   y ;
   int  i ;

   y = 0. ;
   for (i = 0; i < 4; i++)
      y = x*y + pc[3-i] ;
   return(y) ;
   }



/*
--------------------------------------------------------------------
etude de la courbe C definie par l'equation P(x) :
on cherche ici a decomposer la courbe C en segments monotones (croissant ou decroissant)
--------------------------------------------------------------------
allure de la courbe
--------------------------------------------------------------------
*/

void PGL(Laxi_CurveStudy) (double pc[4],double *z1,double *z2,int *n)
   {
   double a,b,c,d,delta,r1,r2 ;

   /* derivee */
   a = 3*pc[3] ;
   b = 2*pc[2] ;
   c = pc[1] ;

   /* etude derivee : recherche des zeros */
   delta = b*b - 4*a*c ;
   if ( fabs(delta) < VZero )
      {
      /* pas de racines : polynome est monotone (croissant ou decroissant) */
      *n = 1 ;
      }
     else
      if ( fabs(delta) > VZero )
         {
         /* 2 racines distinctes : polynome croissant,decroissant,... */
         d = sqrt (delta) ;
         r1 = ( -b-d ) / ( 2*a ) ;
         r2 = ( -b+d ) / ( 2*a ) ;
         /* retour */
         if ( r1 < r2 )
            {
            *z1 = r1 ;
            *z2 = r2 ;
            }
           else
            {
            *z1 = r2 ;
            *z2 = r1 ;
            }
         *n = 3 ;
         }
        else
         {
         /* 1 racine double : polynome croissant,decroissant,... */
         r1 = r2 = -b / ( 2*a ) ;
         /* retour */
         *z1 = r1 ;
         *z2 = r2 ;
         *n = 2 ;
         }
   }





/*
--------------------------------------------------------------------
Recherche Valeurs Propres sur un intervalle donne :
on cherche la racine r de l'intervalle [a_int,b_int] /
            P(r) = pc[3]*r3 + pc[2]*r2 + pc[1]*r + pc[0] = 0
--------------------------------------------------------------------
reduction de l'intervalle de recherche des racines du polynome
--------------------------------------------------------------------
*/
void PGL(Laxi_FindInterval) (double pc[4],double a_int,double b_int,double *a,double *b)
   {
   double   aa,bb,cc,poly_aa,poly_bb,poly_cc ;
   int  non_stop ;

   aa = a_int ;
   bb = b_int ;
   poly_aa = Laxi_PolyValue(pc,aa) ;
   poly_bb = Laxi_PolyValue(pc,bb) ;

   if ( fabs(poly_aa) < VZero )
      {
      *a = aa ;
      *b = bb ;
      }
     else
      if ( fabs(poly_bb) < VZero )
         {
         *a = aa ;
         *b = bb ;
         }
        else
         if ( poly_aa*poly_bb < 0. )
            {
            non_stop = 1 ;
            while ( non_stop )
               {
               cc = ( aa+bb ) / 2. ;
               poly_cc = Laxi_PolyValue(pc,cc) ;
               if ( poly_aa*poly_cc < 0. )
                  bb = cc ;
                 else
                  if ( poly_cc*poly_bb < 0. )
                     aa = cc ;
                    else
                     non_stop = 0 ;
               if ( fabs(aa-bb) <= VZero )
                  non_stop = 0 ;
               }
            *a = aa ;
            *b = bb ;
            }
   }



/*
--------------------------------------------------------------------
calcul racine
--------------------------------------------------------------------
*/
void PGL(Laxi_FindRoots) (double pc[4],double a_int,double b_int,double *t)
   {
   double   a,b,c,poly_a,poly_b,poly_c ;
   int  non_stop ;

   /* reduction intervalle d'etude */
   Laxi_FindInterval (pc,a_int,b_int,&a,&b) ;

   /* recherche racine */
   poly_a = Laxi_PolyValue(pc,a) ;
   poly_b = Laxi_PolyValue(pc,b) ;

   if ( fabs(poly_a) < VZero )
      {
      /* P(a) = 0 */
      *t = a ;
      }
     else
      {
      if ( fabs(poly_b) < VZero )
         {
         /* P(b) = 0 */
         *t = b ;
         }
        else
         {
         if ( poly_a*poly_b < 0. )
            {
            /* recherche racine de P sur intervalle ]a_int,b_int[ */
            non_stop = 1 ;
            while ( non_stop )
               {
               poly_a = Laxi_PolyValue (pc,a) ;
               poly_b = Laxi_PolyValue (pc,b) ;
               c = a - (a-b)*poly_a / (poly_a-poly_b) ;
               poly_c = Laxi_PolyValue (pc,c) ;
               if ( fabs(poly_c) > VZero )
                  if ( fabs(a-b) > VZero )
                     {
                     if ( poly_a*poly_c < 0. )
                        b = c ;
                       else
                        if ( poly_a*poly_c > 0. )
                           a = c ;
                          else
                           {
                           *t = c ;
                           non_stop = 0 ;
                           }
                     }
                    else
                     {
                     *t = c ;
                     non_stop = 0 ;
                     }
                 else
                  {
                  *t = c ;
                  non_stop = 0 ;
                  }
               }
            }
           else
            {
            /* pas racine de P sur intervalle ]a_int,b_int[ */
            *t = VInfini ;
            }
         }
      }
   }


/*
--------------------------------------------------------------------
Recherche Vecteurs Propres Associes aux Valeurs Propres du polynome
resolution systeme lineaire :
methode d'elimination de GAUSS-JORDAN avec pivot maximal par echange de lignes :
! l'algorithme est modifie de facon a considerer le dernier pivot nul (ainsi, le
! vecteur propre calcule est strictement different du vecteur nul)
--------------------------------------------------------------------
recherche vecteur propre associe a une valeur propre SIMPLE
--------------------------------------------------------------------
*/

void PGL(Laxi_FindSingleVP) (double MemGauche[3][3],double MemDroit[3],double Solution[3])
   {
   double a[3][4],b[3][4],pivot,quot,tempo,s,solution[3] ;
   int   i,j,jp,ni,nj,njmoins1,njmoins2,J,Jplus1,i_pivot,li,rang ;
   int   pivnul[3],coef[3] ;

   /* initialisation */
   ni = 3 ;
   nj = 4 ;
   njmoins1 = nj-1;
   njmoins2 = nj-2;

   /*
    * initialisation
    */
   for (i=0;i<ni;i++)
      {
       for (j=0; j < njmoins1; j++)
          a[i][j] = MemGauche[i][j] ;
       a[i][njmoins1] = MemDroit[i] ;
       }
   for (j=0; j<njmoins1; j++)
       {
       coef [j] = j ;
       pivnul [j] = 0 ;
       }

   /*
    * resolution du systeme
    */
   for (J=0;J<njmoins2;J++)
      {

      /*
       * recherche pivot maximal du Jeme coefficient,
       * i.e le plus grand coefficient en valeur absolue sur la colonne J, a partir
       * de la ligne J.
       */
      pivot = a[J][J] ;
      i_pivot = J ;
      Jplus1 = J+1;
      for (i=Jplus1; i<ni; i++)
         {
         if ( fabs(a[i][J]) > fabs(pivot) )
            {
            pivot = a[i][J] ;
            i_pivot = i ;
            }
         }

      /*
       * inversion de lignes ou colones
       */

      if ( fabs(pivot) < VZero )
         {
         /*
          * echange des Jeme et jpeme colonne :
          * le pivot est mis en derniere position (J,jp) et le traitement est
          * recommence
          */
         jp = -1;
         for (j=njmoins2; j>J; j--)
             if ( coef [j] == j )
                {
                jp = j;
                j = -1;
                }

         if ( jp > J )
            {
            /*
             * permutation de colonnes possible
             */
            for (i=0; i<ni; i++)
               {
               tempo = a[i][J] ;
               a[i][J] = a[i][jp] ;
               a[i][jp] = tempo ;
               }

            coef [jp] = J;
            coef [J] = jp;
            pivnul [jp] = 1;
            pivnul [J] = 0;
            J-- ;
            }
         else
            {
            /*
             * permutation de colonnes impossible
             */
            pivnul [J] = 1;
            J = nj;
            }
         }
      else
         {
         if ( i_pivot > 0 )
            {
            /*
             * echanger ligne Jeme ligne et (i_pivot)eme ligne :
             * le pivot est mis en position (J,J) ; le fait qu'il soit le plus
             * grand possible s'avere efficace pour le controle des erreurs d'arrondis
             * lors du calcul.
             */
            for (j=0; j<nj; j++)
               {
               tempo = a[i_pivot][j] ;
               a[i_pivot][j] = a[J][j] ;
               a[J][j] = tempo ;
               }
            }

         /*
          * elimination de Gauss-Jordan :
          * reduire a zero tous les coefficients de la colonne J, en-deca du point (J,J) ;
          * ceci est un pas vers la triangulation.
          */

         for (i=Jplus1; i<ni; i++)
            {
            quot = a[i][J] / pivot;
            for (j=J; j<nj; j++)
               b[i][j] = a[i][j] - a[J][j] * quot ;
            }

         /* echanger matrices a et b */
         for (i=Jplus1; i<ni; i++)
            {
            quot = a[i][J] / pivot;
            for (j=J; j<nj; j++)
               a[i][j] = b[i][j];
            }
         }
      }

   /*
    * calcul solutions
    */

   /*
    * systeme singulier :
    * le dernier pivot est considere comme nul, sinon le systeme (Mat-l.Id)v=0 aurait
    * le vecteur nul pour sulution evidente...
    */
   pivnul [ni-1] = 1 ;

   rang = ni ;
   for (i=0; i<ni; i++)
      if ( pivnul [ni-i-1] )
         {
         solution [ni-i-1] = (double)1. ;
         rang-- ;
         }

   for (i=0; i<rang; i++)
      {
      li = rang-i-1 ;
      s = a[li][njmoins1] ;
      for (j = njmoins2; j > li; j--)
         {
         s -= a[li][j]*solution[j] ;
         }
      solution[li] = s / a[li][li];
      }

   /*
    * ordonner solutions :
    * les colonnes sont permutees de facon a ce que le vecteur trouve soit
    * solution de la matrice initiale
    */
   for (i = 0; i < ni; i++)
      {
      li = coef[i] ;
      Solution[li] = solution[i] ;
      }

#if 0
/* test solutions */
printf("...... Vecteur solution : %f %f %f\n", Solution[0],Solution[1],Solution[2]) ;
printf("...... -> (test) : ") ;
for (i=0;i<ni;i++)
    {
    s = -MemDroit[i] ;
    for (j=0;j<njmoins1;j++)  s += MemGauche[i][j]*Solution[j] ;
    printf("%f ",s) ;
    }
printf("\n\n") ;
#endif
   }



/*-------------------------------------------------------------------*/
void PGL(Laxi_VecteurPropre_1) (double mat3x3[3][3],double valprop,double vectprop[3])
   {
   double   MemGauche[3][3],MemDroit[3] ;
   int  i,j ;

   /* initialisation */
   for (i=0;i<3;i++)
       {
       for (j=0;j<3;j++)  MemGauche[i][j] = -mat3x3[i][j] ;
       MemDroit[i] = 0. ;
       }
   for (i=0;i<3;i++)
      MemGauche[i][i] += valprop ;

   /* vecteurs propres */
   Laxi_FindSingleVP (MemGauche,MemDroit,vectprop) ;

   Laxi_VectNorm (valprop, vectprop);
   }



/*
--------------------------------------------------------------------
recherche vecteur propre associe a une valeur propre DOUBLE
--------------------------------------------------------------------
*/
void PGL(Laxi_FindDoubleVP) (double MemGauche[3][3],double MemDroit[3],
                                double Solution1[3],double Solution2[3])
   {
   double a[3][4],b[3][4],pivot,quot,tempo,s1,s2,solution1[3],solution2[3] ;
   int   i,j,jp,ni,nj,njmoins1,njmoins2,J,Jplus1,i_pivot,li,rang ;
   int   pivnul[3],coef[3] ;

   /* initialisation */
   ni = 3 ;
   nj = 4 ;
   njmoins1 = nj-1;
   njmoins2 = nj-2;

   /*
    * initialisation
    */
   for (i=0;i<ni;i++)
      {
       for (j=0; j < njmoins1; j++)
          a[i][j] = MemGauche[i][j] ;
       a[i][njmoins1] = MemDroit[i] ;
       }
   for (j=0; j<njmoins1; j++)
       {
       coef [j] = j ;
       pivnul [j] = 0 ;
       }

   /*
    * resolution du systeme
    */
   for (J=0;J<njmoins2;J++)
      {

      /*
       * recherche pivot maximal du Jeme coefficient,
       * i.e le plus grand coefficient en valeur absolue sur la colonne J, a partir
       * de la ligne J.
       */
      pivot = a[J][J] ;
      i_pivot = J ;
      Jplus1 = J+1;
      for (i=Jplus1; i<ni; i++)
         {
         if ( fabs(a[i][J]) > fabs(pivot) )
            {
            pivot = a[i][J] ;
            i_pivot = i ;
            }
         }

      /*
       * inversion de lignes ou colones
       */

      if ( fabs(pivot) < VZero )
         {
         /*
          * echange des Jeme et jpeme colonne :
          * le pivot est mis en derniere position (J,jp) et le traitement est
          * recommence
          */
         jp = -1;
         for (j=njmoins2; j>J; j--)
             if ( coef [j] == j )
                {
                jp = j;
                j = -1;
                }

         if ( jp > J )
            {
            /*
             * permutation de colonnes possible
             */
            for (i=0; i<ni; i++)
               {
               tempo = a[i][J] ;
               a[i][J] = a[i][jp] ;
               a[i][jp] = tempo ;
               }

            coef [jp] = J;
            coef [J] = jp;
            pivnul [jp] = 1;
            pivnul [J] = 0;
            J-- ;
            }
         else
            {
            /*
             * permutation de colonnes impossible
             */
            pivnul [J] = 1;
            J = nj;
            }
         }
      else
         {
         if ( i_pivot > 0 )
            {
            /*
             * echanger ligne Jeme ligne et (i_pivot)eme ligne :
             * le pivot est mis en position (J,J) ; le fait qu'il soit le plus
             * grand possible s'avere efficace pour le controle des erreurs d'arrondis
             * lors du calcul.
             */
            for (j=0; j<nj; j++)
               {
               tempo = a[i_pivot][j] ;
               a[i_pivot][j] = a[J][j] ;
               a[J][j] = tempo ;
               }
            }

         /*
          * elimination de Gauss-Jordan :
          * reduire a zero tous les coefficients de la colonne J, en-deca du point (J,J) ;
          * ceci est un pas vers la triangulation.
          */

         for (i=Jplus1; i<ni; i++)
            {
            quot = a[i][J] / pivot;
            for (j=J; j<nj; j++)
               b[i][j] = a[i][j] - a[J][j] * quot ;
            }

         /* echanger matrices a et b */
         for (i=Jplus1; i<ni; i++)
            {
            quot = a[i][J] / pivot;
            for (j=J; j<nj; j++)
               a[i][j] = b[i][j];
            }
         }
      }

   /*
    * calcul solutions
    */

   /*
    * systeme singulier :
    * le dernier pivot est considere comme nul, sinon le systeme (Mat-l.Id)v=0 aurait
    * le vecteur nul pour sulution evidente...
    */
   pivnul [ni-1] = 1 ;

   if (( pivnul[ni-1] ) && ( pivnul[ni-2] ))
      {
      solution1[ni-1] = (double)1. ;
      solution1[ni-2] = (double)0. ;
      solution2[ni-1] = (double)0. ;
      solution2[ni-2] = (double)1. ;
      rang = li = ni-2 ;
      }
   else
      {
      solution1[ni-1] = (double)1. ;
      solution2[ni-1] = (double)0. ;
      rang = li = ni-1 ;
      }

   for (i=0; i<li; i++)
      if ( pivnul [li-i-1] )
         {
         solution1 [li-i-1] = (double)1. ;
         solution2 [li-i-1] = (double)1. ;
         rang-- ;
         }

   for (i=0; i<rang; i++)
      {
      li = rang-i-1 ;
      s1 = a[li][njmoins1] ;
      s2 = a[li][njmoins1] ;
      for (j = njmoins2; j > li; j--)
         {
         s1 -= a[li][j]*solution1[j] ;
         s2 -= a[li][j]*solution2[j] ;
         }
      solution1[li] = s1 / a[li][li];
      solution2[li] = s2 / a[li][li];
      }

   /*
    * ordonner solutions :
    * les colonnes sont permutees de facon a ce que le vecteur trouve soit
    * solution de la matrice initiale
    */
   for (i = 0; i < ni; i++)
      {
      li = coef[i] ;
      Solution1[li] = solution1[i] ;
      Solution2[li] = solution2[i] ;
      }

#if 0
/* test solutions */
printf("...... Vecteur solution1 : %f %f %f\n", Solution1[0],Solution1[1],Solution1[2]) ;
printf("...... -> (test) : ") ;
for (i=0;i<ni;i++)
    {
    s1 = -MemDroit[i] ;
    for (j=0;j<njmoins1;j++)  s1 += MemGauche[i][j]*Solution1[j] ;
    printf("%f ",s1) ;
    }
printf ("\n");
if (( fabs(Solution2[0]) > VZero ) ||
    ( fabs(Solution2[1]) > VZero ) ||
    ( fabs(Solution2[2]) > VZero ))
   {
   printf("...... Vecteur solution2 : %f %f %f\n", Solution2[0],Solution2[1],Solution2[2]) ;
   printf("...... -> (test) : ") ;
   for (i=0;i<ni;i++)
       {
       s2 = -MemDroit[i] ;
       for (j=0;j<njmoins1;j++)  s2 += MemGauche[i][j]*Solution2[j] ;
       printf("%f ",s2) ;
       }
   printf ("\n");
   }
else
   printf("...... Vecteur solution2 : ? ? ?\n") ;
printf("\n\n") ;
#endif
   }


/*-------------------------------------------------------------------*/

void PGL(Laxi_VecteurPropre_2)(double mat3x3[3][3],double valprop,
                                  double vectprop1[3],double vectprop2[3])
   {
   double MemGauche[3][3],MemDroit[3] ;
   int  i,j ;

   /* initialisation */
   for (i = 0; i < 3; i++)
      {
      for (j = 0; j < 3; j++)
         MemGauche[i][j] = -mat3x3[i][j] ;
      MemDroit[i] = 0. ;
      }
   for (i = 0; i < 3; i++)
      MemGauche[i][i] += valprop ;

   /* vecteurs propres */
   Laxi_FindDoubleVP(MemGauche,MemDroit,vectprop1,vectprop2) ;
   Laxi_VectNorm (valprop, vectprop1);
   Laxi_VectNorm (valprop, vectprop2);
   }



/*
--------------------------------------------------------------------
recherche vecteur propre associe a une valeur propre TRIPLE
--------------------------------------------------------------------
*/

void PGL(Laxi_FindTripleVP) (double MemGauche[3][3],double MemDroit[3],double Solution1[3],
                                double Solution2[3],double Solution3[3])
   {
   double a[3][4],b[3][4],pivot,quot,tempo,s1,s2,s3,solution1[3],solution2[3],solution3[3] ;
   int   i,j,jp,ni,nj,njmoins1,njmoins2,J,Jplus1,i_pivot,li,rang ;
   int   pivnul[3],coef[3] ;

   /* initialisation */
   ni = 3 ;
   nj = 4 ;
   njmoins1 = nj-1;
   njmoins2 = nj-2;

   /*
    * initialisation
    */
   for (i=0;i<ni;i++)
      {
       for (j=0; j < njmoins1; j++)
          a[i][j] = MemGauche[i][j] ;
       a[i][njmoins1] = MemDroit[i] ;
       }
   for (j=0; j<njmoins1; j++)
       {
       coef [j] = j ;
       pivnul [j] = 0 ;
       }

   /*
    * resolution du systeme
    */
   for (J=0;J<njmoins2;J++)
      {

      /*
       * recherche pivot maximal du Jeme coefficient,
       * i.e le plus grand coefficient en valeur absolue sur la colonne J, a partir
       * de la ligne J.
       */
      pivot = a[J][J] ;
      i_pivot = J ;
      Jplus1 = J+1;
      for (i=Jplus1; i<ni; i++)
         {
         if ( fabs(a[i][J]) > fabs(pivot) )
            {
            pivot = a[i][J] ;
            i_pivot = i ;
            }
         }

      /*
       * inversion de lignes ou colones
       */

      if ( fabs(pivot) < VZero )
         {
         /*
          * echange des Jeme et jpeme colonne :
          * le pivot est mis en derniere position (J,jp) et le traitement est
          * recommence
          */
         jp = -1;
         for (j=njmoins2; j>J; j--)
             if ( coef [j] == j )
                {
                jp = j;
                j = -1;
                }

         if ( jp > J )
            {
            /*
             * permutation de colonnes possible
             */
            for (i=0; i<ni; i++)
               {
               tempo = a[i][J] ;
               a[i][J] = a[i][jp] ;
               a[i][jp] = tempo ;
               }

            coef [jp] = J;
            coef [J] = jp;
            pivnul [jp] = 1;
            pivnul [J] = 0;
            J-- ;
            }
         else
            {
            /*
             * permutation de colonnes impossible
             */
            pivnul [J] = 1;
            J = nj;
            }
         }
      else
         {
         if ( i_pivot > 0 )
            {
            /*
             * echanger ligne Jeme ligne et (i_pivot)eme ligne :
             * le pivot est mis en position (J,J) ; le fait qu'il soit le plus
             * grand possible s'avere efficace pour le controle des erreurs d'arrondis
             * lors du calcul.
             */
            for (j=0; j<nj; j++)
               {
               tempo = a[i_pivot][j] ;
               a[i_pivot][j] = a[J][j] ;
               a[J][j] = tempo ;
               }
            }

         /*
          * elimination de Gauss-Jordan :
          * reduire a zero tous les coefficients de la colonne J, en-deca du point (J,J) ;
          * ceci est un pas vers la triangulation.
          */

         for (i=Jplus1; i<ni; i++)
            {
            quot = a[i][J] / pivot;
            for (j=J; j<nj; j++)
               b[i][j] = a[i][j] - a[J][j] * quot ;
            }

         /* echanger matrices a et b */
         for (i=Jplus1; i<ni; i++)
            {
            quot = a[i][J] / pivot;
            for (j=J; j<nj; j++)
               a[i][j] = b[i][j];
            }
         }
      }

   /*
    * calcul solutions
    */

   /*
    * systeme singulier :
    * le dernier pivot est considere comme nul, sinon le systeme (Mat-l.Id)v=0 aurait
    * le vecteur nul pour sulution evidente...
    */
   pivnul [ni-1] = 1 ;

   if (( pivnul[ni-1] ) && ( pivnul[ni-2] ) && ( pivnul[ni-3] ))
      {
      solution1[ni-1] = (double)1. ;
      solution1[ni-2] = (double)0. ;
      solution1[ni-3] = (double)0. ;
      solution2[ni-1] = (double)0. ;
      solution2[ni-2] = (double)1. ;
      solution2[ni-3] = (double)0. ;
      solution3[ni-1] = (double)0. ;
      solution3[ni-2] = (double)0. ;
      solution3[ni-3] = (double)1. ;
      rang = li = ni-3 ;
      }
   else
      if (( pivnul[ni-1] ) && ( pivnul[ni-2] ))
         {
         solution1[ni-1] = (double)1. ;
         solution1[ni-2] = (double)0. ;
         solution2[ni-1] = (double)0. ;
         solution2[ni-2] = (double)1. ;
         rang = li = ni-2 ;
         }
      else
         {
         solution1[ni-1] = (double)1. ;
         solution2[ni-1] = (double)0. ;
         rang = li = ni-1 ;
         }

   for (i=0; i<li; i++)
      if ( pivnul [li-i-1] )
         {
         solution1 [li-i-1] = (double)1. ;
         solution2 [li-i-1] = (double)1. ;
         solution3 [li-i-1] = (double)1. ;
         rang-- ;
         }

   for (i=0; i<rang; i++)
      {
      li = rang-i-1 ;
      s1 = a[li][njmoins1] ;
      s2 = a[li][njmoins1] ;
      s3 = a[li][njmoins1] ;
      for (j = njmoins2; j > li; j--)
         {
         s1 -= a[li][j]*solution1[j] ;
         s2 -= a[li][j]*solution2[j] ;
         s3 -= a[li][j]*solution3[j] ;
         }
      solution1[li] = s1 / a[li][li];
      solution2[li] = s2 / a[li][li];
      solution3[li] = s3 / a[li][li];
      }

   /*
    * ordonner solutions :
    * les colonnes sont permutees de facon a ce que le vecteur trouve soit
    * solution de la matrice initiale
    */
   for (i = 0; i < ni; i++)
      {
      li = coef[i] ;
      Solution1[li] = solution1[i] ;
      Solution2[li] = solution2[i] ;
      Solution3[li] = solution3[i] ;
      }

#if 0
/* test solutions */
printf("...... Vecteur solution1 : %f %f %f\n", Solution1[0],Solution1[1],Solution1[2]) ;
printf("...... -> (test) : ") ;
for (i=0;i<ni;i++)
    {
    s1 = -MemDroit[i] ;
    for (j=0;j<njmoins1;j++)  s1 += MemGauche[i][j]*Solution1[j] ;
    printf("%f ",s1) ;
    }
printf ("\n");
if (( fabs(Solution2[0]) > VZero ) ||
    ( fabs(Solution2[1]) > VZero ) ||
    ( fabs(Solution2[2]) > VZero ))
   {
   printf("...... Vecteur solution2 : %f %f %f\n", Solution2[0],Solution2[1],Solution2[2]) ;
   printf("...... -> (test) : ") ;
   for (i=0;i<ni;i++)
       {
       s2 = -MemDroit[i] ;
       for (j=0;j<njmoins1;j++)  s2 += MemGauche[i][j]*Solution2[j] ;
       printf("%f ",s2) ;
       }
   printf ("\n");
   }
else
   printf("...... Vecteur solution2 : ? ? ?\n") ;
if (( fabs(Solution3[0]) > VZero ) ||
    ( fabs(Solution3[1]) > VZero ) ||
    ( fabs(Solution3[2]) > VZero ))
   {
   printf("...... Vecteur solution3 : %f %f %f\n", Solution3[0],Solution3[1],Solution3[2]) ;
   printf("...... -> (test) : ") ;
   for (i=0;i<ni;i++)
       {
       s3 = -MemDroit[i] ;
       for (j=0;j<njmoins1;j++)  s3 += MemGauche[i][j]*Solution3[j] ;
       printf("%f ",s3) ;
       }
   printf ("\n");
   }
else
   printf("...... Vecteur solution3 : ? ? ?\n") ;
printf("\n\n") ;
#endif
   }



/*-------------------------------------------------------------------*/

void PGL(Laxi_VecteurPropre_3) (double mat3x3[3][3],double valprop,double vectprop1[3],
                                    double vectprop2[3],double vectprop3[3])
   {
   double   MemGauche[3][3], MemDroit[3] ;
   int  i,j ;

   /* initialisation */
   for (i = 0; i < 3; i++)
      {
      for (j = 0; j < 3;j++)
         MemGauche[i][j] = -mat3x3[i][j] ;
      MemDroit[i] = 0. ;
      }
   for (i = 0; i < 3; i++)
      MemGauche[i][i] += valprop ;

   /* vecteurs propres */
   Laxi_FindTripleVP (MemGauche,MemDroit,vectprop1,vectprop2,vectprop3) ;
   Laxi_VectNorm (valprop, vectprop1);
   Laxi_VectNorm (valprop, vectprop2);
   Laxi_VectNorm (valprop, vectprop3);
   }

/*
--------------------------------------------------------------------
calcul vecteurs propres
--------------------------------------------------------------------
*/

void PGL(Laxi_VecteursPropres) (double mat3x3[3][3],double pc[4],double val[3],double vect[3][3])
   {
   double   z1,z2,a_int,b_int,t0,t1,t2 ;
   int  n ;

   Laxi_CurveStudy (pc,&z1,&z2,&n) ;

   switch (n)
      {
      case 1:
      /* polynome est monotone sans point d'inflexion */
      /* valeurs propres */
      a_int = -VInfini ;
      b_int =  VInfini ;
      Laxi_FindRoots (pc,a_int,b_int,&t0) ;

      val[0] = t0 ;
      /* vecteurs propres */
      if (( t0 != 0. ) && ( t0 != VInfini ))  Laxi_VecteurPropre_1(mat3x3,t0,vect[0]) ;

      break ;
   case 2:
      /* polynome est monotone avec point d'inflexion */
      /* valeurs propres */
      a_int = -VInfini ;
      b_int =  z1 ;
      Laxi_FindRoots (pc,a_int,b_int,&t0) ;
      val[0] = t0 ;
      a_int =  z1 ;
      b_int =  VInfini ;
      Laxi_FindRoots (pc,a_int,b_int,&t1) ;

      val[0] = t0 ;
      val[1] = t1 ;

      /* vecteurs propres */
      if (( t0 != 0. ) && ( t0 != VInfini ))
         if (( t1 != 0. ) && ( t1 != VInfini ))
            if ( t0 == t1 )  Laxi_VecteurPropre_2(mat3x3,t0,vect[0],vect[1]) ;
            else
               {
               Laxi_VecteurPropre_1(mat3x3,t0,vect[0]) ;
               Laxi_VecteurPropre_1(mat3x3,t1,vect[1]) ;
               }
         else  Laxi_VecteurPropre_1(mat3x3,t0,vect[0]) ;
      else
         if (( t1 != 0. ) && ( t1 != VInfini ))   Laxi_VecteurPropre_1(mat3x3,t1,vect[1]) ;

      break ;
   case 3:
      /* polynome est non monotone */
      /* valeurs propres */
      a_int = -VInfini ;
      b_int =  z1 ;
      Laxi_FindRoots (pc,a_int,b_int,&t0) ;
      a_int =  z1 ;
      b_int =  z2 ;
      Laxi_FindRoots (pc,a_int,b_int,&t1) ;
      a_int =  z2 ;
      b_int =  VInfini ;
      Laxi_FindRoots (pc,a_int,b_int,&t2) ;

      val[0] = t0 ;
      val[1] = t1 ;
      val[2] = t2 ;

      /* vecteurs propres */
      if (( t0 != 0. ) && ( t0 != VInfini ))
         if (( t1 != 0. ) && ( t1 != VInfini ))
             if (( t2 != 0. ) && ( t2 != VInfini ))
                if ( t0 == t1 )
                   if ( t0 == t2 )
                      {
                      Laxi_VecteurPropre_3(mat3x3,t0,vect[0],vect[1],vect[2]) ;
                      }
                   else
                      {
                      Laxi_VecteurPropre_2(mat3x3,t0,vect[0],vect[1]) ;
                      Laxi_VecteurPropre_1(mat3x3,t2,vect[2]) ;
                      }
                else
                   if ( t0 == t2 )
                      {
                      Laxi_VecteurPropre_2(mat3x3,t0,vect[0],vect[2]) ;
                      Laxi_VecteurPropre_1(mat3x3,t1,vect[1]) ;
                      }
                   else
                      if ( t1 == t2 )
                         {
                         Laxi_VecteurPropre_1(mat3x3,t0,vect[0]) ;
                         Laxi_VecteurPropre_2(mat3x3,t1,vect[1],vect[2]) ;
                         }
                      else
                         {
                         Laxi_VecteurPropre_1(mat3x3,t0,vect[0]) ;
                         Laxi_VecteurPropre_1(mat3x3,t1,vect[1]) ;
                         Laxi_VecteurPropre_1(mat3x3,t2,vect[2]) ;
                         }
             else
                if ( t0 == t1 )
                   {
                   Laxi_VecteurPropre_2(mat3x3,t0,vect[0],vect[1]) ;
                   }
                else
                   {
                   Laxi_VecteurPropre_1(mat3x3,t0,vect[0]) ;
                   Laxi_VecteurPropre_1(mat3x3,t1,vect[1]) ;
                   }
         else
            if (( t2 != 0. ) && ( t2 != VInfini ))
               if ( t0 == t2 )
                  {
                  Laxi_VecteurPropre_2(mat3x3,t0,vect[0],vect[2]) ;
                  }
               else
                  {
                  Laxi_VecteurPropre_1(mat3x3,t0,vect[0]) ;
                  Laxi_VecteurPropre_1(mat3x3,t2,vect[2]) ;
                  }
            else
               {
               Laxi_VecteurPropre_1(mat3x3,t0,vect[0]) ;
               }
      else
         if (( t1 != 0. ) && ( t1 != VInfini ))
             if (( t2 != 0. ) && ( t2 != VInfini ))
                if ( t1 == t2 )
                   {
                   Laxi_VecteurPropre_2(mat3x3,t0,vect[1],vect[2]) ;
                   }
                else
                   {
                   Laxi_VecteurPropre_1(mat3x3,t1,vect[1]) ;
                   Laxi_VecteurPropre_1(mat3x3,t2,vect[2]) ;
                   }
             else
                {
                Laxi_VecteurPropre_1(mat3x3,t1,vect[1]) ;
                }
         else
             if (( t2 != 0. ) && ( t2 != VInfini ))
                {
                Laxi_VecteurPropre_1(mat3x3,t2,vect[2]) ;
                }

         break ;
      }
   }



/*
--------------------------------------------------------------------
vecteurs propres :
    mat3x3  : matrice de l'operateur lineaire ( donnee )
    val : valeurs propres associees a l'operateur lineaire ( cherchees )
                  si pas de solution, val = VInfini
    vect    : vecteurs propres associes aux valeurs propres ( cherchees )
                  si pas de solution, vect = (VInfini,VInfini,VInfini)
--------------------------------------------------------------------
*/
void PGL(Laxi_ValproVecpro) (double mat3x3[3][3],double val[3],double vect[3][3])
   {
   double   pc[4] ;

   /* initialisation */
   val[0] = val[1] = val[2] = VInfini ;
   vect[0][0] = vect[0][1] = vect[0][2] = VInfini ;
   vect[1][0] = vect[1][1] = vect[1][2] = VInfini ;
   vect[2][0] = vect[2][1] = vect[2][2] = VInfini ;

   /* traitement */
   Laxi_Polynom(mat3x3,pc) ;
   Laxi_VecteursPropres (mat3x3,pc,val,vect) ;
   }



/*
--------------------------------------------------------------------
vecteurs propres distincts :
    mat3x3  : matrice de l'operateur lineaire ( donnee )
    val : valeurs propres associees a l'operateur lineaire ( cherchees )
                  si pas de solution, val = VInfini
    vect    : vecteurs propres distincts associes aux valeurs propres ( cherchees )
                  si pas de solution, vect = (VInfini,VInfini,VInfini)
--------------------------------------------------------------------
*/
void PGL(Laxi_Vecpro) (double mat3x3[3][3],double vect[3][3],short *marqueur)
   {
   double   pc[4], val[3];

   /* initialisation */
   val[0] = val[1] = val[2] = VInfini ;
   vect[0][0] = vect[0][1] = vect[0][2] = VInfini ;
   vect[1][0] = vect[1][1] = vect[1][2] = VInfini ;
   vect[2][0] = vect[2][1] = vect[2][2] = VInfini ;

   marqueur[0] = 0;
   marqueur[1] = 0;
   marqueur[2] = 0;
   /* traitement */
   Laxi_Polynom (mat3x3,pc) ;
   Laxi_VecteursPropres (mat3x3,pc,val,vect) ;

   if ( vect[1][0] != VInfini )
      if (( vect[0][0] != VInfini ) &&
          ( fabs(vect[1][0]-vect[0][0]) < VZero ) && ( fabs(vect[1][1]-vect[0][1]) < VZero ) && ( fabs(vect[1][2]-vect[0][2]) < VZero ))
         {
         vect[1][0] = vect[1][1] = vect[1][2] = VInfini ;
         }

   if ( vect[2][0] != VInfini )
      {
      if (( vect[0][0] != VInfini ) &&
          ( fabs(vect[2][0]-vect[0][0]) < VZero ) && ( fabs(vect[2][1]-vect[0][1]) < VZero ) && ( fabs(vect[2][2]-vect[0][2]) < VZero ))
         {
         vect[2][0] = vect[2][1] = vect[2][2] = VInfini ;
         }
      if (( vect[1][0] != VInfini ) &&
          ( fabs(vect[2][0]-vect[1][0]) < VZero ) && ( fabs(vect[2][1]-vect[1][1]) < VZero ) && ( fabs(vect[2][2]-vect[1][2]) < VZero ))
         {
         vect[2][0] = vect[2][1] = vect[2][2] = VInfini ;
         }
      }

   if (vect[0][0] < VInfini)
      marqueur[0] = 1;
   if (vect[1][0] < VInfini)
      marqueur[1] = 1;
   if (vect[2][0] < VInfini)
      marqueur[2] = 1;

   }




/*
--------------------------------------------------------------------
vecteurs propres distincts :
    mat3x3  : matrice de l'operateur lineaire ( donnee )
    val : valeurs propres associees a l'operateur lineaire ( cherchees )
                  si pas de solution, val = VInfini
    vect    : vecteurs propres distincts associes aux valeurs propres ( cherchees )
                  si pas de solution, vect = (VInfini,VInfini,VInfini)
--------------------------------------------------------------------
*/
void PGL(Laxi_ComputeVecpro) (double vpmat[3][3],float vpvec[3][3],float vpint[3],
                                 float vpang[3][3],short vpok[3])
   {
   double vect[3][3], vint;
   short j;

   Laxi_Vecpro (vpmat, vect, vpok);

   for (j = 0; j < 3; j++)
      {
      vpint[j] = 0.0;
      vpang[j][0] = 0.0;
      vpang[j][1] = 0.0;
      vpang[j][2] = 0.0;
      vpvec[j][0] = 0.0;
      vpvec[j][1] = 0.0;
      vpvec[j][2] = 0.0;

      if (vpok[j])
         {
         vint = vect[j][0]*vect[j][0] + vect[j][1]*vect[j][1] + vect[j][2]*vect[j][2];
         if (vint > 1.0e-6)
            {
            vint = sqrt (vint);
            vpint[j] = (float)vint;
            vpvec[j][0] = vect[j][0];
            vpvec[j][1] = vect[j][1];
            vpvec[j][2] = vect[j][2];

        vect[j][0] /= vint;
            vect[j][1] /= vint;
            vect[j][2] /= vint;
            vpang[j][0] = 180.0/3.14159265*acos(vect[j][0]);

            if (vpang[j][0] < 0.0)
               vpang[j][0] += 180.0;
            vpang[j][1] = 180.0/3.14159265*acos(vect[j][1]);
            if (vpang[j][1] < 0.0)
               vpang[j][1] += 180.0;
            vpang[j][2] = 180.0/3.14159265*acos(vect[j][2]);
            if (vpang[j][2] < 0.0)
               vpang[j][2] += 180.0;
            }
         }
      }

   }





real_t PGL(Laxi_ComputeInertiaM) (float *pts, float * ponderation, int nbp,float vpvec[3][3],float vpint[3],
                                  float vpang[3][3],short vpok[3],float fvpmat[3][3])
   {
   double vpmat[3][3];
   double rnb;
   double x, y, z;
   double mx, my, mz;
   double mx2, my2, mz2;
   double mxy, mxz, myz;
   int j;
   real_t nbreeldepoints = 0.0; /* nombre de points ajuste selon les poids */

   if (!ponderation) nbreeldepoints = (real_t) nbp;

   for (j = 0; j < 3; j++)
      {
      vpvec[j][0] = 0.0;
      vpvec[j][1] = 0.0;
      vpvec[j][2] = 0.0;
      vpint[j] = 0.0;
      vpang[j][0] = 0.0;
      vpang[j][1] = 0.0;
      vpang[j][2] = 0.0;
      vpok[j] = 0;
      }

   if (nbp > 1)
      {
      rnb = 1.0/ (double) (nbp-1.0); /* A VOIR SI PAS (nbreeldepoints - 1.0) plutot */
      mx = my = mz = mx2 = my2 = mz2 = mxy = mxz = myz = 0.0;
      for (j = 0; j < nbp; j++)
         {
       if (ponderation)
         {
           x = pts[3*j]*ponderation[3*j];
           y = pts[3*j+1]*ponderation[3*j];
           z = pts[3*j+2]*ponderation[3*j];
           nbreeldepoints += ponderation[3*j];
         }
       else
         {
           x = pts[3*j];
           y = pts[3*j+1];
           z = pts[3*j+2];
         }

         mx += x;
         my += y;
         mz += z;
         mx2 += x*x;
         my2 += y*y;
         mz2 += z*z;
         mxy += x*y;
         mxz += x*z;
         myz += y*z;
         }
      mx /= (double)(nbreeldepoints);
      my /= (double)(nbreeldepoints);
      mz /= (double)(nbreeldepoints);
      vpmat[0][0] = (mx2 - nbreeldepoints*mx*mx) * rnb;
      vpmat[1][0] = vpmat[0][1] = (mxy - nbreeldepoints*mx*my) * rnb;
      vpmat[2][0] = vpmat[0][2] = (mxz - nbreeldepoints*mx*mz) * rnb;
      vpmat[1][1] = (my2 - nbreeldepoints*my*my) * rnb;
      vpmat[2][2] = (mz2 - nbreeldepoints*mz*mz) * rnb;
      vpmat[1][2] = vpmat[2][1] = (myz - nbreeldepoints*my*mz) * rnb;

      Laxi_ComputeVecpro (vpmat, vpvec, vpint, vpang, vpok);
      }

   if (fvpmat)
     {
       fvpmat[0][0] = (float) vpmat[0][0];
       fvpmat[1][0] = (float) vpmat[1][0];
       fvpmat[2][0] = (float) vpmat[2][0];
       fvpmat[0][1] = (float) vpmat[0][1];
       fvpmat[1][1] = (float) vpmat[1][1];
       fvpmat[2][1] = (float) vpmat[2][1];
       fvpmat[0][2] = (float) vpmat[0][2];
       fvpmat[1][2] = (float) vpmat[1][2];
       fvpmat[2][2] = (float) vpmat[2][2];
     }

   return nbreeldepoints;
   }


void PGL(Laxi_ComputeInertia) (Point3ArrayPtr pts,float vpvec[3][3],float vpint[3],float vpang[3][3],short vpok[3])
   {


   double vpmat[3][3];
   double rnb;
   double x, y, z;
   double mx, my, mz;
   double mx2, my2, mz2;
   double mxy, mxz, myz;
   Vector3 v1, v2, v3, testdirect;
   int j;
   int nbp =  pts->size();
   for (j = 0; j < 3; j++)
      {
      vpvec[j][0] = 0.0;
      vpvec[j][1] = 0.0;
      vpvec[j][2] = 0.0;
      vpint[j] = 0.0;
      vpang[j][0] = 0.0;
      vpang[j][1] = 0.0;
      vpang[j][2] = 0.0;
      vpok[j] = 0;
      }

   if ( nbp > 1)
      {
      rnb = 1.0/ (double) (nbp-1.0);
      mx = my = mz = mx2 = my2 = mz2 = mxy = mxz = myz = 0.0;
      for (j = 0; j <  nbp; j++)
         {
/*         x = pts[3*j];
         y = pts[3*j+1];
         z = pts[3*j+2];*/
     x =  pts->getAt(j).x();
     y =  pts->getAt(j).y();
     z =  pts->getAt(j).z();
         mx += x;
         my += y;
         mz += z;
         mx2 += x*x;
         my2 += y*y;
         mz2 += z*z;
         mxy += x*y;
         mxz += x*z;
         myz += y*z;
         }
      mx /= (double)(nbp);
      my /= (double)(nbp);
      mz /= (double)(nbp);
      vpmat[0][0] = (mx2 - nbp*mx*mx) * rnb;
      vpmat[1][0] = vpmat[0][1] = (mxy - nbp*mx*my) * rnb;
      vpmat[2][0] = vpmat[0][2] = (mxz - nbp*mx*mz) * rnb;
      vpmat[1][1] = (my2 - nbp*my*my) * rnb;
      vpmat[2][2] = (mz2 - nbp*mz*mz) * rnb;
      vpmat[1][2] = vpmat[2][1] = (myz - nbp*my*mz) * rnb;

      Laxi_ComputeVecpro (vpmat, vpvec, vpint, vpang, vpok);
      }

   /* En cas de triedre indirect, on choisit d'inverser vpvec[0] */
   v1.x() = vpvec[2][0]/vpint[2]; v1.y() = vpvec[2][1]/vpint[2]; v1.z() = vpvec[2][2]/vpint[2];
   v2.x() = vpvec[1][0]/vpint[1]; v2.y() = vpvec[1][1]/vpint[1]; v2.z() = vpvec[1][2]/vpint[1];
   v3.x() = vpvec[0][0]/vpint[0]; v3.y() = vpvec[0][1]/vpint[0]; v3.z() = vpvec[0][2]/vpint[0];

//   Lmath_vCross (v1, v2, testdirect);
   testdirect = cross(v1, v2);

   if ((fabs(testdirect.x()+v3.x()) < 1.0e-6) && (fabs(testdirect.y()+v3.y()) < 1.0e-6) && (fabs(testdirect.z()+v3.z()) < 1.0e-6))
     {
       vpvec[0][0] = -vpvec[0][0];
       vpvec[0][1] = -vpvec[0][1];
       vpvec[0][2] = -vpvec[0][2];
     }
   }


