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

#include "util_progress.h"
#include <boost/thread.hpp>

PGL_USING_NAMESPACE

static void progressprint(const char *msg, float percent) {
	printf(msg, percent);
}

static progressstatusfunction PSFUNC = progressprint;

void PGL::register_progressstatus_func(progressstatusfunction func) {
	PSFUNC = func;
}

void PGL::unregister_progressstatus_func() {
	PSFUNC = progressprint;
}

ProgressStatus::ProgressStatus(uint32_t _nbsteps, const std::string &_message, float _percenttoprint) :
	nbsteps(_nbsteps),
	percenttoprint(_percenttoprint),
	message("\x0d" + _message),
	current(0),
	cpercent(-_percenttoprint) {
}

void ProgressStatus::setMaximum(uint32_t max) {
	nbsteps = max;
	forceSet(current);
}

void ProgressStatus::increment(uint32_t inc) {
	set(current + inc);
}

void ProgressStatus::set(uint32_t value) {
	if (current != value) {
		forceSet(value);
	}
}

void ProgressStatus::set(uint32_t value, uint32_t waitMs) {
	set(value);
	boost::this_thread::sleep_for(boost::chrono::milliseconds(waitMs));
}

void ProgressStatus::forceSet(uint32_t value) {
	current = value;
	
	if (current > nbsteps) {
		current = nbsteps;
	}
	
	if (current == nbsteps) {
		PSFUNC(message.c_str(), 100.0);
	}
	else {
		real_t const ncpercent = 100.0 * current / real_t(nbsteps);
		
		if (cpercent + percenttoprint <= ncpercent) {
			PSFUNC(message.c_str(), ncpercent);
			cpercent = ncpercent;
		}
	}
}

void ProgressStatus::forceSet(uint32_t value, uint32_t waitMs) {
	forceSet(value);
	boost::this_thread::sleep_for(boost::chrono::milliseconds(waitMs));
}

void ProgressStatus::refresh() {
	PSFUNC(message.c_str(), (cpercent > 0.0f) ? cpercent : 0.0f);
}

uint32_t ProgressStatus::get() const {
	return current;
}

bool ProgressStatus::isCompleted() const {
	return !(current < nbsteps);
}
