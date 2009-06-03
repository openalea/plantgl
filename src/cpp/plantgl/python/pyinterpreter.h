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

#ifndef __pyinterpreter_acquirer_h__
#define __pyinterpreter_acquirer_h__

#include <Python.h>

/* ----------------------------------------------------------------------- */

class PythonInterpreterAcquirer {
public:

    PythonInterpreterAcquirer() 
    { 
        /** It seems mandatory to acquire the GIL to call python 
            from C++ internal (during GUI process for instance) */
        assert(PyEval_ThreadsInitialized());
        gstate = PyGILState_Ensure(); 
    }
    ~PythonInterpreterAcquirer()
    { 
      if(gstate)PyGILState_Release(gstate); 
    }

protected:
    PyGILState_STATE gstate;
};

/* ----------------------------------------------------------------------- */

class PyStateSaver {
public:
  PyStateSaver() : _state(0) { }
    ~PyStateSaver() { if (_state) popState(); }

    void pushState () { 
      if(!_state) {
        assert(PyEval_ThreadsInitialized());
        _gstate = PyGILState_Ensure();
	_state = PyEval_SaveThread(); 
      }
    }
    void popState () 
    { 
        if(_state){
            PyEval_RestoreThread(_state); 
             PyGILState_Release(_gstate); 
            _state = NULL; 
        }
    }
 
protected:
    PyGILState_STATE _gstate;
    PyThreadState *_state;

};

/* ----------------------------------------------------------------------- */

#endif
