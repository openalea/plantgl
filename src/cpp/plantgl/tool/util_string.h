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

/*!
	\file util_string.h
    \brief File that contains some utility about string.
*/

#ifndef __util_string__
#define __util_string__

#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <sstream>
#include <vector>
#include "tools_config.h"

PGL_BEGIN_NAMESPACE

#define NUMBER(i) \
  std::stringstream s; \
  s << i; \
  return s.str();


/// Transform a number into a string
  inline std::string number(long int i) {
    NUMBER(i)
  }

/// Transform a number into a string
  inline std::string number(unsigned long int i) {
    NUMBER(i)
  }

/// Transform a number into a string
  inline std::string number(int i) {
    NUMBER(i)
  }

/// Transform a number into a string
  inline std::string number(unsigned int i) {
    NUMBER(i)
  }

/// Transform a number into a string
  inline std::string number(short int i) {
    NUMBER(i)
  }

/// Transform a number into a string
  inline std::string number(unsigned short int i) {
    NUMBER(i)
  }

/// Transform a number into a string
  inline std::string number(double i) {
    NUMBER(i)
  }

/// Transform a number into a string
  inline std::string number(float i) {
    NUMBER(i)
  }

/// Transform a number into a string
  inline std::string number(unsigned char i) {
    NUMBER((int) i)
  }

/// Transform a number into a string
  inline std::string number(char i) {
    NUMBER((int) i)
  }

#ifdef PGL_64_ENV

  /// Transform a number into a string
  inline std::string number(unsigned long long int i) {
    NUMBER(i)
  }


  /// Transform a number into a string
  inline std::string number(long long int i) {
    NUMBER(i)
  }

#endif

  /// Transform a string into a number of type T
  template<typename T>
  inline T toNumber(const std::string &c) {
    std::stringstream s(c);
    T n;
    s >> n;
    return n;
  }

/// Transform all character of the string into lower character.
  inline std::string toLower(const std::string &c) {
    std::string res(c);
    for (std::string::iterator _it = res.begin(); _it != res.end(); _it++)*_it = tolower(*_it);
    return res;
  }

/// Transform all character of the string into upper character.
  inline std::string toUpper(const std::string &c) {
    std::string res(c);
    for (std::string::iterator _it = res.begin(); _it != res.end(); _it++)*_it = toupper(*_it);
    return res;
  }

  inline std::string strip(const std::string &c) {
    std::string res(c);
    while (res.find_first_of(" ") == 0) res.erase(0, 1);
    while (res.find_last_of(" ") == res.size() - 1) res.erase(res.size() - 1, 1);
    return res;
  }

  inline std::vector<std::string> split(const std::string &c, const std::string &delim = " ") {
    std::vector<std::string> vec;
    std::size_t start = 0;
    std::size_t end = 0;

    do {
      end = c.find(delim, start);
      vec.push_back(c.substr(start, end - start));
      start = end + delim.length();
    } while (end != std::string::npos);

    std::vector<std::string>::iterator it = vec.begin() + 1;
    while (it != vec.end()) {
      if (it->empty())
        it = vec.erase(it);
      else
        it++;
    }
    return vec;
  }

PGL_END_NAMESPACE

#endif
