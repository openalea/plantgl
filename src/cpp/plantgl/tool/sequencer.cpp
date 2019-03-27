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


#include "sequencer.h"

PGL_USING_NAMESPACE

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
