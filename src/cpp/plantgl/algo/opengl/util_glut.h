/* Copyright (c) Mark J. Kilgard, 1994, 1997. */
/*
(c) Copyright 1993, Silicon Graphics, Inc.

ALL RIGHTS RESERVED

Permission to use, copy, modify, and distribute this software
for any purpose and without fee is hereby granted, provided
that the above copyright notice appear in all copies and that
both the copyright notice and this permission notice appear in
supporting documentation, and that the name of Silicon
Graphics, Inc. not be used in advertising or publicity
pertaining to distribution of the software without specific,
written prior permission.

THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU
"AS-IS" AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR
OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  IN NO
EVENT SHALL SILICON GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE
ELSE FOR ANY DIRECT, SPECIAL, INCIDENTAL, INDIRECT OR
CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER,
INCLUDING WITHOUT LIMITATION, LOSS OF PROFIT, LOSS OF USE,
SAVINGS OR REVENUE, OR THE CLAIMS OF THIRD PARTIES, WHETHER OR
NOT SILICON GRAPHICS, INC.  HAS BEEN ADVISED OF THE POSSIBILITY
OF SUCH LOSS, HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
ARISING OUT OF OR IN CONNECTION WITH THE POSSESSION, USE OR
PERFORMANCE OF THIS SOFTWARE.

US Government Users Restricted Rights

Use, duplication, or disclosure by the Government is subject to
restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
(c)(1)(ii) of the Rights in Technical Data and Computer
Software clause at DFARS 252.227-7013 and/or in similar or
successor clauses in the FAR or the DOD or NASA FAR
Supplement.  Unpublished-- rights reserved under the copyright
laws of the United States.  Contractor/manufacturer is Silicon
Graphics, Inc., 2011 N.  Shoreline Blvd., Mountain View, CA
94039-7311.

OpenGL(TM) is a trademark of Silicon Graphics, Inc.
*/


#ifndef __util_glut_h_
#define __util_glut_h_

/*! \file util_glut.h
    \brief File that contains some usefull glut command to avoid to use the entire glut lib.
*/

#include "../algo_config.h"

#ifndef WITH_GLUT
#ifndef WITHOUT_GLUT

#ifdef _WIN32
/// By default on windows, we don't use glut.
#define WITHOUT_GLUT
#else
/// Else by default, we use glut.
#define WITH_GLUT
#endif

#endif
#endif

#ifdef WITH_GLUT
#undef WITHOUT_GLUT
#endif


#include "util_gl.h"

/// equivalent to glutWireSphere()
void ALGO_API geomWireSphere(GLdouble radius, GLint slices, GLint stacks);

/// equivalent to glutSolidSphere()
void ALGO_API geomSolidSphere(GLdouble radius, GLint slices, GLint stacks);

/// equivalent to glutWireCube()
void ALGO_API geomWireCube(GLdouble size);

/// equivalent to glutSolidCube()
void ALGO_API geomSolidCube(GLdouble size);

#ifdef WITHOUT_GLUT
//#include <GL/glut.h>
//#else

/// redirect glutWireSphere symbol for not changing code
#define glutWireSphere geomWireSphere
/// redirect glutSolidSphere symbol for not changing code
#define glutSolidSphere geomSolidSphere
/// redirect glutWireCube symbol for not changing code
#define glutWireCube geomWireCube
/// redirect glutSolidCube symbol for not changing code
#define glutSolidCube geomSolidCube

#endif

#endif // __util_glut_h_
