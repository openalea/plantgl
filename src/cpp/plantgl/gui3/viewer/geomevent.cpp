/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): C. Nouguier & F. Boudon
 *                       N. Dones & B. Adam
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

#include "geomevent.h"

PGL_USING_NAMESPACE

/*  ------------------------------------------------------------------------ */

GeomSceneChangeEvent3::GeomSceneChangeEvent3(ScenePtr _scene, 
					   const QString& errlog, 
					   const QString& _file,
					   bool add) :
  ViewSceneChangeEvent3(1),
  scene(_scene),
  error(errlog),
  file(_file),
  addition(add)
{
}

GeomSceneChangeEvent3::~GeomSceneChangeEvent3(){
  // Nothing to do.
}

ViewSceneChangeEvent3 *
GeomSceneChangeEvent3::copy()
{
  return new GeomSceneChangeEvent3(scene,error,file,addition);
}


/* ----------------------------------------------------------------------- */

/// Constructor.
GeomGetSceneEvent3::GeomGetSceneEvent3(PGL(ScenePtr)* _scene) : ViewEvent3(12368),scene(_scene) { }
  
  /// Destructor.
GeomGetSceneEvent3::~GeomGetSceneEvent3() {}


/* ----------------------------------------------------------------------- */

GeomMultiSceneChangeEvent3::GeomMultiSceneChangeEvent3(ScenePtr _scene1, 
						     ScenePtr _scene2,
						     const QString& _errlog , 
						     const QString& _file1 ,
						     const QString& _file2 ):
  
  GeomSceneChangeEvent3(_scene1, _errlog, _file1),
    scene2(_scene2),
    file2(_file2)
{
  setSceneType(2);
}

GeomMultiSceneChangeEvent3::~GeomMultiSceneChangeEvent3()
{
}

ViewSceneChangeEvent3 * 
GeomMultiSceneChangeEvent3::copy()
{
  return new GeomMultiSceneChangeEvent3(scene,scene2,error,file,file2);
}

/* ----------------------------------------------------------------------- */


GeomProjListEvent3::GeomProjListEvent3(const ScenePtr& sc, std::vector<std::pair<uint_t,double> > * _res):
ViewEvent3(12365),objlist(sc),result(_res){}

  
GeomProjListEvent3::~GeomProjListEvent3(){}


/* ----------------------------------------------------------------------- */

/// Constructor.
ViewRayBuff2Event3::ViewRayBuff2Event3(const PGL(ScenePtr)& sc, bool _back_test ,
								   ViewRayPointHitBuffer3 ** res ):
ViewEvent3(12367),objlist(sc),back_test(_back_test),result(res){}


ViewRayBuff2Event3::~ViewRayBuff2Event3() {}

/* ----------------------------------------------------------------------- */

