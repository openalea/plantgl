/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): Ch. Nouguier (christophe.nouguier@cirad.fr)
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




#ifndef __bfstream_h__
#define __bfstream_h__

/*! \file bfstream.h
    \brief File for binary file stream
*/


/* ----------------------------------------------------------------------- */

#include <iostream>
#include <fstream>
#include <string>
//using namespace std;
#include "util_tuple.h"

/* ----------------------------------------------------------------------- */

/// Big Endian value for byte order.
#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN 0
#endif
/// Little Endian value for byte order.
#ifndef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN 1
#endif
/// Byte Order. If not defined (on windows for example), assign it to little endian
#ifndef __BYTE_ORDER
#ifdef _MSC_VER
#pragma message("__BYTE_ORDER macro not defined. Use Little Endian by default.")
#else
#warning "__BYTE_ORDER macro not defined. Use Little Endian by default."
#endif
#define __BYTE_ORDER __LITTLE_ENDIAN
#endif

/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/*! \fn void flipBytes( const char * src, char * dest, size_t n )
    \brief The flipBytes() function flips \e n bytes from the memory area
    \e src to the memory area \e dest. */
inline void flipBytes( const char * src, char * dest, size_t n )
{
  switch(n)
    {
    case 1 :
      dest[0] = src[0];
      break;
    case 2 :
      dest[0] = src[1], dest[1] = src[0];
      break;
    case 4 :
      dest[0] = src[3], dest[1] = src[2];
      dest[2] = src[1], dest[3] = src[0];
      break;
    case 8 :
      dest[0] = src[7], dest[1] = src[6];
      dest[2] = src[5], dest[3] = src[4];
      dest[4] = src[3], dest[5] = src[2];
      dest[6] = src[1], dest[7] = src[0];
      break;
    };
}


/* ----------------------------------------------------------------------- */

/** \class bofstream
    \brief bofstream specializes the \c ofstream class to store variables
    in binary format. */

class TOOLS_API bofstream
{

public:

  /// @name Constructor
  //@{

  /// Constructs a writable binary file stream \e file_name.
  bofstream( const char * file_name ) :
    __stream(file_name,std::ios::out | std::ios::binary)
  {
  }

  /// Constructs a writable binary file stream \e file_name.
  bofstream( const std::string& file_name ) :
    __stream(file_name.c_str(),std::ios::out | std::ios::binary)
  {
  }

  //@}

  /// @name Destructor
  //@{

  /// Destructor.
  virtual ~bofstream();

  //@}

  /** @name Writing functions
      Writes the appropriate built-in type to \e self. */
  //@{

  /// Binary write of a \b bool.
  bofstream& operator<<( bool b )
  {
    return _writeBytes((char *)&b,sizeof(b));
  }

  /// Binary write of a \b char.
  bofstream& operator<<( char c )
  {
    return _writeBytes((char *)&c,sizeof(c));
  }

  /// Binary write of a \b double.
  bofstream& operator<<( double d )
  {
    return _writeBytes((char *)&d,sizeof(d));
  }

  /// Binary write of a \b float.
  bofstream& operator<<( float f )
  {
    return _writeBytes((char *)&f,sizeof(f));
  }

  /// Binary write of a \b int.
  bofstream& operator<<( int i )
  {
    return _writeBytes((char *)&i,sizeof(i));
  }

  /// Binary write of a \b long.
  bofstream& operator<<( long l )
  {
    return _writeBytes((char *)&l,sizeof(l));
  }

  /// Binary write of a \b short.
  bofstream& operator<<( short s )
  {
    return _writeBytes((char *)&s,sizeof(s));
  }

  /// Binary write of an \b unsigned \b char.
  bofstream& operator<<( unsigned char uc )
  {
    return _writeBytes((char *)&uc,sizeof(uc));
  }

  /// Binary write of an \b unsigned \b int.
  bofstream& operator<<( unsigned int ui )
  {
    return _writeBytes((char *)&ui,sizeof(ui));
  }

  /// Binary write of an \b unsigned \b long.
  bofstream& operator<<( unsigned long ul )
  {
    return  _writeBytes((char *)&ul,sizeof(ul));
  }

  /// Binary write of an \b unsigned \b short.
  bofstream& operator<<( unsigned short us )
  {
    return _writeBytes((char *)&us,sizeof(us));
  }

  /// Binary write of a \b string.
  bofstream& operator<<( const char * s )
  {
    __stream << s;
    return *this;
  }

  /// Binary write of a \b string.
  bofstream& operator<<( std::string s )
  {
    __stream << s;
    return *this;
  }

  /// Binary write of a \b Tuple2.
  template<class T>
  bofstream& operator<<( const Tuple2<T>& t )
  {
    return (operator<<(t.getAt(0))).operator<<(t.getAt(1));
  }

  /// Binary write of a \b Tuple3.
  template<class T>
  bofstream& operator<<( const Tuple3<T>& t )
  {
    return ((operator<<(t.getAt(0))).operator<<(t.getAt(1))).operator<<(t.getAt(2));
  }

  /// Binary write of a \b Tuple4.
  template<class T>
  bofstream& operator<<( const Tuple4<T>& t )
  {
    return (((operator<<(t.getAt(0))).operator<<(t.getAt(1))).operator<<(t.getAt(2))).operator<<(t.getAt(3));
  }

  /// Binary write of \e data of size \e size.
  void write( const char * data, size_t size )
  {
    __stream.write(data,size);
  }
  //@}

  /** @name Stream
      Function related to the stream. */
  //@{

  /// Return the current stream.
  std::ofstream& getStream() { return __stream; }

  /// Return the current stream.
  const std::ofstream& getStream() const { return __stream; }

  /// Returns true if \e stream  is valid.
  operator void *( ) const { return (__stream); }

  /// Returns true if \e stream  is not valid.
  bool operator!( ) const { return (!__stream); }

  /// Returns true if \e stream  is at the end.
  bool eof( ) const { return __stream.eof(); }

  //@}



private:

  virtual bofstream& _writeBytes( const char * data, size_t size )
  {
    __stream.write(data,size);
    return *this;
  }

protected:

  std::ofstream __stream;

};


/* ----------------------------------------------------------------------- */

/** \class bifstream
    \brief bifstream specializes the \c ifstream class to restore variables
    in binary format. */

class TOOLS_API bifstream
{

public:

  /// @name Constructors
  //@{

  /// Constructs a readable binary file stream \e file_name.
  bifstream( const char * file_name ) :
    __stream(file_name,std::ios::in | std::ios::binary)
  {
  }

  /// Constructs a readable binary file stream \e file_name.
  bifstream( const std::string& file_name ) :
    __stream(file_name.c_str(),std::ios::in | std::ios::binary)
  {
  }

  //@}

  /// @name Destructor
  //@{

  /// Destructor.
  virtual ~bifstream();

  //@}

  /** @name Reading functions
      Reads the appropriate built-in types from \e self. */
  //@{

  /// Binary read of a \b bool.
  bifstream& operator>>( bool& b )
  {
    return _readBytes((char *)&b,sizeof(b));
  }

  /// Binary read of a \b char.
  bifstream& operator>>( char& c )
  {
    return _readBytes((char *)&c,sizeof(c));
  }

  /// Binary read of a \b double.
  bifstream& operator>>( double& d )
  {
    return _readBytes((char *)&d,sizeof(d));
  }

  /// Binary read of a \b float.
  bifstream& operator>>( float& f )
  {
    return _readBytes((char *)&f,sizeof(f));
  }

  /// Binary read of an \b int.
  bifstream& operator>>( int& i )
  {
    return _readBytes((char *)&i,sizeof(i));
  }

  /// Binary read of a \b long.
  bifstream& operator>>( long& l )
  {
    return _readBytes((char *)&l,sizeof(l));
  }

  /// Binary read of a \b short.
  bifstream& operator>>( short& s )
  {
    return _readBytes((char *)&s,sizeof(s));
  }

  /// Binary read of an \b unsigned \b char.
  bifstream& operator>>( unsigned char& uc )
  {
    return _readBytes((char *)&uc,sizeof(uc));
  }

  /// Binary read of an \b unsigned \b int.
  bifstream& operator>>( unsigned int& ui )
  {
    return _readBytes((char *)&ui,sizeof(ui));
  }

  /// Binary read of an \b unsigned \b long.
  bifstream& operator>>( unsigned long& ul )
  {
    return _readBytes((char *)&ul,sizeof(ul));
  }

  /// Binary read of an \b unsigned \b short.
  bifstream& operator>>( unsigned short& us )
  {
    return _readBytes((char *)&us,sizeof(us));
  }

  /// Binary read of a \b Tuple2.
  template<class T>
  bifstream& operator>>( Tuple2<T>& t )
  {
    return (operator>>(t.getAt(0))).operator>>(t.getAt(1));
  }

  /// Binary read of a \b Tuple3.
  template<class T>
  bifstream& operator>>( Tuple3<T>& t )
  {
    return ((operator>>(t.getAt(0))).operator>>(t.getAt(1))).operator>>(t.getAt(2));
  }

  /// Binary read of a \b Tuple4.
  template<class T>
  bifstream& operator>>( Tuple4<T>& t )
  {
    return (((operator>>(t.getAt(0))).operator>>(t.getAt(1))).operator>>(t.getAt(2))).operator>>(t.getAt(3));
  }

  /// Binary read of \e data of size \e size.
  void read( char * data, size_t size )
  {
    __stream.read(data,size);
  }

  //@}

  /** @name Stream
      Function related to the stream. */
  //@{

  /// Return the current stream.
  std::ifstream& getStream() { return __stream; }

  /// Return the current stream.
  const std::ifstream& getStream() const { return __stream; }

  /// Returns true if \e stream  is valid.
  operator void *( ) const { return (__stream); }

  /// Returns true if \e stream  is not valid.
  bool operator!( ) const { return (!__stream); }

  /// Returns true if \e stream  is at the end.
  bool eof( ) const { return __stream.eof(); }

  //@}


private:

  virtual bifstream& _readBytes( char * data, size_t size )
  {
    __stream.read(data,size);
    return *this;
  }

protected:

  std::ifstream __stream;
};


/* ----------------------------------------------------------------------- */

/** \class leofstream
    \brief leofstream specializes the \c bofstream class to store variables
    in little endian format. */

class TOOLS_API leofstream : public bofstream
{

public:

  /// @name Constructors
  //@{

  /** Constructs a writable little endian byte ordered binary file stream
      \e file_name. */
  leofstream( const char * file_name ) :
    bofstream(file_name)
  {
  }

  leofstream( const std::string& file_name ) :
    bofstream(file_name.c_str())
  {
  }

  virtual ~leofstream();

  //@}

private:

#if __BYTE_ORDER == __BIG_ENDIAN
  virtual bofstream& _writeBytes( const char * data, size_t size )
  {
    register char flipped_data[8];
    flipBytes(data,flipped_data,size);
    __stream.write(flipped_data,size);
    return *this;
  }
#endif

};


/* ----------------------------------------------------------------------- */

/** \class leifstream
    \brief bifstream specializes the \c bifstream class to restore variables
    in little endian format. */

class TOOLS_API leifstream : public bifstream
{

public:

  /// @name Constructors
  //@{

  /** Constructs a readable little endian byte ordered binary file stream
     \e file_name. */
  leifstream( const char * file_name ) :
    bifstream(file_name)
  {
  }

  leifstream( const std::string& file_name ) :
    bifstream(file_name.c_str())
  {
  }

  virtual ~leifstream();

  //@}

private:

#if __BYTE_ORDER == __BIG_ENDIAN
  virtual bifstream& _readBytes( char * data, size_t size )
  {
    register char flipped_data[8];
    __stream.read(flipped_data,size);
    flipBytes(flipped_data,data,size);
    return *this;
  }
#endif

};


/* ----------------------------------------------------------------------- */

/** \class beofstream
    \brief beofstream specializes the \c bofstream class to store variables
    in big endian format. */

class TOOLS_API beofstream : public bofstream
{

public:

  /// @name Constructors
  //@{

  /** Constructs a writable big endian byte ordered binary file stream
      \e file_name. */
  beofstream( const char * file_name ) :
    bofstream(file_name)
  {
  }

  /** Constructs a writable big endian byte ordered binary file stream
      \e file_name. */
  beofstream( const std::string& file_name ) :
    bofstream(file_name.c_str())
  {
  }

  virtual ~beofstream();

  //@}
private:

#if __BYTE_ORDER == __LITTLE_ENDIAN
  virtual bofstream& _writeBytes( const char * data, size_t size )
  {
    register char flipped_data[8];
    flipBytes(data,flipped_data,size);
    __stream.write(flipped_data,size);
    return *this;
  }
#endif

};


/* ----------------------------------------------------------------------- */

/** \class beifstream
    \brief beifstream specializes the \c bifstream class to restore variables
    in big endian format. */

class TOOLS_API beifstream : public bifstream
{

public:

  /// @name Constructors
  //@{

  /** Constructs a readable big endian byte ordered binary file stream
      \e file_name. */
  beifstream( const char * file_name ) :
    bifstream(file_name)
  {
  }

  beifstream( const std::string& file_name ) :
    bifstream(file_name.c_str())
  {
  }

  virtual ~beifstream();

  //@}

private:

#if __BYTE_ORDER == __LITTLE_ENDIAN
 virtual bifstream& _readBytes( char * data, size_t size )
  {
    register char flipped_data[8];
    __stream.read(flipped_data,size);
    flipBytes(flipped_data,data,size);
    return *this;
  }
#endif

};


/* ----------------------------------------------------------------------- */

TOOLS_END_NAMESPACE

/* ----------------------------------------------------------------------- */

// __bfstream_h__
#endif
