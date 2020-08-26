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

#define GL_SILENCE_DEPRECATION

#ifndef __util_gl_h__
#define __util_gl_h__

/*! \file util_gl.h
    \brief File that contains GL command for GEOM type and Data Structure.
*/

/* ----------------------------------------------------------------------- */

#include "../algo_config.h"

#ifndef PGL_WITHOUT_QT

#include <QtOpenGL/qgl.h>
#ifdef QT_NO_OPENGL
#pragma message "Qt compiled without openGL support"
#endif

#else
#if defined(__APPLE__)
# include <OpenGL/gl.h>
#else
# include <GL/gl.h>
#endif

#endif

#if defined(__APPLE__)
# include <OpenGL/glu.h>
#else
# include <GL/glu.h>
#endif


#ifndef PGL_WITHOUT_QT
#include <QtCore/qpoint.h>
#include <QtCore/qrect.h>
#endif

#include <plantgl/math/util_math.h>
#include <plantgl/math/util_vector.h>
#include <plantgl/math/util_matrix.h>
#include <plantgl/tool/util_assert.h>

/* ----------------------------------------------------------------------- */

#ifdef PGL_USE_DOUBLE  // For Double Config

#define GL_GEOM_REAL GL_DOUBLE
/* ----------------------------------------------------------------------- */

/// glVertex for GEOM
inline void glGeomVertex(const real_t& vx,
                         const real_t& vy)
{ glVertex2d(vx,vy);       }
/// glVertex for GEOM

inline void glGeomVertex(const real_t& vx,
                         const real_t& vy,
                         const real_t& vz)
{ glVertex3d(vx,vy,vz);    }

/// glVertex for GEOM
inline void glGeomVertex(const real_t& vx,
                         const real_t& vy,
                         const real_t& vz,
                         const real_t& vw)
{ glVertex4d(vx,vy,vz,vw); }

/// glNormal for GEOM
inline void glGeomNormal(const real_t& vx,
                         const real_t& vy,
                         const real_t& vz)
{ glNormal3d(vx,vy,vz);    }

/// glTexCoord for GEOM
inline void glGeomTexCoord(const real_t& vx )
{ glTexCoord1d(vx);    }

/// glTexCoord for GEOM
inline void glGeomTexCoord(const real_t& vx,
                           const real_t& vy)
{ glTexCoord2d(vx,vy);    }

/// glTexCoord for GEOM
inline void glGeomTexCoord(const real_t& vx,
                           const real_t& vy,
                           const real_t& vz)
{ glTexCoord3d(vx,vy,vz);    }

/// glTexCoord for GEOM
inline void glGeomTexCoord(const real_t& vx,
                           const real_t& vy,
                           const real_t& vz,
                           const real_t& vw)
{ glTexCoord4d(vx,vy,vz,vw);    }

/* ----------------------------------------------------------------------- */

/// glTranslate for GEOM
inline void glGeomTranslate(const real_t& vx,
                            const real_t& vy,
                            const real_t& vz)
{ glTranslated(vx,vy,vz); }

/// glScale for GEOM
inline void glGeomScale(const real_t& v)
{ glScaled(v,v,v); }

/// glScale for GEOM
inline void glGeomScale(const real_t& vx,
                        const real_t& vy,
                        const real_t& vz)
{ glScaled(vx,vy,vz); }

/// glRotate for GEOM
inline void glGeomRotate(const real_t& vx,
                         const real_t& vy,
                         const real_t& vz,
                         const real_t& angle)
{ glRotated(angle,vx,vy,vz); }

/// glRotate for GEOM with angle in radians
inline void glGeomRadRotate(const real_t& vx,
                            const real_t& vy,
                            const real_t& vz,
                            const real_t& angle)
{ glRotated(angle*GEOM_DEG,vx,vy,vz); }

/// gl command for EulerRotate around Z-axis(azimuth), then Y-axis(elevation), and X-axis(roll) with angles in degree for GEOM
inline void glGeomEulerRotateZYX(const real_t& azimuth,
                                 const real_t& elevation,
                                 const real_t& roll)
{ glRotated(azimuth,   0.0,0.0,1.0);
  glRotated(elevation, 0.0,1.0,0.0);
  glRotated(roll,      1.0,0.0,0.0); }

/// gl command for EulerRotate around Z-axis(azimuth), then Y-axis(elevation), and X-axis(roll)  with angles in radians for GEOM
inline void glGeomRadEulerRotateZYX(const real_t& azimuth,
                                    const real_t& elevation,
                                    const real_t& roll)
{ glRotated(azimuth  *GEOM_DEG, 0.0,0.0,1.0);
  glRotated(elevation*GEOM_DEG, 0.0,1.0,0.0);
  glRotated(roll     *GEOM_DEG, 1.0,0.0,0.0); }

/// gl command for EulerRotate around X-axis(azimuth), then Y-axis(elevation), and Z-axis(roll) with angles in degree for GEOM
inline void glGeomEulerRotateXYZ(const real_t& azimuth,
                                 const real_t& elevation,
                                 const real_t& roll)
{ glRotated(azimuth,   1.0,0.0,0.0);
  glRotated(elevation, 0.0,1.0,0.0);
  glRotated(roll,      0.0,0.0,1.0); }

/// gl command for EulerRotate around X-axis(azimuth), then Y-axis(elevation), and Z-axis(roll)  with angles in radians for GEOM
inline void glGeomRadEulerRotateXYZ(const real_t& azimuth,
                                    const real_t& elevation,
                                    const real_t& roll)
{ glRotated(azimuth  *GEOM_DEG, 1.0,0.0,0.0);
  glRotated(elevation*GEOM_DEG, 0.0,1.0,0.0);
  glRotated(roll     *GEOM_DEG, 0.0,0.0,1.0); }

/* ----------------------------------------------------------------------- */

/// glVertex for GEOM
inline void glGeomVertex(const PGL(Vector2)& v)
{ glVertex2dv(v.begin()); }

/// glVertex for GEOM
inline void glGeomVertex(const PGL(Vector3)& v)
{ glVertex3dv(v.begin()); }

/// glVertex for GEOM
inline void glGeomVertex(const PGL(Vector4)& v)
{ glVertex4dv(v.begin()); }

/// glNormal for GEOM
inline void glGeomNormal(const PGL(Vector3)& v)
{ glNormal3dv(v.begin()); }

/// glTexCoord for GEOM
inline void glGeomTexCoord(const PGL(Vector2)& v)
{ glTexCoord2dv(v.begin()); }

/// glVertex for GEOM
inline void glGeomTexCoord(const PGL(Vector3)& v)
{ glTexCoord3dv(v.begin()); }

/// glVertex for GEOM
inline void glGeomTexCoord(const PGL(Vector4)& v)
{ glTexCoord4dv(v.begin()); }

/* ----------------------------------------------------------------------- */

/// glTranslate for GEOM
inline void glGeomTranslate(const PGL(Vector3)& v)
{ glTranslated(v.x(),v.y(),v.z()); }

/// glScale for GEOM
inline void glGeomScale(const PGL(Vector3)& v)
{ glScaled(v.x(),v.y(),v.z()); }

/// glRotate with angle in degree for GEOM
inline void glGeomRotate(const PGL(Vector3)& v,
                         const real_t& angle)
{ glRotated(angle,v.x(),v.y(),v.z()); }

/// glRotate with angle in radians for GEOM
inline void glGeomRadRotate(const PGL(Vector3)& v,
                            const real_t& angle)
{ glRotated(angle*GEOM_DEG,v.x(),v.y(),v.z()); }


/// gl command for EulerRotate around Z-axis(v.x()), then Y-axis(v.y()), and X-axis(v.z()) with angles in degree for GEOM
inline void glGeomEulerRotateZYX(const PGL(Vector3)& v)
{ glRotated(v.x(), 0.0,0.0,1.0);
  glRotated(v.y(), 0.0,1.0,0.0);
  glRotated(v.z(), 1.0,0.0,0.0); }

/// gl command for EulerRotate around Z-axis(v.x()), then Y-axis(v.y()), and X-axis(v.z()) with angles in radians for GEOM
inline void glGeomRadEulerRotateZYX(const PGL(Vector3)& v)
{ glRotated(v.x()*GEOM_DEG, 0.0,0.0,1.0);
  glRotated(v.y()*GEOM_DEG, 0.0,1.0,0.0);
  glRotated(v.z()*GEOM_DEG, 1.0,0.0,0.0);}

/// gl command for EulerRotate around X-axis(v.x()), then Y-axis(v.y()), and Z-axis(v.z()) with angles in degree for GEOM
inline void glGeomEulerRotateXYZ(const PGL(Vector3)& v)
{ glRotated(v.x(), 1.0,0.0,0.0);
  glRotated(v.y(), 0.0,1.0,0.0);
  glRotated(v.z(), 0.0,0.0,1.0); }

/// gl command for EulerRotate around X-axis(v.x()), then Y-axis(v.y()), and Z-axis(v.z()) with angles in radians for GEOM
inline void glGeomRadEulerRotateXYZ(const PGL(Vector3)& v)
{ glRotated(v.x()*GEOM_DEG, 1.0,0.0,0.0);
  glRotated(v.y()*GEOM_DEG, 0.0,1.0,0.0);
  glRotated(v.z()*GEOM_DEG, 0.0,0.0,1.0);}

/* ----------------------------------------------------------------------- */

/// glMultMatrix for GEOM
inline void glGeomMultMatrix(const PGL(Matrix4)& m)
{ glMultMatrixd(PGL(transpose)(m).getData()); }

/// glMultMatrix for GEOM
inline void glGeomMultMatrix(const PGL(Matrix3)& m)
{ PGL(Matrix4) m2(m);glMultMatrixd(PGL(transpose)(m2).getData()); }

/// glLoadMatrix for GEOM
inline void glGeomLoadMatrix(const PGL(Matrix4)& m)
{ glLoadMatrixd(PGL(transpose)(m).getData()); }

/// glLoadMatrix for GEOM
inline void glGeomLoadMatrix(const PGL(Matrix3)& m)
{ PGL(Matrix4) m2(m);glLoadMatrixd(PGL(transpose)(m2).getData()); }

/// glGetMatrix for GEOM, pname must be GL_MODELVIEW_MATRIX, GL_PROJECTION_MATRIX, GL_TEXTURE_MATRIX
inline void glGeomGetMatrix(GLenum pname,PGL(Matrix4)& m)
{ glGetDoublev(pname,m.getData()); m = PGL(transpose)(m); }

/* ----------------------------------------------------------------------- */

/// glLight(GL_POSITION) for GEOM
inline void glGeomLightPosition(GLenum light, const PGL(Vector4)& v )
{ GLfloat val[] = {static_cast<GLfloat>(v.x()),
                   static_cast<GLfloat>(v.y()),
                   static_cast<GLfloat>(v.z()),
                   static_cast<GLfloat>(v.w()) };
  glLightfv (light, GL_POSITION, val); }

/// glLight(GL_POSITION) for GEOM
inline void glGeomLightPosition(GLenum light, const PGL(Vector3)& v )
{ GLfloat val[] = { static_cast<GLfloat>(v.x()), static_cast<GLfloat>(v.y()), static_cast<GLfloat>(v.z()), 0.0 };
  glLightfv (light, GL_POSITION, val); }

/// glLight(GL_SPOT_DIRECTION) for GEOM
inline void glGeomLightDirection(GLenum light, const PGL(Vector3)& v )
{ GLfloat val[] = { static_cast<GLfloat>(v.x()), static_cast<GLfloat>(v.y()), static_cast<GLfloat>(v.z()) };
  glLightfv (light, GL_SPOT_DIRECTION, val); }

/// glGetLight(GL_POSITION) for GEOM
inline void glGeomGetLightPosition(GLenum light, PGL(Vector4)& v )
{ GLfloat val[] = { 0.0 , 0.0 , 0.0 , 0.0 };
  glGetLightfv (light, GL_POSITION, val);
  v.x() = val[0]; v.y() = val[1];
  v.z() = val[2]; v.w() = val[3];}

/// glGetLight(GL_SPOT_DIRECTION) for GEOM
inline void glGeomGetLightDirection(GLenum light, PGL(Vector3)& v )
{ GLfloat val[] = { 0.0 , 0.0 , 0.0 };
  glGetLightfv (light, GL_SPOT_DIRECTION, val);
  v.x() = val[0]; v.y() = val[1]; v.z() = val[2]; }


/* ----------------------------------------------------------------------- */

#else // For Float Config

/* ----------------------------------------------------------------------- */

#define GL_GEOM_REAL GL_FLOAT

/// glVertex for GEOM
inline void glGeomVertex(const real_t& vx,
                         const real_t& vy)
{ glVertex2f(vx,vy);       }

/// glVertex for GEOM
inline void glGeomVertex(const real_t& vx,
                         const real_t& vy,
                         const real_t& vz)
{ glVertex3f(vx,vy,vz);    }

/// glVertex for GEOM
inline void glGeomVertex(const real_t& vx,
                         const real_t& vy,
                         const real_t& vz,
                         const real_t& vw)
{ glVertex4f(vx,vy,vz,vw); }

/// glNormal for GEOM
inline void glGeomNormal(const real_t& vx,
                         const real_t& vy,
                         const real_t& vz)
{ glNormal3f(vx,vy,vz);    }

/// glTexCoord for GEOM
inline void glGeomTexCoord(const real_t& vx)
{ glTexCoord1f(vx);       }

/// glTexCoord for GEOM
inline void glGeomTexCoord(const real_t& vx,
                         const real_t& vy)
{ glTexCoord2f(vx,vy);       }

/// glTexCoord for GEOM
inline void glGeomTexCoord(const real_t& vx,
                         const real_t& vy,
                         const real_t& vz)
{ glTexCoord3f(vx,vy,vz);    }

/// glTexCoord for GEOM
inline void glGeomTexCoord(const real_t& vx,
                         const real_t& vy,
                         const real_t& vz,
                         const real_t& vw)
{ glTexCoord4f(vx,vy,vz,vw); }


/* ----------------------------------------------------------------------- */

/// glTranslate for GEOM
inline void glGeomTranslate(const real_t& vx,
                            const real_t& vy,
                            const real_t& vz)
{ glTranslatef(vx,vy,vz); }

/// glScale for GEOM
inline void glGeomScale(const real_t& v)
{ glScalef(v,v,v); }

/// glScale for GEOM
inline void glGeomScale(const real_t& vx,
                        const real_t& vy,
                        const real_t& vz)
{ glScalef(vx,vy,vz); }

/// glRotate for GEOM
inline void glGeomRotate(const real_t& vx,
                         const real_t& vy,
                         const real_t& vz,
                         const real_t& angle)
{ glRotatef(angle,vx,vy,vz); }

/// glRotate for GEOM with angle in radians
inline void glGeomRadRotate(const real_t& vx,
                            const real_t& vy,
                            const real_t& vz,
                            const real_t& angle)
{ glRotatef(angle*GEOM_DEG,vx,vy,vz); }

/// gl command for EulerRotate around Z-axis(azimuth), then Y-axis(elevation), and X-axis(roll) with angles in degree for GEOM
inline void glGeomEulerRotateZYX(const real_t& azimuth,
                                 const real_t& elevation,
                                 const real_t& roll)
{ glRotatef(azimuth,   0.0,0.0,1.0);
  glRotatef(elevation, 0.0,1.0,0.0);
  glRotatef(roll,      1.0,0.0,0.0); }

/// gl command for EulerRotate around Z-axis(azimuth), then Y-axis(elevation), and X-axis(roll) with angles in radians for GEOM
inline void glGeomRadEulerRotateZYX(const real_t& azimuth,
                                    const real_t& elevation,
                                    const real_t& roll)
{ glRotatef(azimuth  *GEOM_DEG, 0.0,0.0,1.0);
  glRotatef(elevation*GEOM_DEG, 0.0,1.0,0.0);
  glRotatef(roll     *GEOM_DEG, 1.0,0.0,0.0); }

/// gl command for EulerRotate around X-axis(azimuth), then Y-axis(elevation), and Z-axis(roll) with angles in degree for GEOM
inline void glGeomEulerRotateXYZ(const real_t& azimuth,
                                 const real_t& elevation,
                                 const real_t& roll)
{ glRotatef(azimuth,   1.0,0.0,0.0);
  glRotatef(elevation, 0.0,1.0,0.0);
  glRotatef(roll,      0.0,0.0,1.0); }

/// gl command for EulerRotate around X-axis(azimuth), then Y-axis(elevation), and Z-axis(roll) with angles in radians for GEOM
inline void glGeomRadEulerRotateXYZ(const real_t& azimuth,
                                    const real_t& elevation,
                                    const real_t& roll)
{ glRotatef(azimuth  *GEOM_DEG, 1.0,0.0,0.0);
  glRotatef(elevation*GEOM_DEG, 0.0,1.0,0.0);
  glRotatef(roll     *GEOM_DEG, 0.0,0.0,1.0); }

/* ----------------------------------------------------------------------- */

/// glVertex for GEOM
inline void glGeomVertex(const PGL(Vector2)& v)
{ glVertex2fv(v.begin()); }

/// glVertex for GEOM
inline void glGeomVertex(const PGL(Vector3)& v)
{ glVertex3fv(v.begin()); }

/// glVertex for GEOM
inline void glGeomVertex(const PGL(Vector4)& v)
{ glVertex4fv(v.begin()); }

/// glNormal for GEOM
inline void glGeomNormal(const PGL(Vector3)& v)
{ glNormal3fv(v.begin()); }

/// glTexCoord for GEOM
inline void glGeomTexCoord(const PGL(Vector2)& v)
{ glTexCoord2fv(v.begin()); }

/// glTexCoord for GEOM
inline void glGeomTexCoord(const PGL(Vector3)& v)
{ glTexCoord3fv(v.begin()); }

/// glTexCoord for GEOM
inline void glGeomTexCoord(const PGL(Vector4)& v)
{ glTexCoord4fv(v.begin()); }

/* ----------------------------------------------------------------------- */

/// glTranslate for GEOM
inline void glGeomTranslate(const PGL(Vector3)& v)
{ glTranslatef(v.x(),v.y(),v.z()); }

/// glScale for GEOM
inline void glGeomScale(const PGL(Vector3)& v)
{ glScalef(v.x(),v.y(),v.z()); }

/// glRotate with angle in degree for GEOM
inline void glGeomRotate(const PGL(Vector3)& v,
                         const real_t& angle)
{ glRotatef(angle,v.x(),v.y(),v.z()); }

/// glRotate with angle in radians for GEOM
inline void glGeomRadRotate(const PGL(Vector3)& v,
                            const real_t& angle)
{ glRotatef(angle*GEOM_DEG,v.x(),v.y(),v.z()); }


/// gl command for EulerRotate around Z-axis(v.x()), then Y-axis(v.y()), and X-axis(v.z()) with angles in degree for GEOM
inline void glGeomEulerRotateZYX(const PGL(Vector3)& v)
{ glRotatef(v.x(), 0.0,0.0,1.0);
  glRotatef(v.y(), 0.0,1.0,0.0);
  glRotatef(v.z(), 1.0,0.0,0.0);}

/// gl command for EulerRotate around Z-axis(v.x()), then Y-axis(v.y()), and X-axis(v.z())  with angles in radians for GEOM
inline void glGeomRadEulerRotateZYX(const PGL(Vector3)& v)
{ glRotatef(v.x()*GEOM_DEG, 0.0,0.0,1.0);
  glRotatef(v.y()*GEOM_DEG, 0.0,1.0,0.0);
  glRotatef(v.z()*GEOM_DEG, 1.0,0.0,0.0); }

/// gl command for EulerRotate around X-axis(v.x()), then Y-axis(v.y()), and Z-axis(v.z()) with angles in degree for GEOM
inline void glGeomEulerRotateXYZ(const PGL(Vector3)& v)
{ glRotatef(v.x(), 0.0,0.0,1.0);
  glRotatef(v.y(), 0.0,1.0,0.0);
  glRotatef(v.z(), 1.0,0.0,0.0);}

/// gl command for EulerRotate around X-axis(v.x()), then Y-axis(v.y()), and Z-axis(v.z())  with angles in radians for GEOM
inline void glGeomRadEulerRotateXYZ(const PGL(Vector3)& v)
{ glRotatef(v.x()*GEOM_DEG, 1.0,0.0,0.0);
  glRotatef(v.y()*GEOM_DEG, 0.0,1.0,0.0);
  glRotatef(v.z()*GEOM_DEG, 0.0,0.0,1.0); }

/* ----------------------------------------------------------------------- */

/// glMultMatrix for GEOM
inline void glGeomMultMatrix(const PGL(Matrix4)& m)
//{ glMultMatrixf(PGL(transpose)(m).getData()); }
{ glMultMatrixf(PGL(transpose)( m ).getData()); }

/// glMultMatrix for GEOM
inline void glGeomMultMatrix(const PGL(Matrix3)& m)
{ PGL(Matrix4) m2(m);glMultMatrixf(PGL(transpose)(m2).getData()); }

/// glLoadMatrix for GEOM
inline void glGeomLoadMatrix(const PGL(Matrix4)& m)
{ glLoadMatrixf(PGL(transpose)(m).getData()); }

/// glLoadMatrix for GEOM
inline void glGeomLoadMatrix(const PGL(Matrix3)& m)
{ PGL(Matrix4) m2(m);glLoadMatrixf(PGL(transpose)(m2).getData()); }

/// glGetMatrix for GEOM, pname must be GL_MODELVIEW_MATRIX, GL_PROJECTION_MATRIX, GL_TEXTURE_MATRIX
inline void glGeomGetMatrix(GLenum pname,PGL(Matrix4)& m)
{ glGetFloatv(pname,m.getData()); m = PGL(transpose)(m); }

/* ----------------------------------------------------------------------- */

/// glLight(GL_POSITION) for GEOM
inline void glGeomLightPosition(GLenum light, const PGL(Vector4)& v )
{ glLightfv (light, GL_POSITION, v.begin()); }

/// glLight(GL_POSITION) for GEOM
inline void glGeomLightPosition(GLenum light, const PGL(Vector3)& v )
{ GLfloat val[] = { v.x(), v.y(), v.z(), 0.0 };
  glLightfv (light, GL_POSITION, val); }

/// glLight(GL_SPOT_DIRECTION) for GEOM
inline void glGeomLightDirection(GLenum light, const PGL(Vector3)& v )
{ glLightfv (light, GL_SPOT_DIRECTION, v.begin()); }

/// glGetLight(GL_POSITION) for GEOM
inline void glGeomGetLightPosition(GLenum light, PGL(Vector4)& v )
{ glGetLightfv (light, GL_POSITION, v.begin()); }

/// glGetLight(GL_SPOT_DIRECTION) for GEOM
inline void glGeomGetLightDirection(GLenum light, PGL(Vector3)& v )
{ glGetLightfv (light, GL_SPOT_DIRECTION, v.begin()); }

/* ----------------------------------------------------------------------- */

#endif

/* ----------------------------------------------------------------------- */

inline void glGeomFrustum(const PGL(Vector3)& LowerLeft,const PGL(Vector3)& UpperRigth)
{ glFrustum(LowerLeft.y(), UpperRigth.y(),
            LowerLeft.z(), UpperRigth.z(),
            LowerLeft.x(), UpperRigth.x()); }

/// Transformation of a Vector3 in GL coordinates into a Vector3 in GEOM coordinates.
inline PGL(Vector3) gl2geom(const PGL(Vector3)& v){
    return PGL(Vector3)(v.z(),v.x(),v.y());
}

/// Transformation of a Vector3 in GEOM coordinates into a Vector3 in GL coordinates.
inline PGL(Vector3) geom2gl(const PGL(Vector3)& v){
    return PGL(Vector3)(v.y(),v.z(),v.x());
}

/// Transformation of a Vector4 in GL coordinates into a Vector4 in GEOM coordinates.
inline PGL(Vector4) gl2geom(const PGL(Vector4)& v){
    return PGL(Vector4)(v.z(),v.x(),v.y(),v.w());
}

/// Transformation of a Vector4 in GEOM coordinates into a Vector4 in GL coordinates.
inline PGL(Vector4) geom2gl(const PGL(Vector4)& v){
    return PGL(Vector4)(v.y(),v.z(),v.x(),v.w());
}

#ifndef PGL_WITHOUT_QT

inline void glQPoint(const QPoint& p){
  glVertex2i(p.x(),p.y());
}

inline void glQRect(const QRect& r){
  glRecti(r.topLeft().x(),r.topLeft().y(),r.bottomRight().x(),r.bottomRight().y());
}

#endif
/* ----------------------------------------------------------------------- */

#endif
