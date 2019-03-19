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



// #define GEOM_DEBUG

/* ----------------------------------------------------------------------- */
#include <string>
#include <list>
#include <set>
#include "../algo_config.h"


#include <plantgl/scenegraph/geometry/geometry.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/geometry/triangleset.h>

/* ----------------------------------------------------------------------- */

TOOLS_USING(Vector2)

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class SkelTriangle;
typedef RCPtr<SkelTriangle> SkelTrianglePtr;
typedef SkelTriangle* SkelTriangleWeakPtr;

//! A class that associate a vector2 and an index, to get the position of a point within a polyline
class ALGO_API ShapePoint : public RefCountObject
{
public:
  Vector2 m_vec;
  int m_ind;

  ShapePoint(Vector2 vec, int ind = -1) : m_vec(vec), m_ind(ind) {}
  ShapePoint(double x = 0, double y = 0, int ind = -1) : m_vec(Vector2(x,y)), m_ind(ind){}
  double x() const {return m_vec.x();}
  double y() const {return m_vec.y();}
  Vector2 operator()() {return m_vec;}

  bool operator<(const ShapePoint& sp) const
  {
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
    return m_vec == v;
  }
};

typedef RCPtr<ShapePoint> ShapePointPtr;
//typedef ShapePoint* ShapePointWeakPtr;

class Skeleton;
class SkelTriangleSet;
class SkelEdgeSet;
//! A class for an edge, defined by a pair of points
/**
 *  This is a class specific to Skeleton. It allows quick travel
 *  through adjacent triangles. Therefore we point to the two adjacent
 *  triangles, we have a marker to say if we came through this edge while
 *  trying to cover all triangles, a type to say if we have a boundary edge
 *  or not, and a boolean infinite for correspondance with CGAL infinite edges
 *  and faces.
 *  The pair of points used to define the edge are sorted within the class.
 */
class ALGO_API SkelEdge : public RefCountObject
{
public:
  friend class PGL(SkelTriangle);
  friend class PGL(Skeleton);
  friend class PGL(SkelEdgeSet);

  enum TypeEdge{UNDEFINED, BOUNDARY, INTERIOR};

protected:
  //! first point
  ShapePointPtr m_p1;
  //! second point, with first point < second point
  ShapePointPtr m_p2;
  //! type of edge : boundary, interior, or undefined
  TypeEdge m_type;
  //! marker for algorithms of covering
  bool m_visited;
  //! first adjacent triangle
  SkelTriangleWeakPtr m_adjTri1;
  //! second adjacent triangle
  SkelTriangleWeakPtr m_adjTri2;
  //! CGAL infinite edge indicator
  bool m_infinite;

public:
  //! basic constructor from 2 points
  SkelEdge(Vector2 p1,
       Vector2 p2);

  //! constructor from two ShapePoints
  SkelEdge(ShapePointPtr p1 ,
       ShapePointPtr p2 ,
       TypeEdge type = UNDEFINED,
       bool infinite = false,
       bool visited = false);

  //! constructor from two ShapePoints, with a boolean indicating if edge is boundary or interior
  SkelEdge(ShapePointPtr p1 ,
       ShapePointPtr p2 ,
       bool boundary ,
       bool infinite = false,
       bool visited = false);

  //! copy constructor
  SkelEdge(const SkelEdge& e);

  //! destructor
  ~SkelEdge();

  //! calculates the length
  /*!
   * \return length of the segment defined by the two points
   */
  double length() const;

  //! calculates the square length
  /*!
   * \return square length of the segment defined by the two points
   */
  double squareLength() const;

  //! add an adjacent triangle to the edge
  /*!
   * \param t pointer to the SkelTriangle to add
   */
  void addAdjTri(SkelTriangleWeakPtr t);

  //! removes triangle t from adjacent triangles
  /*!
   * \param t the triangle that is to be removed
   */
  void rmAdjTri(SkelTriangle& t);

  //! operator==
  /*!
   * \return length of the segment defined by the two points
   */
  bool operator==(const SkelEdge& e) const;

  //! calculates the middle of the segment
  /*!
   *  \return a Vector2 which is the middle of the edge : this is to say the middle of the segment p1-p2
   */
  Vector2 getMiddle() const;

  //!operator<
  bool operator<(const SkelEdge& e) const;
};

//! Clever pointer type to SkelEdge
typedef RCPtr<SkelEdge> SkelEdgePtr;

//! Class SkelTriangle that describes a triangle with 3 edges
/*!
 *  describing with 3 edges allows to cover the triangulation getting through the edges
 *  more easily, but as we do have redundant information, we have some constraints on the
 *  edges used to make the triangle. Only 3 points can form the 3 edges, and each point must
 *  be used twice. In the creation of the type Triangle, we sort the 3 edges, and we can get
 *  the 3 points of the triangle by getting p1 and p2 on the first edge, and p2 on the second
 *  edge. (the points are also sorted such as p1 < p2 in an edge) (e1 < e2 < e3 in the triangle)
 *  We also give some information on the type of the triangle (depending on how much edges are
 *  on the borders of the shape) and on the infinite type of the triangle (infinite triangles
 *  are on a CGAL triangulation those who are adjacent to the infinite vertex and therefore
 *  describe the space unoccupied by our shapes)
 */
class ALGO_API SkelTriangle : public RefCountObject
{
public:
  friend class PGL(SkelEdge);
  friend class PGL(Skeleton);
  friend class PGL(SkelTriangleSet);

  //! enum for giving a type to the triangle
  /*!
   *  Triangle can be of type SLEEVE if only one side of the triangle is boundary to the shape
   *  Triangle can be of type TERMINATION if two sides of the triangle are boundary to the shape
   *  Triangle can be of type JONCTION if no side of the triangle is boundary to the shape
   *  Triangle is UNDEFINED eitherway.
   */
  enum TypeTriangle{UNDEFINED, SLEEVE, TERMINATION, JONCTION};
protected:
  //! first edge
  SkelEdgePtr m_e1;
  //! second edge, with first edge < second edge
  SkelEdgePtr m_e2;
  //! third edge, with second edge < third edge
  SkelEdgePtr m_e3;
  //! type of triangle
  TypeTriangle m_type;
  //! triangle infinite or not
  bool m_infinite;

public:
  //! constructor
  /*!
   *  \param SkelEdgePtr e1 one edge/segment we want as a side of the triangle
   *  \param SkelEdgePtr e2 another edge/segment we want as a side of the triangle
   *  \param SkelEdgePtr e3 another edge/segment we want as a side of the triangle
   *  \param bool infinite whether we have an infinite face or not
   *  \return a SkelTriangle of type defined by the types of the given edges, with the three given
   *          edges sorted automatically
   */
  SkelTriangle(SkelEdgePtr e1 = 0, SkelEdgePtr e2 = 0, SkelEdgePtr e3 = 0, bool infinite = false);
  //! copy constructor
  SkelTriangle(const SkelTriangle& t);
  //! constructor
  /*!
   *  \param ShapePointPtr p1 one point to define the triangle
   *  \param ShapePointPtr p2 another point to define the triangle
   *  \param ShapePointPtr p3 another point to define the triangle
   *  \return a SkelTriangle finite, of UNDEFINED type, with three edges defined
   *          by the three given points to get a triangle
   */
  SkelTriangle(ShapePointPtr p1, ShapePointPtr p2, ShapePointPtr p3);
  //! destructor
  ~SkelTriangle();
  //! for TERMINATION Triangles only - calculates the index of the point defining the wing tip
  /*!
   *  This function must be called on TERMINATION SkelTriangles or it will return -1
   *  \return the index given in the ShapePoint of the point which is adjacent to two boundary edges
   */
  int getMiddleSummitForTerm();
  //! operator==
  bool operator==(const SkelTriangle& t) const;
  //! calculate the area of the triangle
  double area() const;
  //! operator<
  bool operator<(const SkelTriangle& t) const;
  //! calculate gravity center of the triangle
  Vector2 getGravityCenter () const;
  //! calculate the circumcenter of the triangle
  Vector2 getCircumCenter() const;
  //! check if the triangle is acute (i.e. hes 3 acute angles)
  bool isAcuteAngleTriangle() const;
  //! calculate pseudo-circumcenter
  /*!
   *  The pseudo circumcenter is the circumcenter itself if the triangle is acute
   *  Otherwise it is the middle of the biggest side
   *  \return the pseudocircumcenter
   */
  Vector2 getPseudoCircumCenter();
  //! calculate opposite point to SkelEdge e
  /*!
   *  \return the point opposite to SkelEdge e in the triangle, this is to say the point not in e in the triangle
   */
  ShapePointPtr getOppositePoint (const SkelEdge& e) const;

};

//! class to compare triangle pointers, but to compare what they point rather than adresses
class ALGO_API CompareTrianglePtr
{
public:
  bool operator()(const SkelTrianglePtr tleft, const SkelTrianglePtr tright) const
  {
    return (*tleft < *tright);
  }
};

//! class to compare edge pointers, but to compare what they point rather than adresses
class ALGO_API CompareSkelEdgePtr
{
public:
  bool operator()(const SkelEdgePtr eleft, const SkelEdgePtr eright) const
  {
    return (*eleft < *eright);
  }
};

//! class to compare Vector2
class ALGO_API CompareVector2
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

class ALGO_API SkelTriangleSet : public std::set<SkelTrianglePtr , CompareTrianglePtr>
{
public:
  typedef std::set<SkelTrianglePtr , CompareTrianglePtr>::iterator iterator;

  ~SkelTriangleSet();
  SkelTrianglePtr find_next();
  bool skelErase(SkelTrianglePtr toErase);
};

class ALGO_API SkelEdgeSet : public std::set<SkelEdgePtr, CompareSkelEdgePtr>
{
public:
  typedef std::set<SkelEdgePtr, CompareSkelEdgePtr>::iterator iterator;

  ~SkelEdgeSet();
  bool skelErase(SkelEdgePtr toErase);
};

class SkelBranch;
typedef RCPtr<SkelBranch> SkelBranchPtr;
class ShapePointSet;

class ALGO_API SkelBranch : public RefCountObject
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
  int m_joncjoncBegInd2;
  int m_joncjoncEndInd2;

public:
  SkelBranch();
  SkelBranch(std::list<Vector2> points,
         ShapePointSet* ptr = 0,
         EndType firstEnd = UNDEFINED,
         EndType secondEnd = UNDEFINED,
         int begin = -1,
         int end = -1,
         int middle = -1,
         int jjbeg2 = -1,
         int jjend2 = -1);
  static std::list<Polyline2DPtr> getListPolylines(std::list<SkelBranchPtr> lb);
  Polyline2DPtr getPolylineForAssociatedBump();
  double area();
};

class ALGO_API SkelJonction : public Vector2, public RefCountObject
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

class ALGO_API CompareSkelJonctionPtr
{
public:
  bool operator()(const SkelJonctionPtr jleft, const SkelJonctionPtr jright) const
  {
    return (*jleft < *jright);
  }
};

class ALGO_API SkelJonctionSet : public std::set<SkelJonctionPtr, CompareSkelJonctionPtr>
{
public:
  typedef std::set<SkelJonctionPtr, CompareSkelJonctionPtr>::iterator iterator;
};

class ALGO_API CompareShapePointPtr
{
public:
  bool operator()(const ShapePointPtr pleft, const ShapePointPtr pright) const
  {
    if ((pleft == 0)||(pright == 0))
      return pleft == pright;
    return ((pleft->m_ind) < (pright->m_ind));
  }
};

class ALGO_API ShapePointSet : public std::set<ShapePointPtr, CompareShapePointPtr>
{
public:
  typedef std::set<ShapePointPtr, CompareShapePointPtr>::iterator iterator;
  typedef std::set<ShapePointPtr, CompareShapePointPtr>::const_iterator const_iterator;

  ~ShapePointSet();
  const_iterator findVec(const Vector2& v) const;
  const_iterator findInd(const int indice) const;
};

/*----------------------------------------------------------------------- */


class ALGO_API Skeleton
{
 protected:
  ShapePointSet m_shape;
  SkelEdgeSet m_allEdges;
  SkelTriangleSet m_allTriangles;
  SkelJonctionSet m_jonctions;
  std::list<Polyline2DPtr> m_triangulation;
  std::list<SkelBranchPtr> m_skeleton;

 public:
  Skeleton() {};
  Skeleton(Polyline2DPtr discretizedShape);

 protected:
  void init(const Polyline2DPtr discretizedShape, const Vector2& infinite_vertex);
  TriangleSetPtr getTriangleSet();
  std::list<Polyline2DPtr> getChordalAxisTransform();
  void filterLittleBranchesOnBranchSize(const double sizeMaxBranchesToRemove);
  void filterLittleBranchesOnBranchAreaSize(const double areaMaxBranchesToRemove);
  void filterLittleBranchesOnAreaSize(const double areaMaxTrianglesToRemove);
  void reorganizeBranchesAndJonctionsAfterFiltering(SkelBranchPtr branches[3], int indexBranchToRemove);

 public:

  static Polyline2DPtr removeLoopsInShape(Polyline2DPtr shape);

  static std::list<Polyline2DPtr> getChordalAxisTransform(const Polyline2DPtr discretizedShape,
                              const double areaMaxFilter);

  static std::list<Polyline2DPtr> getSkeletonInformation(const Polyline2DPtr discretizedShape,
                             const double areaMaxFilter,
                             std::list<Vector2> * ends,
                             std::list<Vector2> * end_tgts,
                             std::list<Vector2> * bump_ends,
                             std::list<Vector2> * bump_tgts,
                             std::list<Polyline2DPtr> * bumps);

  static TriangleSetPtr getDelaunayConstrained2DTriangulation(const Polyline2DPtr discretizedShape);

};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

#endif
