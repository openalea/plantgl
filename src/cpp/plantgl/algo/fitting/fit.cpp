/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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

#include "fit.h"

#include <plantgl/algo/codec/printer.h>
#include <plantgl/algo/base/discretizer.h>

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


#define GEOM_DEBUG


/* ----------------------------------------------------------------------- */

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace std;
using namespace STDEXT;

/* ----------------------------------------------------------------------- */

Fit::Fit():
    __pointstofit(),
    __radius(),
    __default_crossSection(),
    __optimization(0)
{
}

Fit::Fit(const Point3ArrayPtr& _pointstofit ):
    __pointstofit(_pointstofit),
    __radius(),
    __default_crossSection(),
    __optimization(0)
{
}

/* ----------------------------------------------------------------------- */


GeometryPtr Fit::use(const string& classname){
    if(! __pointstofit )return GeometryPtr();
	string cl = toUpper(classname);
#ifndef WITHOUT_QHULL
    if(cl =="CONVEXHULL")
	return convexHull();
	else 
#endif
	if(cl =="EXTRUDEDHULL")
	return extrudedHull();
    else if(cl =="ASYMMETRICHULL")
	return asymmetricHull();
    else if(cl =="EXTRUSION")
	return extrusion();
    else if(cl =="SPHERE")
	return sphere();
    else if(cl =="ASPHERE")
	return asphere();
    else if(cl =="BSPHERE")
	return bsphere();
    else if(cl =="CYLINDER")
	return cylinder();
    else if(cl =="ACYLINDER")
	return acylinder();
    else if(cl =="BCYLINDER")
	return bcylinder();
    else if(cl =="ELLIPSOID")
	return ellipsoid();
    else if(cl =="BELLIPSOID2")
	return ellipsoid2();
    else if(cl =="AELLIPSOID")
	return aellipsoid();
    else if(cl =="BELLIPSOID")
	return bellipsoid();
    else if(cl =="AALIGNEDBOX")
	return aalignedbox();
    else if(cl =="BALIGNEDBOX")
	return balignedbox();
    else if(cl =="BOX")
	return box();
    else if(cl =="ABOX")
	return abox();
    else if(cl =="BBOX")
	return bbox();
    else if(cl =="NONE")
	return GeometryPtr();
    else return convexHull();
}

vector<string> Fit::getVolumeClassNames(){
    vector<string> classname;
#ifndef WITHOUT_QHULL
	classname.push_back(string("CONVEXHULL"));
#endif
	classname.push_back(string("ASYMMETRICHULL"));
	classname.push_back(string("EXTRUDEDHULL"));
	classname.push_back(string("SPHERE"));
	classname.push_back(string("ASPHERE"));
	classname.push_back(string("BSPHERE"));
	classname.push_back(string("CYLINDER"));
	classname.push_back(string("ACYLINDER"));
	classname.push_back(string("BCYLINDER"));
	classname.push_back(string("ELLIPSOID"));
//	classname.push_back(string("BELLIPSOID2"));
	classname.push_back(string("AELLIPSOID"));
	classname.push_back(string("BELLIPSOID"));
	classname.push_back(string("AALIGNEDBOX"));
	classname.push_back(string("BALIGNEDBOX"));
	classname.push_back(string("BOX"));
	classname.push_back(string("ABOX"));
	classname.push_back(string("BBOX"));
    return classname;
}

vector<string> Fit::getClassNames(){
    vector<string> classname = Fit::getVolumeClassNames();
	classname.push_back(string("EXTRUSION"));
    return classname;
}

bool Fit::available(const std::string& name){
	string cl = toUpper(name);
    vector<string> classname = Fit::getClassNames();
	for(vector<string>::const_iterator _it = classname.begin();
		_it != classname.end();_it++)
		  if(*_it == cl)return true;
	return false;
}

bool Fit::areRadiusNeeded(string classname){
    return (classname == "extrusion" || classname =="EXTRUSION" 
/*	    || classname == "cylinder" || classname =="CYLINDER"
	    || classname == "bcylinder" || classname =="BCYLINDER"*/ );
}

/* ----------------------------------------------------------------------- */

/// compute inertia axis
bool Fit::inertiaAxis(const Point3ArrayPtr& points, 
					  Vector3& u, Vector3& v, Vector3& w, Vector3& s)
{
  if(points->size()>2){
	float vpvec[3][3];
	float vpint[3] = {0,0,0};
	float vpang[3][3];
	short vpok[3];
	Laxi_ComputeInertia (points,vpvec,vpint,vpang,vpok);
	if(vpok[2]!=0  && vpok[1]!=0 && vpok[0]!=0 && vpint[2]!=0 && vpint[1]!=0 &&  vpint[0]!=0 ){ 
	    u = Vector3(vpvec[0][0]/vpint[0],vpvec[0][1]/vpint[0],vpvec[0][2]/vpint[0]);
	    v = Vector3(vpvec[1][0]/vpint[1],vpvec[1][1]/vpint[1],vpvec[1][2]/vpint[1]);
	    w = Vector3(vpvec[2][0]/vpint[2],vpvec[2][1]/vpint[2],vpvec[2][2]/vpint[2]);
	    s = Vector3(vpint[0],vpint[1],vpint[2]);
		return true;
	}
	else return false;
  }
  return false;
}

bool Fit::inertiaAxis(const Point2ArrayPtr& points, 
						    TOOLS(Vector2)& u, TOOLS(Vector2)& v, TOOLS(Vector2)& s)
{
  if(points->size()>2){
	float vpvec[3][3];
	float vpint[3] = {0,0,0};
	float vpang[3][3];
	short vpok[3];
	Point3ArrayPtr npoints = Point3ArrayPtr(new Point3Array(*points,0));
	Laxi_ComputeInertia (npoints,vpvec,vpint,vpang,vpok);
	if(vpok[2]!=0  && vpok[1]!=0 && vpint[2]!=0 && vpint[1]!=0  ){ 
	    u = Vector2(vpvec[1][0]/vpint[1],vpvec[1][1]/vpint[1]);
	    v = Vector2(vpvec[2][0]/vpint[2],vpvec[2][1]/vpint[2]);
	    s = Vector2(vpint[1],vpint[2]);
		return true;
	}
	else return false;
  }
  return false;
}



GeometryPtr Fit::sphere(){
  if(! __pointstofit )return GeometryPtr();
  if(__pointstofit->size()>2){
	float vpvec[3][3];
	float vpint[3] = {0,0,0};
	float vpang[3][3];
	short vpok[3];
	Vector3 u,v,w,s;
	Laxi_ComputeInertia (__pointstofit,vpvec,vpint,vpang,vpok);
	if(vpok[2]!=0  && vpok[1]!=0 && vpok[0]!=0 && vpint[2]!=0 && vpint[1]!=0 &&  vpint[0]!=0 ){ 
	    w = Vector3(vpvec[2][0],vpvec[2][1],vpvec[2][2]);
	    v = Vector3(vpvec[1][0]/vpint[1],vpvec[1][1]/vpint[1],vpvec[1][2]/vpint[1]);
	    u = Vector3(vpvec[0][0]/vpint[0],vpvec[0][1]/vpint[0],vpvec[0][2]/vpint[0]);
	    s = Vector3(vpint[0],vpint[1],vpint[2]);
	}
	else return GeometryPtr();
	Vector3 _center = __pointstofit->getCenter();
	real_t radius = normL1(s)/3;
	if(radius > GEOM_EPSILON){
	  GeometryPtr geom(new Sphere(radius,32,32));
	  if(fabs(*(_center.getMax())) > GEOM_EPSILON)
		geom = GeometryPtr(new Translated(_center,geom));
	  return geom;
    }
  }
  return GeometryPtr();
}

/* ----------------------------------------------------------------------- */

GeometryPtr Fit::asphere(){
    if(! __pointstofit )return GeometryPtr();
    Vector3 _center = __pointstofit->getCenter();
    real_t _radius = 0.0;
    for(Point3Array::iterator _it = __pointstofit->begin();
	_it != __pointstofit->end();
	_it++)_radius += norm(*_it-_center);
    _radius /= __pointstofit->size();
    if(_radius != 0){
	if(_center.x() > GEOM_EPSILON || _center.y() > GEOM_EPSILON ||_center.z() > GEOM_EPSILON  ){
	    return GeometryPtr(
		new Translated(
		_center, 
		GeometryPtr(new Sphere(
		    _radius,32,32))));
	}
	else {
	    return GeometryPtr(
		new Sphere(
		    _radius,32,32));
	}
    }
    else
        return GeometryPtr();
}


/* ----------------------------------------------------------------------- */
// E. Welzl. Smallest enclosing disks (balls and ellipsoids). 
// In H. Maurer, editor, New
// Results and New Trends in Computer Science, volume 555 
// of Lecture Notes in Computer Science, pages 359-370. Springer Verlag, 1991.

GeometryPtr Fit::bsphere(){
    if(! __pointstofit || __pointstofit->size() < 2)return GeometryPtr();
	Miniball ball;
	ball.check_in(__pointstofit);
	ball.build();
	Vector3 _center = ball.center();
    real_t _radius = sqrt(ball.squared_radius());
    if(_radius != 0){
	  if(_center.x() > GEOM_EPSILON || _center.y() > GEOM_EPSILON ||_center.z() > GEOM_EPSILON  ){
		return GeometryPtr(
		  new Translated(
		  _center, 
		  GeometryPtr(new Sphere(
		  _radius,32,32))));
	  }
	  else {
		return GeometryPtr(
		  new Sphere(
		  _radius,32,32));
	  }
    }
    else
	  return GeometryPtr();
}

/* ----------------------------------------------------------------------- */

#ifdef WITH_CGAL
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#endif
/*!
  Fit the 2D points \e points with a bounding circle.
 */
bool Fit::boundingCircle(const Point2ArrayPtr& _points, Vector2& center, real_t& radius)
{
#ifdef WITH_CGAL

	// typedefs
	typedef  CGAL::Exact_predicates_exact_constructions_kernel K;
	typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
	typedef  CGAL::Min_circle_2<Traits>      Min_circle;
	typedef  Min_circle::Circle				Circle;

	typedef  K::Point_2                      Point;

	std::vector<Point> pointlist(_points->size());
	std::vector<Point>::iterator it2 = pointlist.begin();
	for(Point2Array::const_iterator it = _points->begin(); it != _points->end(); ++it, ++it2)
		*it2 = Point(it->x(),it->y());

    Min_circle  mc( pointlist.begin(), pointlist.end(), true); 
	if (mc.is_degenerate()) return false;
	Circle circle = mc.circle();
    center = Vector2(to_double(circle.center()[0]),to_double(circle.center()[1]));
	radius = sqrt(to_double(circle.squared_radius()));

    return true;
#else
	return false;
#endif

}

/* ----------------------------------------------------------------------- */

GeometryPtr Fit::cylinder(){
    if(! __pointstofit )return GeometryPtr();
    if(__pointstofit->size()>2){
	float vpvec[3][3];
	float vpint[3] = {0,0,0};
	float vpang[3][3];
	short vpok[3];
	Vector3 u,v,w;
	Laxi_ComputeInertia (__pointstofit,vpvec,vpint,vpang,vpok);
	if(vpok[2]!=0  && vpint[2]!=0){ 
	  u = Vector3(vpvec[2][0],vpvec[2][1],vpvec[2][2]);
	  if(vpok[1]!=0)
		v = Vector3(vpvec[1][0]/vpint[1],
					vpvec[1][1]/vpint[1],
					vpvec[1][2]/vpint[1]);
	  else {
		v = cross(u,Vector3::OZ);
		if(v == Vector3::ORIGIN)
		  v = Vector3::OX;
		else
		  v.normalize();
	  }
	  if(vpok[0]!=0)
		w = Vector3(vpvec[0][0]/vpint[0],
					vpvec[0][1]/vpint[0],
					vpvec[0][2]/vpint[0]);
	  else {
		w = cross (u,v);
		if(w == Vector3::ORIGIN)
		  w = Vector3::OY;
		else
		  w.normalize();
	  }
	}
	else return GeometryPtr();
	Vector3 _center = __pointstofit->getCenter();
	real_t radius= max(vpint[1],vpint[0]);
	if(radius)return GeometryPtr(new Translated((_center-u),
					  GeometryPtr(new Oriented(v,w,
					   GeometryPtr(new Cylinder(radius,norm(u)*2,true,32))))));
    }
    else if(__pointstofit->size() == 2){
	return frustum();
    }
    return GeometryPtr();
}

/* ----------------------------------------------------------------------- */

GeometryPtr Fit::acylinder(){
    if(! __pointstofit )return GeometryPtr();
    if(__pointstofit->size()>2){
	float vpvec[3][3];
	float vpint[3] = {0,0,0};
	float vpang[3][3];
	short vpok[3];
	Vector3 u,v,w;
	Laxi_ComputeInertia (__pointstofit,vpvec,vpint,vpang,vpok);
	if(vpok[2]!=0  && vpint[2]!=0){ 
	    u = Vector3(vpvec[2][0],vpvec[2][1],vpvec[2][2]);
		u.normalize();
	    if(vpok[1]!=0)v = Vector3(vpvec[1][0]/vpint[1],vpvec[1][1]/vpint[1],vpvec[1][2]/vpint[1]);
	    else {
		v = cross(u,Vector3::OZ);
		if(v == Vector3::ORIGIN)
		    v = Vector3::OX;
		else
		    v.normalize();
	    }
	    if(vpok[0]!=0)w = Vector3(vpvec[0][0]/vpint[0],vpvec[0][1]/vpint[0],vpvec[0][2]/vpint[0]);
	    else {
		w = cross (u,v);
		if(w == Vector3::ORIGIN)
		    w = Vector3::OY;
		else
		    w.normalize();
	    }
	}
	else return GeometryPtr();
	Vector3 _center = __pointstofit->getCenter();
	real_t radius=0;
	real_t height=0;
	for(Point3Array::iterator _it = __pointstofit->begin();_it != __pointstofit->end();_it++){
	    Vector3 l = *_it - _center;
	    real_t a = dot(l,v);
	    real_t b = dot(l,w);
	    a = sqrt(a*a +b*b);
	    radius += a;
		real_t c = fabs(dot(l,u));
		height += c;
	}
	radius /= __pointstofit->size();
	height /= __pointstofit->size();
	if(radius)return GeometryPtr(new Translated((_center-u*(height)),
					  GeometryPtr(new Oriented(v,w,
					   GeometryPtr(new Cylinder(radius,height*2,true,32))))));
    }
    else if(__pointstofit->size() == 2){
        return frustum();
    }
    return GeometryPtr();
}


/* ----------------------------------------------------------------------- */



GeometryPtr Fit::bcylinder(){
    if(! __pointstofit )return GeometryPtr();
    if(__pointstofit->size()>2){
	ExplicitModelPtr result = dynamic_pointer_cast<ExplicitModel>(convexHull());
	Point3ArrayPtr pts;
	if(!result) pts = __pointstofit;
	else pts = result->getPointList();
	float vpvec[3][3];
	float vpint[3] = {0,0,0};
	float vpang[3][3];
	short vpok[3];
	Vector3 u,v,w;
	Laxi_ComputeInertia (pts,vpvec,vpint,vpang,vpok);
	if(vpok[2]!=0  && vpint[2]!=0){ 
	    u = Vector3(vpvec[2][0],vpvec[2][1],vpvec[2][2]);
	    u.normalize();
	    if(vpok[1]!=0)
		  v = Vector3(vpvec[1][0]/vpint[1],
					  vpvec[1][1]/vpint[1],
					  vpvec[1][2]/vpint[1]);
		else {
		  v = cross(u,Vector3::OZ);
		  if(v == Vector3::ORIGIN)v = Vector3::OX;
		  else  v.normalize();
		}
	    if(vpok[0]!=0)
		  w = Vector3(vpvec[0][0]/vpint[0],
					  vpvec[0][1]/vpint[0],
					  vpvec[0][2]/vpint[0]);
		else {
		  w = cross (u,v);
		  if(w == Vector3::ORIGIN)w = Vector3::OY;
		  else w.normalize();
		}
	}
	else return GeometryPtr();

	const Vector3& first = pts->getAt(0);
	real_t zi = dot(first,u);
	real_t zmax  = zi;
	real_t zmin = zi;
	Point3ArrayPtr pts2(new Point3Array(pts->size()));
	Point3Array::iterator _it2 = pts2->begin();
	for(Point3Array::const_iterator _it1 = pts->begin();
		_it1 != pts->end();_it1++){
	    const Vector3& l = *_it1 ;
		*_it2 = Vector3(dot(l,v),dot(l,w),0);_it2++;
	    real_t c = dot(l,u);
		if(zmax < c)zmax = c;
		if(zmin > c)zmin = c;
	}
	Miniball ball;
	ball.check_in(pts2);
	ball.build();
	Vector3 _center = ball.center();
    real_t radius = sqrt(ball.squared_radius());
	_center.z() =(zmax+zmin)/2;
	_center = v*_center.x()+w*_center.y()+u*_center.z();
	real_t height = zmax - zmin;
	if(radius)
	  return GeometryPtr(new Translated((_center-u*(height/2)),
			  GeometryPtr(new Oriented(v,w,
				GeometryPtr(new Cylinder(radius,height,true,32))))));
    }
    else if(__pointstofit->size() == 2){
        return frustum();
    }
    return GeometryPtr();
}


/* ----------------------------------------------------------------------- */


GeometryPtr Fit::ellipsoid(){
  if(! __pointstofit )return GeometryPtr();
  if(__pointstofit->size()>2){
	float vpvec[3][3];
	float vpint[3] = {0,0,0};
	float vpang[3][3];
	short vpok[3];
	Vector3 u,v,w,s;
	Laxi_ComputeInertia (__pointstofit,vpvec,vpint,vpang,vpok);
	if(vpok[2]!=0  && vpok[1]!=0 && vpok[0]!=0 && vpint[2]!=0 && vpint[1]!=0 &&  vpint[0]!=0 ){ 
	    u = Vector3(vpvec[0][0]/vpint[0],vpvec[0][1]/vpint[0],vpvec[0][2]/vpint[0]);
	    v = Vector3(vpvec[1][0]/vpint[1],vpvec[1][1]/vpint[1],vpvec[1][2]/vpint[1]);
	    w = Vector3(vpvec[2][0],vpvec[2][1],vpvec[2][2]);
	    s = Vector3(vpint[0],vpint[1],vpint[2]);
	}
	else return GeometryPtr();
	Vector3 _center = __pointstofit->getCenter();
	if(fabs(*(s.getMin())) > GEOM_EPSILON){
	  GeometryPtr geom(new Sphere(1,32,32));
	  if(fabs(*((s - Vector3(1,1,1)).getMax())) > GEOM_EPSILON)
		geom = GeometryPtr(new Scaled(s,geom));
	  if(!(u == Oriented::DEFAULT_PRIMARY) && !(v == Oriented::DEFAULT_SECONDARY))
		geom = GeometryPtr(new Oriented(u,v,geom));
	  if(fabs(*(_center.getMax())) > GEOM_EPSILON)
		geom = GeometryPtr(new Translated(_center,geom));
	  return geom;
    }
  }
  return GeometryPtr();
}

/* ----------------------------------------------------------------------- */

GeometryPtr Fit::ellipsoid2(){
  if(! __pointstofit )return GeometryPtr();
  if(__pointstofit->size()>2){
	ExplicitModelPtr result = dynamic_pointer_cast<ExplicitModel>(convexHull());
	Point3ArrayPtr pts;
	if(!result) pts = __pointstofit;
	else pts = result->getPointList();
	float vpvec[3][3];
	float vpint[3] = {0,0,0};
	float vpang[3][3];
	short vpok[3];
	Vector3 u,v,w,s;
	Laxi_ComputeInertia (pts,vpvec,vpint,vpang,vpok);
	if(vpok[2]!=0  && vpok[1]!=0 && vpok[0]!=0 && vpint[2]!=0 && vpint[1]!=0 &&  vpint[0]!=0 ){ 
	    u = Vector3(vpvec[0][0]/vpint[0],vpvec[0][1]/vpint[0],vpvec[0][2]/vpint[0]);
	    v = Vector3(vpvec[1][0]/vpint[1],vpvec[1][1]/vpint[1],vpvec[1][2]/vpint[1]);
	    w = Vector3(vpvec[2][0],vpvec[2][1],vpvec[2][2]);
	    s = Vector3(vpint[0],vpint[1],vpint[2]);
	}
	else return GeometryPtr();
	Vector3 _center = pts->getCenter();
	if(fabs(*(s.getMin())) > GEOM_EPSILON){
	  GeometryPtr geom(new Sphere(1,32,32));
	  if(fabs(*((s - Vector3(1,1,1)).getMax())) > GEOM_EPSILON)
		geom = GeometryPtr(new Scaled(s,geom));
	  if(!(u == Oriented::DEFAULT_PRIMARY) && !(v == Oriented::DEFAULT_SECONDARY))
		geom = GeometryPtr(new Oriented(u,v,geom));
	  if(fabs(*(_center.getMax())) > GEOM_EPSILON)
		geom = GeometryPtr(new Translated(_center,geom));
	  return geom;
    }
  }
  return GeometryPtr();
}

/* ----------------------------------------------------------------------- */

GeometryPtr Fit::aellipsoid(){
  if(! __pointstofit )return GeometryPtr();
  if(__pointstofit->size()>2){
	float vpvec[3][3];
	float vpint[3] = {0,0,0};
	float vpang[3][3];
	short vpok[3];
	Vector3 u,v,w,s;
	Laxi_ComputeInertia (__pointstofit,vpvec,vpint,vpang,vpok);
	if(vpok[2]!=0  && vpok[1]!=0 && vpok[0]!=0 && vpint[2]!=0 && vpint[1]!=0 &&  vpint[0]!=0 ){ 
	    u = Vector3(vpvec[0][0]/vpint[0],vpvec[0][1]/vpint[0],vpvec[0][2]/vpint[0]);
	    v = Vector3(vpvec[1][0]/vpint[1],vpvec[1][1]/vpint[1],vpvec[1][2]/vpint[1]);
	    w = Vector3(vpvec[2][0]/vpint[2],vpvec[2][1]/vpint[2],vpvec[2][2]/vpint[2]);
	    s = Vector3(vpint[0],vpint[1],vpint[2]);
		u.normalize();v.normalize();w.normalize();
	}
	else return GeometryPtr();
	Vector3 _center = __pointstofit->getCenter();
	s = Vector3(0,0,0);
	for(Point3Array::iterator _it = __pointstofit->begin();_it != __pointstofit->end();_it++){
	    Vector3 l = *_it - _center;
	    real_t a = fabs(dot(l,u));
	    real_t b = fabs(dot(l,v));
	    real_t c = fabs(dot(l,w));
		s.x()+=a;s.y()+=b;s.z()+=c;
	}
	s/=__pointstofit->size();
	if(fabs(*(s.getMin())) > GEOM_EPSILON){
	  GeometryPtr geom(new Sphere(1,32,32));
	  if(fabs(*((s - Vector3(1,1,1)).getMax())) > GEOM_EPSILON)
		geom = GeometryPtr(new Scaled(s,geom));
	  if(!(u == Oriented::DEFAULT_PRIMARY) && !(v == Oriented::DEFAULT_SECONDARY))
		geom = GeometryPtr(new Oriented(u,v,geom));
	  if(fabs(*(_center.getMax())) > GEOM_EPSILON)
		geom = GeometryPtr(new Translated(_center,geom));
	  return geom;
    }
  }
  return GeometryPtr();
}


/* ----------------------------------------------------------------------- */



#ifdef WITH_CGAL
#include <CGAL/Cartesian_d.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Approximate_min_ellipsoid_d.h>
#include <CGAL/Approximate_min_ellipsoid_d_traits_d.h>
#include <CGAL/Approximate_min_ellipsoid_d_traits_3.h>
#endif

GeometryPtr Fit::bellipsoid(){
  if(! __pointstofit )return GeometryPtr();
  if(__pointstofit->size()>2){
#ifndef WITH_CGAL
	ExplicitModelPtr result = dynamic_pointer_cast<ExplicitModel>(convexHull());
	Point3ArrayPtr pts;
	if(!result) pts = __pointstofit;
	else pts = result->getPointList();
	float vpvec[3][3];
	float vpint[3] = {0,0,0};
	float vpang[3][3];
	short vpok[3];
	Vector3 u,v,w,s(0,0,0);
	Laxi_ComputeInertia (pts,vpvec,vpint,vpang,vpok);
	if(vpok[2]!=0  && vpok[1]!=0 && vpok[0]!=0 && vpint[2]!=0 && vpint[1]!=0 &&  vpint[0]!=0 ){ 
	    u = Vector3(vpvec[0][0]/vpint[0],vpvec[0][1]/vpint[0],vpvec[0][2]/vpint[0]);
	    v = Vector3(vpvec[1][0]/vpint[1],vpvec[1][1]/vpint[1],vpvec[1][2]/vpint[1]);
	    w = Vector3(vpvec[2][0]/vpint[2],vpvec[2][1]/vpint[2],vpvec[2][2]/vpint[2]);
	    s = Vector3(sq(vpint[0]),sq(vpint[1]),sq(vpint[2]));
	}
	else return GeometryPtr();
	u.normalize(); v.normalize(); w.normalize();
	if(!v.isOrthogonalTo(w)){
	  Vector3 w2 = cross(u,v);
	  real_t f = dot(w2,w);
	  s.z() *= sq(f);
	  w = w2;

	}
	Vector3 _center = pts->getCenter();
	real_t vmax = 1;
	for(Point3Array::iterator _it = pts->begin();
		  _it != pts->end();_it++){
	    Vector3 cl = (*_it) - _center;
		cl = Vector3(dot(cl,u),dot(cl,v),dot(cl,w));
		Vector3 test = _center+u*cl.x()+v*cl.y()+w*cl.z();
		real_t error = norm(*_it - test);
		assert(error < GEOM_EPSILON);
		real_t val = sq(cl.x())/s.x()+sq(cl.y())/s.y()+sq(cl.z())/s.z();
		if(val > vmax){
		  vmax = val;
		}
	}
	s *= vmax;
	s.x() = sqrt(s.x());s.y() = sqrt(s.y());s.z() = sqrt(s.z());
#else
    // std::cerr << "Using CGAL" << std::endl;
    typedef CGAL::Cartesian<double>                              Kernel;
    typedef CGAL::MP_Float                                         ET;
    typedef CGAL::Approximate_min_ellipsoid_d_traits_3<Kernel, ET> AMETraits;
    typedef AMETraits::Point                                       CgalPoint;
    typedef std::vector<CgalPoint>                                 CgalPointList;
    typedef CGAL::Approximate_min_ellipsoid_d<AMETraits>           AME;

    CgalPointList P;
    for(Point3Array::const_iterator it = __pointstofit->begin(); it != __pointstofit->end(); ++it)
        P.push_back(CgalPoint((double)it->x(),(double)it->y(),(double)it->z()));

    // compute approximation:
    const double eps = 0.01;                // approximation ratio is (1+eps)
    AMETraits traits;
    AME ame(eps, P.begin(), P.end(), traits);
    if (!ame.is_valid(true) || !ame.is_full_dimensional()){
	std::cerr << "Not valid bounding ellipsoid" << std::endl;
	return GeometryPtr();
    }
    AME::Center_coordinate_iterator c_it = ame.center_cartesian_begin();
    Vector3 _center(*c_it,*(c_it+1),*(c_it+2));
    AME::Axes_lengths_iterator axeslength = ame.axes_lengths_begin();
    Vector3 s(*axeslength,*(axeslength+1),*(axeslength+2));
    AME::Axes_direction_coordinate_iterator d_it = ame.axis_direction_cartesian_begin(0);
    Vector3 u(*d_it,*(d_it+1),*(d_it+2));
    d_it = ame.axis_direction_cartesian_begin(1);
    Vector3 v(*d_it,*(d_it+1),*(d_it+2));
    d_it = ame.axis_direction_cartesian_begin(2);
    Vector3 w(*d_it,*(d_it+1),*(d_it+2));
#endif
	if(fabs(*(s.getMin())) > GEOM_EPSILON){
	  GeometryPtr geom(new Sphere(1,32,32));
	  if(fabs(*((s - Vector3(1,1,1)).getMax())) > GEOM_EPSILON)
		geom = GeometryPtr(new Scaled(s,geom));
	  if(!(u == Oriented::DEFAULT_PRIMARY) && !(v == Oriented::DEFAULT_SECONDARY))
		geom = GeometryPtr(new Oriented(u,v,geom));
	  if(fabs(*(_center.getMax())) > GEOM_EPSILON)
		geom = GeometryPtr(new Translated(_center,geom));
	  return geom;
         }
         else std::cerr << "Ellipsoid dimension not valid : "<< s.x() << ',' << s.y() << ',' << s.z() << std::endl;
  }
  return GeometryPtr();
}

/* ----------------------------------------------------------------------- */

GeometryPtr Fit::balignedbox(){
  if(__pointstofit->size() > 2){
	pair<Vector3,Vector3> _bounds = __pointstofit->getBounds();
	Vector3 center = _bounds.first+_bounds.second;
	center/=2;
	Vector3 size = _bounds.second-_bounds.first;
	size /=2;
	return GeometryPtr(new Translated(center,GeometryPtr(new Box(size))));
  }
  return GeometryPtr();
}

/* ----------------------------------------------------------------------- */

GeometryPtr Fit::aalignedbox(){
  if(__pointstofit->size() > 2){
	Vector3 center = __pointstofit->getCenter();
	Vector3 s(0,0,0);
	for(Point3Array::iterator _it = __pointstofit->begin();
							  _it != __pointstofit->end();
							  _it++){
	    Vector3 l = *_it - center;
	    real_t a = fabs(l.x());
	    real_t b = fabs(l.y());
	    real_t c = fabs(l.z());
		s.x()+=a;s.y()+=b;s.z()+=c;
	}
	s/=__pointstofit->size();
	return GeometryPtr(new Translated(center,GeometryPtr(new Box(s))));
  }
  return GeometryPtr();
}

/* ----------------------------------------------------------------------- */

GeometryPtr Fit::box(){
  if(! __pointstofit )return GeometryPtr();
  if(__pointstofit->size()>2){
	float vpvec[3][3];
	float vpint[3] = {0,0,0};
	float vpang[3][3];
	short vpok[3];
	Vector3 u,v,w,s;
	Laxi_ComputeInertia (__pointstofit,vpvec,vpint,vpang,vpok);
	if(vpok[2]!=0  && vpok[1]!=0 && vpok[0]!=0 && vpint[2]!=0 && vpint[1]!=0 &&  vpint[0]!=0 ){ 
	    u = Vector3(vpvec[0][0]/vpint[0],vpvec[0][1]/vpint[0],vpvec[0][2]/vpint[0]);
	    v = Vector3(vpvec[1][0]/vpint[1],vpvec[1][1]/vpint[1],vpvec[1][2]/vpint[1]);
	    w = Vector3(vpvec[2][0],vpvec[2][1],vpvec[2][2]);
	    s = Vector3(vpint[0],vpint[1],vpint[2]);
	}
	else return GeometryPtr();
	Vector3 _center = __pointstofit->getCenter();
	if(fabs(*(s.getMin())) > GEOM_EPSILON){
	  GeometryPtr geom(new Box(s));
	  if(!(u == Oriented::DEFAULT_PRIMARY) && !(v == Oriented::DEFAULT_SECONDARY))
		geom = GeometryPtr(new Oriented(u,v,geom));
	  if(fabs(*(_center.getMax())) > GEOM_EPSILON)
		geom = GeometryPtr(new Translated(_center,geom));
	  return geom;
    }
  }
  return GeometryPtr();
}

/* ----------------------------------------------------------------------- */
GeometryPtr Fit::abox(){
  if(! __pointstofit )return GeometryPtr();
  if(__pointstofit->size()>2){
	float vpvec[3][3];
	float vpint[3] = {0,0,0};
	float vpang[3][3];
	short vpok[3];
	Vector3 u,v,w,s;
	Laxi_ComputeInertia (__pointstofit,vpvec,vpint,vpang,vpok);
	if(vpok[2]!=0  && vpok[1]!=0 && vpok[0]!=0 && vpint[2]!=0 && vpint[1]!=0 &&  vpint[0]!=0 ){ 
	    u = Vector3(vpvec[0][0]/vpint[0],vpvec[0][1]/vpint[0],vpvec[0][2]/vpint[0]);
	    v = Vector3(vpvec[1][0]/vpint[1],vpvec[1][1]/vpint[1],vpvec[1][2]/vpint[1]);
	    w = Vector3(vpvec[2][0]/vpint[2],vpvec[2][1]/vpint[2],vpvec[2][2]/vpint[2]);
	    s = Vector3(vpint[0],vpint[1],vpint[2]);
	}
	else return GeometryPtr();
	Vector3 _center = __pointstofit->getCenter();
	s = Vector3(0,0,0);
	for(Point3Array::iterator _it = __pointstofit->begin();_it != __pointstofit->end();_it++){
	    Vector3 l = *_it - _center;
	    real_t a = fabs(dot(l,u));
	    real_t b = fabs(dot(l,v));
	    real_t c = fabs(dot(l,w));
		s.x()+=a;s.y()+=b;s.z()+=c;
	}
	s/=__pointstofit->size();
	if(fabs(*(s.getMin())) > GEOM_EPSILON){
	  GeometryPtr geom(new Box(s));
	  if(!(u == Oriented::DEFAULT_PRIMARY) && !(v == Oriented::DEFAULT_SECONDARY))
		geom = GeometryPtr(new Oriented(u,v,geom));
	  if(fabs(*(_center.getMax())) > GEOM_EPSILON)
		geom = GeometryPtr(new Translated(_center,geom));
	  return geom;
    }
  }
  return GeometryPtr();
}

/* ----------------------------------------------------------------------- */
GeometryPtr Fit::bbox(){
  if(! __pointstofit )return GeometryPtr();
  if(__pointstofit->size()>2){
	float vpvec[3][3];
	float vpint[3] = {0,0,0};
	float vpang[3][3];
	short vpok[3];
	Vector3 u,v,w,s;
	Laxi_ComputeInertia (__pointstofit,vpvec,vpint,vpang,vpok);
	if(vpok[2]!=0  && vpok[1]!=0 && vpok[0]!=0 && vpint[2]!=0 && vpint[1]!=0 &&  vpint[0]!=0 ){ 
	    u = Vector3(vpvec[0][0]/vpint[0],vpvec[0][1]/vpint[0],vpvec[0][2]/vpint[0]);
	    v = Vector3(vpvec[1][0]/vpint[1],vpvec[1][1]/vpint[1],vpvec[1][2]/vpint[1]);
	    w = Vector3(vpvec[2][0]/vpint[2],vpvec[2][1]/vpint[2],vpvec[2][2]/vpint[2]);
	    s = Vector3(vpint[0],vpint[1],vpint[2]);
	}
	else return GeometryPtr();
	const Vector3& first = __pointstofit->getAt(0);
	Vector3 p1(dot(first,u),dot(first,v),dot(first,w));
	Vector3 pmax(p1);
	Vector3 pmin(p1);
	Vector3 _center(0,0,0);
	for(Point3Array::iterator _it1 = __pointstofit->begin();
		_it1 != __pointstofit->end();_it1++){
	    Vector3 l = *_it1 ;
	    real_t a = dot(l,u);
	    real_t b = dot(l,v);
	    real_t c = dot(l,w);
		if(pmax.x() < a)pmax.x() = a;
		if(pmax.y() < b)pmax.y() = b;
		if(pmax.z() < c)pmax.z() = c;
		if(pmin.x() > a)pmin.x() = a;
		if(pmin.y() > b)pmin.y() = b;
		if(pmin.z() > c)pmin.z() = c;
	}
	_center =(pmin+pmax)/2;
	_center = u*_center.x()+v*_center.y()+w*_center.z();
	s = (pmax - pmin)/2;
/*	for(Point3Array::iterator _it = __pointstofit->begin();_it != __pointstofit->end();_it++){
	    Vector3 l = *_it - _center;
	    real_t a = fabs(dot(l,u));
	    real_t b = fabs(dot(l,v));
	    real_t c = fabs(dot(l,w));
		if(s.x() < a)s.x() = a;
		if(s.y() < b)s.y() = b;
		if(s.z() < c)s.z() = c;
	}*/
	if(fabs(*(s.getMin())) > GEOM_EPSILON){
	  GeometryPtr geom(new Box(s));
	  if(!(u == Oriented::DEFAULT_PRIMARY) && !(v == Oriented::DEFAULT_SECONDARY))
		geom = GeometryPtr(new Oriented(u,v,geom));
	  if(fabs(*(_center.getMax())) > GEOM_EPSILON)
		geom = GeometryPtr(new Translated(_center,geom));
	  return geom;
    }
  }
  return GeometryPtr();
}

/* ----------------------------------------------------------------------- */

GeometryPtr Fit::frustum(){
  if(__pointstofit->size() == 2){
	Vector3 u =__pointstofit->getAt(1) -__pointstofit->getAt(0);
	real_t height = u.normalize();
	if(height < GEOM_EPSILON) return GeometryPtr();
	Vector3 w,v = cross(u,Vector3::OZ);	
	if(v == Vector3::ORIGIN){
	  v = Vector3::OX;
	}
	else {
	  v.normalize();
	}
	w = cross (u,v);
	w.normalize();
	if(!__radius){
	  if(v == Vector3::OX && w == Vector3::OY){
		return GeometryPtr(
		  new Translated(__pointstofit->getAt(0),GeometryPtr(
		  new Cylinder(1,
		  height,true,32))));
	  }
	  else {
		return GeometryPtr(
		  new Translated(__pointstofit->getAt(0),GeometryPtr(
		  new Oriented(v,w,GeometryPtr(new Cylinder(1,
								  height,true,32))))));
	  }
	}
	else {
	  if(__radius->getAt(0) == Vector2::ORIGIN && __radius->getAt(1) == Vector2::ORIGIN){
		return GeometryPtr(new Polyline(__pointstofit));
	  }
	  else if (__radius->getAt(0) == Vector2::ORIGIN) {
		if(v == Vector3::OX && w == Vector3::OY)
		  return GeometryPtr(
		  new Translated(__pointstofit->getAt(1),GeometryPtr(		    
		  new Scaled(Vector3(__radius->getAt(1),1),GeometryPtr(
		  new Cone(1,
		  height
		  ))))));
		else
		  return GeometryPtr(
		  new Translated(__pointstofit->getAt(1),GeometryPtr(		    
		  new Oriented(w,v,GeometryPtr(
		  new Scaled(Vector3(__radius->getAt(1),1),GeometryPtr(
		  new Cone(1,
					     height
						 ))))))));
		
	  }
	  else if (__radius->getAt(1) == Vector2::ORIGIN) {
		if(v == Vector3::OX && w == Vector3::OY)
		  return GeometryPtr(
		  new Translated(__pointstofit->getAt(0),GeometryPtr(		    
		  new Scaled(Vector3(__radius->getAt(0),1),GeometryPtr(
		  new Cone(1,
					     height
						 ))))));
		else
		  return GeometryPtr(
		  new Translated(__pointstofit->getAt(0),GeometryPtr(		    
		  new Oriented(v,w,GeometryPtr(
		  new Scaled(Vector3(__radius->getAt(0),1),GeometryPtr(
		  new Cone(1,
					     height
						 ))))))));
	  }
	  else if((__radius->getAt(0).x() != 0) && (__radius->getAt(1).x() != 0) && 
		((__radius->getAt(0).y()/__radius->getAt(0).x()) == 
		(__radius->getAt(1).y()/__radius->getAt(1).x()))) {
		if(__radius->getAt(0).y()==__radius->getAt(0).x()){ /// R[0] = <a,a> et R[1] = <b,b>
		  if(__radius->getAt(0).x() == __radius->getAt(1).x()){ /// R[0] == R[1].
			if(v == Vector3::OX && w == Vector3::OY){
			  return GeometryPtr(
				new Translated(__pointstofit->getAt(0),GeometryPtr(
				new Cylinder(__radius->getAt(0).x(),
				height,true,32
				))));
			}
			else{
			  return GeometryPtr(
				new Translated(__pointstofit->getAt(0),GeometryPtr(
				new Oriented(v,w,GeometryPtr(
				new Cylinder(__radius->getAt(0).x(),
				height,true,32
				))))));
			}
		  }
		  else{  /// R[0] == k x R[1].
			if(v == Vector3::OX && w == Vector3::OY){
			  return GeometryPtr(
				new Translated(__pointstofit->getAt(0),GeometryPtr(
				new Frustum(__radius->getAt(0).x(),
				height,
				__radius->getAt(1).x()/__radius->getAt(0).x(),
				true,32
				))));
			}
			else {
			  return GeometryPtr(
				new Translated(__pointstofit->getAt(0),GeometryPtr(
				new Oriented(v,w,GeometryPtr(
				new Frustum(__radius->getAt(0).x(),
				height,
				__radius->getAt(1).x()/__radius->getAt(0).x(),
				true,32
				))))));
			}
		  }
		}
		else { /// R[0] == a x R[1].
		  if(v == Vector3::OX && w == Vector3::OY){
			return GeometryPtr(
			  new Translated(__pointstofit->getAt(0),GeometryPtr(
			  new Scaled(Vector3(1,__radius->getAt(0).y()/__radius->getAt(0).x(),1), GeometryPtr(
			  new Frustum(__radius->getAt(0).x(),
			  height,
			  __radius->getAt(1).x()/__radius->getAt(0).x(),
			  true,32
			  ))))));
		  }
		  else {
			return GeometryPtr(
			  new Translated(__pointstofit->getAt(0),GeometryPtr(
			  new Oriented(v,w,GeometryPtr(
			  new Scaled(Vector3(1,__radius->getAt(0).y()/__radius->getAt(0).x(),1), GeometryPtr(
			  new Frustum(__radius->getAt(0).x(),
			  height,
			  __radius->getAt(1).x()/__radius->getAt(0).x(),
			  true,32
			  ))))))));
		  }
		}
	  }
	  else {
		if(!__default_crossSection){
		  Point2ArrayPtr _crossSection(new Point2Array(9));
		  _crossSection->setAt(0,Vector2(0.5,0));
		  _crossSection->setAt(1,Vector2(0.353553,0.353553));
		  _crossSection->setAt(2,Vector2(-2.18557e-08,0.5));
		  _crossSection->setAt(3,Vector2(-0.353553,0.353553));
		  _crossSection->setAt(4,Vector2(-0.5,-4.37114e-08));
		  _crossSection->setAt(5,Vector2(-0.353553,-0.353553));
		  _crossSection->setAt(6,Vector2(6.55671e-08,-0.5));
		  _crossSection->setAt(7,Vector2(0.353553,-0.353553));
		  _crossSection->setAt(8,Vector2(0.5,0));
		  __default_crossSection = Curve2DPtr(new Polyline2D(_crossSection));
		  __default_crossSection->setName(string("Default_CrossSection"));
		}
		return GeometryPtr(new Extrusion(LineicModelPtr(new Polyline(__pointstofit)),
		  __default_crossSection,
		  __radius));
	  }
	}
    }
    else return cylinder();
}
/* ----------------------------------------------------------------------- */

GeometryPtr Fit::asymmetricHull(){
    if(! __pointstofit )return GeometryPtr();
    Point3ArrayPtr _points;
    ExplicitModelPtr convex = dynamic_pointer_cast<ExplicitModel>(convexHull());
    if(!convex) _points = __pointstofit;
    else _points = convex->getPointList();
    if(_points->size()<6) return GeometryPtr();
    pair<Point3Array::const_iterator,Point3Array::const_iterator> _bounds;
    Vector3 XMin,XMax,YMin,YMax,bottom,top;
    _bounds = _points->getXMinAndMax();
    XMin = *(_bounds.first);
    XMax = *(_bounds.second);
    _bounds = _points->getYMinAndMax();
    YMin = *(_bounds.first);
    YMax = *(_bounds.second);
    _bounds = _points->getZMinAndMax();
    Vector3 center = _points->getCenter();
    bottom = *(_bounds.first);
    top = *(_bounds.second);
    bottom -= center;
    top -= center;
    real_t negXRadius = center.x()-XMin.x();
    real_t posXRadius = XMax.x()-center.x();
    real_t negYRadius = center.y()-YMin.y();
    real_t posYRadius = YMax.y()-center.y();

    real_t negXHeight = XMin.z() - center.z();
    real_t posXHeight = XMax.z() - center.z();
    real_t negYHeight = YMin.z() - center.z() ;
    real_t posYHeight = YMax.z() - center.z();

    real_t botshape = 0;
    real_t topshape = 0;

    real_t negXBotRadius = negXRadius + bottom.x();  /// coord on -X axis.
    real_t negXBotHeight = negXHeight - bottom.z();
    real_t posXBotRadius = posXRadius - bottom.x();
    real_t posXBotHeight = posXHeight - bottom.z();

    real_t negXTopRadius = negXRadius + top.x();
    real_t negXTopHeight = top.z() - negXHeight;
    real_t posXTopRadius = posXRadius - top.x();
    real_t posXTopHeight = top.z() - posXHeight;

    real_t negYBotRadius = negYRadius + bottom.y();  /// coord on -X axis.
    real_t negYBotHeight = negYHeight - bottom.z();
    real_t posYBotRadius = posYRadius - bottom.y();
    real_t posYBotHeight = posYHeight - bottom.z();

    real_t negYTopRadius = negYRadius + top.x();
    real_t negYTopHeight = top.z() - negYHeight;
    real_t posYTopRadius = posYRadius - top.x();
    real_t posYTopHeight = top.z() - posYHeight;



    for(Point3Array::iterator _it = _points->begin();
	_it != _points->end(); _it++){
	  Vector3 a = *_it - center;
	  if(a.x() < bottom.x() && a.z() <  negXHeight){
		botshape = max(botshape,
		  (real_t)fitShapeFactor(bottom.x()-a.x(),
						   negXBotRadius,
						   negXHeight-a.z(),
						   negXBotHeight )); 
	  }
	  else if(a.x() > bottom.x()  && a.z() <  posXHeight){
		botshape = max(botshape,
		  (real_t)fitShapeFactor(a.x()-bottom.x(),
						   posXBotRadius,
						   posXHeight-a.z(),
						   posXBotHeight )); 
	  }
	  else if(a.x() < top.x() && a.z() >  negXHeight){
		topshape = max(topshape,
		  (real_t)fitShapeFactor(top.x()-a.x(),
						   negXTopRadius,
						   a.z()-negXHeight,
						   negXTopHeight )); 
	  }
	  else if(a.x() > top.x()  && a.z() >  posXHeight){
		topshape = max(topshape,
		  (real_t)fitShapeFactor(a.x()-top.x(),
						   posXTopRadius,
						   a.z()-posXHeight,
						   posXTopHeight )); 
	  }
	  
	  if(a.y() < bottom.y() && a.z() <  negYHeight){
		botshape = max(botshape,
		  (real_t)fitShapeFactor(bottom.y()-a.y(),
						   negYBotRadius,
						   negYHeight-a.z(),
						   negYBotHeight )); 
	  }
	  else if(a.y() > bottom.y()  && a.z() <  posYHeight){
		botshape = max(botshape,
		  (real_t)fitShapeFactor(a.y()-bottom.y(),
						   posYBotRadius,
						   posYHeight-a.z(),
						   posYBotHeight )); 
	  }
	  else if(a.y() < top.y() && a.z() >  negYHeight){
		topshape = max(topshape,
		  (real_t)fitShapeFactor(top.y()-a.y(),
						   negYTopRadius,
						   a.z()-negYHeight,
						   negYTopHeight )); 
	  }
	  else if(a.y() > top.y()  && a.z() >  posYHeight){
		topshape = max(topshape,
		  (real_t)fitShapeFactor(a.y()-top.y(),
						   posYTopRadius,
						   a.z()-posYHeight,
						   posYTopHeight )); 
	  }
    }

    if(!botshape)botshape = 0.2;
    if(!topshape)topshape = 0.2;

    if(center.x() < GEOM_EPSILON && center.y() < GEOM_EPSILON && center.z() < GEOM_EPSILON )
        return GeometryPtr(new AsymmetricHull(negXRadius,posXRadius,
                                              negYRadius,posYRadius,
                                              negXHeight,posXHeight,
                                              negYHeight,posYHeight,
                                              bottom,top,
                                              botshape,topshape,
                                              AsymmetricHull::DEFAULT_SLICES,
                                              AsymmetricHull::DEFAULT_STACKS));
    else
        return GeometryPtr(new Translated(center,
                                          GeometryPtr(new AsymmetricHull(negXRadius,posXRadius,
                                                                         negYRadius,posYRadius,
                                                                         negXHeight,posXHeight,
                                                                         negYHeight,posYHeight,
                                                                         bottom,top,
                                                                         botshape,topshape,
                                                                         AsymmetricHull::DEFAULT_SLICES,
                                                                         AsymmetricHull::DEFAULT_STACKS))));

}


double Fit::fitShapeFactor(double x, double r, double y, double h){
    assert(r !=0 && h != 0);
	if(x == 0 || y == 0)return 0;
	if(x >= r || y >= h) return 0;
	double X = x / r;
	double Y = y / h;
    double lnX = log( X );
	double lnY = log( Y );
	double n1 = 0;
	double fn1 = 1;
	if(X+Y>1){
	  n1 = 1;
	  fn1 = X+Y-1;
	}
	double n2 = log(0.5)/(X>Y?lnX:lnY);
	double fn2 = pow(X,n2)+pow(Y,n2)-1;
	if(fabs(fn2) < GEOM_EPSILON)return n2;
	if(fn2 > GEOM_EPSILON) {
	  cerr << "Error with 0.5 assumption" << endl;
	  cerr << "X=" << X << " - Y=" << Y << " : n2=" << n2 << " - fn2=" << fn2 << endl;
	  return 0;
	}
	double n = n1;
	double fn = fn1;
	while(fabs(fn) > GEOM_EPSILON){	  
	  n = n1-fn1*(n2-n1)/(fn2-fn1);//(n1+n2)/2;
	  fn = pow(X,n)+pow(Y,n)-1;
	  if(fn>= 0){n1 = n; fn1 = fn; }
	  else {n2 = n; fn2 = fn; }
	}
	return n;
/*
    double lna = ln( x / r );
	double lnb = ln( y / h );
    double ln2a = lna * lna;
    double ln2b = lnb * lnb;
    double res = - (lna +lnb)/(ln2a +ln2b);
    return res;*/
}

/* ----------------------------------------------------------------------- */


GeometryPtr Fit::extrudedHull(){
    if(! __pointstofit )return GeometryPtr();
  
    uint_t numpoints = __pointstofit->size();
    if(numpoints>3){

	Vector3 _translation;
	/// Horizontal Profile.
	Point2ArrayPtr horizontal_proj(new Point2Array(numpoints)) ;
	uint_t k=0;
	for(Point3Array::iterator _it = __pointstofit->begin();
	    _it != __pointstofit->end();
	    _it++)
	    horizontal_proj->setAt(k++,Vector2(_it->x(),_it->y()));
	
	Point2ArrayPtr horizontal_profile = convexPolyline(horizontal_proj);

	/// Vertical Profile.
	Point2ArrayPtr vertical_proj(new Point2Array(numpoints));
	k=0;
	for(Point3Array::iterator _it2 = __pointstofit->begin();
	    _it2 != __pointstofit->end();
	    _it2++)
	    vertical_proj->setAt(k++,Vector2(_it2->x(),_it2->z()));
  

	Point2ArrayPtr vertical_profile = convexPolyline(vertical_proj);


	if(!vertical_profile||!horizontal_profile){
	    if(vertical_profile) return GeometryPtr(new Oriented(Vector3::OX,Vector3::OZ,
								 GeometryPtr(new Polyline2D(vertical_profile))));
	    else if(horizontal_profile) return GeometryPtr(new Polyline2D(horizontal_profile));
	    else return GeometryPtr();
	}

/*	return GeometryPtr(new ExtrudedHull(Curve2DPtr(new Polyline2D(vertical_profile)),
						Curve2DPtr(new Polyline2D(horizontal_profile)),
						ExtrudedHull::DEFAULT_CCW));
*/
	    
/*	Vector2 average = horizontal_profile->getCenter();
	if(fabs(average.x()) > GEOM_EPSILON || fabs(average.y()) > GEOM_EPSILON){
	    for(Point2Array::iterator _iter = horizontal_profile->begin();
		_iter != horizontal_profile->end();
		_iter++)
		(*_iter) -= average;
	}
	_translation.x() =  average.x();
	_translation.y() =  average.y();

	average = vertical_profile->getCenter();
	average.x() = _translation.x();

	if(fabs(average.x()) > GEOM_EPSILON || fabs(average.y()) > GEOM_EPSILON){
	    for(Point2Array::iterator _iter = vertical_profile->begin();
		_iter != vertical_profile->end();
		_iter++)
		(*_iter) -= average;
	}
	_translation.z() =  average.y();*/

  
	vector<Vector2> gauche(0);
	vector<Vector2> droite(0);
	vector<Vector2> gauche2(0);
	vector<Vector2> droite2(0);

	Point2Array::iterator _itf,_itf2=vertical_profile->begin(),
	    _itf3=vertical_profile->begin();
	
	for(_itf=vertical_profile->begin()+1;_itf!=vertical_profile->end();_itf++){
	    if(_itf2->y()>_itf->y())_itf2=_itf; // min 
	    if(_itf3->y()<_itf->y())_itf3=_itf; // max 
	}
	if(distance(_itf2,_itf3)>0){
/*	    cerr << "Decoupage 0 - " << distance(vertical_profile->begin(),_itf2) << " - " 
		 << distance(vertical_profile->begin(),_itf3) << " - " 
		 << distance(vertical_profile->begin(),vertical_profile->end())<<  endl;
*/	    
	    gauche.insert(gauche.begin(),_itf2,_itf3);
//	    cerr << "Gauche set made" << endl;
	    if(_itf3!=vertical_profile->end())
		droite.insert(droite.begin(),_itf3,vertical_profile->end());
	    if(_itf2!=vertical_profile->begin())
		droite.insert(droite.end(),vertical_profile->begin(),_itf2);
	}
	else {
/*	    cerr << "*Decoupage 0 - " << distance(vertical_profile->begin(),_itf3) << " - " 
		 << distance(vertical_profile->begin(),_itf2) << " - " 
		 << distance(vertical_profile->begin(),vertical_profile->end())<<  endl;*/
	    vector<Vector2>::iterator _itf4=_itf2;
	    while(_itf4!=_itf3){gauche.push_back(*_itf4);_itf4--;}
	    while(_itf4!=vertical_profile->begin()){droite.push_back(*_itf4);_itf4--;}
	    if(_itf4==vertical_profile->begin())droite.push_back(*_itf4);
	    _itf4=vertical_profile->end()-1;
	    while(_itf4!=_itf2){droite.push_back(*_itf4);_itf4--;}
	}
//	cerr << "Scinde l'ensemble en 2." << endl;
//	cerr << "Ensemble initial :" << endl;
/*	_itf2 = vertical_profile->begin();
	while(_itf2!=vertical_profile->end()){
	    cerr << *_itf2 << ",";_itf2++;
	}
	cerr  << endl;
	cerr << "Gauche :" << endl;
	_itf2 = gauche.begin();
	while(_itf2!=gauche.end()){
	    cerr << *_itf2 << ",";_itf2++;
	}
	cerr  << endl << "Droite :" << endl;
	_itf3 = droite.end();
	while(_itf3!=droite.begin()){
	    _itf3--;cerr << *_itf3 << ",";
	}
	cerr  << endl<< endl;*/
	_itf2 = gauche.begin()+1;
	gauche2.push_back(*gauche.begin());
	_itf3 = droite.end()-1;
	while(_itf2!=gauche.end()){
//	    cerr << *_itf2 << "-" << *_itf3 << endl;
	    if(_itf2->y() < _itf3->y()){
		if(_itf3 == droite.end()-1){
/*		    cerr << "1/Moyenne de " << *_itf3 << " et " << *(gauche.begin()) << " en " << _itf2->y() <<  endl;
		    cerr << "Calcul : " << (_itf2->y()-gauche.begin()->y()) 
			 << "*" << (_itf3->x()-gauche.begin()->x()) << "/" 
			 <<  (_itf3->y()-gauche.begin()->y()) << " + " << gauche.begin()->x() << endl;*/
		    droite2.push_back(Vector2(((_itf2->y()-gauche.begin()->y())*(_itf3->x()-gauche.begin()->x())/(_itf3->y()-gauche.begin()->y()))
					      +gauche.begin()->x(),
					      _itf2->y()));
		}
		else{
//		    cerr << "2/Moyenne de " << *_itf3 << " et " << *(_itf3+1)  << " en " << _itf2->y() << endl;
		    droite2.push_back(Vector2(((_itf2->y()-(_itf3+1)->y())*(_itf3->x()-(_itf3+1)->x())/(_itf3->y()-(_itf3+1)->y()))
					      +(_itf3+1)->x(),
					      _itf2->y()));
		}
//		cerr << "Add to droite " << *(droite2.end()-1) << endl;
		gauche2.push_back(*_itf2);
		_itf2++;//cerr << "++ " << *_itf2 << "-" << *_itf3 << endl;
	    }
	    else if(_itf2->y() > _itf3->y()){
		if(_itf3->y() == (gauche.begin())->y()){
//		    cerr << "Multiple first point !" << endl;
		    gauche2.insert(gauche2.begin(),Vector2(((gauche.begin())->x()+_itf3->x())/2,_itf3->y()));
		}
		else if(_itf3->y() == (droite.begin())->y()){
//		    cerr << "Multiple last point !" << endl;
		    droite2.push_back(Vector2(((droite.begin())->x()+_itf3->x())/2,_itf3->y()));
		}
		else{
/*		    cerr << "4/Moyenne de " << *_itf2 << " et " << *(_itf2-1)  << " en " << _itf3->y() << endl;
		    cerr << "Calcul : " 
			 << (_itf3->y()-(_itf2-1)->y()) << "*" 
			 << (_itf2->x()-(_itf2-1)->x()) << "/" 
			 << (_itf2->y()-(_itf2-1)->y()) << "+" << (_itf2-1)->x() << endl;*/
		    gauche2.push_back(Vector2(((_itf3->y()-(_itf2-1)->y())*(_itf2->x()-(_itf2-1)->x())/(_itf2->y()-(_itf2-1)->y()))
					      +(_itf2-1)->x(),
					      _itf3->y()));
//		    cerr << "Add to gauche " << *(gauche2.end()-1) << endl;
		}
		droite2.push_back(*_itf3);
		_itf3--;//cerr << "-- " << *_itf2 << "-" << *_itf3 << endl;
	    }
	    else {
//		cerr << "Found equivalent points" << endl;
		gauche2.push_back(*_itf2);
		_itf2++;
		droite2.push_back(*_itf3);
		_itf3--;
	    }
	}
//	cerr << "finish gauche list" << endl;
	while(_itf3 != droite.begin()){
	    droite2.push_back(*_itf3);
	    if(_itf3->y() == (gauche.begin())->y()){
//		cerr << "Multiple first point !" << endl;
		gauche2.insert(gauche2.begin(),Vector2(((gauche.begin())->x()+_itf3->x())/2,_itf3->y()));
	    }
	    else if(_itf3->y() == (droite.begin())->y()){
//		cerr << "Multiple last point !" << endl;
		droite2.push_back(Vector2(((droite.begin())->x()+_itf3->x())/2,_itf3->y()));
	    }
	    else{	    
//		cerr << "5/Moyenne de " << *(gauche.end()-1) << " et " << *(droite.begin())  << " en " << _itf3->y() << endl;
		gauche2.push_back(Vector2(((_itf3->y()-(gauche.end()-1)->y())*(droite.begin()->x()-(gauche.end()-1)->x())/(droite.begin()->y()-(gauche.end()-1)->y()))
					  +(gauche.end()-1)->x(),_itf3->y()));
	    }
//	    cerr << "Add to gauche " << *(gauche2.end()-1) << endl;
//	    cerr << "Add to droite " << *(droite2.end()-1) << endl;
	    _itf3--;
	}
	droite2.push_back(*_itf3);

//	cerr << "Fin d'Ajout de points" << endl;
	vertical_profile = Point2ArrayPtr(new Point2Array(0));;
	if(gauche2[1].x()<droite2[0].x()){
//	    cerr << "Normal orientation" << endl;
	    vertical_profile->insert(vertical_profile->begin(),gauche2.begin(),gauche2.end());
	    for(_itf3 = droite2.end();_itf3 != droite2.begin();){
		_itf3--;vertical_profile->push_back(*_itf3);
	    }
	}
	else{
//	    cerr << "Inverse orientation" << endl;
	    vertical_profile->push_back(*gauche2.begin());
	    vertical_profile->insert(vertical_profile->end(),droite2.begin(),droite2.end());
	    for(_itf3 = gauche2.end();_itf3 != gauche2.begin()+1;){
		_itf3--;vertical_profile->push_back(*_itf3);
	    }	  
	}
//	cerr << "Fin fusion des points" << endl;
	

	_itf2=vertical_profile->begin();_itf3=vertical_profile->begin();
	Point2Array::iterator _itf2bis=vertical_profile->begin(),
	    _itf3bis=vertical_profile->begin();
	
	for(_itf=vertical_profile->begin()+1;_itf!=vertical_profile->end();_itf++){
	    if(_itf2->y()>_itf->y())_itf2=_itf; // min 
	    else if(_itf2->y()==_itf->y())_itf2bis=_itf;
	    if(_itf3->y()<_itf->y())_itf3=_itf; // max
	    else if(_itf3->y()==_itf->y())_itf3bis=_itf;
	}

	if(_itf2!=_itf2bis&&_itf2->y()==_itf2bis->y()){
//	    cerr << "Multiple bottom points : " << distance(_itf2,_itf2bis) << endl;
	}
	if(_itf2!=_itf2bis&&_itf2->y()==_itf2bis->y()){
//	    cerr << "Multiple top points : " <<  distance(_itf3,_itf3bis) << endl;
	}

	if(_translation.x() < GEOM_EPSILON && _translation.y() < GEOM_EPSILON && _translation.z() < GEOM_EPSILON )
	    return GeometryPtr(new ExtrudedHull(Curve2DPtr(new Polyline2D(vertical_profile)),
						Curve2DPtr(new Polyline2D(horizontal_profile)),
						ExtrudedHull::DEFAULT_CCW));
	else
	    return GeometryPtr(new Translated(_translation,GeometryPtr(new ExtrudedHull(Curve2DPtr(new Polyline2D(vertical_profile)),
											Curve2DPtr(new Polyline2D(horizontal_profile)),
											ExtrudedHull::DEFAULT_CCW))));
	
    }
    else if(numpoints==3){
	Index3ArrayPtr topo(new Index3Array());
	topo->push_back(Index3(0,1,2));
	return GeometryPtr(new TriangleSet(__pointstofit,topo,true,
					   true,true,
					   PolylinePtr(new Polyline(__pointstofit->getAt(0),__pointstofit->getAt(1)))));
	
    }
    else if(numpoints==2){
	return GeometryPtr(new Polyline(__pointstofit));
    }
    else return GeometryPtr();
    
}

/* ----------------------------------------------------------------------- */

GeometryPtr
Fit::convexHull(){
#ifdef WITHOUT_QHULL
	return GeometryPtr();
#else
  if(! __pointstofit )return GeometryPtr();

  /// Transform Point3Array in qhull format.

  /* dimension of points */
  int dim=3;


  /* number of points */
  int numpoints =  __pointstofit->size();

//  cerr << "Fit " << numpoints << " points 3D." << endl;

  /* array of coordinates for each point */
  coordT * points = new coordT[dim*numpoints];

  /* True if qhull should free points in qh_freeqhull() or reallocation */
  boolT ismalloc = False;

  /* option flags for qhull, see qh_opt.htm */
  char flags[]= "qhull Tv QJ Pp";

  /* output from qh_produce_output()
     use NULL to skip qh_produce_output() */
  FILE *outfile= NULL;

  /* error messages from qhull code */
  FILE *errfile= NULL;

  /* 0 if no error from qhull */
  int exitcode;

  /* set by FORALLfacets */
  facetT *facet;

  /* memory remaining after qh_memfreeshort */
  int curlong, totlong;

  if(numpoints>3){
#ifndef _WIN32
    /* error messages from qhull code */
    errfile= fopen("/dev/null","w");
#else
        char * tf = getenv("TMP");
        if(tf == NULL)tf = "C:\temp";
        string f = tf + string("\\qhull.log");
    errfile= fopen(f.c_str(),"w");
#endif

//        errfile = fopen("/dev/null","w");
  int k=0;
  for(Point3Array::iterator _it = __pointstofit->begin();
      _it != __pointstofit->end();
      _it++){
    points[k++]=(coordT)(_it->x());
    points[k++]=(coordT)(_it->y());
    points[k++]=(coordT)(_it->z());
  }


  /* initialize dim, numpoints, points[], ismalloc here */
  exitcode= qh_new_qhull (dim, numpoints, points, ismalloc,flags, outfile, errfile);

  typedef pgl_hash_map<int,int> memtype;
  memtype mem;
  Point3ArrayPtr resulting_points(new Point3Array());
  int p_count=0;
  Index3ArrayPtr resulting_topo(new Index3Array());
  int t_count=0;
//  unsigned visitid = 0;
  vertexT *vertex,**vertexp;
  if (!exitcode) { /* if no error */
      /* 'qh facet_list' contains the convex hull */
      FORALLfacets {
          Vector3 _normal((real_t)facet->normal[0],(real_t)facet->normal[1],(real_t)facet->normal[2]);
          setT* _vertices = facet->vertices;
          Index topo;
          int lt_count=0;
          FOREACHvertex_(_vertices){
              int _id = vertex->id;
              memtype::const_iterator _it;
              if((_it=mem.find(_id) )!=mem.end()){
                  topo.push_back(_it->second);
                  lt_count++;
              }
              else {
                  resulting_points->push_back(Vector3((real_t)vertex->point[0],
                                                     (real_t)vertex->point[1],
                                                     (real_t)vertex->point[2]));
                  mem[_id]= p_count;
                  topo.push_back(p_count);lt_count++;
                  p_count++;
                  /* ... your code ... */
              }
          }
          if(lt_count!=3)cerr << "Bad triangulation !" << endl;
          else {
              Vector3 _norm = cross(resulting_points->getAt(topo.getAt(1))
                                    - resulting_points->getAt(topo.getAt(0)),
                                    resulting_points->getAt(topo.getAt(2))
                                    - resulting_points->getAt(topo.getAt(0)));
              if(dot(_norm,_normal) < GEOM_EPSILON)
                  resulting_topo->push_back(Index3(topo.getAt(0),topo.getAt(2),topo.getAt(1)));
              else
                  resulting_topo->push_back(Index3(topo.getAt(0),topo.getAt(1),topo.getAt(2)));
              t_count++;
          }
      }
  }
  qh_freeqhull(!qh_ALL);
  qh_memfreeshort (&curlong, &totlong);
  if (curlong || totlong)
    cerr << "qhull internal warning (main): did not free "<< totlong << "  bytes of long memory ("<< curlong <<" pieces)" << endl;

  fclose(errfile);
  if(p_count==0)return GeometryPtr();
  pair<Point3Array::const_iterator,Point3Array::const_iterator> _skel = resulting_points->getZMinAndMax();
  return GeometryPtr(new TriangleSet(resulting_points,resulting_topo,true,true,true,
                                     PolylinePtr(new Polyline(*(_skel.first),*(_skel.second)))));

  }
  else if(numpoints==3){
      Index3ArrayPtr topo(new Index3Array());
      topo->push_back(Index3(0,1,2));
      return GeometryPtr(new TriangleSet(__pointstofit,topo, true,true, true,
                                         PolylinePtr(new Polyline(__pointstofit->getAt(0),__pointstofit->getAt(1)))));

  }
  else if(numpoints==2){
      return GeometryPtr(new Polyline(__pointstofit));
  }
  else return GeometryPtr();
#endif
}
/* ----------------------------------------------------------------------- */


Point2ArrayPtr Fit::convexPolyline(const Point2ArrayPtr& _points){
#ifdef WITHOUT_QHULL
	return Point2ArrayPtr();
#else

    /* dimension of points */
    int dim=2;

    /* number of points */
    int numpoints =  _points->size();


    /* array of coordinates for each point */
    coordT * points = new coordT[dim*numpoints];

    /* True if qhull should free points in qh_freeqhull() or reallocation */
    boolT ismalloc = False;

    /* option flags for qhull, see qh_opt.htm */
    char flags[]= "qhull Tv Pp";

    /* output from qh_produce_output()
     use NULL to skip qh_produce_output() */
    FILE *outfile= NULL;

#ifndef _WIN32
    /* error messages from qhull code */
    FILE *errfile= fopen("/dev/null","w");
#else
        char * tf = getenv("TMP");
        if(tf == NULL)tf = "C:\temp";
        string f = tf + string("\\qhull.log");
    FILE *errfile= fopen(f.c_str(),"w");
#endif

    /* 0 if no error from qhull */
    int exitcode;

    /* set by FORALLfacets */
    facetT *facet;

    /* memory remaining after qh_memfreeshort */
    int curlong, totlong;


    assert(_points->isValid());

    Point2ArrayPtr _result(new Point2Array(0));


    if(numpoints>3){

        int k=0;
        for(Point2Array::iterator _it = _points->begin();
            _it != _points->end();
            _it++){
            points[k++]=(coordT)(_it->x());
            points[k++]=(coordT)(_it->y());
        }

        /* initialize dim, numpoints, points[], ismalloc here */
        exitcode= qh_new_qhull (dim, numpoints, points, ismalloc,flags, outfile, errfile);

        vertexT *vertex,**vertexp;
        vector<vector<Vector2> > r_points(0);
        vector<Vector2>::iterator _itf;
        vector<vector<Vector2> >::iterator _lf,_lf2;

        if (!exitcode) { /* if no error */
            /* 'qh facet_list' contains the convex hull */
            FORALLfacets {
                setT* _vertices = facet->vertices;
                vector<Vector2> _vf;
                FOREACHvertex_(_vertices){
                    _vf.push_back(Vector2((real_t)vertex->point[0],(real_t)vertex->point[1]));
                }

                for(_lf=r_points.begin();_lf!=r_points.end() &&
                        (*(_lf->begin())!=_vf[0]) && (*(_lf->end()-1)!=_vf[0])
                        && (*(_lf->begin())!=_vf[1]) && (*(_lf->end()-1)!=_vf[1]);_lf++);
                if(_lf==r_points.end()){
                    r_points.push_back(_vf);
                }
                else if((*(_lf->begin())==_vf[0]) || (*(_lf->end()-1)==_vf[0])){
                    if(*(_lf->begin())==_vf[0]){
                        // Insert _vf[1]  At the begining.
                        _lf->insert(_lf->begin(),_vf[1]);
                    }
                    else {
                        // Insert _vf[1]  At the end.
                        _lf->push_back(_vf[1]);
                    }
                    for(_lf2=_lf+1;_lf2!=r_points.end() && (*(_lf2->begin())!=_vf[1]) && (*(_lf2->end()-1)!=_vf[1]);_lf2++);
                    if(_lf2!=r_points.end()){
                        if(*(_lf2->begin())==_vf[1]){
                            if(*(_lf->begin())==_vf[1]){ // Jointure debut lf = debut lf2
                                for(_itf=_lf2->begin()+1;_itf!=_lf2->end();_itf++)_lf->insert(_lf->begin(),*_itf);
                            }
                            else {                       // Jointure fin lf = debut lf2
                                _lf->insert(_lf->end(),_lf2->begin()+1,_lf2->end());
                            }
                            r_points.erase(_lf2);
                        }
                        else if(*(_lf2->end()-1)==_vf[1]){
                            if(*(_lf->begin())==_vf[1]){  // Jointure debut lf = fin lf2
                                _lf->insert(_lf->begin(),_lf2->begin(),_lf2->end()-1);
                            }
                            else {                        // Jointure fin lf = fin lf2
                                for(_itf=_lf2->end()-2;_itf!=_lf2->begin();_itf--){ _lf->push_back(*_itf); }
                                _lf->push_back(*_lf2->begin());
                            }
                            r_points.erase(_lf2);
                        }
                    }
                }
                else if((*(_lf->begin())==_vf[1]) || (*(_lf->end()-1)==_vf[1])){
                    if(*(_lf->begin())==_vf[1]){
                        // Insert _vf[0] At the begining.
                        _lf->insert(_lf->begin(),_vf[0]);
                    }
                    else {
                        _lf->push_back(_vf[0]);
                        // Insert _vf[0] At the end.
                    }
                    for(_lf2=_lf+1;_lf2!=r_points.end() && (*(_lf2->begin())!=_vf[0]) && (*(_lf2->end()-1)!=_vf[0]);_lf2++);
                    if(_lf2!=r_points.end()){
                        if(*(_lf2->begin())==_vf[0]){
                            if(*(_lf->begin())==_vf[0]){ // Jointure debut lf = debut lf2
                                for(_itf=_lf2->begin()+1;_itf!=_lf2->end();_itf++)_lf->insert(_lf->begin(),*_itf);
                            }
                            else {                       // Jointure fin lf = debut lf2
                                _lf->insert(_lf->end(),_lf2->begin()+1,_lf2->end());
                            }
                            r_points.erase(_lf2);
                        }
                        else if(*(_lf2->end()-1)==_vf[0]){
                            if(*(_lf->begin())==_vf[0]){  // Jointure debut lf = fin lf2
                                _lf->insert(_lf->begin(),_lf2->begin(),_lf2->end()-1);
                            }
                            else {                        // Jointure fin lf = fin lf2
                                for(_itf=_lf2->end()-2;_itf!=_lf2->begin();_itf--)_lf->push_back(*_itf);
                                _lf->push_back(*_lf2->begin());
                            }
                            r_points.erase(_lf2);
                        }
                    }
                }
            }
        }

        // Test
        assert(r_points.size()<=1);
        if(r_points.size()==0){
            fclose(errfile);
            return Point2ArrayPtr();
        }

        _lf=r_points.begin();
        while(_lf!=r_points.end()&&_lf->empty()) _lf++;
        if(*(_lf->begin())==*(_lf->end()-1))_lf->erase(_lf->end()-1);
        if(_lf!=r_points.end()){
            vector<Vector2>::iterator _itf2=_lf->begin();
            for(_itf=_lf->begin()+1;_itf!=_lf->end();_itf++)if(_itf2->y()>_itf->y())_itf2=_itf;
            float x,x2;
            if(_itf2!=(_lf->end()-1))x=(_itf2+1)->x();
            else x=_lf->begin()->x();
            if(_itf2!=_lf->begin())x2=(_itf2-1)->x();
            else x2=(_lf->end()-1)->x();
            if(x2<x){
                if(_itf2!=_lf->end())
                    _result->insert(_result->begin(),_itf2,_lf->end());
                if(_itf2!=_lf->begin())
                    _result->insert(_result->end(),_lf->begin(),_itf2);
            }
            else{
                _result->push_back(*_itf2);
                for(vector<Vector2>::iterator _itf3=_itf2;_itf3!=_lf->begin();){
                    _itf3--; _result->push_back(*_itf3);}
                for(vector<Vector2>::iterator _itf3b=_lf->end()-1;_itf3b!=_itf2;){
                    _result->push_back(*_itf3b);_itf3b--;}
            }
        }
        qh_freeqhull(!qh_ALL);
        qh_memfreeshort (&curlong, &totlong);
        if (curlong || totlong)
            cerr << "qhull internal warning (main): did not free "<< totlong << "  bytes of long memory ("<< curlong <<" pieces)" << endl;

    }
    fclose(errfile);
    return _result;
#endif
}

/* ----------------------------------------------------------------------- */

GeometryPtr Fit::extrusion(){
    if(! __pointstofit )return GeometryPtr();
    if(__radius && __pointstofit->size() != __radius->size()){
	cerr << "Not valid data!" << endl;
	return GeometryPtr();
    }
    if(!__default_crossSection){
	Point2ArrayPtr _crossSection(new Point2Array(9));
	_crossSection->setAt(0,Vector2(0.5,0));
	_crossSection->setAt(1,Vector2(0.353553,0.353553));
	_crossSection->setAt(2,Vector2(-2.18557e-08,0.5));
	_crossSection->setAt(3,Vector2(-0.353553,0.353553));
	_crossSection->setAt(4,Vector2(-0.5,-4.37114e-08));
	_crossSection->setAt(5,Vector2(-0.353553,-0.353553));
	_crossSection->setAt(6,Vector2(6.55671e-08,-0.5));
	_crossSection->setAt(7,Vector2(0.353553,-0.353553));
	_crossSection->setAt(8,Vector2(0.5,0));
	__default_crossSection = Curve2DPtr(new Polyline2D(_crossSection));
	__default_crossSection->setName(string("Default_CrossSection"));
    }
    if(__pointstofit->size() < 2) return GeometryPtr();
    else if(__pointstofit->size() == 2){
	return frustum();
    }
    else {

	if(!__radius){
	    return GeometryPtr(new Extrusion(nurbsCurve(),
					     __default_crossSection));
	}
	else{
	  uint_t size = 1; 
	  bool _double = false;
	  bool _unique = true;
	  for(Point2Array::iterator _it= __radius->begin()+1; _it !=__radius->end(); _it++){
		if(*_it==*(_it-1)){
		  if(!_double){ 
			size++;
			_double = true; 
		  }
		}
		else {
		  _unique = false;
		  _double = false; 
		  size++;
		}
	  }
	  if(_unique ){
		if(__radius->getAt(0) == Vector2(1,1))
		  return GeometryPtr(
		  new Extrusion(nurbsCurve(),
		  __default_crossSection));
		else
		  return GeometryPtr(
		  new Extrusion(nurbsCurve(),
		  __default_crossSection,
		  Point2ArrayPtr(
		  new Point2Array(1,__radius->getAt(0)))));
	  }
	  else if(size <= (__radius->size()*2/3)){
		return GeometryPtr(new Extrusion(nurbsCurve(),
		  __default_crossSection,
		  __radius));
	  }
	  else {
		Point2ArrayPtr _radius(new Point2Array(size));
		RealArrayPtr _knot(new RealArray(size));
		uint_t _i = 1, _j =1; 
		_double = false;
		real_t interval = __radius->size()-1;
		_radius->setAt(0,__radius->getAt(0));_knot->setAt(0,0.0);
		for(Point2Array::iterator _it= __radius->begin()+1; _it !=__radius->end(); _it++){
		  if(*_it == *(_it-1)){
			if(!_double){
			  _double = true;
			  _radius->setAt(_i,__radius->getAt(_j));
			  _knot->setAt(_i,((real_t)_j /interval));
			  _i++;
			}
			else {
			  _knot->setAt(_i-1,((real_t)_j /interval));
			}
		  }
		  else{ 
			if(_double)_double = false;
			_radius->setAt(_i,__radius->getAt(_j));
			_knot->setAt(_i,((real_t)_j /interval));
			_i++;
		  }
		  _j++;
		}
		return  GeometryPtr(new Extrusion(nurbsCurve(),
		  __default_crossSection,
		  _knot,_radius));
	  }
	}
    }
}


/* ----------------------------------------------------------------------- */


LineicModelPtr 
Fit::nurbsCurve(){
  if(!__pointstofit){
    cerr << get_filename(__FILE__) << ":" << __LINE__ << " : " << "No point to fit with a Nurbs Curve." << endl;
    return LineicModelPtr();
  }
  if(__pointstofit->size()<4)
    return LineicModelPtr(new Polyline(__pointstofit));
  else {
	LineicModelPtr line = leastSquares(__pointstofit,3,
					 max(uint_t(4),min(__pointstofit->size()/3,uint_t(10))));
	if(!line){
	  cerr << get_filename(__FILE__) << ":" << __LINE__ << " : Error with leastSquares computation." << endl;
	  return LineicModelPtr(new Polyline(__pointstofit));
	}
	NurbsCurvePtr nurbs = dynamic_pointer_cast<NurbsCurve>(line);
	if(nurbs){
	  if(nurbs->getCtrlPointList()->size() == 4){
		return LineicModelPtr(new BezierCurve(nurbs->getCtrlPointList(),min((__pointstofit->size()-1),uint_t(10))));
	  }
	  else return line;
	}
	else {
	  cerr << get_filename(__FILE__) << ":" << __LINE__ << " : Error with computed object." << endl;
	  return line;
	}
  }
}

LineicModelPtr Fit::nurbsCurve(const Point3ArrayPtr & Q, int degC, int n){
  if(!Q  || Q->size() < n ){
	  cerr << get_filename(__FILE__) << ":" << __LINE__ << " : " << "Not enougth points (" << (Q?Q->size():0) << ") to fit with a Nurbs Curve." << endl;
    return LineicModelPtr();
  }
  if(degC > n - 1 ){
    cerr << get_filename(__FILE__) << ":" << __LINE__ << " : " << "Not enougth control points to fit a Nurbs Curve." << endl;
    return LineicModelPtr();
  }
  LineicModelPtr line = leastSquares(Q,degC,n);
	if(!line){
	  cerr << get_filename(__FILE__) << ":" << __LINE__ << " : Error with leastSquares computation." << endl;
	  return LineicModelPtr(new Polyline(Q));
	}
	NurbsCurvePtr nurbs = dynamic_pointer_cast<NurbsCurve>(line);
	if(nurbs){
	  if(nurbs->getCtrlPointList()->size() == degC+1){
		return LineicModelPtr(new BezierCurve(nurbs->getCtrlPointList()));
	  }
	  else return line;
	}
	else {
	  cerr << get_filename(__FILE__) << ":" << __LINE__ << " : Error with computed object." << endl;
	  return line;
	}
}

LineicModelPtr Fit::leastSquares(const Point3ArrayPtr & Q, int degC, int n){
    if(!Q){
      cerr << get_filename(__FILE__) << ":" << __LINE__ << " : " << "No point to fit with a Nurbs Curve." << endl;
      return LineicModelPtr();
    }
	real_t totalLength;
    return leastSquares(Q,degC,n,chordLengthParam(Q,totalLength));
}


RealArrayPtr Fit::chordLengthParam(const Point3ArrayPtr &Q,real_t& totalLength){
  uint_t i ;
  RealArrayPtr ub(new RealArray(Q->size()));
  totalLength = 0;

  ub->setAt(0, 0) ;
  for(i=1;i<ub->size();i++){
      totalLength += norm(Q->getAt(i)-Q->getAt(i-1)) ;
  }
  if(totalLength>0){
    for(i=1;i<ub->size()-1;++i){
      ub->setAt(i, ub->getAt(i-1) + norm(Q->getAt(i)-Q->getAt(i-1))/totalLength) ;
    }
    ub->setAt(ub->size()-1, 1.0) ; // In case there is some addition round-off
  }
  else{
    for(i=1;i<ub->size()-1;++i)
      ub->setAt(i, real_t(i)/real_t(ub->size()-1)) ;
    ub->setAt(ub->size()-1, 1.0) ; // In case there is some addition round-off
  }
  return ub;
}


LineicModelPtr Fit::leastSquares(const Point3ArrayPtr & Q,
                                int degC, int n, const RealArrayPtr& ub){
    int i,j;
    real_t d,a ;
    if(ub->size() != Q->size()){
	cerr << get_filename(__FILE__) << ":" << __LINE__ << " : " << "Wrong size of Estimate  Knot Vector." << endl;
	return LineicModelPtr();
    }
    RealArrayPtr U(new RealArray(n+degC+1,1.0));

    // Changing the method to generate a U compare to the one
    // described by Piegl and Tiller in the NURBS book (eq 9.69)

    d = (real_t)(Q->size())/(real_t)(n) ;
    for(j=0;j<=degC;++j)
        U->setAt(j, 0) ;

    for( j = 1 ; j < n-degC ; ++j ){
        U->setAt( degC+j , 0.0) ;

        for( int k = j ; k<j+degC ; ++k ){
            i = (int)(k*d) ;
            a = real_t(k*d)-real_t(i) ;
            int i2 = (int)((k-1)*d) ;
            U->setAt(degC+j, U->getAt(degC+j) +
                     a*ub->getAt(i2)+(1-a)*ub->getAt(i)) ;
        }
        U->setAt(degC+j, U->getAt(degC+j) / degC) ;
    }

    return leastSquares(Q, degC, n, ub, U) ;
}

LineicModelPtr Fit::leastSquares(const Point3ArrayPtr & Q,
                                int degC, int n, const RealArrayPtr& ub,
                                const RealArrayPtr& knot){

    uint_t span;
    const uint_t& m=Q->size() ;

    if(ub->size() != Q->size()){
	cerr << get_filename(__FILE__) << ":" << __LINE__ << " : " << "Wrong size of Estimate Knot Vector." << endl;
	return LineicModelPtr();
    }
    if(knot->size() != (uint_t)n+degC+1){
	cerr << get_filename(__FILE__) << ":" << __LINE__ << " : " << "Wrong size of Knot Vector." << endl;
	return LineicModelPtr();
    }

    Point4ArrayPtr P(new Point4Array(n));

    Point3ArrayPtr R(new Point3Array(n)), rk(new Point3Array(m));
    RealArrayPtr funs;
    DoubleArray2 N(m,uint_t(n),double(0)) ;
    R->setAt(0,Q->getAt(0));
    R->setAt(n-1,Q->getAt(m-1));

    // Set up N
    N.setAt(0,0,1.0) ;
    N.setAt(m-1,n-1,1.0) ;

//    cerr << "Knot : " << *knot << endl;

    for(uint_t i=0;i<m;i++){
//      cerr << "u = " << ub->getAt(i) << endl;
        span = findSpan(ub->getAt(i),degC,knot) ;
//      cerr << "Span = " << span << endl;
        funs = basisFunctions(span,ub->getAt(i),degC,knot);
        //cerr << "funs = " << *funs << endl;
        for(int j=0;j<=degC;++j){ // BOOO
            N.setAt(i,span-degC+j , (double)funs->getAt(j)) ;
        }
        rk->setAt(i, Q->getAt(i)-Q->getAt(0)*(real_t)N.getAt(i,0)-
                  Q->getAt(m-1)*(real_t)N.getAt(i,n-1)) ;
    }

    // Set up R
    for(int i2=0;i2<n;i2++){
		R->setAt(i2,Vector3::ORIGIN) ;
		for(uint_t j=0;j<m;j++){
			R->setAt(i2, R->getAt(i2)+ rk->getAt(j)*(real_t)(N.getAt(j,i2))) ;
		}
		Vector3 a = R->getAt(i2);
		if(
			a.x()*a.x()<GEOM_EPSILON && 
			a.y()*a.y()<GEOM_EPSILON &&
			a.z()*a.z()<GEOM_EPSILON){
			cerr << "\x0d" <<get_filename(__FILE__) << ":" << __LINE__ << " : " << "Precision Error(" << GEOM_EPSILON << ") : " << a << endl;
		return LineicModelPtr() ;
		}
    }

    // Solve      N^T*N*P = R

    // must check for the case where we want a curve of degree 1 having
    // only 2 points.
    if(n-2>0){ 
	DoubleArray2 X(n-2,3),B(uint_t(n-2),uint_t(3),double(0));
	for(uint_t i=0;i<B.getRowsNb();i++){
//	    cerr << "B(" << i << ",0)=" << R->getAt(i+1).x() << endl;
	    B.setAt(i,0,(double)(R->getAt(i+1).x()) );
//	    cerr << "B(" << i << ",1)=" << R->getAt(i+1).y() << endl;
	    B.setAt(i,1,(double)(R->getAt(i+1).y()) );
//	    cerr << "B(" << i << ",2)=" << R->getAt(i+1).z() << endl;
	    B.setAt(i,2,(double)(R->getAt(i+1).z()) );
	}
	DoubleArray2 Ns = N.get(1,1,m-2,n-2);
	if(Ns.size() == 0){
	    cerr << get_filename(__FILE__) << ":" << __LINE__ << " : " << "Error in getting sub-matrix." << endl;
	    return LineicModelPtr();
	}
//	cerr << "Ns " << Ns.getRowsNb() <<"x" << Ns.getColsNb() <<" : " << Ns << endl;
	DoubleArray2 TNs = transpose(Ns);
//	cerr << "N^T " << TNs.getRowsNb() <<"x" << TNs.getColsNb() <<" : " << TNs << endl;
	DoubleArray2 Mult = TNs*Ns;
//	cerr << "N^T*N " << Mult.getRowsNb() <<"x" << Mult.getColsNb() <<" : " << Mult  << endl;
//	cerr << "B : " << B << endl;
	solve(TNs*Ns,B,X) ;

	for(uint_t i3=0;i3<X.getRowsNb();i3++){
	    P->setAt(i3+1,Vector4((real_t)X.getAt(i3,0),(real_t)X.getAt(i3,1),(real_t)X.getAt(i3,2),1.0));
	}
    }
    P->setAt(0, Vector4(Q->getAt(0),1)) ;
    P->setAt(n-1, Vector4(Q->getAt(m-1),1)) ;
    return  LineicModelPtr(new NurbsCurve(P,knot,degC,uint_t(Q->size()-1))) ;
}
/*

LineicModelPtr
Fit::selectModel(real_t ki, Point3ArrayPtr pts, real_t& kirestant){
  real_t deg = __degree;
  LineicModelPtr res;
  real_t nnmax = (ki -sizeof(int))/(real_t)(5*sizeof(real_t))-(deg+1)/5.0;
  if(nnmax >=deg+1){
	  res = leastSquares(pts,deg,nnmax);
	  NurbsCurvePtr nurbs;
	  if(nurbs.cast(res)){
	  if(nurbs->getCtrlPointList()->size() == deg+1){
		res = LineicModelPtr(new BezierCurve(nurbs->getCtrlPointList(),deg));
		kirestant = ki - 3*nurbs->getCtrlPointListSize()*sizeof(real_t)+sizeof(int);
	  }
	  else {
		kirestant = ki - (5*nurbs->getCtrlPointListSize()+deg)*sizeof(real_t)+sizeof(int);
	  }
  }
}
*/
