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

#ifndef __util_appegl_h__
#define __util_appegl_h__

/*! \file util_appegl.h
    \brief File that contains GL command for GEOM and Qt color.
*/

/* ----------------------------------------------------------------------- */

#include "util_gl.h"
#include <plantgl/scenegraph/appearance/color.h>
#include <QtGui/QColor>

/* ----------------------------------------------------------------------- */

/*! \fn void glGeomLightMaterial(GLenum light, GLenum pname, const PGL(Color3)& v, const real_t& alpha = 1.0 )
    \brief  glLight for GEOM
    \param light must be GL_LIGHT0, ... , GL_LIGHT7.
    \param pname must be GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR.
    \param v must be a valid color.
    \param alpha must be transparency value in [0..1].
*/
inline void glGeomLightMaterial(GLenum light, GLenum pname, const PGL(Color3)& v, const real_t& alpha = 1.0 )
{ GEOM_ASSERT(pname == GL_AMBIENT || pname == GL_DIFFUSE || pname == GL_SPECULAR );
  GLfloat val[] = { v.getRedClamped(), v.getGreenClamped(), v.getBlueClamped(), alpha };
  glLightfv (light, pname, val); }

/*! \fn void glGeomLightMaterial(GLenum light, GLenum pname, const PGL(Color4)& v  )
    \brief  glLight for GEOM
    \param light must be GL_LIGHT0, ... , GL_LIGHT7.
    \param pname must be GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR.
    \param v must be a valid color.
*/
inline void glGeomLightMaterial(GLenum light, GLenum pname, const PGL(Color4)& v )
{ GEOM_ASSERT(pname == GL_AMBIENT || pname == GL_DIFFUSE || pname == GL_SPECULAR );
  GLfloat val[] = { v.getRedClamped(), v.getGreenClamped(), v.getBlueClamped(), v.getAlphaClamped() };
  glLightfv (light, pname, val); }

/*! \fn void glGeomLightMaterial(GLenum light, GLenum pname, const QColor& v  )
    \brief  glLight for GEOM
    \param light must be GL_LIGHT0, ... , GL_LIGHT7.
    \param pname must be GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR.
    \param v must be a valid color.
*/
inline void glGeomLightMaterial(GLenum light, GLenum pname, const QColor& v )
{ GEOM_ASSERT(pname == GL_AMBIENT || pname == GL_DIFFUSE || pname == GL_SPECULAR );
  QRgb rgb = v.rgb();
  GLfloat val[] = { (GLfloat)(qRed(rgb))/255.0,  (GLfloat)(qGreen(rgb))/255.0,
                                        (GLfloat)(qBlue(rgb))/255.0, (GLfloat)(qAlpha(rgb))/255.0 };
  glLightfv (light, pname, val); }

/*! \fn void glGeomLightMaterial(GLenum light, GLenum pname, const QRgb& v  )
    \brief  glLight for GEOM
    \param light must be GL_LIGHT0, ... , GL_LIGHT7.
    \param pname must be GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR.
    \param v must be a valid color.
*/
inline void glGeomLightMaterial(GLenum light, GLenum pname, const QRgb& rgb )
{ GEOM_ASSERT(pname == GL_AMBIENT || pname == GL_DIFFUSE || pname == GL_SPECULAR );
  GLfloat val[] = { (GLfloat)(qRed(rgb))/255.0,  (GLfloat)(qGreen(rgb))/255.0,
                                        (GLfloat)(qBlue(rgb))/255.0, (GLfloat)(qAlpha(rgb))/255.0 };
  glLightfv (light, pname, val); }

/*! \fn void glGeomGetLightMaterial(GLenum light, GLenum pname,PGL(Color4)& v )
    \brief  glLight for GEOM
    \param light must be GL_LIGHT0, ... , GL_LIGHT7.
    \param pname must be GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR.
    \param v the returned color.
    \return the color of the ligth.
*/
inline void glGeomGetLightMaterial(GLenum light, GLenum pname,PGL(Color4)& v )
{ GEOM_ASSERT(pname == GL_AMBIENT || pname == GL_DIFFUSE || pname == GL_SPECULAR );
  GLfloat val[] = { 0.0 , 0.0 , 0.0 , 0.0 };
  glGetLightfv (light, pname, val);
  v.getRed() = uchar(val[0]*255);  v.getGreen() = uchar(val[1]*255);
  v.getBlue() = uchar(val[2]*255); v.getAlpha() = uchar(val[3]*255);}

/* ----------------------------------------------------------------------- */

/// glFog( GL_FOG_COLOR ) for GEOM
inline void glGeomFogColor(const PGL(Color4)& v)
{ GLfloat val[] = { v.getRedClamped(), v.getGreenClamped(), v.getBlueClamped(), v.getAlphaClamped() };
  glFogfv ( GL_FOG_COLOR, val); }

/// glFog( GL_FOG_COLOR ) for GEOM
inline void glGeomFogColor(const PGL(Color3)& v, const real_t& alpha = 1.0)
{ GLfloat val[] = { v.getRedClamped(), v.getGreenClamped(), v.getBlueClamped(), alpha };
  glFogfv ( GL_FOG_COLOR, val); }

/// glFog( GL_FOG_COLOR ) for Qt
inline void glGeomFogColor(const QColor& v)
{ QRgb rgb = v.rgb();
  GLfloat val[] = { (GLfloat)(qRed(rgb))/255.0,  (GLfloat)(qGreen(rgb))/255.0,
                                        (GLfloat)(qBlue(rgb))/255.0, (GLfloat)(qAlpha(rgb))/255.0 };
  glFogfv ( GL_FOG_COLOR, val); }

/// glFog( GL_FOG_COLOR ) for Qt
inline void glGeomFogColor(const QRgb& rgb)
{ GLfloat val[] = { (GLfloat)(qRed(rgb)/255.0),  (GLfloat)(qGreen(rgb))/255.0,
                                        (GLfloat)(qBlue(rgb))/255.0, (GLfloat)(qAlpha(rgb))/255.0 };
  glFogfv ( GL_FOG_COLOR, val); }

/* ----------------------------------------------------------------------- */

/// glColor for GEOM
inline void glGeomColor(const PGL(Color3)& v){
  glColor3ubv(v.begin());
}

/// glColor for GEOM
inline void glGeomColor(const PGL(Color3)& v,const uchar_t& alpha){
  glColor4ub(v.getRed(),v.getGreen(),v.getBlue(),alpha);
}

/// glColor for GEOM
inline void glGeomColor(const PGL(Color4)& v){
  glColor4ubv(v.begin());
}

/// glColor for Qt
inline void glGeomColor(const QColor& v)
{ QRgb rgb = v.rgb();
  GLfloat val[] = { (GLfloat)(qRed(rgb))/255.0,  (GLfloat)(qGreen(rgb))/255.0,
                                        (GLfloat)(qBlue(rgb))/255.0, (GLfloat)(qAlpha(rgb))/255.0 };
   glColor4fv( val ); }

/// glColor for Qt
inline void glGeomColor(const QRgb& rgb )
{ GLfloat val[] = { (GLfloat)(qRed(rgb))/255.0,  (GLfloat)(qGreen(rgb))/255.0,
                                        (GLfloat)(qBlue(rgb))/255.0, (GLfloat)(qAlpha(rgb))/255.0 };
   glColor4fv( val ); }

/// glMaterial for GEOM
inline void glGeomMaterial(GLenum face, GLenum pname, const PGL(Color4)& v)
{ GLfloat val[] = { v.getRedClamped(), v.getGreenClamped(), v.getBlueClamped(), v.getAlphaClamped() };
  glMaterialfv (face, pname, val);}

/// glMaterial for GEOM
inline void glGeomMaterial(GLenum face, GLenum pname, const PGL(Color3)& v, const real_t& alpha = 1.0)
{ GLfloat val[] = { v.getRedClamped(), v.getGreenClamped(), v.getBlueClamped(), alpha };
  glMaterialfv (face, pname, val);}

/// glMaterial for Qt
inline void glGeomMaterial(GLenum face, GLenum pname,const QColor& v)
{ QRgb rgb = v.rgb();
  GLfloat val[] = { (GLfloat)(qRed(rgb))/255.0,  (GLfloat)(qGreen(rgb))/255.0,
                                        (GLfloat)(qBlue(rgb))/255.0, (GLfloat)(qAlpha(rgb))/255.0 };
  glMaterialfv (face, pname, val);}

/// glMaterial for Qt
inline void glGeomMaterial(GLenum face, GLenum pname,const QRgb& rgb)
{ GLfloat val[] = { (GLfloat)(qRed(rgb))/255.0,  (GLfloat)(qGreen(rgb))/255.0,
                                        (GLfloat)(qBlue(rgb))/255.0, (GLfloat)(qAlpha(rgb))/255.0 };
  glMaterialfv (face, pname, val);}

/* ----------------------------------------------------------------------- */


#endif
