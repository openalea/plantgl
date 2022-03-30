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



/*! \file vxl_octreenode.h
    \brief Definition of OctreeNode.
*/


#ifndef __vxl_octreenode_h__
#define __vxl_octreenode_h__

/* ----------------------------------------------------------------------- */

#include "voxel.h"
#include <plantgl/scenegraph/scene/scene.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

 /**
     \class OctreeNode
     \brief A Octree Node is a Voxel that can be decomposed into 8 sub voxel to make an octree.
 */

/* ----------------------------------------------------------------------- */

class ALGO_API OctreeNode : public Voxel {

     public :


     /** Default constructor. */
        OctreeNode(Tile * Complex = 0,
                   const unsigned char Scale = 0,
                   const TileType Type = Empty,
                   const unsigned char Num = 0,
                   const Vector3& PMin = TOOLS(Vector3::ORIGIN),
                   const Vector3& PMax = TOOLS(Vector3::ORIGIN) );

     /// Destructor
     virtual ~OctreeNode();


     /// Return components of \e self.
     const OctreeNode * getComponents() const {
        return __components;
     }

     /** Set components \e _components to \e self.
        \pre
        - \e the number of components must be equal to 8. */
     bool setComponents(OctreeNode * _components);

     virtual uchar_t getComponentsSize() const {
        return (__components ? uchar_t(8) : uchar_t(0));
     }

     /** Returns the \e i-th components  of \e self
        \pre
       - \e i must be strictly less than the number of components of \e self. */
    inline OctreeNode * getComponent(unsigned char i) const {
           return &(__components[i]);
    }

  /// Return geometry intersecting \e self.
  const ScenePtr& getGeometry() const {
    return __objects;
  }

  /// Return geometry intersecting \e self.
  ScenePtr& getGeometry() {
    return __objects;
  }

  /** Returns the \e i-th geometry intersecting  \e self
      \pre
      - \e i must be strictly less than the number of components of \e self. */
  inline  Shape3DPtr getGeometry(uint_t i)  {
    return __objects->getAt(i);
  }

  inline  void setGeometry(uint_t i, const Shape3DPtr& s)  {
    __objects->setAt(i,s);
  }

  virtual uint_t getGeometrySize()  {
    return __objects->size();
  }

  virtual bool isDecomposed() const;

     /*! Decompose \e self into 8 sub octree nodes with default values :
       \post
       -  complex is \e self
       -  scale is self's scale + 1
       -  type is the same has \e self
       -  Num are automaticaly attributed
       -  pmin and pmax are calculated
       -
       -  z = 0,
       -  ----------> y
       -  | 0 | 4 |
       -  -----
       -  | 1 | 5 |
       -  -----
       -  |
       -  x
       -
       -  and z = 1
       -  ----------> y
       -  | 2 | 6 |
       -  -----
       -  | 3 | 7 |
       -  -----
       -  |
       -  x

     */
  virtual OctreeNode *  decompose();

  /// @name 6-connectivity at a scale
  //@{

  /// Return Left neighbour.
  OctreeNode * getLeft() const ;

  /// Return Right neighbour.
  OctreeNode * getRight() const ;

  /// Return Up neighbour.
  OctreeNode * getUp() const ;

  /// Return Down neighbour.
  OctreeNode * getDown() const ;

  /// Return Front neighbour.
  OctreeNode * getFront() const ;

  /// Return Back neighbour.
  OctreeNode * getBack() const ;

  ///@}



protected :

  /// Components of \e self.
  OctreeNode * __components;

  /// Object contains in the node
  ScenePtr __objects;

};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
