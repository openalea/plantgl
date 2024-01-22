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
             

/*! \file light.h
    \brief Definition of light properties.
*/



#ifndef __ZBufferLight_h__
#define __ZBufferLight_h__

/* ----------------------------------------------------------------------- */

#include <plantgl/tool/rcobject.h>
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/appearance/color.h>
#include "../algo_config.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Light : public RefCountObject {
public:
    Light(const Vector3& position = Vector3(0,0,1), const Color3& ambient = Color3(255,255,255), const Color3& diffuse = Color3(255,255,255), const Color3& specular = Color3(255,255,255), bool directional = false);
    virtual ~Light();

    void set(const Vector3& position, const Color3& color = Color3(255,255,255), bool directional = false);
    void set(const Vector3& position, const Color3& ambient, const Color3& diffuse, const Color3& specular = Color3(255,255,255), bool directional = false);

    void setEnabled(bool enabled) { __enabled = enabled; }
    bool isEnabled() const { return __enabled ; }

    void setDirectionnal(bool enabled) { __directional = enabled; }
    bool isDirectionnal() const { return __directional ; }

    const Vector3& position() const { return __position; }
    Vector3& position() { return __position; }

    const Color3& ambient() const { return __ambient; }
    Color3& ambient() { return __ambient; }

    const Color3& diffuse() const { return __diffuse; }
    Color3& diffuse() { return __diffuse; }
    
    const Color3& specular() const { return __specular; }
    Color3& specular() { return __specular; }

    Vector3 direction(const Vector3& fromPosition) const;

protected:
  bool __enabled;
  bool __directional;
  Vector3 __position;
  Color3 __ambient;
  Color3 __diffuse;
  Color3 __specular;

};

typedef RCPtr<Light> LightPtr;

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
