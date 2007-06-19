/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon.
 *
 *       $Source: /usr/cvsmaster/AMAPmod/src/GEOM/view_configuration.cpp,v $
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


#include "configuration.h"
#include <qapplication.h>
#include <qfileinfo.h>

/*--------------------------------------------------------------*/

QStringList ViewerSettings3::args;
QString ViewerSettings3::appliname;
QString ViewerSettings3::applidir;

/*--------------------------------------------------------------*/

void ViewerSettings3::setArgs(int argc, char** argv)
{
	for(int i = 0; i < argc; ++i)
		args.append(QString(argv[i]));
	if(argc > 0) {
		QFileInfo f(args[0]);
		QString basename = f.baseName();
		if(!basename.isEmpty()){
			if(basename.right(2) == "-d") // Remove '-d' flag name for debug compilation
				basename.remove(basename.length(),2);
			setAppliName(basename);
#if QT_VERSION >= 0x040000
			applidir = f.absolutePath();
#else
			applidir = f.absFilePath();
#endif
		}
	}
	if(getAppliName().isEmpty()){
		setAppliName("PlantGLViewer");
	}
#if QT_VERSION >= 0x040000
	if(applidir.isEmpty())
		applidir = QCoreApplication::applicationDirPath();
#endif
}

void ViewerSettings3::setAppliName(const QString& _appliname)
{
	appliname = _appliname;
}

ViewerSettings3::ViewerSettings3():
#if QT_VERSION >= 0x040000
	QSettings(QSettings::IniFormat,QSettings::UserScope,"OpenAlea",appliname) {}
#else
	QSettings(QSettings::Ini)
{
	setPath("OpenAlea",appliname);
}
#endif



/*--------------------------------------------------------------*/
