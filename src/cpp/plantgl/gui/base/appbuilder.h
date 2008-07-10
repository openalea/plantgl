/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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

#include "../gui_config.h"


class Viewer;

class VIEW_API ViewerBuilder {
public:
	ViewerBuilder(){}
	virtual ~ViewerBuilder(){}
	virtual Viewer * build() = 0;
};


template<class T>
struct ViewerTBuilder : public ViewerBuilder {
public:
	ViewerTBuilder() {}
	virtual ~ViewerTBuilder() {}
	typedef T ViewerType;
	virtual Viewer * build() { return new T(); }
};

class VIEW_API ThreadStateSaver {
public:
    ThreadStateSaver() {}
    virtual ~ThreadStateSaver() {}
    virtual void pushState() = 0;
    virtual void popState() = 0;
};

class VIEW_API ThreadStateSaverFactory {
public:
    ThreadStateSaverFactory() {}
    virtual ~ThreadStateSaverFactory() {}
    virtual ThreadStateSaver * produceStateSaver() = 0;
};

template<class T>
class ThreadStateSaverTFactory : public ThreadStateSaverFactory {
public:
    ThreadStateSaverTFactory() {}
    virtual ~ThreadStateSaverTFactory() {}
    virtual ThreadStateSaver * produceStateSaver() { return new T; }
};

#endif

