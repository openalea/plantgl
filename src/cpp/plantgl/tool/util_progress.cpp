/*
** util_progress.cpp for plantgl in
**
** Made by julien1.benoit@epitech.eu
** Login   <julien1.benoit@epitech.eu>
**
** Started on  lun. juil. 11:00 2018 Julien Benoit
** Last update lun. juil. 11:00 2018 Julien Benoit
*/

#include "util_progress.h"

TOOLS_USING_NAMESPACE

static void progressprint(const char *msg, float percent) { printf(msg, percent); }

static progressstatusfunction PSFUNC = progressprint;

void TOOLS(register_progressstatus_func)(TOOLS(progressstatusfunction) func) {
  PSFUNC = func;
}

void TOOLS(unregister_progressstatus_func)() {
  PSFUNC = progressprint;
}

ProgressStatus::ProgressStatus(uint32_t _nbsteps, const std::string &_message, float _percenttoprint)
        : nbsteps(_nbsteps),
          percenttoprint(_percenttoprint),
          message(_message),
          current(1),
          cpercent(-_percenttoprint) {

}

void ProgressStatus::increment(uint32_t inc) {
  current += inc;
  if (current <= nbsteps) {
    if (current == nbsteps) {
      std::string msg("\x0d");
      msg += message;
      msg += "\n";
      PSFUNC(msg.c_str(), 100.0);
    } else {
      real_t ncpercent = 100 * current / float(nbsteps);
      if (cpercent + percenttoprint <= ncpercent) {
        std::string msg("\x0d");
        msg += message;
        PSFUNC(msg.c_str(), ncpercent);
        cpercent = ncpercent;
      }
    }
  }
}
