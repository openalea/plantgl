/* -*-c++-*- 
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): Ch. Nouguier (christophe.nouguier@cirad.fr) 
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





#include "timer.h"
#include "util_assert.h"

TOOLS_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


Timer::Timer( ) :
  _on(true), 
  _start((clock_t)0), 
  _stop((clock_t)0)
{
}


double Timer::elapsedTime( ) const
{
  return (double)((_on ? clock() : _stop) - _start) / CLOCKS_PER_SEC;
}


double Timer::reset( )
{
  register double laps = elapsedTime();
  _start = _stop = clock();
  return laps;
}


void Timer::start( )
{
  _start = _stop = clock();
  _on = true;
}


double Timer::stop( )
{
  register double laps = elapsedTime();
  _stop = clock();
  _on = false;
  return laps;
}


void Timer::wait( double sec )
{
  // preconditions
  assert(0 < sec);

  clock_t goal = (long)(sec * CLOCKS_PER_SEC) + clock();
  while( goal > clock() );
}


/* ----------------------------------------------------------------------- */

TOOLS_END_NAMESPACE

