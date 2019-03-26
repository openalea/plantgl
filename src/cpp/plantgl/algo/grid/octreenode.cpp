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



#include "octreenode.h"
#include <plantgl/math/util_math.h>
#include <bitset>

PGL_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */

OctreeNode::OctreeNode(Tile * Complex,
                       unsigned char Scale,
                       TileType Type,
                       const unsigned char Num,
                       const Vector3& PMin,
                       const Vector3& PMax) :
    Voxel(Complex,Scale,Type,Num,PMin,PMax),
    __components(NULL),
    __objects(){
}


OctreeNode::~OctreeNode(){
    if(__components) delete [] __components;
}

bool OctreeNode::setComponents(OctreeNode * _components){
    __components =_components;
    return true;
}

bool OctreeNode::isDecomposed() const{
    return (__components != NULL);
}


OctreeNode *  OctreeNode::decompose(){
    if(!__components){

        Vector3 __center = (__ll+__ur)/2;
        if(!(fabs(__center.x()-__ll.x()) > GEOM_EPSILON &&
             fabs(__center.y()-__ll.y()) > GEOM_EPSILON &&
                 fabs(__center.y()-__ll.y()) > GEOM_EPSILON )){
                __components = NULL;
      return __components;
        }

        __components = new OctreeNode[8];
        for(unsigned char _i1 = 0; _i1 < 8 ; _i1++)
                __components[_i1] = OctreeNode(this,(unsigned int)(__scale+1),__type,0,__ll,__center);

        for(unsigned char _i = 1; _i < 8 ; _i++)
            __components[_i].getNum() = _i;

        __components[1].getMinCoord() = Vector3(__center.x() , __ll.y()     , __ll.z());
        __components[1].getMaxCoord() = Vector3(__ur.x()     , __center.y() , __center.z());

        __components[2].getMinCoord() = Vector3(__ll.x()     , __ll.y()     , __center.z());
        __components[2].getMaxCoord() = Vector3(__center.x() , __center.y() , __ur.z());

        __components[3].getMinCoord() = Vector3(__center.x() , __ll.y()     , __center.z());
        __components[3].getMaxCoord() = Vector3(__ur.x()     , __center.y() , __ur.z());

        __components[4].getMinCoord() = Vector3(__ll.x()     , __center.y() , __ll.z());
        __components[4].getMaxCoord() = Vector3(__center.x() , __ur.y()     , __center.z());

        __components[5].getMinCoord() = Vector3(__center.x() , __center.y() , __ll.z());
        __components[5].getMaxCoord() = Vector3(__ur.x()     , __ur.y()     , __center.z());

        __components[6].getMinCoord() = Vector3(__ll.x()     , __center.y() , __center.z());
        __components[6].getMaxCoord() = Vector3(__center.x() , __ur.y()     , __ur.z());

        __components[7].getMinCoord() = __center;
        __components[7].getMaxCoord() = __ur;

    }

    return __components;
}



/* ----------------------------------------------------------------------- */

OctreeNode * OctreeNode::getRight() const {
    if(!__Complex) return NULL;
    OctreeNode * _a = (OctreeNode *)__Complex;

    bitset<8> location(__num);
    if(  location.test(2) ) // y==1
      {
      // assert(int(__num) < 4);
      _a = _a->getRight();
      if(!(_a) || !_a->isDecomposed()) return _a;
      }

    return _a->getComponent( location.flip(2).to_ulong() );
}

OctreeNode * OctreeNode::getLeft() const {
    if(!__Complex) return NULL;
    OctreeNode * _a = (OctreeNode *)__Complex;
    bitset<8> location(__num);
    if( !location.test(2) ) // y==0
      {
      // assert(int(__num) > 3);
      _a = _a->getLeft();
      if(!(_a) || !_a->isDecomposed()) return _a;
      }

    return _a->getComponent( location.flip(2).to_ulong() );
}

OctreeNode * OctreeNode::getUp() const {
    if(!__Complex) return NULL;
    OctreeNode * _a = (OctreeNode *)__Complex;

    bitset<8> location(__num);
    if( location.test(1) ) // z==1
      {
      _a = _a->getUp();
      if(!(_a) || !_a->isDecomposed()) return _a;
      }

    return _a->getComponent( location.flip(1).to_ulong() );
}

OctreeNode * OctreeNode::getDown() const {
    if(!__Complex) return NULL;
    OctreeNode * _a = (OctreeNode *)__Complex;

    bitset<8> location(__num);
    if( !location.test(1) ) // z==0
      {
      _a = _a->getDown();
      if(!(_a) || !_a->isDecomposed()) return _a;
      }

    return _a->getComponent( location.flip(1).to_ulong() );
}

OctreeNode * OctreeNode::getFront() const {
    if(!__Complex) return NULL;
    OctreeNode * _a = (OctreeNode *)__Complex;
    bitset<8> location(__num);
    if( location.test(0) ) // x==1
      {
      _a = _a->getFront();
      if(!(_a) || !_a->isDecomposed()) return _a;
      }

    return _a->getComponent( location.flip(0).to_ulong() );
}

OctreeNode * OctreeNode::getBack() const {
    if(!__Complex) return NULL;
    OctreeNode * _a = (OctreeNode *)__Complex;
    bitset<8> location(__num);
    if( !location.test(0) ) // x==0
      {
      _a = _a->getBack();
      if(!(_a) || !_a->isDecomposed()) return _a;
      }

    return _a->getComponent( location.flip(0).to_ulong() );
}
