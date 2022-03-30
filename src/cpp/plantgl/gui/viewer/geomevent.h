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


/*! \file view_geomevent.h
    \brief Definition of the viewer class events about geom.
*/


#ifndef __view_geomevent_h__
#define __view_geomevent_h__

/* ----------------------------------------------------------------------- */

#include "../base/event.h"
#include <plantgl/scenegraph/scene/scene.h>

/* ----------------------------------------------------------------------- */

/**
    \class GeomSceneChangeEvent
    \brief Event for GEOM Scene Change.
*/

/* ----------------------------------------------------------------------- */


class VIEW_API GeomSceneChangeEvent : public ViewSceneChangeEvent {


  public :
      enum GeomSceneEventType {
             eFirstGeomSceneEvent = 0,
             eGeomSceneEvent = eFirstGeomSceneEvent,
             eMultiSceneEvent,
             eLastGeomSceneEvent
      };


  /// Constructor.
  GeomSceneChangeEvent(PGL(ScenePtr) scene,
               const QString& = QString(),
               const QString& _file = QString(),
               bool add = false);

  /// Destructor.
  ~GeomSceneChangeEvent();

  /// copy object.
  virtual ViewSceneChangeEvent * copy();

  /// The scene to display.
  PGL(ScenePtr) scene;

  /// The error message to display.
  QString error;

  /// File.
  QString file;

  bool addition;

};

/**
    \class GeomMultiSceneChangeEvent
    \brief Event for GEOM Scene Change using multiple scene.
*/
class VIEW_API GeomMultiSceneChangeEvent : public GeomSceneChangeEvent {

  public :

  /// Constructor.
  GeomMultiSceneChangeEvent(PGL(ScenePtr) scene1,
                PGL(ScenePtr) scene2,
                const QString& errlog = QString(),
                const QString& file1 = QString(),
                const QString& file2 = QString());

  /// Destructor.
  ~GeomMultiSceneChangeEvent();

  /// copy object.
  virtual ViewSceneChangeEvent * copy();

  /// The scene to display.
  PGL(ScenePtr) scene2;

  /// File.
  QString file2;

};

/* ----------------------------------------------------------------------- */

typedef TViewGeomEvent<ViewGeomEvent::eGetScene,PGL(ScenePtr)> GeomGetSceneEvent;
typedef TViewGeomEvent<ViewGeomEvent::eProjList,std::vector<std::pair<uint_t,double> >,PGL(ScenePtr)> GeomProjListEvent;
typedef TViewGeomEvent<ViewGeomEvent::eIntegratedProjList,std::vector<std::pair<uint_t,uint_t> >,double*> ViewIntegratedProjListEvent;
class ViewRayPointHitBuffer;
typedef TViewGeomEvent<ViewGeomEvent::eRayBuff2,ViewRayPointHitBuffer *,PGL(ScenePtr),bool> ViewRayBuff2Event;


/* ----------------------------------------------------------------------- */

#endif

