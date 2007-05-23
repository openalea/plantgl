/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
 *
 *       $Source$
 *       $Id: vxl_octreenode.h 1703 2006-06-16 13:04:47Z fboudon $
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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


/*! \file vxl_octreenode.h
    \brief Definition of OctreeNode.
*/


#ifndef __vxl_octreenode_h__
#define __vxl_octreenode_h__

/* ----------------------------------------------------------------------- */

#include "voxel.h"
#include <scenegraph/scene/scene.h>

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
                   const TOOLS(Vector3)& PMin = TOOLS(Vector3::ORIGIN),
                   const TOOLS(Vector3)& PMax = TOOLS(Vector3::ORIGIN) );

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
  inline  Shape3DPtr getGeometry(uint32_t i)  {
    return __objects->getAt(i);
  }

  inline  void setGeometry(uint32_t i, const Shape3DPtr& s)  {
    __objects->setAt(i,s);
  }

  virtual uint32_t getGeometrySize()  {
    return __objects->getSize();
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
