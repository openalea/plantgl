/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier 
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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

/*! \file util_colorarray.h
    \brief Definition of the container class Color3Array and Color4Array.
*/

#ifndef UTIL_COLORARRAY_H
#define UTIL_COLORARRAY_H

#include "util_array.h"
#include "appe_color.h"

GEOM_BEGIN_NAMESPACE

class Color3Array : public Array1<Color3>
{

public:


  /// Constructs an Array1 of size \e size
  Color3Array( uint32_t size = 0 ) :
    Array1<Color3>(size) {
 }

  /// Constructs an Array1 with \e size copies of \e t.
  Color3Array( uint32_t size, const Color3& t ) :
    Array1<Color3>(size,t) {
  }

  /// Constructs an Array1 with the range [\e begin, \e end).
  template <class InIterator>
  Color3Array( InIterator begin, InIterator end ) :
    Array1<Color3>(begin,end) {
  }

  /// Destructor
  virtual ~Color3Array( ) {
  }

  inline uchar_t * toUcharArray() const {
	  size_t size = getSize();
	  uchar_t * data = new uchar_t[3*size];
	  for( size_t i = 0 ; i < size; ++i )
      {
        const Color3& color = getAt( i );
        data[ 3*i ] = color.getRed();
        data[ 3*i+1 ] = color.getGreen();
        data[ 3*i+2 ] = color.getBlue();
      }
  }
};

typedef RCPtr<Color3Array> Color3ArrayPtr;

class Color4Array : public Array1<Color4>
{

public:


  /// Constructs an Array1 of size \e size
  Color4Array( uint32_t size = 0 ) :
    Array1<Color4>(size) {
 }

  /// Constructs an Array1 with \e size copies of \e t.
  Color4Array( uint32_t size, const Color4& t ) :
    Array1<Color4>(size,t) {
  }

  /// Constructs an Array1 with the range [\e begin, \e end).
  template <class InIterator>
  Color4Array( InIterator begin, InIterator end ) :
    Array1<Color4>(begin,end) {
  }

  /// Destructor
  virtual ~Color4Array( ) {
  }

  inline uchar_t * toUcharArray() const {
	  size_t size = getSize();
	  uchar_t * data = new uchar_t[4*size];
	  for( size_t i = 0 ; i < size; ++i )
      {
        const Color4& color = getAt( i );
        data[ 4*i ] = color.getRed();
        data[ 4*i+1 ] = color.getGreen();
        data[ 4*i+2 ] = color.getBlue();
        data[ 4*i+3 ] = color.getAlpha();
      }
  }
};

typedef RCPtr<Color4Array> Color4ArrayPtr;

GEOM_END_NAMESPACE

#endif // UTIL_COLORARRAY_H

