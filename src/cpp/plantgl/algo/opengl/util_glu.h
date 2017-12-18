/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP
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

#ifndef __util_glu_h__
#define __util_glu_h__

/*! \file util_glu.h
    \brief File that contains GLU command for GEOM type and Data Structure.
*/

/* ----------------------------------------------------------------------- */
#include "../algo_config.h"

#include "util_gl.h"

#if defined(__APPLE__)
# include <OpenGL/glu.h>
#else
# include <GL/glu.h>
#endif

/* ----------------------------------------------------------------------- */

/*
    A number of GLU function are deprecated.
*/

/// gluLookAt for GEOM
ALGO_API void glGeomLookAt(const TOOLS(Vector3)& eye, const TOOLS(Vector3)& center, const TOOLS(Vector3)& up );

/// gluPerspective for GEOM
ALGO_API void  geomPerspective (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

ALGO_API const char * gluGeomErrorString(GLenum error);
ALGO_API const char * gluGeomGetString(GLenum name);

#ifndef PGL_WITHOUT_QT
ALGO_API void  geomPickMatrix (const QRect& region);
ALGO_API void geomPickMatrix (const QPoint& point, GLdouble delta = 2.0);
#endif

ALGO_API bool geomUnProject(GLdouble winX,  GLdouble winY,  GLdouble winZ, GLdouble* objX,  GLdouble* objY,  GLdouble* objZ);
ALGO_API bool geomUnProject(GLdouble winX,  GLdouble winY,  GLdouble winZ, const GLdouble *model, const GLdouble *proj, const GLint *view, GLdouble* objX,  GLdouble* objY,  GLdouble* objZ);
;
/* ----------------------------------------------------------------------- */

#endif
