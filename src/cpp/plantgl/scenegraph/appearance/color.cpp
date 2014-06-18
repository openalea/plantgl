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

TOOLS(Tuple3)<uchar_t> Color3::toHSV8() const {
    TOOLS(Tuple3)<uchar_t> hsv(0,0,0);
    Color3::const_iterator itmax = getMax();
    uchar_t rgb_max = *itmax;
    uchar_t rgb_min = *getMin();
    float rgb_extend = rgb_max - rgb_min;

    hsv[2] = rgb_max;
    if (hsv[2] == 0) return hsv;

    hsv[1] = uchar_t(255 * rgb_extend /rgb_max);
    if (hsv[1] == 0 ) return hsv;

    switch (std::distance(begin(), itmax)){
    case 0: // RED
        hsv[0] = 0 + 43 * (__GREEN - __BLUE) / rgb_extend;
        break;
    case 1: //GREEN
        hsv[0] = 85 + 43 * (__BLUE - __RED) / rgb_extend;
        break;
    default: // BLUE
        hsv[0] = 171 + 43 * (__RED - __GREEN) / rgb_extend;
    }
    
    return hsv;
}

Color3 Color3::fromHSV(const TOOLS(Tuple3)<uchar_t>& hsv) {
    uchar_t h = hsv[0];
    uchar_t s = hsv[1];
    uchar_t v = hsv[2];

    if (s == 0) {
        return Color3(v,v,v);
    }

    float nh  = h / 43.;
    int i = int(nh);
    float ff = nh - i; 

    float sn = s / 255.;

    uchar_t p = v * (1 - sn);
    uchar_t q = v * (1 - (sn * ff));
    uchar_t t = v * (1 - (sn * (1 - ff)));

    uchar_t r   = 0;
    uchar_t g = 0;
    uchar_t b  = 0;

    switch (i){
        case 0:
          r = v; g = t; b = p;
          break;
        case 1:
          r = q; g = v; b = p;
          break;
        case 2:
          r = p; g = v; b = t;
          break;
        case 3:
          r = p; g = q; b = v;
          break;
        case 4:
          r = t; g = p; b = v;
          break;
        case 5:
          r = v; g = p; b = q;
          break;
    }
    return Color3(r,g,b);

}

TOOLS(Tuple3)<real_t> Color3::toHSV() const {
    TOOLS(Tuple3)<real_t> hsv(0,0,0);
    Color3::const_iterator itmax = getMax();
    real_t rgb_max = real_t(*itmax);

    hsv[2] = rgb_max;
    if (hsv[2] <= GEOM_EPSILON) return hsv;

 
    real_t rgb_min = real_t(*getMin());
    real_t rgb_extend = rgb_max - rgb_min;

    hsv[1] = rgb_extend /rgb_max;
    if (hsv[1] <= GEOM_EPSILON ) return hsv;

    real_t r = (__RED   -  rgb_min)/ rgb_extend;
    real_t g = (__GREEN -  rgb_min)/ rgb_extend;
    real_t b = (__BLUE  -  rgb_min)/ rgb_extend;

    switch (std::distance(begin(), itmax)){
    case 0: // RED
        hsv[0] = 0 + 60. * (g - b) ;
        if (hsv[0] < 0) hsv[0] += 360;
        break;
    case 1: //GREEN
        hsv[0] = 120. + 60. * (b - r) ;
        break;
    default: // BLUE
        hsv[0] = 240. + 60. * (r - g) ;
    }
    
    return hsv;
}

Color3 Color3::fromHSV(const TOOLS(Tuple3)<real_t>& hsv) {
    real_t h = hsv[0];
    real_t s = hsv[1];
    real_t v = hsv[2];


    if (s <= GEOM_EPSILON) {
        return Color3(v,v,v);
    }

    float nh  = h / 60.;
    int i = int(nh);
    float ff = nh - i; 

    uchar_t p = v * (1 - s);
    uchar_t q = v * (1 - (s * ff));
    uchar_t t = v * (1 - (s * (1 - ff)));

    uchar_t r   = 0;
    uchar_t g = 0;
    uchar_t b  = 0;

    switch (i){
        case 0:
          r = v; g = t; b = p;
          break;
        case 1:
          r = q; g = v; b = p;
          break;
        case 2:
          r = p; g = v; b = t;
          break;
        case 3:
          r = p; g = q; b = v;
          break;
        case 4:
          r = t; g = p; b = v;
          break;
        case 5:
          r = v; g = p; b = q;
          break;
    }
    return Color3(r,g,b);

}

Color3 Color3::interpolate(const Color3& c1, const Color3& c2, real_t t)
{
    if (t < 0) t = 0.;
    else if(t >= 1) t = 1.;
    real_t oneminust = 1- t;

    TOOLS(Tuple3)<real_t> hsv1 = c1.toHSV();
    TOOLS(Tuple3)<real_t> hsv2 = c2.toHSV();
    real_t h1 = hsv1[0];
    real_t h2 = hsv2[0];
    real_t minhue = std::min(h1,h2);
    real_t maxhue = std::max(h1,h2);
    bool hzenith = (maxhue - minhue > (minhue + 360 - maxhue));
    real_t interpolhue = h1*oneminust+h2*t;
    if (hzenith) {
        if (h1 > h2) h2  += 360;
        else h1 += 360;
        interpolhue = h1*oneminust+h2*t;
        if (interpolhue >= 360) interpolhue -= 360;
    }

    TOOLS(Tuple3)<real_t> hsvi(interpolhue,
                               hsv1[1]*oneminust+hsv2[1]*t,
                               hsv1[2]*oneminust+hsv2[2]*t);
    return Color3::fromHSV(hsvi);

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


TOOLS(Tuple4)<uchar_t> Color4::toHSVA8() const
{
    TOOLS(Tuple3)<uchar_t> hsv = Color3(__RED,__GREEN,__BLUE).toHSV8();
    return TOOLS(Tuple4)<uchar_t>(hsv[0],hsv[1],hsv[2],__ALPHA);
}

TOOLS(Tuple4)<real_t> Color4::toHSVA() const
{
    TOOLS(Tuple3)<uchar_t> hsv = Color3(__RED,__GREEN,__BLUE).toHSV8();
    return TOOLS(Tuple4)<real_t>(hsv[0],hsv[1],hsv[2],__ALPHA/255);
}

Color4 Color4::fromHSVA(const TOOLS(Tuple4)<uchar_t>& hsva)
{
    Color3 c = Color3::fromHSV(TOOLS(Tuple3)<uchar_t>(hsva[0],hsva[1],hsva[2]));
    return Color4(c.getRed(),c.getGreen(),c.getBlue(),hsva[3]);

}

Color4 Color4::fromHSVA(const TOOLS(Tuple4)<real_t>& hsva)
{
    Color3 c = Color3::fromHSV(TOOLS(Tuple3)<real_t>(hsva[0],hsva[1],hsva[2]));
    return Color4(c.getRed(),c.getGreen(),c.getBlue(),hsva[3]*255.);
}

Color4 Color4::interpolate(const Color4& c1, const Color4& c2, real_t t)
{
    if (t < 0) t = 0.;
    else if(t >= 1) t = 1.;
    real_t oneminust = 1- t;

    TOOLS(Tuple4)<real_t> hsva1 = c1.toHSVA();
    TOOLS(Tuple4)<real_t> hsva2 = c2.toHSVA();
    TOOLS(Tuple4)<real_t> hsvai(hsva1[0]*oneminust+hsva2[0]*t,hsva1[1]*oneminust+hsva2[1]*t,hsva1[2]*oneminust+hsva2[2]*t,hsva1[3]*oneminust+hsva2[3]*t);
    return Color4::fromHSVA(hsvai);

}


std::ostream& PGL(operator<<( std::ostream& stream, const Color4& c )) {
  return stream << "<" << (uint16_t)c.getRed() << "," << (uint16_t)c.getGreen() << "," 
		<< (uint16_t)c.getBlue() << "," << (uint16_t)c.getAlpha() << ">";
} 
