/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): C. Xavier
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

#ifndef __actn_skeleton_h__
#define __actn_skeleton_h__

#include <plantgl/algo/codec/printer.h>
#include <plantgl/algo/base/discretizer.h>
#include <set>
#include <stack>
#include <map>

#include <plantgl/scenegraph/transformation/scaled.h>
#include <plantgl/scenegraph/transformation/translated.h>
#include <plantgl/scenegraph/transformation/oriented.h>
#include <plantgl/scenegraph/geometry/cylinder.h>
#include <plantgl/scenegraph/geometry/sphere.h>
#include <plantgl/scenegraph/geometry/triangleset.h>
#include <plantgl/scenegraph/geometry/extrudedhull.h>
#include <plantgl/scenegraph/geometry/asymmetrichull.h>
#include <plantgl/scenegraph/geometry/cone.h>
#include <plantgl/scenegraph/geometry/box.h>
#include <plantgl/scenegraph/geometry/frustum.h>
#include <plantgl/scenegraph/geometry/extrusion.h>
#include <plantgl/scenegraph/geometry/nurbscurve.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/tool/util_string.h>
#include <plantgl/tool/dirnames.h>

#include <plantgl/math/util_matrixmath.h>
#include "miniball.h"
#include "eigenvector.h"

// #define WITHOUT_QHULL

#ifndef WITHOUT_QHULL
extern "C" {
#include <qhull/qhull_a.h>
}
#endif

TOOLS_USING_NAMESPACE


#define GEOM_DEBUG

/* ----------------------------------------------------------------------- */
#include <string>
#include "../algo_config.h"
#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/geometry/polyline.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class SkelTriangle;
//typedef RCPtr<SkelTriangle> SkelTrianglePtr;
typedef SkelTriangle* SkelTriangleWeakPtr;

class ShapePoint
{
public:
  Vector2 m_vec;
  int m_ind;

  ShapePoint(Vector2 vec = Vector2(), int ind = -1) : m_vec(vec), m_ind(ind) {}
  ShapePoint(double x = 0, double y = 0, int ind = -1) : m_vec(Vector2(x,y)), m_ind(ind){}
  double x() const {return m_vec.x();}
  double y() const {return m_vec.y();}
  Vector2 operator()() {return m_vec;}
  bool operator<(const ShapePoint& sp) const
  {
    //std::cerr << "operator < shapepoint" << std::endl;
    if (x() == sp.x())
      return y() < sp.y();
    else
      return x() < sp.x();
  }
  bool operator==(const ShapePoint& sp) const
  {
    return m_vec == sp.m_vec;
  }
  bool operator==(const Vector2& v) const
  {
    //std::cerr << "comp vector2 with pointshape" << std::endl;
    return m_vec == v;
  }
};

typedef ShapePoint* ShapePointWeakPtr;
class Skeleton;
class SkelTriangleSet;
class SkelEdgeSet;
class SkelEdge : public TOOLS::RefCountObject
{
public:
  friend class PGL::SkelTriangle;
  friend class PGL::Skeleton;
  friend class PGL::SkelEdgeSet;

  enum TypeEdge{UNDEFINED, BOUNDARY, INTERIOR};

protected:
  ShapePointWeakPtr m_p1;
  ShapePointWeakPtr m_p2;
  TypeEdge m_type;
  bool m_visited;
  SkelTriangleWeakPtr m_adjTri1;
  SkelTriangleWeakPtr m_adjTri2;
  bool m_infinite;

public:
  // TODDO : add constructors from vector2
  SkelEdge(Vector2 p1,
	   Vector2 p2);

  SkelEdge(ShapePointWeakPtr p1 ,
	   ShapePointWeakPtr p2 , 
	   TypeEdge type = UNDEFINED, 
	   bool infinite = false, 
	   bool visited = false);

  SkelEdge(ShapePointWeakPtr p1 ,
	   ShapePointWeakPtr p2 , 
	   bool boundary , 
	   bool infinite = false,
	   bool visited = false);

  SkelEdge(const SkelEdge& e);

  ~SkelEdge();

  double length() const;

  double squareLength() const;

  void addAdjTri(SkelTriangleWeakPtr t);

  void rmAdjTri(SkelTriangle& t);

  bool operator==(const SkelEdge& e) const;

  Vector2 getMiddle() const;

  bool operator<(const SkelEdge& e) const;
};

typedef RCPtr<SkelEdge> SkelEdgePtr;

class SkelTriangle : public TOOLS::RefCountObject
{
public:
  friend class PGL::SkelEdge;
  friend class PGL::Skeleton;
  friend class PGL::SkelTriangleSet;

  enum TypeTriangle{UNDEFINED, SLEEVE, TERMINATION, JONCTION};
protected:
  SkelEdgePtr m_e1;
  SkelEdgePtr m_e2;
  SkelEdgePtr m_e3;
  TypeTriangle m_type;
  bool m_infinite;

public:
  SkelTriangle(SkelEdgePtr e1 = 0, SkelEdgePtr e2 = 0, SkelEdgePtr e3 = 0, bool infinite = false);
  SkelTriangle(const SkelTriangle& t);
  SkelTriangle(ShapePointWeakPtr p1, ShapePointWeakPtr p2, ShapePointWeakPtr p3);
  ~SkelTriangle();
  int getMiddleSummitForTerm();
  bool operator==(const SkelTriangle& t) const;
  double area() const;
  bool operator<(const SkelTriangle& t) const;
  Vector2 getGravityCenter () const;
  Vector2 getCircumCenter() const;
  bool isAcuteAngleTriangle() const;
  Vector2 getPseudoCircumCenter();
  ShapePointWeakPtr getOppositePoint (const SkelEdge& e) const;

};

class CompareTrianglePtr
{
public:
  bool operator()(const SkelTriangleWeakPtr tleft, const SkelTriangleWeakPtr tright) const
  {
    return (*tleft < *tright);
  }
};

class CompareSkelEdgePtr
{
public:
  bool operator()(const SkelEdgePtr eleft, const SkelEdgePtr eright) const
  {
    return (*eleft < *eright);
  }
};

class CompareVector2
{
public:
  bool operator()(const Vector2 v1, const Vector2 v2) const
  {
    if (v1.x() == v2.x())
      return v1.y() < v2.y();
    else
      return v1.x() < v2.x();
  }
};

class SkelTriangleSet : public std::set<SkelTriangleWeakPtr , CompareTrianglePtr>
{
public:
  typedef std::set<SkelTriangleWeakPtr , CompareTrianglePtr>::iterator iterator;

  ~SkelTriangleSet();
  SkelTriangleWeakPtr find_next();
  bool skelErase(SkelTriangleWeakPtr toErase);
};

class SkelEdgeSet : public std::set<SkelEdgePtr, CompareSkelEdgePtr>
{
public:
  typedef std::set<SkelEdgePtr, CompareSkelEdgePtr>::iterator iterator;

  ~SkelEdgeSet();
  bool skelErase(SkelEdgePtr toErase);
};

class SkelBranch;
typedef RCPtr<SkelBranch> SkelBranchPtr;
class ShapePointSet;

class SkelBranch : public TOOLS::RefCountObject
{
  friend class Skeleton;
public:
  enum EndType{JONCTION,TERMINATION,UNDEFINED};
protected:
  Polyline2DPtr m_branch;
  EndType m_firstEnd;
  EndType m_secondEnd;
  ShapePointSet* m_pointSet;
 
  int m_beginBumpIndice;
  int m_endBumpIndice;
  int m_middleBumpIndice;

public:
  SkelBranch();
  SkelBranch(std::list<Vector2> points,
	     ShapePointSet* ptr = 0, 
	     EndType firstEnd = UNDEFINED, 
	     EndType secondEnd = UNDEFINED, 
	     int begin = -1, 
	     int end = -1, 
	     int middle = -1);
  static std::list<Polyline2DPtr> getListPolylines(std::list<SkelBranchPtr> lb);
  double area();
};

class SkelJonction : public Vector2, public TOOLS::RefCountObject
{
  friend class Skeleton;
protected:
  bool m_exists;
  int m_nbBranches;
  SkelBranchPtr m_branches[3];

public:
  SkelJonction();
  SkelJonction(double x, double y);
  SkelJonction(Vector2& v);
  bool operator<(const SkelJonction& sj) const;
  bool equals(const Vector2& v) const;
  void addBranch(SkelBranchPtr branch);
};
typedef RCPtr<SkelJonction> SkelJonctionPtr;

class CompareSkelJonctionPtr
{
public:
  bool operator()(const SkelJonctionPtr jleft, const SkelJonctionPtr jright) const
  {
    return (*jleft < *jright);
  }
};

class SkelJonctionSet : public std::set<SkelJonctionPtr, CompareSkelJonctionPtr>
{
public:
  typedef std::set<SkelJonctionPtr, CompareSkelJonctionPtr>::iterator iterator;
};

class CompareShapePointWeakPtr
{
public:
  bool operator()(const ShapePointWeakPtr pleft, const ShapePointWeakPtr pright) const
  {
    //std::cerr << "class compareshapepoint"<< std::endl;
    if ((pleft == 0)||(pright == 0))
      return pleft == pright;
    return ((pleft->m_ind) < (pright->m_ind));
  }
};

class ShapePointSet : public std::set<ShapePointWeakPtr, CompareShapePointWeakPtr>
{
public:
  typedef std::set<ShapePointWeakPtr, CompareShapePointWeakPtr>::iterator iterator;

  ~ShapePointSet();
  iterator findVec(const Vector2& v) const;
  iterator findInd(const int indice) const;
};

/*----------------------------------------------------------------------- */


class Skeleton
{
  ShapePointSet m_shape;
  SkelEdgeSet m_allEdges;
  SkelTriangleSet m_allTriangles;
  SkelJonctionSet m_jonctions;
  std::list<Polyline2DPtr> m_triangulation;
  std::list<SkelBranchPtr> m_skeleton;
 public:
  Skeleton() {};
  Skeleton(Polyline2DPtr discretizedShape);

  //void filterLoops(CDTplus cdt);
  void init(Polyline2DPtr discretizedShape);
  TriangleSetPtr getTriangleSet();
  std::list<Polyline2DPtr> getChordalAxisTransform();
  void filterLittleBranchesOnBranchSize(double sizeMaxBranchesToRemove);
  void filterLittleBranchesOnBranchAreaSize(double areaMaxBranchesToRemove);
  void filterLittleBranchesOnAreaSize(double areaMaxTrianglesToRemove);

  static  Polyline2DPtr rediscretize(const Polyline2DPtr discretizedShape);
  static std::list<Polyline2DPtr> getChordalAxisTransform(const Polyline2DPtr discretizedShape, double areaMaxFilter);
  static TriangleSetPtr getDelaunayConstrained2DTriangulation(const Polyline2DPtr discretizedShape);

};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

#endif
