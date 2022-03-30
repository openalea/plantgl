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

PGL_USING_NAMESPACE
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
