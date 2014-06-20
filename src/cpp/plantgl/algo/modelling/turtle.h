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
    static void register_warning_handler(error_msg_handler_func f = NULL);

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

    inline const TOOLS(Matrix4) getTransformationMatrix() const
	{ return __params->getTransformationMatrix(); }

    inline const TOOLS(Matrix3) getOrientationMatrix() const
	{ return __params->getOrientationMatrix(); }

    inline real_t getWidth() const
	{ return __params->width; }
    
    inline int getColor() const
	{ return __params->color; }
    
    inline uint_t getId() const
	{ return id; }
    
    inline const TOOLS(Vector3)& getTropism() const
	{ return __params->tropism; }

	inline void setTropism(const TOOLS(Vector3)& val)
	{ __params->tropism= val.normed(); }

	inline void setTropism(real_t x = 0, real_t y = 0, real_t z = 1)
	{ __params->tropism= TOOLS(Vector3)(x,y,z).normed(); }

    inline const real_t& getElasticity() const
	{ return __params->elasticity; }
	
	void setElasticity(real_t val = 0) 
	{ __params->elasticity = val; }

	/// push the current turtle state on a stack
    virtual void push();

	/// pop the turtle state from a stack and assign it to self
    virtual void pop();

	/// Set Id
    virtual void setId(uint_t i) { id = i; }
    void incId(uint_t i = 1);
    void decId(uint_t i = 1) ;

	/// Move of l step in heading direction
    inline void f() { f(default_step); }
    virtual void f(real_t length);

    /// Move of l step in heading direction and draw a cylinder (or a frustum if topradius is given)
    inline void F() { F(default_step,-1); }
    inline void F(real_t length) { F(length,-1); }
    virtual void F(real_t length, real_t topradius);
    
    void nF(real_t length, real_t dl);
    void nF(real_t length, real_t dl, real_t radius, const QuantisedFunctionPtr radiusvariation = NULL);

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

	/// Roll intrinsically in the left direction of angle degrees
    virtual void iRollL(real_t angle);
    inline void iRollL() { iRollL(angle_increment); }

	/// Roll intrinsically in the rigth direction of angle degrees
	virtual void iRollR(real_t angle) { iRollL(-angle); }
    inline void iRollR() { iRollL(-angle_increment); }

	/// Turn Around turtle
	inline void turnAround() { left(180); }

	/// Roll such as up vector comes in the Z direction
    inline void rollToVert() { rollToVert(TOOLS(Vector3::OZ)); }
    
	/// Roll such as up vector comes in the top direction
    virtual void rollToVert(const TOOLS(Vector3)& top);
    
	/// set Heading and Up vectors
    virtual void setHead(const TOOLS(Vector3)& h, const TOOLS(Vector3)& u = TOOLS(Vector3::OX));

	/// set Heading and Up vectors
    inline void setHead(real_t hx = 0, real_t hy = 0, real_t hz = 1, 
						  real_t ux = 1, real_t uy = 0, real_t uz = 0)
	{ setHead(TOOLS(Vector3)(hx,hy,hz),TOOLS(Vector3)(ux,uy,uz)); }	
    
	/// Change position to pos
    virtual void move(const TOOLS(Vector3)& pos);
    
    /// Change position to (x,y,z)
    inline void move(real_t x = 0, real_t y = 0, real_t z = 0)
	{ move(TOOLS(Vector3)(x,y,z)); }
    
	/// Shift the position of pos
    virtual void shift(const TOOLS(Vector3) & pos);
    
    inline void shift(real_t x = 0, real_t y = 0, real_t z = 0)
	{ shift(TOOLS(Vector3)(x,y,z)); }

	/// Trace line to v without changing the orientation
    virtual void lineTo(const TOOLS(Vector3) & v, real_t topradius = -1.0);

	/// Trace line to pos+v without changing the orientation
    virtual void lineRel(const TOOLS(Vector3) & v, real_t topradius = -1.0);

	/// Change the orientation to pinpoint v
    void pinpoint(const TOOLS(Vector3) & v);

	/// Change the orientation to pinpoint pos+v
    virtual void pinpointRel(const TOOLS(Vector3) & v);

	/// Trace line toward v and change the orientation
    void oLineTo(const TOOLS(Vector3)& v, real_t topradius = -1.0);

	/// Trace line toward pos+v and change the orientation
    virtual void oLineRel(const TOOLS(Vector3)& v, real_t topradius = -1.0);

	inline void lineTo(real_t x = 0, real_t y = 0, real_t z = 0, real_t topradius = -1.0)
	{ lineTo(TOOLS(Vector3)(x,y,z),topradius); }

	inline void pinpoint(real_t x = 0, real_t y = 0, real_t z = 0)
	{ pinpoint(TOOLS(Vector3)(x,y,z)); }

	inline void oLineTo(real_t x = 0, real_t y = 0, real_t z = 0, real_t topradius = -1.0)
	{ oLineTo(TOOLS(Vector3)(x,y,z),topradius); }

	inline void lineRel(real_t x = 0, real_t y = 0, real_t z = 0, real_t topradius = -1.0)
	{ lineRel(TOOLS(Vector3)(x,y,z),topradius); }

	inline void pinpointRel(real_t x = 0, real_t y = 0, real_t z = 0)
	{ pinpointRel(TOOLS(Vector3)(x,y,z)); }

	inline void oLineRel(real_t x = 0, real_t y = 0, real_t z = 0, real_t topradius = -1.0)
	{ oLineRel(TOOLS(Vector3)(x,y,z),topradius); }

    virtual void transform(const TOOLS(Matrix3)& matrix);

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

    virtual void interpolateColors(int val1, int val2, real_t alpha = 0.5);
    
    void setCustomAppearance(const AppearancePtr app);
    inline void removeCustomAppearance() { setCustomAppearance(AppearancePtr()); }
    
    virtual void setWidth(real_t val);
    
    inline void incWidth()
    { setWidth( getWidth() + width_increment ); }	
    
    inline void decWidth()
	{ setWidth( getWidth() - width_increment ); }	

    virtual void startPolygon();
    
    virtual void stopPolygon(bool concavetest = false);
    
    virtual void polygonPoint();
    
	/// start Generalized Cylinder
    virtual void startGC();
    
	/// stop Generalized Cylinder
    virtual void stopGC();
    
    
    virtual size_t getColorListSize() const
	  { return 0; }
    
    inline void sphere()
	{ _sphere(getWidth()); }

    void sphere(real_t radius );
    
    inline void circle()
	{ _circle(getWidth()); }

    void circle(real_t radius );

    void box(real_t length, real_t topradius);
    inline void box(real_t length) { box(length,getWidth());}
    inline void box() { box(default_step,getWidth());}

    void quad(real_t length, real_t topradius);
    inline void quad(real_t length) { quad(length,getWidth());}
    inline void quad() { quad(default_step,getWidth());}

    virtual void label(const std::string& text, int size = -1 );

    virtual void surface(const std::string& name, real_t scale=1);

	inline void frame() { frame(default_step); }
    virtual void frame(real_t heigth, real_t cap_heigth_ratio = 0.2, real_t cap_radius_ratio = 2, real_t color = 1.0, real_t transparency = 0.0);

    inline void setTextureScale(real_t u, real_t v) { setTextureScale(TOOLS(Vector2)(u,v)); }
    virtual void setTextureScale(const TOOLS(Vector2)& s);

    virtual void setTextureVScale(real_t v);
    virtual void setTextureUScale(real_t u);

    inline void setTextureRotation(real_t angle, real_t ucenter = 0.5, real_t vcenter = 0.5) 
    { setTextureRotation(angle, TOOLS(Vector2)(ucenter,vcenter)); }
    virtual void setTextureRotation(real_t angle, const TOOLS(Vector2)& center);

    inline void setTextureTranslation(real_t u = 0.5, real_t v = 0.5)
    { setTextureTranslation(TOOLS(Vector2)(u,v)); }

    virtual void setTextureTranslation(const TOOLS(Vector2)& t);

    virtual void setTextureTransformation(const  TOOLS(Vector2)& scaling, 
									      const TOOLS(Vector2)&  translation, 
										  real_t angle, const TOOLS(Vector2)& rotcenter);

    inline void setTextureTransformation(real_t uscaling, real_t vscaling, 
								  real_t utranslation, real_t vtranslation, 
								  real_t angle, real_t urotcenter, real_t vrotcenter)
    { setTextureTransformation(TOOLS(Vector2)(uscaling,vscaling),TOOLS(Vector2)(utranslation,vtranslation), angle, TOOLS(Vector2)(urotcenter, vrotcenter)); }

	inline void setDefaultStep(real_t val)
	{ default_step = (val > 0 ? val : - val); }

	inline real_t getDefaultStep() const
	{ return default_step ; }

	inline void setAngleIncrement(real_t val)
	{ angle_increment = (val > 0 ? val : - val); }

	inline real_t getAngleIncrement() const 
	{ return angle_increment; }

	inline void setWidthIncrement(real_t val)
	{ width_increment = (val > 0 ? val : - val); }
    
	inline real_t getWidthIncrement() const
	{ return width_increment; }
    
	inline void setColorIncrement(int val)
	{ color_increment = (val > 0 ? val : - val); }

	inline int getColorIncrement() const
	{ return color_increment; }

	inline void setScaleMultiplier(real_t val)
	{ scale_multiplier = (val > 0 ? val : - val); }

    void setCrossSection(const Curve2DPtr& curve, bool ccw = false);
	void setDefaultCrossSection(size_t slicenb);
	inline void setDefaultCrossSection()
	{ setDefaultCrossSection(getParameters().sectionResolution); }

	void setSectionResolution(uint_t resolution) ;

	inline const uint_t& getSectionResolution() const
	{ return getParameters().sectionResolution; }

	inline void setGuide(const Curve2DPtr& path, real_t length, bool yorientation = false, bool ccw = false)
	{ getParameters().guide = TurtlePathPtr(new Turtle2DPath(path,length,yorientation,ccw)); }

	inline void setGuide(const LineicModelPtr& path, real_t length)
	{ getParameters().guide = TurtlePathPtr(new Turtle3DPath(path,length)); }

	inline void clearGuide()
	{ getParameters().guide = TurtlePathPtr(); }

	void setPositionOnGuide(real_t t);

    void sweep(const Curve2DPtr& path, const Curve2DPtr& section, real_t length, real_t dl, real_t radius = 1.0, const QuantisedFunctionPtr radiusvariation = NULL);
    void sweep(const LineicModelPtr& path, const Curve2DPtr& section, real_t length, real_t dl, real_t radius = 1.0, const QuantisedFunctionPtr radiusvariation = NULL);


    inline void setScreenCoordinatesEnabled(bool enabled = true)
    { getParameters().screenCoordinates = enabled; }

    virtual void error(const std::string& error_string);
    virtual void warning(const std::string& error_string);

	bool warn_on_error;

	inline void setWarnOnError(bool b) { warn_on_error = b; }
	inline bool warnOnError() const { return warn_on_error; }

	void leftReflection();
	void upReflection();
	void headingReflection();

protected:
    void _setCrossSection(const Curve2DPtr& curve, bool ccw = false, bool defaultSection = false);

	virtual void _frustum(real_t length, real_t topradius){}
    virtual void _cylinder(real_t length){}

    virtual void _sweep(real_t length, real_t topradius);

    virtual void _polygon(const Point3ArrayPtr& points, bool concavetest = false){}
    
    virtual void _generalizedCylinder(const Point3ArrayPtr& points,
									  const std::vector<TOOLS(Vector3)>& left,
									  const std::vector<real_t>& radius,
									  const Curve2DPtr& crossSection,
									  bool crossSectionCCW,
									  bool currentcolor = false){}

    virtual void _sphere(real_t radius){}
    
    virtual void _circle(real_t radius){}

    virtual void _box(real_t radius, real_t botradius, real_t topradius){}
    
    virtual void _quad(real_t radius, real_t botradius, real_t topradius){}

    virtual void _surface(const std::string& name, real_t scale){}

	virtual void _frame(real_t heigth, real_t cap_heigth_ratio, real_t cap_radius_ratio, real_t color, real_t transparency) { }

    virtual void _label(const std::string& text, int size = -1){}

    TurtleParam& getParameters()
	  { return *__params; }

	void _applyTropism();

	void _applyGuide(real_t& l);
	void _ajustToGuide();

	void _tendTo(const TOOLS(Vector3)& t, real_t strength = 1.0);

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
