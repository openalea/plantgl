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



/*! \file view_icons.h
    \brief Definition of the viewer class ViewerIcon.
*/


#ifndef __view_icons_h__
#define __view_icons_h__

#include "../gui_config.h"

class QPixmap;

/* ----------------------------------------------------------------------- */

/**   
   \class ViewerIcon
   \brief A virtual class who contains all the icons used for the viewer.
*/

/* ----------------------------------------------------------------------- */

class VIEW_API ViewerIcon 
{

public :

	enum PredefinedIcon {
		exit = 0,
		filefloppy,
		fileopen,
		fileprint,
		fileclose,
		reload,
		document,
		camera,
		camerafile,
		perspective,
		orthographic,
		fullscreen,
		memory,
		wheel,
		wizard,
		wizardmenu,
		wizardrect,
		notwizard,
		home,
		locerase,
		geometry,
		transformed,
		appearance,
		shape,
		attribut,
		attributptr,
		color,
		plantlogo,
		flower,
		bbox,
		cross,
		line_width,
		ctrlpoint,
		rcactive,
		rccentered,
		rcvisible,
		skeleton,
		solid,
		wire,
		light,
		eye,
		eyef,
		linked,
		unlinked,
		grid,
		gridXY,
		gridXZ,
		gridYZ,
		axis,
		logobar,
		pov,
		vrml,
		nbIcons
	} ;


	/* Cursor Drawn  by Nicolas Dones and Boris Adam for the AMAPmod Viewer */
	/// Cursor bits for rotation.
	static  const unsigned char ROTATE_BITS[208];

	/// Cursor mask for rotation.
	static  const unsigned char ROTATE_MASK[208];

	/// Cursor bits for zoom.
	static  const unsigned char ZOOM_BITS[208];

	/// Cursor mask for zoom.
	static  const unsigned char ZOOM_MASK[208];

	/// Cursor bits for lignt.
	static  const unsigned char LIGHT_BITS[208];

	/// Cursor mask for light.
	static  const unsigned char LIGHT_MASK[208];

/* ----------------------------------------------------------------------- */
	/// Static function that create pixmap and put them on a cache for next use.
	static const char * const* getPixmap(const PredefinedIcon id);
	static const char * const* getPixmap(const char * const obj[]);
	static QPixmap getPixmap(const char * file);

	/// Static function that clear the cache use to store pixmap.
	static void clearCache();

protected:
	/// Constructor.
	ViewerIcon(){
	};
	
	/// Destructor.
	virtual ~ViewerIcon(){
	};
    
	
};

/* ----------------------------------------------------------------------- */


// __view_icons_h__
#endif

