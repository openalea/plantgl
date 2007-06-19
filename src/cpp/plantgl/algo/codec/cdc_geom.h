/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
 *
 *       $Source: /usr/cvsmaster/AMAPmod/src/GEOM/scne_scene.h,v $
 *       $Id: scne_scene.h,v 1.39 2006/06/16 13:04:46 fboudon Exp $
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


/*! \file cdc_geom.h
    \brief Definition of the geom codec.
*/

#ifndef __cdc_geom_h__
#define __cdc_geom_h__

/* ----------------------------------------------------------------------- */

#include "codec_config.h"
#include <plantgl/scenegraph/scene/factory.h>
#include <iostream>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

CODEC_API bool geom_read(std::istream& stream, SceneObjectSymbolTable& table, ScenePtr& scene, const std::string& fname = "");

/* ----------------------------------------------------------------------- */

class CODEC_API GeomCodec : public SceneCodec {
public :

	GeomCodec();

	virtual SceneFormatList formats() const;

	virtual ScenePtr read(const std::string& fname);

	virtual void write(const std::string& fname,const ScenePtr&	scene);

};


PGL_END_NAMESPACE

#endif

