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



#ifndef __util_mutex_h__
#define __util_mutex_h__

#include "tools_config.h"

#ifndef PGL_WITHOUT_QT

#include <QtCore/qglobal.h>

#ifdef QT_THREAD_SUPPORT
#include <QtCore/qmutex.h>
#define PGL_THREAD_SUPPORT
#define PglMutex QMutex
#endif

#else

#if _MSC_VER == 1500  
// PGL_THREAD_SUPPORT not activated
#else

#include <mutex>
#define PGL_THREAD_SUPPORT
#define PglMutex std::mutex

#endif

#endif

#endif