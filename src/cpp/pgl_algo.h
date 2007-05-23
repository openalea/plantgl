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


/*! \file all_action.h
    \brief Include of all the action.
*/

#ifndef __action_h__
#define __action_h__

/// Discretization
#include "algo/base/discretizer.h"
#include "algo/base/tesselator.h"
#include "algo/base/merge.h"


/// Printer
#include "algo/codec/printer.h"
#include "algo/codec/binaryprinter.h"
#include "algo/codec/plyprinter.h"
#include "algo/codec/binaryprinter.h"
#include "algo/codec/vrmlprinter.h"
#include "algo/codec/povprinter.h"
#include "algo/codec/linetreeprinter.h"
#include "algo/codec/vgstarprinter.h"
#include "algo/codec/x3dprinter.h"

/// Reader
#include "algo/codec/dtafile.h"
#include "algo/codec/ligfile.h"
#include "algo/codec/vgsfile.h"
#include "algo/codec/scne_binaryparser.h"
#include "algo/codec/scne_scanner.h"
#include "algo/codec/scne_parser.h"

/// Codec
#include "algo/codec/codecs.h"
#include "algo/codec/cdc_geom.h"
#include "algo/codec/cdc_pov.h"
#include "algo/codec/cdc_vgstar.h"

/// GL Renderer
#include "algo/opengl/glrenderer.h"
#include "algo/opengl/glbboxrenderer.h"
#include "algo/opengl/glctrlptrenderer.h"
#include "algo/opengl/glskelrenderer.h"
#include "algo/opengl/gltransitionrenderer.h"


/// Various computation
#include "algo/base/bboxcomputer.h"
#include "algo/base/bspherecomputer.h"
#include "algo/base/skelcomputer.h"
#include "algo/base/surfcomputer.h"
#include "algo/base/volcomputer.h"
#include "algo/base/polygoncomputer.h"
#include "algo/base/wirecomputer.h"
#include "algo/base/statisticcomputer.h"
#include "algo/base/matrixcomputer.h"

/// Extension
#include "algo/fitting/fit.h"
#include "algo/base/amaptranslator.h"

// __action_h__
#endif
