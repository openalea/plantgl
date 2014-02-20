/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): Ch. Nouguier (christophe.nouguier@cirad.fr)
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
        \file std.h
    \brief File that include all the stl.
*/


#ifndef __std_h__
#define __std_h__

#include "tools_config.h"

// Basic stl support
#include <functional>
#include <iterator>
#include <memory>
#include <numeric>
#include <utility>

// Containers support
#include <vector>
#include <list>
#include <deque>
#include <set> // contains also multiset
#include <map> // contains also multimap

// hash set
#include "util_hashmap.h"
#include "util_hashset.h"


#include <stack>
#include <queue> // contains also priority_queue

// Algorithms support
#include <algorithm>

// Stream support
#include <iostream>
#include <fstream>

#include <sstream>

// String support
#include <string>

// RTTI (Run-Time Type Identification) support
#include <typeinfo>

//using namespace std;




/* ----------------------------------------------------------------------- */

// __std_h__
#endif
