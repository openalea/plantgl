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

#ifndef __PGL_TURTLE_H__
#define __PGL_TURTLE_H__

#include "turtleparam.h"

#include <string>
#include <stack>


PGL_BEGIN_NAMESPACE

/// Turtle class allow rotation, displacement and drawing operation
class ALGO_API Turtle {
public:
    typedef void (* error_msg_handler_func) ( const std::string& );
    static void register_error_handler(error_msg_handler_func f = NULL);

    Turtle(TurtleParam * params = NULL);
    virtual ~Turtle();
    
    virtual std::string str() const;
    
    virtual void reset();
    void resetValues();
    
    void dump() const ;
    
    inline const TurtleParam& getParameters() const
	{ return *__params; }
   
	/// test validity of self
    bool isValid() const ;
    
	/// first command to call to initialize the turtle
    void start();
    
	/// last command to call
    void stop();
    
    inline const std::stack<TurtleParam *>& getStack() const
	{ return __paramstack; }
    
    inline bool emptyStack() const
	{ return __paramstack.empty(); }
    
    inline const TOOLS(Vector3)& getPosition() const
	{ return __params->position; }
        
    const TOOLS(Vector3)& getHeading() const
	{ return __params->heading; }
    
    inline const TOOLS(Vector3)& getUp() const
	{ return __params->up; }
    
    inline const TOOLS(Vector3)& getLeft() const
	{ return __params->left; }
    
    inline const TOOLS(Vector3)& getScale() const
	{ return __params->scale; }

    inline real_t getWidth() const
	{ return __params->width; }
    
    inline int getColor() const
	{ return __params->color; }
    
    inline uint_t getId() const
	{ return id; }
    
	/// push the current turtle state on a stack
    virtual void push();

	/// pop the turtle state from a stack and assign it to self
    virtual void pop();

	/// Set Id
    virtual void setId(uint_t i) { id = i; }
    void incId();
    void decId() ;

	/// Move of l step in heading direction
    inline void f() { f(default_step); }
    virtual void f(real_t length);

    /// Move of l step in heading direction and draw a cylinder (or a frustum if topdiam is given)
    inline void F() { F(default_step,-1); }
    inline void F(real_t length) { F(length,-1); }
    virtual void F(real_t length, real_t topdiam);
    
    /// Rotate on the left of default angle
    inline void left()
	{ left(angle_increment); }

	/// Rotate on the left of angle degrees
    virtual void left(real_t angle);
    
	/// Rotate on the right of default angle
    inline void right()
	{ left(-angle_increment); }

	/// Rotate on the right of angle degrees
    virtual void right(real_t angle)
	{ left(-angle); }
    
	/// Rotate in up direction of default angle
    inline void up()
	{ down(-angle_increment); }

	/// Rotate in up direction of angle degrees
    virtual void up(real_t angle)
	{ down(-angle); }
    
	/// Rotate in down direction of default angle
    inline void down()
	{ down(angle_increment); }

	/// Rotate in down direction of angle degrees
    virtual void down(real_t angle);
    
    /// Roll in the left direction of default angle
	inline void rollL() { rollL(angle_increment); }

	/// Roll in the left direction of angle degrees
    virtual void rollL(real_t angle);
    
	/// Roll in the right direction of default angle
    inline void rollR() { rollR(angle_increment); }

	/// Roll in the right direction of angle degrees
    virtual void rollR(real_t angle)  { rollL(-angle); }
    
	/// Roll such as up vector comes in the Z direction
    inline void rollToVert() { rollToVert(TOOLS(Vector3::OZ)); }
    
	/// Roll such as up vector comes in the top direction
    virtual void rollToVert(const TOOLS(Vector3)& top);
    
	/// set Heading and Up vectors
    virtual void setHead(const TOOLS(Vector3)& h, const TOOLS(Vector3)& u);

	/// set Heading and Up vectors
    inline void setHead(real_t hx = 0, real_t hy = 0, real_t hz = 1, 
						  real_t ux = 1, real_t uy = 0, real_t uz = 0)
	{ setHead(TOOLS(Vector3)(hx,hy,hz),TOOLS(Vector3)(ux,uy,uz)); }	
    
	/// Change position to pos
    virtual void move(const TOOLS(Vector3)& pos);
    
    /// Change position to (x,y,z)
    inline void move(real_t x = 0, real_t y = 0, real_t z = 0)
	{ move(TOOLS(Vector3)(x,y,z)); }
    
	/// Decal the position of pos
    virtual void decal(const TOOLS(Vector3) & pos);
    
    inline void decal(real_t x = 0, real_t y = 0, real_t z = 0)
	{ decal(TOOLS(Vector3)(x,y,z)); }

    virtual void transform(const TOOLS(Matrix3)& matrix);

    inline void traceTo(real_t x, real_t y, real_t z)
	{ traceTo(TOOLS(Vector3)(x,y,z)); }

    virtual void traceTo(const TOOLS(Vector3)& v);

	virtual void scale(const TOOLS(Vector3)&);

   	inline void scale(real_t sx, real_t sy, real_t sz)
	{  scale(TOOLS(Vector3)(sx,sy,sz)); }

   	inline void scale(real_t s )
	{ scale(TOOLS(Vector3)(s,s,s)); }

   	inline void scale()
	{ scale(TOOLS(Vector3)(1,1,1)); }

   	inline void multScale(const TOOLS(Vector3)& s)
	{ const TOOLS(Vector3)& cur_scale = getScale();
	  scale(TOOLS(Vector3)(cur_scale.x() * s.x(),cur_scale.y() * s.y(),cur_scale.z() * s.z())); }

   	inline void multScale(real_t sx, real_t sy, real_t sz)
	{ const TOOLS(Vector3)& cur_scale = getScale();
	  scale(TOOLS(Vector3)(cur_scale.x() * sx,cur_scale.y() * sy,cur_scale.z() * sz)); }

   	inline void multScale(real_t s)
	{ multScale(TOOLS(Vector3)(s,s,s)); }

   	inline void multScale()
    {  multScale(TOOLS(Vector3)(scale_multiplier,scale_multiplier,scale_multiplier)); }

   	inline void divScale(const TOOLS(Vector3)& s)
	{ const TOOLS(Vector3)& cur_scale = getScale();
	  scale(TOOLS(Vector3)(cur_scale.x() / s.x(),cur_scale.y() / s.y(),cur_scale.z() / s.z())); }

   	inline void divScale(real_t sx, real_t sy, real_t sz)
	{ const TOOLS(Vector3)& cur_scale = getScale();
	  scale(TOOLS(Vector3)(cur_scale.x() / sx,cur_scale.y() / sy,cur_scale.z() / sz)); }

   	inline void divScale(real_t s)
	{ divScale(TOOLS(Vector3)(s,s,s)); }

   	inline void divScale()
	{ divScale(TOOLS(Vector3)(scale_multiplier,scale_multiplier,scale_multiplier)); }

    virtual void setColor(int val);

    inline void incColor()
	{ setColor( getColor() + color_increment ); }
    
    inline void decColor()
	{  setColor( getColor() - color_increment ); }
    
    virtual void setWidth(real_t val);
    
    inline void incWidth()
    { setWidth( getWidth() + width_increment ); }	
    
    inline void decWidth()
	{ setWidth( getWidth() - width_increment ); }	

    virtual void setTexture(int val);

    inline void incTexture()
	{ setTexture( getColor() + color_increment ); }
    
    inline void decTexture()
	{ setTexture( getColor() - color_increment ); }
    
    virtual void startPolygon();
    
    virtual void stopPolygon();
    
	/// start Generalized Cylinder
    virtual void startGC();
    
	/// stop Generalized Cylinder
    virtual void stopGC();
    
    virtual size_t getColorListSize() const
	  { return 0; }
    
    virtual size_t getTextureListSize() const
	  { return 0; }

    inline void sphere()
	{ _sphere(getWidth()); }

    inline void sphere(real_t radius )
	{ _sphere(radius); }
    
    inline void circle()
	{ _circle(getWidth()); }

    inline void circle(real_t radius )
	{ _circle(radius); }
    
    virtual void label(const std::string& text )
	{ if(!text.empty())_label(text); }

    virtual void surface(const std::string& name, real_t scale=1)
	{ if (!name.empty()) _surface(name,(scale > GEOM_EPSILON ? scale : 1)); }

	inline void setDefaultStep(real_t val)
	{ default_step = (val > 0 ? val : - val); }

	inline void setAngleIncrement(real_t val)
	{ angle_increment = (val > 0 ? val : - val); }
    
	inline void setWidthIncrement(real_t val)
	{ width_increment = (val > 0 ? val : - val); }
    
	inline void setColorIncrement(int val)
	{ color_increment = (val > 0 ? val : - val); }
    
	inline void setScaleMultiplier(real_t val)
	{ scale_multiplier = (val > 0 ? val : - val); }
    
    virtual void error(const std::string& error_string);
    virtual void warning(const std::string& error_string);

protected:
	virtual void _frustum(real_t length, real_t topdiam){}
    
    virtual void _cylinder(real_t length){}

    virtual void _polygon(const std::vector<TOOLS(Vector3)>& points){}
    
    virtual void _generalizedCylinder(const std::vector<TOOLS(Vector3)>& points,
									  const std::vector<real_t>& radius){}

    virtual void _sphere(real_t radius){}
    
    virtual void _circle(real_t radius){}

    virtual void _surface(const std::string& name, real_t scale){}

    virtual void _label(const std::string& text ){}

    TurtleParam& getParameters()
	  { return *__params; }

    uint_t popId();

    TurtleParam *        __params;

    std::stack<TurtleParam *> __paramstack;

	real_t default_step;
	real_t angle_increment;
	real_t width_increment;
	int color_increment;
	real_t scale_multiplier;
    uint_t id;
    uint_t parentId;

};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
