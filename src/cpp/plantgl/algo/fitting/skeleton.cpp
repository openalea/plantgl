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

#include "skeleton.h"

#include <math.h>

#include <cassert>

#include <iostream>
#include <string>
#include <stack>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

#ifdef WITH_CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/intersections.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Constrained_triangulation_plus_2.h>
#include <CGAL/basic.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_overlay.h>
#include <CGAL/Arr_default_overlay_traits.h>

#ifdef CGAL_USE_GMP
// GMP is installed. Use the GMP rational number-type.
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq                                    Number_type;
#else
// GMP is not installed. Use CGAL's exact rational number-type.
#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>

#endif

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_2<K>              Vb;
typedef CGAL::Constrained_triangulation_face_base_2<K>    Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>       TDS;
typedef CGAL::Exact_intersections_tag                     Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<K,TDS,Itag> CDT;
typedef CGAL::Constrained_triangulation_plus_2<CDT>       CDTplus;
typedef CDTplus::Point                                    Point_2;
typedef CDTplus::Segment                                  Segment_2;
typedef TDS::Vertex_handle                                V_handle;

Vector2 toVec2(const Point_2& p) { return Vector2(to_double(p.x()),to_double(p.y())); }
Point_2 toPoint2(const Vector2 &v) {  return Point_2(v.x(),v.y());}

#endif

class Debug
{
public:

  static inline void debug(std::string msg, int i = 1)
  {
    int N = 3;
    if (i < N)
      std::cout << msg << std::endl;
  }

  static inline void debug(std::string msg, double valueToCheck, int i = 1)
  {
    int N = 3;
    if (i < N)
      std::cout << msg << " " << valueToCheck << std::endl;
  }

  static inline void debug(std::string msg, int valueToCheck, int i = 1)
  {
    int N = 3;
    if (i < N)
      std::cout << msg << " " << valueToCheck << std::endl;
  }

  static inline void debug(std::string msg, void* pointerToCheck, int i = 1)
  {
    int N = 3;
    if (i < N)
      std::cout << msg << " " << pointerToCheck << std::endl;
  }
};

ShapePointSet::const_iterator ShapePointSet::findVec(const Vector2& v) const
{
  ShapePointSet::const_iterator it;
  for(it = begin(); it != end(); it++)
    {
      if (*(*it) == v)
	{
	  return it;
	}
    }
  return end();
}

ShapePointSet::const_iterator ShapePointSet::findInd(const int indice) const
{
  ShapePointWeakPtr ptr = new ShapePoint(0,0,indice);
  ShapePointSet::const_iterator it = find(ptr);
  delete ptr;
  return it; 
}

ShapePointSet::~ShapePointSet()
{
  ShapePointSet::iterator it = begin();
  while(it!= end())
    {
      delete *it;
      it++;
    }
}

SkelEdge::SkelEdge(Vector2 p1,
		   Vector2 p2)
  :m_type(SkelEdge::UNDEFINED),
   m_visited(true),
   m_infinite(false)
{
  if (p1 < p2)
    {
      m_p1 = new ShapePoint(p1);
      m_p2 = new ShapePoint(p2);
    }
  else
    {
      m_p1 = new ShapePoint(p2);
      m_p2 = new ShapePoint(p1);
    }
}

SkelEdge::SkelEdge(ShapePointWeakPtr p1 ,ShapePointWeakPtr p2 , TypeEdge type , bool infinite, bool visited )
  :m_type(type),
   m_visited(visited),
   m_infinite(infinite)
{
  if ((p1 == 0) || (p2 == 0))
    {
      std::cerr << "Invalid parameters for SkelEdge constructor : cannot build an edge with null pointers" << std::endl;
      return;
    }
  if (*p1 < *p2)
    {
      m_p1 = p1;
      m_p2 = p2;
    }
  else
    {
      m_p1 = p2;
      m_p2 = p1;
    }
  m_adjTri1 = 0;
  m_adjTri2 = 0;
}

SkelEdge::SkelEdge(const SkelEdge& e):m_p1(e.m_p1),
				      m_p2(e.m_p2),
				      m_type(e.m_type),
				      m_visited(e.m_visited),
				      m_adjTri1(e.m_adjTri1),
				      m_adjTri2(e.m_adjTri2),
				      m_infinite(e.m_infinite)
{
}

SkelEdge::SkelEdge(ShapePointWeakPtr p1 ,
		   ShapePointWeakPtr p2 , 
		   bool boundary , 
		   bool infinite,
		   bool visited )
  :m_visited(visited),
   m_infinite(infinite)
{
  if ((p1 == 0) || (p2 == 0))
    {
      std::cerr << "Invalid parameters for SkelEdge constructor : cannot build an edge with null pointers" << std::endl;
      return;
    }
  if (*p1 < *p2)
    {
      m_p1 = p1;
      m_p2 = p2;
    }
  else
    {
      m_p1 = p2;
      m_p2 = p1;
    }
  m_adjTri1 = 0;
  m_adjTri2 = 0;
  if (boundary)
    m_type = BOUNDARY;
  else
    m_type = INTERIOR;
}

SkelEdge::~SkelEdge()
{
}

double SkelEdge::length() const
{
  if ((m_p2 == 0)||(m_p1 == 0))
    return -1.0;
  Vector2 e = (*m_p2)() - (*m_p1)();
  return sqrt(e.x()*e.x()+e.y()*e.y());
}

double SkelEdge::squareLength() const
{
  if ((m_p2 == 0)||(m_p1 == 0))
    return -1.0;
  Vector2 e = (*m_p2)() - (*m_p1)();
  return (e.x()*e.x()+e.y()*e.y());
}

void SkelEdge::addAdjTri(SkelTriangleWeakPtr t)
{
  if (t==0)
    {
      std::cerr << "param is a null pointer" << std::endl;
      return;
    }
  if (m_adjTri1 == 0)
    {
      m_adjTri1 = t;
      return;
    }
  if ((m_adjTri2 == 0) && !(*t == *m_adjTri1))
    {
      m_adjTri2 = t;
    }
}


bool SkelEdge::operator==(const SkelEdge& e) const 
{
  if ((e.m_p1 == 0)||(e.m_p2 == 0)||(m_p1 == 0)||(m_p2 == 0))
    {
      return ((((e.m_p1) == m_p1)&&((e.m_p2) == m_p2))&&(m_type == e.m_type));
    }
  bool b = (((*(e.m_p1) == *m_p1)&&(*(e.m_p2) == *m_p2)));
  return b;
}

Vector2 SkelEdge::getMiddle() const
{
  if ((m_p1 == 0) || (m_p2 == 0))
    {
      std::cerr << "SkelEdge, getMiddle : uninitialized edge" << std::endl;
      return Vector2();
    }
  return 0.5*((*m_p1)()+(*m_p2)());
}

void SkelEdge::rmAdjTri(SkelTriangle& t)
{
  if (m_adjTri1 != 0)
    {
      if (*m_adjTri1 == t)
	{
	  m_adjTri1 = 0;
	}
    }
  if (m_adjTri2 != 0)
    {
      if (*m_adjTri2 == t)
	{
	  m_adjTri2 = 0;
	}
    }
}

bool SkelEdge::operator<(const SkelEdge& e) const
{
  if (*this == e)
    return false;
  
  if (*m_p1 == *e.m_p1)
    {
      if (*m_p2 == *e.m_p2)
	{
	  if (m_type == e.m_type)
	    return false;
	  else
	    return m_type < e.m_type;
	}
      else
	return *m_p2 < *e.m_p2;
    }
  else
    return *m_p1 < *e.m_p1;
}

SkelTriangle::SkelTriangle(SkelEdgePtr e1 , SkelEdgePtr e2 , SkelEdgePtr e3, bool infinite)
  :m_e1(e1),
   m_e2(e2),
   m_e3(e3),
   m_infinite(infinite)
{
  if ((e1 == 0) || (e2 == 0) || (e3 == 0))
    {
      std::cerr << "one edge is null !!!!!!!!" << std::endl;
      m_type = UNDEFINED;
    }
  else
    {
      if (*m_e2 < *m_e1)
	{
	  SkelEdgePtr temp = m_e1;
	  m_e1 = m_e2;
	  m_e2 = temp;
	}
      // *m_e1 <= *m_e2
      if (*m_e3 < *m_e2)
	{
	  SkelEdgePtr temp = m_e2;
	  m_e2 = m_e3;
	  m_e3 = temp;
	}
      // maintenant *m_e3 > *m_e2 et *m_e3 > *m_e1
      if (*m_e2 < *m_e1)
	{
	  SkelEdgePtr temp = m_e1;
	  m_e1 = m_e2;
	  m_e2 = temp;
	}
      // *m_e1 < *m_e2 < *m_e3
      if (!(*(m_e1->m_p1) == *(m_e2->m_p1)))
	{
	  std::cerr << "SkelTriangle, constructor : invalid configuration 1" << std::endl;
	  return;
	}
      if (!(*(m_e1->m_p2) == *(m_e3->m_p1)))
	{
	  std::cerr << "SkelTriangle, constructor : invalid configuration 2" << std::endl;
	  return;
	}
      if (!(*(m_e2->m_p2) ==*(m_e3->m_p2)))
	{
	  std::cerr << "SkelTriangle, constructor : invalid configuration 3" << std::endl;
	  return;
	}

      int nbBoundaryEdges = 0;
      if (m_e1->m_type == SkelEdge::UNDEFINED || m_e2->m_type == SkelEdge::UNDEFINED || m_e3->m_type == SkelEdge::UNDEFINED)
	{
	  std::cerr << "one edge is null !!!!!!!!" << std::endl;
	  m_type = UNDEFINED;
	}
      else
	{
	  if (m_e1->m_type == SkelEdge::BOUNDARY)
	    {
	      nbBoundaryEdges++;
	    }
	  if (m_e2->m_type == SkelEdge::BOUNDARY)
	    {
	      nbBoundaryEdges++;
	    }
	  if (m_e3->m_type == SkelEdge::BOUNDARY)
	    {
	      nbBoundaryEdges++;
	    }
	  switch(nbBoundaryEdges)
	    {
	    case 0:
	      m_type = JONCTION;
	      break;
	    case 1:
	      m_type = SLEEVE;
	      break;
	    case 2:
	      m_type = TERMINATION;
	      break;
	    default:
	      std::cerr << "one edge is null !!!!!!!!" << std::endl;
	      m_type = UNDEFINED;
	      break;
	    }
	}
    }
}

SkelTriangle::SkelTriangle(ShapePointWeakPtr p1, ShapePointWeakPtr p2, ShapePointWeakPtr p3)
  :m_type(SkelTriangle::UNDEFINED),
   m_infinite(false)
{
  ShapePointWeakPtr p = 0;
  if (*p2 < *p1)
    {
      p = p1;
      p1 = p2;
      p2 = p;
    } 
  if (*p3 < *p2)
    {
      p = p3;
      p3 = p2;
      p2 = p;
    } 
  if (*p2 < *p1)
    {
      p = p1;
      p1 = p2;
      p2 = p;
    } 
  m_e1 = new SkelEdge(p1,p2);
  m_e2 = new SkelEdge(p1,p3);
  m_e3 = new SkelEdge(p2,p3);
}

SkelTriangle::SkelTriangle(const SkelTriangle& t):m_e1(t.m_e1),
						  m_e2(t.m_e2),
						  m_e3(t.m_e3),
						  m_type(t.m_type),
						  m_infinite(t.m_infinite)
{}

int SkelTriangle::getMiddleSummitForTerm()
{
  if (m_type != TERMINATION)
    return -1;
  if (m_e1->m_type == SkelEdge::BOUNDARY)
    {
      if (m_e2->m_type == SkelEdge::BOUNDARY)
	{
	  return m_e1->m_p1->m_ind;
	}
      else
	{
	  return m_e1->m_p2->m_ind;
	}
    }
  else if (m_e2->m_type == SkelEdge::BOUNDARY)
    {
      if (m_e3->m_type == SkelEdge::BOUNDARY)
	{
	  return m_e2->m_p2->m_ind;
	}
      else
	{
	  pglError("SkelTriangle, getMiddleSummitForTerm(), problems with types");
	  return -1;
	}
    }
}

bool SkelTriangle::operator==(const SkelTriangle& t) const
{
  if ((m_e1 == 0)||(m_e2 == 0)||(m_e3 == 0))
    {
      std::cerr << "SkelTriangle, operator==, problem : deprecated or uninitialized triangle" << std::endl;
      return false;
    }
  if ((*m_e1 == *(t.m_e1)) && (*m_e2 == *(t.m_e2)))
    return true;
  else
    return false;
}

double SkelTriangle::area() const
{
  if ((m_e1 == 0)||(m_e2 == 0)||(m_e3 == 0))
    {
      std::cerr << "SkelTriangle, area : uninitialized or deprecated triangle" << std::endl;
      return 0.0;
    }

  // infinite face
  if ((m_e1->m_p1 == 0)||(m_e1->m_p2 == 0)||(m_e2->m_p1 == 0)||(m_e2->m_p2 == 0))
    {
      std::cerr << "SkelTriangle, area : edges have null shapepoint" << std::endl;
      return -1.0;
    }

  if ((m_e1->squareLength() == 0) || (m_e2->squareLength() == 0) || (m_e3->squareLength() == 0))
    return 0.0;

  Vector2 side1 = m_e1->m_p2->m_vec - m_e1->m_p1->m_vec;
  Vector2 side2 = (*(m_e2->m_p2))() - (*(m_e2->m_p1))();

  if ((side1.x()*side2.y()-side1.y()*side2.x()) == 0)
    return 0.0;

  double toTest = (m_e1->squareLength()*m_e2->squareLength())
    -(side1.x()*side2.x()+side1.y()*side2.y())*(side1.x()*side2.x()+side1.y()*side2.y());

  if (toTest < 0)
    {
      std::cerr << "pb : we're gonna calculate a negative nb sqrt" << std::endl;
    }
  double area = (0.5*sqrt(toTest));

  if (area == 0)
    {
      std::cerr << "side1 : (" << side1.x() << ", " << side1.y() << ")" << std::endl;
      std::cerr << "side2 : (" << side2.x() << ", " << side2.y() << ")" << std::endl;
    }
  return area;
}

bool SkelTriangle::operator<(const SkelTriangle& t) const
{
  if (*this == t)
    {
      return false;
    }
  else
    { 
      if ((m_e1->m_p1 == 0)||(m_e1->m_p2 == 0)||(m_e2->m_p1 == 0)||(m_e2->m_p2 == 0))
	{
	  std::cerr << "SkelTriangle, operator < : uninitialized or deprecated triangle" << std::endl;
	  return true;
	}
      if (*m_e1 == *t.m_e1)
	return (*m_e2 < *(t.m_e2));
      else
	return (*m_e1 < *(t.m_e1));
    }
}



Vector2 SkelTriangle::getGravityCenter () const
{
  if ((m_e1 == 0)||(m_e2 == 0)||(m_e3 == 0))
    {
      std::cerr << "SkelTriangle, getGravityCenter, problem : deprecated or uninitialized triangle" << std::endl;
      return Vector2();
    }
  Vector2 p1, p2, p3;
  p1 = (*(m_e1->m_p1))();
  p2 = (*(m_e1->m_p2))();
  if (*(m_e2->m_p1) == p1 || *(m_e2->m_p1) == p2)
    p3 = (*(m_e2->m_p2))();
  else
    p3 = (*(m_e2->m_p1))();    
  return 1./3.*(p1+p2+p3);
}

Vector2 SkelTriangle::getCircumCenter() const
{
  if ((m_e1 == 0)||(m_e2 == 0)||(m_e3 == 0))
    {
      std::cerr << "SkelTriangle, getCircumCenter, problem : deprecated or uninitialized triangle" << std::endl;
      return Vector2();
    }
  // the circumcenter is at the intersection of the orthogonal bissectors
 
  Vector2 side1, side2, commonPoint;
  if (m_e1->m_p1 == m_e2->m_p1)
    {
      side1 = (*(m_e1->m_p2))() - (*(m_e1->m_p1))();
      side2 = (*(m_e2->m_p2))() - (*(m_e2->m_p1))();
      commonPoint = (*(m_e1->m_p1))();
    }
  else if (m_e1->m_p1 == m_e2->m_p2)
    {
      side1 = (*(m_e1->m_p2))() - (*(m_e1->m_p1))();
      side2 = (*(m_e2->m_p1))() - (*(m_e2->m_p2))();
      commonPoint = m_e1->m_p1->m_vec;
    }
  else if (m_e1->m_p2 == m_e2->m_p1)
    {
      side1 = (*(m_e1->m_p1))() - (*(m_e1->m_p2))();
      side2 = (*(m_e2->m_p2))() - (*(m_e2->m_p1))();
      commonPoint = (*(m_e1->m_p2))();
    }
  else
    {
      side1 = (*(m_e1->m_p1))() - (*(m_e1->m_p2))();
      side2 = (*(m_e2->m_p1))() - (*(m_e2->m_p2))();
      commonPoint = (*(m_e1->m_p2))();
    }

  double det = side1.x()*side2.y() - side1.y()*side2.x();
  det*=2;

  double normSide1 = side1.x()*side1.x() + side1.y()*side1.y();
  double normSide2 = side2.x()*side2.x() + side2.y()*side2.y();

  Vector2 res = Vector2((-1.0/det)*(normSide2*side1.y() - normSide1*side2.y()),
			(-1.0/det)*(normSide1*side2.x() - normSide2*side1.x()));

  res = res + commonPoint;
  return res;
}

bool SkelTriangle::isAcuteAngleTriangle() const
{
  if ((m_e1 == 0)||(m_e2 == 0)||(m_e3 == 0))
    {
      std::cerr << "SkelTriangle, isAcuteTriangle, problem : deprecated or uninitialized triangle" << std::endl;
      return false;
    }
  double lengthBiggerSide, length1, length2;
  if ((m_e1->m_p1 == 0)||(m_e1->m_p2 == 0)||(m_e2->m_p1 == 0)||(m_e2->m_p2 == 0))
    {
      std::cerr << "SkelTriangle, isAcuteTriangle, problem : deprecated or uninitialized triangle" << std::endl;
      return false;
    }
  Vector2 side1 = (*(m_e1->m_p1))() - (*(m_e1->m_p2))();
  Vector2 side2 = (*(m_e2->m_p1))() - (*(m_e2->m_p2))();
  Vector2 side3 = (*(m_e3->m_p1))() - (*(m_e3->m_p2))();

  length1 = sqrt((side1.x())*(side1.x())+(side1.y())*(side1.y()));
  length2 = sqrt((side2.x())*(side2.x())+(side2.y())*(side2.y()));
  lengthBiggerSide = sqrt((side3.x())*(side3.x())+(side3.y())*(side3.y()));

  if (length1 > lengthBiggerSide)
    {
      double temp = length1;
      length1 = lengthBiggerSide;
      lengthBiggerSide = temp;
    }

  if (length2 > lengthBiggerSide)
    {
      double temp = length2;
      length2 = lengthBiggerSide;
      lengthBiggerSide = temp;
    }
  double cosBiggerSide = (length1*length1+length2*length2 - lengthBiggerSide*lengthBiggerSide); ///(2*length1*length2);
  return cosBiggerSide > 0;
}

Vector2 SkelTriangle::getPseudoCircumCenter()
{
  if (area() == 0)
    {
      std::cerr << "Skeleton, SkelTriangle, getPseudoCircumCenter : deprecated or uninitialized triangle" << std::endl;
      return Vector2();
    }
  if (isAcuteAngleTriangle())
    {
      return getCircumCenter();
    }
  if (m_e1->length() >= m_e2->length())
    {
      if (m_e1->length() >= m_e3->length())
	return m_e1->getMiddle();
      else
	return m_e3->getMiddle();
    }
  else
    {
      if (m_e2->length() >= m_e3->length())
	return m_e2->getMiddle();
      else
	return m_e3->getMiddle();
    }
}

ShapePointWeakPtr SkelTriangle::getOppositePoint (const SkelEdge& e) const
{
  if ((m_e1 == 0)||(m_e2 == 0)||(m_e3 == 0))
    {
      std::cerr << "SkelTriangle, getOppositePoint, problem : deprecated or uninitialized triangle" << std::endl;
      return 0;
    }
  if (*m_e1 == e)
    {
      if (((*m_e2).m_p1 == e.m_p1) || ((*m_e2).m_p1 == e.m_p2))
	return (*m_e2).m_p2;
      else
	return (*m_e2).m_p1;
    }
  else
    {
      if (((*m_e1).m_p1 == e.m_p1) || ((*m_e1).m_p1 == e.m_p2))
	return (*m_e1).m_p2;
      else
	return (*m_e1).m_p1;
    }
}

SkelTriangle::~SkelTriangle()
{
}


bool SkelEdgeSet::skelErase(SkelEdgePtr toErase)
{
  iterator it;
  it = find(toErase);
  if (it != end())
    {
      erase(it);
	  return true;
    }
  return false;
}

SkelEdgeSet::~SkelEdgeSet()
{
  iterator clear_edges_it;
  for (clear_edges_it = begin(); clear_edges_it != end(); clear_edges_it++)
    {
      erase(clear_edges_it);
    }
  clear();
}

bool SkelTriangleSet::skelErase(SkelTriangleWeakPtr toErase)
{
  iterator it;
  it = find(toErase);
  if (it != end())
    {
      (*it)->m_e1->rmAdjTri(*(*it));
      (*it)->m_e2->rmAdjTri(*(*it));
      (*it)->m_e3->rmAdjTri(*(*it));
      delete (*it);
      erase(it);
	  return true;
    }
  return false;
}

SkelTriangleSet::~SkelTriangleSet()
{
  iterator it;
  for (it = begin(); it != end(); it++)
    {
      erase(it);
    }
  clear();
}

SkelTriangleWeakPtr SkelTriangleSet::find_next()
{
  // try to get the first jonction
  // in default of a jonction, try to get a termination
  // in default return any sleeve
  if (empty())
    {
      return 0;
    }
  else
    {
      iterator tri_it = begin();
      SkelTriangleWeakPtr current_tri = 0;
      bool join_found = false;
      while (tri_it != end() && !join_found)
	{
	  join_found = ((*tri_it)->m_type == SkelTriangle::JONCTION);
	  if (join_found)
	    {
	      current_tri = *tri_it;
	      return current_tri;
	    }
	  else
	    {
	      if (((*tri_it)->m_type == SkelTriangle::TERMINATION) && (current_tri == 0))
		{
		  current_tri = *tri_it;
		}
	    }
	  tri_it ++;
	}
      if (current_tri == 0) 
	current_tri = *(begin());
      if (current_tri == 0)
	std::cerr << "SkelTriangleSet, find_next : Interesting ! null pointers in allTriangles !" << std::endl;
      return current_tri;
    }
}

SkelBranch::SkelBranch()
{

}

SkelBranch::SkelBranch(std::list<Vector2> points, 
		       ShapePointSet * ptr,
		       EndType firstEnd, 
		       EndType secondEnd,
		       int begin,
		       int end, 
		       int middle)
  :m_firstEnd(firstEnd), 
   m_secondEnd(secondEnd),
   m_pointSet(ptr),
   m_beginBumpIndice(begin),
   m_endBumpIndice(end),
   m_middleBumpIndice(middle)  
{
  m_branch = new Polyline2D(new Point2Array(points.begin(), points.end()));
  if ((m_beginBumpIndice < m_middleBumpIndice) && (m_endBumpIndice < m_middleBumpIndice))
    {
      if (m_beginBumpIndice < m_endBumpIndice)
	{
	  int temp = m_beginBumpIndice;
	  m_beginBumpIndice = m_endBumpIndice;
	  m_endBumpIndice = temp;
	}
      return;
    }
  if ((m_beginBumpIndice > m_middleBumpIndice) && (m_endBumpIndice < m_middleBumpIndice))
    {
	  int temp = m_beginBumpIndice;
	  m_beginBumpIndice = m_endBumpIndice;
	  m_endBumpIndice = temp;
    }
}

double SkelBranch::area()
{
  ShapePointSet::const_iterator it1, it2, it3;
  it1 = m_pointSet->findInd(m_beginBumpIndice); 
  it2 = m_pointSet->findInd(m_endBumpIndice); 
  if ((it1 == m_pointSet->end())||(it2 == m_pointSet->end()))
    return 0.0;
  it3 = it1;
  it3++;
  if (it3 == m_pointSet->end())
    it3 = m_pointSet->begin();
  double area = 0.0;
  int i = 0;
  while ((it1 != it2) && (it1 != it3) && (it2 != it3))
    {
      SkelTriangle st = SkelTriangle((*it1),(*it2),(*it3));
      area += st.area();
      i++;
      if (i%2 == 0)
	{
	  it2 = it3;
	  it3 = it1;
	  it3++;
	  if (it3 == m_pointSet->end())
	    it3 = m_pointSet->begin();
	}
      else
	{
	  it1 = it3;
	  it3 = it2;
	  it3--;
	  if (it3 == m_pointSet->end())
	    it3--;
	}
    }
  return area;
}

std::list<Polyline2DPtr> SkelBranch::getListPolylines(std::list<SkelBranchPtr> lb)
{
  std::list<Polyline2DPtr> res;
  std::list<SkelBranchPtr>::iterator it;
  for(it = lb.begin(); it != lb.end(); it++)
    {
      res.push_back((*it)->m_branch);
    }
  return res;
}

SkelJonction::SkelJonction() : m_exists(false), m_nbBranches(0)
{
  for (int i = 0; i < 3; i++)
    m_branches[i] = 0;
}

SkelJonction::SkelJonction(Vector2& v) : Vector2(v), m_exists(true), m_nbBranches(0)
{
  for (int i = 0; i < 3; i++)
    m_branches[i] = 0;
}

SkelJonction::SkelJonction(double x, double y):Vector2(x,y), m_exists(true), m_nbBranches(0)
{
  for (int i = 0; i < 3; i++)
    m_branches[i] = 0;
}

bool SkelJonction::equals(const Vector2& v) const
{
  return ((x() == v.x()) && (y() == v.y()));
}

bool SkelJonction::operator<(const SkelJonction& sj) const
{
  if (x() == sj.x())
    {
      if (y() == sj.y())
	return false;
      else 
	return y() < sj.y();
    }
  else 
    return x() < sj.x();
}

void SkelJonction::addBranch(SkelBranchPtr branch)
{
  if (m_branches[m_nbBranches] == 0)
    m_branches[m_nbBranches] = branch;
  if (m_nbBranches < 3)
    m_nbBranches ++;
}

PGL::Skeleton::Skeleton(const Polyline2DPtr discretizedShape)
{
  for (int i = 0; i < discretizedShape->getPointListSize(); ++i)
    {
      Vector2 v = discretizedShape->getPointListAt(i);
      ShapePointSet::const_iterator it = m_shape.findVec(v);
      if (it == m_shape.end())
		m_shape.insert(new ShapePoint(v,i));
      else
	Debug::debug("ALREADY INSERTED POINT : continue...",10,10);
    }
  init(discretizedShape);
}

#ifdef WITH_CGAL
CDTplus getCGALTriangulation(const Polyline2DPtr discretizedShape)
{
  CDTplus cdt;
  cdt.clear();
  V_handle va = cdt.insert(toPoint2(discretizedShape->getPointListAt(0)));
  V_handle vfirst = va;
  for (int i = 1; i < discretizedShape->getPointListSize(); ++i)
    {
      V_handle vb = cdt.insert(toPoint2(discretizedShape->getPointListAt(i)));
      cdt.insert_constraint(va, vb);
      va = vb;
    }
  if (toPoint2(discretizedShape->getPointListAt(0)) 
      != toPoint2(discretizedShape->getPointListAt(discretizedShape->getPointListSize()-1)))
    {
      cdt.insert_constraint(vfirst,va);
    }
  assert(cdt.is_valid());    
  return cdt;
}

std::pair<V_handle,V_handle> getCGALVerticesDefinedEdge(CDT::Edge ed)
{
  std::pair<V_handle,V_handle> res;
  TDS::Face_handle face_handle = ed.first;
  int edgeIndexInFace = ed.second;
  res.first = face_handle->vertex((edgeIndexInFace+1)%3);
  res.second = face_handle->vertex((edgeIndexInFace+2)%3);
  return res;
}

V_handle getEdgeTarget(TDS::Edge ed, V_handle src)
{
  std::pair<V_handle, V_handle> edge = getCGALVerticesDefinedEdge(ed);
  if ((edge.first)->point() == src->point())
    return edge.second;
  else if ((edge.second)->point() == src->point())
    return edge.first;
  else
    {
      pglError("Skeleton.cpp getEdgeTarget : problem, the second parameter should be one point of the edge");
    } 
}

void filterLoopsInShape(CDTplus cdt, ShapePointSet& blabla)
{

}
#endif

void PGL::Skeleton::init(const Polyline2DPtr discretizedShape)
{
#ifndef WITH_CGAL
      pglError("Skeleton::init : Cannot compute skeleton without CGAL!");
      return;
#else
  if(discretizedShape->getPointListSize() < 3)
    {
      pglError("Skeleton::init : Are you kidding me ? Give me more than 3 points to process, because I feel that you insult my intelligence !");
      return;
    }
  CDTplus cdt = getCGALTriangulation(discretizedShape); 

  std::cerr << "nb de vertex introduits : " << m_shape.size() << std::endl;
  std::cerr << "nb de vertex après triangulation : " << cdt.number_of_vertices() << std::endl;


//   if (m_shape.size() != cdt.number_of_vertices())
//     return;

  // We get back the result of the triangulation
  std::stack<SkelTriangleWeakPtr> outOfTheShapeTriangles;
  CDT::Finite_vertices_iterator it;
  ShapePointWeakPtr infiniteVertexPtr = new ShapePoint(toVec2((cdt.infinite_vertex())->point()));
  for (it = cdt.finite_vertices_begin(); it != cdt.finite_vertices_end() ; ++it)
    {
      CDT::Edge_circulator ec = cdt.incident_edges(it);
      CDT::Edge_circulator done = ec;
      if (ec != 0)
	{
	  SkelEdgePtr prev_ed, cur_ed;
	  do
	    {
	      ShapePointWeakPtr shPtr1, shPtr2;
	      ShapePointSet::const_iterator point_it = m_shape.findVec(toVec2((cdt.segment(ec)).target()));
	      if (point_it == m_shape.end())
		{
		  shPtr1 = infiniteVertexPtr;
		  if ((cdt.segment(ec)).target() != ((cdt.infinite_vertex())->point()))
		    {
		      Vector2 vvv = toVec2((cdt.segment(ec)).target());
		      shPtr1 = new ShapePoint(vvv, m_shape.size());
		      m_shape.insert(shPtr1);
		    }
		}
	      else
		{
		  shPtr1 = *point_it;
		}
	      
	      point_it = m_shape.findVec(toVec2((cdt.segment(ec)).source()));
	      if (point_it == m_shape.end())
		{
		  shPtr2 = infiniteVertexPtr;
		  if ((cdt.segment(ec)).source() != ((cdt.infinite_vertex())->point()))
		    {
		      Vector2 vvv = toVec2((cdt.segment(ec)).source());
		      shPtr2 = new ShapePoint(vvv, m_shape.size());
		      m_shape.insert(shPtr2);
		    }		
		}
	      else
		{
		  shPtr2 = *point_it;
		}
	      cur_ed = new SkelEdge(shPtr1,
				    shPtr2,
				    cdt.is_constrained(*ec),
				    cdt.is_infinite(ec));

	      V_handle opp_p2_vhandle = getEdgeTarget(*ec,it);
	      Point_2 opp_p2 = opp_p2_vhandle->point();

	      CDT::Edge_circulator ec_p = ec;
	      while (++ec_p != done)
		{
		  point_it = m_shape.findVec(toVec2((cdt.segment(ec_p)).target()));
		  if (point_it == m_shape.end())
		    {
		      shPtr1 = infiniteVertexPtr; // infinite vertex
		      if ((cdt.segment(ec_p)).target() != ((cdt.infinite_vertex())->point()))
			{
 			  Vector2 vvv = toVec2((cdt.segment(ec_p)).target());
			  shPtr1 = new ShapePoint(vvv, m_shape.size());
			  m_shape.insert(shPtr1);
			}
		    }
		  else
		    {
		      shPtr1 = *point_it;
		    }
		  
		  point_it = m_shape.findVec(toVec2((cdt.segment(ec_p)).source()));
		  if (point_it == m_shape.end())
		    {
		      shPtr2 = infiniteVertexPtr;
		      if ((cdt.segment(ec_p)).source() != ((cdt.infinite_vertex())->point()))
			{
			  Vector2 vvv = toVec2((cdt.segment(ec_p)).source());
			  shPtr2 = new ShapePoint(vvv, m_shape.size());
			  m_shape.insert(shPtr2);
			}
		    }
		  else
		    {
		      shPtr2 = *point_it;
		    }

		  prev_ed = new SkelEdge(shPtr1,
					 shPtr2,
					 cdt.is_constrained(*ec_p),
					 cdt.is_infinite(ec_p));
		  
		  V_handle opp_p1_vhandle = getEdgeTarget(*ec_p,it);
		  Point_2 opp_p1 = opp_p1_vhandle->point();
		  
		  TDS::Face_handle face_handle3;
		  if (cdt.is_face(opp_p1_vhandle, opp_p2_vhandle, it, face_handle3))
		    {
		      // so a triangle does exist with 
		      // edges prev_ed, cur_ed, and SkelEdge(opp_p1,opp_p2)
		      TDS::Face_handle fh_ed;
		      int i_ed;
		      if (cdt.is_edge(opp_p1_vhandle, opp_p2_vhandle, fh_ed, i_ed))
			{
			  TDS::Edge ec_opp2 ;
			  ec_opp2.first = fh_ed;
			  ec_opp2.second = i_ed;
			  point_it = m_shape.findVec(toVec2((cdt.segment(ec_opp2)).target()));
			  if (point_it == m_shape.end())
			    {
			      shPtr1 = infiniteVertexPtr; // infinite vertex
			      if ((cdt.segment(ec_opp2)).target() != ((cdt.infinite_vertex())->point()))
				{
				  Vector2 vvv = toVec2((cdt.segment(ec_opp2)).target());
				  shPtr1 = new ShapePoint(vvv, m_shape.size());
				  m_shape.insert(shPtr1);
				}
			    }
			  else
			    {
			      shPtr1 = *point_it;
			    }
			  
			  point_it = m_shape.findVec(toVec2((cdt.segment(ec_opp2)).source()));
			  if (point_it == m_shape.end())
			    {
			      shPtr2 = infiniteVertexPtr;
			      if ((cdt.segment(ec_opp2)).source() != ((cdt.infinite_vertex())->point()))
				{
				  Vector2 vvv = toVec2((cdt.segment(ec_opp2)).source());
				  shPtr2 = new ShapePoint(vvv, m_shape.size());
				  m_shape.insert(shPtr2);
				}			   
			    }
			  else
			    {
			      shPtr2 = *point_it;
			    }

			  SkelEdgePtr last_ed = new SkelEdge(shPtr1,
							     shPtr2,
							     cdt.is_constrained(ec_opp2),
							     cdt.is_infinite(ec_opp2));
			  SkelTriangle cur_tri = SkelTriangle(prev_ed, cur_ed, last_ed);
			  
			  if ((cur_tri.area() != 0))
			    {
			      SkelEdgeSet::iterator edge_it;
			      std::pair<SkelEdgeSet::iterator, bool> res_insertion;
			      SkelEdgePtr toInsert = new SkelEdge(*cur_ed);
			      res_insertion = (m_allEdges.insert(toInsert));
			      edge_it = res_insertion.first;
			      bool inserted = res_insertion.second;
			      SkelEdgePtr cur_ed_ptr = *edge_it;
			      
			      toInsert = new SkelEdge(*prev_ed);
			      
			      res_insertion = (m_allEdges.insert(toInsert));
			      
			      edge_it = res_insertion.first;
			      inserted = res_insertion.second;
			      
			      SkelEdgePtr prev_ed_ptr = *edge_it;
			      
			      toInsert = new SkelEdge(*last_ed);
			      res_insertion = (m_allEdges.insert(toInsert));
			      edge_it = res_insertion.first;
			      inserted = res_insertion.second;
			      SkelEdgePtr last_ed_ptr = *edge_it;
			      
			      SkelTriangleWeakPtr toInsertTriangle = new SkelTriangle(prev_ed_ptr,
										      last_ed_ptr,
										      cur_ed_ptr,
										      cdt.is_infinite(face_handle3));
			      std::pair<SkelTriangleSet::iterator, bool> res_insertion_triangle;
			      SkelTriangleSet::iterator tria_it;
			      res_insertion_triangle = (m_allTriangles.insert(toInsertTriangle));
			      tria_it = res_insertion_triangle.first;
			      inserted =  res_insertion_triangle.second;
			      SkelTriangleWeakPtr tri_ptr = *tria_it;
			      
			      prev_ed_ptr->addAdjTri(tri_ptr);
			      last_ed_ptr->addAdjTri(tri_ptr);
			      cur_ed_ptr ->addAdjTri(tri_ptr);
			      if (inserted && tri_ptr->m_infinite)
				outOfTheShapeTriangles.push(tri_ptr);
			    }
			}
		    }
		}
	    }
	  while (++ec != done);
	}
      
    }

  Debug::debug("size outOfTheShapeTriangles : ",(int)outOfTheShapeTriangles.size(),10);
  Debug::debug("size m_allTriangles : ",(int)m_allTriangles.size(),10);

  // We must eliminate all the faces and edges outside our shape 
  // We will check on infinite edges 

  while (!outOfTheShapeTriangles.empty())
    {
      SkelTriangleWeakPtr toErase = outOfTheShapeTriangles.top();
      outOfTheShapeTriangles.pop();
      if ((!toErase->m_e1->m_infinite) && (toErase->m_e1->m_type == SkelEdge::INTERIOR))
	{
	  if ((((toErase->m_e1)->m_adjTri1) != 0)&&(*((toErase->m_e1)->m_adjTri1) == *toErase))
	    {
	      if ((toErase->m_e1)->m_adjTri2 != 0 )
		{
		  if ((m_allTriangles.find((toErase->m_e1)->m_adjTri2) != m_allTriangles.end())
		      && !(((toErase->m_e1)->m_adjTri2)->m_infinite))
		    outOfTheShapeTriangles.push((toErase->m_e1)->m_adjTri2);
		}
	    }
	  else 
	    {
	      if ((toErase->m_e1)->m_adjTri1 != 0 )
		{
		  if ((m_allTriangles.find((toErase->m_e1)->m_adjTri1) != m_allTriangles.end())
		      && !(((toErase->m_e1)->m_adjTri1)->m_infinite))
		    outOfTheShapeTriangles.push((toErase->m_e1)->m_adjTri1);
		}
	    }
	}
      if ((!toErase->m_e2->m_infinite) && (toErase->m_e2->m_type == SkelEdge::INTERIOR))
	{
	  if ((((toErase->m_e2)->m_adjTri1)!=0)&&(*((toErase->m_e2)->m_adjTri1) == *toErase))
	    {
	      if ((toErase->m_e2)->m_adjTri2 != 0 )
		{
		  if ((m_allTriangles.find((toErase->m_e2)->m_adjTri2) != m_allTriangles.end())
		      && !(((toErase->m_e2)->m_adjTri2)->m_infinite))
		    outOfTheShapeTriangles.push((toErase->m_e2)->m_adjTri2);
		}
	    }
	  else 
	    {
	      if ((toErase->m_e2)->m_adjTri1 != 0 )
		{
		  if ((m_allTriangles.find((toErase->m_e2)->m_adjTri1) != m_allTriangles.end())
		      && !(((toErase->m_e2)->m_adjTri1)->m_infinite))
		    outOfTheShapeTriangles.push((toErase->m_e2)->m_adjTri1);
		}
	    }
	}
      if ((!toErase->m_e3->m_infinite) && (toErase->m_e3->m_type == SkelEdge::INTERIOR))
	{
	  if ((((toErase->m_e3)->m_adjTri1) != 0)&&(*((toErase->m_e3)->m_adjTri1) == *toErase))
	    {
	      if ((toErase->m_e3)->m_adjTri2 != 0 )
		{
		  if ((m_allTriangles.find((toErase->m_e3)->m_adjTri2) != m_allTriangles.end())
		      && !(((toErase->m_e3)->m_adjTri2)->m_infinite))
		    outOfTheShapeTriangles.push((toErase->m_e3)->m_adjTri2);
		}
	    }
	  else
	    {
	      if ((toErase->m_e3)->m_adjTri1 != 0 )
		{
		  if ((m_allTriangles.find((toErase->m_e3)->m_adjTri1) != m_allTriangles.end())
		      && !(((toErase->m_e3)->m_adjTri1)->m_infinite))
		    outOfTheShapeTriangles.push((toErase->m_e3)->m_adjTri1);
		}
	    }
	}
      m_allTriangles.skelErase(toErase);
    }
  Debug::debug("number of triangles : ",(int)m_allTriangles.size(),10);
#endif
}


std::list<Polyline2DPtr> PGL::Skeleton::getChordalAxisTransform()
{
  // Now we have a set of Triangles and Edges, with their relationships
  // We can go through all of them to generate the skeleton
  std::list<Vector2> polyline;
  std::stack<SkelTriangleWeakPtr> leftTriangles;
  
  SkelTriangleSet::iterator tri_it;
  if (!m_allTriangles.empty())
    {
      SkelBranch::EndType lastEndType;
      int bumpBeg, bumpEnd, bumpMid;
      SkelTriangleWeakPtr current_tri = m_allTriangles.find_next();
      if (current_tri != 0)
	{
	  switch(current_tri->m_type)
	    {
	    case SkelTriangle::TERMINATION:
	      bumpMid = current_tri->getMiddleSummitForTerm();
	      lastEndType = SkelBranch::TERMINATION;
	      break;
	    case SkelTriangle::JONCTION:
	      lastEndType = SkelBranch::JONCTION;
	      break;
	    default:
	      lastEndType = SkelBranch::UNDEFINED;
	      break;
	    }
	}
      
      int nbTours = 0;
      do
  	{
  	  Vector2 mid1;
  	  Vector2 mid2;
  	  if (current_tri == 0)
	    {
  	      if (!leftTriangles.empty())
  		{
  		  current_tri = leftTriangles.top();
  		  leftTriangles.pop();
		  lastEndType = SkelBranch::JONCTION;
  		}
  	      else
  		{
  		  current_tri = m_allTriangles.find_next();
		  if (current_tri != 0)
		    {
		      switch(current_tri->m_type)
			{
			case SkelTriangle::TERMINATION:
			  bumpMid = current_tri->getMiddleSummitForTerm();
			  lastEndType = SkelBranch::TERMINATION;
			  break;
			case SkelTriangle::JONCTION:
			  lastEndType = SkelBranch::JONCTION;
			  break;
			default:
			  lastEndType = SkelBranch::UNDEFINED;
			  break;
			}
		    }
  		}
	    }
  	  else if (current_tri->area() == 0)
  	    {
	      m_allTriangles.skelErase(current_tri);
  	      if (!leftTriangles.empty())
  		{
  		  current_tri = leftTriangles.top();
  		  leftTriangles.pop();
		  lastEndType = SkelBranch::JONCTION;
  		}
  	      else
  		{
  		  current_tri = m_allTriangles.find_next();
		  if (current_tri != 0)
		    {
		      switch(current_tri->m_type)
			{
			case SkelTriangle::TERMINATION:
			  bumpMid = current_tri->getMiddleSummitForTerm();
			  lastEndType = SkelBranch::TERMINATION;
			  break;
			case SkelTriangle::JONCTION:
			  lastEndType = SkelBranch::JONCTION;
			  break;
			default:
			  lastEndType = SkelBranch::UNDEFINED;
			  break;
			}
		    }
  		}
  	      std::cerr << "Skeleton, Chordal Axis Transform : error, uninitialized or deprecated triangle" << std::endl;
  	    }
  	  else
  	    {
	      SkelJonctionPtr jonc;
	      SkelBranchPtr branch;
	      SkelJonctionSet::iterator it_jonction;
  	      switch (current_tri->m_type)
  		{
  		case SkelTriangle::SLEEVE:
  		  // In that case we had to the list the middles of the
  		  // two internal edges
  		  if (current_tri->m_e1->m_type == SkelEdge::BOUNDARY)
  		    {
  		      if (current_tri->m_e2->m_visited)
  			{
  			  mid1 = current_tri->m_e2->getMiddle();
  			  mid2 = current_tri->m_e3->getMiddle();
  			  current_tri->m_e3->m_visited = true;
			  if ((current_tri->m_e3->m_adjTri1 == 0)&&(current_tri->m_e3->m_adjTri2 == 0))
			    {
			      if (!polyline.empty())
				m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
			      polyline.clear();
			      lastEndType = SkelBranch::UNDEFINED;
			      m_allTriangles.skelErase(current_tri);
			      if (!leftTriangles.empty())
				{
				  current_tri = leftTriangles.top();
				  leftTriangles.pop();
				  lastEndType = SkelBranch::JONCTION;
				}
			      else
				{
				  current_tri = m_allTriangles.find_next();
				  if (current_tri != 0)
				    {
				      switch(current_tri->m_type)
					{
					case SkelTriangle::TERMINATION:
					  bumpMid = current_tri->getMiddleSummitForTerm();
					  lastEndType = SkelBranch::TERMINATION;
					  break;
					case SkelTriangle::JONCTION:
					  lastEndType = SkelBranch::JONCTION;
					  break;
					default:
					  lastEndType = SkelBranch::UNDEFINED;
					  break;
					}
				    }
				}
			    }
			  else
			    {
			      if (polyline.empty())
				polyline.push_back(mid1);
			      polyline.push_back(mid2);
			      if ((current_tri->m_e3->m_adjTri1 != 0) && (*current_tri == *(current_tri->m_e3->m_adjTri1)))
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e3->m_adjTri1;
				  current_tri = (current_tri->m_e3->m_adjTri2);
				  m_allTriangles.skelErase(temp);
				}
			      else if (current_tri->m_e3->m_adjTri1 == 0)
				{
				  current_tri = (current_tri->m_e3->m_adjTri2);
				}
			      else
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e3->m_adjTri2;
				  current_tri = (current_tri->m_e3->m_adjTri1);
				  m_allTriangles.skelErase(temp);
				}
			    }
  			}
  		      else
  			{
  			  mid2 = current_tri->m_e2->getMiddle();
  			  mid1 = current_tri->m_e3->getMiddle();
  			  current_tri->m_e2->m_visited = true;
			  if ((current_tri->m_e2->m_adjTri1 == 0)&&(current_tri->m_e2->m_adjTri2 == 0))
			    {
			      if (!polyline.empty())
				m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
			      polyline.clear();
			      lastEndType = SkelBranch::UNDEFINED;
			      m_allTriangles.skelErase(current_tri);
			      if (!leftTriangles.empty())
				{
				  current_tri = leftTriangles.top();
				  leftTriangles.pop();
				  lastEndType = SkelBranch::JONCTION;
				}
			      else
				{
				  current_tri = m_allTriangles.find_next();
				  if (current_tri != 0)
				    {
				      switch(current_tri->m_type)
					{
					case SkelTriangle::TERMINATION:
					  bumpMid = current_tri->getMiddleSummitForTerm();
					  lastEndType = SkelBranch::TERMINATION;
					  break;
					case SkelTriangle::JONCTION:
					  lastEndType = SkelBranch::JONCTION;
					  break;
					default:
					  lastEndType = SkelBranch::UNDEFINED;
					  break;
					}
				    }
				}
			    }
			  else
			    {
			      if (polyline.empty())
				polyline.push_back(mid1);
			      polyline.push_back(mid2);
			      if ((current_tri->m_e2->m_adjTri1 != 0)&&(*current_tri == *(current_tri->m_e2->m_adjTri1)))
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e2->m_adjTri1;
				  current_tri = (current_tri->m_e2->m_adjTri2);
				  m_allTriangles.skelErase(temp);
				}
			      else if (current_tri->m_e2->m_adjTri1 == 0)
				{
				  current_tri = (current_tri->m_e2->m_adjTri2);
				}
			      else
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e2->m_adjTri2;
				  current_tri = (current_tri->m_e2->m_adjTri1);
				  m_allTriangles.skelErase(temp);
				}
			    }
  			}
  		    }
  		  else if (current_tri->m_e2->m_type == SkelEdge::BOUNDARY)
  		    {
  		      if (current_tri->m_e1->m_visited)
  			{
  			  mid1 = current_tri->m_e1->getMiddle();
  			  mid2 = current_tri->m_e3->getMiddle();
  			  current_tri->m_e3->m_visited = true;
			  if ((current_tri->m_e3->m_adjTri1 == 0)&&(current_tri->m_e3->m_adjTri2 == 0))
			    {
			      if (!polyline.empty())
				m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
			      polyline.clear();
			      lastEndType = SkelBranch::UNDEFINED;
			      m_allTriangles.skelErase(current_tri);
			      if (!leftTriangles.empty())
				{
				  current_tri = leftTriangles.top();
				  leftTriangles.pop();
				  lastEndType = SkelBranch::JONCTION;
				}
			      else
				{
				  current_tri = m_allTriangles.find_next();
				  if (current_tri != 0)
				    {
				      switch(current_tri->m_type)
					{
					case SkelTriangle::TERMINATION:
					  bumpMid = current_tri->getMiddleSummitForTerm();
					  lastEndType = SkelBranch::TERMINATION;
					  break;
					case SkelTriangle::JONCTION:
					  lastEndType = SkelBranch::JONCTION;
					  break;
					default:
					  lastEndType = SkelBranch::UNDEFINED;
					  break;
					}
				    }
				}
			    }
			  else
			    {
			      if (polyline.empty())
				polyline.push_back(mid1);
			      polyline.push_back(mid2);
			      if ((current_tri->m_e3->m_adjTri1 != 0)&&(*current_tri == *(current_tri->m_e3->m_adjTri1)))
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e3->m_adjTri1;
				  current_tri = (current_tri->m_e3->m_adjTri2);
				  m_allTriangles.skelErase(temp);
				}
			      else if (current_tri->m_e3->m_adjTri1 == 0)
				{
				  current_tri = (current_tri->m_e3->m_adjTri2);
				}
			      else
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e3->m_adjTri2;
				  current_tri = (current_tri->m_e3->m_adjTri1);
				  m_allTriangles.skelErase(temp);
				}
			    }
  			}
  		      else
  			{
  			  mid2 = current_tri->m_e1->getMiddle();
  			  mid1 = current_tri->m_e3->getMiddle();
  			  current_tri->m_e1->m_visited = true;
			  if ((current_tri->m_e1->m_adjTri1 == 0)&&(current_tri->m_e1->m_adjTri2 == 0))
			    {
			      if (!polyline.empty())
				m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
			      polyline.clear();
			      lastEndType = SkelBranch::UNDEFINED;
			      m_allTriangles.skelErase(current_tri);
			      if (!leftTriangles.empty())
				{
				  current_tri = leftTriangles.top();
				  leftTriangles.pop();
				  lastEndType = SkelBranch::JONCTION;
				}
			      else
				{
				  current_tri = m_allTriangles.find_next();
				  if (current_tri != 0)
				    {
				      switch(current_tri->m_type)
					{
					case SkelTriangle::TERMINATION:
					  bumpMid = current_tri->getMiddleSummitForTerm();
					  lastEndType = SkelBranch::TERMINATION;
					  break;
					case SkelTriangle::JONCTION:
					  lastEndType = SkelBranch::JONCTION;
					  break;
					default:
					  lastEndType = SkelBranch::UNDEFINED;
					  break;
					}
				    }
				}
			    }
			  else
			    {
			      if (polyline.empty())
				polyline.push_back(mid1);
			      polyline.push_back(mid2);
			      if ((current_tri->m_e1->m_adjTri1 != 0)&&(*current_tri == *(current_tri->m_e1->m_adjTri1)))
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e1->m_adjTri1;
				  current_tri = (current_tri->m_e1->m_adjTri2);
				  m_allTriangles.skelErase(temp);
				}
			      else if (current_tri->m_e1->m_adjTri1 == 0)
				{
				  current_tri = (current_tri->m_e1->m_adjTri2);
				}
			      else
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e1->m_adjTri2;
				  current_tri = (current_tri->m_e1->m_adjTri1);
				  m_allTriangles.skelErase(temp);
				}
			    }
  			}
  		    }
  		  else
  		    {
  		      if (current_tri->m_e1->m_visited)
  			{
  			  mid1 = current_tri->m_e1->getMiddle();
  			  mid2 = current_tri->m_e2->getMiddle();
  			  current_tri->m_e2->m_visited = true;
			  if ((current_tri->m_e2->m_adjTri1 == 0)&&(current_tri->m_e2->m_adjTri2 == 0))
			    {
			      if (!polyline.empty())
				m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
			      polyline.clear();
			      lastEndType = SkelBranch::UNDEFINED;
			      m_allTriangles.skelErase(current_tri);
			      if (!leftTriangles.empty())
				{
				  current_tri = leftTriangles.top();
				  leftTriangles.pop();
				  lastEndType = SkelBranch::JONCTION;
				}
			      else
				{
				  current_tri = m_allTriangles.find_next();
				  if (current_tri != 0)
				    {
				      switch(current_tri->m_type)
					{
					case SkelTriangle::TERMINATION:
					  bumpMid = current_tri->getMiddleSummitForTerm();
					  lastEndType = SkelBranch::TERMINATION;
					  break;
					case SkelTriangle::JONCTION:
					  lastEndType = SkelBranch::JONCTION;
					  break;
					default:
					  lastEndType = SkelBranch::UNDEFINED;
					  break;
					}
				    }
				}
			    }
			  else
			    {
			      if (polyline.empty())
				polyline.push_back(mid1);
			      polyline.push_back(mid2);
			      if ((current_tri->m_e2->m_adjTri1 != 0)&&(*current_tri == *(current_tri->m_e2->m_adjTri1)))
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e2->m_adjTri1;
				  current_tri = (current_tri->m_e2->m_adjTri2);
				  m_allTriangles.skelErase(temp);
				}
			      else if (current_tri->m_e2->m_adjTri1 == 0)
				{
				  current_tri = (current_tri->m_e2->m_adjTri2);
				}
			      else
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e2->m_adjTri2;
				  current_tri = (current_tri->m_e2->m_adjTri1);
				  m_allTriangles.skelErase(temp);
				}
			    }
  			}
  		      else
  			{
  			  mid2 = current_tri->m_e1->getMiddle();
  			  mid1 = current_tri->m_e2->getMiddle();
  			  current_tri->m_e1->m_visited = true;
			  if ((current_tri->m_e1->m_adjTri1 == 0)&&(current_tri->m_e1->m_adjTri2 == 0))
			    {
			      if (!polyline.empty())
				m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
			      polyline.clear();
			      lastEndType = SkelBranch::UNDEFINED;
			      if (!leftTriangles.empty())
				{
				  current_tri = leftTriangles.top();
				  leftTriangles.pop();
				  lastEndType = SkelBranch::JONCTION;
				}
			      else
				{
				  current_tri = m_allTriangles.find_next();
				  if (current_tri != 0)
				    {
				      switch(current_tri->m_type)
					{
					case SkelTriangle::TERMINATION:
					  bumpMid = current_tri->getMiddleSummitForTerm();
					  lastEndType = SkelBranch::TERMINATION;
					  break;
					case SkelTriangle::JONCTION:
					  lastEndType = SkelBranch::JONCTION;
					  break;
					default:
					  lastEndType = SkelBranch::UNDEFINED;
					  break;
					}
				    }
				}
			    }
			  else
			    {
			      if (polyline.empty())
				polyline.push_back(mid1);
			      polyline.push_back(mid2);
			      if ((current_tri->m_e1->m_adjTri1 != 0)&&(*current_tri == *(current_tri->m_e1->m_adjTri1)))
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e1->m_adjTri1;
				  current_tri = (current_tri->m_e1->m_adjTri2);
				  m_allTriangles.skelErase(temp);
				}
			      else if (current_tri->m_e1->m_adjTri1 == 0)
				{
				  current_tri = (current_tri->m_e1->m_adjTri2);
				}
			      else
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e1->m_adjTri2;
				  current_tri = (current_tri->m_e1->m_adjTri1);
				  m_allTriangles.skelErase(temp);
				}
			    }
  			}
  		    }
  		  break;
  		case SkelTriangle::TERMINATION:
  		  // here we add the middle of interior edge and gravity center
  		  if (current_tri->m_e1->m_type == SkelEdge::INTERIOR)
  		    {
  		      mid1 = current_tri->m_e1->getMiddle();
  		    }
  		  else if (current_tri->m_e2->m_type == SkelEdge::INTERIOR)
  		    {
  		      mid1 = current_tri->m_e2->getMiddle();
  		    }
  		  else
  		    {
  		      mid1 = current_tri->m_e3->getMiddle();
  		    }
  		  polyline.push_back(mid1);
  		  polyline.push_back(current_tri->getGravityCenter());
		
  		  // As we are at the end of a branch, we save the polyline realised
  		  // And begin with a new one
		  bumpMid = current_tri->getMiddleSummitForTerm();
		  branch = new SkelBranch(polyline, &m_shape, lastEndType, SkelBranch::TERMINATION, bumpBeg, bumpEnd, bumpMid);
  		  m_skeleton.push_back(branch);
		  it_jonction = (m_jonctions.insert(new SkelJonction(*(polyline.begin())))).first;
		  if ((it_jonction != m_jonctions.end()))
		    {
		      (*it_jonction)->addBranch(branch);
		    }
  		  m_allTriangles.skelErase(current_tri);
  		  polyline.clear();
  		  // Now we begin a new polyline
  		  // So we search the last jonction not completely explored
  		  // Or a new jonction
  		  if (!leftTriangles.empty())
  		    {
  		      current_tri = (leftTriangles.top());
  		      leftTriangles.pop();
		      lastEndType = SkelBranch::JONCTION;
  		    }
  		  else
  		    {
  		      // This should not happen in real life
  		      // As the skeleton is supposed to be connex
  		      current_tri = m_allTriangles.find_next();
		      if (current_tri != 0)
			{
			  switch(current_tri->m_type)
			    {
			    case SkelTriangle::TERMINATION:
			      bumpMid = current_tri->getMiddleSummitForTerm();
			      lastEndType = SkelBranch::TERMINATION;
			      break;
			    case SkelTriangle::JONCTION:
			      lastEndType = SkelBranch::JONCTION;
			      break;
			    default:
			      lastEndType = SkelBranch::UNDEFINED;
			      break;
			    }
			}
		      else
			lastEndType = SkelBranch::UNDEFINED;
  		    }
		  if (current_tri == 0)
		    {
		      lastEndType = SkelBranch::UNDEFINED;
		    }
		  else
		    {
		      switch (current_tri->m_type)
			{
			case SkelTriangle::JONCTION:
			  lastEndType = SkelBranch::JONCTION;
			  break;
			case SkelTriangle::TERMINATION:
			  bumpMid = current_tri->getMiddleSummitForTerm();
			  lastEndType = SkelBranch::TERMINATION;
			  break;
			default:
			  lastEndType = SkelBranch::UNDEFINED;
			  break;
			}
		    }
  		  break;
  		case SkelTriangle::JONCTION:
  		  mid1 = current_tri->getPseudoCircumCenter();
		  jonc = new SkelJonction(mid1);
		  it_jonction = (m_jonctions.insert(jonc)).first;
		  if (!polyline.empty())
		    {
		      jonc = *it_jonction;
		      polyline.push_back(mid1);
		      if (lastEndType == SkelBranch::TERMINATION)
			{
			  if ((current_tri->m_e1->m_p1->m_ind < bumpMid) && (current_tri->m_e1->m_p2->m_ind > bumpMid))
			    {
			      bumpBeg = current_tri->m_e1->m_p1->m_ind;
			      bumpEnd = current_tri->m_e1->m_p2->m_ind;
			    }
			  if ((current_tri->m_e1->m_p1->m_ind > bumpMid) && (current_tri->m_e1->m_p2->m_ind < bumpMid))
			    {
			      bumpBeg = current_tri->m_e1->m_p2->m_ind;
			      bumpEnd = current_tri->m_e1->m_p1->m_ind;
			    }
			  if ((current_tri->m_e2->m_p1->m_ind < bumpMid) && (current_tri->m_e2->m_p2->m_ind > bumpMid))
			    {
			      bumpBeg = current_tri->m_e2->m_p1->m_ind;
			      bumpEnd = current_tri->m_e2->m_p2->m_ind;
			    }
			  if ((current_tri->m_e2->m_p1->m_ind > bumpMid) && (current_tri->m_e2->m_p2->m_ind < bumpMid))
			    {
			      bumpBeg = current_tri->m_e2->m_p2->m_ind;
			      bumpEnd = current_tri->m_e2->m_p1->m_ind;
			    }
			  if ((current_tri->m_e3->m_p1->m_ind < bumpMid) && (current_tri->m_e3->m_p2->m_ind > bumpMid))
			    {
			      bumpBeg = current_tri->m_e3->m_p1->m_ind;
			      bumpEnd = current_tri->m_e3->m_p2->m_ind;
			    }
			  if ((current_tri->m_e3->m_p1->m_ind > bumpMid) && (current_tri->m_e3->m_p2->m_ind < bumpMid))
			    {
			      bumpBeg = current_tri->m_e3->m_p2->m_ind;
			      bumpEnd = current_tri->m_e3->m_p1->m_ind;
			    }
			}
		      else
			{
			  bumpBeg = -1;
			  bumpEnd = -1;
			}
		      branch = new SkelBranch(polyline, 
					      &m_shape, 
					      lastEndType, 
					      SkelBranch::JONCTION, 
					      bumpBeg, 
					      bumpEnd, 
					      bumpMid);
		      lastEndType = SkelBranch::JONCTION;
		      jonc->addBranch(branch);
		      m_skeleton.push_back(branch);
		  
		      polyline.clear();
		    }
		  polyline.push_back(mid1);

  		  if (!current_tri->m_e1->m_visited)
  		    {
  		      current_tri->m_e1->m_visited = true;
		      bumpBeg = current_tri->m_e1->m_p1->m_ind;
		      bumpEnd = current_tri->m_e1->m_p2->m_ind;
  		      if ((current_tri->m_e1->m_adjTri1 == 0)&&(current_tri->m_e1->m_adjTri2 == 0))
  			{
			  // We might have met a suppressed triangle for smoothing
			  // So we have to end the polyline if it is not empty
			  // And get to the next thing
			  if (!polyline.empty())
			    m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
			  polyline.clear();
			  if (!leftTriangles.empty())
			    {
			      current_tri = leftTriangles.top();
			      leftTriangles.pop();
			      lastEndType = SkelBranch::JONCTION;
			    }
			  else
			    {
			      current_tri = m_allTriangles.find_next();
			      if ( current_tri != 0)
				{
				  switch(current_tri->m_type)
				    {
				    case SkelTriangle::TERMINATION:
				      bumpMid = current_tri->getMiddleSummitForTerm();
				      lastEndType = SkelBranch::TERMINATION;
				      break;
				    case SkelTriangle::JONCTION:
				      lastEndType = SkelBranch::JONCTION;
				      break;
				    default:
				      lastEndType = SkelBranch::UNDEFINED;
				      break;
				    }
				}
			    }
  			}
		      else
			{
			  polyline.push_back(current_tri->m_e1->getMiddle());
			  if ((!current_tri->m_e2->m_visited)||(!current_tri->m_e3->m_visited))
			    leftTriangles.push(current_tri);
			  else
			    {
			      m_allTriangles.skelErase(current_tri);
			    }
			  if (current_tri->m_e1->m_adjTri1 == 0)
			    {
			      current_tri = (current_tri->m_e1->m_adjTri2);
			    }
			  else if (*current_tri == *(current_tri->m_e1->m_adjTri1))
			    {
			      current_tri = (current_tri->m_e1->m_adjTri2);
			    }
			  else if (current_tri->m_e1->m_adjTri2 == 0)
			    {
			      current_tri = (current_tri->m_e1->m_adjTri1);
			    }
			  else
			    {
			      current_tri = (current_tri->m_e1->m_adjTri1);
			    }
			}
  		    }
  		  else if (!current_tri->m_e2->m_visited)
  		    {
		      bumpBeg = current_tri->m_e2->m_p1->m_ind;
		      bumpEnd = current_tri->m_e2->m_p2->m_ind;
  		      current_tri->m_e2->m_visited = true;
  		      if ((current_tri->m_e2->m_adjTri1 == 0)&&(current_tri->m_e2->m_adjTri2 == 0))
  			{
			  // We might have met a suppressed triangle for smoothing
			  // So we have to end the polyline if it is not empty
			  // And get to the next thing
			  if (!polyline.empty())
			    m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
			  polyline.clear();
			  if (!leftTriangles.empty())
			    {
			      current_tri = leftTriangles.top();
			      leftTriangles.pop();
			      lastEndType = SkelBranch::JONCTION;
			    }
			  else
			    {
			      current_tri = m_allTriangles.find_next();
			      if (current_tri != 0)
				{
				  switch(current_tri->m_type)
				    {
				    case SkelTriangle::TERMINATION:
				      bumpMid = current_tri->getMiddleSummitForTerm();
				      lastEndType = SkelBranch::TERMINATION;
				      break;
				    case SkelTriangle::JONCTION:
				      lastEndType = SkelBranch::JONCTION;
				      break;
				    default:
				      lastEndType = SkelBranch::UNDEFINED;
				      break;
				    }
				}
			    }
  			}
		      else
			{
			  polyline.push_back(current_tri->m_e2->getMiddle());
			  if ((!current_tri->m_e1->m_visited)||(!current_tri->m_e3->m_visited))
			    leftTriangles.push(current_tri);
			  else
			    m_allTriangles.skelErase(current_tri);
			  if (current_tri->m_e2->m_adjTri1 == 0)
			    {
			      current_tri = (current_tri->m_e2->m_adjTri2);
			    }
			  else if (*current_tri == *(current_tri->m_e2->m_adjTri1))
			    {
			      current_tri = (current_tri->m_e2->m_adjTri2);
			    }
			  else if (current_tri->m_e2->m_adjTri2 == 0)
			    {
			      current_tri = (current_tri->m_e2->m_adjTri1);
			    }
			  else
			    {
			      current_tri = (current_tri->m_e2->m_adjTri1);
			    }
			}
  		    }
  		  else if (!current_tri->m_e3->m_visited)
  		    {
		      bumpBeg = current_tri->m_e3->m_p1->m_ind;
		      bumpEnd = current_tri->m_e3->m_p2->m_ind;
  		      current_tri->m_e3->m_visited = true;
  		      if ((current_tri->m_e3->m_adjTri1 == 0)&&(current_tri->m_e3->m_adjTri2 == 0))
  			{
			  //Debug::debug("getChordalAxisTransform jonction 13");
			  // We might have met a suppressed triangle for smoothing
			  // So we have to end the polyline if it is not empty
			  // And get to the next thing
			  if (!polyline.empty())
			    m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
			  polyline.clear();
			  if (!leftTriangles.empty())
			    {
			      current_tri = leftTriangles.top();
			      leftTriangles.pop();
			      lastEndType = SkelBranch::JONCTION;
			    }
			  else
			    {
			      current_tri = m_allTriangles.find_next();
			      if (current_tri != 0)
				{
				  switch(current_tri->m_type)
				    {
				    case SkelTriangle::TERMINATION:
				      bumpMid = current_tri->getMiddleSummitForTerm();
				      lastEndType = SkelBranch::TERMINATION;
				      break;
				    case SkelTriangle::JONCTION:
				      lastEndType = SkelBranch::JONCTION;
				      break;
				    default:
				      lastEndType = SkelBranch::UNDEFINED;
				      break;
				    }
				}
			    }
  			}
		      else
			{
			  polyline.push_back(current_tri->m_e3->getMiddle());
			  if ((!current_tri->m_e2->m_visited)||(!current_tri->m_e1->m_visited))
			    {
			      leftTriangles.push(current_tri);
			    }
			  else
			    {
			      m_allTriangles.skelErase(current_tri);
			    }
			  if (current_tri->m_e3->m_adjTri1 == 0)
			    {
			      current_tri = (current_tri->m_e3->m_adjTri2);
			    }
			  else if (*current_tri == *(current_tri->m_e3->m_adjTri1))
			    {
			      current_tri = (current_tri->m_e3->m_adjTri2);
			    }
			  else if (current_tri->m_e3->m_adjTri2 == 0)
			    {
			      current_tri = (current_tri->m_e3->m_adjTri1);
			    }
			  else
			    {
			      current_tri = (current_tri->m_e3->m_adjTri1);
			    }
			}
  		    }
  		  else
  		    {
  		      m_allTriangles.skelErase(current_tri);
  		      if (!leftTriangles.empty())
  			{
  			  current_tri = leftTriangles.top();
  			  leftTriangles.pop();
			  lastEndType = SkelBranch::JONCTION;
  			}
  		      else
  			{
  			  current_tri = m_allTriangles.find_next();
			  if (current_tri != 0)
			    {
			      switch(current_tri->m_type)
				{
				case SkelTriangle::TERMINATION:
				  bumpMid = current_tri->getMiddleSummitForTerm();
				  lastEndType = SkelBranch::TERMINATION;
				  break;
				case SkelTriangle::JONCTION:
				  lastEndType = SkelBranch::JONCTION;
				  break;
				default:
				  lastEndType = SkelBranch::UNDEFINED;
				  break;
				}
			    }
  			}
  		      std::cerr << "Skeleton, ChordalAxisTransform : we should not be there... Triangle done !" << std::endl;
  		    }
  		  break;
  		default:
  		  std::cerr << "Skeleton, Chordal Axis Transform : Type of Triangle is undefined" << std::endl;
  		}
  	    }
	  //Debug::debug("getChordalAxisTransform 5");
  	}
      while ((!m_allTriangles.empty())||(!leftTriangles.empty()));
      if (!polyline.empty())
	{
	  SkelBranchPtr branch = new SkelBranch(polyline, &m_shape, lastEndType, SkelBranch::TERMINATION, bumpBeg, bumpEnd, bumpMid);
	  m_skeleton.push_back(branch);
	  if (lastEndType == SkelBranch::JONCTION)
	    {
	      SkelJonctionSet::iterator it_jonction;
	      it_jonction = m_jonctions.find(new SkelJonction((*polyline.begin())));
	      if (it_jonction != m_jonctions.end())
		{
		  (*it_jonction)->addBranch(branch);
		}
	    }
	}
    }
  else
    {
      std::cerr << "Skeleton, Chordal Axis Transform : Triangulation is empty" << std::endl;
      assert(!m_allTriangles.empty());
    }
  return SkelBranch::getListPolylines(m_skeleton);
}

void PGL::Skeleton::filterLittleBranchesOnBranchSize(double sizeMaxBranchesToRemove)
{
  SkelJonctionSet::iterator it;
  for (it = m_jonctions.begin(); it != m_jonctions.end();it ++)
    {
      for (int i = (*it)->m_nbBranches-1 ; i >= 0 ; --i)
	{
	  SkelBranchPtr branch = (*it)->m_branches[i];
	  int branch_size = ((branch->m_branch)->getPointListSize())-1;
	  if (branch->m_secondEnd == SkelBranch::TERMINATION)
	    {
	      SkelEdge ed = SkelEdge(branch->m_branch->getPointListAt(branch_size),
				     branch->m_branch->getPointListAt(branch_size-1));
	      while ((ed.length() < sizeMaxBranchesToRemove) && (branch_size > 0))
		{
		  branch->m_branch = (branch->m_branch->split(branch_size-1)).first;
		  branch_size--;
		  ed = SkelEdge(branch->m_branch->getPointListAt(branch_size),
				branch->m_branch->getPointListAt(branch_size-1));
		}
	    }
	  if (branch->m_firstEnd == SkelBranch::TERMINATION)
	    {
	      SkelEdge ed = SkelEdge(branch->m_branch->getPointListAt(0),
				     branch->m_branch->getPointListAt(1));
	      while ((ed.length() < sizeMaxBranchesToRemove) && (branch_size > 0))
		{
		  branch_size--;
		  branch->m_branch = (branch->m_branch->split(1)).second;
		  ed = SkelEdge(branch->m_branch->getPointListAt(0),
				branch->m_branch->getPointListAt(1));
		}
	    }
	}
    }
}

void PGL::Skeleton::filterLittleBranchesOnAreaSize(double areaMaxTrianglesToRemove)
{
  SkelJonctionSet::iterator it;
  for (it = m_jonctions.begin(); it != m_jonctions.end();it ++)
    {
      for (int i = (*it)->m_nbBranches-1 ; i >= 0 ; --i)
	{
	  SkelBranchPtr branch = (*it)->m_branches[i];
	  int branch_size = ((branch->m_branch)->getPointListSize())-1;
	  int bumpMiddle = branch->m_middleBumpIndice;
	  ShapePointSet::const_iterator it_sps = m_shape.findInd(bumpMiddle);
	  if ((branch->m_firstEnd == SkelBranch::TERMINATION)||(branch->m_secondEnd == SkelBranch::TERMINATION))
	    {
	      if (it_sps != m_shape.end())
		{
		  it_sps--;
		  ShapePointSet::const_iterator prec_sps = it_sps;
		  if (it_sps == m_shape.end())
		    prec_sps = (--m_shape.end());
		  else
		    prec_sps = it_sps;
		  it_sps++;
		  it_sps++;
		  ShapePointSet::const_iterator next_sps;
		  if (it_sps == m_shape.end())
		    next_sps = m_shape.begin();
		  else
		    next_sps = it_sps;
		  it_sps--;
		  if ((it_sps == m_shape.end())||(prec_sps == m_shape.end())||(next_sps == m_shape.end()))
		    std::cerr << "there might be a pb trying to access end iterator ! " << std::endl; 
		  SkelTriangle tr = SkelTriangle(*it_sps,*(prec_sps),(*next_sps));
		  if (tr.area() < areaMaxTrianglesToRemove)
		    {
		      m_skeleton.remove((*it)->m_branches[i]);
		      if (i != (*it)->m_nbBranches-1)
			{
			  (*it)->m_branches[i] = (*it)->m_branches[i+1];
			  (*it)->m_branches[i+1] = 0;
			}
		      else
			(*it)->m_branches[i] = 0;
		      (*it)->m_nbBranches --;
		    }
		}
	    }
	}
    }
}

void PGL::Skeleton::filterLittleBranchesOnBranchAreaSize(double areaMaxBranchesToRemove)
{
  SkelJonctionSet::iterator it;
  for (it = m_jonctions.begin(); it != m_jonctions.end();it ++)
    {
      for (int i = (*it)->m_nbBranches-1 ; i >= 0 ; --i)
	{
	  SkelBranchPtr branch = (*it)->m_branches[i];
	  int branch_size = ((branch->m_branch)->getPointListSize())-1;
	  if ((branch->m_firstEnd == SkelBranch::TERMINATION)||(branch->m_secondEnd == SkelBranch::TERMINATION))
	    {
	      if (branch->area() < areaMaxBranchesToRemove)
		{
		  m_skeleton.remove((*it)->m_branches[i]);
		  if (i != (*it)->m_nbBranches-1)
		    {
		      (*it)->m_branches[i] = (*it)->m_branches[i+1];
		      (*it)->m_branches[i+1] = 0;
		    }
		  else
		    (*it)->m_branches[i] = 0;
		  (*it)->m_nbBranches --;
		}
	    }
	}
    }
}


TriangleSetPtr PGL::Skeleton::getTriangleSet()
{
  SkelTriangleSet::iterator it;
  std::list<Index3> indices_list;
  for (it = m_allTriangles.begin(); it != m_allTriangles.end(); it ++)
    {
      Index3 i;
      i = Index3((*it)->m_e1->m_p1->m_ind, (*it)->m_e1->m_p2->m_ind, (*it)->m_e2->m_p2->m_ind);
      indices_list.push_back(i);
    }
  Index3ArrayPtr indices = new Index3Array(indices_list.begin(), indices_list.end()); 

  std::list<Vector3> point_list;
  ShapePointSet::iterator iitt;
  for(iitt = m_shape.begin(); iitt != m_shape.end(); iitt++ )
    {
      Vector3 v = Vector3((*iitt)->x(), (*iitt)->y(), 0);
      point_list.push_back(v);
    }
  TriangleSetPtr res = new TriangleSet(new Point3Array(point_list.begin(),point_list.end()),indices);
  return res;
}

class MyCompareVector2
{
public:
  bool operator()(const Vector2& v1, const Vector2& v2)
  {
    if (v1 == v2)
      return false;
    else
      {
	return v1 < v2;
      }
  }
};

Polyline2DPtr PGL::Skeleton::rediscretize(const Polyline2DPtr discretizedShape)
{
  std::list<Vector2> polyline;
  std::set<Vector2, MyCompareVector2> points;
  for (int i = 0; i < discretizedShape->getPointListSize(); i++)
    {
      std::pair<std::set<Vector2, MyCompareVector2>::iterator, bool> res;
      Vector2 v = discretizedShape->getPointListAt(i);
      res = points.insert(v);
      if (res.second)
	{
	  polyline.push_back(v);
	}
      else
	{
	  if ((polyline.back() != v) && (polyline.back() != *res.first))
	    polyline.push_back(*res.first);
	}
    }
  return new Polyline2D(new Point2Array(polyline.begin(),polyline.end()));
}

std::list<Polyline2DPtr> PGL::Skeleton::getChordalAxisTransform(const Polyline2DPtr discretizedShape, double areaMaxFilter)
{
  // Polyline2DPtr new_polyline = Skeleton::rediscretize(discretizedShape);
//   Skeleton skel(new_polyline);
  filterAndHomogenize(*(discretizedShape->getPointList()));
  Skeleton skel(discretizedShape);
  skel.getChordalAxisTransform();
  skel.filterLittleBranchesOnBranchAreaSize(areaMaxFilter);
  return SkelBranch::getListPolylines(skel.m_skeleton);
}

TriangleSetPtr PGL::Skeleton::getDelaunayConstrained2DTriangulation(const Polyline2DPtr discretizedShape)
{
  Skeleton skel(Skeleton::rediscretize(discretizedShape));
  return skel.getTriangleSet();
}
