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

TOOLS_BEGIN_NAMESPACE

  typedef void (*progressstatusfunction)(const char *, float);

  TOOLS_API void register_progressstatus_func(progressstatusfunction func);

  TOOLS_API void unregister_progressstatus_func();

  class ProgressStatus {
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

TOOLS_END_NAMESPACE

#endif /* !__util_progress_h__ */
