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
#include <iostream>
#include <iterator>
#include <vector>


PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

#ifdef WITH_CGAL

// #define CGAL_TRIANGULATION_NO_PRECONDITIONS 1

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
#if CGAL_VERSION_NR >= 1030401000
#include <CGAL/Arr_overlay_2.h>
#else
#include <CGAL/Arr_overlay.h>
#endif
#include <CGAL/Arr_default_overlay_traits.h>
#include <CGAL/Cartesian.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Sweep_line_2_algorithms.h>
#include <list>


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
typedef CGAL::Quotient<CGAL::MP_Float>                    NT;
typedef CGAL::Cartesian<NT>                               Kernel;
typedef Kernel::Point_2                                   Point_intersections;
typedef CGAL::Arr_segment_traits_2<Kernel>                Traits_2;
typedef Traits_2::Curve_2                                 Segment_intersections;

Vector2 toVec2(const Point_2& p) { return Vector2(to_double(p.x()),to_double(p.y())); }
Point_2 toPoint2(const Vector2 &v) {  return Point_2(v.x(),v.y());}
Vector2 toVec2(const Point_intersections& p) { return Vector2(to_double(p.x()),to_double(p.y())); }
Point_intersections toPoint_intersections(const Vector2 &v) {  return Point_intersections(v.x(),v.y());}

#endif

class Debug
{
public:
  static const int LEVEL = 3;

  static inline void debug(std::string msg, int i = 1)
  { if (i < LEVEL) std::cout << msg << std::endl; }

  static inline void debug(std::string msg, double valueToCheck, int i = 1)
  { if (i < LEVEL) std::cout << msg << " " << valueToCheck   << std::endl; }

  static inline void debug(std::string msg, int valueToCheck, int i = 1)
  { if (i < LEVEL) std::cout << msg << " " << valueToCheck   << std::endl; }

  static inline void debug(std::string msg, void* pointerToCheck, int i = 1)
  { if (i < LEVEL) std::cout << msg << " " << pointerToCheck << std::endl; }
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
  ShapePointPtr ptr = new ShapePoint(0,0,indice);
  ShapePointSet::const_iterator it = find(ptr);
  return it; 
}

ShapePointSet::~ShapePointSet()
{
  clear();
//   ShapePointSet::iterator it = begin();
//   while(it!= end())
//     {
//       delete *it;
//       it++;
//     }
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

SkelEdge::SkelEdge(ShapePointPtr p1 ,ShapePointPtr p2 , TypeEdge type , bool infinite, bool visited )
  :m_type(type),
   m_visited(visited),
   m_infinite(infinite)
{
  if ((p1 == 0) || (p2 == 0))
    {
      pglError( "Invalid parameters for SkelEdge constructor : cannot build an edge with null pointers");
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

SkelEdge::SkelEdge(ShapePointPtr p1 ,
		   ShapePointPtr p2 , 
		   bool boundary , 
		   bool infinite,
		   bool visited )
  :m_visited(visited),
   m_infinite(infinite)
{
  if ((p1 == 0) || (p2 == 0))
    {
      pglError( "Invalid parameters for SkelEdge constructor : cannot build an edge with null pointers" );
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
      pglError( "param is a null pointer" );
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
      pglError( "SkelEdge, getMiddle : uninitialized edge" );
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
      pglError( "one edge is null !!!!!!!!" );
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
	  pglError( "SkelTriangle, constructor : invalid configuration 1" );
	  return;
	}
      if (!(*(m_e1->m_p2) == *(m_e3->m_p1)))
	{
	  pglError( "SkelTriangle, constructor : invalid configuration 2" );
	  return;
	}
      if (!(*(m_e2->m_p2) ==*(m_e3->m_p2)))
	{
	  pglError( "SkelTriangle, constructor : invalid configuration 3" );
	  return;
	}

      int nbBoundaryEdges = 0;
      if (m_e1->m_type == SkelEdge::UNDEFINED || m_e2->m_type == SkelEdge::UNDEFINED || m_e3->m_type == SkelEdge::UNDEFINED)
	{
	  pglError( "one edge is null !!!!!!!!" );
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
	      pglError( "one edge is null !!!!!!!!" );
	      m_type = UNDEFINED;
	      break;
	    }
	}
    }
}

SkelTriangle::SkelTriangle(ShapePointPtr p1, ShapePointPtr p2, ShapePointPtr p3)
  :m_type(SkelTriangle::UNDEFINED),
   m_infinite(false)
{
  ShapePointPtr p = 0;
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
      pglError( "SkelTriangle, operator==, problem : deprecated or uninitialized triangle" );
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
      pglError( "SkelTriangle, area : uninitialized or deprecated triangle" );
      return 0.0;
    }

  // infinite face
  if ((m_e1->m_p1 == 0)||(m_e1->m_p2 == 0)||(m_e2->m_p1 == 0)||(m_e2->m_p2 == 0))
    {
      pglError( "SkelTriangle, area : edges have null shapepoint" );
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
      pglError( "pb : we're gonna calculate a negative nb sqrt" );
      return 0.0;
    }
  double area = (0.5*sqrt(toTest));

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
	  pglError( "SkelTriangle, operator < : uninitialized or deprecated triangle" );
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
      pglError( "SkelTriangle, getGravityCenter, problem : deprecated or uninitialized triangle" );
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
      pglError( "SkelTriangle, getCircumCenter, problem : deprecated or uninitialized triangle" );
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
      pglError( "SkelTriangle, isAcuteTriangle, problem : deprecated or uninitialized triangle" );
      return false;
    }
  double lengthBiggerSide, length1, length2;
  if ((m_e1->m_p1 == 0)||(m_e1->m_p2 == 0)||(m_e2->m_p1 == 0)||(m_e2->m_p2 == 0))
    {
      pglError( "SkelTriangle, isAcuteTriangle, problem : deprecated or uninitialized triangle" );
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
      pglError( "Skeleton, SkelTriangle, getPseudoCircumCenter : deprecated or uninitialized triangle" );
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

ShapePointPtr SkelTriangle::getOppositePoint (const SkelEdge& e) const
{
  if ((m_e1 == 0)||(m_e2 == 0)||(m_e3 == 0))
    {
      pglError( "SkelTriangle, getOppositePoint, problem : deprecated or uninitialized triangle" );
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
//   for (clear_edges_it = begin(); clear_edges_it != end(); clear_edges_it++)
//     {
//       erase(clear_edges_it);
//     }
//   while (!empty())
//     {
//       //*begin() = 0;
//       erase(begin());
//     }
  clear();
}

bool SkelTriangleSet::skelErase(SkelTrianglePtr toErase)
{
  iterator it;
  it = find(toErase);
  if (it != end())
    {
      (*it)->m_e1->rmAdjTri(*(*it));
      (*it)->m_e2->rmAdjTri(*(*it));
      (*it)->m_e3->rmAdjTri(*(*it));
      //delete (*it);
      erase(it);
      return true;
    }
  return false;
}

SkelTriangleSet::~SkelTriangleSet()
{
//   iterator it;
//   for (it = begin(); it != end(); it++)
//     {
//       erase(it);
//     }
//   while (!empty())
//     {
//       delete *begin();
//       erase(begin());
//     }
  clear();
}

SkelTrianglePtr SkelTriangleSet::find_next()
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
      SkelTrianglePtr current_tri = 0;
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
	{
	  pglError( "SkelTriangleSet, find_next : Interesting ! null pointers in allTriangles !" );
	  return 0;
	}
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
		       int middle,
		       int jjbeg2,
		       int jjend2)
  :m_firstEnd(firstEnd), 
   m_secondEnd(secondEnd),
   m_pointSet(ptr),
   m_beginBumpIndice(begin),
   m_endBumpIndice(end),
   m_middleBumpIndice(middle),
   m_joncjoncBegInd2(jjbeg2),
   m_joncjoncEndInd2(jjend2)  
{
  m_branch = new Polyline2D(new Point2Array(points.begin(), points.end()));
  if (m_middleBumpIndice != -1)
    {
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
}

Polyline2DPtr SkelBranch::getPolylineForAssociatedBump()
{
  std::vector<Vector2> polyline;
  ShapePointSet::const_iterator it;
  ShapePointSet::const_iterator end_it = m_pointSet->findInd(m_endBumpIndice);
  if (end_it == m_pointSet->begin())
    end_it = m_pointSet->end();
  for (it = m_pointSet->findInd(m_beginBumpIndice); it != end_it; it++)
    {
      if (it == m_pointSet->end())
	it = m_pointSet->begin();
      polyline.push_back((*it)->m_vec);
    }
  return new Polyline2D(new Point2Array(polyline.begin(), polyline.end()));
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
	float xmax = (discretizedShape->getPointListAt(0)).x(); 
	float ymax = (discretizedShape->getPointListAt(0)).y();
	for (int i = 0; i < discretizedShape->getPointListSize(); ++i)
	{
		Vector2 v = discretizedShape->getPointListAt(i);
		if (v.x() > xmax) xmax = v.x();
		if (v.y() > ymax) ymax = v.y();
		ShapePointSet::const_iterator it = m_shape.findVec(v);
		if (it == m_shape.end())
			m_shape.insert(new ShapePoint(v,i));
		else
			Debug::debug("ALREADY INSERTED POINT : continue...",10,10);
	}
	init(discretizedShape, Vector2(xmax + 0.5, ymax + 0.5));
}

#ifdef WITH_CGAL
CDTplus getCGALTriangulation(const Polyline2DPtr discretizedShape, 
							const Vector2& infinite_vertex)
{
	CDTplus cdt;
	// cdt.clear();

	// insert points and segments as constraints
	Point2ArrayPtr pointList = discretizedShape->getPointList();
	V_handle va = cdt.insert(toPoint2(pointList->getAt(0)));
	V_handle vfirst = va;
	for(Point2Array::const_iterator itPglPoint = pointList->begin(); 
		itPglPoint != pointList->end()-1; ++itPglPoint)
	{
		V_handle vb = cdt.insert(toPoint2(*itPglPoint));
		cdt.insert_constraint(va, vb);
		va = vb;
	}

	// Check if last point is repeating first one or not
	Vector2 lastPoint = pointList->getAt(pointList->size()-1);
	if (norm(pointList->getAt(0) - lastPoint) < GEOM_EPSILON)
	{   // if first and last are the same, we just connect last-1 to first one
		cdt.insert_constraint(va,vfirst);
	}
	else{
		// insert last point and connect it to first one
		V_handle vb = cdt.insert(toPoint2(lastPoint));
		cdt.insert_constraint(va, vb);
		va = vb;
		cdt.insert_constraint(va,vfirst);
	}

	// infinite vertex
	// V_handle infinite = cdt.insert(toPoint2(infinite_vertex));
	// cdt.set_infinite_vertex(infinite);

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
      return edge.first;
    } 
}

void filterLoopsInShape(CDTplus * cdt, ShapePointSet * shape)
{
  CDTplus::Finite_vertices_iterator it;
  for (it = cdt->finite_vertices_begin(); it != cdt->finite_vertices_end(); it++)
    {
      // If this point is not in the shape, it must have been added by CGAL, 
      // so this means that there is an intersection between constrained edges.
      if (shape->findVec(toVec2((*it).point())) == shape->end())
	{
	  CDTplus::Vertex_circulator ec = (*it).incident_vertices();
	  CDTplus::Vertex_circulator end = --ec;
	  ec++;
	  ShapePointSet::const_iterator it_n = shape->end(), 
	    it_n_1 = shape->end(),
	    it_r = shape->end(), 
	    it_r_1 = shape->end();
	  int i = 0;
	  while (ec != end)
	    {
	      if (ec != cdt->infinite_vertex())
		{
		  if (it_n == shape->end())
		    {
		      it_n = shape->findVec(toVec2((*ec).point()));
		    }
		  else
		    {
		      ShapePointSet::const_iterator ittemp = shape->findVec(toVec2((*ec).point()));
		      if (((*ittemp)->m_ind == (*it_n)->m_ind +1)||
			  (((*ittemp)->m_ind == 0)&&((*it_n)->m_ind == shape->size()-1)))
			{
			  it_n_1 = ittemp;
			}
		      else if (((*ittemp)->m_ind == (*it_n)->m_ind - 1)||
			       (((*ittemp)->m_ind == shape->size()-1)&&((*it_n)->m_ind == 0)))
			{
			  it_n_1 = it_n;
			  it_n = ittemp;
			}
		      else
			{
			  if (it_r == shape->end())
			    {
			      it_r = shape->findVec(toVec2((*ec).point()));
			    }
			  else
			    {
			      ShapePointSet::const_iterator ittemp2 = shape->findVec(toVec2((*ec).point()));		      
			      if (((*ittemp2)->m_ind == (*it_r)->m_ind +1)||
				  (((*ittemp2)->m_ind == 0)&&((*it_r)->m_ind == shape->size()-1)))
				{
				  it_r_1 = ittemp2;
				}
			      else if (((*ittemp2)->m_ind == (*it_r)->m_ind - 1)||
				       (((*ittemp2)->m_ind == shape->size()-1)&&((*it_r)->m_ind == 0)))
				{
				  it_r_1 = it_r;
				  it_r = ittemp2;
				}
			      else
				{
				  pglError( "Skeleton, filterLoops Aaaaaaargh" );
				  return;
				}
			    }
			}
		    }
		}
	      ec++;
	      i++;
	    }
	  if (it_n == shape->end() ||it_n_1 == shape->end() ||it_r == shape->end() ||it_r_1 == shape->end())
	    {
	      pglError( "Skeleton, filterLoops : Well, there might be a problem, trying to access end iterator" );
	      return;
	    }
	  else
	    {
	      // okay, now we have organized the beginnings and end of loops
	      // we must calculate the criteria for suppressing one loop
	      std::list<Vector2> empty_list;
	      SkelBranch firstLoop = SkelBranch(empty_list, 
						shape, 
						SkelBranch::UNDEFINED, 
						SkelBranch::UNDEFINED,
						(*it_n)->m_ind,
						(*it_r_1)->m_ind + 1,
						(*it_r_1)->m_ind);
	      SkelBranch secondLoop = SkelBranch(empty_list, 
						 shape, 
						 SkelBranch::UNDEFINED, 
						 SkelBranch::UNDEFINED,
						 (*it_r)->m_ind,
						 (*it_n_1)->m_ind + 1,
						 (*it_n_1)->m_ind);
	    }
	}
    }
}

inline ShapePointPtr toShapePoint(const Point_2& ppp, 
								  CDTplus& cdt, 
								  ShapePointSet& m_shape, 
								  const ShapePointPtr& infiniteVertexPtr) 
{
	ShapePointPtr result = infiniteVertexPtr;
	Vector2 vvv = toVec2(ppp);
	ShapePointSet::const_iterator point_it = m_shape.findVec(vvv);
	if (point_it == m_shape.end())
	{
		if (!cdt.is_infinite((V_handle)&ppp))
		{
			result = new ShapePoint(vvv, m_shape.size());
			m_shape.insert(result);
		}
	}
	else result = *point_it;
	return result;
}

template<class CgalEdge>
inline SkelEdgePtr toSkelEdge(const CgalEdge& edge, 
							  CDTplus& cdt, 
							  ShapePointSet& m_shape, 
							  const ShapePointPtr& infiniteVertexPtr)
{

	ShapePointPtr shPtr1, shPtr2;
	if(!cdt.is_infinite(edge)){
		Segment_2 segment = cdt.segment(edge);
		shPtr1 = toShapePoint(segment.target(), cdt, m_shape, infiniteVertexPtr);
		shPtr2 = toShapePoint(segment.source(), cdt, m_shape, infiniteVertexPtr);
	}
	else
	 {  // look for one of the vertex being the infinite vertex.
		V_handle p1 = edge.first->vertex(cdt.ccw(edge.second));
		if (cdt.is_infinite(p1))
			shPtr1 =  infiniteVertexPtr;
		else shPtr1 =  toShapePoint(p1->point(), cdt, m_shape, infiniteVertexPtr);
		// same test for second vertex of the edge
		V_handle p2 = edge.first->vertex(cdt.cw(edge.second));
		if (cdt.is_infinite(p2))
			shPtr2 =  infiniteVertexPtr;
		else shPtr2 =  toShapePoint(p2->point(), cdt, m_shape, infiniteVertexPtr);
		
	}

	return new SkelEdge(shPtr1, shPtr2, cdt.is_constrained(edge), cdt.is_infinite(edge));
}
#endif


void PGL::Skeleton::init(const Polyline2DPtr discretizedShape, const Vector2& infinite_vertex)
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
	CDTplus cdt = getCGALTriangulation(discretizedShape, infinite_vertex); 


	// We get back the result of the triangulation
	std::stack<SkelTrianglePtr> outOfTheShapeTriangles;

	// ShapePointPtr infiniteVertexPtr = new ShapePoint(toVec2((cdt.infinite_vertex())->point()));
	ShapePointPtr infiniteVertexPtr = new ShapePoint(infinite_vertex);
	// m_shape.insert(infiniteVertexPtr);

	CDT::Finite_vertices_iterator it;
	for (it = cdt.finite_vertices_begin(); 
		 it != cdt.finite_vertices_end() ; ++it)
	{
		CDT::Edge_circulator ec = cdt.incident_edges(it);
		CDT::Edge_circulator done = ec;
		if (ec != 0)
		{
			SkelEdgePtr previous_edge, current_edge, last_edge ;
			do
			{
				// first edge of the skeleton
				current_edge = toSkelEdge(*ec, cdt, m_shape, infiniteVertexPtr);

				// get opposite point of it of edge ec_p
				V_handle opp_p2_vhandle = getEdgeTarget(*ec,it);
				Point_2 opp_p2 = opp_p2_vhandle->point();

				CDT::Edge_circulator ec_p = ec;
				while (++ec_p != done)
				{
					// second edge of the skeleton
					previous_edge = toSkelEdge(*ec_p, cdt,  m_shape, infiniteVertexPtr);

					// get opposite point of it of edge ec_p
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

							// last edge of the skeleton
							SkelEdgePtr last_edge = toSkelEdge(ec_opp2, cdt,  m_shape, infiniteVertexPtr);

							SkelTriangle cur_tri = SkelTriangle(previous_edge, current_edge, last_edge);

							if ((cur_tri.area() != 0))
							{

								/// Insert edges
								current_edge  = *(m_allEdges.insert(current_edge).first);
								previous_edge = *(m_allEdges.insert(previous_edge).first);
								last_edge     = *(m_allEdges.insert(last_edge).first);

								/// Create Triangle
								SkelTrianglePtr triangle = new SkelTriangle(previous_edge,
																			current_edge,
																			last_edge,
																			cdt.is_infinite(face_handle3));

								/// Insert triangle
								std::pair<SkelTriangleSet::iterator, bool> 
								     res_insertion_triangle = (m_allTriangles.insert(triangle));

								triangle = *res_insertion_triangle.first;
								bool inserted =  res_insertion_triangle.second;

								/// Connect triangle to edge
								previous_edge->addAdjTri(triangle.get());
								last_edge->addAdjTri(triangle.get());
								current_edge ->addAdjTri(triangle.get());

								// If it is infinite, should be start of removal
								if (inserted && triangle->m_infinite)
									outOfTheShapeTriangles.push(triangle);
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
		SkelTrianglePtr toErase = outOfTheShapeTriangles.top();
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
  std::stack<SkelTrianglePtr> leftTriangles;
  
  SkelTriangleSet::iterator tri_it;
  if (!m_allTriangles.empty())
    {
      SkelBranch::EndType lastEndType;
      int bumpBeg, bumpEnd, bumpMid;
      SkelTrianglePtr current_tri = m_allTriangles.find_next();
      int size_allTriangles = m_allTriangles.size();
      SkelTrianglePtr prec_tri = current_tri;
      bool terminationIsFirstEnd;
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
  	      pglError( "Skeleton, Chordal Axis Transform : error, uninitialized or deprecated triangle" );
	      std::list<Polyline2DPtr> empty_CAT_list;
	      empty_CAT_list.clear();
	      return empty_CAT_list;
  	    }
  	  else
  	    {
	      SkelJonctionPtr jonc;
	      SkelBranchPtr branch;
	      SkelJonctionSet::iterator it_jonction;
  	      switch (current_tri->m_type)
  		{
  		case SkelTriangle::SLEEVE:
 		  size_allTriangles = m_allTriangles.size();
 		  prec_tri = current_tri;
  		  // In that case we had to the list the middles of the
  		  // two internal edges
  		  if (current_tri->m_e1->m_type == SkelEdge::BOUNDARY)
  		    {
  		      if (current_tri->m_e2->m_visited)
  			{
  			  mid1 = current_tri->m_e2->getMiddle();
  			  mid2 = current_tri->m_e3->getMiddle();
			  if (polyline.empty())
			    polyline.push_back(mid1);
			  polyline.push_back(mid2);
  			  current_tri->m_e3->m_visited = true;
			  if ((current_tri->m_e3->m_adjTri1 == 0)&&(current_tri->m_e3->m_adjTri2 == 0))
			    {
			      if (!polyline.empty())
				{
				  m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
				}
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
			      current_tri->m_e3->m_visited = true;
			      if ((current_tri->m_e3->m_adjTri1 != 0) && (*current_tri == *(current_tri->m_e3->m_adjTri1)))
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e3->m_adjTri1;
				  current_tri = (current_tri->m_e3->m_adjTri2);
				  m_allTriangles.skelErase(temp);
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
			  if (polyline.empty())
			    polyline.push_back(mid1);
			  polyline.push_back(mid2);
  			  current_tri->m_e2->m_visited = true;
			  if ((current_tri->m_e2->m_adjTri1 == 0)&&(current_tri->m_e2->m_adjTri2 == 0))
			    {
			      if (!polyline.empty())
				{
				  m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
				}
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
			      current_tri->m_e2->m_visited = true;
			      if ((current_tri->m_e2->m_adjTri1 != 0)&&(*current_tri == *(current_tri->m_e2->m_adjTri1)))
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e2->m_adjTri1;
				  current_tri = (current_tri->m_e2->m_adjTri2);
				  m_allTriangles.skelErase(temp);
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
			  if (polyline.empty())
			    polyline.push_back(mid1);
			  polyline.push_back(mid2);
  			  current_tri->m_e3->m_visited = true;
			  if ((current_tri->m_e3->m_adjTri1 == 0)&&(current_tri->m_e3->m_adjTri2 == 0))
			    {
			      if (!polyline.empty())
				{
				  m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
				}
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
			      current_tri->m_e3->m_visited = true;
			      if ((current_tri->m_e3->m_adjTri1 != 0)&&(*current_tri == *(current_tri->m_e3->m_adjTri1)))
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e3->m_adjTri1;
				  current_tri = (current_tri->m_e3->m_adjTri2);
				  m_allTriangles.skelErase(temp);
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
			  if (polyline.empty())
			    polyline.push_back(mid1);
			  polyline.push_back(mid2);
  			  current_tri->m_e1->m_visited = true;
			  if ((current_tri->m_e1->m_adjTri1 == 0)&&(current_tri->m_e1->m_adjTri2 == 0))
			    {
			      if (!polyline.empty())
				{
				  m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
				}
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
			      current_tri->m_e1->m_visited = true;
			      if ((current_tri->m_e1->m_adjTri1 != 0)&&(*current_tri == *(current_tri->m_e1->m_adjTri1)))
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e1->m_adjTri1;
				  current_tri = (current_tri->m_e1->m_adjTri2);
				  m_allTriangles.skelErase(temp);
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
			  if (polyline.empty())
			    polyline.push_back(mid1);
			  polyline.push_back(mid2);
  			  current_tri->m_e2->m_visited = true;
			  if ((current_tri->m_e2->m_adjTri1 == 0)&&(current_tri->m_e2->m_adjTri2 == 0))
			    {
			      if (!polyline.empty())
				{
				  m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
				}
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
			      current_tri->m_e2->m_visited = true;
			      if ((current_tri->m_e2->m_adjTri1 != 0)&&(*current_tri == *(current_tri->m_e2->m_adjTri1)))
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e2->m_adjTri1;
				  current_tri = (current_tri->m_e2->m_adjTri2);
				  m_allTriangles.skelErase(temp);
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
			  // m_e1 not visited
  			  mid2 = current_tri->m_e1->getMiddle();
  			  mid1 = current_tri->m_e2->getMiddle();
			  if (polyline.empty())
			    polyline.push_back(mid1);
			  polyline.push_back(mid2);
  			  current_tri->m_e1->m_visited = true;
			  if ((current_tri->m_e1->m_adjTri1 == 0)&&(current_tri->m_e1->m_adjTri2 == 0))
			    {
			      // this case should only happen if the skeleton performs a loop,
			      // which seems quite impossible if the shape is defined by a polyline only
			      if (!polyline.empty())
				{
				  m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
				}
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
			      // now we pass through edge 1
			      current_tri->m_e1->m_visited = true;
			      if ((current_tri->m_e1->m_adjTri1 != 0)&&(*current_tri == *(current_tri->m_e1->m_adjTri1)))
				{
				  SkelTriangleWeakPtr temp = current_tri->m_e1->m_adjTri1;
				  current_tri = (current_tri->m_e1->m_adjTri2);
				  m_allTriangles.skelErase(temp);
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
		  if (m_allTriangles.size() == size_allTriangles)
		    {
		      pglError("on n'a pas supprimé de triangle");
		      //m_allTriangles.skelErase(prec_tri);
		    }
  		  break;
  		case SkelTriangle::TERMINATION:
  		  // here we add the middle of interior edge and gravity center
  		  if (current_tri->m_e1->m_type == SkelEdge::INTERIOR)
  		    {
  		      mid1 = current_tri->m_e1->getMiddle();
		      terminationIsFirstEnd = !current_tri->m_e1->m_visited;
		      current_tri->m_e1->m_visited = true;
  		    }
  		  else if (current_tri->m_e2->m_type == SkelEdge::INTERIOR)
  		    {
  		      mid1 = current_tri->m_e2->getMiddle();
		      terminationIsFirstEnd = !current_tri->m_e2->m_visited;
		      current_tri->m_e2->m_visited = true;
  		    }
  		  else
  		    {
  		      mid1 = current_tri->m_e3->getMiddle();
		      terminationIsFirstEnd = !current_tri->m_e3->m_visited;
		      current_tri->m_e3->m_visited = true;
  		    }
		
  		  // As we are at the end of a branch, we save the polyline realised
  		  // And begin with a new one
		  if (terminationIsFirstEnd)
		    {
		      if (!polyline.empty())
			{
			  polyline.push_back(mid1);
			  polyline.push_back(current_tri->getGravityCenter());
			  bumpMid = current_tri->getMiddleSummitForTerm();
			  branch = new SkelBranch(polyline, 
						  &m_shape, 
						  lastEndType, 
						  SkelBranch::TERMINATION, 
						  bumpBeg, 
						  bumpEnd, 
						  bumpMid);
			  m_skeleton.push_back(branch);
			  it_jonction = (m_jonctions.insert(new SkelJonction(*(polyline.begin())))).first;
			  if ((it_jonction != m_jonctions.end()))
			    {
			      (*it_jonction)->addBranch(branch);
			    }
			  else
			    {
			      pglError("How strange that we get a end() iterator there !");
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
			  pglError("Skeleton, getChordalAxisTransform : this should be the first end, why polyline isn't empty ?");
			  std::list<Polyline2DPtr> empty_CAT_list;
			  empty_CAT_list.clear();
			  return empty_CAT_list;
			}
		      else
			{
			  polyline.push_back(current_tri->getGravityCenter());
			  //polyline.push_back(mid1);
			  lastEndType = SkelBranch::TERMINATION;
			  if (current_tri->m_e1->m_type == SkelEdge::INTERIOR)
			    {
			      bumpBeg = current_tri->m_e1->m_p1->m_ind;
			      bumpEnd = current_tri->m_e1->m_p2->m_ind;
			      if (*(current_tri->m_e1->m_adjTri1) == *current_tri)
				{
				  SkelTrianglePtr temp = current_tri;
				  current_tri = current_tri->m_e1->m_adjTri2;
				  m_allTriangles.skelErase(temp);				
				}
			      else
				{
				  SkelTrianglePtr temp = current_tri;
				  current_tri = current_tri->m_e1->m_adjTri1;
				  m_allTriangles.skelErase(temp);				
				}
			    }
			  else if (current_tri->m_e2->m_type == SkelEdge::INTERIOR)
			    {
			      bumpBeg = current_tri->m_e2->m_p1->m_ind;
			      bumpEnd = current_tri->m_e2->m_p2->m_ind;
			      if (*(current_tri->m_e2->m_adjTri1) == *current_tri)
				{			
				  SkelTrianglePtr temp = current_tri;
				  current_tri = current_tri->m_e2->m_adjTri2;
				  m_allTriangles.skelErase(temp);				
				}
			      else
				{
				  SkelTrianglePtr temp = current_tri;
				  current_tri = current_tri->m_e2->m_adjTri1;
				  m_allTriangles.skelErase(temp);				
				}
			    }
			  else
			    {
			      bumpBeg = current_tri->m_e3->m_p1->m_ind;
			      bumpEnd = current_tri->m_e3->m_p2->m_ind;
			      if (*(current_tri->m_e3->m_adjTri1) == *current_tri)
				{
				  SkelTrianglePtr temp = current_tri;
				  current_tri = current_tri->m_e3->m_adjTri2;
				  m_allTriangles.skelErase(temp);
				}
			      else
				{
				  SkelTrianglePtr temp = current_tri;
				  current_tri = current_tri->m_e3->m_adjTri1;
				  m_allTriangles.skelErase(temp);
				}
			    }
			}
		    }
		  else
		    {
		      //polyline.push_back(mid1);
		      polyline.push_back(current_tri->getGravityCenter());
		      bumpMid = current_tri->getMiddleSummitForTerm();
		      branch = new SkelBranch(polyline, 
					      &m_shape, 
					      lastEndType, 
					      SkelBranch::TERMINATION, 
					      bumpBeg, 
					      bumpEnd, 
					      bumpMid);
		      m_skeleton.push_back(branch);
		      it_jonction = (m_jonctions.insert(new SkelJonction(*(polyline.begin())))).first;
		      if ((it_jonction != m_jonctions.end()))
			{
			  (*it_jonction)->addBranch(branch);
			}
		      else
			{
			  pglError("How strange thet we get an end iterator there ! #2");
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
		      int bumpBeg2 = -1;
		      int bumpEnd2 = -1;
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
		      else if (lastEndType == SkelBranch::JONCTION)
			{
			  bumpMid = -1;
			  int triInd1 = current_tri->m_e1->m_p1->m_ind;
			  int triInd2 = current_tri->m_e1->m_p2->m_ind;
			  int triInd3 = current_tri->m_e2->m_p2->m_ind;
			  // now let's order those 3 indices : bumpBeg2 < triInd3 < bumpEnd2
			  if (triInd2 < triInd1)
			    {
			      int temp = triInd2;
			      triInd2 = triInd1;
			      triInd1 = temp;
			    }
			  if (triInd2 > triInd3)
			    {
			      int temp = triInd2;
			      triInd2 = triInd3;
			      triInd3 = temp;
			    }
			  if (triInd2 < triInd1)
			    {
			      int temp = triInd2;
			      triInd2 = triInd1;
			      triInd1 = temp;
			    }
			  // Now we search which pair of those indices respect bumpBeg < bumpBeg2 < bumpEnd2 < bumpEnd
			  // or bumpBeg2 < bumpBeg < bumpEnd < bumpEnd2
			  if (bumpBeg > bumpEnd)
			    {
			      int temp = bumpBeg;
			      bumpBeg = bumpEnd;
			      bumpEnd = temp;
			    }
			  if ( bumpBeg <= triInd1 )
			    {
			      if (bumpEnd <= triInd1)
				{
				  bumpBeg2 = triInd1;
				  bumpEnd2 = triInd3;
				}
			      else if ( bumpEnd >= triInd3 )
				{
				  bumpBeg2 = triInd1;
				  bumpEnd2 = triInd3;
				}
			      else if ((bumpBeg == triInd1) && (bumpEnd <= triInd2))
				{
				  bumpBeg2 = triInd1;
				  bumpEnd2 = triInd2;
				}
			      else
				{
				  pglError("fffffffffffffffffffffffffffffffffffffffffffff #1");
				  bumpBeg2 = -1;
				  bumpEnd2 = -1;
				}
			    }
			  else if (bumpBeg >= triInd3)
			    {
			      bumpBeg2 = triInd1;
			      bumpEnd2 = triInd3;
			    }
			  else if ( (bumpBeg <= triInd2) && (bumpEnd <= triInd2) )
			    {
			      bumpBeg2 = triInd1;
			      bumpEnd2 = triInd2;
			    }
			  else if ( (bumpBeg >= triInd2) && (bumpBeg <= triInd3) 
				    && (bumpEnd >= triInd2) && (bumpEnd <= triInd3) )
			    {
			      bumpBeg2 = triInd2;
			      bumpEnd2 = triInd3;
			    }
			  else
			    {
			      pglError("ffffffffffffffffffffffffffffffffffff #2");
			      bumpBeg2 = -1;
			      bumpEnd2 = -1;
			    }
			}
		      else
			{
			  pglError("ffffffffffffffffffffffffffffffffffff #3");
			  bumpBeg = -1;
			  bumpEnd = -1;
			}
		      branch = new SkelBranch(polyline, 
					      &m_shape, 
					      lastEndType, 
					      SkelBranch::JONCTION, 
					      bumpBeg, 
					      bumpEnd, 
					      bumpMid,
					      bumpBeg2,
					      bumpEnd2);
		      if (lastEndType == SkelBranch::JONCTION)
			{
			  it_jonction = (m_jonctions.insert(new SkelJonction(*(polyline.begin())))).first;
			  if ((it_jonction != m_jonctions.end()))
			    {
			      (*it_jonction)->addBranch(branch);
			    }
			  else
			    {
			      pglError("How strange that we get a end() iterator there !");
			    }
			}
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
			    {
			      m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
			    }
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
			    {
			      m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
			    }
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
			  // We might have met a suppressed triangle for smoothing
			  // So we have to end the polyline if it is not empty
			  // And get to the next thing
			  if (!polyline.empty())
			    {
			      m_skeleton.push_back(new SkelBranch(polyline, &m_shape));
			    }
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
			  polyline.push_back(mid1);
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
  		      pglError( "Skeleton, ChordalAxisTransform : we should not be there... Triangle done !" );
		      std::list<Polyline2DPtr> empty_CAT_list;
		      empty_CAT_list.clear();
		      return empty_CAT_list;
  		    }
  		  break;
  		default:
  		  pglError( "Skeleton, Chordal Axis Transform : Type of Triangle is undefined" );
		  std::list<Polyline2DPtr> empty_CAT_list;
		  empty_CAT_list.clear();
		  return empty_CAT_list;
		  break;
  		}
  	    }
  	}
      while ((!m_allTriangles.empty())||(!leftTriangles.empty()));
      if (!polyline.empty())
	{
	  SkelBranchPtr branch = new SkelBranch(polyline, 
						&m_shape, 
						lastEndType, 
						SkelBranch::TERMINATION, 
						bumpBeg, 
						bumpEnd, 
						bumpMid);
	  m_skeleton.push_back(branch);
	  if (lastEndType == SkelBranch::JONCTION)
	    {
	      SkelJonctionSet::iterator it_jonction;
	      it_jonction = m_jonctions.find(new SkelJonction((*polyline.begin())));
	      if (it_jonction != m_jonctions.end())
		{
		  (*it_jonction)->addBranch(branch);
		}
	      else
		{
		  pglError("How strange to get ann end() iterator there ! #3");
		}
	    }
	}
    }
  else
    {
      pglError( "Skeleton, Chordal Axis Transform : Triangulation is empty" );
      std::list<Polyline2DPtr> empty_CAT_list;
      empty_CAT_list.clear();
      return empty_CAT_list;
      assert(!m_allTriangles.empty());
    }
  return SkelBranch::getListPolylines(m_skeleton);
}

void PGL::Skeleton::filterLittleBranchesOnBranchSize(const double sizeMaxBranchesToRemove)
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

void PGL::Skeleton::filterLittleBranchesOnAreaSize(const double areaMaxTrianglesToRemove)
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
		    {
		      pglError( "Skeleton, filterLittleBranchOnAreaSize : "
				"there might be a pb trying to access end iterator ! " );
		      return;
		    } 
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

void PGL::Skeleton::filterLittleBranchesOnBranchAreaSize(const double areaMaxBranchesToRemove)
{
  SkelJonctionSet::iterator it;
  SkelJonctionSet::iterator toErase = m_jonctions.end();
  int i = 0;
  for (it = m_jonctions.begin(); it != m_jonctions.end(); it ++)
    {
      if (toErase != m_jonctions.end())
	{
	  m_jonctions.erase(toErase);
	  toErase = m_jonctions.end();
	}
      for (int i = (*it)->m_nbBranches-1 ; i >= 0 ; --i)
	{
	  SkelBranchPtr branch = (*it)->m_branches[i];
	  if ((branch->m_firstEnd == SkelBranch::TERMINATION)||(branch->m_secondEnd == SkelBranch::TERMINATION))
	    {
	      if (branch->area() < areaMaxBranchesToRemove)
		{
		  // then we must remove that branch, and give its info to the other branches
		  reorganizeBranchesAndJonctionsAfterFiltering((*it)->m_branches, i);
		  m_skeleton.remove((*it)->m_branches[i]);
		  if (i == 0)
		    {
 		      (*it)->m_branches[i] = (*it)->m_branches[i+1];
 		      (*it)->m_branches[i+1] = (*it)->m_branches[i+2];
		      (*it)->m_branches[i+2] = 0;
		    }
		  else if (i == 1)
		    {
 		      (*it)->m_branches[i] = (*it)->m_branches[i+1];
 		      (*it)->m_branches[i+1] = 0;
		    }
		  else if (i == 2)
		    (*it)->m_branches[i] = 0;
		  else
		    pglError ("Skeleton, filterLittleBranchesOnBranchAreaSize, pb of index");
		  (*it)->m_nbBranches --;
		  if ((*it)->m_nbBranches == 0)
		    {
		      toErase = it;
		    }
		}
	    }
	}
    }
}

void PGL::Skeleton::reorganizeBranchesAndJonctionsAfterFiltering(SkelBranchPtr branches[3], int indexBranchToRemove)
{
  if (indexBranchToRemove < 0 && indexBranchToRemove > 2)
    {
      pglError("Error in paramaters of reorganizeBranchesAndJonctionsAfterFiltering : index out of range");
      return;
    }
  else
    {
      int nbBranchesToRemain = 0;
      if (branches[((indexBranchToRemove + 1) % 3)] != 0)
	nbBranchesToRemain ++;
      if (branches[((indexBranchToRemove + 2) % 3)] != 0)
	nbBranchesToRemain ++;
      if (nbBranchesToRemain == 0)
	{
	  pglError("\n ***********************************************************"
		   "\n this scenario should not happen, unless programming mistake"
		   "\n ***********************************************************");
	  return;
	}
      else if (nbBranchesToRemain == 1)
	{
	  if (branches[((indexBranchToRemove + 1) % 3)] == 0)
	    {
	      int indexToModify = ((indexBranchToRemove + 2) % 3);
	      if ( (branches[indexToModify]->m_secondEnd == SkelBranch::TERMINATION)
		  || (branches[indexToModify]->m_firstEnd == SkelBranch::TERMINATION) )
		{
		  // now looking at the indexes bumpBeg and bumpEnd to modify them, including in
		  // the new bump the bump of the suppressed branch. the middle is unchanged
		  int index1 = branches[indexBranchToRemove]->m_beginBumpIndice ;
		  int index2 = branches[indexBranchToRemove]->m_endBumpIndice ;
		  if (index1 == (branches[indexToModify]->m_beginBumpIndice))
		    {
		      branches[indexToModify]->m_beginBumpIndice = index2;
		    }
		  else if (index1 == (branches[indexToModify]->m_endBumpIndice))
		    {
		      branches[indexToModify]->m_endBumpIndice = index2;
		    }
		  else if (index2 == (branches[indexToModify]->m_beginBumpIndice))
		    {
		      branches[indexToModify]->m_beginBumpIndice = index1;
		    }
		  else if (index2 == (branches[indexToModify]->m_endBumpIndice)) 
		    {
		      branches[indexToModify]->m_endBumpIndice = index1;
		    }
		  else
		    {
		      pglError("Mmmmmh... Are you sure that these are neighbouring branches ???");
		    } 
		}
	      else
		{
		  branches[indexToModify]->m_middleBumpIndice 
		    = branches[indexBranchToRemove]->m_middleBumpIndice;
		  int index1 = branches[indexBranchToRemove]->m_beginBumpIndice ;
		  int index2 = branches[indexBranchToRemove]->m_endBumpIndice ;
		  if ( (index1 == branches[indexToModify]->m_beginBumpIndice)
		       || (index1 == branches[indexToModify]->m_endBumpIndice)
		       || (index2 == branches[indexToModify]->m_beginBumpIndice)
		       || (index2 == branches[indexToModify]->m_endBumpIndice) )
		    {
		      branches[indexToModify]->m_beginBumpIndice
			= branches[indexToModify]->m_joncjoncBegInd2;
		      branches[indexToModify]->m_endBumpIndice
			= branches[indexToModify]->m_joncjoncEndInd2;
		      branches[indexToModify]->m_joncjoncBegInd2 = -1;
		      branches[indexToModify]->m_joncjoncEndInd2 = -1;
		      branches[indexToModify]->m_firstEnd = SkelBranch::TERMINATION;
		    }
		  else
		    {
		      branches[indexToModify]->m_joncjoncBegInd2 = -1;
		      branches[indexToModify]->m_joncjoncEndInd2 = -1;
		      branches[indexToModify]->m_secondEnd = SkelBranch::TERMINATION;
		    }
		}
	    }
	  else if (branches[((indexBranchToRemove + 2) % 3)] == 0)
	    {
	      int indexToModify = ((indexBranchToRemove + 1) % 3);
	      if ( (branches[indexToModify]->m_secondEnd == SkelBranch::TERMINATION)
		  || (branches[indexToModify]->m_secondEnd == SkelBranch::TERMINATION) )
		{
		  // now looking at the indexes bumpBeg and bumpEnd to modify them, including in
		  // the new bump the bump of the suppressed branch. the middle is unchanged
		  int index1 = branches[indexBranchToRemove]->m_beginBumpIndice ;
		  int index2 = branches[indexBranchToRemove]->m_endBumpIndice ;
		  if (index1 == (branches[indexToModify]->m_beginBumpIndice))
		    {
		      branches[indexToModify]->m_beginBumpIndice = index2;
		    }
		  else if (index1 == (branches[indexToModify]->m_endBumpIndice))
		    {
		      branches[indexToModify]->m_endBumpIndice = index2;
		    }
		  else if (index2 == (branches[indexToModify]->m_beginBumpIndice))
		    {
		      branches[indexToModify]->m_beginBumpIndice = index1;
		    }
		  else if (index2 == (branches[indexToModify]->m_endBumpIndice)) 
		    {
		      branches[indexToModify]->m_endBumpIndice = index1;
		    }
		  else
		    {
		      pglError("Mmmmmh... Are you sure that these are neighbouring branches ???");
		    } 
		}
	      else
		{
		  branches[indexToModify]->m_middleBumpIndice 
		    = branches[indexBranchToRemove]->m_middleBumpIndice;
		  int index1 = branches[indexBranchToRemove]->m_beginBumpIndice ;
		  int index2 = branches[indexBranchToRemove]->m_endBumpIndice ;
		  if ( (index1 == branches[indexToModify]->m_beginBumpIndice)
		       || (index1 == branches[indexToModify]->m_endBumpIndice)
		       || (index2 == branches[indexToModify]->m_beginBumpIndice)
		       || (index2 == branches[indexToModify]->m_endBumpIndice) )
		    {
		      branches[indexToModify]->m_beginBumpIndice
			= branches[indexToModify]->m_joncjoncBegInd2;
		      branches[indexToModify]->m_endBumpIndice
			= branches[indexToModify]->m_joncjoncEndInd2;
		      branches[indexToModify]->m_joncjoncBegInd2 = -1;
		      branches[indexToModify]->m_joncjoncEndInd2 = -1;
		      branches[indexToModify]->m_firstEnd = SkelBranch::TERMINATION;
		    }
		  else
		    {
		      branches[indexToModify]->m_joncjoncBegInd2 = -1;
		      branches[indexToModify]->m_joncjoncEndInd2 = -1;
		      branches[indexToModify]->m_secondEnd = SkelBranch::TERMINATION;
		    }
		}
	    }
	  else
	    {
	      pglError(" ?????????????????? ok, weird indeed ????????????????????????");
	      return;
	    }
	}
      else if (nbBranchesToRemain == 2)
	{
	  // branches suppressed are only TERMINATION-JONCTION or TERMINATION-TERMINATION ( last case
	  // is very highly improbable, especially as we are linked to a jonction with 2 other branches )
	  if ( (branches[indexBranchToRemove]->m_firstEnd == SkelBranch::TERMINATION) 
	       && (branches[indexBranchToRemove]->m_secondEnd == SkelBranch::TERMINATION) )
	    {
	      pglError("Mmmm, this is difficult to explain ! Programming Error, this means");
	      return;
	    }
	  // first we will look if there is one other jonction-termination
	  if ( (branches[((indexBranchToRemove + 1) % 3)]->m_firstEnd == SkelBranch::TERMINATION) 
	       || (branches[((indexBranchToRemove + 1) % 3)]->m_secondEnd == SkelBranch::TERMINATION) )
	    {
	      // now looking at the indexes bumpBeg and bumpEnd to modify them, including in
	      // the new bump the bump of the suppressed branch. the middle is unchanged
	      int index1 = branches[indexBranchToRemove]->m_beginBumpIndice ;
	      int index2 = branches[indexBranchToRemove]->m_endBumpIndice ;
	      if (index1 == (branches[((indexBranchToRemove + 1) % 3)]->m_beginBumpIndice))
		{
		  branches[((indexBranchToRemove + 1) % 3)]->m_beginBumpIndice = index2;
		}
	      else if (index1 == (branches[((indexBranchToRemove + 1) % 3)]->m_endBumpIndice))
		{
		  branches[((indexBranchToRemove + 1) % 3)]->m_endBumpIndice = index2;
		}
	      else if (index2 == (branches[((indexBranchToRemove + 1) % 3)]->m_beginBumpIndice))
		{
		  branches[((indexBranchToRemove + 1) % 3)]->m_beginBumpIndice = index1;
		}
	      else if (index2 == (branches[((indexBranchToRemove + 1) % 3)]->m_endBumpIndice)) 
		{
		  branches[((indexBranchToRemove + 1) % 3)]->m_endBumpIndice = index1;
		}
	      else
		{
		  pglError("Mmmmmh... Are you sure that these are neighbouring branches ???");
		}
	    }
	  else if ( (branches[((indexBranchToRemove + 2) % 3)]->m_firstEnd == SkelBranch::TERMINATION) 
		    || (branches[((indexBranchToRemove + 2) % 3)]->m_secondEnd == SkelBranch::TERMINATION) )
	    {
	      // now looking at the indexes bumpBeg and bumpEnd to modify them, including in
	      // the new bump the bump of the suppressed branch. the middle is unchanged
	      int index1 = branches[indexBranchToRemove]->m_beginBumpIndice ;
	      int index2 = branches[indexBranchToRemove]->m_endBumpIndice ;
	      if (index1 == (branches[((indexBranchToRemove + 2) % 3)]->m_beginBumpIndice))
		{
		  branches[((indexBranchToRemove + 2) % 3)]->m_beginBumpIndice = index2;
		}
	      else if (index1 == (branches[((indexBranchToRemove + 2) % 3)]->m_endBumpIndice))
		{
		  branches[((indexBranchToRemove + 2) % 3)]->m_endBumpIndice = index2;
		}
	      else if (index2 == (branches[((indexBranchToRemove + 2) % 3)]->m_beginBumpIndice))
		{
		  branches[((indexBranchToRemove + 2) % 3)]->m_beginBumpIndice = index1;
		}
	      else if (index2 == (branches[((indexBranchToRemove + 2) % 3)]->m_endBumpIndice)) 
		{
		  branches[((indexBranchToRemove + 2) % 3)]->m_endBumpIndice = index1;
		}
	      else
		{
		  pglError("Mmmmmh... Are you sure that these are neighbouring branches ???");
		} 
	    }
	  else
	    {
	      // We have two jonction-jonction branches leaving from this jonction
	      // But we do not really care about that case : we will probably not need information
	      // from a branch jonction-jonction
	      // If possible, we should suppress that jonction and make one new branch
	      // But not now, structures do not allow that
	      // Anyway, randomly choosing one branch...
	      int index1 = branches[indexBranchToRemove]->m_beginBumpIndice ;
	      int index2 = branches[indexBranchToRemove]->m_endBumpIndice ;
	      if (index1 == (branches[((indexBranchToRemove + 2) % 3)]->m_beginBumpIndice))
		{
		  branches[((indexBranchToRemove + 2) % 3)]->m_beginBumpIndice = index2;
		}
	      else if (index1 == (branches[((indexBranchToRemove + 2) % 3)]->m_endBumpIndice))
		{
		  branches[((indexBranchToRemove + 2) % 3)]->m_endBumpIndice = index2;
		}
	      else if (index2 == (branches[((indexBranchToRemove + 2) % 3)]->m_beginBumpIndice))
		{
		  branches[((indexBranchToRemove + 2) % 3)]->m_beginBumpIndice = index1;
		}
	      else if (index2 == (branches[((indexBranchToRemove + 2) % 3)]->m_endBumpIndice)) 
		{
		  branches[((indexBranchToRemove + 2) % 3)]->m_endBumpIndice = index1;
		}
	      else
		{
		  pglError("Mmmmmh... Are you sure that these are neighbouring branches ???");
		}
	    }
	}
      else
	{
	  pglError("Interesting... Incrementing twice at best from 0, but still having a number other than 0, 1, 2");
	  return;
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
		Vector2 e1 = (*it)->m_e1->m_p1->m_vec - (*it)->m_e1->m_p2->m_vec ;
		Vector2 e2 = (*it)->m_e1->m_p1->m_vec - (*it)->m_e2->m_p2->m_vec ;
		float cp = e1.x()*e2.y() - e2.x() * e1.y();
		if (cp > 0) 
			i = Index3((*it)->m_e1->m_p1->m_ind, (*it)->m_e1->m_p2->m_ind, (*it)->m_e2->m_p2->m_ind);
		else
			i = Index3((*it)->m_e1->m_p1->m_ind, (*it)->m_e2->m_p2->m_ind, (*it)->m_e1->m_p2->m_ind);
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

#ifdef WITH_CGAL
typedef std::pair<Point_intersections,int> Pts_counter;
static bool operator<(const Pts_counter p1, const Pts_counter p2)
{
  return (p1.first < p2.first);
};

class Compare_Pts_counter
{
public:
  bool operator()(const Pts_counter p1, const Pts_counter p2) const
  {
    return p1.first < p2.first;
  }
};
#endif

bool isInside(Vector2 p, Vector2 seg_src, Vector2 seg_tgt)
{
  Vector2 seg = seg_tgt - seg_src;
  Vector2 toTest = p - seg_src;
  if ((p == seg_src) || (p == seg_tgt))
    return false;
  // determinant : if not null vectors are not colinear,
  // and p is not within seg_src and seg_tgt
  double determinant = seg.x()*toTest.y() - seg.y()*toTest.x();
  if ((determinant > GEOM_TOLERANCE) || (determinant < -GEOM_TOLERANCE))
    return false;
  if ((toTest.x()*seg.x()+toTest.y()*seg.y()) < 0)
    return false;
  toTest = seg_tgt - p;
  if ((toTest.x()*seg.x()+toTest.y()*seg.y()) < 0)
    return false;
  return true;
}


#ifdef WITH_CGAL
Polyline2DPtr removeOneLoop(std::list<Vector2> polyline, std::list<Point_intersections> pts_intersection)
{
  if (pts_intersection.size() == 0)
    {
      std::vector<Vector2> v_polyline; 
      v_polyline.assign(polyline.begin(), polyline.end());
      return new Polyline2D(new Point2Array(v_polyline.begin(),v_polyline.end()));
    }
  bool found = false;
  bool new_point = false;
  std::list<Vector2>::iterator polyline_it, it1 = polyline.end(), it2 = polyline.end();
  std::list<Point_intersections>::iterator it, it_begin, it_end;
  it_begin = pts_intersection.begin();
  it_end = pts_intersection.end();
  // we find the place within the polyline where are the intersections points
  // to get the indices for both segments giving the intersection
  // and the type of the intersection : new point or polyline point
  for (polyline_it = polyline.begin(); (polyline_it != polyline.end()) && (!found); polyline_it++)
    {
      for (it = it_begin ; it != it_end ; it++)
	{
	  if (*polyline_it == toVec2(*it))
	    {
	      new_point = false;
	      if (it1 == polyline.end())
		{
		  it1 = polyline_it;
		  it_begin = it;
		  it_end = it;
		  it_end++;
		}
	      else if ((it2 == polyline.end()) && (it1 != polyline_it))
		it2 = polyline_it;
	      found == (it1 != polyline.end()) && (it2 != polyline.end());
	    }
	  else
	    {
	      std::list<Vector2>::iterator polyline_suiv = polyline_it;
	      polyline_suiv ++;
	      if (polyline_suiv == polyline.end())
		polyline_suiv = polyline.begin();
	      if (isInside(toVec2(*it), *polyline_it, *polyline_suiv))
		{
		  new_point = true;
		  // insertion of intersection point in the polyline
		  polyline.insert(polyline_suiv, toVec2(*it));
		  if (it1 == polyline.end())
		    {
		      it1 = polyline_it;
		      it1++;
		      it_begin = it;
		      it_end = it;
		      it_end++;
		    }
		  else if ((it2 == polyline.end()) && (it1 != polyline_it))
		    {
		      it2 = polyline_it;
		      it2++;
		    }
		  found == (it1 != polyline.end()) && (it2 != polyline.end());
		}
	    }
	}
    }

  pts_intersection.erase(it_begin);

  // Now we have an intersection point and the two segments intersecting it
  // We must therefore calculate the lengths of the 2 loops departing from 
  // the intersection point
  double length1 = 0.0, length2 = 0.0;
  bool shorterLoopFound = false;
  bool loop1Completed = false;
  bool loop2Completed = false;
  bool loop2includesEnd = false;
  std::list<Vector2>::iterator it1_save = it1, it2_save = it2;
  while(!shorterLoopFound)
    {
      if (!loop1Completed)
	{
	  std::list<Vector2>::iterator iti_prec = it1;
	  it1++;
	  length1 += norm(*it1 - *iti_prec);
	  if (it1 == it2_save)
	    loop1Completed = true;
	}
      if (!loop2Completed)
	{
	  std::list<Vector2>::iterator iti_prec = it2;
	  it2++;
	  if (it2 == polyline.end())
	    {
	      it2 = polyline.begin();
	      loop2includesEnd = true;
	    }
	  length2 += norm(*it2 - *iti_prec);
	  if (it2 == it1_save)
	    loop2Completed = true;
	}
      if (loop1Completed && (length1 < length2))
	shorterLoopFound = true;
      if (loop2Completed && (length2 < length1))
	shorterLoopFound = true;
      if (loop1Completed && loop2Completed)
	shorterLoopFound = true;
    }

  // Now we know which loop is the shortest
  // And we have to suppress it
  if (length1 <= length2)
    {
      polyline.erase(it1_save, it1);
    }
  else
    {
      if (loop2includesEnd)
	{
	  polyline.erase(it2_save, polyline.end());
	  polyline.erase(polyline.begin(), it2);
	}
      else 
	{
	  polyline.erase(it2_save, it2);
	}
    }
  return removeOneLoop(polyline, pts_intersection);
}
#endif

Polyline2DPtr PGL::Skeleton::removeLoopsInShape(Polyline2DPtr shape)
{
#ifdef WITH_CGAL
  std::list<Point_intersections> pts_intersection;
  std::list<Segment_intersections> pts_shape;
  std::list<Vector2> polyline; 
  std::set<Pts_counter, Compare_Pts_counter> pts;
  Point_intersections p_last = toPoint_intersections(shape->getPointListAt(0)), p_cur;
  polyline.push_back(shape->getPointListAt(0));
  pts.insert(std::pair<Point_intersections,int>(p_last,1));
  int n_max = shape->getPointListSize();
  if (shape->getPointListAt(0) == shape->getPointListAt(n_max-1))
    n_max--;
  int nb_intersections = 0;
  for (int i = 1; i < n_max; i++)
    {
      polyline.push_back(shape->getPointListAt(i));
      p_cur = toPoint_intersections(shape->getPointListAt(i));
      pts_shape.push_back(Segment_intersections(p_last, p_cur));
      std::pair<std::set<Pts_counter, Compare_Pts_counter>::iterator,bool> res = pts.insert(std::pair<Point_intersections,int>(p_cur,i));
      if (!res.second)
	{
	  // we have an intersection point
	  // we may suppress at once one loop
	  pts_intersection.push_back((*(res.first)).first);
	}
      p_last = p_cur;
    }

  if (toPoint_intersections(shape->getPointListAt(0)) != p_last)
    pts_shape.push_back(Segment_intersections(p_last,toPoint_intersections(shape->getPointListAt(0))));

  //std::list<Segment_intersections>::iterator return_it = 
#if CGAL_VERSION_NR >= 1030401000
  CGAL::compute_intersection_points
#else
  CGAL::get_intersection_points
#endif
	  (pts_shape.begin(), pts_shape.end(), std::back_inserter (pts_intersection));

  return removeOneLoop(polyline, pts_intersection);
#else
  return shape;
#endif
}



std::list<Polyline2DPtr> 
PGL::Skeleton::getChordalAxisTransform
(const Polyline2DPtr discretizedShape, const double areaMaxFilter)
{
  filterAndHomogenize(*(discretizedShape->getPointList()));
  Skeleton skel(discretizedShape);
  skel.getChordalAxisTransform();
  skel.filterLittleBranchesOnBranchAreaSize(areaMaxFilter);
  return SkelBranch::getListPolylines(skel.m_skeleton);
}

std::list<Polyline2DPtr> 
PGL::Skeleton::getSkeletonInformation(const Polyline2DPtr discretizedShape, 
				      const double areaMaxFilter,
				      std::list<Vector2> * ends,
				      std::list<Vector2> * end_tgts,
				      std::list<Vector2> * bump_ends,
				      std::list<Vector2> * bump_tgts,
				      std::list<Polyline2DPtr> * bumps)
{
  filterAndHomogenize(*(discretizedShape->getPointList()));
  Skeleton skel(discretizedShape);
  skel.getChordalAxisTransform();
  skel.filterLittleBranchesOnBranchAreaSize(areaMaxFilter);
  // here we must get back information on skeleton
  std::list<SkelBranchPtr>::iterator it;
  for (it = skel.m_skeleton.begin(); it != skel.m_skeleton.end(); it ++)
    {
      int pol_branch_size = (*it)->m_branch->getPointListSize();
      if (pol_branch_size < 2)
	continue;
      if ((*it)->m_firstEnd == SkelBranch::TERMINATION)
	{
	  ends->push_back((*it)->m_branch->getPointListAt(0));
	  end_tgts->push_back((*it)->m_branch->getPointListAt(1)-(*it)->m_branch->getPointListAt(0));
	  if ((*it)->m_secondEnd == SkelBranch::TERMINATION)
	    {
	      ends->push_back((*it)->m_branch->getPointListAt((*it)->m_branch->getPointListSize() - 1));
	      end_tgts->push_back((*it)->m_branch->getPointListAt((*it)->m_branch->getPointListSize() - 2)
				  - (*it)->m_branch->getPointListAt((*it)->m_branch->getPointListSize() - 1));
	    }
	}
      else if ((*it)->m_secondEnd == SkelBranch::TERMINATION)
	{
	  ends->push_back((*it)->m_branch->getPointListAt((*it)->m_branch->getPointListSize() - 1));
	  end_tgts->push_back((*it)->m_branch->getPointListAt((*it)->m_branch->getPointListSize() - 2)
			     - (*it)->m_branch->getPointListAt((*it)->m_branch->getPointListSize() - 1));
	}
      else
	continue;
      int shape_size = skel.m_shape.size();
      bump_ends->push_back((*(skel.m_shape.findInd((*it)->m_middleBumpIndice)))->m_vec);
      if ((*it)->m_middleBumpIndice == 0)
	{
	  bump_tgts->push_back((*(skel.m_shape.findInd(shape_size - 1)))->m_vec 
			       - (*(skel.m_shape.findInd((*it)->m_middleBumpIndice + 1)))->m_vec);
	}
      else if ((*it)->m_middleBumpIndice == (shape_size - 1))
	{
	  bump_tgts->push_back((*(skel.m_shape.findInd((*it)->m_middleBumpIndice - 1)))->m_vec 
			       - (*(skel.m_shape.findInd(0)))->m_vec);
	}
      else
	{
	  bump_tgts->push_back((*(skel.m_shape.findInd((*it)->m_middleBumpIndice - 1)))->m_vec 
			       - (*(skel.m_shape.findInd((*it)->m_middleBumpIndice + 1)))->m_vec);
	}
      bumps->push_back((*it)->getPolylineForAssociatedBump());
    }
  return SkelBranch::getListPolylines(skel.m_skeleton);
}

TriangleSetPtr PGL::Skeleton::getDelaunayConstrained2DTriangulation(const Polyline2DPtr discretizedShape)
{
  filterAndHomogenize(*(discretizedShape->getPointList()));
#ifdef WITH_CGAL
  removeLoopsInShape(discretizedShape);
#endif
  Skeleton skel(discretizedShape);
  return skel.getTriangleSet();
}
