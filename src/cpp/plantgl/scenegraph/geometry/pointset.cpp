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




#include "pointset.h"
#include <plantgl/scenegraph/core/pgl_messages.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/transformation/transformed.h>

PGL_USING_NAMESPACE

/* ----------------------------------------------------------------------- */

const uchar_t PointSet::DEFAULT_WIDTH = 1;

/* ----------------------------------------------------------------------- */


PointSet::Builder::Builder( ) :
  ExplicitModel::Builder(),
  Width(0){
}


PointSet::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr PointSet::Builder::build( ) const {
  if (isValid())
      return SceneObjectPtr(new PointSet(*PointList,ColorList?*ColorList:Color4ArrayPtr(),Width?*Width:DEFAULT_WIDTH));
  // Returns null as self is not valid.
  return SceneObjectPtr();
}


void PointSet::Builder::destroy( ) {
  EMDestroy();
  if (Width) delete Width;
}


bool PointSet::Builder::isValid( ) const {
    if(!EMValid()) return false;

    if (ColorList && *ColorList) {
        uint_t _colorListSize = (*ColorList)->size();
        if(_colorListSize != (*PointList)->size()){
            pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_VALUE_sss),"PointSet","ColorList","Number of colors must be compatible to PointList size.");
            return false;
        }
    }
    return true;

}


/* ----------------------------------------------------------------------- */


PointSet::PointSet( ) :
  ExplicitModel(),__width(DEFAULT_WIDTH){
}

PointSet::PointSet( const Point3ArrayPtr& points, const Color4ArrayPtr& colors, uchar_t width ) :
  ExplicitModel(points,colors),__width(width){
  GEOM_ASSERT(isValid());
}

PointSet::~PointSet( ) {
}

bool PointSet::isValid( ) const {
  Builder _builder;
  if(__pointList)_builder.PointList = const_cast<Point3ArrayPtr *>(&__pointList);
  if(__colorList)_builder.ColorList = const_cast<Color4ArrayPtr *>(&__colorList);
  return _builder.isValid();
}

bool  PointSet::isACurve( ) const {
  return false;
}

bool  PointSet::isASurface( ) const {
  return false;
}

bool  PointSet::isAVolume( ) const {
  return false;
}


const Vector3&
PointSet::getPointListAt( uint_t i ) const {
  GEOM_ASSERT(i < __pointList->size());
  return __pointList->getAt(i);
}

Vector3&
PointSet::getPointListAt( uint_t i ) {
  GEOM_ASSERT(i < __pointList->size());
  return __pointList->getAt(i);
}


ExplicitModelPtr
PointSet::transform( const Transformation3DPtr& transformation ) const {
  GEOM_ASSERT(transformation);
  return ExplicitModelPtr(new PointSet(transformation->transform(__pointList),__colorList));
}

SceneObjectPtr
PointSet::copy(DeepCopier& copier) const
{
  PointSet * ptr = new PointSet(*this);
  copier.copy_attribute(ptr->getPointList());
  copier.copy_attribute(ptr->getColorList());
  return SceneObjectPtr(ptr);
}


Vector3
PointSet::findClosest(const Vector3& point, uint_t * index) const
{

    std::pair<Point3Array::const_iterator,real_t> res = PGL::findClosest(*__pointList,point);
    if(index) *index = std::distance<Point3Array::const_iterator>(__pointList->begin(),res.first);
    return *res.first;
}

/* ----------------------------------------------------------------------- */



PointSet2D::Builder::Builder( ) :
  PlanarModel::Builder(),
  PointList(0),
  Width(0){
}


PointSet2D::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr PointSet2D::Builder::build( ) const {
  if (isValid())
      return SceneObjectPtr(new PointSet2D(*PointList,Width?*Width:PointSet::DEFAULT_WIDTH));
  // Returns null as self is not valid.
  return SceneObjectPtr();
}


void PointSet2D::Builder::destroy( ) {
  if (PointList) delete PointList;
  if (Width) delete Width;
}


bool PointSet2D::Builder::isValid( ) const {

  // PointList
  if (! PointList) {
    pglErrorEx(PGLWARNINGMSG(UNINITIALIZED_FIELD_ss),"PointSet2D","PointList");
    return false;
  };
  if ((*PointList)->size() < 1) {
    pglErrorEx(PGLWARNINGMSG(INVALID_FIELD_SIZE_sss),"PointSet2D","PointList","Number of points must be greater than 0.");
    return false;
  };

  return true;
}


/* ----------------------------------------------------------------------- */


PointSet2D::PointSet2D( const Point2ArrayPtr& points, uchar_t width ) :
  PlanarModel(),
  __pointList(points), __width(width) {
  GEOM_ASSERT(isValid());
}

PointSet2D::PointSet2D(  ) :
  PlanarModel(),
  __pointList(),
  __width(PointSet::DEFAULT_WIDTH) {
}

PointSet2D::~PointSet2D( ) {
}

bool PointSet2D::isValid( ) const {
  Builder _builder;
  _builder.PointList = const_cast<Point2ArrayPtr *>(&__pointList);
  return _builder.isValid();
}

bool PointSet2D::isACurve( ) const {
  return false;
}

bool PointSet2D::isASurface( ) const {
  return false;
}

bool PointSet2D::isAVolume( ) const {
  return false;
}

SceneObjectPtr
PointSet2D::copy(DeepCopier& copier) const
{
  PointSet2D * ptr = new PointSet2D(*this);
  copier.copy_attribute(ptr->getPointList());
  return SceneObjectPtr(ptr);
}

const Vector2&
PointSet2D::getPointListAt( uint_t i ) const {
  GEOM_ASSERT(i < __pointList->size());
  return __pointList->getAt(i);
}

Vector2&
PointSet2D::getPointListAt( uint_t i ) {
  GEOM_ASSERT(i < __pointList->size());
  return __pointList->getAt(i);
}

const Point2ArrayPtr&
PointSet2D::getPointList( ) const {
  return __pointList;
}

Point2ArrayPtr&
PointSet2D::getPointList( ) {
  return __pointList;
}

uint_t
PointSet2D::getPointListSize( ) const {
  return __pointList->size();
}

/*
ExplicitModel2DPtr
PointSet2D::transform( const Transformation2DPtr& transformation ) const {
  GEOM_ASSERT(transformation);
  return ExplicitModel2DPtr(new Point2DSet(transformation->transform(__pointList)));
}
*/

Vector2
PointSet2D::findClosest(const Vector2& point, uint_t * index) const
{
    std::pair<Point2Array::const_iterator,real_t> res = PGL::findClosest(*__pointList,point);
    if(index) *index = std::distance<Point2Array::const_iterator>(__pointList->begin(),res.first);
    return *res.first;
}

/* ----------------------------------------------------------------------- */
