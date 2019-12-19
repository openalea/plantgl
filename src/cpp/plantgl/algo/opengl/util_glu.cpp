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
