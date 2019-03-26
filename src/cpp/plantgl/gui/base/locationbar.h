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

/*! \file view_locationbar.h
    \brief Definition of the viewer class ViewLocationBar.
*/

#ifndef __view_locationbar_h__
#define __view_locationbar_h__

/* ----------------------------------------------------------------------- */

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qcombobox.h>
#else
    #include <QtGui/qcombobox.h>
#endif
#include "util_qwidget.h"

/* ----------------------------------------------------------------------- */

/// Viewer Location Bar
class ViewLocationBar : public ViewToolBar
{
    Q_OBJECT

public:
    ViewLocationBar( const QString &label,  QWidget *mw, const char *name=0);
    ~ViewLocationBar();


    QComboBox* __Location;


};

/* ----------------------------------------------------------------------- */
#endif
 // VIEWLOCATIONBAR_H
