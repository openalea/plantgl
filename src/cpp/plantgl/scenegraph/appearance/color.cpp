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





#include "color.h"
#include <iostream>
#include <algorithm>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

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

Vector3 Color3::toClampedValues() const {
    return Vector3(getRedClamped(), getGreenClamped(), getBlueClamped());
}

uint_t Color3::toUint(eColor3Format format) const {
    return PACKVALi(uint_t(__RED),  CHANNELPOS3(format,0)) +
           PACKVALi(uint_t(__GREEN),CHANNELPOS3(format,1)) +
           PACKVALi(uint_t(__BLUE), CHANNELPOS3(format,2));
}

Color3 Color3::fromUint(uint_t value, eColor3Format format) {
  Color3 res;
  for (int i = 0; i < 3; ++i)
    res[i]   = UNPACKVALi(value, CHANNELPOS3(format,i));

  /*res.__RED = uchar_t((value & 0xff0000) >> 16);
  res.__GREEN = uchar_t((value & 0x00ff00) >> 8);
  res.__BLUE = uchar_t(value & 0x0000ff);*/
  return res;
}

Tuple3<uchar_t> Color3::toHSV8() const {
    Tuple3<uchar_t> hsv(0,0,0);
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

Color3 Color3::fromHSV(const Tuple3<uchar_t>& hsv) {
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

Tuple3<real_t> Color3::toHSV() const {
    Tuple3<real_t> hsv(0,0,0);
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

Color3 Color3::fromHSV(const Tuple3<real_t>& hsv) {
    real_t h = hsv[0];
    real_t s = hsv[1];
    real_t v = hsv[2];


    if (s <= GEOM_EPSILON) {
        return Color3(v,v,v);
    }

    float nh  = h / 60.;
    int i = int(nh);
    float ff = nh - i;

    real_t p = v * (1 - s);
    real_t q = v * (1 - (s * ff));
    real_t t = v * (1 - (s * (1 - ff)));

    real_t r   = 0;
    real_t g = 0;
    real_t b  = 0;

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

    Tuple3<real_t> hsv1 = c1.toHSV();
    Tuple3<real_t> hsv2 = c2.toHSV();
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

    Tuple3<real_t> hsvi(interpolhue,
                               hsv1[1]*oneminust+hsv2[1]*t,
                               hsv1[2]*oneminust+hsv2[2]*t);
    return Color3::fromHSV(hsvi);

}


Color3& Color3::operator*=(const Color3& c)
{
    real_t r = getRedClamped() * c.getRedClamped();
    real_t g = getGreenClamped() * c.getGreenClamped();
    real_t b = getBlueClamped() * c.getBlueClamped();
    __RED = uchar_t(r * 255);
    __GREEN = uchar_t(g * 255);
    __BLUE = uchar_t(b * 255);
    return *this;
}

Color3& Color3::operator*=(const real_t& v){
    real_t r = getRedClamped() * v;
    real_t g = getGreenClamped() * v;
    real_t b = getBlueClamped() * v;
    r *= 255; g *= 255; b *= 255;
    __RED = uchar_t(pglMin(r,real_t(255.)));
    __GREEN = uchar_t(pglMin(g,real_t(255.)));
    __BLUE = uchar_t(pglMin(b,real_t(255.)));
    return *this;
}

Color3 Color3::operator*(const Color3& c) const
{
    Color3 nc(*this);
    nc *= c;
    return nc;

}

Color3 Color3::operator*(const real_t& c) const
{
    Color3 nc(*this);
    nc *= c;
    return nc;

}

Color3& Color3::operator+=(const Color3& c)
{
    uint16_t red = uint16_t(__RED) + c.__RED;
    uint16_t green = uint16_t(__GREEN) + c.__GREEN;
    uint16_t blue = uint16_t(__BLUE) + c.__BLUE;
    __RED = (uchar_t)pglMin<uint16_t>(red, 255);
    __GREEN = (uchar_t)pglMin<uint16_t>(green, 255);
    __BLUE = (uchar_t)pglMin<uint16_t>(blue, 255);
    return *this;

}

Color3 Color3::operator+(const Color3& c) const
{
    Color3 nc(*this);
    nc += c;
    return nc;

}

Color3 PGL(operator*)( const real_t& s, const Color3& v ) {
    return v * s;
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

Vector4 Color4::toClampedValues() const {
    return Vector4(getRedClamped(), getGreenClamped(), getBlueClamped(), getAlphaClamped());
}

uint_t Color4::toUint(eColor4Format format) const {

    return PACKVALi(uint_t(__RED),  CHANNELPOS4(format,0)) +
           PACKVALi(uint_t(__GREEN),CHANNELPOS4(format,1)) +
           PACKVALi(uint_t(__BLUE), CHANNELPOS4(format,2)) +
           PACKVALi(uint_t(__ALPHA),CHANNELPOS4(format,3));

    /*if (format == eARGB)
       return (uint_t(__ALPHA) << 24) + (uint_t(__RED) << 16) + (uint_t(__GREEN) << 8) + uint_t(__BLUE);
    else
       return (uint_t(__RED) << 24) + (uint_t(__GREEN) << 16) + (uint_t(__BLUE) << 8) + uint_t(__ALPHA);*/
}

Color4 Color4::fromUint(uint_t value, eColor4Format format) {
  Color4 res;
  for (int i = 0; i < 4; ++i)
    res[i]   = UNPACKVALi(value, CHANNELPOS4(format,i));

  /*res.__ALPHA = ((value & 0xff000000) >> (format == eARGB?24:0));
  res.__RED = ((value & 0x00ff0000) >> (format == eARGB?16:24));
  res.__GREEN = ((value & 0x0000ff00) >> (format == eARGB?8:16));
  res.__BLUE = ((value & 0x000000ff) >> (format == eARGB?0:24));*/
  return res;
}


Tuple4<uchar_t> Color4::toHSVA8() const
{
    Tuple3<uchar_t> hsv = Color3(__RED,__GREEN,__BLUE).toHSV8();
    return Tuple4<uchar_t>(hsv[0],hsv[1],hsv[2],__ALPHA);
}

Tuple4<real_t> Color4::toHSVA() const
{
    Tuple3<uchar_t> hsv = Color3(__RED,__GREEN,__BLUE).toHSV8();
    return Tuple4<real_t>(hsv[0],hsv[1],hsv[2],__ALPHA/255);
}

Color4 Color4::fromHSVA(const Tuple4<uchar_t>& hsva)
{
    Color3 c = Color3::fromHSV(Tuple3<uchar_t>(hsva[0],hsva[1],hsva[2]));
    return Color4(c.getRed(),c.getGreen(),c.getBlue(),hsva[3]);

}

Color4 Color4::fromHSVA(const Tuple4<real_t>& hsva)
{
    Color3 c = Color3::fromHSV(Tuple3<real_t>(hsva[0],hsva[1],hsva[2]));
    return Color4(c.getRed(),c.getGreen(),c.getBlue(),hsva[3]*real_t(255.));
}

Color4 Color4::interpolate(const Color4& c1, const Color4& c2, real_t t)
{
    if (t < 0) t = 0.;
    else if(t >= 1) t = 1.;
    real_t oneminust = 1- t;

    Tuple4<real_t> hsva1 = c1.toHSVA();
    Tuple4<real_t> hsva2 = c2.toHSVA();
    Tuple4<real_t> hsvai(hsva1[0]*oneminust+hsva2[0]*t,hsva1[1]*oneminust+hsva2[1]*t,hsva1[2]*oneminust+hsva2[2]*t,hsva1[3]*oneminust+hsva2[3]*t);
    return Color4::fromHSVA(hsvai);

}


std::ostream& PGL(operator<<( std::ostream& stream, const Color4& c )) {
  return stream << "<" << (uint16_t)c.getRed() << "," << (uint16_t)c.getGreen() << ","
        << (uint16_t)c.getBlue() << "," << (uint16_t)c.getAlpha() << ">";
}



Color4& Color4::operator*=(const Color4& c)
{
    real_t r = getRedClamped() * c.getRedClamped();
    real_t g = getGreenClamped() * c.getGreenClamped();
    real_t b = getBlueClamped() * c.getBlueClamped();
    real_t a = getAlphaClamped() * c.getAlphaClamped();
    __RED = uchar_t(r * 255);
    __GREEN = uchar_t(g * 255);
    __BLUE = uchar_t(b * 255);
    __ALPHA = uchar_t(a * 255);
    return *this;
}

Color4& Color4::operator*=(const real_t& v){
    real_t r = getRedClamped() * v;
    real_t g = getGreenClamped() * v;
    real_t b = getBlueClamped() * v;
    real_t a = getAlphaClamped() * v;
    r *= 255; g *= 255; b *= 255; a *= 255;
    __RED = uchar_t(pglMin(r,real_t(255.)));
    __GREEN = uchar_t(pglMin(g,real_t(255.)));
    __BLUE = uchar_t(pglMin(b,real_t(255.)));
    __ALPHA = uchar_t(pglMin(a,real_t(255.)));
    return *this;
}

Color4 Color4::operator*(const Color4& c) const
{
    Color4 nc(*this);
    nc *= c;
    return nc;

}

Color4 Color4::operator*(const real_t& c) const
{
    Color4 nc(*this);
    nc *= c;
    return nc;

}

Color4& Color4::operator+=(const Color4& c)
{
    uint16_t red = uint16_t(__RED) + c.__RED;
    uint16_t green = uint16_t(__GREEN) + c.__GREEN;
    uint16_t blue = uint16_t(__BLUE) + c.__BLUE;
    uint16_t alpha = uint16_t(__ALPHA) + c.__ALPHA;
    __RED = (uchar_t)pglMin<uint16_t>(red, 255);
    __GREEN = (uchar_t)pglMin<uint16_t>(green, 255);
    __BLUE = (uchar_t)pglMin<uint16_t>(blue, 255);
    __ALPHA = (uchar_t)pglMin<uint16_t>(alpha, 255);
    return *this;

}

Color4 Color4::operator+(const Color4& c) const
{
    Color4 nc(*this);
    nc += c;
    return nc;

}

Color4 PGL(operator*)( const real_t& s, const Color4& v ) {
    return v * s ;
}
