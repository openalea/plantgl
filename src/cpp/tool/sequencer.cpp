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

void Sequencer::_sleep(clock_t wait) const {
   clock_t goal;
   goal = wait + clock();
   while( goal > clock() )
      ;
}

void Sequencer::sleep(double t) const {
  _sleep(t*CLOCKS_PER_SEC);
}
