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

#include "util_glu.h"
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

#ifndef PGL_MIN_MAX
#define PGL_MIN_MAX

template <typename T>
inline const T &pglMin(const T &a, const T &b) { return (a < b) ? a : b; }

template <typename T>
inline const T &pglMax(const T &a, const T &b) { return (a < b) ? b : a; }


#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
#endif

/// gluLookAt for GEOM
void glGeomLookAt(const Vector3& eye, const Vector3& center, const Vector3& up )
{ gluLookAt(eye.x(),    eye.y(),    eye.z(),
            center.x(), center.y(), center.z(),
            up.x(),     up.y(),     up.z()); }

void geomPerspective (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{ gluPerspective (fovy, aspect, zNear, zFar); }

const char *  gluGeomErrorString(GLenum error)
{ return (const char *)gluErrorString(error); }

const char *  gluGeomGetString(GLenum error)
{ return (const char *)gluGetString(error); }


#ifndef PGL_WITHOUT_QT

void geomPickMatrix (const QRect& region){
  GLint viewport[4];
  int x = region.center().x();
  int y = region.center().y();
  glGetIntegerv(GL_VIEWPORT, viewport);
  gluPickMatrix((GLdouble)x,(GLdouble)viewport[3]-y, (GLdouble)pglMax(region.width(),2), (GLdouble)pglMax(region.height(),2) ,viewport);
}

void geomPickMatrix (const QPoint& point, GLdouble delta){
  GLint viewport[4];
  int x = point.x();
  int y = point.y();
  glGetIntegerv(GL_VIEWPORT, viewport);
  gluPickMatrix((GLdouble)x,(GLdouble)viewport[3]-y, delta, delta ,viewport);
}
#endif

bool geomUnProject(GLdouble winX,  GLdouble winY,  GLdouble winZ, GLdouble* objX,  GLdouble* objY,  GLdouble* objZ){
    GLint viewport[4];
    GLdouble modelMatrix[16], projMatrix[16];
    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
    return geomUnProject(winX,winY, winZ, modelMatrix, projMatrix, viewport, objX, objY, objZ);
}

bool geomUnProject(GLdouble winX,  GLdouble winY,  GLdouble winZ,  const GLdouble *model, const GLdouble *proj, const GLint *view,  GLdouble* objX,  GLdouble* objY,  GLdouble* objZ){
    return gluUnProject(winX,winY, winZ, model, proj, view, objX, objY, objZ) == GL_TRUE;
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
