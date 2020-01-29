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

class SG_API SceneCodec : public RefCountObject{
public :
    friend class SceneFactory;

    enum Mode {
        Nothing = 0,
        Read  = 1,
        Write = 2,
        ReadWrite = 3,
    };

    SceneCodec(const std::string& name, Mode mode = Nothing);
    virtual ~SceneCodec();

    virtual SceneFormatList formats() const = 0;

    virtual bool test(const std::string& fname, Mode openingMode);

    virtual ScenePtr read(const std::string& fname) { return ScenePtr(); }

    virtual bool write(const std::string& fname,const ScenePtr& scene) { return false; }

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

class SG_API SceneFactory : public RefCountObject
{

public:
    typedef std::vector<SceneCodecPtr> CodecList;

    ~SceneFactory();
    static SceneFactory& get();
    static void finalize();

    SceneFormatList formats( SceneCodec::Mode openingMode = SceneCodec::Mode::Nothing ) const;

    bool isReadable(const std::string& fname);
    bool isWritable(const std::string& fname);

    ScenePtr read(const std::string& fname);
    bool write(const std::string& fname,const ScenePtr& scene);

    ScenePtr read(const std::string& fname, const std::string& codecname);
    bool write(const std::string& fname,const ScenePtr& scene, const std::string& codecname);

    void registerCodec(const SceneCodecPtr& codec);
    void unregisterCodec(const SceneCodecPtr& codec);

    bool installLib(const std::string& libname);
    bool installDefaultLib();
    void clear();

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
