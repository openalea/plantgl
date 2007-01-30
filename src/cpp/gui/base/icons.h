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


/* Drawn  by Mark Donohoe for the K Desktop Environment */
/* See http://www.kde.org */

        /// Icon for Exit.
        static const char *icon_exit[30];
	
	/// Icon for saving files.
	static  const char *icon_filefloppy[29];

	/// Icon for opening files.
	static  const char *icon_fileopen[29];

	/// Icon for printing.
	static  const char *icon_fileprint[29];
	
	/// Icon for closing.
	static  const char *icon_fileclose[20];
	
	/// Icon for reloading a file.
	static  const char *reload[26];

	/// Icon for new document.
	static  const char *icon_document[24];

	static  const char *icon_camera[157];
	static  const char *icon_camerafile[158];
	static  const char *icon_perspective[26];
	static  const char *icon_orthographic[26];

	/// Icon for fullscreen.
	static  const char *fullscreen[24];

	/// Icon of memory : features of the viewer.
	static  const  char *icon_memory[104];

	/// Icon of wheel.
        static const char * icon_wheel[34];

	/// Icon wizard.
        static const char * icon_wizard[27];
        static const char * icon_wizardmenu[27];
        static const char * icon_wizardrect[27];

	/// Icon wizard.
        static const char * icon_notwizard[28];

	/// Icon home.
        static const char * icon_home[29];

	/// Icon home.
        static const char * icon_locerase[20];

	/// Icon geometry.
        static const char * icon_geometry[27];

	/// Icon transformed.
        static const char * icon_transformed[26];

	/// Icon appearance.
        static const char * icon_appearance[27];

	/// Icon shape.
        static const char * icon_shape[31];

	/// Icon attribut.
    static const char * icon_attribut[22];

	/// Icon attribut.
    static const char * icon_attributptr[22];

	/* Icons Drawn  by Frederic Boudon for the Viewer */
	/// Icon of the background color.
	static  const char *icon_color[247];

	/// Logo of Amapmod.
	static  const char * icon_plantlogo[4748];
	static  const char * icon_flower[276];
	
	/// Icon for the bounding box.
	static  const char *icon_bbox[32];

	/// A cross icon.
	static  const char * icon_cross[8];

	/// A line icon.
	static const char * icon_line_width[101];

	/// An icon.
	static const char * icon_ctrlpoint[21];

	/// An icon.
	static const char * icon_rcactive[22];

	/// An icon.
	static const char * icon_rccentered[22];

	/// An icon.
	static const char * icon_rcvisible[20];

	/// An icon.
	static const char * icon_skeleton[20];

	/// An icon.
	static const char * icon_solid[22];

        /// An icon.
	static const char * icon_wire[19];

	/// An icon.
	static const char * icon_light[66];

	static const char * icon_eye[54];
	static const char * icon_eyef[54];
	static const char *icon_linked[105];
	static const char *icon_unlinked[70];

	/// An icon.
	static const char * icon_grid[19];
	static const char * icon_gridXY[21];
	static const char * icon_gridXZ[21];
	static const char * icon_gridYZ[21];

	/// An icon.
	static const char * icon_axis[21];

	static  const char *icon_logobar[2850];

/* Icon draw by the povray team. */
        /// icon of povray. 
	static  const char *icon_pov[34];

/* Icon draw by the vrml team. */

	/// An icon for vrml.
	static const char * icon_vrml[86];

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
	static QPixmap& getPixmap(const char ** obj);
	static QPixmap getPixmap(const char * file);

	/// Static function that clear the cache use to store pixmap.
	static void clearCache();

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

