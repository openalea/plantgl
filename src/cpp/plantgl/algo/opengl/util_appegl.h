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


#ifndef __util_appegl_h__
#define __util_appegl_h__

/*! \file util_appegl.h
    \brief File that contains GL command for GEOM and Qt color.
*/

/* ----------------------------------------------------------------------- */

#include "util_gl.h"
#include <plantgl/scenegraph/appearance/color.h>
#ifndef PGL_WITHOUT_QT
#include <QtGui/QColor>
#endif

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
  GLfloat val[] = { (GLfloat)v.getRedClamped(), (GLfloat)v.getGreenClamped(), (GLfloat)v.getBlueClamped(), (GLfloat)alpha };
  glLightfv (light, pname, val); }

/*! \fn void glGeomLightMaterial(GLenum light, GLenum pname, const PGL(Color4)& v  )
    \brief  glLight for GEOM
    \param light must be GL_LIGHT0, ... , GL_LIGHT7.
    \param pname must be GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR.
    \param v must be a valid color.
*/
inline void glGeomLightMaterial(GLenum light, GLenum pname, const PGL(Color4)& v )
{ GEOM_ASSERT(pname == GL_AMBIENT || pname == GL_DIFFUSE || pname == GL_SPECULAR );
  GLfloat val[] = { (GLfloat)v.getRedClamped(), (GLfloat)v.getGreenClamped(), (GLfloat)v.getBlueClamped(), (GLfloat)v.getAlphaClamped() };
  glLightfv (light, pname, val); }

#ifndef PGL_WITHOUT_QT
/*! \fn void glGeomLightMaterial(GLenum light, GLenum pname, const QColor& v  )
    \brief  glLight for GEOM
    \param light must be GL_LIGHT0, ... , GL_LIGHT7.
    \param pname must be GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR.
    \param v must be a valid color.
*/
inline void glGeomLightMaterial(GLenum light, GLenum pname, const QColor& v )
{ GEOM_ASSERT(pname == GL_AMBIENT || pname == GL_DIFFUSE || pname == GL_SPECULAR );
  QRgb rgb = v.rgb();
  GLfloat val[] = { (GLfloat)(qRed(rgb))/(GLfloat)255.0,  (GLfloat)(qGreen(rgb))/(GLfloat)255.0,
                                        (GLfloat)(qBlue(rgb))/(GLfloat)255.0, (GLfloat)(qAlpha(rgb))/(GLfloat)255.0 };
  glLightfv (light, pname, val); }

/*! \fn void glGeomLightMaterial(GLenum light, GLenum pname, const QRgb& v  )
    \brief  glLight for GEOM
    \param light must be GL_LIGHT0, ... , GL_LIGHT7.
    \param pname must be GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR.
    \param v must be a valid color.
*/
inline void glGeomLightMaterial(GLenum light, GLenum pname, const QRgb& rgb )
{ GEOM_ASSERT(pname == GL_AMBIENT || pname == GL_DIFFUSE || pname == GL_SPECULAR );
  GLfloat val[] = { (GLfloat)(qRed(rgb))/(GLfloat)255.0,  (GLfloat)(qGreen(rgb))/(GLfloat)255.0,
                                        (GLfloat)(qBlue(rgb))/(GLfloat)255.0, (GLfloat)(qAlpha(rgb))/(GLfloat)255.0 };
  glLightfv (light, pname, val); }
#endif

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
  v.getRed() = uchar_t(val[0]*255);  v.getGreen() = uchar_t(val[1]*255);
  v.getBlue() = uchar_t(val[2]*255); v.getAlpha() = uchar_t(val[3]*255);}

/* ----------------------------------------------------------------------- */

/// glFog( GL_FOG_COLOR ) for GEOM
inline void glGeomFogColor(const PGL(Color4)& v)
{ GLfloat val[] = { (GLfloat)v.getRedClamped(), (GLfloat)v.getGreenClamped(), (GLfloat)v.getBlueClamped(), (GLfloat)v.getAlphaClamped() };
  glFogfv ( GL_FOG_COLOR, val); }

/// glFog( GL_FOG_COLOR ) for GEOM
inline void glGeomFogColor(const PGL(Color3)& v, const real_t& alpha = 1.0)
{ GLfloat val[] = { (GLfloat)v.getRedClamped(), (GLfloat)v.getGreenClamped(), (GLfloat)v.getBlueClamped(), (GLfloat)alpha };
  glFogfv ( GL_FOG_COLOR, val); }

#ifndef PGL_WITHOUT_QT
/// glFog( GL_FOG_COLOR ) for Qt
inline void glGeomFogColor(const QColor& v)
{ QRgb rgb = v.rgb();
  GLfloat val[] = { (GLfloat)(qRed(rgb))/(GLfloat)255.0,  (GLfloat)(qGreen(rgb))/(GLfloat)255.0,
                                        (GLfloat)(qBlue(rgb))/(GLfloat)255.0, (GLfloat)(qAlpha(rgb))/(GLfloat)255.0 };
  glFogfv ( GL_FOG_COLOR, val); }

/// glFog( GL_FOG_COLOR ) for Qt
inline void glGeomFogColor(const QRgb& rgb)
{ GLfloat val[] = { (GLfloat)(qRed(rgb)/(GLfloat)255.0),  (GLfloat)(qGreen(rgb))/(GLfloat)255.0,
                                        (GLfloat)(qBlue(rgb))/(GLfloat)255.0, (GLfloat)(qAlpha(rgb))/(GLfloat)255.0 };
  glFogfv ( GL_FOG_COLOR, val); }
#endif

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

#ifndef PGL_WITHOUT_QT
/// glColor for Qt
inline void glGeomColor(const QColor& v)
{ QRgb rgb = v.rgb();
  GLfloat val[] = { (GLfloat)(qRed(rgb))/(GLfloat)255.0,  (GLfloat)(qGreen(rgb))/(GLfloat)255.0,
                                        (GLfloat)(qBlue(rgb))/(GLfloat)255.0, (GLfloat)(qAlpha(rgb))/(GLfloat)255.0 };
   glColor4fv( val ); }

/// glColor for Qt
inline void glGeomColor(const QRgb& rgb )
{ GLfloat val[] = { (GLfloat)(qRed(rgb))/(GLfloat)255.0,  (GLfloat)(qGreen(rgb))/(GLfloat)255.0,
                                        (GLfloat)(qBlue(rgb))/(GLfloat)255.0, (GLfloat)(qAlpha(rgb))/(GLfloat)255.0 };
   glColor4fv( val ); }
#endif

/// glMaterial for GEOM
inline void glGeomMaterial(GLenum face, GLenum pname, const PGL(Color4)& v)
{ GLfloat val[] = { (GLfloat)v.getRedClamped(), (GLfloat)v.getGreenClamped(), (GLfloat)v.getBlueClamped(), (GLfloat)v.getAlphaClamped() };
  glMaterialfv (face, pname, val);}

/// glMaterial for GEOM
inline void glGeomMaterial(GLenum face, GLenum pname, const PGL(Color3)& v, const real_t& alpha = 1.0)
{ GLfloat val[] = { (GLfloat)v.getRedClamped(), (GLfloat)v.getGreenClamped(), (GLfloat)v.getBlueClamped(), (GLfloat)alpha };
  glMaterialfv (face, pname, val);}

#ifndef PGL_WITHOUT_QT
/// glMaterial for Qt
inline void glGeomMaterial(GLenum face, GLenum pname,const QColor& v)
{ QRgb rgb = v.rgb();
  GLfloat val[] = { (GLfloat)(qRed(rgb))/(GLfloat)255.0,  (GLfloat)(qGreen(rgb))/(GLfloat)255.0,
                                        (GLfloat)(qBlue(rgb))/(GLfloat)255.0, (GLfloat)(qAlpha(rgb))/(GLfloat)255.0 };
  glMaterialfv (face, pname, val);}

/// glMaterial for Qt
inline void glGeomMaterial(GLenum face, GLenum pname,const QRgb& rgb)
{ GLfloat val[] = { (GLfloat)(qRed(rgb))/(GLfloat)255.0,  (GLfloat)(qGreen(rgb))/(GLfloat)255.0,
                                        (GLfloat)(qBlue(rgb))/(GLfloat)255.0, (GLfloat)(qAlpha(rgb))/(GLfloat)255.0 };
  glMaterialfv (face, pname, val);}
#endif
/* ----------------------------------------------------------------------- */


#endif
