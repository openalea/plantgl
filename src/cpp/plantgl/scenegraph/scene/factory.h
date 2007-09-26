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


/*! \file scne_scene.h
    \brief Definition of the scene class Scene.
*/

#ifndef __scne_scenefactory_h__
#define __scne_scenefactory_h__

/* ----------------------------------------------------------------------- */

#include "scene.h"
#include <string>
#include <vector>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


/**
   \class SceneFActory
   \brief A Factory for Scene.
*/

/* ----------------------------------------------------------------------- */

struct SG_API SceneFormat {
	std::string name;
	std::vector<std::string> suffixes;
	std::string comment;
    bool operator==(const SceneFormat& sf) const
    { return name == sf.name && suffixes == sf.suffixes && comment == comment; }
};

typedef std::vector<SceneFormat> SceneFormatList;

class SceneFactory;

class SG_API SceneCodec : public TOOLS(RefCountObject){
public :
	friend class SceneFactory;

	enum Mode {
		None      = 0,
		Read  = 1,
		Write = 2,
		ReadWrite = 3,
	};

	SceneCodec(const std::string& name, Mode mode = None);
	virtual ~SceneCodec();

	virtual SceneFormatList formats() const = 0;

	virtual bool test(const std::string& fname, Mode openingMode);

	virtual ScenePtr read(const std::string& fname) { return ScenePtr(0); }

	virtual void write(const std::string& fname,const ScenePtr&	scene) { }

    void setName(const std::string& name) { __name = name; }
    const std::string& getName() const { return __name; }

    void setMode(Mode mode) { __mode = mode; }
    Mode getMode() const { return __mode; }
protected:
	std::string __name;
	Mode __mode;
};

typedef RCPtr<SceneCodec> SceneCodecPtr;

typedef RCPtr<SceneFactory> SceneFactoryPtr;

class SG_API SceneFactory : public TOOLS(RefCountObject)
{

public:
	typedef std::vector<SceneCodecPtr> CodecList;

	~SceneFactory();
	static SceneFactory& get();

	SceneFormatList formats( SceneCodec::Mode openingMode = SceneCodec::None ) const;

    bool isReadable(const std::string& fname);
    bool isWritable(const std::string& fname);

    ScenePtr read(const std::string& fname);
	void write(const std::string& fname,const ScenePtr&	scene);

	ScenePtr read(const std::string& fname, const std::string& codecname);
	void write(const std::string& fname,const ScenePtr&	scene, const std::string& codecname);

	void registerCodec(const SceneCodecPtr& codec);
	void unregisterCodec(const SceneCodecPtr& codec);

	bool installLib(const std::string& libname);
	bool installDefaultLib();

    typedef CodecList::const_iterator const_iterator;
    const_iterator begin() const { return __codecs.begin(); }
    const_iterator end() const { return __codecs.end(); }
protected:

	SceneFactory();
	SceneFactory& operator=(const SceneFactory&);

	SceneCodecPtr findCodec(const std::string& codecname);

	CodecList __codecs;

private:
	static SceneFactoryPtr __factory;
};


PGL_END_NAMESPACE

#endif

