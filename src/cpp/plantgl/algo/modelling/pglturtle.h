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

#ifndef __PGL_TURTLE_GEOM_H__
#define __PGL_TURTLE_GEOM_H__

#include "turtle.h"
#include "pglturtledrawer.h"

#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/scene/scene.h>
#include <plantgl/tool/util_hashmap.h>


PGL_BEGIN_NAMESPACE

class ALGO_API PglTurtle : public Turtle {
public:
    // static Polyline2DPtr DEFAULT_CROSS_SECTION;
    typedef pgl_hash_map_string<GeometryPtr> SurfaceMap;

    explicit PglTurtle(TurtleDrawerPtr drawer = TurtleDrawerPtr(new PglTurtleDrawer()),
              TurtleParam * param = NULL);

    ~PglTurtle() override;

    void reset() override;

    void clear() override;

    ScenePtr partialView();

    void defaultValue() override;

    virtual void appendMaterial(const AppearancePtr& mat);

    virtual void setMaterial(size_t pos, const AppearancePtr& mat);

    virtual void insertMaterial(size_t pos, const AppearancePtr& mat);

    inline void appendMaterial(const ImageTexturePtr& mat)
    { appendMaterial(AppearancePtr(new Texture2D(mat))); }

    inline void setMaterial(size_t pos, const ImageTexturePtr& mat)
    { setMaterial(pos,AppearancePtr(new Texture2D(mat))); }

    inline void insertMaterial(size_t pos, const ImageTexturePtr& mat)
    { insertMaterial(pos,AppearancePtr(new Texture2D(mat))); }

    virtual AppearancePtr getMaterial(size_t pos);
    virtual AppearancePtr getCurrentMaterial()  override;

    virtual void appendColor(uint_t red, uint_t green, uint_t blue);

    virtual void appendColor(float red, float green, float blue);

    virtual void appendColor(const Color3& mat);

    virtual void setColorAt(size_t pos, uint_t red, uint_t green, uint_t blue );

    virtual void setColorAt(size_t pos, float red, float green, float blue );

    virtual void setColorAt(size_t pos, const Color3& mat);

    virtual void removeColor(size_t pos);

    inline void clearColorList() {
        __appList.clear();
    }

    virtual inline size_t getColorListSize() const override
    { return __appList.size(); }

    inline const std::vector<AppearancePtr>& getColorList() const {
        return __appList;
    }

    inline void setColorList(const std::vector<AppearancePtr>& applist) {
        __appList = applist;
    }

    virtual void interpolateColors(int val1, int val2, real_t alpha = 0.5) override;

    virtual void setTextureBaseColor(const Color4& v) override;
    virtual void setTextureBaseColor(int v) override;
    virtual void interpolateTextureBaseColors(int val1, int val2, real_t alpha = 0.5) override;


    void clearSurfaceList();

    const PglTurtle::SurfaceMap& getSurfaceList() const;

    void setSurface(const std::string& name, const GeometryPtr& surf)
    {  __surfList[name] = surf; }

    void removeSurface(const std::string& name);

    GeometryPtr getSurface(const std::string& name);

    void pglShape(const GeometryPtr smb, real_t scale = 1.0);
    void pglShape(const ShapePtr shape, real_t scale = 1.0);
    void pglShape(const ScenePtr shape, real_t scale = 1.0);

    /// draw the surface identified by name, scaled by a factor scale
    void surface(const std::string& name,real_t scale) override;

    void customGeometry(const GeometryPtr smb, real_t scale=1);
protected:
    std::vector<AppearancePtr> __appList;
    SurfaceMap __surfList;

    // ScenePtr __scene;
    
};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
