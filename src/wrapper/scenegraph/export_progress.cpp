/*
** export_progress.cpp for plantgl in
**
** Made by julien1.benoit@epitech.eu
** Login   <julien1.benoit@epitech.eu>
**
** Started on  lun. juil. 11:59 2018 Julien Benoit
** Last update lun. juil. 11:59 2018 Julien Benoit
*/

#include <boost/python.hpp>
#include <plantgl/tool/util_progress.h>

TOOLS_USING_NAMESPACE
using namespace boost::python;

// to control display of progress
static boost::python::object pyprogressfunction;

void py_c_progressfunc(const char *msg, float percent) {
  pyprogressfunction(msg, percent);
}

void py_register_progressstatus_func(boost::python::object func) {
  pyprogressfunction = func;
  register_progressstatus_func(py_c_progressfunc);
}

void py_unregister_progressstatus_func() {
  pyprogressfunction = boost::python::object();
  unregister_progressstatus_func();
}

void export_Progress() {
  def("pgl_register_progressstatus_func", &py_register_progressstatus_func, args("func"));
  def("pgl_unregister_progressstatus_func", &py_unregister_progressstatus_func);
}