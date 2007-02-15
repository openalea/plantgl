/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon.
 *
 *       $Source: /usr/cvsmaster/AMAPmod/src/GEOM/view_configuration.h,v $
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
#ifndef __view_configuration_h__
#define __view_configuration_h__


#include "../gui_config.h"
#include <qstringlist.h>
#include <qsettings.h>


/*--------------------------------------------------------------*/

class VIEW_API ViewerSettings : public QSettings {
	static QStringList args;
	static QString appliname;
	static QString applidir;

public:
	static void setArgs(int argc, char** argv);
	static const QStringList& getArgs() { return args; }

	static void setAppliName(const QString& appliname);
	static const QString& getAppliName() { return appliname; }
	static const QString& getAppliDir() { return applidir; }

	ViewerSettings();
	

};

/*--------------------------------------------------------------*/

#endif // __view_configuration_h__
