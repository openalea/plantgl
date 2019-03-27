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
