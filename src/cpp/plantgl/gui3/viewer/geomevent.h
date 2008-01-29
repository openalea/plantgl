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
#include <plantgl/scenegraph/scene/scene.h>

/* ----------------------------------------------------------------------- */

/** 
    \class GeomSceneChangeEvent3
    \brief Event for GEOM Scene Change.
*/

/* ----------------------------------------------------------------------- */

class VIEW3_API GeomSceneChangeEvent3 : public ViewSceneChangeEvent3 {
  
  public :
    
  /// Constructor.
  GeomSceneChangeEvent3(PGL(ScenePtr) scene, 
		       const QString& = QString::null, 
		       const QString& _file = QString::null,
			   bool add = false);
  
  /// Destructor.
  ~GeomSceneChangeEvent3();
    
  /// copy object.
  virtual ViewSceneChangeEvent3 * copy();

  /// The scene to display.
  PGL(ScenePtr) scene;
      
  /// The error message to display.
  QString error;

  /// File.
  QString file;

  bool addition;

};

/** 
    \class GeomMultiSceneChangeEvent3
    \brief Event for GEOM Scene Change using multiple scene.
*/
class VIEW3_API GeomMultiSceneChangeEvent3 : public GeomSceneChangeEvent3 {
  
  public :
    
  /// Constructor.
  GeomMultiSceneChangeEvent3(PGL(ScenePtr) scene1, 
			    PGL(ScenePtr) scene2,
			    const QString& errlog = QString::null, 
			    const QString& file1 = QString::null,
			    const QString& file2 = QString::null);
  
  /// Destructor.
  ~GeomMultiSceneChangeEvent3();
    
  /// copy object.
  virtual ViewSceneChangeEvent3 * copy();

  /// The scene to display.
  PGL(ScenePtr) scene2;
      
  /// File.
  QString file2;

};

/* ----------------------------------------------------------------------- */

class VIEW3_API GeomGetSceneEvent3 : public ViewEvent3 {
  
  public :
    
  /// Constructor.
  GeomGetSceneEvent3(PGL(ScenePtr)* scene);
  
  /// Destructor.
  ~GeomGetSceneEvent3();
    
  /// The scene to display.
  PGL(ScenePtr)* scene;
      
};

/* ----------------------------------------------------------------------- */

class GeomProjListEvent3 : public ViewEvent3 {

public :

  /// Constructor.
	GeomProjListEvent3(const PGL(ScenePtr)& sc, std::vector<std::pair<uint_t,double> > * res);

  /// Destructor.
  ~GeomProjListEvent3();

  std::vector<std::pair<uint_t,double> > *  result;
  PGL(ScenePtr) objlist;
};

/* ----------------------------------------------------------------------- */
class ViewRayPointHitBuffer3;

class ViewRayBuff2Event3 : public ViewEvent3 {

public :

  /// Constructor.
	ViewRayBuff2Event3(const PGL(ScenePtr)& sc, 
					  bool back_test = true,
					  ViewRayPointHitBuffer3 ** res = NULL);
	
	/// Destructor.
	~ViewRayBuff2Event3();
	
	ViewRayPointHitBuffer3 **  result;
    PGL(ScenePtr) objlist;
	bool back_test;

};

/* ----------------------------------------------------------------------- */

#endif

