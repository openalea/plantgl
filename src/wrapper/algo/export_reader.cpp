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




#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_list.h>
#include "export_printer.h"
#include <plantgl/scenegraph/scene/scene.h>

#ifdef PGL_WITH_BISONFLEX
#include <plantgl/algo/codec/cdc_geom.h>
#include <plantgl/algo/codec/scne_parser.h>
#include <plantgl/scenegraph/core/smbtable.h>
#endif
#include <plantgl/algo/codec/scne_binaryparser.h>
#include <sstream>

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
using namespace boost::python;
#define bp boost::python

#ifdef PGL_WITH_BISONFLEX

object pgl_read(const std::string& txt, const std::string& fname)
{
    SceneObjectSymbolTable table;
    ScenePtr scene;
    std::stringstream input(txt, std::ios_base::in);
        bool res = geom_read(input,table,scene);
    return make_tuple(scene,make_dict(table)());
}

#endif

void export_PglReader()
{
#ifdef PGL_WITH_BISONFLEX
    def("pgl_read",&pgl_read, (bp::arg("input"),bp::arg("fname")="string"));
#endif
    def("pglParserVerbose",&parserVerbose, (bp::arg("verbose")=true));
    def("isPglParserVerbose",&isParserVerbose);
}
