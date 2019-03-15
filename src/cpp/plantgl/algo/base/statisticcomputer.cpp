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





#include "statisticcomputer.h"

#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/pgl_container.h>
#include <plantgl/scenegraph/scene/shape.h>

PGL_USING_NAMESPACE

using namespace std;

/* ----------------------------------------------------------------------- */


#define GEOM_BEGIN(obj) \
  if (obj->isNamed()) { \
    if (! __cache.insert(obj->SceneObject::getId()).second) { \
      return true; \
    }; \
    __named++; \
  } \

#define GEOM_COMPUTE(obj,num) \
    GEOM_ASSERT(obj); \
    GEOM_BEGIN(obj); \
    __element++; \
    __shape[num]++; \
    __memsize += sizeof(*obj); \

#define GEOM_APPLY(obj,field) \
    if(obj->get##field())obj->get##field()->apply(*this);


/* ----------------------------------------------------------------------- */


StatisticComputer::StatisticComputer( ) :
  Action(),
  __cache(),
  __element(0),
  __named(0),
  __shape((unsigned int)45,0),
  __memsize(0){
}

StatisticComputer::~StatisticComputer( ) {
}

const uint_t
StatisticComputer::getSize() const{
  return __element;
}

const uint_t
StatisticComputer::getNamed() const{
  return __named;
}

const uint_t
StatisticComputer::getMemorySize() const {
  return __memsize;
}

const vector<uint_t>&
StatisticComputer::getElements() const{
  return __shape;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process(Shape * shape){
    GEOM_COMPUTE(shape,0);
    GEOM_APPLY(shape,Geometry);
    GEOM_APPLY(shape,Appearance);
    return true;
}

/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Material * material ) {
    GEOM_COMPUTE(material,1);
    return true;


}

/* ----------------------------------------------------------------------- */

bool StatisticComputer::process( ImageTexture * texture ) {
    GEOM_COMPUTE(texture,39);
    return true;
}

/* ----------------------------------------------------------------------- */

bool StatisticComputer::process( Texture2D * texture ) {
    GEOM_COMPUTE(texture,42);
    GEOM_APPLY(texture,Image);
    GEOM_APPLY(texture,Transformation);
    return true;
}

/* ----------------------------------------------------------------------- */

bool StatisticComputer::process( Texture2DTransformation * texture ) {
    GEOM_COMPUTE(texture,43);
    return true;
}

/* ----------------------------------------------------------------------- */

bool StatisticComputer::process( MonoSpectral * monoSpectral ) {
    GEOM_COMPUTE(monoSpectral,2);
    return true;

}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( MultiSpectral * multiSpectral ) {
    GEOM_COMPUTE(multiSpectral,3);

    __memsize += sizeof(*(multiSpectral->getReflectance()));
    __memsize += sizeof(*(multiSpectral->getTransmittance()));

    return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( AmapSymbol * amapSymbol ) {
    GEOM_COMPUTE(amapSymbol,4);

    __memsize += sizeof(*(amapSymbol->getPointList()));
    __memsize += sizeof(*(amapSymbol->getIndexList()));
    __memsize += sizeof(*(amapSymbol->getNormalList()));

    GEOM_APPLY(amapSymbol,Skeleton);

    return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( AsymmetricHull * asymmetricHull ) {
    GEOM_COMPUTE(asymmetricHull,5);

    return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( AxisRotated * axisRotated ) {
    GEOM_COMPUTE(axisRotated,6);

    GEOM_APPLY(axisRotated,Geometry);

    return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( BezierCurve * bezierCurve ) {
    GEOM_COMPUTE(bezierCurve,7);

    __memsize += sizeof(*(bezierCurve->getCtrlPointList()));

    return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( BezierPatch * bezierPatch ) {
    GEOM_COMPUTE(bezierPatch,8);

    __memsize += sizeof(*(bezierPatch->getCtrlPointMatrix()));

    return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Box * box ) {
  GEOM_COMPUTE(box,9);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Cone * cone ) {
  GEOM_COMPUTE(cone,10);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Cylinder * cylinder ) {
  GEOM_COMPUTE(cylinder,11);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( ElevationGrid * elevationGrid ) {
  GEOM_COMPUTE(elevationGrid,12);

  __memsize += sizeof(*(elevationGrid->getHeightList()));

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( EulerRotated * eulerRotated ) {
  GEOM_COMPUTE(eulerRotated,13);

  GEOM_APPLY(eulerRotated,Geometry);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( ExtrudedHull * extrudedHull ) {
  GEOM_COMPUTE(extrudedHull,14);

  GEOM_APPLY(extrudedHull,Vertical);
  GEOM_APPLY(extrudedHull,Horizontal);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( FaceSet * faceSet ) {
  GEOM_COMPUTE(faceSet,15);

  __memsize += sizeof(*(faceSet->getPointList()));
  __memsize += sizeof(*(faceSet->getIndexList()));

  GEOM_APPLY(faceSet,Skeleton);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Frustum * frustum ) {
  GEOM_COMPUTE(frustum,16);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Extrusion * extrusion ) {
  GEOM_COMPUTE(extrusion,17);

  GEOM_APPLY(extrusion,Axis);
  GEOM_APPLY(extrusion,CrossSection);
  if(extrusion->getProfileTransformation()){
      __memsize += sizeof(*(extrusion->getProfileTransformation()));
      if(extrusion->getProfileTransformation()->getScale())
           __memsize += sizeof(*(extrusion->getProfileTransformation()->getScale()));
      if(extrusion->getProfileTransformation()->getScale())
           __memsize += sizeof(*(extrusion->getProfileTransformation()->getScale()));
      if(extrusion->getProfileTransformation()->getOrientation())
           __memsize += sizeof(*(extrusion->getProfileTransformation()->getOrientation()));
      if(!extrusion->getProfileTransformation()->isKnotListToDefault())
           __memsize += sizeof(*(extrusion->getProfileTransformation()->getKnotList()));
  }

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Group * group  ) {
  GEOM_COMPUTE(group,18);

  for(uint_t _i = 0 ; _i < group->getGeometryListSize( ) ; _i++)
      group->getGeometryListAt(_i)->apply(*this);

  GEOM_APPLY(group,Skeleton);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( NurbsCurve * nurbsCurve ) {
  GEOM_COMPUTE(nurbsCurve,19);

  __memsize += sizeof(*(nurbsCurve->getCtrlPointList()));

  return true;
}

/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( NurbsPatch * nurbsPatch ) {
  GEOM_COMPUTE(nurbsPatch,20);

  __memsize += sizeof(*(nurbsPatch->getCtrlPointMatrix()));

  return true;
}

/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Oriented * oriented ) {
  GEOM_COMPUTE(oriented,21);

  GEOM_APPLY(oriented,Geometry);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Paraboloid * paraboloid ) {
  GEOM_COMPUTE(paraboloid,22);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( PointSet * pointSet ) {
  GEOM_COMPUTE(pointSet,23);

  __memsize += sizeof(*(pointSet->getPointList()));

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Polyline * polyline ) {
  GEOM_COMPUTE(polyline,24);

  __memsize += sizeof(*(polyline->getPointList()));

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Revolution * revolution ) {
  GEOM_COMPUTE(revolution,25);

  GEOM_APPLY(revolution,Profile);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( QuadSet * quadSet ) {
  GEOM_COMPUTE(quadSet,26);

  __memsize += sizeof(*(quadSet->getPointList()));
  __memsize += sizeof(*(quadSet->getIndexList()));

  GEOM_APPLY(quadSet,Skeleton);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Scaled * scaled ) {
  GEOM_COMPUTE(scaled,27);

  GEOM_APPLY(scaled,Geometry);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( ScreenProjected * scp ) {
  GEOM_COMPUTE(scp,44);

  GEOM_APPLY(scp,Geometry);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Sphere * sphere ) {
  GEOM_COMPUTE(sphere,28);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Tapered * tapered ) {
  GEOM_COMPUTE(tapered,29);

  GEOM_APPLY(tapered,Primitive);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Translated * translated ) {
  GEOM_COMPUTE(translated,30);

  GEOM_APPLY(translated,Geometry);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( TriangleSet * triangleSet ) {
    GEOM_COMPUTE(triangleSet,31);

    __memsize += sizeof(*(triangleSet->getPointList()));
    __memsize += sizeof(*(triangleSet->getIndexList()));

    GEOM_APPLY(triangleSet,Skeleton);

    return true;
}


/* ------------------------------------------------------------------------- */


bool StatisticComputer::process( BezierCurve2D * bezierCurve ) {
  GEOM_COMPUTE(bezierCurve,32);

  __memsize += sizeof(*(bezierCurve->getCtrlPointList()));

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Disc * disc ) {
  GEOM_COMPUTE(disc,33);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( NurbsCurve2D * nurbsCurve ) {
  GEOM_COMPUTE(nurbsCurve,34);

  __memsize += sizeof(*(nurbsCurve->getCtrlPointList()));

  return true;
}

/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( PointSet2D * pointSet ) {
  GEOM_COMPUTE(pointSet,35);

  __memsize += sizeof(*(pointSet->getPointList()));

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Polyline2D * polyline ) {
  GEOM_COMPUTE(polyline,36);

  __memsize += sizeof(*(polyline->getPointList()));

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( Swung * swung )
{
  GEOM_COMPUTE(swung,37);

  __memsize += sizeof(*(swung->getAngleList()));

  GEOM_APPLY(swung,ProfileList);

  return true;
}


/* ----------------------------------------------------------------------- */


bool StatisticComputer::process( IFS * ifs ) {
  GEOM_COMPUTE(ifs,38);
GEOM_TRACE("process IFS");

//todo OK

  const Transform4ArrayPtr& tList= ifs->getTransfoList();
  Transform4Ptr t4= *(tList->begin());

  for( uchar_t i= 0; i < tList->size(); i++ )
    __memsize += sizeof(*t4) * sizeof(*(ifs->getTransfoList()));

  GEOM_APPLY(ifs,Geometry);

  return true;
}


/* ----------------------------------------------------------------------- */

bool StatisticComputer::process( Text * text ) {
  GEOM_COMPUTE(text,40);
  GEOM_APPLY(text,FontStyle);
  return true;
}

/* ----------------------------------------------------------------------- */

bool StatisticComputer::process( Font * font ) {
  GEOM_COMPUTE(font,41);
  return true;
}

/* ----------------------------------------------------------------------- */


const uint_t StatisticComputer::getShape() const {
  return __shape[0];
}


const uint_t StatisticComputer::getMaterial() const {
  return __shape[1];
}

const uint_t StatisticComputer::getImageTexture() const {
  return __shape[39];
}


const uint_t StatisticComputer::getMonoSpectral() const {
  return __shape[2];
}

const uint_t StatisticComputer::getMultiSpectral() const {
  return __shape[3];
}

const uint_t StatisticComputer::getAmapSymbol() const {
  return __shape[4];
}

const uint_t StatisticComputer::getAsymmetricHull() const {
  return __shape[5];
}

const uint_t StatisticComputer::getAxisRotated() const {
  return __shape[6];
};

const uint_t StatisticComputer::getBezierCurve() const {
  return __shape[7];
}

const uint_t StatisticComputer::getBezierPatch() const {
  return __shape[8];
}

const uint_t StatisticComputer::getBox() const {
  return __shape[9];
}

const uint_t StatisticComputer::getCone() const {
  return __shape[10];
}


const uint_t StatisticComputer::getCylinder() const {
  return __shape[11];
}

const uint_t StatisticComputer::getElevationGrid() const {
  return __shape[12];
}

const uint_t StatisticComputer::getEulerRotated() const {
  return __shape[13];
}

const uint_t StatisticComputer::getExtrudedHull() const {
  return __shape[14];
}

const uint_t StatisticComputer::getFaceSet() const {
  return __shape[15];
}

const uint_t StatisticComputer::getFrustum() const {
  return __shape[16];
}

const uint_t StatisticComputer::getExtrusion() const {
  return __shape[17];
}

const uint_t StatisticComputer::getGroup() const {
  return __shape[18];
}

const uint_t StatisticComputer::getNurbsCurve() const {
  return __shape[19];
}
const uint_t StatisticComputer::getNurbsPatch() const {
  return __shape[20];
}

const uint_t StatisticComputer::getOriented() const {
  return __shape[21];
}

const uint_t StatisticComputer::getParaboloid() const {
  return __shape[22];
}

const uint_t StatisticComputer::getPointSet() const {
  return __shape[23];
}

const uint_t StatisticComputer::getPolyline() const {
  return __shape[24];
}

const uint_t StatisticComputer::getQuadSet() const {
  return __shape[25];
}

const uint_t StatisticComputer::getRevolution() const {
  return __shape[26];
}

const uint_t StatisticComputer::getScaled() const {
  return __shape[27];
}

const uint_t StatisticComputer::getScreenProjected() const {
  return __shape[44];
}

const uint_t StatisticComputer::getSphere() const {
  return __shape[28];
}

const uint_t StatisticComputer::getTapered() const {
  return __shape[29];
}

const uint_t StatisticComputer::getTranslated() const {
  return __shape[30];
}

const uint_t StatisticComputer::getTriangleSet() const {
  return __shape[31];
}

const uint_t StatisticComputer::getBezierCurve2D() const {
  return __shape[32];
}

const uint_t StatisticComputer::getDisc() const {
  return __shape[33];
}

const uint_t StatisticComputer::getNurbsCurve2D() const {
  return __shape[34];
}

const uint_t StatisticComputer::getPointSet2D() const {
  return __shape[35];
}

const uint_t StatisticComputer::getPolyline2D() const {
  return __shape[36];
}

const uint_t StatisticComputer::getSwung() const {
  return __shape[37];
}

const uint_t StatisticComputer::getIFS() const {
  return __shape[38];
}

const uint_t StatisticComputer::getText() const {
  return __shape[40];
}

const uint_t StatisticComputer::getFont() const {
  return __shape[41];
}

const uint_t StatisticComputer::getTexture2D() const {
  return __shape[42];
}

const uint_t StatisticComputer::getTexture2DTransformation() const {
  return __shape[43];
}
