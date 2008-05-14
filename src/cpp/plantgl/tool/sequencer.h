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

#ifndef __sequencer_h__
#define __sequencer_h__

#include <time.h>
#include "tools_config.h"

/* ----------------------------------------------------------------------- */

TOOLS_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/* This class allow to be sure that
   enought time has been spend between two loop step */

class TOOLS_API Sequencer {
protected:
  clock_t __timestep;
  clock_t __time ;
  void _sleep(clock_t) const;
  
public:
  // Initialize the sequencer
  Sequencer(double timestep = 0.01);

  void setTimeStep(double timestep);

  double getTimeStep() const;

  double elapsedTime() const ;
  bool ok() const ;

  /* check that between two call of this function
  timestep msec has been spend
  Else sleep remaining time */
  void touch();
  void touch(double nexttimestep);

  void sleep(double) const;

};

/* ----------------------------------------------------------------------- */

TOOLS_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
