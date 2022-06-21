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

#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define __BYTE_ORDER __LITTLE_ENDIAN
#else
    #define __BYTE_ORDER __BIG_ENDIAN
#endif

#else

#ifdef _MSC_VER
#pragma message("__BYTE_ORDER macro not defined. Use Little Endian by default.")
#else
#warning "__BYTE_ORDER macro not defined. Use Little Endian by default."
#endif
#define __BYTE_ORDER __LITTLE_ENDIAN
#endif

#endif

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/*! \fn void flipBytes( const char * src, char * dest, size_t n )
    \brief The flipBytes() function flips \e n bytes from the memory area
    \e src to the memory area \e dest. */
inline void flipBytes( const char * src, char * dest, size_t n )
{
        switch(n)
        {
        case 1:
                dest[0] = src[0];
                break;
        case 2:
                dest[0] = src[1], dest[1] = src[0];
                break;
        case 4:
                dest[0] = src[3], dest[1] = src[2];
                dest[2] = src[1], dest[3] = src[0];
                break;
        case 8:
                dest[0] = src[7], dest[1] = src[6];
                dest[2] = src[5], dest[3] = src[4];
                dest[4] = src[3], dest[5] = src[2];
                dest[6] = src[1], dest[7] = src[0];
                break;
        };
}

enum PglByteOrder {
    PglBigEndian,
    PglLittleEndian
} ;

/* ----------------------------------------------------------------------- */

/** \class fostream
    \brief fostream specializes the \c ofstream class to store variables
    big endian or little endian format. */

class TOOLS_API fostream
{

public:

/// @name Constructor
//@{

// /// Constructs a writable binary file stream \e file_name.
// bostream( const char * file_name, PglByteOrder byteorder = PglBigEndian) :
//         __stream(file_name,std::ios::out | std::ios::binary),
//         __order(byteorder)
// {
// }

// /// Constructs a writable binary file stream \e file_name.
// bostream( const std::string& file_name, PglByteOrder byteorder = PglBigEndian ) :
//         __stream(file_name.c_str(),std::ios::out | std::ios::binary),
//         __order(byteorder)
// {
// }

// Constructs a writable stream with given byte order.
fostream( std::ostream& stream, PglByteOrder byteorder = PglBigEndian) :
        __stream(stream),
        __order(byteorder)
{
}


//@}

/// @name Destructor
//@{

/// Destructor.
virtual ~fostream();

//@}

/** @name Writing functions
    Writes the appropriate built-in type to \e self. */
//@{

/// Binary write of a \b bool.
fostream& operator<<( bool b )
{
        return _writeBytes((char *)&b,sizeof(b));
}

/// Binary write of a \b char.
fostream& operator<<( char c )
{
        return _writeBytes((char *)&c,sizeof(c));
}

/// Binary write of a \b double.
fostream& operator<<( double d )
{
        return _writeBytes((char *)&d,sizeof(d));
}

/// Binary write of a \b float.
fostream& operator<<( float f )
{
        return _writeBytes((char *)&f,sizeof(f));
}

/// Binary write of a \b int.
fostream& operator<<( int i )
{
        return _writeBytes((char *)&i,sizeof(i));
}

/// Binary write of a \b long.
fostream& operator<<( long l )
{
        return _writeBytes((char *)&l,sizeof(l));
}

/// Binary write of a \b short.
fostream& operator<<( short s )
{
        return _writeBytes((char *)&s,sizeof(s));
}

/// Binary write of an \b unsigned \b char.
fostream& operator<<( unsigned char uc )
{
        return _writeBytes((char *)&uc,sizeof(uc));
}

/// Binary write of an \b unsigned \b int.
fostream& operator<<( unsigned int ui )
{
        return _writeBytes((char *)&ui,sizeof(ui));
}

/// Binary write of an \b unsigned \b long.
fostream& operator<<( unsigned long ul )
{
        return _writeBytes((char *)&ul,sizeof(ul));
}

/// Binary write of an \b unsigned \b short.
fostream& operator<<( unsigned short us )
{
        return _writeBytes((char *)&us,sizeof(us));
}

/// Binary write of a \b string.
fostream& operator<<( const char * s )
{
        __stream << s;
        return *this;
}

/// Binary write of a \b string.
fostream& operator<<( std::string s )
{
        __stream << s;
        return *this;
}

/// Binary write of a \b Tuple2.
template<class T>
fostream& operator<<( const Tuple2<T>& t )
{
        return (operator<<(t.getAt(0))).operator<<(t.getAt(1));
}

/// Binary write of a \b Tuple3.
template<class T>
fostream& operator<<( const Tuple3<T>& t )
{
        return ((operator<<(t.getAt(0))).operator<<(t.getAt(1))).operator<<(t.getAt(2));
}

/// Binary write of a \b Tuple4.
template<class T>
fostream& operator<<( const Tuple4<T>& t )
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
std::ostream& getStream() {
        return __stream;
}

/// Return the current stream.
const std::ostream& getStream() const {
        return __stream;
}

/// Returns true if \e stream  is valid.
operator bool( ) const { return (bool)(__stream); }

/// Returns true if \e stream  is not valid.
bool operator!( ) const {
        return !(bool)(__stream);
}

/// Returns true if \e stream  is at the end.
bool eof( ) const {
        return __stream.eof();
}

//@}

void setByteOrder(PglByteOrder order) {
    __order = order;
}

bool getByteOrder() const {
    return __order;
}


private:

virtual fostream& _writeBytes( const char * data, size_t size )
{
#if __BYTE_ORDER == __BIG_ENDIAN
    if (__order == PglLittleEndian)
#else
    if (__order == PglBigEndian)        
#endif
    {
        char flipped_data[8];
        flipBytes(data,flipped_data,size);
        __stream.write(flipped_data,size);
    }
    else {
        __stream.write(data,size);
    }
    return *this;
}

protected:

std::ostream& __stream;

PglByteOrder __order;

};


class TOOLS_API bofstream : public fostream
{

public:

/// @name Constructors
//@{

/** Constructs a writable little endian byte ordered binary file stream
    \e file_name. */
bofstream( const char * file_name, PglByteOrder byteorder = PglBigEndian ) :
        __fstream(file_name,std::ios::out | std::ios::binary),
        fostream(__fstream)

{
}

bofstream( const std::string& file_name, PglByteOrder byteorder = PglBigEndian ) :
        __fstream(file_name,std::ios::out | std::ios::binary),
        fostream(__fstream)
{
}

virtual ~bofstream();

protected:
   std::ofstream __fstream;

};


/* ----------------------------------------------------------------------- */

/** \class fistream
    \brief fistream specializes the \c istream class to restore variables
    in binary format. */

class TOOLS_API fistream
{

public:

/// @name Constructors
//@{

/// Constructs a readable binary file stream \e file_name.
/*fistream( const char * file_name ) :
        __stream(file_name,std::ios::in | std::ios::binary)
{
}

/// Constructs a readable binary file stream \e file_name.
fistream( const std::string& file_name ) :
        __stream(file_name.c_str(),std::ios::in | std::ios::binary)
{
}
*/


fistream( std::istream& stream, PglByteOrder byteorder = PglBigEndian) :
        __stream(stream),
        __order(byteorder)
{
}
//@}

/// @name Destructor
//@{

/// Destructor.
virtual ~fistream();

//@}

/** @name Reading functions
    Reads the appropriate built-in types from \e self. */
//@{

/// Binary read of a \b bool.
fistream& operator>>( bool& b )
{
        return _readBytes((char *)&b,sizeof(b));
}

/// Binary read of a \b char.
fistream& operator>>( char& c )
{
        return _readBytes((char *)&c,sizeof(c));
}

/// Binary read of a \b double.
fistream& operator>>( double& d )
{
        return _readBytes((char *)&d,sizeof(d));
}

/// Binary read of a \b float.
fistream& operator>>( float& f )
{
        return _readBytes((char *)&f,sizeof(f));
}

/// Binary read of an \b int.
fistream& operator>>( int& i )
{
        return _readBytes((char *)&i,sizeof(i));
}

/// Binary read of a \b long.
fistream& operator>>( long& l )
{
        return _readBytes((char *)&l,sizeof(l));
}

/// Binary read of a \b short.
fistream& operator>>( short& s )
{
        return _readBytes((char *)&s,sizeof(s));
}

/// Binary read of an \b unsigned \b char.
fistream& operator>>( unsigned char& uc )
{
        return _readBytes((char *)&uc,sizeof(uc));
}

/// Binary read of an \b unsigned \b int.
fistream& operator>>( unsigned int& ui )
{
        return _readBytes((char *)&ui,sizeof(ui));
}

/// Binary read of an \b unsigned \b long.
fistream& operator>>( unsigned long& ul )
{
        return _readBytes((char *)&ul,sizeof(ul));
}

/// Binary read of an \b unsigned \b short.
fistream& operator>>( unsigned short& us )
{
        return _readBytes((char *)&us,sizeof(us));
}

/// Binary read of a \b Tuple2.
template<class T>
fistream& operator>>( Tuple2<T>& t )
{
        return (operator>>(t.getAt(0))).operator>>(t.getAt(1));
}

/// Binary read of a \b Tuple3.
template<class T>
fistream& operator>>( Tuple3<T>& t )
{
        return ((operator>>(t.getAt(0))).operator>>(t.getAt(1))).operator>>(t.getAt(2));
}

/// Binary read of a \b Tuple4.
template<class T>
fistream& operator>>( Tuple4<T>& t )
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
std::istream& getStream() {
        return __stream;
}

/// Return the current stream.
const std::istream& getStream() const {
        return __stream;
}

/// Returns true if \e stream  is valid.
operator bool( ) const { return (bool)(__stream); }

/// Returns true if \e stream  is not valid.
bool operator!( ) const {
        return !(bool)(__stream);
}

/// Returns true if \e stream  is at the end.
bool eof( ) const {
        return __stream.eof();
}

//@}

void setByteOrder(PglByteOrder order) {
    __order = order;
}

bool getByteOrder() const {
    return __order;
}



private:

virtual fistream& _readBytes( char * data, size_t size )
{
#if __BYTE_ORDER == __BIG_ENDIAN
    if (__order == PglLittleEndian)
#else
    if (__order == PglBigEndian)        
#endif
    {
        char flipped_data[8];
        __stream.read(flipped_data,size);
        flipBytes(flipped_data,data,size);
    }
    else {
            __stream.read(data,size);
    }
    return *this;
}

protected:

    std::istream& __stream;

    PglByteOrder __order;

};


class TOOLS_API bifstream : public fistream
{

public:

/// @name Constructors
//@{

/** Constructs a writable little endian byte ordered binary file stream
    \e file_name. */
bifstream( const char * file_name, PglByteOrder byteorder = PglBigEndian ) :
        __fstream(file_name,std::ios::in | std::ios::binary),
        fistream(__fstream)

{
}

bifstream( const std::string& file_name, PglByteOrder byteorder = PglBigEndian ) :
        __fstream(file_name,std::ios::in | std::ios::binary),
        fistream(__fstream)
{
}

virtual ~bifstream();

protected:
   std::ifstream __fstream;

};

class TOOLS_API leifstream : public bifstream
{

public:

/// @name Constructors
//@{

/** Constructs a writable little endian byte ordered binary file stream
    \e file_name. */
    leifstream( const char * file_name) :
        bifstream(file_name, PglLittleEndian) {}    

    leifstream( const std::string& file_name) :
        bifstream(file_name, PglLittleEndian) {}    

    ~leifstream();

};

class TOOLS_API beifstream : public bifstream
{

public:

/// @name Constructors
//@{

/** Constructs a writable big endian byte ordered binary file stream
    \e file_name. */
    beifstream( const char * file_name) :
        bifstream(file_name, PglBigEndian) {}    

    beifstream( const std::string& file_name) :
        bifstream(file_name, PglBigEndian) {}    

    ~beifstream();

};


/* ----------------------------------------------------------------------- */

// class TOOLS_API bofstream : public bostream
// {

// public:

// /// @name Constructors
// //@{

// /** Constructs a writable little endian byte ordered binary file stream
//     \e file_name. */
// leofstream( const char * file_name ) :
//         bostream(file_name)
// {
// }

// leofstream( const std::string& file_name ) :
//         bostream(file_name.c_str())
// {
// }

// virtual ~leofstream();

// //@}

// private:

// #if __BYTE_ORDER == __BIG_ENDIAN
// virtual bostream& _writeBytes( const char * data, size_t size )
// {
//         char flipped_data[8];
//         flipBytes(data,flipped_data,size);
//         __stream.write(flipped_data,size);
//         return *this;
// }
// #endif

// };


/** \class leofstream
    \brief leofstream specializes the \c bostream class to store variables
    in little endian format. */

class TOOLS_API leofstream : public bofstream
{

public:

/// @name Constructors
//@{

/** Constructs a writable little endian byte ordered binary file stream
    \e file_name. */
leofstream( const char * file_name ) :
        bofstream(file_name, PglLittleEndian)
{
}

leofstream( const std::string& file_name ) :
        bofstream(file_name.c_str(), PglLittleEndian)
{
}

virtual ~leofstream();

};


// /* ----------------------------------------------------------------------- */

// /** \class leifstream
//     \brief bifstream specializes the \c bifstream class to restore variables
//     in little endian format. */

// class TOOLS_API leifstream : public bifstream
// {

// public:

// /// @name Constructors
// //@{

// /** Constructs a readable little endian byte ordered binary file stream
//    \e file_name. */
// leifstream( const char * file_name ) :
//         bifstream(file_name)
// {
// }

// leifstream( const std::string& file_name ) :
//         bifstream(file_name.c_str())
// {
// }

// virtual ~leifstream();

// //@}

// private:

// #if __BYTE_ORDER == __BIG_ENDIAN
// virtual bifstream& _readBytes( char * data, size_t size )
// {
//         char flipped_data[8];
//         __stream.read(flipped_data,size);
//         flipBytes(flipped_data,data,size);
//         return *this;
// }
// #endif

// };


// /* ----------------------------------------------------------------------- */

// /** \class beofstream
//     \brief beofstream specializes the \c bostream class to store variables
//     in big endian format. */

class TOOLS_API beofstream : public bofstream
{

public:

/// @name Constructors
//@{

/** Constructs a writable big endian byte ordered binary file stream
    \e file_name. */
beofstream( const char * file_name ) :
        bofstream(file_name, PglBigEndian)
{
}

/** Constructs a writable big endian byte ordered binary file stream
    \e file_name. */
beofstream( const std::string& file_name ) :
        bofstream(file_name.c_str(), PglBigEndian)
{
}

virtual ~beofstream();


};


// /* ----------------------------------------------------------------------- */

// /** \class beifstream
//     \brief beifstream specializes the \c bifstream class to restore variables
//     in big endian format. */

// class TOOLS_API beifstream : public bifstream
// {

// public:

// /// @name Constructors
// //@{

// /** Constructs a readable big endian byte ordered binary file stream
//     \e file_name. */
// beifstream( const char * file_name ) :
//         bifstream(file_name)
// {
// }

// beifstream( const std::string& file_name ) :
//         bifstream(file_name.c_str())
// {
// }

// virtual ~beifstream();

// //@}

// private:

// #if __BYTE_ORDER == __LITTLE_ENDIAN
// virtual bifstream& _readBytes( char * data, size_t size )
// {
//         char flipped_data[8];
//         __stream.read(flipped_data,size);
//         flipBytes(flipped_data,data,size);
//         return *this;
// }
// #endif

// };


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

// __bfstream_h__
#endif
