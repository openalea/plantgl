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




/* ----------------------------------------------------------------------- */

#include "bfstream.h"

/* ----------------------------------------------------------------------- */
#ifdef _MSC_VER
#define STRING(val) ##val
#pragma message("_MSC_VER:" STRING(_MSC_VER))
#ifdef _WIN64
#pragma message("64bit ARCHITECTURE TARGETED")
#else
#ifdef _WIN32
#pragma message("32bit ARCHITECTURE TARGETED")
#else
#pragma message("NO ARCHITECTURE TARGETED")
#endif
#endif
#endif

TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

/// Destructor.
bofstream::~bofstream() { }

/* ----------------------------------------------------------------------- */

/// Destructor.
bifstream::~bifstream() { }

/* ----------------------------------------------------------------------- */

/// Destructor.
leofstream::~leofstream() { }

/* ----------------------------------------------------------------------- */

/// Destructor.
leifstream::~leifstream() { }

/* ----------------------------------------------------------------------- */

/// Destructor.
beofstream::~beofstream() { }

/* ----------------------------------------------------------------------- */

/// Destructor.
beifstream::~beifstream() { }


