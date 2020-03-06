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

#ifndef __PGL_TURTLE_PARAM_H__
#define __PGL_TURTLE_PARAM_H__

#include "../algo_config.h"
#include "turtlepath.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/math/util_matrix.h>
#include <plantgl/scenegraph/appearance/color.h>
#include <plantgl/scenegraph/geometry/curve.h>
#include <plantgl/scenegraph/geometry/lineicmodel.h>
#include <plantgl/scenegraph/function/function.h>
#include <plantgl/scenegraph/appearance/appearance.h>
#include <vector>

PGL_BEGIN_NAMESPACE
class TurtleDrawParameter {
public:

  TurtleDrawParameter();

  int color;
  AppearancePtr customMaterial;

  Curve2DPtr crossSection;
  bool crossSectionCCW;
  bool defaultSection;

  Vector2 texCoordScale;
  Vector2 texCoordTranslation;
  Vector2 texCoordRotCenter;
  real_t texCoordRotAngle;
  Color4 texBaseColor;

  real_t axialLength;

  void reset();
};

/**! Class that contains all the parameters needed by the turtle :
     position, orientation [heading,left,up] vectors
     color id, width, polygon and generalized cylinder flag */

class ALGO_API TurtleParam  : public TurtleDrawParameter {

public:
    /// Constructor
    TurtleParam();

    /// Constructor
    virtual ~TurtleParam();

    /// reset parameter to initial value
    void reset();

    /// make a deep copy of this. usefull for putting a copy of this on a stack
    virtual TurtleParam * copy();

    /// write main parameters values
    void dump();

    /// transform this according to a Matrix3
    void transform(const Matrix3&);

    /// get Current Orientation on the form of a matrix """
    Matrix3 getOrientationMatrix() const;

    /// get transformtation on the form of a matrix """
    Matrix4 getTransformationMatrix() const;

    /// test validity of . Should be orthonormal and normalized """
    bool isValid() const;

    /// keep last point for drawing generalized cylinder. usefull for lateral axis
    void keepLastPoint();

    /// clear the points stacks
    void removePoints();

    /// test if polygon flag is on
    bool isPolygonOn() const
    {  return __polygon; }


    /// test if generalized cylinder flag is on
    bool isGeneralizedCylinderOn() const
    { return __generalizedCylinder; }

    /// test if generalized cylinder flag is on but no drawing has been made
    bool isGeneralizedCylinderOnInit() const
    { return __generalizedCylinder && pointList->size() <= 1; }

    /// test if generalized cylinder flag is on but no drawing has been made
    bool isGCorPolygonOnInit() const
    { return (__generalizedCylinder||__polygon) && pointList->size() <= 1; }

    /// set polygon flag
    void polygon(bool);

    /// set generalized cylinder flag
    void generalizedCylinder(bool);

    /// push the current position in the polygon points list
    void pushPosition();

    /// push the current position in the polygon points list
    void pushRadius();

    void setPosition(const Vector3& pos){
      position = pos;
    }

    const Vector3& getPosition() const {
      return position ;
    }

    void forward(real_t l);

    void left(real_t angle);

    void up(real_t angle);

    void rollR(real_t angle);

public:

  Vector3 position;
  Vector3 heading;
  Vector3 left;
  Vector3 up;
  Vector3 scale;
  Vector3 reflection;

  real_t width;

  uint_t customId;
  uint_t customParentId;
  uint_t lastId;

  uint_t sectionResolution;

  Point3ArrayPtr pointList;
  std::vector<Vector3> leftList;
  std::vector<real_t> radiusList;

  TurtleDrawParameter initial;

  Vector3 tropism;
  real_t elasticity;

  TurtlePathPtr guide;

  bool screenCoordinates;

protected:
  bool __polygon;
  bool __generalizedCylinder;

};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

#endif
