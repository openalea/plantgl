/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP
 *
 *       File author(s): F. Boudon et al.
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


#ifndef __pgl_class_info__
#define __pgl_class_info__
/* ----------------------------------------------------------------------- */

#include "../scenegraph/sg_config.h"
#include "tools_config.h"

#include <string>

/* ----------------------------------------------------------------------- */

template<class T>
class PglClassInfo { public: static std::string name() { throw ; } };

#define PGL_DECLARE_TYPE(mtype) \
    PGL_END_NAMESPACE \
    template<> class PglClassInfo<PGL(mtype)> { public:  static std::string name() { return #mtype; } };  \
    PGL_BEGIN_NAMESPACE


#define TOOLS_DECLARE_TYPE(mtype) \
    TOOLS_END_NAMESPACE \
    template<> class PglClassInfo<TOOLS(mtype)> { public:  static std::string name() { return #mtype; } }; \
    TOOLS_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

// __pgl_class_info__
#endif
