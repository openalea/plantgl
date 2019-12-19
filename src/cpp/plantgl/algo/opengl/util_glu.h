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
ALGO_API void glGeomLookAt(const PGL(Vector3)& eye, const PGL(Vector3)& center, const PGL(Vector3)& up );

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
