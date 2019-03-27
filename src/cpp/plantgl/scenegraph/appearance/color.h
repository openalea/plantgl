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



/*! \file appe_color.h
    \brief Definition of the appearance class Color3 and Color4.
*/


#ifndef __matl_color_h__
#define __matl_color_h__

#include <plantgl/tool/util_tuple.h>
#include <plantgl/math/util_vector.h>
#include <iostream>
#include "../sg_config.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

#ifdef GEOM_DLL
template class SG_API Tuple3<uchar_t>;
#endif

/**
   \class Color3
   \brief A 3 component color expressed in \c red, \c green and \c blue.
*/

#define PACKVALi(value,i) (value << (8 * i))
#define UNPACKVALi(packedvalues,i) ((packedvalues & (0xff << 8*i)) >> (8*i))

#define CHANNELORDER4(r,g,b,a) (PACKVALi(r,0) + PACKVALi(g,1) + PACKVALi(b,2) + PACKVALi(a,3))
#define CHANNELORDER3(r,g,b)   (PACKVALi(r,0) + PACKVALi(g,1) + PACKVALi(b,2))
#define CHANNELPOS3(colorcode, channel) UNPACKVALi(colorcode, (2-channel))
#define CHANNELPOS4(colorcode, channel) UNPACKVALi(colorcode, (3-channel))




class Color4;
class SG_API Color3 : public Tuple3<uchar_t>
{

  friend class Color4;

public:

  enum eColor3Format {
    eRGB = CHANNELORDER3(0,1,2),
    eRBG = CHANNELORDER3(0,2,1),
    eGRB = CHANNELORDER3(1,0,2),
    eGBR = CHANNELORDER3(2,0,1),
    eBGR = CHANNELORDER3(2,1,0),
    eBRG = CHANNELORDER3(1,2,0)
  };

  /// The black color.
  static const Color3 BLACK;

  /// The blue color.
  static const Color3 BLUE;

  /// The cyan color.
  static const Color3 CYAN;

  /// The green color.
  static const Color3 GREEN;

  /// The magenta color.
  static const Color3 MAGENTA;

  /// The red color.
  static const Color3 RED;

  /// The white color.
  static const Color3 WHITE;

  /// The yellow color.
  static const Color3 YELLOW;

  /// Constructs a Color3 with the \e r, \e g and \e b values.
  explicit Color3( uchar_t r , uchar_t g , uchar_t b );

  /// Constructs a Color3 with the \e v value for the red, blue and green components.
  explicit Color3( uchar_t v  = 255) ;

  /// Constructs a Color3 with the 3 element array \e rgb.
  explicit Color3( const uchar_t * rgb );

  /// Constructs a Color3 with the 3 uchar_t element of \e rgb.
  explicit Color3( uint32_t rgb );

  /// Constructs a Color3 with the red, blue and green components of c4.
  Color3( const Color4& c4) ;

  /// Destructor
  ~Color3();

  /// Returns whether \e self is equal to \e c.
  bool operator==( const Color3& c ) const;

  /// Returns the \c blue component of \e self.
  uchar_t getBlue( ) const ;

  /// Returns a reference to the \c blue component of \e self.
  uchar_t& getBlue( ) ;

  /// Returns the \c green component of \e self clamped to [0,1].
  real_t getBlueClamped( ) const ;

  /// Returns the \c green component of \e self
  uchar_t getGreen( ) const ;

  /// Returns a reference to the \c green component of \e self.
  uchar_t& getGreen( );

  /// Returns the \c green component of \e self clamped to [0,1].
  real_t getGreenClamped( ) const;

 /// Returns the \c red component of \e self.
  uchar_t getRed( ) const;

  /// Returns a reference the \c red component of \e self.
  uchar_t& getRed( ) ;

  /// Returns the \c red component of \e self clamped to [0,1].
  real_t getRedClamped( ) const ;

  /// Returns the \e average of the red, green and blue value.
  real_t getAverage() const ;

  /// Returns the \e average of the red, green and blue value.
  real_t getAverageClamped() const ;

  /// Encode the rgb value onto an uint
  uint_t toUint(eColor3Format format = eRGB) const;

  /// Decode the rgb value from an uint
  static Color3 fromUint(uint_t value, eColor3Format format = eRGB);

  Tuple3<uchar_t> toHSV8() const;
  Tuple3<real_t> toHSV() const;

  Vector3 toClampedValues() const;

  static Color3 fromHSV(const Tuple3<uchar_t>& hsv);
  static Color3 fromHSV(const Tuple3<real_t>& hsv);

  static Color3 interpolate(const Color3& c1, const Color3& c2, real_t t = 0.5);


  Color3& operator*=(const Color3&);
  Color3& operator*=(const real_t&);

  Color3 operator*(const Color3&) const ;
  Color3 operator*(const real_t&) const ;

  Color3& operator+=(const Color3&);
  Color3 operator+(const Color3&) const ;

  friend SG_API Color3 operator*( const real_t& s, const Color3& v );


  /// Prints \e v to the output stream \e stream.
//  friend std::ostream& operator<<( std::ostream& stream, const Color3& c );

}; // Color3


SG_API Color3 operator*( const real_t& s, const Color3& v );

/* ----------------------------------------------------------------------- */

#ifdef GEOM_DLL
template class SG_API Tuple4<uchar_t>;
#endif

/**
   \class Color4
   \brief A 4 component color expressed in \c red, \c green, \c blue
   and \c alpha.
*/

class SG_API Color4 : public Tuple4<uchar_t>
{
public:

  enum eColor4Format {
    eARGB = CHANNELORDER4(1,2,3,0),
    eARBG = CHANNELORDER4(1,3,2,0),
    eAGRB = CHANNELORDER4(2,1,3,0),
    eAGBR = CHANNELORDER4(3,1,2,0),
    eABGR = CHANNELORDER4(3,2,1,0),
    eABRG = CHANNELORDER4(2,3,1,0),

    eRGBA = CHANNELORDER4(0,1,2,3),
    eRBGA = CHANNELORDER4(0,2,1,3),
    eGRBA = CHANNELORDER4(1,0,2,3),
    eGBRA = CHANNELORDER4(2,0,1,3),
    eBGRA = CHANNELORDER4(2,1,0,3),
    eBRGA = CHANNELORDER4(1,2,0,3)
  };


  /// The black color.
  static const Color4 BLACK;

  /// The blue color.
  static const Color4 BLUE;

  /// The cyan color.
  static const Color4 CYAN;

  /// The green color.
  static const Color4 GREEN;

  /// The magenta color.
  static const Color4 MAGENTA;

  /// The red color.
  static const Color4 RED;

  /// The white color.
  static const Color4 WHITE;

  /// The yellow color.
  static const Color4 YELLOW;

  /// Constructs a Color4 with the \e r, \e g, \e b and \e a values.
  explicit Color4( uchar_t r , uchar_t g , uchar_t b, uchar_t a = 0 ) ;

  /// Constructs a Color4 with the \e v values for the 4 components.
  explicit Color4( uchar_t v = 255);

  /// Constructs a Color4 with the 4 element array \e rgb.
  Color4( const uchar_t * rgba );

  /// Constructs a Color4 with the 4 uchar_t element of \e argb.
  explicit Color4( uint32_t argb );

  /// Constructs a Color4 with the Color3 \e c and the alpha value \e alpha.
  Color4( const Color3& c, uchar_t alpha = 0);

  /// Destructor.
  ~Color4(  );

  /// Returns whether \e self is equal to \e c.
  bool operator==( const Color4& c ) const;

  /// Returns the \c alpha component of \e self.
  uchar_t getAlpha( ) const ;

  /// Returns a reference to the \c alpha component of \e self.
  uchar_t& getAlpha( );

  /// Returns the \c alpha component of \e self clamped to [0,1].
  real_t getAlphaClamped( ) const ;

  /// Returns the \c blue component of \e self.
  uchar_t getBlue( ) const ;

  /// Returns a reference to the \c blue component of \e self.
  uchar_t& getBlue( ) ;

  /// Returns the \c green component of \e self clamped to [0,1].
  real_t getBlueClamped( ) const ;

  /// Returns the \c green component of \e self
  uchar_t getGreen( ) const ;

  /// Returns a reference to the \c green component of \e self.
  uchar_t& getGreen( ) ;

  /// Returns the \c green component of \e self clamped to [0,1].
  real_t getGreenClamped( ) const ;

 /// Returns the \c red component of \e self.
  uchar_t getRed( ) const ;

  /// Returns a reference the \c red component of \e self.
  uchar_t& getRed( ) ;

  /// Returns the \c red component of \e self clamped to [0,1].
  real_t getRedClamped( ) const ;

  /// Returns the \e average of the red, green, blue and alpha value.
  real_t getRGBAverage() const ;

  /// Returns the \e average of the red, green, blue and alpha  value.
  real_t getRGBAverageClamped() const ;

  /// Returns the \e average of the red, green, blue and alpha value.
  real_t getAverage() const ;

  /// Returns the \e average of the red, green, blue and alpha  value.
  real_t getAverageClamped() const ;

  /// Encode the argb value onto an uint
  uint_t toUint(eColor4Format format = eARGB) const;

  /// Decode the argb value from an uint
  static Color4 fromUint(uint_t, eColor4Format format = eARGB);

  Tuple4<uchar_t> toHSVA8() const;
  Tuple4<real_t> toHSVA() const;

  Vector4 toClampedValues() const;

  static Color4 fromHSVA(const Tuple4<uchar_t>& hsv);
  static Color4 fromHSVA(const Tuple4<real_t>& hsv);

  static Color4 interpolate(const Color4& c1, const Color4& c2, real_t t = 0.5);


  Color4& operator*=(const Color4&);
  Color4& operator*=(const real_t&);

  Color4 operator*(const Color4&) const ;
  Color4 operator*(const real_t&) const ;

  Color4& operator+=(const Color4&);
  Color4 operator+(const Color4&) const ;

  friend SG_API Color4 operator*( const real_t& s, const Color4& v );

  /// Prints \e v to the output stream \e stream.
//  friend std::ostream& operator<<( std::ostream& stream, const Color4& c ) ;

}; // Color4


SG_API Color4 operator*( const real_t& s, const Color4& v );

/* ----------------------------------------------------------------------- */

/// Write Color3 \b c in \b stream.
SG_API std::ostream& operator<<( std::ostream& stream, const PGL(Color3&) c );
SG_API std::ostream& operator<<( std::ostream& stream, const PGL(Color4&) c );

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
// __matl_color_h__
#endif

