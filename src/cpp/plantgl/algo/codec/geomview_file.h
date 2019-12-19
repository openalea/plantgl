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

#ifndef __geomview_file_h__
#define __geomview_file_h__

/* ----------------------------------------------------------------------- */

#include <string>
#include <iostream>
#include <stack>

#include "codec_config.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/tool/util_hashmap.h>
#include <plantgl/scenegraph/appearance/color.h>
#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/scene/scene.h>
#include <plantgl/scenegraph/container/indexarray.h>

/* ----------------------------------------------------------------------- */


PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class CODEC_API GeomViewBuilder  {

public :
    GeomViewBuilder(std::istream& input,std::ostream& output);
    ~GeomViewBuilder() {}

    ScenePtr parse();
    GeometryPtr parseGeometry();
    GeometryPtr parseGeometry(std::string& token);

    std::ostream& output() { return __output; }
    std::istream& input() { return *(__input.top()); }

    std::string readline();
    std::string processToken();
    std::string readToken();
    float readFloat();
    int readInt();

    Vector2 readVector2();
    Vector3 readVector3();
    Vector4 readVector4();
    Vector3 readVector3(int n);
    Vector4 readVector4(int n);
    Color4  readUColor4();
    Color3  readUColor3();
    Color4  readColor4();
    Color3  readColor3();
    Index readIndex();


    static ScenePtr Parse(const std::string& filename, std::ostream& output);
    bool pushFile(const std::string&);
    bool popFile();
    bool endFile();
    bool endObject();
    bool endListObject();

protected :


    std::stack<std::istream *> __input;
    std::ostream& __output;
    std::string __token;
    pgl_hash_map_string<GeometryPtr> __defines;
};

/* ----------------------------------------------------------------------- */

class CODEC_API GenericBuilder {
public :
    GenericBuilder(GeomViewBuilder& d) :
       __data(d){}

    virtual ~GenericBuilder(){}
    virtual bool setTag(const std::string&) = 0;
    virtual GeometryPtr parse() = 0;
    virtual void clear() = 0;

protected :
    GeomViewBuilder& __data;
};

/* ----------------------------------------------------------------------- */

class CODEC_API GenericPointBuilder : public GenericBuilder {
public :

    GenericPointBuilder(GeomViewBuilder& d) :
       GenericBuilder(d),
       __normals(false),
       __textured(false),
       __colored(false),
       __dim4(false){}
    virtual ~GenericPointBuilder(){}

    bool hasNormal() const { return __normals; }
    bool isTextured() const { return __textured; }
    bool hasColor() const { return __colored; }
    bool isDim4() const { return __dim4; }
    virtual void clear() {
       __normals = false;
       __textured = false;
       __colored = false; 
       __dim4 = false;
    }

protected :
    bool __normals;
    bool __textured;
    bool __colored;
    bool __dim4;
};

/* ----------------------------------------------------------------------- */

class CODEC_API BezBuilder : public GenericPointBuilder {

public :
    BezBuilder(GeomViewBuilder&);
    virtual ~BezBuilder(){}

    virtual bool setTag(const std::string&);
    virtual GeometryPtr parse();
    virtual void clear();

protected :

    int __Nu;
    int __Nv;
};

/* ----------------------------------------------------------------------- */

class CODEC_API OffBuilder : public GenericPointBuilder {

public :
    OffBuilder(GeomViewBuilder&);
    virtual ~OffBuilder(){}

    virtual bool setTag(const std::string&);
    virtual GeometryPtr parse();
    virtual void clear();

protected :
    int __n;
};
/* ----------------------------------------------------------------------- */

class CODEC_API GVMeshBuilder : public GenericPointBuilder {

public :
    GVMeshBuilder(GeomViewBuilder&);
    virtual ~GVMeshBuilder(){}

    virtual bool setTag(const std::string&);
    virtual GeometryPtr parse();
    virtual void clear();

protected :
    bool __grid;
    bool __uwrapped;
    bool __vwrapped;
    int __n;
};

/* ----------------------------------------------------------------------- */

class CODEC_API QuadBuilder : public GenericPointBuilder {

public :
    QuadBuilder(GeomViewBuilder&);
    virtual ~QuadBuilder(){}

    virtual bool setTag(const std::string&);
    virtual GeometryPtr parse();

};

/* ----------------------------------------------------------------------- */

class CODEC_API VectBuilder : public GenericBuilder {

public :
    VectBuilder(GeomViewBuilder&);
    virtual ~VectBuilder(){}

    virtual bool setTag(const std::string&);
    virtual GeometryPtr parse();

    bool isDim4() const { return __dim4; }
    virtual void clear() {
       __dim4 = false;
    }

protected :

    bool __dim4;
};


/* ----------------------------------------------------------------------- */

class CODEC_API GVSphereBuilder : public GenericBuilder {

public :
    GVSphereBuilder(GeomViewBuilder&);
    virtual ~GVSphereBuilder(){}

    virtual bool setTag(const std::string&);
    virtual GeometryPtr parse();
    virtual void clear() {}
protected :

};


/* ----------------------------------------------------------------------- */


PGL_END_NAMESPACE

#endif
