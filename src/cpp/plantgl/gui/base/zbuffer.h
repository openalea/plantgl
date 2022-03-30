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



/*! \file view_zbuffer.h
    \brief Definition of the viewer class ViewGLFrame.
*/



#ifndef __view_zbuffer_h__
#define __view_zbuffer_h__

#include <vector>

/* ----------------------------------------------------------------------- */

#include "../gui_config.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/tool/util_array2.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/colorarray.h>
#include <plantgl/scenegraph/appearance/color.h>

struct VIEW_API RayHit {
public:
    RayHit (size_t _id,real_t _zmin,real_t _zmax):id(_id),zmin(_zmin),zmax(_zmax){}

    inline bool operator==(const RayHit& phit) const
    {  return phit.id == id &&  phit.zmin == zmin && phit.zmax == zmax; }

    size_t id;
    real_t zmin;
    real_t zmax;
};

typedef std::vector<RayHit > RayHitList;

class VIEW_API ViewRayBuffer : public PGL(Array2)<RayHitList>
{
public:
    ViewRayBuffer(size_t w, size_t h): PGL(Array2)<RayHitList>(w,h){}

    void setAt(size_t i, size_t j, void * buffer, size_t size,const PGL(Vector3)& position) ;
};

struct VIEW_API RayPointHit {
public:
    RayPointHit (size_t _id,
                 const PGL(Vector3)& _zmin,
                 const PGL(Vector3)& _zmax):
        id(_id),zmin(_zmin),zmax(_zmax){}

    inline bool operator==(const RayPointHit& phit) const
    {  return phit.id == id &&  phit.zmin == zmin && phit.zmax == zmax; }

    size_t id;
    PGL(Vector3) zmin;
    PGL(Vector3) zmax;
};

typedef std::vector<RayPointHit > RayPointHitList;

class VIEW_API ViewRayPointHitBuffer : public PGL(Array2)<RayPointHitList>
{
public:
    ViewRayPointHitBuffer(size_t w, size_t h): PGL(Array2)<RayPointHitList>(w,h){}

    /** \brief Add pBuff to \e self.
      \pre
      - \e pBuff and self must have the same size. */
    ViewRayPointHitBuffer& operator+=(const ViewRayPointHitBuffer& pBuff);

    /** \brief Add pBuff to \e self.
      \pre
      - \e pBuff and self must have the same size. */
    ViewRayPointHitBuffer operator+(const ViewRayPointHitBuffer& pBuff)const;
};

struct VIEW_API ZBufferUnit {
public:
    ZBufferUnit(PGL(Vector3) _pos = PGL(Vector3::ORIGIN),
                PGL(Color4) _color = PGL(Color4::BLACK),
                float _depth = 1.0):pos(_pos),color(_color),depth(_depth){}

    inline bool operator==(const ZBufferUnit & pValue) const
    { return pValue.pos == pos && pValue.color == color && pValue.depth == depth; }

    PGL(Vector3) pos;
    PGL(Color4) color;
    float depth;
};

class VIEW_API ViewZBuffer : public PGL(Array2)<ZBufferUnit>
{
public:
    ViewZBuffer(size_t w, size_t h): PGL(Array2)<ZBufferUnit>(w,h){}

    /// import depth and color buffer
    static ViewZBuffer* importglZBuffer(bool alldepth = true, bool invertalpha = true);

    /// import depth buffer
    static ViewZBuffer* importglDepthBuffer(bool alldepth = true);

    /// import depth and color points
    static std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)> importglZBufferPoints(bool invertalpha = true);

    /// import depth and color points
    static std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)> importglZBufferPointsWithJitter(real_t jitter = 0, int raywidth = 0, bool invertalpha = true, bool mixcolor = true);

};


#endif

