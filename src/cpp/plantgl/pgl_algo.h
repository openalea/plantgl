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
