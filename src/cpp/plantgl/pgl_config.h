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



#ifndef __pgl_config_h__
#define __pgl_config_h__

/* ----------------------------------------------------------------------- */
/*! \file config.h
 *  \brief Configuration file
 *
 *   This file control the compilation with some predefined macro.\n
 *      Uncomment the macro for particular compilation option. \n
 *      - Use double instead of float for real_t values \n
 *      \#define \b PGL_USE_DOUBLE \n\n
 *      - Make debug code and output. \n
 *      \#define \b GEOM_DEBUG \n\n
 *      - Make debug code  and output about reference counting object. \n
 *      \#define \b RCOBJECT_DEBUG \n\n
 *      - Compile without namespace. \n
 *      \#define \b NO_NAMESPACE \n\n
 *      - Force the use of the lib glut. \n
 *      \#define \b WITH_GLUT \n\n
 *      - Force to not use the lib glut. \n
 *      \#define \b WITHOUT_GLUT \n\n
 *      - Force to not use the lib RogueWave. \n
 *      \#define \b RWOUT \n\n
 *      - Control if bison output some cpp.h or hpp file for some include \n
 *  If bison version is >= 1.30 then uncomment this macro. \n
 *  On windows force bison hpp by default. \n
 *      \#define \b BISON_HPP \n\n
 *      - Use forward definition \n
 *   By default, on windows when making DLL force not using fwd declaration, Else use it. \n
 *      \#define \b GEOM_FWDEF \n\n
 *      - Not creating dll (Win32 only) \n
 *      \#define \b GEOM_NODLL \n\n
 *      - Using lib GEOM as a dll (Win32 only) \n
 *      \#define \b GEOM_DLL \n\n
 *      - Creating GEOM dll (Win32 only) \n
 *      \#define \b GEOM_MAKEDLL
 *
 */


/*! \def PGL_USE_DOUBLE
    \brief Use double instead of float for real_t values

    Uncomment to use this functionnality
*/
#ifndef PGL_USE_FLOAT

#ifndef PGL_USE_DOUBLE
#define PGL_USE_DOUBLE
#endif

#endif



/*! \def GEOM_DEBUG
    \brief Make debug code and output.

    Uncomment to use this functionnality
*/
// #define GEOM_DEBUG

/*! \def RCOBJECT_DEBUG
    \brief Make debug code  and output about reference counting object.

    Uncomment to use this functionnality
*/
// #define RCOBJECT_DEBUG


/*! \def NO_NAMESPACE
    \brief Compile without namespace.

    Uncomment to use this functionnality
*/
// #define NO_NAMESPACE

#ifndef NO_NAMESPACE


/*! \def GEOM_NAMESPACE_NAME
    \brief Redefined the GEOM namespace name.

    Uncomment to use this functionnality
*/
#define PGL_NAMESPACE_NAME PGL


#endif

/*! \def WITH_GLUT
    \brief force the use of the lib glut.

    Uncomment to use this functionnality
*/
// #define WITH_GLUT

#ifndef WITH_GLUT
#ifndef WITHOUT_GLUT

/*! \def WITHOUT_GLUT
    \brief force to not use the lib glut.

    Uncomment to use this functionnality
*/
#define WITHOUT_GLUT 1

#endif
#endif

/*! \def BISON_HPP
    \brief Macro used for bison output

    Control if bison output some cpp.h or hpp file for some include
        If bison version is >= 1.30 then uncomment this macro.
    On windows force bison hpp by default.
        Uncomment to use this functionnality
*/

#if defined(WITH_BISON) && defined(WITH_FLEX)

#define WITH_BISONFLEX

#ifndef BISON_HPP
#define BISON_HPP
#endif

#ifdef _WIN32
#ifndef BISON_HPP
#define BISON_HPP 1
#endif
#endif

#endif


/*! \def QT_DLL
    \brief QT Macro
*/
#ifndef QT_DLL
    #define QT_DLL
#endif

#ifndef QT_THREAD_SUPPORT
    #define QT_THREAD_SUPPORT
#endif

#ifdef PGL_WITHOUT_QT
    #define PGL_CORE_WITHOUT_QT
#endif

/*! \def GEOM_FWDEF
    \brief Use forward definition

    By default, on windows when making DLL force not using fwd declaration, Else use it.
        Uncomment to use this functionnality
*/
#define GEOM_FWDEF

/*! \def STL_EXTENSION
    \brief Include hash ccontainer as extension of STL
*/

#define USING_UNORDERED_MAP


#if defined(__GNUC__)
  #ifndef GNU_STL_EXTENSION

    #if defined(__MINGW32__) || defined(__APPLE__)
        // do not include
    #else
        #include <features.h>
    #endif

    #if defined(__clang__)
        #define STL_EXTENSION
    #elif defined (__GNUC_PREREQ)
      #if __GNUC_PREREQ(3,0)
        #define GNU_TR1_STL_EXTENSION
      #endif
    #elif defined (__MINGW32__)
      #define GNU_TR1_STL_EXTENSION
    #elif defined (__APPLE__)
      #define GNU_TR1_STL_EXTENSION
    #elif defined (SYSTEM_IS__CYGWIN)
      #define GNU_TR1_STL_EXTENSION
    #endif
    // #endif

  #endif
#endif

#ifdef GNU_TR1_STL_EXTENSION
    #define FMTFLAGS ios_base::fmtflags
#else
    #define FMTFLAGS unsigned long
#endif

#ifdef GNU_TR1_STL_EXTENSION
    #define STL_EXTENSION
#endif

#if _MSC_VER >= 1300 // Visual C++ 7
    #define WIN32_STL_EXTENSION
    #define STL_EXTENSION
#endif

#ifdef STL_EXTENSION
	#ifdef GNU_TR1_STL_EXTENSION
            #define STDEXT __gnu_cxx
	#elif defined (WIN32_STL_EXTENSION)
			#define STDEXT stdext
	#else
			#define STDEXT std
	#endif

	#define STDEXT_USING_NAMESPACE using namespace STDEXT;

#else
	#define STDEXT std
	#define STDEXT_USING_NAMESPACE
#endif



#ifndef USING_UNORDERED_MAP  // !defined(__GNUC__)  || defined (__APPLE__)
// Gcc use tr1 extension
// msvc did not integrate it yet.
    #define USING_OLD_HASHMAP
#endif


#if defined( _MSC_VER )
 // Turn off warnings generated by long std templates
 // This warns about truncation to 255 characters in debug/browse info
 #   pragma warning (disable : 4786) //  name was truncated in debug informations
 #   pragma warning (disable : 4250) //  'class' : inherits 'member' via dominance
 #   pragma warning (disable : 4251) //  class 'type' needs to have dll-interface to be used by clients of class 'type2'
 #   pragma warning (disable : 4275) //  DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
 #   pragma warning (disable : 4503) //  decorated name length exceeded, name was truncated
 #   pragma warning ( 1 : 4996)      //  'function' has been declared deprecated
#endif


/// deprecated attribute definition
#ifdef __GNUC__
    #define attribute_deprecated __attribute__((deprecated))
#elif defined( _MSC_VER )
    #define attribute_deprecated __declspec(deprecated)
#else
    #define attribute_deprecated
#endif
/* ----------------------------------------------------------------------- */

#if _WIN32 || _WIN64
    #if _WIN64
        #define PGL_64_ENV
    #else
        #define PGL_32_ENV
    #endif
#endif

#if __GNUC__
    #if __x86_64__ || __ppc64__
        #define PGL_64_ENV
    #else
        #define PGL_32_ENV
    #endif
#endif

// __config_h__
#endif
