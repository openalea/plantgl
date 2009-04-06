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




#include "color.h"
#include <iostream>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/*  --------------------------------------------------------------------- */


#define __RED getAt(0)
#define __GREEN getAt(1)
#define __BLUE getAt(2)
#define __ALPHA getAt(3)

/* ----------------------------------------------------------------------- */


const Color3 Color3::BLACK(0,0,0);
const Color3 Color3::BLUE(0,0,255);
const Color3 Color3::CYAN(0,255,255);
const Color3 Color3::GREEN(0,255,0);
const Color3 Color3::MAGENTA(255,0,255);
const Color3 Color3::RED(255,0,0);
const Color3 Color3::WHITE(255,255,255);
const Color3 Color3::YELLOW(255,255,0);

/* ----------------------------------------------------------------------- */

Color3::Color3( uchar_t r , uchar_t g , uchar_t b ) :
  Tuple3<uchar_t>(r,g,b) {
}

Color3::Color3( uchar_t v ) :
  Tuple3<uchar_t>(v,v,v) {
}

Color3::Color3( const uchar_t * rgb ) :
  Tuple3<uchar_t>(rgb) {
}

Color3::Color3( const Color4& c4) :
	Tuple3<uchar_t>(c4.getRed(),c4.getGreen(),c4.getBlue()) {
}

Color3::Color3( uint32_t rgb ) :
  Tuple3<uchar_t>(uchar_t((rgb & 0xff0000) >> 16),uchar_t((rgb & 0x00ff00) >> 8),uchar_t(rgb & 0x0000ff))
{}

Color3::~Color3(){
}

/* ----------------------------------------------------------------------- */


bool Color3::operator==( const Color3& c ) const {
  return 
    (__RED == c.__RED) && 
    (__GREEN == c.__GREEN) && 
    (__BLUE == c.__BLUE);
}


/* ----------------------------------------------------------------------- */

uchar_t Color3::getBlue( ) const {
  return __BLUE;
}

uchar_t& Color3::getBlue( ) {
  return __BLUE;
}

real_t Color3::getBlueClamped( ) const {
  return (real_t)__BLUE / 255;
} 

uchar_t Color3::getGreen( ) const {
  return __GREEN;
}

uchar_t& Color3::getGreen( ) {
  return __GREEN;
} 

real_t Color3::getGreenClamped( ) const {
  return (real_t)__GREEN / 255;
}

 uchar_t Color3::getRed( ) const {
  return __RED;
}

uchar_t& Color3::getRed( ) {
  return __RED;
}

real_t Color3::getRedClamped( ) const {
  return (real_t)__RED / 255;
}
real_t Color3::getAverage() const {
    return (real_t)(__RED + __GREEN +__BLUE)/(real_t)3;
}

real_t Color3::getAverageClamped() const {
    return (real_t)(__RED + __GREEN +__BLUE)/(real_t)765;
}


uint_t Color3::toUint() const {
	return (uint_t(__RED) << 16) + (uint_t(__GREEN) << 8) + uint_t(__BLUE);
}

Color3 Color3::fromUint(uint_t value) {
  Color3 res;
  res.__RED = uchar_t((value & 0xff0000) >> 16);
  res.__GREEN = uchar_t((value & 0x00ff00) >> 8);
  res.__BLUE = uchar_t(value & 0x0000ff);
  return res;
}

std::ostream& PGL(operator<<( std::ostream& stream, const Color3& c )) {
  return stream << "<" << (uint16_t)c.getRed() << "," << (uint16_t)c.getGreen() << "," << (uint16_t)c.getBlue() << ">";
}

/* ----------------------------------------------------------------------- */


const Color4 Color4::BLACK(0,0,0,0);
const Color4 Color4::BLUE(0,0,255,0);
const Color4 Color4::CYAN(0,255,255,0);
const Color4 Color4::GREEN(0,255,0,0);
const Color4 Color4::MAGENTA(255,0,255,0);
const Color4 Color4::RED(255,0,0,0);
const Color4 Color4::WHITE(255,255,255,0);
const Color4 Color4::YELLOW(255,255,0,0);

/* ----------------------------------------------------------------------- */

Color4::Color4( uchar_t r, uchar_t g , uchar_t b , uchar_t a ) :
  Tuple4<uchar_t>(r,g,b,a) {
}

Color4::Color4( uchar_t v ) :
  Tuple4<uchar_t>(v,v,v,v) {
}

Color4::Color4( const uchar_t * rgba ) :
  Tuple4<uchar_t>(rgba) {
}

Color4::Color4( const Color3& c, uchar_t alpha ) :
  Tuple4<uchar_t>(c.getRed(),c.getGreen(),c.getBlue(),alpha) {
}

Color4::Color4( uint32_t argb ):
  Tuple4<uchar_t>(uchar_t((argb & 0xff0000) >> 16),uchar_t((argb & 0x00ff00) >> 8),
	              uchar_t(argb & 0x0000ff),uchar_t((argb & 0xff000000) >> 24))
{}


Color4::~Color4(  )  {
}

/* ----------------------------------------------------------------------- */


bool Color4::operator==( const Color4& c ) const {
  return 
    (__RED == c.__RED) && 
    (__GREEN == c.__GREEN) && 
    (__BLUE == c.__BLUE) &&
    (__ALPHA == c.__ALPHA);
}


/* ----------------------------------------------------------------------- */

uchar_t Color4::getAlpha( ) const {  return __ALPHA;}

uchar_t& Color4::getAlpha( ) { return __ALPHA;}

real_t Color4::getAlphaClamped( ) const { return (real_t)__ALPHA / 255;}

uchar_t Color4::getBlue( ) const { return __BLUE;}

uchar_t& Color4::getBlue( ) { return __BLUE;}

real_t Color4::getBlueClamped( ) const { return (real_t)__BLUE / 255;} 

uchar_t Color4::getGreen( ) const { return __GREEN;}

uchar_t& Color4::getGreen( ) { return __GREEN;} 

real_t Color4::getGreenClamped( ) const { return (real_t)__GREEN / 255;}

uchar_t Color4::getRed( ) const { return __RED;}

uchar_t& Color4::getRed( ) { return __RED;}

real_t Color4::getRedClamped( ) const { return (real_t)__RED / 255;}

real_t Color4::getRGBAverage() const {
    return (real_t)(__RED + __GREEN +__BLUE)/(real_t)3;
}

real_t Color4::getRGBAverageClamped() const {
    return (real_t)(__RED + __GREEN +__BLUE+__ALPHA)/(real_t)765;
}
real_t Color4::getAverage() const {
    return (real_t)(__RED + __GREEN +__BLUE)/(real_t)3;
}

real_t Color4::getAverageClamped() const {
    return (real_t)(__RED + __GREEN +__BLUE+__ALPHA)/(real_t)1020;
}

uint_t Color4::toUint() const {
	return (uint_t(__ALPHA) << 24) + (uint_t(__RED) << 16) + (uint_t(__GREEN) << 8) + uint_t(__BLUE);
}

Color4 Color4::fromUint(uint_t value) {
  Color4 res;
  res.__ALPHA = ((value & 0xff000000) >> 24);
  res.__RED = ((value & 0x00ff0000) >> 16);
  res.__GREEN = ((value & 0x0000ff00) >> 8);
  res.__BLUE = (value & 0x000000ff);
  return res;
}

std::ostream& PGL(operator<<( std::ostream& stream, const Color4& c )) {
  return stream << "<" << (uint16_t)c.getRed() << "," << (uint16_t)c.getGreen() << "," 
		<< (uint16_t)c.getBlue() << "," << (uint16_t)c.getAlpha() << ">";
} 
