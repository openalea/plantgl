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





#ifndef __timer_h__
#define __timer_h__

/*! \file timer.h
    \brief File for Timer.
*/

/* ----------------------------------------------------------------------- */


#include <ctime>
#include "tools_config.h"
#include "util_assert.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


/*!

  \class Timer.

  \brief A timer used to measure elapsed CPU time in seconds.

*/

class TOOLS_API Timer
{

 public:

  /*!
    Construcs and starts a Timer.
  */
  Timer( ) :
	 _on(true),
	 _start((clock_t)0),
	 _stop((clock_t)0),
     _elapsed((clock_t)0)
  {
  }

  // Destructor
	 ~Timer() ;

  /*!
    Returns the number of seconds elpased since \e this timer is
    in the running state.
  */
  inline double elapsedTime( ) const {
    return (double(_elapsed) / CLOCKS_PER_SEC) + _elapsedTime();
  }

  /*!
    returns true if \e this timer is in the running state.
  */
  inline bool isRunning( ) const { return _on; }

  /*!
    Resets \e this timer and returns the elapsed time.
  */
  inline double reset( )
  {
    double laps = elapsedTime();
    _elapsed = (clock_t)0;
    _start = _stop = clock();
    return laps;
  }

  /*!
    Puts \e this timer in the running state.
  */
  inline void start( ){
	_start = _stop = clock();
	_on = true;
  }


  /*!
    Stops \e this. and returns the elapsed time.
  */
  inline double stop( )
  {
   _stop = clock();
   _on = false;
   _elapsed += (_stop - _start);
   double laps = _elapsedTime();
   _start = _stop = clock();
   return laps;
  }


  /*!
    Pauses for a specified number of seconds \e sec

    \par Preconditions
    - \e sec must be positive.
  */
  inline static void wait( double sec )
  {
	// preconditions
	assert(0 < sec);

	clock_t goal = (long)(sec * CLOCKS_PER_SEC) + clock();
	while( goal > clock() );
  }

 private:
  /*!
    Returns the number of seconds elpased since \e this timer is
    in the running state.
  */
  inline double _elapsedTime( ) const {
    return  (double)((_on ? clock() : _stop) - _start) / CLOCKS_PER_SEC;
  }


  bool _on; /// < running state flag.
  clock_t _start; /// < time at start
  clock_t _stop;  /// < time at stop
  clock_t _elapsed;

}; // class Timer


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

// Timer_h
#endif
