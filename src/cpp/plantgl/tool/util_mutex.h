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




#ifndef __util_mutex_h__
#define __util_mutex_h__

#include "tools_config.h"

#ifndef PGL_CORE_WITHOUT_QT

    #include <QtCore/qglobal.h>

    #ifdef QT_THREAD_SUPPORT
        #include <QtCore/qmutex.h>
        #define PGL_THREAD_SUPPORT
        #define PglMutexInternal QMutex
        #define PglMutexInternalTryLock(mutex) mutex.tryLock()


    #endif

#else

    #if _MSC_VER == 1500  
        // PGL_THREAD_SUPPORT not activated
    #else

        #include <mutex>
        #define PGL_THREAD_SUPPORT
        #define PglMutexInternal std::mutex
        #define PglMutexInternalTryLock(mutex) mutex.try_lock()

    #endif

#endif


#ifdef PGL_THREAD_SUPPORT

    struct PglMutex {
    public:
        PglMutex()  {}
        void lock() { __mutexinternal.lock(); }
        void unlock() { __mutexinternal.unlock(); }
        bool tryLock() { return PglMutexInternalTryLock(__mutexinternal); }

    protected:
        PglMutexInternal __mutexinternal;

    };
#else
    struct PglMutex {
    public:
        PglMutex()  {}
        void lock() { }
        void unlock() { }
        bool tryLock() { return true; }
    };

#endif

#endif
