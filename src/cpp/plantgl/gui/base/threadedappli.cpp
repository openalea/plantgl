/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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



#include "threadedappli.h"

#ifdef QT_THREAD_SUPPORT
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0) 
    #include <QtWidgets/qapplication.h>
#else
    #include <QtGui/qapplication.h>
#endif
#include "viewer.h"
#include "event.h"
#include <plantgl/tool/dirnames.h>

PGL_USING_NAMESPACE

ViewerThreadedAppli::ViewerThreadedAppli() : 
			 ViewerAppliInternal(), QThread()
{ 
	// printf("Threaded Appli\n");
	launch();
}


ViewerThreadedAppli::~ViewerThreadedAppli(){ 
};

void 
ViewerThreadedAppli::startSession(){
    if(!isRunning())   launch();
	else if(!running()) contcond.wakeAll();
}

bool 
ViewerThreadedAppli::stopSession(){
    if(running()){
	   activateStateSaver(false);
	   sendAnEvent(new ViewEndEvent());
	   activateStateSaver(true);
       return true;
    }
    else return false;
}

bool 
ViewerThreadedAppli::exit() {
	if(isRunning()){
	  startSync();
	  __continue.unlock();
	  if(running()){ 
		  activateStateSaver(false);
		  sendAnEvent(new ViewEndEvent());
		  activateStateSaver(true);
	  }
	  else  contcond.wakeAll();
	  sync();
	  terminate();
      return true;
    }
	else return false;
}


bool 
ViewerThreadedAppli::running() {
   if (!isRunning()) return false;
   else if (__running.tryLock()) { __running.unlock(); return false; }
   else return true;
}

bool 
ViewerThreadedAppli::Wait ( unsigned long time ) {
   sessionmutex.lock();
   bool b = session.wait(&sessionmutex,time);
   sessionmutex.unlock();
   return b;
}

void 
ViewerThreadedAppli::launch(){ 	
	__continue.tryLock(); 
	startSync(); start(); sync(); 
}

void 
ViewerThreadedAppli::startSync() { syncmutex.lock(); }

void 
ViewerThreadedAppli::sync() { synccond.wait(&syncmutex); syncmutex.unlock(); }

void 
ViewerThreadedAppli::join() { while(!syncmutex.tryLock()){;} syncmutex.unlock(); synccond.wakeAll(); }


void 
ViewerThreadedAppli::init(){
    int argc = 0;
    char ** argv = NULL;
    if(!qApp)  new QApplication(argc,argv);
    if(!getViewer()) build();
}

void 
ViewerThreadedAppli::cleanup(){
	deleteViewer();
	if(qApp)delete qApp;
}

void 
ViewerThreadedAppli::exec(){
  if(getViewer() == NULL){
    std::cerr << "Null Viewer !!" << std::endl;
    return;
  }
      getViewer()->setVisible(true);
	  getViewer()->displayTrayIcon(false);
	  qApp->exec();
	  getViewer()->saveConfig();
	  __selection = getViewer()->getSelection();
}

void 
ViewerThreadedAppli::run(){
	init();
	std::string p = get_cwd();
    __running.lock();
	join();
	while(!__continue.tryLock()){
	  exec();
	  session.wakeAll();
	  if(__continue.tryLock()) __continue.unlock();
	  else { contcond.wait(&__running); }
	}
	__continue.unlock();
	chg_dir(p);
	cleanup();
	join();
}

std::vector<uint_t>
ViewerThreadedAppli::getSelection() {
	std::vector<uint_t> res;
    if(running()){
      ViewSelectRecoverEvent * event = new ViewSelectRecoverEvent(&res) ;
      sendAnEvent(event);
    }
    else  res = __selection;
    return res;
}


#endif
