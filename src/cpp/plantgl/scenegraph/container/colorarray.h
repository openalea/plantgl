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

/*! \file util_colorarray.h
    \brief Definition of the container class Color3Array and Color4Array.
*/

#ifndef UTIL_COLORARRAY_H
#define UTIL_COLORARRAY_H

#include <plantgl/tool/util_array.h>
#include <plantgl/scenegraph/appearance/color.h>

PGL_BEGIN_NAMESPACE

class Color3Array : public TOOLS(Array1)<Color3>
{

public:


  /// Constructs an Array1 of size \e size
  Color3Array( uint_t size = 0 ) :
    TOOLS(Array1)<Color3>(size) {
 }

  /// Constructs an Array1 with \e size copies of \e t.
  Color3Array( uint_t size, const Color3& t ) :
    TOOLS(Array1)<Color3>(size,t) {
  }

  /// Constructs an Array1 with the range [\e begin, \e end).
  template <class InIterator>
  Color3Array( InIterator begin, InIterator end ) :
    TOOLS(Array1)<Color3>(begin,end) {
  }

  /// Destructor
  virtual ~Color3Array( ) {
  }

  inline uchar_t * toUcharArray() const {
	  size_t len = size();
	  uchar_t * data = new uchar_t[3*len];
	  for( size_t i = 0 ; i < len; ++i )
      {
        const Color3& color = getAt( i );
        data[ 3*i ] = color.getRed();
        data[ 3*i+1 ] = color.getGreen();
        data[ 3*i+2 ] = color.getBlue();
      }
      return data;
  }
};

typedef RCPtr<Color3Array> Color3ArrayPtr;
PGL_DECLARE_TYPE(Color3Array)

class Color4Array : public TOOLS(Array1)<Color4>
{

public:

  /// Constructs an Array1 of size \e size
  Color4Array( uint_t size = 0 ) :
    TOOLS(Array1)<Color4>(size) {
 }

  /// Constructs an Array1 with \e size copies of \e t.
  Color4Array( uint_t size, const Color4& t ) :
    TOOLS(Array1)<Color4>(size,t) {
  }

  /// Constructs an Array1 with the range [\e begin, \e end).
  template <class InIterator>
  Color4Array( InIterator begin, InIterator end ) :
    TOOLS(Array1)<Color4>(begin,end) {
  }

  /// Destructor
  virtual ~Color4Array( ) {
  }

  inline uchar_t * toUcharArray() const {
	  size_t len = size();
	  uchar_t * data = new uchar_t[4*len];
	  for( size_t i = 0 ; i < len; ++i )
      {
        const Color4& color = getAt( i );
        data[ 4*i ] = color.getRed();
        data[ 4*i+1 ] = color.getGreen();
        data[ 4*i+2 ] = color.getBlue();
        data[ 4*i+3 ] = color.getAlpha();
      }
      return data;
  }
};

typedef RCPtr<Color4Array> Color4ArrayPtr;
PGL_DECLARE_TYPE(Color4Array)

template <class ColorArray>
RCPtr<ColorArray>
apply_colormap(const RCPtr<ColorArray> colormap, const TOOLS(RealArrayPtr) values)
{  
    std::pair<TOOLS(RealArray)::const_iterator,TOOLS(RealArray)::const_iterator> minmax = values->getMinAndMax(true);
    return apply_colormap(colormap, values, *minmax.first, *minmax.second);
}

template <class ColorArray>
RCPtr<ColorArray>
apply_colormap(const RCPtr<ColorArray> colormap, const TOOLS(RealArrayPtr) values, real_t minvalue, real_t maxvalue)
{
    size_t nbcolor = colormap->size();
    real_t valuerange = maxvalue - minvalue;
    real_t valuestep = valuerange / nbcolor;
    RCPtr<ColorArray> result = new ColorArray(values->size());
    typename ColorArray::iterator itresult = result->begin();
    for(TOOLS(RealArray)::const_iterator itv = values->begin(); itv != values->end(); ++itv, ++itresult){
        int colorid = int((*itv - minvalue) / valuestep);
        if (colorid < 0) colorid = 0;
        else if (colorid >= nbcolor) colorid = nbcolor -1;
        *itresult = colormap->getAt( colorid );
    }
    return result;
}


PGL_END_NAMESPACE

#endif // UTIL_COLORARRAY_H

