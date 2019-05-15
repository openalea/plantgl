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

/*
** util_progress.h for plantgl in
**
** Made by julien1.benoit@epitech.eu
** Login   <julien1.benoit@epitech.eu>
**
** Started on  lun. juil. 10:58 2018 Julien Benoit
** Last update lun. juil. 10:58 2018 Julien Benoit
*/

#ifndef __util_progress_h__
#define __util_progress_h__

#include "tools_config.h"
#include "util_types.h"

#include <cstdio>
#include <string>

PGL_BEGIN_NAMESPACE

  typedef void (*progressstatusfunction)(const char *, float);

  TOOLS_API void register_progressstatus_func(progressstatusfunction func);

  TOOLS_API void unregister_progressstatus_func();

  class TOOLS_API ProgressStatus {
  public:
    ProgressStatus(uint32_t _nbsteps, const std::string &_message = " %.2f processed.", float _percenttoprint = 1);

    inline ProgressStatus &operator++() {
      increment();
      return *this;
    }

    void increment(uint32_t inc = 1);

  protected:
    uint32_t current;
    uint32_t nbsteps;
    real_t cpercent;
    real_t percenttoprint;
    std::string message;
  };

PGL_END_NAMESPACE

#endif /* !__util_progress_h__ */
