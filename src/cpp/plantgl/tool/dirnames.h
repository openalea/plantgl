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





#ifndef _DIRNAMES_H__
#define _DIRNAMES_H__

/*! \file dirnames.h
    \brief File for filename queries.
*/

#include <string>
#include "tools_config.h"

PGL_BEGIN_NAMESPACE

/*! get the directory part of a filename (the filename might be not expanded).
    the filename stored  "../../fruit.drf" or fruit.drf for example.
    get_dirname() returns ../.. and . respectively
*/
std::string TOOLS_API get_dirname(const std::string& filename);

/*! get the file part (not directory name part) of a filename (the filename might be not expanded).
    the filename might be "../../fruit.drf" or fruit.drf for example.
    get_notdirname returns fruit.drf in both cases.
*/
std::string TOOLS_API get_filename(const std::string& filename);

/*! Expands the forms ./../ in a filename and resolves the symbolic links.
 */
std::string TOOLS_API absolute_dirname(const std::string& filename);

/*! Expands the forms ./../ in a filename and resolves the symbolic links.
 */
std::string TOOLS_API absolute_filename(const std::string& filename);


/*! Expands filename in a short name on win32 else do nothing.
 */
std::string TOOLS_API short_dirname(const std::string& filename);


/*! Gets the name of the current working directory. The string is empty if an error occured.
 */
std::string TOOLS_API get_cwd();

/*! changes the current working directory to newdir. New dir can integrate '.' chars that
    are thus interpreted according to the former working directory. Returns true if succeeded.
 */
bool TOOLS_API chg_dir(const std::string& newdir);


/*! cat a dirname and a filename to make up a complete filename
    ex: cat_dir_file("../foodir", "name1") -> "../foodir/name1"
    ex: cat_dir_file("../foodir/", "name1")-> "../foodir/name1"
*/
std::string TOOLS_API cat_dir_file(const std::string &filename, const std::string &);

/*!
 Return the extension of filename. If none exist, return empty string.
*/
std::string TOOLS_API get_suffix(const std::string& filename);

/*!
  Change the current extension of the filename by \e ext.
*/
std::string TOOLS_API set_suffix(const std::string& filename,const std::string& ext);

/*!
  Return whether the file already exists.
*/
bool TOOLS_API exists(const std::string& filename);

/*!
  Return whether 2 filename are similar.
*/
bool TOOLS_API similar_dir(const std::string& filename,const std::string& filename2);

bool TOOLS_API copy(const std::string& src,const std::string& dest);

PGL_END_NAMESPACE

#endif
