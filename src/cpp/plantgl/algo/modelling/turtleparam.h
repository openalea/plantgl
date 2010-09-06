/* ---------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al.
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
 *
 # ---------------------------------------------------------------------------
 #
 #                      GNU General Public Licence
 #
 #       This program is free software; you can redistribute it and/or
 #       modify it under the terms of the GNU General Public License as
 #       published by the Free Software Foundation; either version 2 of
 #       the License, or (at your option) any later version.
 #
 #       This program is distributed in the hope that it will be useful,
 #       but WITHOUT ANY WARRANTY; without even the implied warranty of
 #       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
 #       GNU General Public License for more details.
 #
 #       You should have received a copy of the GNU General Public
 #       License along with this program; see the file COPYING. If not,
 #       write to the Free Software Foundation, Inc., 59
 #       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 #
 # ---------------------------------------------------------------------------
 */

#ifndef __PGL_TURTLE_PARAM_H__
#define __PGL_TURTLE_PARAM_H__

#include "../algo_config.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/math/util_matrix.h>
#include <plantgl/scenegraph/geometry/curve.h>
#include <plantgl/scenegraph/geometry/lineicmodel.h>
#include <plantgl/scenegraph/function/function.h>
#include <vector>

PGL_BEGIN_NAMESPACE


class TurtlePath;
typedef RCPtr<TurtlePath> TurtlePathPtr;

/// Class that contains a path parameter that should be followed by the turtle
class ALGO_API TurtlePath : public TOOLS(RefCountObject){
public:
	TurtlePath(real_t totalLength, real_t actualLength) : __totalLength(totalLength), __actualLength(actualLength), __scale(totalLength/actualLength), __actualT(0)  { }
	virtual ~TurtlePath();

	virtual bool is2D() const { return true; }

	virtual TurtlePathPtr copy() const = 0;

	virtual void setPosition(real_t t)  = 0;

	real_t __totalLength;
	real_t __actualLength;
	real_t __scale;
	QuantisedFunctionPtr __arclengthParam;
	real_t __actualT;
};

/// Class that contains a 2D path parameter that should be followed by the turtle
class ALGO_API Turtle2DPath : public TurtlePath {
public:
	Turtle2DPath(Curve2DPtr curve, real_t totalLength, bool orientation = false, bool ccw = false);

	virtual TurtlePathPtr copy() const;
	virtual void setPosition(real_t t) ;
	
	// Path to follow
	Curve2DPtr __path;
	// Tell whether path is oriented with Y as first heading or X
	bool __orientation;
	// Tell whether the resulting structure is in CCW
	bool __ccw;

	// Position on the curve
	TOOLS(Vector2) __lastPosition;
	// Last direction on the curve
	TOOLS(Vector2) __lastHeading;
};

/// Class that contains a 2D path parameter that should be followed by the turtle
class ALGO_API Turtle3DPath : public TurtlePath {
public:
	Turtle3DPath(LineicModelPtr curve, real_t totalLength);

	virtual TurtlePathPtr copy() const;
	virtual void setPosition(real_t t) ;
	
	virtual bool is2D() const { return false; }

	// 3D Path to follow
	LineicModelPtr __path;

	// Position on the curve
	TOOLS(Vector3) __lastPosition;

	// Reference frame on the curve
	TOOLS(Vector3) __lastHeading;
	TOOLS(Vector3) __lastUp;
	TOOLS(Vector3) __lastLeft;

};

class TurtleDrawParameter {
public:

  TurtleDrawParameter();

  int color;

  Curve2DPtr crossSection;
  bool crossSectionCCW;
  bool defaultSection;

  TOOLS(Vector2) texCoordScale;
  TOOLS(Vector2) texCoordTranslation;
  TOOLS(Vector2) texCoordRotCenter;
  real_t texCoordRotAngle;

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
    void transform(const TOOLS(Matrix3)&);
    
    /// get Current Orientation on the form of a matrix """
    TOOLS(Matrix3) getOrientationMatrix() const;
    
    /// get transformtation on the form of a matrix """
    TOOLS(Matrix4) getTransformationMatrix() const;
    
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
	{ return __generalizedCylinder && pointList.size() <= 1; }
    
	/// test if generalized cylinder flag is on but no drawing has been made
    bool isGCorPolygonOnInit() const
	{ return (__generalizedCylinder||__polygon) && pointList.size() <= 1; }
    
	/// set polygon flag
    void polygon(bool);
    
	/// set generalized cylinder flag
    void generalizedCylinder(bool);

	/// push the current position in the polygon points list
    void pushPosition();

	/// push the current position in the polygon points list
    void pushRadius();
    
    void setPosition(const TOOLS(Vector3)& pos){
	  position = pos;
	}

    const TOOLS(Vector3)& getPosition() const {
	  return position ;
	}

public:
  
  TOOLS(Vector3) position;
  TOOLS(Vector3) heading;
  TOOLS(Vector3) left;
  TOOLS(Vector3) up;
  TOOLS(Vector3) scale;

  uint_t lastId;
  real_t width;

  std::vector<TOOLS(Vector3)> pointList;
  std::vector<TOOLS(Vector3)> leftList;
  std::vector<real_t> radiusList;

  uint_t customId;
  uint_t customParentId;

  uint_t sectionResolution;

  TurtleDrawParameter initial;

  TOOLS(Vector3) tropism;
  real_t elasticity;

  TurtlePathPtr guide;

protected:
  bool __polygon;
  bool __generalizedCylinder;

};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

#endif
