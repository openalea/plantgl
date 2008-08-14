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

/*! \file util_eigenvector.h
    \brief Algorithm to compute eigen vector from points.
    This code was written by Philippe Borianne.
*/


#ifndef __util_eigenvector_h__
#define __util_eigenvector_h__

/* ----------------------------------------------------------------------- */

#include "../algo_config.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/container/pointarray.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

const double VZero=0.000001;
const double VInfini=99999999999.;


void ALGO_API Laxi_VectNorm (double valprop,double vectprop[3]);

/*! polynome caracteristique : \n
  P(t) = determinant(I*t-mat3x3) \n
  P(t) = pc[3]*t3 + pc[2]*t2 + pc[1]*t + pc[0] \n
*/
extern void ALGO_API Laxi_Polynom (const double mat3x3[3][3],double pc[4]);


/*! estimation P(x) = pc[3]*x3 + pc[2]*x2 + pc[1]*x + pc[0]
*/
extern double ALGO_API Laxi_PolyValue(double pc[4],double x);


/*! etude de la courbe C definie par l'equation P(x) : \n
  on cherche ici a decomposer la courbe C en segments monotones (croissant ou decroissant) \n
  -------------------------------------------------------------------- \n
  allure de la courbe \n
  -------------------------------------------------------------------- \n
*/

extern void ALGO_API Laxi_CurveStudy (double pc[4],double *z1,double *z2,int *n);

/*! Recherche Valeurs Propres sur un intervalle donne : \n
on cherche la racine r de l'intervalle [a_int,b_int] / \n
            P(r) = pc[3]*r3 + pc[2]*r2 + pc[1]*r + pc[0] = 0 \n
-------------------------------------------------------------------- \n
reduction de l'intervalle de recherche des racines du polynome \n
-------------------------------------------------------------------- \n
*/
extern void ALGO_API Laxi_FindInterval (double pc[4],double a_int,double b_int,double *a,double *b);


/*! calcul racine
*/
extern void ALGO_API Laxi_FindRoots (double pc[4],double a_int,double b_int,double *t);

/*!
Recherche Vecteurs Propres Associes aux Valeurs Propres du polynome \n
resolution systeme lineaire : \n
methode d'elimination de GAUSS-JORDAN avec pivot maximal par echange de lignes : \n
- l'algorithme est modifie de facon a considerer le dernier pivot nul (ainsi, le \n
vecteur propre calcule est strictement different du vecteur nul) \n
-------------------------------------------------------------------- \n
recherche vecteur propre associe a une valeur propre SIMPLE \n
-------------------------------------------------------------------- \n
*/
extern void ALGO_API Laxi_FindSingleVP (double MemGauche[3][3],double MemDroit[3],double Solution[3]);


/*-------------------------------------------------------------------*/
extern void ALGO_API Laxi_VecteurPropre_1 (double mat3x3[3][3],double valprop,double vectprop[3]);


/*! recherche vecteur propre associe a une valeur propre DOUBLE
*/
extern void ALGO_API Laxi_FindDoubleVP (double MemGauche[3][3],double MemDroit[3],double Solution1[3],double Solution2[3]);

/*-------------------------------------------------------------------*/
extern void ALGO_API Laxi_VecteurPropre_2(double mat3x3[3][3],double valprop, double vectprop1[3],double vectprop2[3]);


/*!
recherche vecteur propre associe a une valeur propre TRIPLE
*/
extern void ALGO_API Laxi_FindTripleVP (double MemGauche[3][3],double MemDroit[3],double Solution1[3],double Solution2[3],double Solution3[3]);


/*-------------------------------------------------------------------*/
extern void ALGO_API Laxi_VecteurPropre_3 (double mat3x3[3][3],double valprop,double vectprop1[3],double vectprop2[3],double vectprop3[3]);


/* !
calcul vecteurs propres
*/
extern void ALGO_API Laxi_VecteursPropres (double mat3x3[3][3],double pc[4],double val[3],double vect[3][3]);


/*! vecteurs propres : \n
	mat3x3	: matrice de l'operateur lineaire ( donnee ) \n
	val	: valeurs propres associees a l'operateur lineaire ( cherchees ) \n
                  si pas de solution, val = Infini \n
	vect	: vecteurs propres associes aux valeurs propres ( cherchees ) \n
                  si pas de solution, vect = (Infini,Infini,Infini) \n
*/
extern void ALGO_API Laxi_ValproVecpro (double mat3x3[3][3],double val[3],double vect[3][3]);


/*! vecteurs propres distincts : \n
	mat3x3	: matrice de l'operateur lineaire ( donnee ) \n
	val	: valeurs propres associees a l'operateur lineaire ( cherchees ) \n
                  si pas de solution, val = Infini \n
	vect	: vecteurs propres distincts associes aux valeurs propres ( cherchees ) \n
                  si pas de solution, vect = (Infini,Infini,Infini) \n
*/
extern void ALGO_API Laxi_Vecpro (double mat3x3[3][3],double vect[3][3],short *marqueur);


/*! vecteurs propres distincts : \n
	mat3x3	: matrice de l'operateur lineaire ( donnee ) \n
	val	: valeurs propres associees a l'operateur lineaire ( cherchees ) \n
                  si pas de solution, val = Infini \n
	vect	: vecteurs propres distincts associes aux valeurs propres ( cherchees ) \n
                  si pas de solution, vect = (Infini,Infini,Infini) \n
*/
extern void ALGO_API Laxi_ComputeVecpro (double vpmat[3][3],float vpvec[3][3],float vpint[3],float vpang[3][3],short vpok[3]);

/*! Calcul de la matrice d'inertie */
extern real_t ALGO_API Laxi_ComputeInertiaM (float *pts, float * ponderation, int nbp,float vpvec[3][3],float vpint[3],
			    float vpang[3][3],short vpok[3],float fvpmat[3][3]);


extern void ALGO_API Laxi_ComputeInertia (Point3ArrayPtr pts,float vpvec[3][3],float vpint[3],float vpang[3][3],short vpok[3]);



/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

#endif // __util_eigenvector_h__
