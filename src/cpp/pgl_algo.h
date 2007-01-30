/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr)
 *
 *       $Source: /usr/cvsmaster/AMAPmod/src/GEOM/all_action.h,v $
 *       $Id: all_action.h,v 1.3 2003/08/21 13:22:36 pradal Exp $
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


/*! \file all_action.h
    \brief Include of all the action.
*/

#ifndef __action_h__
#define __action_h__

/// Discretization
#include "actn_discretizer.h"
#include "actn_tesselator.h"


/// Printer
#include "actn_printer.h"
#include "actn_binaryprinter.h"
#include "actn_plyprinter.h"
#include "actn_binaryprinter.h"
#include "actn_vrmlprinter.h"
#include "actn_povprinter.h"


/// GL Renderer
#include "actn_glrenderer.h"
#include "actn_glbboxrenderer.h"
#include "actn_glctrlptrenderer.h"
#include "actn_glskelrenderer.h"
#include "actn_gltransitionrenderer.h"


/// Various computation
#include "actn_bboxcomputer.h"
#include "actn_skelcomputer.h"
#include "actn_surfcomputer.h"
#include "actn_polygoncomputer.h"
#include "actn_statisticcomputer.h"

/// Extension
#include "Geomext/actn_fit.h"
#include "actn_amaptranslator.h"

// __action_h__
#endif
