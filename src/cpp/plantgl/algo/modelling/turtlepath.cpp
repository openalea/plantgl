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


#include "turtlepath.h"
#include <plantgl/scenegraph/geometry/polyline.h>
#include <plantgl/math/util_math.h>
#include <plantgl/tool/util_types.h>
#include <plantgl/tool/util_string.h>

#include <sstream>

using namespace std;
PGL_USING_NAMESPACE
/*----------------------------------------------------------*/

TurtlePath::~TurtlePath() { }

Turtle2DPath::Turtle2DPath(Curve2DPtr curve, real_t totalLength, real_t actualLength, bool orientation, bool ccw, QuantisedFunctionPtr arclengthParam) :
    TurtlePath(totalLength,actualLength?actualLength:curve->getLength(),arclengthParam),
    __path(curve),
    __orientation(orientation),
    __lastHeading(orientation?0:1,orientation?1:0)
{

      if(is_null_ptr(arclengthParam))__arclengthParam = curve->getArcLengthToUMapping();
      __lastPosition = curve->getPointAt(curve->getFirstKnot());
      __ccw = ccw;
      if (__ccw) {
          if(orientation) { __lastHeading = Vector2(0,-1); }
          else { __lastHeading = Vector2(-1,0); }
      }
}

TurtlePathPtr Turtle2DPath::copy() const
{ return TurtlePathPtr(new Turtle2DPath(*this)); }

void Turtle2DPath::setPosition(real_t t)
{
    __actualT = t / __totalLength;
    if  (__actualT <= 0) {__actualT = 0;}
    else if (__actualT >= 1.0) {__actualT = 1.0;}
    real_t actualU = __arclengthParam->getValue(__actualT);
    __lastPosition = __path->getPointAt(actualU);
    __lastHeading = __path->getTangentAt(actualU);
    __lastHeading.normalize();
}


Turtle3DPath::Turtle3DPath(LineicModelPtr curve, real_t totalLength, real_t actualLength, QuantisedFunctionPtr arclengthParam) :
    TurtlePath(totalLength,actualLength?actualLength:curve->getLength(),arclengthParam), __path(curve), __lastHeading(0,0,1), __lastUp(1,0,0), __lastLeft(0,-1,0)
{
    if(is_null_ptr(arclengthParam))__arclengthParam = curve->getArcLengthToUMapping();
    __lastPosition = curve->getPointAt(curve->getFirstKnot());
}

void Turtle3DPath::setPosition(real_t t)
{
    __actualT = t / __totalLength;
    if  (__actualT <= 0) {__actualT = 0;}
    else if (__actualT >= 1.0){ __actualT = 1.0;}
    real_t actualU = __arclengthParam->getValue(__actualT);
    __lastPosition = __path->getPointAt(actualU);
    __lastHeading = __path->getTangentAt(actualU);
    __lastHeading.normalize();
    __lastUp = __path->getNormalAt(actualU);
    __lastUp.normalize();
    __lastLeft = cross(__lastUp,__lastHeading);
    __lastLeft.normalize();
}

TurtlePathPtr Turtle3DPath::copy() const
{ return TurtlePathPtr(new Turtle3DPath(*this)); }
