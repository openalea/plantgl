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

#include "octree.h"
#include "tile.h"
#include "voxelintersection.h"
#include "../base/discretizer.h"


// #define OCTREE_FACE
// #define CPL_DEBUG

#include "../base/tesselator.h"
#include <plantgl/scenegraph/geometry/triangleset.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/scenegraph/scene/shape.h>
#include "../raycasting/ray.h"
#include "../raycasting/rayintersection.h"

#include <plantgl/tool/timer.h>
#include <plantgl/math/util_math.h>

PGL_USING_NAMESPACE

using namespace std;

Octree::Octree(const ScenePtr&  Scene,
               uint_t maxscale,
               uint_t maxelements,
               Octree::ConstructionMethod method) :
    __root(0,0,Tile::Undetermined),
    __size(0,0,0),
    __center(0,0,0),
    __scene(Scene),
    __maxscale(maxscale),
    __maxelts(maxelements),
    __nbnode(0),
    __method(method){
   build();
}

Octree::Octree(const ScenePtr&  Scene,
               const Vector3& Center, const Vector3& Size,
               uint_t maxscale,
               uint_t maxelements,
               Octree::ConstructionMethod method) :
    __root(0,0,Tile::Undetermined),
    __size(Size),
    __center(Center),
    __scene(Scene),
    __maxscale(maxscale),
    __maxelts(maxelements),
    __nbnode(0),
    __method(method){
    __root.getMinCoord() = Center-Size;
    __root.getMaxCoord() = Center+Size;
   build();
}

Octree::~Octree(){
}


bool Octree::setScene( const ScenePtr& scene){
    __scene = scene;
    __root = OctreeNode(0,0,Tile::Undetermined);
    build();
    return true;
}


bool Octree::isValid( ) const {
    return true;
}


/////////////////////////////////////////////////////////////////////////////
void Octree::build()
/////////////////////////////////////////////////////////////////////////////
{
    if (__method == ShapeBased) build1();
    else build2();
}

#define URDELTA_P(a) a *= (a >= 0 ? 1.1 : 0.9 );
#define URDELTA(a) URDELTA_P(a.x())URDELTA_P(a.y())URDELTA_P(a.z())

#define LLDELTA_P(a) a *= (a >= 0 ? 0.9 : 1.1 );
#define LLDELTA(a) LLDELTA_P(a.x())LLDELTA_P(a.y())LLDELTA_P(a.z())

/////////////////////////////////////////////////////////////////////////////
void Octree::build1()
/////////////////////////////////////////////////////////////////////////////
{
    Discretizer discretizer;
    BBoxComputer bboxcomputer(discretizer);
    VoxelIntersection intersection(bboxcomputer);

    if( __root.getMinCoord() == Vector3::ORIGIN &&
        __root.getMaxCoord() == Vector3::ORIGIN )
        {
        if(bboxcomputer.process(__scene)){
            BoundingBoxPtr bb(bboxcomputer.getBoundingBox());
            URDELTA(bb->getUpperRightCorner());
            LLDELTA(bb->getLowerLeftCorner());
            __root.setBBox(bb);
            __center = bb->getCenter();
            __size = bb->getSize();
        }
    }

    OctreeNode * node;

    OctreeNode * Complex;
    queue<OctreeNode *> _myQueue;
    _myQueue.push(&__root);
    __root.getGeometry() = __scene;
    double max_count = (pow((double)8,(double)__maxscale+1)-1)/7.0;
    double max_count2 = max_count;
    double count = 100.0;
    double count_percent = 0;
    Timer t;
    t.start();

    while(!_myQueue.empty()){
        Complex = _myQueue.front();

        if(Complex->getType() == Tile::Undetermined && Complex->getScale() < __maxscale){
          if(Complex->decompose() !=NULL){
            count+=800.0;

            ScenePtr s(Complex->getGeometry());

            for(unsigned char i = 0 ; i <  8 ; i++){
                node = Complex->getComponent(i);
                node->setComponents(0);
                ScenePtr n(new Scene());
                intersection.setVoxel(node);
                bool filled = false;
                Scene::iterator _it;
                for( _it = s->begin(); _it!=s->end() /* || filled */; _it++)
                  {
                  if((*_it)->apply(intersection))
                  n->add(*_it);
                  if(intersection.isFilled())filled = true;
                  }
                if(filled)
                  {
                  node->getType() = Tile::Filled;
                  node->getGeometry()=n;
                  int diffscale = (int)__maxscale - (int)node->getScale();
                  if(diffscale > 0)
                    {
                        double suppnb = (((double)pow(8.,diffscale+1)-1.0)/7.0)-1.0;
                    max_count-= suppnb;
                    }
                  }
                else if (n->empty()){
                    node->getType() = Tile::Empty;
                        int maxsc = (int)__maxscale;
                        int cscale = (int)node->getScale();
                    int diffscale = maxsc - cscale;
                        if(diffscale > 0){
                          double suppnb = (((double)pow(8.,diffscale+1)-1.0)/7.0)-1.0;
                      max_count-= suppnb;
                        }
                }
                else {
                    node->getType() = Tile::Undetermined;
                    node->getGeometry()=n;
                        Vector3 ns = node->getSize()/2;
                        if(fabs(ns.x()) > GEOM_EPSILON &&fabs(ns.y()) > GEOM_EPSILON &&fabs(ns.z()) > GEOM_EPSILON)
                    _myQueue.push(node);
                }
            }
        }
        }
        _myQueue.pop();
    }
    __nbnode = (uint_t)max_count2;
}
/////////////////////////////////////////////////////////////////////////////
void Octree::build3()
/////////////////////////////////////////////////////////////////////////////
{
    /** A first implementation of the triangle based octree sorting */      
    Tesselator discretizer;
    BBoxComputer bboxcomputer(discretizer);
    VoxelIntersection intersection(bboxcomputer);

    if( __root.getMinCoord() == Vector3::ORIGIN &&
        __root.getMaxCoord() == Vector3::ORIGIN )
        {
        if(bboxcomputer.process(__scene)){
            BoundingBoxPtr bb(bboxcomputer.getBoundingBox());
            URDELTA(bb->getUpperRightCorner());
            LLDELTA(bb->getLowerLeftCorner());
            __root.setBBox(bb);
            __center = bb->getCenter();
            __size = bb->getSize();
        }
    }

    OctreeNode * node;

    OctreeNode * Complex;
    queue<OctreeNode *> _myQueue;
    _myQueue.push(&__root);
    __root.getGeometry() = __scene;
    double max_count = (pow((double)8,(double)__maxscale+1)-1)/7.0;
    double max_count2 = max_count;
    double count = 100.0;
    double count_percent = 0;
    Timer t;
    t.start();

    while(!_myQueue.empty()){
        Complex = _myQueue.front();

        if(Complex->getType() == Tile::Undetermined && Complex->getScale() < __maxscale){
          if(Complex->decompose() !=NULL){
            count+=800.0;

            ScenePtr s(Complex->getGeometry());

            for(unsigned char i = 0 ; i <  8 ; i++){
                node = Complex->getComponent(i);
                node->setComponents(0);
                ScenePtr n(new Scene());
                Scene::iterator _it;
                uint_t nb_triangles= 0;
                for( _it = s->begin(); _it!=s->end(); _it++ )
                  {
                  if((*_it)->apply(discretizer))
                    {
                    TriangleSetPtr ts= discretizer.getTriangulation( );
                    Index3ArrayPtr triangles= intersect( ts, node);
                    if( ! triangles->empty() )
                      {
                      nb_triangles+= triangles->size();
                      TriangleSetPtr tri(new TriangleSet( ts->getPointList(),
                                                          triangles,
														  ts->getNormalPerVertex(),
                                                          ts->getCCW(),
                                                          ts->getSolid(),
                                                          ts->getSkeleton()));

                      n->add(Shape3DPtr(new Shape( dynamic_pointer_cast<Geometry>(tri), AppearancePtr() )));
                      }
                    }
                  }

                if( nb_triangles == 0 )
                  {
                  node->getType() = Tile::Empty;
                  int maxsc = (int)__maxscale;
                  int cscale = (int)node->getScale();
                  int diffscale = maxsc - cscale;
                  if(diffscale > 0)
                    {
                    double suppnb = (((double)pow((double)8,diffscale+1)-1.0)/7.0)-1.0;
                    max_count-= suppnb;
                    }
                  }
                else
                  {
                  node->getGeometry()= n;
                  if( nb_triangles < __maxelts )
                    {
                    node->getType() = Tile::Filled;
                    int diffscale = (int)__maxscale - (int)node->getScale();
                    if(diffscale > 0)
                      {
                      double suppnb = (((double)pow((double)8,diffscale+1)-1.0)/7.0)-1.0;
                      max_count-= suppnb;
                      }
                    }
                  else
                    {
                    node->getType() = Tile::Undetermined;
                    Vector3 ns = node->getSize()/2;
                    if( fabs(ns.x()) > GEOM_EPSILON &&
                        fabs(ns.y()) > GEOM_EPSILON &&
                        fabs(ns.z()) > GEOM_EPSILON )
                    _myQueue.push(node);
                    }
                  }

            }
        }
        }
        _myQueue.pop();
    }
    __nbnode = (uint_t)max_count2;
}

/////////////////////////////////////////////////////////////////////////////
void Octree::build2()
/////////////////////////////////////////////////////////////////////////////
{
    /** Implementation of the triangle based octree sorting 
        with max number of triangles per voxel condition used
        and fast overestimating marking of intercepted voxel 
        based on triangle bounding box */      
    Tesselator discretizer;

    BBoxComputer bboxcomputer(discretizer);
    VoxelIntersection intersection(bboxcomputer);

    if( __root.getMinCoord() == Vector3::ORIGIN &&
        __root.getMaxCoord() == Vector3::ORIGIN )
      {
      if(bboxcomputer.process(__scene))
        {
        BoundingBoxPtr bb(bboxcomputer.getBoundingBox());
         URDELTA(bb->getUpperRightCorner());
         LLDELTA(bb->getLowerLeftCorner());
        __root.setBBox(bb);
        __center = bb->getCenter();
        __size = bb->getSize();
        }
      }

    __scene->applyGeometryOnly(discretizer);

    OctreeNode * node;
    OctreeNode * Complex;

    queue<OctreeNode *> _myQueue;

    _myQueue.push(&__root);
    __root.getGeometry() = __scene;

    uint_t max_count2= 0;
    uint_t total_triangles= 0;
    uchar_t i;
    while(!_myQueue.empty())
      {
      max_count2+= 8;
      Complex = _myQueue.front();
      _myQueue.pop();

      if( Complex->getType() == Tile::Undetermined &&
          Complex->getScale() < __maxscale )
        {
        if( Complex->decompose() )
          {
          ScenePtr s(Complex->getGeometry());
          ScenePtr n[8];
          uint_t nb_triangles[8];
          double xm[8], ym[8], zm[8];
          for( i = 0 ; i <  8 ; i++ )
            {
            n[i]= ScenePtr(new Scene());
            nb_triangles[i]= 0;
            xm[i]= 0.; ym[i]= 0.; zm[i]= 0.;
            }

          // on parcourt tous les triangles du noeud complex (pere)
          // on les repartit dans le fils adequat
          Scene::iterator _it;
          for( _it = s->begin(); _it!=s->end(); _it++ )
            {
            if((*_it)->apply(discretizer))
              {
              TriangleSetPtr ts= discretizer.getTriangulation( );

              Index3ArrayPtr triangles[8];
              real_t _xm[8], _ym[8], _zm[8];
              topDown(Complex, ts, triangles, _xm, _ym, _zm);
              for( i = 0 ; i <  8 ; i++ )
                {
                if( ! triangles[i]->empty() )
                  {
                  nb_triangles[i]+= triangles[i]->size();
                  xm[i]+= _xm[i]; ym[i]+= _ym[i]; zm[i]+= _zm[i];
                  TriangleSetPtr tri(new TriangleSet( ts->getPointList(),
                                                      triangles[i],
													  ts->getNormalPerVertex(),
                                                      ts->getCCW(),
                                                      ts->getSolid(),
                                                      ts->getSkeleton()));

                  n[i]->add(Shape3DPtr(new Shape( GeometryPtr(tri), AppearancePtr() )));
                  }
                }
              }
            }

          for( i = 0 ; i <  8 ; i++ )
            {
            node = Complex->getComponent(i);
            if( ! nb_triangles[i] )
              node->getType() = Tile::Empty;
            else
              {
              node->getGeometry()= n[i];
              if( nb_triangles[i] < __maxelts )
                {
                node->getType() = Tile::Filled;
                total_triangles+= nb_triangles[i];
                }
              else
                {
                node->getType() = Tile::Undetermined;
                Vector3 ns = node->getSize()/2.;
/*                if( fabs(ns.x()) > max(xm[i],GEOM_EPSILON) &&
                    fabs(ns.y()) > max(ym[i],GEOM_EPSILON) &&
                    fabs(ns.z()) > max(zm[i],GEOM_EPSILON) )
                   {*/
                   _myQueue.push(node);
                   if( node->getScale() == __maxscale )
                     total_triangles+= nb_triangles[i];
/*                   }
                else
                  total_triangles+= nb_triangles[i];*/
                }
              }
            } // for 2
          } //if2
        }//if1
      }

    __nbnode = (uint_t)max_count2;
}

ScenePtr Octree::getRepresentation() const{
    ScenePtr _scene(new Scene());
    queue<const OctreeNode *> _myQueue;
    const OctreeNode * node = &__root;
    _myQueue.push(node);
    while(!_myQueue.empty()){
        node = _myQueue.front();
        if(node->isDecomposed()){
            for(unsigned char i = 0 ; i <  8 ; i++)
                _myQueue.push(node->getComponent(i));
        }
        else _scene->add(node->representation());
        _myQueue.pop();
    }
    return _scene;
}

/////////////////////////////////////////////////////////////////////////////
real_t Octree::getVolume(uint_t scale) const
/////////////////////////////////////////////////////////////////////////////
{
  real_t vol = 0;
  queue<const OctreeNode *> _myQueue;
  const OctreeNode * node = &__root;
  _myQueue.push(node);
  while(!_myQueue.empty()){
    node = _myQueue.front();
    if(node->isDecomposed() && (scale ==0 || node->getScale() < scale )){
      for(unsigned char i = 0 ; i <  8 ; i++)
                _myQueue.push(node->getComponent(i));
    }
    else if(node->getType() != Tile::Empty){
      Vector3 size(node->getSize());
      vol += size.x()*size.y()*size.z()*8;
    }
    _myQueue.pop();
  }
  return vol;
}

/////////////////////////////////////////////////////////////////////////////
vector<vector<uint_t> >
Octree::getDetails() const
/////////////////////////////////////////////////////////////////////////////
{
#ifdef GEOM_DEBUG
  double count = 0;
  real_t count_percent = 0;
  double nbnode = __nbnode / 100;
#endif

  vector<vector<uint_t> > result(__maxscale+1,vector<uint_t>(4,0));
  for(uint_t i = 1 ; i < __maxscale+1; i++){
    result[i][0] = i;
  }
  queue<const OctreeNode *> _myQueue;
  const OctreeNode * node = &__root;
  _myQueue.push(node);
  while(!_myQueue.empty()){
    node = _myQueue.front();
    if(node->isDecomposed()){
      for(unsigned char i = 0 ; i <  8 ; i++)
        _myQueue.push(node->getComponent(i));
    }
    if(node->getType() == Tile::Empty){
      result[node->getScale()][3]++;
    }
    else if(node->getType() == Tile::Undetermined){
      result[node->getScale()][2]++;
    }
    else if(node->getType() == Tile::Filled){
      result[node->getScale()][1]++;
    }
    _myQueue.pop();
#ifdef GEOM_DEBUG
    count++;
    if(count / nbnode - count_percent > 1){
      count_percent = count / nbnode;
      cerr << "\x0d" << "Already computed " << (uint_t)count_percent << " %" << flush;
    }
#endif
  }
  return result;
}

/////////////////////////////////////////////////////////////////////////////
vector<Vector3>
Octree::getSizes() const
/////////////////////////////////////////////////////////////////////////////
{

  vector<Vector3> result(__maxscale+1,__size);
  for(uint_t i = 1 ; i < __maxscale+1; i++){
    result[i] /= pow((double)2,(double)i);
  }
  return result;
}

/////////////////////////////////////////////////////////////////////////////
Index3ArrayPtr Octree::intersect( const TriangleSetPtr& mesh,
                                  const OctreeNode* voxel ) const
/////////////////////////////////////////////////////////////////////////////
{

  Point3ArrayPtr points(mesh->getPointList());
  Index3ArrayPtr indices(mesh->getIndexList());

  Index3ArrayPtr result( new Index3Array());
  result->reserve(mesh->getIndexList()->size());

/*
  for(Point3Array::iterator _it1 = points->begin();
      _it1 != points->end();
      _it1++)
    if(__voxel->intersect( *_it1))return true;
*/

  Index3Array::iterator _it;
  for( _it = indices->begin(); _it != indices->end(); _it++ )
    {
    Vector3 p0= points->getAt(_it->getAt(0));
    Vector3 p1= points->getAt(_it->getAt(1));
    Vector3 p2= points->getAt(_it->getAt(2));

    if( voxel->intersect( p0 ) || voxel->intersect( p1 ) ||
        voxel->intersect( p2 ) || voxel->intersect( (p0+p1+p2)/3.) )
      result->push_back(*_it);
    else
    if( voxel->intersect( p0, p1 ) )
      result->push_back(*_it);
    else
    if( voxel->intersect( p1, p2 ) )
      result->push_back(*_it);
    else
    if( voxel->intersect( p2, p0 ) )
      result->push_back(*_it);
    else
    if( voxel->intersect( p0, p1, p2 ) )
      result->push_back(*_it);
    }

  return result;
}

void printVoxel( const Tile* voxel )
{
 if( voxel->getComplex() )
   printVoxel(voxel->getComplex());
 cout<<int(voxel->getNum());
}

/////////////////////////////////////////////////////////////////////////////
bool Octree::intersect( const Ray& ray,
                        Vector3& intersection ) const
/////////////////////////////////////////////////////////////////////////////
{
  const Vector3& P= ray.getOrigin();
  const Vector3& D= ray.getDirection();

  bool x_plus= (D.x()>=0);
  bool y_plus= (D.y()>=0);
  bool z_plus= (D.z()>=0);
  /*if(fabs(D.x()) < GEOM_EPSILON)D.x() = GEOM_EPSILON;
  if(fabs(D.y()) < GEOM_EPSILON)D.y() = GEOM_EPSILON;
  if(fabs(D.z()) < GEOM_EPSILON)D.z() = GEOM_EPSILON;*/

  // on calcul le voxel dans lequel est situe le point P
  const OctreeNode* voxel= getLeafNode( P, D, &__root );

  // on intersecte les triangles contenus dans le voxel avec le rayon
  Discretizer discretizer;
  RayIntersection actionRay(discretizer);
  actionRay.setRay(ray);

  bool isOk= false;
  Vector3 pt(P);

#ifdef CPL_DEBUG
  int loop= 0;
#endif

  while( voxel )
    {
    if(voxel->getGeometry())
      {
      isOk= voxel->getGeometry()->applyGeometryOnly(actionRay);
      if(isOk) break;
      }

#ifdef CPL_DEBUG
  loop++;
#endif

    // quel est le noeud suivant?
    real_t x= pt.x(), y= pt.y(), z= pt.z();
    real_t Px= (x_plus) ? voxel->getMaxCoord().x() : voxel->getMinCoord().x();
    real_t Py= (y_plus) ? voxel->getMaxCoord().y() : voxel->getMinCoord().y();
    real_t Pz= (z_plus) ? voxel->getMaxCoord().z() : voxel->getMinCoord().z();

    real_t lx= (fabs(D.x()) < GEOM_EPSILON ? REAL_MAX : (Px-x)/D.x());
    real_t ly= (fabs(D.y()) < GEOM_EPSILON ? REAL_MAX : (Py-y)/D.y());
    real_t lz= (fabs(D.z()) < GEOM_EPSILON ? REAL_MAX : (Pz-z)/D.z());

    assert(lx> -GEOM_EPSILON); assert(ly> -GEOM_EPSILON); assert(lz>-GEOM_EPSILON); // on avance !!

    real_t l= 0;
    if( lx > ly )
      if( ly > lz )
        {
        if( z_plus )
          voxel= voxel->getUp();
        else
          voxel= voxel->getDown();
        l= lz;
        }
      else
        {
        if( y_plus )
          voxel= voxel->getRight();
        else
          voxel= voxel->getLeft();
        l= ly;
        }
    else
      if( lx > lz )
        {
        if( z_plus )
          voxel= voxel->getUp();
        else
          voxel= voxel->getDown();
        l= lz;
        }
      else
        {
        if( x_plus )
          voxel= voxel->getFront();
        else
          voxel= voxel->getBack();
        l= lx;
        }

    if(!voxel)
      return false;

    pt+= D * l; // nouveau pt
    voxel= getLeafNode(pt, D, voxel);
    }
#ifdef CPL_DEBUG
  cerr<<"nb de voxels parcourus: "<<loop<<endl;
#endif

  if(isOk)
    {
    // liste des points, prendre le plus proche
    real_t dist= REAL_MAX;
    Point3ArrayPtr pts= actionRay.getIntersection();
    assert(!pts->empty());
#ifdef CPL_DEBUG
    cerr<<"nb pts: "<<pts->size()<<endl;
#endif

    Point3Array::const_iterator it= pts->begin();
    for( ; it != pts->end(); it++ )
      {
      real_t d_cur= normSquared(P-(*it));
#ifdef CPL_DEBUG
    cerr<<"distance "<<d_cur<<" au point d'int "<< *it <<endl;
#endif
      if( d_cur < dist )
        {
        intersection= *it;
        dist= d_cur;
        }
      }
    return true;
    }
  else
    return false;
}

/////////////////////////////////////////////////////////////////////////////
const OctreeNode* Octree::getLeafNode( const Vector3& point,
                                       const Vector3& dir,
                                       const OctreeNode* iComplex ) const
/////////////////////////////////////////////////////////////////////////////
{
  const OctreeNode* node= iComplex;
  while( node->isDecomposed() )
    {
    Vector3 rest= point - node->getCenter();
    uchar_t pos= ( rest.x() > GEOM_EPSILON ) ? 1 :
                        ( ( rest.x() < -GEOM_EPSILON ) ? 0 :
                        ( ( dir.x() >= 0 ) ? 1 : 0 ) );
    pos+= ( rest.z() > GEOM_EPSILON ) ? 2 :
          ( ( rest.z() < -GEOM_EPSILON ) ? 0 :
          ( ( dir.z() >= 0 ) ? 2 : 0 ) );
    pos+= ( rest.y() > GEOM_EPSILON ) ? 4 :
          ( ( rest.y() < -GEOM_EPSILON ) ? 0 :
          ( ( dir.y() >= 0 ) ? 4 : 0 ) );
    node= node->getComponent(pos);
    }
  return node;
}

bool Octree::contains(const Vector3& v) const
{
  return __root.intersect(v);
}

bool Octree::findFirstPoint(const Ray& ray, Vector3& pt ) const
{
    real_t tnear,tfar;
    if(!ray.intersect(BoundingBox(__root.getMinCoord(),__root.getMaxCoord()),tnear,tfar))return false;
    else { pt =  ray.getAt(tnear); return true; }
}


/////////////////////////////////////////////////////////////////////////////
bool Octree::topDown( const OctreeNode* voxel,
                      const TriangleSetPtr& mesh,
                      Index3ArrayPtr triangles[],
                      real_t* xm, real_t* ym, real_t* zm )
/////////////////////////////////////////////////////////////////////////////
{
  Vector3 center= voxel->getCenter();
  uchar_t i= 0;
  for( i= 0; i < 8; i++ )
    {
    triangles[i]= Index3ArrayPtr( new Index3Array());
    xm[i]= 0.; ym[i]= 0.; zm[i]= 0.;
    }

  Point3ArrayPtr points(mesh->getPointList());
  Index3ArrayPtr indices(mesh->getIndexList());

  Vector3 p0, p1, p2;
  Vector3 d0, d1, d2;
  Vector3 h0, h1, h2;
  bool x0, x1, x2, y0, y1, y2, z0, z1, z2;
  Index3Array::iterator _it;
  for( _it = indices->begin(); _it != indices->end(); _it++ )
    {
    p0= points->getAt(_it->getAt(0));
    p1= points->getAt(_it->getAt(1));
    p2= points->getAt(_it->getAt(2));

    d0= p0 - center; d1= p1 - center; d2= p2 - center;
    h0= p0 - p1; h1= p1 - p2; h2= p2 - p0;
    real_t max_x= max(fabs(h0.x()), max(fabs(h1.x()),fabs(h2.x())));
    real_t max_y= max(fabs(h0.y()), max(fabs(h1.y()),fabs(h2.y())));
    real_t max_z= max(fabs(h0.z()), max(fabs(h1.z()),fabs(h2.z())));

    x0= ( d0.x() > 0 ); x1= ( d1.x() > 0 ); x2= ( d2.x() > 0 );
    y0= ( d0.y() > 0 ); y1= ( d1.y() > 0 ); y2= ( d2.y() > 0 );
    z0= ( d0.z() > 0 ); z1= ( d1.z() > 0 ); z2= ( d2.z() > 0 );

    bool x_cst= (x0&&x1&&x2) || (!x0&&!x1&&!x2); // x=0 or x=1
    bool y_cst= (y0&&y1&&y2) || (!y0&&!y1&&!y2); // y=0 or y=1
    bool z_cst= (z0&&z1&&z2) || (!z0&&!z1&&!z2); // z=0 or z=1

    if( x_cst && y_cst && z_cst )
      {
      uchar_t l(0);
      if(x0) l |= 1;
      if(z0) l |= 2;
      if(y0) l |= 4;
      triangles[l]->push_back(*_it);
      xm[l]+= max_x; ym[l]+= max_y; zm[l]+= max_z;
      }
    else
      {
      bool xy= x_cst && y_cst, xz= x_cst && z_cst, yz= y_cst && z_cst;
      if( xy || xz || yz )
        {
        uchar_t l1(0), l2(0);
        if( x_cst && x0 ) { l1|= 1; l2|= 1; }
        if( z_cst && z0 ) { l1|= 2; l2|= 2; }
        if( y_cst && y0 ) { l1|= 4; l2|= 4; }

        l2|= (yz) ? 1 : (xy) ? 2 : 4;
        triangles[l1]->push_back(*_it);
        triangles[l2]->push_back(*_it);
        xm[l1]+= max_x; ym[l1]+= max_y; zm[l1]+= max_z;
        xm[l2]+= max_x; ym[l2]+= max_y; zm[l2]+= max_z;
        }
      else
      if( x_cst || y_cst || z_cst )
        {
        // to do
        uchar_t l1(0), l2(0), l3(0), l4(0);
        if( x_cst && x0 ) { l1|= 1; l2|= 1; l3|= 1; l4|= 1; }
        if( z_cst && z0 ) { l1|= 2; l2|= 2; l3|= 2; l4|= 2; }
        if( y_cst && y0 ) { l1|= 4; l2|= 4; l3|= 4; l4|= 4; }
        if( x_cst ) { l2|= 2; l4|= 2; l3|= 4; l4|= 4; }
        if( y_cst ) { l2|= 1; l4|= 1; l3|= 2; l4|= 2; }
        if( z_cst ) { l2|= 1; l4|= 1; l3|= 4; l4|= 4; }
        triangles[l1]->push_back(*_it);
        triangles[l2]->push_back(*_it);
        triangles[l3]->push_back(*_it);
        triangles[l4]->push_back(*_it);
        xm[l1]+= max_x; ym[l1]+= max_y; zm[l1]+= max_z;
        xm[l2]+= max_x; ym[l2]+= max_y; zm[l2]+= max_z;
        xm[l3]+= max_x; ym[l3]+= max_y; zm[l3]+= max_z;
        xm[l4]+= max_x; ym[l4]+= max_y; zm[l4]+= max_z;
        }
      else
        {
        // gloups on est en diagonal. umps. difffficile.
        // todo
        for( i= 0; i < 8; i++ )
          {
          triangles[i]->push_back(*_it);
          xm[i]+= max_x; ym[i]+= max_y; zm[i]+= max_z;
          }
        }
      }
    }

  for( i= 0; i < 8; i++ )
    {
    real_t n= real_t(triangles[i]->size());
    xm[i]/= n; ym[i]/= n; zm[i]/= n;
    }

  return true;
}
