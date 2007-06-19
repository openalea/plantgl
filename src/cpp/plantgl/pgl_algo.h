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
#include "plantgl/algo/base/discretizer.h"
#include "plantgl/algo/base/tesselator.h"
#include "plantgl/algo/base/merge.h"


/// Printer
#include "plantgl/algo/codec/printer.h"
#include "plantgl/algo/codec/binaryprinter.h"
#include "plantgl/algo/codec/plyprinter.h"
#include "plantgl/algo/codec/binaryprinter.h"
#include "plantgl/algo/codec/vrmlprinter.h"
#include "plantgl/algo/codec/povprinter.h"
#include "plantgl/algo/codec/linetreeprinter.h"
#include "plantgl/algo/codec/vgstarprinter.h"
#include "plantgl/algo/codec/x3dprinter.h"

/// Reader
#include "plantgl/algo/codec/dtafile.h"
#include "plantgl/algo/codec/ligfile.h"
#include "plantgl/algo/codec/vgsfile.h"
#include "plantgl/algo/codec/scne_binaryparser.h"
#include "plantgl/algo/codec/scne_scanner.h"
#include "plantgl/algo/codec/scne_parser.h"

/// Codec
#include "plantgl/algo/codec/codecs.h"
#include "plantgl/algo/codec/cdc_geom.h"
#include "plantgl/algo/codec/cdc_pov.h"
#include "plantgl/algo/codec/cdc_vgstar.h"

/// GL Renderer
#include "plantgl/algo/opengl/glrenderer.h"
#include "plantgl/algo/opengl/glbboxrenderer.h"
#include "plantgl/algo/opengl/glctrlptrenderer.h"
#include "plantgl/algo/opengl/glskelrenderer.h"
#include "plantgl/algo/opengl/gltransitionrenderer.h"


/// Various computation
#include "plantgl/algo/base/bboxcomputer.h"
#include "plantgl/algo/base/bspherecomputer.h"
#include "plantgl/algo/base/skelcomputer.h"
#include "plantgl/algo/base/surfcomputer.h"
#include "plantgl/algo/base/volcomputer.h"
#include "plantgl/algo/base/polygoncomputer.h"
#include "plantgl/algo/base/wirecomputer.h"
#include "plantgl/algo/base/statisticcomputer.h"
#include "plantgl/algo/base/matrixcomputer.h"

/// Extension
#include "plantgl/algo/fitting/fit.h"
#include "plantgl/algo/base/amaptranslator.h"

// __action_h__
#endif
