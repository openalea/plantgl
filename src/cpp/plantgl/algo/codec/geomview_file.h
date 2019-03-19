/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
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
