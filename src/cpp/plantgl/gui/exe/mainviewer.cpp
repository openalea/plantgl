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




#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qapplication.h>
#else
    #include <QtGui/qapplication.h>
#endif
#include <QtOpenGL/qgl.h>
#include <QtGui/qpixmap.h>

#include "../viewer/pglviewer.h"
#include "../base/daemon.h"
#include "../base/icons.h"

int main( int argc, char **argv )
{

  QApplication a( argc, argv );
  if ( !QGLFormat::hasOpenGL() ) {
    qWarning( "This system has no OpenGL support. Exiting." );
    return -1;
  }

  a.setWindowIcon(QIcon(QPixmap(ViewerIcon::getPixmap(ViewerIcon::flower))));
  // if(!ViewClientObj().request(argc, argv)){
    PGLViewer m_viewer(argc, argv) ;
    // m_viewer.startDaemon();
#ifndef __GNUC__
    m_viewer.displayTrayIcon(true);
#endif
    m_viewer.show();
    return a.exec();
#ifndef __GNUC__
    m_viewer.displayTrayIcon(false);
#endif
    // m_viewer.stopDaemon();
  // }

  return 0;
}

/*
#include "../viewer/pglapplication.h"

int main( int argc, char **argv )
{

    PGLViewerApplication::init();
    PGLViewerApplication::start();
    // PGLViewerApplication::wait(10000);
    PGLViewerApplication::exit();

  return 0;
}

*/
