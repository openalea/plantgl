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
inline QString toQString(const PGL(Vector4)& a){
  return'<'+QString::number(a.x())+
    ','+QString::number(a.y())+
    ','+QString::number(a.z())+
    ','+QString::number(a.w())+'>';
}

/// Transform a Vector3 to QString
inline QString toQString(const PGL(Vector3)& a){
  return'<'+QString::number(a.x())+
    ','+QString::number(a.y())+
    ','+QString::number(a.z())+'>';
}

/// Transform a Vector2 to QString
inline QString toQString(const PGL(Vector2)& a){
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

