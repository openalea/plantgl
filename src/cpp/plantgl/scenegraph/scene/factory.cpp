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


#include "factory.h"
#include <algorithm>
#include <plantgl/tool/dirnames.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/scenegraph/core/pgl_messages.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

SceneCodec::SceneCodec(const std::string& name, Mode mode):
    __name(name), __mode(mode){}

SceneCodec::~SceneCodec(){}

bool SceneCodec::test(const std::string& fname, Mode openingMode)
{
    int mode = openingMode & __mode;
    if (mode == openingMode)
    {
        SceneFormatList _formats = formats();
        std::string ext = toUpper(get_suffix(fname));
        for(SceneFormatList::const_iterator itFormat = _formats.begin();
            itFormat != _formats.end(); ++itFormat){
            for(std::vector<std::string>::const_iterator itSuffix = itFormat->suffixes.begin();
                itSuffix != itFormat->suffixes.end(); ++itSuffix){
                    if (ext == toUpper(*itSuffix)){
                        if (openingMode & Read) return exists(fname);
                        else return true;
                    }
            }
        }
    }
    return false;
}
/* ----------------------------------------------------------------------- */

SceneFactoryPtr SceneFactory::__factory;


SceneFactory::SceneFactory()
{
}

SceneFactory::~SceneFactory(){}

SceneFactory& SceneFactory::get()
{
    if(!__factory) {
        __factory = SceneFactoryPtr(new SceneFactory());
        __factory->installDefaultLib();
    }
    return *__factory;
}

void SceneFactory::finalize(){
    if(__factory) {
        __factory->clear();
    }
}

void SceneFactory::clear() { __codecs.clear(); }

SceneFormatList
SceneFactory::formats( SceneCodec::Mode openingMode ) const
{
    SceneFormatList formats;
    for(CodecList::const_iterator it = __codecs.begin();
        it !=__codecs.end(); ++it){
            if (((*it)->__mode & openingMode) == openingMode){
                SceneFormatList lformats = (*it)->formats();
                formats.insert(formats.end(),lformats.begin(),lformats.end());
            }
    }
    return formats;
}

bool SceneFactory::isReadable(const std::string& fname)
{
    if (! exists(fname))  return false;
    for(CodecList::reverse_iterator it = __codecs.rbegin(); it !=__codecs.rend(); ++it){
            SceneCodecPtr codec = *it;
        if (codec->__mode & SceneCodec::Read){
            if(codec->test(fname,SceneCodec::Read))
                return true;
        }
    }
    return false;
}

bool SceneFactory::isWritable(const std::string& fname)
{
    for(CodecList::reverse_iterator it = __codecs.rbegin(); it !=__codecs.rend(); ++it){
            SceneCodecPtr codec = *it;
        if (codec->__mode & SceneCodec::Write){
            if(codec->test(fname,SceneCodec::Write))
                return true;
        }
    }
    return false;
}

ScenePtr
SceneFactory::read(const std::string& fname)
{
    if (! exists(fname)) {
      pglErrorEx(PGLERRORMSG(C_FILE_OPEN_ERR_s),fname.c_str());
      return ScenePtr();
    };
    std::string cwd = get_cwd();
    for(CodecList::reverse_iterator it = __codecs.rbegin(); it !=__codecs.rend(); ++it){
            SceneCodecPtr codec = *it;
            if (codec->__mode & SceneCodec::Read){
                if(codec->test(fname,SceneCodec::Read)){
                    ScenePtr sc = codec->read(fname);
                    if(sc) {
                        if(get_cwd() != cwd) chg_dir(cwd);
                        return sc;
                    }
                }
            }
    }
    pglError("Cannot find codec to read scene in '%s'.",fname.c_str());
    if(get_cwd() != cwd) chg_dir(cwd);
    return ScenePtr();
}

bool SceneFactory::write(const std::string& fname,const ScenePtr& scene)
{
    std::string cwd = get_cwd();
    bool done = false;
    for(CodecList::reverse_iterator it = __codecs.rbegin();
        it !=__codecs.rend(); ++it){
            if ((*it)->__mode & SceneCodec::Write){
                if((*it)->test(fname,SceneCodec::Write)){
                    (*it)->write(fname,scene);
                    done = true;
                    break;
                }
            }
    }
    if (!done) pglError("Cannot find codec to write scene in '%s'.",fname.c_str());
    if(get_cwd() != cwd) chg_dir(cwd);
    return done;
}

SceneCodecPtr
SceneFactory::findCodec(const std::string& codecname)
{
    for(CodecList::reverse_iterator it = __codecs.rbegin();
        it !=__codecs.rend(); ++it){
            if ((*it)->__name == codecname) return *it;
    }
    return SceneCodecPtr();
}

ScenePtr
SceneFactory::read(const std::string& fname, const std::string& codecname)
{
    SceneCodecPtr codec = findCodec(codecname);
    if (codec) // && codec->test(fname,SceneCodec::Read))
        return codec->read(fname);
    else {
        // if (!codec)
            pglError("Cannot find codec : '%s'.",codecname.c_str());
        // else std::cerr << "Incompatible codec : '" << codecname << "'" << std::endl;
        return ScenePtr();
    }
}

bool
SceneFactory::write(const std::string& fname,const ScenePtr& scene, const std::string& codecname)
{
    SceneCodecPtr codec = findCodec(codecname);
    if (codec) // && codec->test(fname,SceneCodec::Write))
        return codec->write(fname,scene);

    else {
        // if (!codec)
        pglError("Cannot find codec : '%s'.",codecname.c_str() );
        // else std::cerr << "Incompatible codec : '" << codecname << "'" << std::endl;
        return false;
    }
}

void SceneFactory::registerCodec(const SceneCodecPtr& codec)
{
    if(codec){
        CodecList::iterator it = find(__codecs.begin(),__codecs.end(),codec);
        if(it == __codecs.end()) __codecs.push_back(codec);
    }
}

void SceneFactory::unregisterCodec(const SceneCodecPtr& codec)
{
    CodecList::iterator it = find(__codecs.begin(),__codecs.end(),codec);
    if(it != __codecs.end())__codecs.erase(it);
}

#ifndef PGL_CORE_WITHOUT_QT
#include <QtCore/QLibrary>
#endif

bool SceneFactory::installDefaultLib()
{
    return false;
}

typedef void (*installFunc)();

bool SceneFactory::installLib(const std::string& libname)
{
#ifndef PGL_CORE_WITHOUT_QT
    installFunc installCodecs = (installFunc)QLibrary::resolve(QString(libname.c_str()),"installCodecs");
    if(installCodecs){
         installCodecs();
         return true;
     }
     else  return false;
#else
        return false;
#endif
}
