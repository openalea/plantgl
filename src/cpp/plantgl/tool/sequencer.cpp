/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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

#include "sequencer.h"

TOOLS_USING_NAMESPACE

Sequencer::Sequencer(double timestep ):
  __timestep(timestep*CLOCKS_PER_SEC),
  __time(0){
}

void
Sequencer::setTimeStep(double timestep){
  __timestep = timestep * CLOCKS_PER_SEC;
}

double
Sequencer::getTimeStep() const {
  return __timestep / CLOCKS_PER_SEC;
}

double
Sequencer::elapsedTime() const {
  return (clock() - __time) / CLOCKS_PER_SEC;
}

bool
Sequencer::ok() const {
  clock_t deltaT = clock() - __time;
  return (deltaT >= __timestep);
}

void
Sequencer::touch(){
  if (__time == 0) __time = clock();
  else{
	clock_t deltaT = clock() - __time;
	if (deltaT < __timestep) _sleep(__timestep-deltaT);
	__time = clock();
  }
}

void Sequencer::touch(double nexttimestep){
    touch();
    setTimeStep(nexttimestep);
}

void Sequencer::_sleep(clock_t wait) const {
   clock_t goal;
   goal = wait + clock();
   while( goal > clock() )
      ;
}

void Sequencer::sleep(double t) const {
  _sleep(t*CLOCKS_PER_SEC);
}
