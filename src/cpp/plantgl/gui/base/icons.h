/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al. 
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use, 
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info". 
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability. 
 *       
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or 
 *   data to be ensured and,  more generally, to use and operate it in the 
 *   same conditions as regards security. 
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
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

    static void getPixmapInfo(const PredefinedIcon id, int& width, int& heigth,int& nbcolors,int& header);
    static int getPixmapNbLines(const PredefinedIcon id);

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

