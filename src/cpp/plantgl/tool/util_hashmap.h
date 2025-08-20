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



#ifndef __util_pgl_hashmap_h__
#define __util_pgl_hashmap_h__
#include <string>
#include "tools_config.h"

/*! \file util_hashmap.h
    \brief Utility for hashmap with std::string.
*/

#ifdef USING_UNORDERED_MAP

#ifdef GNU_TR1_STL_EXTENSION
	#include <tr1/unordered_map>
	#define pgl_hash_map std::tr1::unordered_map

	#ifndef pgl_hash
		#define pgl_hash std::tr1::hash
	#endif

#else
    #include <unordered_map>
	#if (_MSC_VER == 1500)
		#define pgl_hash_map std::tr1::unordered_map
	#else
		#define pgl_hash_map std::unordered_map
	#endif
	#ifndef pgl_hash
		#define pgl_hash std::hash
	#endif
#endif



template <class T>
struct pgl_hash_map_string : public pgl_hash_map<std::string, T >{};

#else

#ifdef GNU_TR1_STL_EXTENSION
	#include <ext/hash_map>
#else
	#if defined(__GNUC__)
		#warning GNU STL Extension not activated ! Old GCC version used ?
	#endif
	#include <hash_map>
#endif

#ifndef pgl_hash
#define pgl_hash STDEXT::hash
#endif
#define pgl_hash_map STDEXT::hash_map

#include "util_hash.h"

#ifndef WIN32_STL_EXTENSION

/**
   \class hash_map_string
   \brief Class used for parsing a stream.
*/

template <class T>
struct pgl_hash_map_string : public pgl_hash_map<std::string, T, pgl_hashstr, pgl_eqstr>
{};

#else

template <class T>
struct pgl_hash_map_string : public pgl_hash_map<std::string, T >
{};

#endif

#endif

#endif
