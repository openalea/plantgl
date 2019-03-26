/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
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

#include "../gui_config.h"
#include "../base/viewer.h"
#include <plantgl/scenegraph/scene/scene.h>

/* ----------------------------------------------------------------------- */

/**
   \class PGLViewer
   \brief The Main Window of the viewer
*/

/* ----------------------------------------------------------------------- */

class VIEW_API PGLViewer : public Viewer
{
public:

  /// Constructor.
  PGLViewer(  QWidget * parent = 0, Qt::WindowFlags f = 0 );

  /// Constructor.
  PGLViewer( int argc, char ** argv );

  /// Destructor.
  virtual ~PGLViewer();

  void changeScene( const PGL(ScenePtr)& s );

};


/* ----------------------------------------------------------------------- */

// __view_viewer_h__
#endif
