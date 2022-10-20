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

    PglTurtle(TurtleDrawerPtr drawer = TurtleDrawerPtr(new PglTurtleDrawer()), 
              TurtleParam * param = NULL);

    virtual ~PglTurtle();

    virtual void reset();

    void clear();

    ScenePtr partialView();

    virtual void interpolateColors(int val1, int val2, real_t alpha = 0.5);

    virtual void setTextureBaseColor(const Color4& v);
    virtual void setTextureBaseColor(int v);
    virtual void interpolateTextureBaseColors(int val1, int val2, real_t alpha = 0.5);

    void clearColorList();

    const std::vector<AppearancePtr>& getColorList() const;

    void setColorList(const std::vector<AppearancePtr>&);

    void clearSurfaceList();

    const SurfaceMap& getSurfaceList() const;

    void defaultValue();

    size_t getColorListSize() const
    { return __appList.size(); }

    void appendMaterial(const AppearancePtr& mat);

    void setMaterial(size_t pos, const AppearancePtr& mat);

    void insertMaterial(size_t pos, const AppearancePtr& mat);

    inline void appendMaterial(const ImageTexturePtr& mat)
    { appendMaterial(AppearancePtr(new Texture2D(mat))); }

    void setMaterial(size_t pos, const ImageTexturePtr& mat)
    { setMaterial(pos,AppearancePtr(new Texture2D(mat))); }

    void insertMaterial(size_t pos, const ImageTexturePtr& mat)
    { insertMaterial(pos,AppearancePtr(new Texture2D(mat))); }

    AppearancePtr getMaterial(size_t pos);

    void appendColor(uint_t red, uint_t green, uint_t blue);

    void appendColor(float red, float green, float blue);

    void appendColor(const Color3& mat);

    void setColorAt(size_t pos, uint_t red, uint_t green, uint_t blue );

    void setColorAt(size_t pos, float red, float green, float blue );

    void setColorAt(size_t pos, const Color3& mat);

    void removeColor(size_t pos);

    void setSurface(const std::string& name, const GeometryPtr& surf)
    {  __surfList[name] = surf; }

    void removeSurface(const std::string& name);

    GeometryPtr getSurface(const std::string& name);

    void pglShape(const GeometryPtr smb, real_t scale = 1.0);
    void pglShape(const ShapePtr shape, real_t scale = 1.0);
    void pglShape(const ScenePtr shape, real_t scale = 1.0);

    /// draw the surface identified by name, scaled by a factor scale
    virtual void surface(const std::string& name,real_t scale);

protected:


    SurfaceMap __surfList;
    std::vector<AppearancePtr> __appList;

    AppearancePtr getCurrentMaterial() const;
    AppearancePtr getCurrentInitialMaterial() const;

    GeometryPtr getCircle(real_t radius) const;

    // GeometryPtr transform(const GeometryPtr& obj, bool scaled = true) const;

    /// draw a frustum of length = length, bottom diameter = current width and top diameter = topdiam
    virtual void _frustum(real_t length,real_t topdiam);

    /// draw the polygon with current polygon points
    virtual void _polygon(const Point3ArrayPtr& points, bool concavetest = false);

    virtual void _quad(real_t radius, real_t botradius, real_t topradius);

    virtual void _label(const std::string& text , int size = -1);

    virtual void _frame(real_t heigth, real_t cap_heigth_ratio, real_t cap_radius_ratio, real_t color, real_t transparency);

    virtual void _arrow(real_t heigth, real_t cap_heigth_ratio, real_t cap_radius_ratio);

    // ScenePtr __scene;
    
};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
