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


/* ----------------------------------------------------------------------- */

#include "cdc_vgstar.h"

#include "vgsfile.h"
#include <plantgl/algo/base/tesselator.h>
#include <plantgl/tool/errormsg.h>
#include "vgstarprinter.h"
#include <fstream>

PGL_USING_NAMESPACE


using namespace std;

/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

VgStarCodec::VgStarCodec() : 
    SceneCodec("VGS", ReadWrite ) 
    {}

SceneFormatList VgStarCodec::formats() const
{
    SceneFormat _format;
    _format.name = "VGS";
    _format.suffixes.push_back("vgs");
    _format.suffixes.push_back("vgx");
    _format.comment = "The VegeStar format.";
    SceneFormatList _formats;
    _formats.push_back(_format);
    return _formats;
}

ScenePtr VgStarCodec::read(const std::string& fname)
{
  return VegeStarFile::read(fname,*PglErrorStream::error);
}

bool VgStarCodec::write(const std::string& fname,const ScenePtr& scene)
{
    std::ofstream stream(fname.c_str());
    if(stream){
        Tesselator t;
        VgstarPrinter p(stream,t);
        p.header();
        scene->apply(p);
        return true;
    }
    return false;
}
