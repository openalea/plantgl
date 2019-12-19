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
