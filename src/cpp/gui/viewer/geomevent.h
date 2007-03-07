/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): F. Boudon
 *
 *       $Source$
 *       $Id$
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

/*! \file view_geomevent.h
    \brief Definition of the viewer class events about geom.
*/


#ifndef __view_geomevent_h__
#define __view_geomevent_h__

/* ----------------------------------------------------------------------- */

#include "../base/event.h"
#include <scenegraph/scene/scene.h>

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
		       const QString& = QString::null, 
		       const QString& _file = QString::null,
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
			    const QString& errlog = QString::null, 
			    const QString& file1 = QString::null,
			    const QString& file2 = QString::null);
  
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

class VIEW_API GeomGetSceneEvent : public ViewGeomEvent {
  
  public :
    
  /// Constructor.
  GeomGetSceneEvent(PGL(ScenePtr)* scene);
  
  /// Destructor.
  ~GeomGetSceneEvent();
    
  /// The scene to display.
  PGL(ScenePtr)* scene;
      
};

/* ----------------------------------------------------------------------- */

class GeomProjListEvent : public ViewGeomEvent {

public :

  /// Constructor.
	GeomProjListEvent(const PGL(ScenePtr)& sc, std::vector<std::pair<uint32_t,double> > * res);

  /// Destructor.
  ~GeomProjListEvent();

  std::vector<std::pair<uint32_t,double> > *  result;
  PGL(ScenePtr) objlist;
};

/* ----------------------------------------------------------------------- */
class ViewRayPointHitBuffer;

class ViewRayBuff2Event : public ViewGeomEvent {

public :

  /// Constructor.
	ViewRayBuff2Event(const PGL(ScenePtr)& sc, 
					  bool back_test = true,
					  ViewRayPointHitBuffer ** res = NULL);
	
	/// Destructor.
	~ViewRayBuff2Event();
	
	ViewRayPointHitBuffer **  result;
    PGL(ScenePtr) objlist;
	bool back_test;

};

/* ----------------------------------------------------------------------- */

#endif

