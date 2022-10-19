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

#ifndef __PGL_TURTLEDRAWER_H__
#define __PGL_TURTLEDRAWER_H__

#include "turtleparam.h"

#include <string>
#include <stack>


PGL_BEGIN_NAMESPACE

class Index3Array;
typedef RCPtr<Index3Array> Index3ArrayPtr;

/// Turtle class allow rotation, displacement and drawing operation
class ALGO_API TurtleDrawer : public RefCountObject {
public:



    TurtleDrawer();

    virtual ~TurtleDrawer();

    virtual void cylinder(const uint_t id,
                          AppearancePtr appearance,
                          const FrameInfo& frameinfo, 
                          real_t length,
                          real_t radius,
                          uint_t sectionResolution){}

    virtual void frustum( const uint_t id,
                          AppearancePtr appearance,
                          const FrameInfo& frameinfo, 
                          real_t length, 
                          real_t baseradius, 
                          real_t topradius, 
                          uint_t sectionResolution){}

    virtual void generalizedCylinder(const uint_t id,
                                     AppearancePtr appearance,
                                     bool screenprojection,
                                     const Point3ArrayPtr& points,
                                     const std::vector<Vector3>& left,
                                     const std::vector<real_t>& radius,
                                     const Curve2DPtr& crossSection,
                                     bool crossSectionCCW){}

    virtual void sphere(const uint_t id,
                        AppearancePtr appearance,
                        const FrameInfo& frameinfo, 
                        real_t radius){}

    virtual void circle(const uint_t id,
                        AppearancePtr appearance,
                        const FrameInfo& frameinfo, 
                        real_t radius){}

    virtual void box(const uint_t id,
                     AppearancePtr appearance,
                     const FrameInfo& frameinfo, 
                     real_t length,
                     real_t botradius,
                     real_t topradius){}

    virtual void quad(const uint_t id,
                      AppearancePtr appearance,
                      const FrameInfo& frameinfo, 
                      real_t length, 
                      real_t botradius, 
                      real_t topradius){}

    virtual void polygon(const uint_t id,
                         AppearancePtr appearance,
                         bool screenprojection,
                         const Point3ArrayPtr& points,
                         const Index3ArrayPtr& indices){}

    virtual void arrow(const FrameInfo& frameinfo, 
                       real_t heigth, 
                       real_t cap_heigth_ratio, 
                       real_t cap_radius_ratio,
                       Color4 color) { }

    virtual void label(const Vector3& position, 
                       bool screenprojection,
                       const uint_t id,
                       AppearancePtr appearance, 
                       const std::string& text, 
                       int size = -1){}

    virtual void customGeometry(const uint_t id,
                                AppearancePtr appearance,
                                const FrameInfo& frameinfo, 
                                const GeometryPtr smb, 
                                real_t scale = 1.0);
};

typedef RCPtr<TurtleDrawer> TurtleDrawerPtr;

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

