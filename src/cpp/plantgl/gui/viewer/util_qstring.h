/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
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

/*! \file util_qstring.h
    \brief File that convert GEOM Basic type in QString .
*/

#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/appearance/color.h>
#include <QtCore/qstring.h>

/// Transform a Color3 to QString
inline QString toQString(const PGL(Color3)& a){
  if(a == PGL(Color3)::BLACK)  return QString("Black");
  if(a == PGL(Color3)::BLUE)   return QString("Blue");
  if(a == PGL(Color3)::CYAN)   return QString("Cyan");
  if(a == PGL(Color3)::GREEN)  return QString("Green");
  if(a == PGL(Color3)::MAGENTA)return QString("Magenta");
  if(a == PGL(Color3)::RED)    return QString("Red");
  if(a == PGL(Color3)::WHITE)  return QString("White");
  if(a == PGL(Color3)::YELLOW) return QString("Yellow");
  return'<'+QString::number(int(a.getRed()))+
    ','+QString::number(int(a.getGreen()))+
    ','+QString::number(int(a.getBlue()))+'>';
}

inline QString toQString(const PGL(Color4)& a){
  if(a == PGL(Color4)::BLACK)  return QString("Black");
  if(a == PGL(Color4)::BLUE)   return QString("Blue");
  if(a == PGL(Color4)::CYAN)   return QString("Cyan");
  if(a == PGL(Color4)::GREEN)  return QString("Green");
  if(a == PGL(Color4)::MAGENTA)return QString("Magenta");
  if(a == PGL(Color4)::RED)    return QString("Red");
  if(a == PGL(Color4)::WHITE)  return QString("White");
  if(a == PGL(Color4)::YELLOW) return QString("Yellow");
  return'<'+QString::number(int(a.getRed()))+
    ','+QString::number(int(a.getGreen()))+
    ','+QString::number(int(a.getBlue()))+
    ','+QString::number(int(a.getAlpha()))+'>';
}

/// Transform a Vector4 to QString
inline QString toQString(const TOOLS(Vector4)& a){
  return'<'+QString::number(a.x())+
    ','+QString::number(a.y())+
    ','+QString::number(a.z())+
    ','+QString::number(a.w())+'>';
}

/// Transform a Vector3 to QString
inline QString toQString(const TOOLS(Vector3)& a){
  return'<'+QString::number(a.x())+
    ','+QString::number(a.y())+
    ','+QString::number(a.z())+'>';
}

/// Transform a Vector2 to QString
inline QString toQString(const TOOLS(Vector2)& a){
  return'<'+QString::number(a.x())+
    ','+QString::number(a.y())+'>';
}

/// Transform a boolean to QString
inline QString toQString(bool a){
  return QString((a?"True":"False"));
}


/// Transform an Index3 to QString
inline QString toQString(const PGL(Index3)& a){
  return'['+QString::number(a.getAt(0))+
    ','+QString::number(a.getAt(1))+
    ','+QString::number(a.getAt(2))+']';
}

/// Transform an Index4 to QString
inline QString toQString(const PGL(Index4)& a){
  return'<'+QString::number(a.getAt(0))+
    ','+QString::number(a.getAt(1))+
    ','+QString::number(a.getAt(2))+
    ','+QString::number(a.getAt(3))+'>';
}

/// Transform an Index3 to QString
inline QString toQString(const PGL(Index)& a){
	QString res = "<";
	for(uint_t i = 0 ; i < a.size()-1 ; i++)
		res += QString::number(a.getAt(i))+',';
	return res+QString::number(a.getAt(a.size()-1))+'>';
}

/// Transform a real to QString
inline QString toQString(real_t a){
	return QString::number(a);
}

