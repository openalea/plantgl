/* -*-c++-*- 
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): Ch. Nouguier (christophe.nouguier@cirad.fr) 
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

TOOLS_BEGIN_NAMESPACE

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
	 _stop((clock_t)0)
  {
  }

  // Destructor
	 ~Timer() ; 

  /*!
    Returns the number of seconds elpased since \e this timer is 
    in the running state.
  */
  inline double elapsedTime( ) const {
	return (double)((_on ? clock() : _stop) - _start) / CLOCKS_PER_SEC;
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
    register double laps = elapsedTime();
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
    Stops \e this. and returns the elpased time.
  */
  inline double stop( )
  {
   register double laps = elapsedTime();
   _stop = clock();
   _on = false;
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

  bool _on; /// < running state flag.
  clock_t _start; /// < time at start
  clock_t _stop;  /// < time at stop

}; // class Timer


/* ----------------------------------------------------------------------- */

TOOLS_END_NAMESPACE

/* ----------------------------------------------------------------------- */

// Timer_h
#endif
