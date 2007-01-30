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

GeomSceneChangeEvent::GeomSceneChangeEvent(ScenePtr _scene, 
					   const QString& errlog, 
					   const QString& _file,
					   bool add) :
  ViewSceneChangeEvent(1),
  scene(_scene),
  error(errlog),
  file(_file),
  addition(add)
{
}

GeomSceneChangeEvent::~GeomSceneChangeEvent(){
  // Nothing to do.
}

ViewSceneChangeEvent *
GeomSceneChangeEvent::copy()
{
  return new GeomSceneChangeEvent(scene,error,file,addition);
}


/* ----------------------------------------------------------------------- */

GeomMultiSceneChangeEvent::GeomMultiSceneChangeEvent(ScenePtr _scene1, 
						     ScenePtr _scene2,
						     const QString& _errlog , 
						     const QString& _file1 ,
						     const QString& _file2 ):
  
  GeomSceneChangeEvent(_scene1, _errlog, _file1),
    scene2(_scene2),
    file2(_file2)
{
  setSceneType(2);
}

GeomMultiSceneChangeEvent::~GeomMultiSceneChangeEvent()
{
}

ViewSceneChangeEvent * 
GeomMultiSceneChangeEvent::copy()
{
  return new GeomMultiSceneChangeEvent(scene,scene2,error,file,file2);
}

/* ----------------------------------------------------------------------- */


GeomProjListEvent::GeomProjListEvent(const ScenePtr& sc, std::vector<std::pair<uint32_t,double> > * _res):
ViewEvent(12365),objlist(sc),res(_res){}

  
GeomProjListEvent::~GeomProjListEvent(){}


/* ----------------------------------------------------------------------- */
