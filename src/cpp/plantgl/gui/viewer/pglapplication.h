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

  static std::vector<std::pair<uint_t,double> > getProjectionSizes(const PGL(ScenePtr)& sc);
  static std::vector<std::pair<uint_t,uint_t> > getProjectionPerShape(double& pixelsize);

  static ViewRayPointHitBuffer * castRays2(const PGL(ScenePtr)& sc, bool back_test = false );


  static void init();
};

/* ----------------------------------------------------------------------- */
#endif

