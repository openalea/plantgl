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


/*! \file view_appbuilder.h
    \brief Definition of the viewer class ViewerApplication.
*/

#ifndef __view_appbuilder_h__
#define __view_appbuilder_h__

#include "../gui_config.h"


class Viewer;

class VIEW_API ViewerBuilder {
public:
    ViewerBuilder(){}
    virtual ~ViewerBuilder(){}
    virtual Viewer * build() = 0;
};


template<class T>
struct ViewerTBuilder : public ViewerBuilder {
public:
    ViewerTBuilder() {}
    virtual ~ViewerTBuilder() {}
    typedef T ViewerType;
    virtual Viewer * build() { return new T(); }
};

class VIEW_API ThreadStateSaver {
public:
    ThreadStateSaver() {}
    virtual ~ThreadStateSaver() {}
    virtual void pushState() = 0;
    virtual void popState() = 0;
};

class VIEW_API ThreadStateSaverFactory {
public:
    ThreadStateSaverFactory() {}
    virtual ~ThreadStateSaverFactory() {}
    virtual ThreadStateSaver * produceStateSaver() = 0;
};


template<class T>
class ThreadStateSaverTFactory : public ThreadStateSaverFactory {
  template<class U>
  class TSSEncapsuler : public ThreadStateSaver {
    public:
    TSSEncapsuler() : ThreadStateSaver(), _value() {}
    U _value;
    virtual void pushState() { _value.pushState(); }
    virtual void popState()  { _value.popState();  }
  };

public:
    ThreadStateSaverTFactory() {}
    virtual ~ThreadStateSaverTFactory() {}
    virtual ThreadStateSaver * produceStateSaver() { return new TSSEncapsuler<T>(); }
};

#endif

