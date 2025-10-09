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

#include "projectionengine.h"
#include <plantgl/math/util_math.h>
#include "projectionrenderer.h"


/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE



ProjectionEngine::ProjectionEngine(enum eIdPolicy idPolicy):
    __camera(0),
    __idPolicy(idPolicy),
    __primitiveId(0)
{
    setOrthographicCamera(-1, 1, -1, 1, 0, 2);
    lookAt(Vector3(0,1,0),Vector3(0,0,0),Vector3(0,0,1));

}

ProjectionEngine::~ProjectionEngine()
{
}

void ProjectionEngine::process(ScenePtr scene)
{
    Discretizer d;
    Tesselator t;
    ProjectionRenderer r(*this, t, d);
    beginProcess();
    scene->apply(r);
    endProcess();
}

pgl_hash_map<uint32_t,std::pair<real_t,std::vector<real_t> > > ProjectionEngine::aggregate(pgl_hash_map<uint32_t,real_t> values) const
{
    typedef pgl_hash_map<uint32_t,std::pair<real_t,std::vector<real_t> > > ResultType;
    ResultType result;
    for(pgl_hash_map<uint32_t,uint32_t>::const_iterator it = __idMap.begin(); it != __idMap.end(); ++it){
        ResultType::iterator itRes;
        uint32_t idshape = it->second;
        pgl_hash_map<uint32_t,real_t>::const_iterator itvalue = values.find(it->first);
        real_t value = ( itvalue ==values.end() ? 0.0 : itvalue->second);
        if ((itRes = result.find(idshape))==result.end()){
            std::pair<real_t,std::vector<real_t> > lresult;
            lresult.second.push_back(value);
            lresult.first += value;
            result[idshape] = lresult;
        }
        else {
            itRes->second.second.push_back(value);
            itRes->second.first += value;
        }
    }
    return result;
}

ImageProjectionEngine::ImageProjectionEngine(uint16_t imageWidth, 
                                   uint16_t imageHeight,
                                   eIdPolicy idPolicy):
    ProjectionEngine(idPolicy),
    __imageWidth(imageWidth), 
    __imageHeight(imageHeight)
{
}

ImageProjectionEngine::~ImageProjectionEngine()
{
}

