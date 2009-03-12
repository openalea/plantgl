/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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

#include "factory.h"
#include <algorithm>
#include <plantgl/tool/dirnames.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/scenegraph/core/pgl_messages.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

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
	if(get_cwd() != cwd) chg_dir(cwd);
	return ScenePtr();
}

void SceneFactory::write(const std::string& fname,const ScenePtr& scene)
{
	std::string cwd = get_cwd();
	for(CodecList::reverse_iterator it = __codecs.rbegin();
		it !=__codecs.rend(); ++it){
			if ((*it)->__mode & SceneCodec::Write){
				if((*it)->test(fname,SceneCodec::Write)){
					(*it)->write(fname,scene);
					break;
				}
			}
	}
	if(get_cwd() != cwd) chg_dir(cwd);
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
        if (!codec) std::cerr << "Cannot find codec : '" << codecname << "'" << std::endl;
        else std::cerr << "Incompatible codec : '" << codecname << "'" << std::endl;
        return ScenePtr();
    }
}

void 
SceneFactory::write(const std::string& fname,const ScenePtr& scene, const std::string& codecname)
{
	SceneCodecPtr codec = findCodec(codecname);
	if (codec) // && codec->test(fname,SceneCodec::Write))
		codec->write(fname,scene);
    else {
        if (!codec) std::cerr << "Cannot find codec : '" << codecname << "'" << std::endl;
        else std::cerr << "Incompatible codec : '" << codecname << "'" << std::endl;
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

#include <qlibrary.h>

bool SceneFactory::installDefaultLib()
{
	return false;
}

typedef void (*installFunc)();

bool SceneFactory::installLib(const std::string& libname)
{
	installFunc installCodecs = (installFunc)QLibrary::resolve(QString(libname.c_str()),"installCodecs");
	 if(installCodecs){
		 installCodecs();
		 return true;
	 }
	 else return false;
}
