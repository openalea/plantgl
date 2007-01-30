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



#include "pglviewer.h"
#include "editgeomscenegl.h"
#include "geomevent.h"

/* ----------------------------------------------------------------------- */


  /// Constructor.
PGLViewer::PGLViewer(  QWidget * parent, WFlags f ):
	Viewer(parent,"",new ViewMultiscaleEditGeomSceneGL(NULL,NULL), f)
{
}

  /// Constructor.
PGLViewer::PGLViewer( int argc, char ** argv ):
	Viewer(argc, argv,new ViewMultiscaleEditGeomSceneGL(NULL,NULL))
{
}

/// Destructor.
PGLViewer::~PGLViewer()
{
}
  
void PGLViewer::changeScene( const PGL(ScenePtr)& s )
{
  GeomSceneChangeEvent k( s );
  getSceneRenderer()->sceneChangeEvent( &k );
}


