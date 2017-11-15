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


#include "configuration.h"
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0) 
    #include <QtWidgets/QApplication>
#else
    #include <QtGui/QApplication>
#endif
#include <QtCore/qfileinfo.h>

/*--------------------------------------------------------------*/

QStringList ViewerSettings::args;
QString ViewerSettings::appliname;
QString ViewerSettings::applidir;

/*--------------------------------------------------------------*/

void ViewerSettings::setArgs(int argc, char** argv)
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
#if QT_VERSION >= QT_VERSION_CHECK(4,0,0)
			applidir = f.absolutePath();
#else
			applidir = f.absFilePath();
#endif
		}
	}
	if(appliname.isEmpty()){
		setAppliName("PlantGLViewer");
	}
#if QT_VERSION >= QT_VERSION_CHECK(4,0,0)
	if(applidir.isEmpty())
		applidir = QCoreApplication::applicationDirPath();
#endif
}

void ViewerSettings::setAppliName(const QString& _appliname)
{
	appliname = _appliname;
}

const QString& ViewerSettings::getAppliName() 
{ 
    if(appliname.isEmpty()) setAppliName("PlantGLViewer");
    return appliname; 
}

ViewerSettings::ViewerSettings():
#if QT_VERSION >= QT_VERSION_CHECK(4,0,0)
	QSettings(QSettings::IniFormat,QSettings::UserScope,"OpenAlea",getAppliName()) {}
#else
	QSettings(QSettings::Ini)
{
	setPath("OpenAlea",getAppliName());
}
#endif



/*--------------------------------------------------------------*/
