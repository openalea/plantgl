/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry 
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): C. Nouguier & F. Boudon
 *                       N. Dones & B. Adam
 *
 *       $Source: /usr/cvsmaster/AMAPmod/src/GEOM/view_viewer.h,v $
 *       $Id: view_viewer.h,v 1.46 2006/06/16 13:04:46 fboudon Exp $
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
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


/*! \file view_viewer.h
    \brief Definition of the main viewer class : Viewer.
*/


#ifndef __view_pglviewer_h__
#define __view_pglviewer_h__

/* ----------------------------------------------------------------------- */

#include "../gui3_config.h"
#include "../base/viewer.h"
#include <scenegraph/scene/scene.h>

/* ----------------------------------------------------------------------- */

/**   
   \class PGLViewer3
   \brief The Main Window of the viewer
*/

/* ----------------------------------------------------------------------- */

class VIEW3_API PGLViewer3 : public Viewer3
{
public:
   
  /// Constructor.
  PGLViewer3(  QWidget * parent = 0, WFlags f = 0 );

  /// Constructor.
  PGLViewer3( int argc, char ** argv );

  /// Destructor.
  virtual ~PGLViewer3();
  
  void changeScene( const PGL(ScenePtr)& s );

};


/* ----------------------------------------------------------------------- */

// __view_viewer_h__
#endif
