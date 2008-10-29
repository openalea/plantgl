/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
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






#include "amaptranslator.h"
#include "discretizer.h"
#include "bboxcomputer.h"

#include <plantgl/math/util_math.h>

#include <plantgl/pgl_appearance.h>
#include <plantgl/pgl_geometry.h>
#include <plantgl/pgl_transformation.h>
#include <plantgl/scenegraph/scene/shape.h>
#include <plantgl/scenegraph/geometry/boundingbox.h>
#include <plantgl/scenegraph/container/indexarray.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */
#define GEOM_TRANSLATE(geom) \
   GEOM_ASSERT(geom); \
    if(geom->apply(__discretizer)){ \
        ExplicitModelPtr _discret = __discretizer.getDiscretization(); \
        if (_discret) return _discret->apply(*this); \
        else { \
            __symbol = AmapSymbolPtr(); \
            return false; \
        }; \
    } \
    else { \
        __symbol = AmapSymbolPtr(); \
        return false; \
    }; \

/* ----------------------------------------------------------------------- */

AmapTranslator::AmapTranslator(Discretizer& _discretizer) :
    Action(),
    __discretizer(_discretizer),
    __normalized(true){
	__discretizer.computeTexCoord(true);
}


AmapTranslator::~AmapTranslator() {
}

AmapSymbolPtr AmapTranslator::getSymbol() {
  return __symbol;
}

void
AmapTranslator::setNormalized(bool b){
  __normalized =b;
}

const bool
AmapTranslator::isNormalized(){
  return __normalized;
}

/* ----------------------------------------------------------------------- */


bool AmapTranslator::process(Shape * Shape) {
  GEOM_ASSERT(Shape);
  return Shape->geometry->apply(*this);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Material * material ) {
  GEOM_ASSERT(material);
  // nothing to do
  __symbol = AmapSymbolPtr();
  return false;
}


/* ----------------------------------------------------------------------- */

bool AmapTranslator::process( ImageTexture * texture ) {
  GEOM_ASSERT(texture);
  // nothing to do
  __symbol = AmapSymbolPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( MonoSpectral * monoSpectral ) {
  GEOM_ASSERT(monoSpectral);
  // nothing to do
  __symbol = AmapSymbolPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( MultiSpectral * multiSpectral ) {
  GEOM_ASSERT(multiSpectral);
  // nothing to do
  __symbol = AmapSymbolPtr();
  return false;
}



/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( AmapSymbol * amapSymbol ) {
    GEOM_ASSERT(amapSymbol);
    // nothing to do
    __symbol = AmapSymbolPtr(amapSymbol);
    return true;
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( AsymmetricHull * asymmetricHull ) {
    GEOM_TRANSLATE(asymmetricHull);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( AxisRotated * axisRotated ) {
  GEOM_TRANSLATE(axisRotated);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( BezierCurve * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);
  // nothing to do
  __symbol = AmapSymbolPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( BezierPatch * bezierPatch ) {
  GEOM_TRANSLATE(bezierPatch);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Box * box ) {
  GEOM_TRANSLATE(box);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Cone * cone ) {
  GEOM_TRANSLATE(cone);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Cylinder * cylinder ) {
  GEOM_TRANSLATE(cylinder);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( ElevationGrid * elevationGrid ) {
  GEOM_TRANSLATE(elevationGrid);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( EulerRotated * eulerRotated ) {
  GEOM_TRANSLATE(eulerRotated);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( ExtrudedHull * extrudedHull ) {
  GEOM_TRANSLATE(extrudedHull);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( FaceSet * faceSet ) {
    GEOM_ASSERT(faceSet);
    if(__normalized){
        BBoxComputer _bbc(__discretizer);
        faceSet->apply(_bbc);
        BoundingBoxPtr _bb = _bbc.getBoundingBox();
        Vector3 _bbox_size = _bb->getSize();
        Vector3 _translation = _bb->getCenter();
        _translation.z()=_bb->getLowerLeftCorner().z();
        _translation *= -1;
        Vector3 _scale(1,1,1);
        if(fabs(2*_bbox_size.x())>1)_scale.x()=1/(fabs(_bbox_size.x()*2));
        if(fabs(2*_bbox_size.y())>1)_scale.y()=1/(fabs(_bbox_size.y()*2));
        if(fabs(2*_bbox_size.z())>1)_scale.z()=1/(fabs(_bbox_size.z()*2));
        real_t a = *(_scale.getMin());
        _scale = Vector3(a,a,a);
        ExplicitModelPtr _m;
        if(fabs(_translation.x()) > GEOM_EPSILON ||
           fabs(_translation.y()) > GEOM_EPSILON ||
           fabs(_translation.z()) > GEOM_EPSILON){
            _m = faceSet->transform(Transformation3DPtr(new Translation(_translation)));
                        __translation = -_translation;
                }
        else {
                        __translation = Vector3::ORIGIN;
                        _m = ExplicitModelPtr(faceSet);
                }
        if(fabs(_scale.x()-1) > GEOM_EPSILON){
                        _m = _m->transform(Transformation3DPtr(new Scaling(_scale)));
                        __normfactor = Vector3(1/a,1/a,1/a);
                }
                else __normfactor = Vector3(1,1,1);
        AmapSymbol * _smb = new AmapSymbol(dynamic_pointer_cast<FaceSet>(_m));
        __symbol = AmapSymbolPtr(_smb);
    }
    else {
        __symbol = AmapSymbolPtr(new AmapSymbol(FaceSetPtr(faceSet)));
    }
    return true;
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Frustum * frustum ) {
  GEOM_TRANSLATE(frustum);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Extrusion * extrusion   ) {
  GEOM_TRANSLATE(extrusion);
}

/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Group * group ) {
  GEOM_TRANSLATE(group);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( IFS * ifs ) {
  GEOM_TRANSLATE(ifs);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( NurbsCurve * nurbsCurve ) {
    GEOM_ASSERT(nurbsCurve);
    // nothing to do
    __symbol = AmapSymbolPtr();
    return false;
}

/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( NurbsPatch * nurbsPatch ) {
  GEOM_TRANSLATE(nurbsPatch);
}

/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Oriented * oriented ) {
  GEOM_TRANSLATE(oriented);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Paraboloid * paraboloid ) {
  GEOM_TRANSLATE(paraboloid);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( PointSet * pointSet ) {
  GEOM_ASSERT(pointSet);
  // nothing to do
  __symbol = AmapSymbolPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Polyline * polyline ) {
  GEOM_ASSERT(polyline);
  // nothing to do
  __symbol = AmapSymbolPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Revolution * revolution ) {
  GEOM_TRANSLATE(revolution);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Swung * swung )
{
  GEOM_TRANSLATE(swung);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( QuadSet * quadSet ) {
  GEOM_ASSERT(quadSet);
  Index4ArrayPtr _ind = quadSet->getIndexList();
  IndexArrayPtr indices(new IndexArray(_ind->getSize()));
  IndexArray::iterator _it2 = indices->getBegin();
  for(Index4Array::const_iterator _it = _ind->getBegin();_it!=_ind->getEnd();_it++){
      (*_it2) = Index(0);
      _it2->insert(_it2->getBegin(),_it->getBegin(),_it->getEnd());
      _it2++;
  }
  FaceSetPtr f(new FaceSet(quadSet->getPointList(), indices, quadSet->getNormalPerVertex(),
             quadSet->getCCW(), quadSet->getSolid(), quadSet->getSkeleton()));
  return f->apply(*this);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Scaled * scaled ) {
  GEOM_TRANSLATE(scaled);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Sphere * sphere ) {
  GEOM_TRANSLATE(sphere);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Tapered * tapered ) {
  GEOM_TRANSLATE(tapered);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Translated * translated ) {
  GEOM_TRANSLATE(translated);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( TriangleSet * triangleSet ) {
  GEOM_ASSERT(triangleSet);
  Index3ArrayPtr _ind = triangleSet->getIndexList();
  IndexArrayPtr indices(new IndexArray(_ind->getSize()));
  IndexArray::iterator _it2 = indices->getBegin();
  for(Index3Array::const_iterator _it = _ind->getBegin();_it!=_ind->getEnd();_it++){
      (*_it2) = Index(0);
      _it2->insert(_it2->getBegin(),_it->getBegin(),_it->getEnd());_it2++;
  }
  FaceSetPtr f( new FaceSet( triangleSet->getPointList(), indices, triangleSet->getNormalPerVertex(),
             triangleSet->getCCW(), triangleSet->getSolid(), triangleSet->getSkeleton()));
  return f->apply(*this);
}


/* ------------------------------------------------------------------------- */


bool AmapTranslator::process( BezierCurve2D * bezierCurve ) {
  GEOM_ASSERT(bezierCurve);
  // nothing to do
  __symbol = AmapSymbolPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Disc * disc ) {
  GEOM_TRANSLATE(disc);
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( NurbsCurve2D * nurbsCurve ) {
  GEOM_ASSERT(nurbsCurve);
  // nothing to do
  __symbol = AmapSymbolPtr();
  return false;
}

/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( PointSet2D * pointSet ) {
  GEOM_ASSERT(pointSet);
  // nothing to do
  __symbol = AmapSymbolPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Polyline2D * polyline ) {
  GEOM_ASSERT(polyline);
  // nothing to do
  __symbol = AmapSymbolPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Text * text ) {
  GEOM_ASSERT(text);
  // nothing to do
  __symbol = AmapSymbolPtr();
  return false;
}


/* ----------------------------------------------------------------------- */


bool AmapTranslator::process( Font * font ) {
  GEOM_ASSERT(font);
  // nothing to do
  __symbol = AmapSymbolPtr();
  return false;
}


/* ----------------------------------------------------------------------- */
