/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): Ch. Godin (christophe.godin@cirad.fr)
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



#ifndef __util_hashmap_h__
#define __util_hashmap_h__

#include "config.h"

/*! \file util_hashmap.h
    \brief Utility for hashmap with std::string.
*/

#ifdef GNU_STL_EXTENSION
	#include <ext/hash_map>
	#include <ext/hash_set>
#else
	#if defined(__GNUC__)
		#warning GNU STL Extension not activated ! Old GCC version used ?
	#endif
	#include <hash_set>
	#include <hash_map>
#endif

#include <string>

#ifndef WIN32_STL_EXTENSION

/**
   \strust eqstr
   \brief Comparison between 2 string.
*/

struct eqstr
{
  /// Compare the 2 string.
  bool operator() (const std::string& s1, const std::string& s2) const
    { return s1 == s2; }
};


/**
   \strust hashstr
   \brief Find using a hasher a place for the string.
*/
struct hashstr
{
  /// hash of the string.
  size_t operator() (const std::string& s1) const
    {
    STDEXT::hash<const char*> my_hasher;
    return my_hasher(s1.c_str());
    }
};


/**
   \class hash_map_string
   \brief Class used for parsing a stream.
*/

template <class T>
struct hash_map_string : public STDEXT::hash_map<std::string, T, hashstr, eqstr>
{};

struct hash_set_string : public STDEXT::hash_set<std::string, hashstr, eqstr>
{};

#else

template <class T>
struct hash_map_string : public STDEXT::hash_map<std::string, T >
{};

struct hash_set_string : public STDEXT::hash_set<std::string>
{};

#endif

#endif
