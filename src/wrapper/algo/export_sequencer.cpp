/* ---------------------------------------------------------------------------
 #
 #       File author(s): F. Boudon (frederic.boudon@loria.fr)
 #
 # ---------------------------------------------------------------------------
 #
 #                      GNU General Public Licence
 #
 #       This program is free software; you can redistribute it and/or
 #       modify it under the terms of the GNU General Public License as
 #       published by the Free Software Foundation; either version 2 of
 #       the License, or (at your option) any later version.
 #
 #       This program is distributed in the hope that it will be useful,
 #       but WITHOUT ANY WARRANTY; without even the implied warranty of
 #       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
 #       GNU General Public License for more details.
 #
 #       You should have received a copy of the GNU General Public
 #       License along with this program; see the file COPYING. If not,
 #       write to the Free Software Foundation, Inc., 59
 #       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 #
 # ---------------------------------------------------------------------------
 */

#include <plantgl/tool/sequencer.h>
#include <boost/python.hpp>
using namespace boost::python;

TOOLS_USING_NAMESPACE

void export_Sequencer(){

  class_<Sequencer>
	("Sequencer", init<optional<double> >("Sequencer([timestep])"))
	.def("__call__", &Sequencer::touch)
	.def("touch", &Sequencer::touch)
	.add_property("timeStep",&Sequencer::getTimeStep,&Sequencer::setTimeStep)
	.def("elapsedTime",&Sequencer::elapsedTime)
	.def("ok", &Sequencer::ok)
	.def("sleep", (void(Sequencer::*)(double)const)&Sequencer::sleep)
	;
}