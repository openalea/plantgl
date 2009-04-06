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

#include "branchcompressor.h"

#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/geometry/pointset.h>
#include <plantgl/scenegraph/geometry/extrusion.h>
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/scenegraph/geometry/disc.h>
#include <plantgl/scenegraph/geometry/frustum.h>
#include <plantgl/scenegraph/geometry/cylinder.h>
#include <plantgl/scenegraph/transformation/scaled.h>
#include <plantgl/scenegraph/transformation/oriented.h>
#include <plantgl/scenegraph/transformation/translated.h>
#include <plantgl/scenegraph/geometry/group.h>
#include <plantgl/scenegraph/container/geometryarray2.h>
#include "fit.h"
#include <plantgl/tool/timer.h>
#include <plantgl/math/util_math.h>
#include <plantgl/tool/dirnames.h>
#include <plantgl/tool/util_hashmap.h>

#include <stdio.h>  
#include <fstream>
 
#include <stack>

TOOLS_USING_NAMESPACE
PGL_USING_NAMESPACE
using namespace std;
STDEXT_USING_NAMESPACE


/* ----------------------------------------------------------------------- */

#define PR     if(__verbose)printf

/* ----------------------------------------------------------------------- */

Point3ArrayPtr  
PGL(discretizeWithCurvature)(NurbsCurvePtr C,int NumberOfPoint){
  if(!C)return Point3ArrayPtr();
  Point3ArrayPtr theVector(new Point3Array(NumberOfPoint));
  theVector->setAt(0,C->getPointAt(C->getFirstKnot()));
  theVector->setAt(NumberOfPoint-1,C->getPointAt(C->getLastKnot()));
  if(NumberOfPoint > 2){
	Vector3 Zero(0,0,0);
	real_t courbureG=0.0;
	{
	  for(real_t i=C->getFirstKnot();i<C->getLastKnot();i+=0.001f){
		real_t courbureL=0.0;
		real_t courbureLden =0.0;
		
		Vector4 C1_u = C->getDerivativeAt(i,1);
		Vector4 C2_u = C->getDerivativeAt(i,2);
		
		
		courbureL=sqrt(sq((C1_u.y()*C2_u.z()-C1_u.z()*C2_u.y()))
		  +sq((C1_u.z()*C2_u.x()-C1_u.x()*C2_u.z()))
		  +sq((C1_u.x()*C2_u.y()-C1_u.y()*C2_u.x())));
		
		courbureLden= pow((sqrt(C1_u.x() * C1_u.x() 
		  + C1_u.y() * C1_u.y() 
		  + C1_u.z()* C1_u.z() )),3);
		if(courbureLden!=0.0){
		  courbureL=courbureL/courbureLden;
		  courbureG+=courbureL;
		}
	  }
	}
	
#ifdef GEOM_DEBUG
	printf("Global Curvature : %f\n",(courbureG*0.001));
#endif
	if(courbureG > GEOM_EPSILON){
	  real_t courbureSomme=0.0;
	  real_t courbureStep=courbureG/(NumberOfPoint-1);
	  real_t courbureA = courbureStep;
	  int index=1;
	  for(real_t i=C->getFirstKnot();
	  i<C->getLastKnot() && index<(NumberOfPoint-1);i+=0.001f){
		real_t courbureL = 0.0;
		real_t courbureLden =0.0;
		Vector4 C1_u = C->getDerivativeAt(i,1);
		Vector4 C2_u = C->getDerivativeAt(i,2);
		
		courbureL=sqrt(sq((C1_u.y()*C2_u.z()-C1_u.z()*C2_u.y()))
		  +sq((C1_u.z()*C2_u.x()-C1_u.x()*C2_u.z()))
		  +sq((C1_u.x()*C2_u.y()-C1_u.y()*C2_u.x())));
		courbureLden= pow((sqrt(C1_u.x() * C1_u.x() 
		  + C1_u.y() * C1_u.y() 
		  + C1_u.z()* C1_u.z() )),3);
		if(courbureLden!=0.0){
		  courbureL=courbureL/courbureLden;
		  courbureSomme+=courbureL;
		}
		if(courbureSomme >= courbureA){
		  theVector->setAt(index,C->getPointAt(i));
#ifdef GEOM_DEBUG
		  if(theVector->getAt(index)==Zero)
			printf("Point Calcul error ! (%s:%s)\n",get_filename(__FILE__).c_str(),__LINE__);
		  printf("Find the %d (of %d) interior point  at u=%f: %f - %f -> <%f,%f,%f>\n",
			index,(NumberOfPoint),i,courbureSomme*0.001,courbureA*0.001,
			theVector->getAt(index).x(),theVector->getAt(index).y(),theVector->getAt(index).z());
#endif
		  index++;
		  courbureA+=courbureStep;
		} 
	  }
	}
	else {
	  theVector = Point3ArrayPtr(new Point3Array(2));
	  theVector->setAt(0,C->getPointAt(C->getFirstKnot()));
	  theVector->setAt(1,C->getPointAt(C->getLastKnot()));
	}
  }
  return theVector;
}
										       
Point3ArrayPtr  
PGL(compressPolyline)(Point3ArrayPtr C,int NumberOfPoint){
  if(!C)return Point3ArrayPtr();
  if(C->size() <= NumberOfPoint) return C;
  Point3ArrayPtr theVector(new Point3Array(NumberOfPoint));
  theVector->setAt(0,C->getAt(0));
  theVector->setAt(NumberOfPoint-1,C->getAt(C->size()-1));
  if(NumberOfPoint > 2){
	real_t polygonLength;
	RealArrayPtr ub = Fit::chordLengthParam(C,polygonLength) ;
	real_t step = 1.0 / (real_t)(NumberOfPoint -1);
	real_t s = ub->getAt(0) + step;
	int j = 0;
	for(int i = 1 ; i < NumberOfPoint - 1; i++){
	  while(j < ub->size() && ub->getAt(j) < s)j++;
	  real_t u1 = ub->getAt(j) - s;
	  real_t u2 = s - ub->getAt(j-1);
	  real_t interv = ub->getAt(j) - ub->getAt(j-1);
	  u1 /= interv; u2 /= interv;
	  theVector->setAt(i,C->getAt(j)*u2 + C->getAt(j-1)*u1);
	  s += step;
	}
  }
  return theVector;
}

/* ----------------------------------------------------------------------- */
/*
NurbsCurvePtr 
PGL(fitt)(const Point3ArrayPtr&  MyVector, 
				   int deg, 
				   int CPbegin, 
				   int nbPtCtrlMax, 
				   real_t ErreurBound, 
				   int &  Nbloop, 
				   real_t & SommeEk){
  
  //######################################
  //#      Approximation des points      # 
  //######################################
  
  NurbsCurvePtr C(0); 
  NurbsCurvePtr C2(0); 
  NurbsCurvePtr Cold(0);
  real_t EkOld=0;
  real_t polygonLength=0.0;
  int k=MyVector->size();
  RealArrayPtr ub;
  ub = Fit::chordLengthParam(MyVector,polygonLength) ;
  real_t oldLength=polygonLength;
  real_t localLength=0.0;
  int NoErr=1;
  int ok=1;
  int backup=0;
  
  do{
    int nbtour=0;EkOld=0;SommeEk=0;
    while(((nbtour<3)||(EkOld-SommeEk>0.01))&&ok){
	  C.cast(Fit::leastSquares(MyVector,deg,CPbegin,ub));
      if(!C){
		NoErr=0;
		printf("%s:%d:Error with least Squares Function when n=%d at %d loop.\n",
		  get_filename(__FILE__).c_str(),__LINE__,CPbegin,nbtour);
		if(nbtour==0){
		  if(CPbegin>deg+1 && Cold.isValid()){
			C = Cold;
		  }
		  CPbegin++;
		  break;
		}
		else{
		  if(C2.isValid()){
			C = C2;
			CPbegin++;
			NoErr = 1;
		  }
		  break;
		}
      }
	  else C2 = C;
      EkOld=SommeEk;
      SommeEk=0;
      nbtour++;
      for(int i=0;i<MyVector->size();i++){
		real_t u_i ; 
		Vector3 r_i = C->projectTo(MyVector->getAt(i),ub->getAt(i),u_i) ;
		SommeEk+=norm(r_i-MyVector->getAt(i)) ;
		ub->setAt(i,u_i);	
      }
      if(nbtour==1&&CPbegin>deg+1){
		double delta;
		if((delta=fabs(C->getLength()-polygonLength))>oldLength*3/2){
		  break;}
      }
    }
	if(NoErr < 1)break;
	if(C)localLength=C->getLength();
	else localLength=FLT_MAX;
    if(fabs(localLength-polygonLength)>oldLength) ok=0;
    if(C)oldLength=fabs(localLength-polygonLength);
    if(NoErr){
#ifdef GEOM_DEBUG
      printf("< %4d - %2d >Fitting error : %f -*- Length Delta : %f\n",nbtour,CPbegin,SommeEk,(localLength-polygonLength));
#endif
      if(SommeEk>(ErreurBound*k)&&ok){
		backup=1;
	    Cold = C;
		CPbegin++;
      }
      else if(backup){
		C = Cold;
		backup=0;
      }
    }	
    Nbloop+=nbtour;
  }while(((SommeEk>(ErreurBound*k)||NoErr<=0)&&ok)&&CPbegin<=nbPtCtrlMax);
  if(!C){
	return NurbsCurvePtr();
  }
  return C;
}
*/
/* ----------------------------------------------------------------------- */

NurbsCurvePtr 
PGL(fitt)(const Point3ArrayPtr&  MyVector, 
				   int deg, 
				   int CPbegin, 
				   int nbPtCtrlMax, 
				   real_t ErreurBound, 
				   int &  Nbloop, 
				   real_t & SommeEk){
  
  //######################################
  //#      Approximation des points      # 
  //######################################
  
  NurbsCurvePtr C; 
  NurbsCurvePtr C2; 
  NurbsCurvePtr Cold;
  real_t EkOld=0;
  real_t polygonLength=0.0;
  int k=MyVector->size();
  RealArrayPtr ub;
  ub = Fit::chordLengthParam(MyVector,polygonLength) ;
  real_t oldLength=polygonLength;
  real_t localLength=0.0;
  int NoErr=1;
  int ok=1;
  int backup=0;
  
  do{
    EkOld=0;SommeEk=0;
	C = dynamic_pointer_cast<NurbsCurve>(Fit::leastSquares(MyVector,deg,CPbegin,ub));
    if(!C){
		NoErr=0;
		printf("%s:%d:Error with least Squares Function when n=%d.\n",
		  get_filename(__FILE__).c_str(),__LINE__,CPbegin);
		  if(CPbegin>deg+1 && Cold)C = Cold;
		  CPbegin++;
    }
    else C2 = C;
	if(C){
	  EkOld=SommeEk;
	  SommeEk=0;
	  for(int i=0;i<MyVector->size();i++){
		real_t u_i ; 
		Vector3 r_i = C->projectTo(MyVector->getAt(i),ub->getAt(i),u_i) ;
		SommeEk+=norm(r_i-MyVector->getAt(i)) ;
		ub->setAt(i,u_i);	
	  }
	  if(CPbegin>deg+1){
		double delta;
		if((delta=fabs(C->getLength()-polygonLength))>oldLength*3/2)break;
	  }
	}
	if(NoErr < 1)break;
	if(C)localLength=C->getLength();
	else localLength=FLT_MAX;
    if(fabs(localLength-polygonLength)>oldLength) ok=0;
    if(C)oldLength=fabs(localLength-polygonLength);
    if(NoErr){
#ifdef GEOM_DEBUG
      printf("< %2d >Fitting error : %f -*- Length Delta : %f\n",CPbegin,SommeEk,(localLength-polygonLength));
#endif
      if(SommeEk>(ErreurBound*k)&&ok){
		backup=1;
	    Cold = C;
		CPbegin++;
      }
      else if(backup){
		C = Cold;
		backup=0;
      }
    }	
  }while(((SommeEk>(ErreurBound*k)||NoErr<=0)&&ok)&&CPbegin<=nbPtCtrlMax);
  if(!C){
	return NurbsCurvePtr();
  }
  return C;
}

/* ----------------------------------------------------------------------- */

Curve2DPtr BranchCompressor::DEFAULT_CROSS_SECTION;

/* ----------------------------------------------------------------------- */

BranchCompressor::BranchCompressor():__verbose(false),__roots(-1){
  if(!DEFAULT_CROSS_SECTION){
	int slices = 6;
	Point2ArrayPtr pts(new Point2Array(slices+1));
	for(int i = 0; i <= slices; i++){
	  pts->setAt(i,Vector2::Polar(1,GEOM_TWO_PI*i/slices));
	}
	DEFAULT_CROSS_SECTION = Curve2DPtr(new Polyline2D(pts));
	DEFAULT_CROSS_SECTION->setName("BrCompressorCrossSection");
  }
};

void 
BranchCompressor::setData(const std::vector<BranchInput>& i, 
						  int sorttype){
  clear();
  switch(sorttype){
  case(0):__inputs = sortByDecOrder(i);break;
  case(1):__inputs = sortByIncOrder(i);break;
  case(2):__inputs = sortByDecSize(i);break;
  case(3):__inputs = sortByIncSize(i);break;
  case(4):__inputs = i;break;
  default:__inputs = sortByDecOrder(i);break;
  }
/*  hash_map<int,int> l;
  for(vector<BranchInput>::const_iterator _it1 = __inputs.begin();
	_it1!=__inputs.end();_it1++){
	  l[_it1->points->size()]++;
	}
	for(hash_map<int,int>::const_iterator  _it2 = l.begin();
	_it2 != l.end(); _it2++)
	  cerr << "Length = " << _it2->first << ", Nb = " << _it2->second << endl;*/
}

std::vector<BranchInput> 
BranchCompressor::sortByIncSize(const std::vector<BranchInput>&i){
//  cerr << "sortByIncreasingSize" << endl;
  vector<BranchInput>::const_iterator _it1 = i.begin();
  vector<BranchInput> res;
  vector<BranchInput>::iterator _i1 = res.begin();

  for(;_it1!=i.end();_it1++){
	if(res.empty()){
	  res.push_back(*_it1);
	}
	else {
	  _i1 = res.begin();
	  while(_i1 != res.end() &&
			_i1->points->size() > _it1->points->size()){
		_i1++;
	  }
	  res.insert(_i1,*_it1);
	}
  }
  return res;
}

std::vector<BranchInput> 
BranchCompressor::sortByDecSize(const std::vector<BranchInput>&i)
{
//  cerr << "sortByDecreasingSize" << endl;
  vector<BranchInput>::const_iterator _it1 = i.begin();
  vector<BranchInput> res;
  vector<BranchInput>::iterator _i1 = res.begin();

  for(;_it1!=i.end();_it1++){
	if(res.empty()){
	  res.push_back(*_it1);
	}
	else {
	  _i1 = res.begin();
	  while(_i1 != res.end() &&
			_i1->points->size() < _it1->points->size()){
		_i1++;
	  }
	  res.insert(_i1,*_it1);
	}
  }
  return res;
}

typedef pgl_hash_map<int,pair<int,int> > GraphMap;

std::vector<BranchInput> 
BranchCompressor::sortByIncOrder(const std::vector<BranchInput>&i)
{
//  cerr << "sortByIncreasingOrder" << endl;
  GraphMap graph;
  for(std::vector<BranchInput>::const_iterator _it = i.begin(); _it != i.end(); _it++)
	graph[_it->id] = pair<int,int>(_it->father,(_it->father <=0?0:-1));
  stack<GraphMap::iterator> st;
  for(GraphMap::iterator _it2 = graph.begin(); _it2 != graph.end(); _it2++){
	if(_it2->second.second == -1 ){
	  int f = _it2->second.first;
	  GraphMap::iterator _it3 = graph.find(f);
	  while( _it3 != graph.end() && _it3->second.second == -1 ){
		st.push(_it3);_it3 = graph.find(_it3->second.first);
	  }
	  int order = 0;
	  if(_it3 != graph.end())order = _it3->second.second +1;
	  while(!st.empty()){
		_it3 = st.top();
		_it3->second.second = order; order++;
		st.pop();
	  }
	  _it2->second.second = order;
	}
  }
  vector<BranchInput>::const_iterator _it1 = i.begin();
  vector<BranchInput> res;
  vector<BranchInput>::iterator _i1 = res.begin();

  for(;_it1!=i.end();_it1++){
	if(res.empty()){
	  res.push_back(*_it1);
	}
	else {
	  _i1 = res.begin();
	  while(_i1 != res.end() && 
		     (graph[_i1->id].second < graph[_it1->id].second || 
			 (graph[_i1->id].second == graph[_it1->id].second && _i1->points->size() > _it1->points->size()))){
		_i1++;
	  }
	  res.insert(_i1,*_it1);
	}
  }
  return res;
}

std::vector<BranchInput> 
BranchCompressor::sortByDecOrder(const std::vector<BranchInput>&i)
{
//  cerr << "sortByDecreasingOrder" << endl;
  GraphMap graph;
  for(std::vector<BranchInput>::const_iterator _it = i.begin(); _it != i.end(); _it++){
	graph[_it->id] = pair<int,int>(_it->father,(_it->father <=0?0:-1));
	if(_it->father <=0)__roots= _it->id;
  }
  std::stack<GraphMap::iterator> st;
  for(GraphMap::iterator _it2 = graph.begin(); _it2 != graph.end(); _it2++){
	if(_it2->second.second == -1 ){
	  int f = _it2->second.first;
	  GraphMap::iterator _it3 = graph.find(f);
	  while( _it3 != graph.end() && _it3->second.second == -1 ){
		st.push(_it3);
		_it3 = graph.find(_it3->second.first);
	  }
	  int order = 0;
	  if(_it3 != graph.end())order = _it3->second.second +1;
	  while(!st.empty()){
		_it3 = st.top();
		_it3->second.second = order; order++;
		st.pop();
	  }
	  _it2->second.second = order;
	}
  }
  vector<BranchInput>::const_iterator _it1 = i.begin();
  vector<BranchInput> res;
  vector<BranchInput>::iterator _i1 = res.begin();

  for(;_it1!=i.end();_it1++){
	if(res.empty()){
	  res.push_back(*_it1);
	}
	else {
	  _i1 = res.begin();
	  while(_i1 != res.end() && 
		    (graph[_i1->id].second > graph[_it1->id].second || 
			 (graph[_i1->id].second == graph[_it1->id].second &&_i1->points->size() < _it1->points->size()))){
		_i1++;
	  }
	  res.insert(_i1,*_it1);
	}
  }
  return res;
}

void BranchCompressor::clear()
{
  __inputs.clear();
  __roots = -1;
}

void 
BranchCompressor::addScene(ScenePtr scene, int c_branch,
						   const Vector3& p1,const Vector3& p2) const {
  Point2ArrayPtr _radius = __inputs[c_branch].radius;
  if(p1 == p2){
	Vector2 radius = _radius->getAt(0);
	GeometryPtr geom;
	if(norm(radius) > GEOM_EPSILON){
	  geom = GeometryPtr(new Disc(radius.x()));
	  if(radius.x() != radius.y()){
	    geom = GeometryPtr(new Disc(1));
		geom = GeometryPtr(new Scaled(Vector3(radius,1),geom));
	  }
	  else geom = GeometryPtr(new Disc(radius.x()));
	  if(p1 != Vector3::ORIGIN)geom = GeometryPtr(new Translated(p1,geom));
	}
	else {
	  Point3ArrayPtr pts(new Point3Array(1,p1));
	  geom = GeometryPtr(new PointSet(pts));
	}
	Shape3DPtr a(new Shape(geom,__inputs[c_branch].appearance,__inputs[c_branch].id));
	assert(a->isValid());
	scene->add(a);
  }
  else {
	real_t botrad = _radius->getAt(0).x();
	real_t toprad = _radius->getAt(_radius->size()-1).x();
	real_t length = norm(p2-p1);
	GeometryPtr geom;
	if(botrad != toprad)
	  geom = GeometryPtr(new Frustum(botrad,length,toprad/botrad));
	else
	  geom = GeometryPtr(new Cylinder(botrad,length));
	Vector3 h = p2-p1;
	h.normalize();
	Vector3 u,v;
	real_t a = fabs(dot(h,Vector3::OZ));
	real_t b = fabs(dot(h,Vector3::OX));
	real_t c = fabs(dot(h,Vector3::OY));
	if(a < b && a < c)u = cross(h,Vector3::OZ);
	else if(b < c)    u = cross(h,Vector3::OX);
	else              u = cross(h,Vector3::OY);
    v = cross(h,u);
	u.normalize(); v.normalize();
	if(u != Vector3::OX && v != Vector3::OY){
	  geom = GeometryPtr(new Oriented(u,v,geom));
	}
	if(p1 != Vector3::ORIGIN)
	  geom = GeometryPtr(new Translated(p1,geom));
	Shape3DPtr sh(new Shape(geom,__inputs[c_branch].appearance,__inputs[c_branch].id));
	assert(sh);
	scene->add(sh);
  }
}

void 
BranchCompressor::addScene(ScenePtr scene, int c_branch,LineicModelPtr axis) const{
  if(axis && axis->isValid()){
	GeometryPtr geom;
	if(!axis->isExplicit()){
	  real_t polygonalLength;
	  RealArrayPtr ub;
	  ub = Fit::chordLengthParam(__inputs[c_branch].points,polygonalLength);
	  Point2ArrayPtr radius = __inputs[c_branch].radius;
	  removeDouble(radius,ub);
	  geom = GeometryPtr(new Extrusion(axis,DEFAULT_CROSS_SECTION,
										  ub,radius));
	}
	else {
	  RealArrayPtr ub;
	  Point2ArrayPtr radius = __inputs[c_branch].radius;
	  removeDouble(radius,ub);
	  if(!ub)geom = GeometryPtr(new Extrusion(axis,DEFAULT_CROSS_SECTION,
									   radius));
	  else geom = GeometryPtr(new Extrusion(axis,DEFAULT_CROSS_SECTION,
									   ub,radius));
	}
	Shape3DPtr a(new Shape(geom,__inputs[c_branch].appearance,__inputs[c_branch].id));
	assert(a->isValid());
	scene->add(a);
  }
}

void 
BranchCompressor::addScene(ScenePtr scene, int c_branch,GeometryPtr geom) const{
  if(geom&&geom->isValid()){
	Shape3DPtr a(new Shape(geom,__inputs[c_branch].appearance,__inputs[c_branch].id));
	assert(a->isValid());
	scene->add(a);
  }
}

/* ----------------------------------------------------------------------- */
ScenePtr
BranchCompressor::inputScene() const{
  int i = 0;
  ScenePtr scene(new Scene);    
  for(vector<BranchInput>::const_iterator _it = __inputs.begin();
  _it != __inputs.end(); _it++){
	if(_it->points->size() == 1)
	  addScene(scene, i,GeometryPtr(new PointSet(_it->points)));
	else if(_it->points->size() == 2)
	  addScene(scene,i,_it->points->getAt(0),_it->points->getAt(1));
	else addScene(scene,i,LineicModelPtr(new Polyline(_it->points)));
	i++;
  }
  return scene;
}

ScenePtr
BranchCompressor::compress(real_t taux,
		 real_t * obtainedrate,
		 real_t * computationtime,
		 real_t * _fittime,
		 real_t ErreurBound,
		 int degre,
		 int option){
	if(taux <= 0.0f) return inputScene();

    real_t SumError=0.0;
    Timer timer;
    Timer fittimer;
	double fittime = 0;
    int nbPtCtrlMax=-1;
  
	const vector<BranchInput>& GeometryVector = __inputs;
    Point3ArrayPtr  MyVector;
    vector<int> VectorNotRep(0);
    Vector3 Zero;

    int Nmin=1000;
    int Nmax=0;
    int Bmin=1000;
    int Bmax=0;
    int Pmin=1000;
    int Pmax=0;
    int Emax=0;

	ScenePtr scene(new Scene);
    
	int NbCourbe=0;
    int Nbloop=0;
    int NbBezierCurve=0;
    int NbPolyline=0;
    int NbPoint=0;
    int NbNotRep=0;
    int NbNotRep2=0;

    int DataIn2;
    int DataRemind;
    int DataOut=0;

	bool first_pass = true;


    timer.start();

    PR("Wanted Compression rate : %.2f %\n",taux);
    PR("Wanted degree : %d\n",degre);
    if(__verbose)cout << "Error Bound for the fitting :" << ErreurBound << "\n";


    DataIn2=0;
	{
	  for(vector<BranchInput>::const_iterator _it = GeometryVector.begin();
		  _it!=GeometryVector.end();_it++)
	  DataIn2+=_it->points->size();
	}
    DataIn2=DataIn2*3*sizeof(real_t); 
    int DataIn = DataIn2;
    DataRemind=(int)((real_t)DataIn2*(100.0-taux)/100.0);
	int DataRoot = 0;
	if(__roots > 0){
	  DataRoot = 6*sizeof(real_t);
	  if(DataRemind > DataRoot)DataRemind -= DataRoot;
	  else DataRoot = 0;
	}

    PR("Memory Ressource : %d - Data to use : %d\n",DataRemind,DataIn2);
    if(!__verbose)cout << "Generating 3D representation :\n"<<flush;


    /** Compression de la géometrie de chaque branche */

	int indexGeometry = GeometryVector.size();
//    int nbBranch=indexGeometry -1;
	int currentBranch = 0;
	int k = 0;
    bool working=false;
    //	int id = -2;
    if(indexGeometry!=0){
      working=true;
      MyVector=GeometryVector[currentBranch].points;
      k=MyVector->size();
    }
    while(working){
      if(GeometryVector[currentBranch].id == __roots){
		DataRemind += DataRoot;
	  }
      
      /* Les branches ayant plus de 3 points digitalisés */
      if(k>=3){
		int jj=0;
		NbCourbe++;
		jj=k; 
		nbPtCtrlMax=jj;
		int deg;
		if(jj < degre+1 )deg = jj - 1;
		else {deg = degre;jj=degre+1;}
		
		if(option==0){
		  real_t calcul = ( 3.0 * (real_t)k * (real_t)DataRemind/(real_t)DataIn2 -(real_t)deg -1.0) / 5.0;
		  calcul = calcul - ((real_t)sizeof(int))/(5.0*(real_t)sizeof(real_t));
		  nbPtCtrlMax = (int)calcul;
		}
		else if(option==1)nbPtCtrlMax = deg+1;
		else nbPtCtrlMax = 0;
		
		if(nbPtCtrlMax<deg+1){
		  real_t calcul = ((4.0 *(real_t)(deg + 1) +((real_t)sizeof(int)/(real_t)sizeof(real_t)))*(real_t)DataIn2)/(3.0*(real_t)DataRemind);
		  if(k>calcul)nbPtCtrlMax = deg+1;
		}
		
		/** Ajustement d'une courbe de Bézier ou d'une Nurbs */
		if((nbPtCtrlMax>=deg+1)&&(option!=2)){
		  
		  NurbsCurvePtr C;
		  
		  PR("## %d-th Curve(s) : %2d ##\n",NbCourbe,k);  
		  PR("<#> Digitized Point Vector Size  : %d.\n",k);
		  PR("<#> Wanted degree    : %d.\n",deg);
		  PR("<#> Estimated Max Point Control Number    : %d.\n",nbPtCtrlMax);
		  
		  real_t SommeEk=0;
		  fittimer.start();
		  C = fitt(MyVector,deg, jj, nbPtCtrlMax, ErreurBound, Nbloop,SommeEk );
		  fittime += fittimer.stop();
		  SumError+=SommeEk; 
		  int localDataOut=sizeof(int);
		  if(C){
			PR("<#> Number of Control Points : %d.\n",C->getCtrlPoints()->size());
			localDataOut=localDataOut+( 4 * (C->getCtrlPoints()->size()) * sizeof(real_t) );
			if(C->getCtrlPoints()->size()==deg+1){
			  addScene(scene, currentBranch,LineicModelPtr(new BezierCurve(C->getCtrlPoints())));
			  // scene->push_back(GeometryPtr(new BezierCurve(C->getCtrlPoints())));
			  PR("Using Bezier curve ...\n");
			  NbBezierCurve++;			
			  if(k>Bmax)Bmax=k;
			  if(k<Bmin)Bmin=k;
			}
			else{
			  addScene(scene, currentBranch,LineicModelPtr(C));
			  //			scene->push_back(GeometryPtr(C));
			  PR("<#> Knot Vector Size         : %d.\n",C->getKnotList()->size());
			  PR("Using NURBS curve ...\n");
			  localDataOut+=((C->getKnotList()->size())*sizeof(real_t));
			  if(k>Nmax)Nmax=k;
			  if(k<Nmin)Nmin=k;
			}
		  }
		  DataOut+=localDataOut;    
		  DataRemind-=localDataOut;
		  real_t localrate=0;
		  localrate=(100-((real_t)localDataOut*100.0)/((real_t)k *sizeof(real_t) * 3.0));
		  PR("Compressed at %.2f %\n\n",localrate);
		  
		}
		/* Ajustement d'une ligne polygonale avec lissage*/
		else if(((real_t)k*(real_t)DataRemind)/(real_t)DataIn2 >=2.0){
		  
		  PR("## %d-th Curve(s) : %2d ##\n",NbCourbe,k);  
		  PR("<#> Digitized Point Vector Size  : %d.\n",k);
		  PR("<#> Using Polyline Model with smoothing.\n");
		  int maxPoint = (int)(((real_t)k * (real_t)DataRemind) / (real_t)DataIn2);
		  PR("<#> Estimated number of point left   : %d.\n",maxPoint);
		  if(k>Pmax)Pmax=k;
		  if(k<Pmin)Pmin=k;
		  
		  NbPolyline++;		  
		  
		  Point3ArrayPtr pts;
		  if(maxPoint > 3 && MyVector->size() > 3){
			NurbsCurvePtr C;
			if(k < degre+1 )deg = k - 1;
			else {deg = degre;}	    
			
			real_t SommeEk=0;
			fittimer.start();
			C = fitt(MyVector, deg, deg+1, deg+1, ErreurBound, Nbloop, SommeEk);
			// Methode utilisant la courbure générale
			if(C)pts = discretizeWithCurvature(C,maxPoint);
			fittime += fittimer.stop();
		  }
		  if(!pts) {
			fittimer.start();
			pts = compressPolyline(MyVector,maxPoint);
		    fittime += fittimer.stop();
		  }
		  if(pts){
			if(pts->size() == 2)
			  addScene(scene, currentBranch,
			  pts->getAt(0),pts->getAt(1));
			else addScene(scene, currentBranch,LineicModelPtr(new Polyline(pts)));
			DataOut+=(3*pts->size()*sizeof(real_t));
			DataRemind-=3*pts->size()*sizeof(real_t);
			PR("Compressed at %d %\n\n",(100-(pts->size()*100/k)));
		  }
		}
		else if(currentBranch< indexGeometry&&currentBranch>=0){
		  VectorNotRep.push_back(currentBranch);
		  if(Emax<k)Emax=k;
		}		
      }
      else if(k==2){
		
		if((real_t)DataRemind*(real_t)k*3.0/(real_t)DataIn2>=(real_t)k*3.0){
		  PR("## %d-th Curve(s) : 2 -> 2 ##\n",NbCourbe);  
		  if(k>Pmax)Pmax=k;
		  if(k<Pmin)Pmin=k;
		  NbPolyline++;
		  if(MyVector->size() == 2)
			addScene(scene, currentBranch,
			MyVector->getAt(0),MyVector->getAt(1));
		  else addScene(scene, currentBranch,LineicModelPtr(new Polyline(MyVector)));
		  // scene->push_back(GeometryPtr(new Polyline(MyVector)));
		  DataOut+=(6*sizeof(real_t));
		  DataRemind-=(6*sizeof(real_t));
		}
		else if(currentBranch<indexGeometry&&currentBranch>=0){
		  if(Emax<2)Emax=2;
		  VectorNotRep.push_back(currentBranch);
		}
      }
      else if(k==1){
		if((real_t)DataRemind/(real_t)DataIn2>=1.0){
		  NbCourbe++;
		  addScene(scene, currentBranch,GeometryPtr(new PointSet(MyVector)));
		  NbPoint++;
		  DataOut+=(3*sizeof(real_t));
		  DataRemind-=(3*sizeof(real_t));
		  
		}
		else if(currentBranch<indexGeometry&&currentBranch>=0){
		  if(Emax<1)Emax=1;
		  VectorNotRep.push_back(currentBranch);
		}		
      }
	  
	  
      DataIn2-=(3*k*sizeof(real_t));
	  
      if(currentBranch < GeometryVector.size()-1&& currentBranch>=0){
		if(!__verbose&&currentBranch>0){
		  printf("\x0dGenerate : %.2f % ",((real_t)currentBranch*100.0/(real_t)GeometryVector.size()));
		  cout <<flush;
		}		
		//		nbBranch--;
		currentBranch++;
		MyVector=GeometryVector[currentBranch].points;
		k=MyVector->size();
      } 
      else{
		if(DataRemind<=0)working=false;
		else if(first_pass && currentBranch >=GeometryVector.size()-1){
		  DataIn2=0;
		  for(int m=0;m<NbNotRep;m++)
			DataIn2+=(GeometryVector[VectorNotRep[m]].points->size());
		  DataIn2=DataIn2*3*sizeof(real_t); 
		  NbNotRep2=0;
		  //		  nbBranch=-1;
		  first_pass = false;
		  PR("Nb Curve Not Represented : %d - %d bytes to %d bytes\n",NbNotRep,DataIn2,DataRemind);
		  PR("Data Out : %d - Data In : %d - Rate : %.2f %\n",DataOut,DataIn2,100.0-(100.0*(real_t)DataOut/(real_t)DataIn2));
		  if(!__verbose){
			printf("\x0dGenerate : 100.00 %\n");
		  }
		}
		if(NbNotRep2<NbNotRep){
		  currentBranch = VectorNotRep[NbNotRep2];
		  MyVector=GeometryVector[currentBranch].points;
		  k=MyVector->size();
		  //PR("Curve %d -> %d of %d points. Remind ressource : %d for %d bytes\n",NbNotRep2,VectorNotRep[NbNotRep2],k,DataRemind,DataIn2);
		  NbNotRep2++;
		}
		else working=false;
      }
    }
	
    PR("Memory Ressource remind : %d - Data not used : %d \n",DataRemind, DataIn2);

    timer.stop();

    PR("\n##############################################\n");
    PR("# Number of Object in the list : %d\n",scene->size());
    PR("##############################################\n");
    PR("# Input  : %d bytes - %d Point 3D.\n",DataIn,(DataIn/(3*sizeof(real_t))));
    PR("# Output : %d bytes.\n",DataOut);
    taux=(100-((100.0*(real_t)DataOut)/((real_t)DataIn)));
    PR("# Compression : %.2f %\n",taux);
    PR("# Error Sum : %.2f .\n",SumError);
    PR("# Error Average : %.2f per Point 3D.\n",((SumError*3*sizeof(real_t))/DataIn));
    PR("#                 %.2f per Curve.\n",(SumError/GeometryVector.size()));
    PR("# Loop Number : %d .\n",Nbloop);
    if(!scene->empty()){
      PR("#   Average   : %d per Curve.\n",(Nbloop/scene->size()));}
    real_t timing = timer.elapsedTime();
    char time[100];
    if (timing > 60.0){
      int min=(int)(timing/60.0);
      real_t sec = timing - ((real_t)min*60.0);
      sprintf(time,"%d min %.2f",min,sec );
    }
    else sprintf(time,"%.2f",timing);
    PR("# Timer : %s sec.\n",time);
    else {
	  cerr << "Calculus Time : " << time << " sec [fit=" << fittime <<"]- Compression rate : " << taux << "% [" << DataOut << '/' << DataIn << ']' << endl;
	}
	if(obtainedrate)*obtainedrate = taux;
	if(computationtime)*computationtime = timing;
	if(_fittime)*_fittime = fittime;

    PR("# Number of Curve found : %d.\n",GeometryVector.size());
    if((scene->size()-NbBezierCurve)!=0)
      PR("# NURBS : %.2f % (%d : Max %d, Min %d).\n",((real_t)scene->size()-(real_t)NbBezierCurve)*100.0/(real_t)GeometryVector.size(),(scene->size()-NbBezierCurve),Nmax,Nmin);
    if(NbBezierCurve!=0)
      PR("# Bezier : %.2f % (%d : Max %d, Min %d).\n",(real_t)NbBezierCurve*100.0/(real_t)GeometryVector.size(),NbBezierCurve,Bmax,Bmin);
    if(NbPolyline!=0)
      PR("# Polyline : %.2f % (%d : Max %d, Min %d).\n",(real_t)NbPolyline*100.0/(real_t)GeometryVector.size(),NbPolyline,Pmax,Pmin);
    if(NbPoint!=0)
      PR("# Point : %.2f % (%d).\n",(real_t)NbPoint*100.0/(real_t)indexGeometry,NbPoint);
    if((GeometryVector.size()-scene->size()-NbPolyline-NbPoint)!=0)
      PR("# None : %.2f % (%d : Max %d, Min 0).\n",(real_t)(GeometryVector.size()-scene->size()-NbPolyline-NbPoint)*100.0/(real_t)GeometryVector.size(),
	 (GeometryVector.size()-scene->size()-NbPolyline-NbPoint),Emax);
    PR("##############################################\n");
	interConnection(scene);

	return scene;
}

void
BranchCompressor::interConnection(ScenePtr& scene){
  pgl_hash_map<int,int> graph;
  for(std::vector<BranchInput>::const_iterator _iter = __inputs.begin(); 
  _iter != __inputs.end(); _iter++)
	graph[_iter->id] = _iter->father;
  int id = 0;
  int father = 0;
  Vector3 origin;
  for(Scene::iterator _it = scene->begin(); _it != scene->end(); _it++){
	id = (*_it)->getId();
	father = graph[id];
    while(father > 0 && !(scene->getShapeId(father)))
		  father = graph[father];
	if(father > 0){
	  ShapePtr sh = dynamic_pointer_cast<Shape>(scene->getShapeId(id));
	  if(sh){
		ExtrusionPtr ex = dynamic_pointer_cast<Extrusion>(sh->getGeometry());
		if(ex){
		  if(ex->getAxis()->isExplicit()){
			PolylinePtr pol = dynamic_pointer_cast<Polyline>(ex->getAxis());
			if(pol)pol->getPointList()->setAt(0,
			  connectionTo(pol->getPointList()->getAt(0),scene,father));
		  }
		  else {
			BezierCurvePtr bez = dynamic_pointer_cast<BezierCurve>(ex->getAxis());
			if(bez)bez->getCtrlPointList()->setAt(0,
			  connectionTo(bez->getCtrlPointList()->getAt(0),scene,father));
		  }
		}
		else {
		  Vector3 o = Vector3::ORIGIN;
		  Vector3 u = Vector3::OX;
		  Vector3 v = Vector3::OY;
		  Vector3 h = Vector3::OZ;
		  GeometryPtr geom = sh->getGeometry();
		  TransformedPtr tr = dynamic_pointer_cast<Transformed>(geom);
		  if(tr){
			TranslatedPtr trans;
			OrientedPtr oriented;
			do{
			  if((trans = dynamic_pointer_cast<Translated>(geom)))o = trans->getTranslation();
			  else if ((oriented =dynamic_pointer_cast<Oriented>(geom))) {
				u = oriented->getPrimary(); 
				v = oriented->getSecondary();
				h = cross(u,v);
			  }
			  geom = tr->getGeometry();
			} while ((tr= dynamic_pointer_cast<Transformed>(geom)));
		  }
		  ConePtr cone = dynamic_pointer_cast<Cone>(geom);
		  if(cone){
			real_t height = cone->getHeight();
			Vector3 pt = o+h*height;
			Vector3 newo = connectionTo(o,scene,father);
			if(norm(newo - o) > GEOM_EPSILON){
			  Vector3 h = pt-newo;
			  real_t length = h.normalize();
			  cone->getHeight() = length;
			  real_t a = fabs(dot(h,Vector3::OZ));
			  real_t b = fabs(dot(h,Vector3::OX));
			  real_t c = fabs(dot(h,Vector3::OY));
			  if(a < b && a < c)u = cross(h,Vector3::OZ);
			  else if(b < c)    u = cross(h,Vector3::OX);
			  else              u = cross(h,Vector3::OY);
			  v = cross(h,u);
			  u.normalize(); v.normalize();
			  if(u != Vector3::OX && v != Vector3::OY){
				geom = GeometryPtr(new Oriented(u,v,geom));
			  }
			  if(newo != Vector3::ORIGIN)
				geom = GeometryPtr(new Translated(newo,geom));
			  sh->geometry = geom;
			}
		  }	
		}
	  }
	}
  }
}

Vector4
BranchCompressor::connectionTo(const Vector4& p, 
							   const ScenePtr& scene,
							   int fid){
  return Vector4(connectionTo(p.project(),scene,fid),1);
}

Vector3
BranchCompressor::connectionTo(const Vector3& p, 
							   const ScenePtr& scene,
							   int fid){
  if(fid <= 0 || !scene){
	return p;
  }
  ShapePtr sh = dynamic_pointer_cast<Shape>(scene->getShapeId(fid));
  if(!sh) {
	return p;
  }
  ExtrusionPtr ex = dynamic_pointer_cast<Extrusion>(sh->getGeometry());
  if(ex){
	return ex->getAxis()->findClosest(p);
  }
  else {
	Vector3 o = Vector3::ORIGIN;
	Vector3 h = Vector3::OZ;
	GeometryPtr geom = sh->getGeometry();
	TransformedPtr tr = dynamic_pointer_cast<Transformed>(geom);
	if(tr){
	  TranslatedPtr trans;
	  OrientedPtr oriented;
	  do{
		if((trans= dynamic_pointer_cast<Translated>(geom)))o = trans->getTranslation();
		else if((oriented= dynamic_pointer_cast<Oriented>(geom))) h = cross(oriented->getPrimary(),oriented->getSecondary());
		geom = tr->getGeometry();
	  }while((tr = dynamic_pointer_cast<Transformed>(geom)));
	}
	ConePtr cone = dynamic_pointer_cast<Cone>(geom);
	if(cone){
	  real_t height = cone->getHeight();
	  Vector3 pt = p;
	  closestPointToSegment(pt,o,o+h*height);
	  return pt;
	}	
	return p;
  }
}

/* ----------------------------------------------------------------------- */
void 
BranchCompressor::removeDouble(Point2ArrayPtr& rad,RealArrayPtr& knot) const
{
  if(!rad||rad->size()<3)return;
  if(knot&&rad->size() != knot->size()){
	cerr << "Input Knot vector not valid" << endl;
	return;
  }
  bool k_def = true;
  if(!knot){
	uint_t _size = rad->size();
	knot = RealArrayPtr(new RealArray(_size));
    knot->setAt(0,0.0);
    for(uint_t _i = 1; _i < _size; _i++)
        knot->setAt(_i, ((real_t)_i /(real_t)(_size - 1)));
    knot->setAt(_size-1,1);
	k_def = false;
  }

  int nbdouble = 0;
  RealArrayPtr knot2 = RealArrayPtr(new RealArray(1,knot->getAt(0)));
  Point2ArrayPtr rad2 = Point2ArrayPtr(new Point2Array(1,rad->getAt(0)));

  RealArray::iterator _it2 = knot->begin()+1;
  for(Point2Array::const_iterator _it = rad->begin() +1;
	  _it != rad->end()-1; _it++){
	if(norm(*_it-((*(_it-1)+*(_it+1))/2))>GEOM_EPSILON){
	  knot2->push_back(*_it2);
	  rad2->push_back(*_it);
	}
	else nbdouble++;
	_it2++;
  }
  knot2->push_back(*(knot->end()-1));
  rad2->push_back(*(rad->end()-1));
  if(knot2->size()!=rad2->size())cerr << "Error with knot vector size !!" << endl;

  if(isDefault(knot)){
	knot = RealArrayPtr();
	rad = rad2;
  }
  else {
	if(k_def){ rad = rad2; knot = knot2; }
	else {
	  if(knot2->size() <= 2*nbdouble){ rad = rad2; knot = knot2; }
	  else { knot = RealArrayPtr(); /*rad = rad;*/ }
	}
  }
  if(knot&&knot->size()!=rad->size())cerr << "Error with final knot vector size !!" << endl;
}

bool 
BranchCompressor::isDefault(RealArrayPtr& knot) const
{
  uint_t _size = knot->size();
  if(fabs(knot->getAt(0)) > GEOM_EPSILON)return false;
  for(uint_t _i = 1; _i < _size; _i++)
    if(fabs(knot->getAt(_i) - ((real_t)_i /(real_t)(_size - 1))) > GEOM_EPSILON)
	  return false;
  return true;
}

/* ----------------------------------------------------------------------- */
