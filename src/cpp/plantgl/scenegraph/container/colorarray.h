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


/*! \file util_colorarray.h
    \brief Definition of the container class Color3Array and Color4Array.
*/

#ifndef UTIL_COLORARRAY_H
#define UTIL_COLORARRAY_H

#include <plantgl/tool/util_array.h>
#include <plantgl/scenegraph/appearance/color.h>

PGL_BEGIN_NAMESPACE

class Color3Array : public Array1<Color3>
{

public:


  /// Constructs an Array1 of size \e size
  Color3Array( uint_t size = 0 ) :
    Array1<Color3>(size) {
 }

  /// Constructs an Array1 with \e size copies of \e t.
  Color3Array( uint_t size, const Color3& t ) :
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

class Color4Array : public Array1<Color4>
{

public:

  /// Constructs an Array1 of size \e size
  Color4Array( uint_t size = 0 ) :
    Array1<Color4>(size) {
 }

  /// Constructs an Array1 with \e size copies of \e t.
  Color4Array( uint_t size, const Color4& t ) :
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
apply_colormap(const RCPtr<ColorArray> colormap, const RealArrayPtr values)
{
    std::pair<RealArray::const_iterator,RealArray::const_iterator> minmax = values->getMinAndMax(true);
    return apply_colormap(colormap, values, *minmax.first, *minmax.second);
}

template <class ColorArray>
RCPtr<ColorArray>
apply_colormap(const RCPtr<ColorArray> colormap, const RealArrayPtr values, real_t minvalue, real_t maxvalue)
{
    size_t nbcolor = colormap->size();
    real_t valuerange = maxvalue - minvalue;
    real_t valuestep = valuerange / nbcolor;
    RCPtr<ColorArray> result = new ColorArray(values->size());
    typename ColorArray::iterator itresult = result->begin();
    for(RealArray::const_iterator itv = values->begin(); itv != values->end(); ++itv, ++itresult){
        int colorid = int((*itv - minvalue) / valuestep);
        if (colorid < 0) colorid = 0;
        else if (colorid >= nbcolor) colorid = nbcolor -1;
        *itresult = colormap->getAt( colorid );
    }
    return result;
}


PGL_END_NAMESPACE

#endif // UTIL_COLORARRAY_H

