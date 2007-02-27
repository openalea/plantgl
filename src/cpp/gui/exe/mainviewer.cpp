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
 



#include <qapplication.h>
#include <qgl.h>
#include <qpixmap.h>


#include "../viewer/pglviewer.h"
#include "../base/daemon.h"

int main( int argc, char **argv )
{

  QApplication a( argc, argv );
  if ( !QGLFormat::hasOpenGL() ) {
	qWarning( "This system has no OpenGL support. Exiting." );
	return -1;
  }
  
  // if(!ViewClientObj().request(argc, argv)){
	PGLViewer m_viewer(argc, argv) ;
	m_viewer.startDaemon();
	m_viewer.displayTrayIcon(true);
	m_viewer.show();
	return a.exec();
  // }

  return 0;
}

/*
#include "../viewer/pglapplication.h"

int main( int argc, char **argv )
{

	PGLViewerApplication::init();
	PGLViewerApplication::start();
	PGLViewerApplication::wait(10000);
	PGLViewerApplication::exit();

  return 0;
}
*/
