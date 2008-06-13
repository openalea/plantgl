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


#ifndef __util_hashmap_h__
#define __util_hashmap_h__

#include "tools_config.h"

/*! \file util_hashmap.h
    \brief Utility for hashmap with std::string.
*/

#ifdef GNU_STL_EXTENSION
	#include <ext/hash_map>
#else
	#if defined(__GNUC__)
		#warning GNU STL Extension not activated ! Old GCC version used ?
	#endif
	#include <hash_map>
#endif

#include "util_hash.h"

#ifndef WIN32_STL_EXTENSION

/**
   \class hash_map_string
   \brief Class used for parsing a stream.
*/

template <class T>
struct hash_map_string : public STDEXT::hash_map<std::string, T, hashstr, eqstr>
{};

#else

template <class T>
struct hash_map_string : public STDEXT::hash_map<std::string, T >
{};

#endif

#endif
