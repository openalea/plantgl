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

    inline const Vector3& getPosition() const
    { return __params->position; }

    const Vector3& getHeading() const
    { return __params->heading; }

    inline const Vector3& getUp() const
    { return __params->up; }

    inline const Vector3& getLeft() const
    { return __params->left; }

    inline const Vector3& getScale() const
    { return __params->scale; }

    inline const Matrix4 getTransformationMatrix() const
    { return __params->getTransformationMatrix(); }

    inline const Matrix3 getOrientationMatrix() const
    { return __params->getOrientationMatrix(); }

    inline real_t getWidth() const
    { return __params->width; }

    inline int getColor() const
    { return __params->color; }

    inline uint_t getId() const
    { return id; }

    inline const Vector3& getTropism() const
    { return __params->tropism; }

    inline void setTropism(const Vector3& val)
    { __params->tropism= val.normed(); }

    inline void setTropism(real_t x = 0, real_t y = 0, real_t z = 1)
    { __params->tropism= Vector3(x,y,z).normed(); }

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
    void setNoId();

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

    /// Roll such as up vector comes in the top direction
    virtual void rollToVert(real_t alpha = 1.0, const Vector3& top = TOOLS(Vector3::OZ));

    /// Roll such as head vector comes in the horizontal plane
    virtual void rollToHorizontal(real_t alpha = 1.0, const Vector3& top = TOOLS(Vector3::OZ)) ;

    /// set Heading and Up vectors
    virtual void setHead(const Vector3& h, const Vector3& u = TOOLS(Vector3::OX));

    /// set Heading and Up vectors
    inline void setHead(real_t hx = 0, real_t hy = 0, real_t hz = 1,
                          real_t ux = 1, real_t uy = 0, real_t uz = 0)
    { setHead(Vector3(hx,hy,hz),Vector3(ux,uy,uz)); }

    /// Orient the turtle according to the absolute euler angles given
    virtual void eulerAngles(real_t azimuth = 180, real_t elevation = 90, real_t roll = 0);

    /// Change position to pos
    virtual void move(const Vector3& pos);

    /// Change position to (x,y,z)
    inline void move(real_t x = 0, real_t y = 0, real_t z = 0)
    { move(Vector3(x,y,z)); }

    /// Shift the position of pos
    virtual void shift(const Vector3 & pos);

    inline void shift(real_t x = 0, real_t y = 0, real_t z = 0)
    { shift(Vector3(x,y,z)); }

    /// Trace line to v without changing the orientation
    virtual void lineTo(const Vector3 & v, real_t topradius = -1.0);

    /// Trace line to pos+v without changing the orientation
    virtual void lineRel(const Vector3 & v, real_t topradius = -1.0);

    /// Change the orientation to pinpoint v
    void pinpoint(const Vector3 & v);

    /// Change the orientation to pinpoint pos+v
    virtual void pinpointRel(const Vector3 & v);

    /// Trace line toward v and change the orientation
    void oLineTo(const Vector3& v, real_t topradius = -1.0);

    /// Trace line toward pos+v and change the orientation
    virtual void oLineRel(const Vector3& v, real_t topradius = -1.0);

    inline void lineTo(real_t x = 0, real_t y = 0, real_t z = 0, real_t topradius = -1.0)
    { lineTo(Vector3(x,y,z),topradius); }

    inline void pinpoint(real_t x = 0, real_t y = 0, real_t z = 0)
    { pinpoint(Vector3(x,y,z)); }

    inline void oLineTo(real_t x = 0, real_t y = 0, real_t z = 0, real_t topradius = -1.0)
    { oLineTo(Vector3(x,y,z),topradius); }

    inline void lineRel(real_t x = 0, real_t y = 0, real_t z = 0, real_t topradius = -1.0)
    { lineRel(Vector3(x,y,z),topradius); }

    inline void pinpointRel(real_t x = 0, real_t y = 0, real_t z = 0)
    { pinpointRel(Vector3(x,y,z)); }

    inline void oLineRel(real_t x = 0, real_t y = 0, real_t z = 0, real_t topradius = -1.0)
    { oLineRel(Vector3(x,y,z),topradius); }

    virtual void transform(const Matrix3& matrix);

    virtual void scale(const Vector3&);

    inline void scale(real_t sx, real_t sy, real_t sz)
    {  scale(Vector3(sx,sy,sz)); }

    inline void scale(real_t s )
    { scale(Vector3(s,s,s)); }

    inline void scale()
    { scale(Vector3(1,1,1)); }

    inline void multScale(const Vector3& s)
    { const Vector3& cur_scale = getScale();
      scale(Vector3(cur_scale.x() * s.x(),cur_scale.y() * s.y(),cur_scale.z() * s.z())); }

    inline void multScale(real_t sx, real_t sy, real_t sz)
    { const Vector3& cur_scale = getScale();
      scale(Vector3(cur_scale.x() * sx,cur_scale.y() * sy,cur_scale.z() * sz)); }

    inline void multScale(real_t s)
    { multScale(Vector3(s,s,s)); }

    inline void multScale()
    {  multScale(Vector3(scale_multiplier,scale_multiplier,scale_multiplier)); }

    inline void divScale(const Vector3& s)
    { const Vector3& cur_scale = getScale();
      scale(Vector3(cur_scale.x() / s.x(),cur_scale.y() / s.y(),cur_scale.z() / s.z())); }

    inline void divScale(real_t sx, real_t sy, real_t sz)
    { const Vector3& cur_scale = getScale();
      scale(Vector3(cur_scale.x() / sx,cur_scale.y() / sy,cur_scale.z() / sz)); }

    inline void divScale(real_t s)
    { divScale(Vector3(s,s,s)); }

    inline void divScale()
    { divScale(Vector3(scale_multiplier,scale_multiplier,scale_multiplier)); }

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
    virtual void frame(real_t heigth, real_t cap_heigth_ratio = 0.2, real_t cap_radius_ratio = 2, real_t colorV = 1.0, real_t transparency = 0.0);

    inline void arrow() { arrow(default_step); }
    virtual void arrow(real_t heigth, real_t cap_heigth_ratio = 0.2, real_t cap_radius_ratio = 2);

    inline void setTextureScale(real_t u, real_t v) { setTextureScale(Vector2(u,v)); }
    virtual void setTextureScale(const Vector2& s);

    virtual void setTextureVScale(real_t v);
    virtual void setTextureUScale(real_t u);

    inline void setTextureRotation(real_t angle, real_t ucenter = 0.5, real_t vcenter = 0.5)
    { setTextureRotation(angle, Vector2(ucenter,vcenter)); }
    virtual void setTextureRotation(real_t angle, const Vector2& center);

    inline void setTextureTranslation(real_t u = 0.5, real_t v = 0.5)
    { setTextureTranslation(Vector2(u,v)); }

    virtual void setTextureTranslation(const Vector2& t);

    virtual void setTextureTransformation(const  Vector2& scaling,
                                          const Vector2&  translation,
                                          real_t angle, const Vector2& rotcenter);

    inline void setTextureTransformation(real_t uscaling, real_t vscaling,
                                  real_t utranslation, real_t vtranslation,
                                  real_t angle, real_t urotcenter, real_t vrotcenter)
    { setTextureTransformation(Vector2(uscaling,vscaling),Vector2(utranslation,vtranslation), angle, Vector2(urotcenter, vrotcenter)); }

    virtual void setTextureBaseColor(const Color4& v);
    virtual void setTextureBaseColor(int v);
    virtual void interpolateTextureBaseColors(int val1, int val2, real_t alpha = 0.5);

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

    void setGuide(const Curve2DPtr& path, real_t length, bool yorientation = false, bool ccw = false);

    void setGuide(const LineicModelPtr& path, real_t length);

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

    bool path_info_cache_enabled;

    inline void enablePathInfoCache(bool b) { path_info_cache_enabled = b; }
    inline bool pathInfoCacheEnabled() const { return path_info_cache_enabled; }

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
                                      const std::vector<Vector3>& left,
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

    virtual void _arrow(real_t heigth, real_t cap_heigth_ratio, real_t cap_radius_ratio) { }

    virtual void _label(const std::string& text, int size = -1){}

    TurtleParam& getParameters()
      { return *__params; }

    void _applyTropism();

    void _applyGuide(real_t& l);
    void _ajustToGuide();

    void _tendTo(const Vector3& t, real_t strength = 1.0);

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

    PathInfoMap __pathinfos;


};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif
