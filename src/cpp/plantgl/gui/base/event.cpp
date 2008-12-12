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

#include "event.h"

ViewEvent::ViewEvent(int type):
	QEvent(QEvent::Type(type)),
	sent_event(false)
{
#ifdef PGL_DEBUG
    printf("Create event %i of type %i\n",size_t(this),type);
#endif
}

ViewEvent::~ViewEvent(){
#ifdef PGL_DEBUG
    printf("Destroy event %i of type %i\n",size_t(this),type());
#endif
}

/*  ------------------------------------------------------------------------ */

ViewSceneChangeEvent::ViewSceneChangeEvent(int type) :
  ViewEvent(eSceneChange),
  scene_type(type)
{
}

ViewSceneChangeEvent::~ViewSceneChangeEvent()
{
  // Nothing to do.
}

ViewSceneChangeEvent *
ViewSceneChangeEvent::copy()
{
  return new ViewSceneChangeEvent(scene_type);
}

const int&
ViewSceneChangeEvent::getSceneType() const
{
  return scene_type;
}

void 
ViewSceneChangeEvent::setSceneType(const int& i)
{
  scene_type =i;
}


/* ----------------------------------------------------------------------- */

ViewLightSetEvent::ViewLightSetEvent(const Vector3& _pos,
                    QColor _ambient,
                    QColor _diffuse,
                    QColor _specular,
                    bool _activation,
                    eLightAttribute _def):
  ViewEvent(eLightSet),
  position(_pos),ambient(_ambient),diffuse(_diffuse),specular(_specular),activation(_activation),def(_def){
}

ViewLightSetEvent::~ViewLightSetEvent(){
  // Nothing to do.
}
/* ----------------------------------------------------------------------- */

ViewLightGetEvent::ViewLightGetEvent(Vector3* _pos,
                    QColor* _ambient,
                    QColor* _diffuse,
                    QColor* _specular,
                    bool* _activation,
                    ViewLightSetEvent::eLightAttribute _def):
  ViewEvent(eLightGet),
  position(_pos),ambient(_ambient),diffuse(_diffuse),specular(_specular),activation(_activation),def(_def){
}

ViewLightGetEvent::~ViewLightGetEvent(){
  // Nothing to do.
}

/* ----------------------------------------------------------------------- */
