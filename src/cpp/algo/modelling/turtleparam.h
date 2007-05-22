/* ---------------------------------------------------------------------------
 #
 #       File author(s): F. Boudon (frederic.boudon@loria.fr)
 #
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
#include <math/util_vector.h>
#include <math/util_matrix.h>
#include <vector>

PGL_BEGIN_NAMESPACE

/**! Class that contains all the parameters needed by the turtle :
	 position, orientation [heading,left,up] vectors
     color id, width, polygon and generalized cylinder flag */

class ALGO_API TurtleParam {

public:
	/// Constructor
	TurtleParam();
    
	/// reset parameter to initial value
    void reset();
    
	/// make a deep copy of this. usefull for putting a copy of this on a stack 
    virtual TurtleParam * copy();
    
	/// write main parameters values 
    void dump();
    
	/// transform this according to a Matrix3
    void transform(const TOOLS(Matrix3)&);
    
    /// get Current Orientation on the form of a matrix """
    TOOLS(Matrix3) getMatrix() const;
    
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
    
	/// set polygon flag
    void polygon(bool);
    
	/// set generalized cylinder flag
    void generalizedCylinder(bool);

	/// push the current position in the polygon points list
    void pushPosition();
    
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
  int color;
  int texture;
  real_t width;
  std::vector<TOOLS(Vector3)> pointList;
  std::vector<real_t> radiusList;

protected:
  bool __polygon;
  bool __generalizedCylinder;

};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */

#endif
