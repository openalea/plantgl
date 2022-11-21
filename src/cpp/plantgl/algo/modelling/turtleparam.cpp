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


#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/geometry/cylinder.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/indexarray.h>
#include <plantgl/scenegraph/container/geometryarray2.h>
#include <plantgl/math/util_math.h>
#include <plantgl/tool/util_types.h>
#include <plantgl/tool/util_string.h>

#include <sstream>

#include "turtleparam.h"

using namespace std;
PGL_USING_NAMESPACE

#define FABS(a) (a < 0 ? -(a) : a)

/*----------------------------------------------------------*/
TurtleDrawParameter::TurtleDrawParameter():
  color(1),
  customMaterial(),
  crossSection(),
  crossSectionCCW(true),
  defaultSection(true),
  texCoordScale(1,1),
  texCoordTranslation(0,0),
  texCoordRotCenter(0.5,0.5),
  texCoordRotAngle(0),
  texBaseColor(255,255,255,0),
  axialLength(0)
{
}

void TurtleDrawParameter::reset(){
  color = 1;
  customMaterial = AppearancePtr();
  texCoordScale = Vector2(1,1);
  texCoordTranslation = Vector2(0,0);
  texCoordRotCenter = Vector2(0.5,0.5);
  texCoordRotAngle = 0;
  texBaseColor = Color4(255,255,255,0);
  crossSection = Curve2DPtr();
  crossSectionCCW = true;
  defaultSection = true;
  axialLength = 0;
}


TurtleParam::TurtleParam() :
  TurtleDrawParameter(),
  position(0,0,0),
  heading(0,0,1),
  left(0,-1,0),
  up(1,0,0),
  scale(1,1,1),
  reflection(1,1,1),
  lastId(Shape::NOID),
  width(0.1f),
  tropism(0,0,1),
  elasticity(0),
  screenCoordinates(false),
  __polygon(false),
  __generalizedCylinder(false),
  pointList(new Point3Array()),
  customId(Shape::NOID),
  customParentId(Shape::NOID),
  sectionResolution(Cylinder::DEFAULT_SLICES),
  initial(),
  frame_info{position, heading, left, up, scale, screenCoordinates}
{
}

TurtleParam::TurtleParam(const TurtleParam& origin) :
        TurtleDrawParameter(origin),
        position(origin.position),
        heading(origin.heading),
        left(origin.left),
        up(origin.up),
        scale(origin.scale),
        reflection(origin.reflection),
        lastId(origin.lastId),
        width(origin.width),
        tropism(origin.tropism),
        elasticity(origin.elasticity),
        screenCoordinates(origin.screenCoordinates),
        __polygon(origin.__polygon),
        __generalizedCylinder(origin.__generalizedCylinder),
        pointList(new Point3Array(*origin.pointList.get())),
        customId(origin.customId),
        customParentId(origin.customParentId),
        sectionResolution(origin.sectionResolution),
        initial(origin.initial),
        frame_info{position, heading, left, up, scale, screenCoordinates}
{
}

TurtleParam::~TurtleParam() {}

void
TurtleParam::reset(){
  TurtleDrawParameter::reset();
  position = Vector3(0,0,0);
  heading  = Vector3(0,0,1);
  left     = Vector3(0,-1,0);
  up       = Vector3(1,0,0);
  scale    = Vector3(1,1,1);
  reflection = Vector3(1,1,1);
  lastId = Shape::NOID;
  customId = Shape::NOID;
  customParentId = Shape::NOID;
  sectionResolution = Cylinder::DEFAULT_SLICES;
  width = 0.1f;
  tropism = Vector3(0,0,1);
  elasticity = 0;
  screenCoordinates = false;
  __polygon = false;
  __generalizedCylinder = false;
  pointList->clear();
  leftList.clear();
  radiusList.clear();
  initial.reset();
  guide = TurtlePathPtr();
}

TurtleParam * TurtleParam::copy(){
  TurtleParam * t = new TurtleParam(*this);
  if(t->guide)t->guide = t->guide->copy();
  if(!__polygon)t->pointList = new Point3Array(*t->pointList);
  return t;
}

void TurtleParam::dump(){
   std::cerr << "Position : " << position << std::endl;
   std::cerr << "Heading  : " << heading  << std::endl;
   std::cerr << "Left     : " << left << std::endl;
   std::cerr << "Up       : " << up << std::endl;
   std::cerr << "Color    : " << color << std::endl;
   std::cerr << "Width    : " << width << std::endl;
}

void TurtleParam::transform(const Matrix3& m){
   heading = m * heading;
   left = m * left;
   up = m * up;
}

Matrix3 TurtleParam::getOrientationMatrix() const{
   return Matrix3(heading,left,up);
}

Matrix4 TurtleParam::getTransformationMatrix() const{
   return Matrix4(heading*scale.x(),left*scale.y(),up*scale.z(),position);
}

bool TurtleParam::isValid() const{
   return heading.isNormalized() &&
          left.isNormalized() &&
          up.isNormalized() &&
          heading.isOrthogonalTo(left) &&
          heading.isOrthogonalTo(up) &&
          left.isOrthogonalTo(up);
}

void TurtleParam::keepLastPoint(){
  if(!pointList->empty()){
    Vector3 lastp = *(pointList->end()-1);
    pointList->clear();
    pointList->push_back(lastp);
    // pointList = vector<Vector3>(1,*(pointList.end()-1));
    // pointList = vector<Vector3>(1,*(pointList.end()-1));
  }
  if(!leftList.empty())
    leftList = vector<Vector3>(1,*(leftList.end()-1));
  if(!radiusList.empty())
    radiusList = vector<real_t>(1,*(radiusList.end()-1));
}

void TurtleParam::removePoints(){
  pointList->clear();
  leftList.clear();
  radiusList.clear();
}

void TurtleParam::polygon(bool t){
  __polygon = t;
  removePoints();
  if (t){
    initial.color = color;
    initial.customMaterial = customMaterial;
  }
  else {
    initial.reset();
  }
}

void TurtleParam::generalizedCylinder(bool t){
  __generalizedCylinder = t;
  removePoints();
  if (t){
    initial = TurtleDrawParameter(*this);
  }
  else {
    initial.reset();
  }

}

void TurtleParam::pushPosition(){
  pointList->push_back(position);
  if(__generalizedCylinder) {
    leftList.push_back(left);
    radiusList.push_back(width);
  }
}

void TurtleParam::pushRadius(){
    *(radiusList.end()-1) = width;
}

void TurtleParam::setCrossSection(const Curve2DPtr &curve, bool ccw, bool defaultSection) {
    crossSection = curve;
    crossSectionCCW = ccw;
    this->defaultSection = defaultSection;
    if(isGeneralizedCylinderOnInit())
    {
        initial.crossSection = curve;
        initial.crossSectionCCW = ccw;
        initial.defaultSection = defaultSection;
    }
}
