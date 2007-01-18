/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon (boudon@cirad.fr)
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

/*! 
	\file util_string.h
    \brief File that contains some utility about string.
*/

#ifndef __util_string__
#define __util_string__


#include <stdio.h>
#include <ctype.h>
#include <string>
#include <sstream>
#include "tools_namespace.h"

TOOLS_BEGIN_NAMESPACE

#define NUMBER(i) \
  std::stringstream s; \
  s << i; \
  return s.str();


/// Transform a number into a string
inline std::string number( long int i) {
  NUMBER(i)
/*
  char * tmp = new char[64];
  sprintf(tmp,"%li",i);
  return std::string(tmp);
*/
}

/// Transform a number into a string
inline std::string number ( unsigned long int i ) {
  NUMBER(i)
/*
  char * tmp = new char[64];
  sprintf(tmp,"%lu",i);
  return std::string(tmp);
*/
}

/// Transform a number into a string
inline std::string  number ( int i ) {
  NUMBER(i)
/*
  char * tmp = new char[64];
  sprintf(tmp,"%i",i);
  return std::string(tmp);
*/
}

/// Transform a number into a string
inline std::string  number ( unsigned int i) {
  NUMBER(i)
/*
  char * tmp = new char[64];
  sprintf(tmp,"%u",i);
  return std::string(tmp);
*/
}

/// Transform a number into a string
inline std::string  number ( short int i ) {
  NUMBER(i)
/*
  char * tmp = new char[64];
  sprintf(tmp,"%hi",i);
  return std::string(tmp);
*/
}

/// Transform a number into a string
inline std::string  number ( unsigned short int i) {
  NUMBER(i)
/*
  char * tmp = new char[64];
  sprintf(tmp,"%hu",i);
  return std::string(tmp);
*/
}

/// Transform a number into a string
inline std::string  number ( double i ) {
  NUMBER(i)
/*
  char * tmp = new char[64];
  sprintf(tmp,"%e",i);
  return std::string(tmp);
*/
}

/// Transform a number into a string
inline std::string  number ( float i ) {
  NUMBER(i)
/*
  char * tmp = new char[64];
  sprintf(tmp,"%e",i);
  return std::string(tmp);
*/
}

/// Transform a number into a string
inline std::string  number ( unsigned char i ) {
  NUMBER((int)i)
/*
  char * tmp = new char[64];
  sprintf(tmp,"%hhi",i);
  return std::string(tmp);
*/
}

/// Transform a number into a string
inline std::string  number ( char i ) {
  NUMBER((int)i)
/*
  char * tmp = new char[64];
  sprintf(tmp,"%hhi",i);
  return std::string(tmp);
*/
}

/// Transform all character of the string into lower character.
inline std::string toLower(const std::string& c){
  std::string res(c);
  for(std::string::iterator _it = res.begin(); _it != res.end(); _it++)*_it = tolower(*_it);
  return res;
}

/// Transform all character of the string into upper character.
inline std::string toUpper(const std::string& c){
  std::string res(c);
  for(std::string::iterator _it = res.begin(); _it != res.end(); _it++)*_it = toupper(*_it);
  return res;
}

TOOLS_END_NAMESPACE

#endif
