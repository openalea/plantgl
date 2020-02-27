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

#ifndef __PGL_PGLTURTLEDRAWER_H__
#define __PGL_PGLTURTLEDRAWER_H__

#include "turtledrawer.h"
#include "turtledrawer.h"

#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/scene/scene.h>


PGL_BEGIN_NAMESPACE

/// Turtle class allow rotation, displacement and drawing operation
class ALGO_API PglTurtleDrawer : public TurtleDrawer {
public:

    static AppearancePtr HEADING_FRAME_MATERIAL;
    static AppearancePtr UP_FRAME_MATERIAL;
    static AppearancePtr LEFT_FRAME_MATERIAL;
    static GeometryPtr DEFAULT_SPHERE;

    PglTurtleDrawer();

    virtual ~PglTurtleDrawer();

    const ScenePtr& getScene() const
    { return __scene;  }

    virtual void cylinder(const TurtleParam * params, real_t length, real_t radius);

    virtual void frustum(const TurtleParam * params, real_t length, real_t baseradius, real_t topradius);

    virtual void generalizedCylinder(const TurtleParam * params,
                                     const Point3ArrayPtr& points,
                                     const std::vector<Vector3>& left,
                                     const std::vector<real_t>& radius,
                                     const Curve2DPtr& crossSection,
                                      bool crossSectionCCW,
                                      bool currentcolor = false);

    virtual void sphere(const TurtleParam * params, real_t radius);

    virtual void circle(const TurtleParam * params, real_t radius);

    virtual void box(const TurtleParam * params, real_t radius, real_t botradius, real_t topradius);

    virtual void quad(const TurtleParam * params, real_t radius, real_t botradius, real_t topradius);

    virtual void polygon(const TurtleParam * params, const Point3ArrayPtr& points, bool concavetest = false);

    virtual void vector(const TurtleParam * params, real_t heigth, real_t cap_heigth_ratio, real_t cap_radius_ratio, real_t color, real_t transparency);

    virtual void label(const TurtleParam * params, const std::string& text, int size = -1);

    virtual void customGeometry(const GeometryPtr smb, real_t scale = 1.0);
protected:

    AppearancePtr getCurrentMaterial() const;

    AppearancePtr getCurrentInitialMaterial() const;

    GeometryPtr getCircle(real_t radius) const;

    GeometryPtr transform(const GeometryPtr& obj, bool scaled = true) const;


    void _addToScene(const TurtleParam * params, const GeometryPtr geom, bool customid = false, AppearancePtr app = NULL, bool projection = true);

    ScenePtr __scene;

};

typedef RCPtr<PglTurtleDrawer> PglTurtleDrawerPtr;

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

