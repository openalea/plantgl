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


#ifndef __view_threadedappli_h__
#define __view_threadedappli_h__

#include "../gui_config.h"

#ifndef QT_THREAD_SUPPORT
#ifdef __GNUC__
#warning "Qt compiled without openGL support"
#else
#pragma message "Qt compiled without openGL support"
#endif
#else

#include "appli.h"
#include <QtCore/qthread.h>
#include <QtCore/qmutex.h>
#include <QtCore/qwaitcondition.h>

class QApplication;
class Viewer;

class ViewerThreadedAppli : public ViewerAppliInternal, public QThread {
    public:
    ViewerThreadedAppli();
    virtual ~ViewerThreadedAppli();

    virtual void startSession();
    virtual bool stopSession() ;
    virtual bool exit() ;

    virtual bool running() ;
    virtual bool Wait ( unsigned long time = ULONG_MAX ) ;

    virtual std::vector<uint_t> getSelection();

protected:

    void launch();
    virtual void init();
    virtual void cleanup();
    virtual void exec();
    virtual void run();

private :

    void startSync() ;
    void sync() ;
    void join() ;

    QMutex         syncmutex;
    QWaitCondition synccond;

    /// Wait Condition for initialisation.
    QWaitCondition contcond;

    /// Wait Condition for end viewer session.
    QMutex         sessionmutex;
    QWaitCondition session;

    QMutex         __continue;
    QMutex         __running;

    std::vector<uint_t> __selection;

};

#endif

#endif
