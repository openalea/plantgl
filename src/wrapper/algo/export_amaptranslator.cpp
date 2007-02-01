/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       GeomPy: Python wrapper for the Plant Graphic Library
 *
 *       Copyright 2004 UMR AMAP 
 *
 *       File author(s): C. Pradal (christophe.pradal@cirad.fr)
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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

#include "amaptranslator.h"

#include "actn_amaptranslator.h"
#include "actn_discretizer.h"

#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

GEOM_USING_NAMESPACE
TOOLS_USING_NAMESPACE

using namespace boost::python;
using namespace std;

/*
AmapSymbolPtr at_getSymbol( AmapTranslator* d )
{ return d->getSymbol(); }
*/
AmapTranslator* at_make(Discretizer* d)
{
  return new AmapTranslator(*d);
}
void class_AmapTranslator()
{
  class_< AmapTranslator,bases< Action >,boost::noncopyable >
    ( "AmapTranslator", no_init )
    .def("__init__",make_constructor(at_make) , ( const char* )"AmapTranslator(Discretizer d) -> Translate a Geom object to an Amap Symbol.")
    .def("setNormalized",&AmapTranslator::setNormalized)
    .def("isNormalized",&AmapTranslator::isNormalized)
    ;
}
