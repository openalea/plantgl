/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): F. Boudon
 *
 *       $Source$
 *       $Id: util_serializedobj.h 2504 2007-01-18 10:02:20Z boudon $
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


#ifndef __threaded_data_h__
#define __threaded_data_h__

#include "../gui_config.h"
#include <QtGlobal>

#ifdef QT_THREAD_SUPPORT

#include <QtCore/qmutex.h>

template <class T>
class ThreadedData {

public:

  typedef T data_type;

  typedef QMutex Lock;
  typedef QMutexLocker ReadLocker;
  typedef QMutexLocker WriteLocker;

  ThreadedData(data_type * val):
      __val(val){
    }

  inline void deleteData()
  {
    WriteLocker l(&lock);
    if (__val != NULL) delete __val;
    __val = NULL;
  }

  inline ThreadedData operator = (data_type * val) {
    set(val);
    return *this;
  }

  inline void set(data_type * val) {
    ReadLocker l(&lock);
    __val = val;
  }

  inline operator const data_type * () { return getConst(); }

  inline data_type * get( ) {
    ReadLocker l(&lock);
    data_type * val = __val;
    return val;
  }

  inline const data_type * getConst( ) {
    ReadLocker l(&lock);
    data_type * val = __val;
    return val;
  }

private:
    ThreadedData(ThreadedData<T>& copy): __val(copy.get()){};
    ThreadedData<T>& operator=(ThreadedData<T>& copy){ WriteLocker l(&lock); __val = copy.get(); }

  data_type * __val;
  Lock lock;

};

#endif

#endif

