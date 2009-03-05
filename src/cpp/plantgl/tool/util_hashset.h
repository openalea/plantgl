/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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


#ifndef __util_hashset_h__
#define __util_hashset_h__

/*! \file util_hashset.h
    \brief Utility for hashset with std::string.
*/
#include "tools_config.h"

#ifndef USING_OLD_HASHMAP
#ifdef GNU_STL_EXTENSION
	#include <tr1/unordered_set>
	#define pgl_hash_set std::tr1::unordered_set
#else
	#include <unordered_set>
	#define pgl_hash_set unordered_set
#endif

#ifndef pgl_hash
#define pgl_hash std::tr1::hash
#endif

typedef pgl_hash_set<std::string> hash_set_string ;
typedef pgl_hash_set<uint_t> hash_set_uint32;

#else

#ifdef GNU_STL_EXTENSION
	#include <ext/hash_set>
#else
	#if defined(__GNUC__)
		#warning GNU STL Extension not activated ! Old GCC version used ?
	#endif
	#include <hash_set>
#endif

#ifndef pgl_hash
#define pgl_hash STDEXT::hash
#endif
#define pgl_hash_set STDEXT::hash_set

#include "util_hash.h"

#ifndef WIN32_STL_EXTENSION

/**
   \class hash_set_string
   \brief Class for using hash_set with string.
*/

typedef pgl_hash_set<std::string, hashstr, eqstr> hash_set_string;
typedef pgl_hash_set<uint_t,pgl_hash<uint_t>,std::equal_to<uint_t> > hash_set_uint32;

#else

typedef pgl_hash_set<std::string> hash_set_string ;
typedef pgl_hash_set<uint_t> hash_set_uint32;


#endif

#endif

#endif
