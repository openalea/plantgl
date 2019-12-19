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
