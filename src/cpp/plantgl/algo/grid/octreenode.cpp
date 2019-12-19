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
