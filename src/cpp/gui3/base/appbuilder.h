/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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

/*! \file view_appbuilder.h
    \brief Definition of the viewer class ViewerApplication.
*/

#ifndef __view_appbuilder_h__
#define __view_appbuilder_h__

#include "../gui3_config.h"

class Viewer3;

class VIEW3_API ViewerBuilder3 {
public:
	ViewerBuilder3(){}
	virtual ~ViewerBuilder3(){}
	virtual Viewer3 * build() = 0;
};

template<class T>
struct ViewerTBuilder3 : public ViewerBuilder3 {
public:
	ViewerTBuilder3() {}
	virtual ~ViewerTBuilder3() {}
	typedef T ViewerType;
	virtual Viewer3 * build() { return new T(); }
};

#endif

