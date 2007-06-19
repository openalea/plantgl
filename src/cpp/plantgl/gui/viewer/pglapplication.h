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
 *       $Source: /usr/cvsmaster/AMAPmod/src/GEOM/view_application.h,v $
 *       $Id: view_application.h,v 1.20 2006/08/12 21:44:09 barbier Exp $
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

/*! \file view_application.h
    \brief Definition of the viewer class PGLViewerApplication.
*/

#ifndef __view_pglapplication_h__
#define __view_pglapplication_h__

/* ----------------------------------------------------------------------- */

#include "../gui_config.h"
#include "../base/application.h"
#include <plantgl/scenegraph/scene/scene.h>
#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/math/util_vector.h>

/* ----------------------------------------------------------------------- */

class ViewRayPointHitBuffer;


/* ----------------------------------------------------------------------- */

/**
      \class PGLPGLViewerApplication
      \brief This class manage thread or not application for the viewer

*/

/* ----------------------------------------------------------------------- */

class VIEW_API PGLViewerApplication : public ViewerApplication {

public :

  /// Constructor.
  PGLViewerApplication();

  /// Destructor.
  virtual ~PGLViewerApplication();

  /// Display the scene _scene.
  static void display(const PGL(ScenePtr)& s);
  static void display(const PGL(GeometryPtr)& g);
  static void add(const PGL(ScenePtr)& s);
  static void add(const PGL(GeometryPtr)& g);

  static PGL(ScenePtr) getCurrentScene();

  static std::vector<std::pair<uint32_t,double> > getProjectionSizes(const PGL(ScenePtr)& sc); 

  static ViewRayPointHitBuffer * castRays2(const PGL(ScenePtr)& sc, bool back_test = false );


  static void init();
};

/* ----------------------------------------------------------------------- */
#endif

