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




#ifndef __util_pgl_hash_h__
#define __util_pgl_hash_h__

#include "tools_config.h"

/*! \file util_hash.h
    \brief Utility for hashing std::string.
*/

#include <string>


#ifndef WIN32_STL_EXTENSION

/**
   \strust eqstr
   \brief Comparison between 2 string.
*/

struct pgl_eqstr
{
  /// Compare the 2 string.
  bool operator() (const std::string& s1, const std::string& s2) const
    { return s1 == s2; }
};


/**
   \strust hashstr
   \brief Find using a hasher a place for the string.
*/
struct pgl_hashstr
{
  typedef pgl_hash<const char*> hash_cstr;

  /// hash of the string.
  size_t operator() (const std::string& s1) const
    { return my_hasher(s1.c_str()); }

   hash_cstr my_hasher;
};


#endif

#endif
