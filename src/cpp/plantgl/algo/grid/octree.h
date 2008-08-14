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


/*! \file mvs_octree.h
    \brief Definition of Octree.
*/


#ifndef __mvs_octree_h__
#define __mvs_octree_h__

/* ----------------------------------------------------------------------- */

#include "mvs.h"
#include "octreenode.h"
#include <queue>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

class TriangleSet;
typedef RCPtr<TriangleSet> TriangleSetPtr;
class Index3Array;
typedef RCPtr<Index3Array> Index3ArrayPtr;
class Ray;
/* ----------------------------------------------------------------------- */

/**
    \class Octree
    \brief A multiscale space partionning based on regular subdivision.
*/

/* ----------------------------------------------------------------------- */

class ALGO_API Octree : public Mvs
{

public:
    enum ConstructionMethod {
        TriangleBased,
        ShapeBased
    } ;

  /// Default constructor. Use Bouding Box of \e Scene for center and Size of the space.
  Octree( const ScenePtr& Scene,
          uint_t maxscale = 10,
          uint_t maxelements = 10,
          ConstructionMethod method = TriangleBased);

  /// Constructor. Use center and size to define the space the decomposed space.
  Octree( const ScenePtr& Scene,
          const TOOLS(Vector3)& center, const TOOLS(Vector3)& size,
          uint_t maxscale = 10,
          uint_t maxelements = 10,
          ConstructionMethod method = TriangleBased);

  /// Destructor
  virtual ~Octree( );

  ///  Get the size from \e self.
  virtual const TOOLS(Vector3)& getSize() const{
    return __size;
  }

  ///  Get the center from \e self.
  virtual const TOOLS(Vector3)& getCenter() const{
    return __center;
  }

  ///  Get the node from \e self.
  virtual const OctreeNode& getRoot() const{
    return __root;
  }

  ///  Return the maximum scale from \e self.
  virtual uint_t getDepth() const{
    return __maxscale;
  }

  ///  Set the scene \e scene to \e self.
  virtual bool setScene( const ScenePtr&  scene);

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

  /// Return a representation of the octree.
  ScenePtr getRepresentation() const;

  /// Return the volume of the octree at a scale
  real_t getVolume(uint_t scale = 0) const;

  /*! Return the details of the octree.
    on a vector of set of real values.
    the set contains the scale, the nb of entity filled, undetermined, empty. (a tab of 4 uint_t)
  */
  std::vector<std::vector<uint_t> > getDetails() const;

  /// Return the size of the entity at the different scale.
  std::vector<TOOLS(Vector3) > getSizes() const;

  //

  bool intersect( const Ray& ray, TOOLS(Vector3)& intersection ) const;

  bool contains(const TOOLS(Vector3)& v) const;

  bool findFirstPoint(const Ray& ray, TOOLS(Vector3)& pt ) const;

protected:

  /// Build method
  void build();

  /// Shape based octree sorting. It includes interception test based on implicit equation.
  void build1();

  /*! Implementation of the triangle based octree sorting. 
      with max number of triangles per voxel condition used
      and fast overestimating marking of intercepted voxel 
      based on triangle bounding box */
  void build2();

  /*! A first implementation of the triangle based octree sorting */
  void build3();

  /// The recursive structure.
  OctreeNode __root;

  /// Size of the scene.
  TOOLS(Vector3) __size;

  /// Center of the scene.
  TOOLS(Vector3) __center;

  /// Scene contained in the octree.
  ScenePtr __scene;

  /// Maximum scale of the octree.
  uint_t __maxscale;

  /// Maximum number of elements store by each node.
  uint_t __maxelts;

  /// number of node  of the octree.
  uint_t __nbnode;

  /// The construction method
  ConstructionMethod __method;

private:
  Index3ArrayPtr intersect( const TriangleSetPtr& mesh,
                            const OctreeNode* voxel ) const;

  /// get the deepest node where point is suituated
  const OctreeNode * getLeafNode( const TOOLS(Vector3)& point,
                                 const TOOLS(Vector3)& dir,
                                 const OctreeNode* iComplex ) const;

  bool topDown( const OctreeNode* voxel,
                const TriangleSetPtr& mesh,
                Index3ArrayPtr triangles[],
                real_t* xm, real_t* ym, real_t* zm );

  template <class T> static T power( const T& a, uchar_t b )
    {
    T r= a;
    for( uchar_t i= 0; i < b-1; i++ )
      r*= a;
    return r;
    }

}; // class Octree

  template<class condition>
        ScenePtr getCondRepresentation(const Octree& o, condition a) {
    ScenePtr _scene(new Scene());
        std::queue<const OctreeNode *> _myQueue;
    const OctreeNode * node = &o.getRoot();
    _myQueue.push(node);
    while(!_myQueue.empty()){
      node = _myQueue.front();
      if(node->isDecomposed()){
            for(unsigned char i = 0 ; i <  8 ; i++)
          _myQueue.push(node->getComponent(i));
      }
      if(a(node))_scene->add(node->representation());
      _myQueue.pop();
        }
    return _scene;
  }


/// Octree Pointer
typedef RCPtr<Octree> OctreePtr;


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
// __mvs_octree_h__
#endif
