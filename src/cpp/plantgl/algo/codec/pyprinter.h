#ifndef __actn_pyprinter_h__
#define __actn_pyprinter_h__

#include <plantgl/tool/rcobject.h>
#include "printer.h"


/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */


class CODEC_API PyPrinter : public Printer
{
public:
	PyPrinter(std::ostream& stream);
	virtual ~PyPrinter();

	  /// @name Pre and Post Processing
	//@{
	virtual bool beginProcess();

	virtual bool endProcess();
	//@}


	/// @name Shape
    //@{	
	virtual bool process( Shape * shape );

	//@}

	/// @name Material
    //@{
	virtual bool process( Material * material );

	virtual bool process( Texture2D * texture );

	virtual bool process( ImageTexture * texture );

	virtual bool process( Texture2DTransformation * texturetransfo );

	//@}
	
	/// @name Geom3D
    //@{
	// virtual bool process( AmapSymbol * amapSymbol );

	virtual bool process( AsymmetricHull * asymmetricHull );

	virtual bool process( AxisRotated * axisRotated );

	virtual bool process( BezierCurve * bezierCurve );

	virtual bool process( BezierPatch * bezierPatch );  

	virtual bool process( Box * box );

	virtual bool process( Cone * cone );

	virtual bool process( Cylinder * cylinder );

	virtual bool process( ElevationGrid * elevationGrid );  //

	virtual bool process( EulerRotated * eulerRotated );

	virtual bool process( ExtrudedHull * extrudedHull );

	virtual bool process( Group * group );  //

	virtual bool process( NurbsCurve * nurbsCurve );

	virtual bool process( NurbsPatch * nurbsPatch );

	virtual bool process( PointSet * pointSet );

	virtual bool process( Polyline * polyline );

	virtual bool process( QuadSet * quadSet );

	virtual bool process( Sphere * sphere );

	virtual bool process( Scaled * scaled );

	virtual bool process( Swung * swung );

	virtual bool process( Translated * translated );

	virtual bool process( TriangleSet * triangleSet );

	 //@}

	/// @name Geom2D
    //@{
    virtual bool process( BezierCurve2D * bezierCurve );

	virtual bool process( Disc * disc );

	virtual bool process( NurbsCurve2D * nurbsCurve );

	virtual bool process( PointSet2D * pointSet );

	virtual bool process( Polyline2D * polyline );


	//@}

	inline void setIndentation(const std::string& tab) { __indentation = tab; }
	inline const std::string& getIndentation() const { return __indentation; }

	inline void setIndentationIncrement(const std::string& tab) { __indentation_increment = tab; }
	inline const std::string& getIndentationIncrement() const { return __indentation_increment; }

	inline void incrementIndentation() { __indentation += __indentation_increment; }
	inline void decrementIndentation() { __indentation.erase(__indentation.end() - __indentation_increment.size(),__indentation.end());; }

	inline void setPglNamespace(const std::string& pglnamespace ) { __pglnamespace  = pglnamespace ; }
	inline const std::string& getPglNamespace() const { return __pglnamespace ; }

	inline void setReferenceDir(const std::string& dir) { __referencedir  = dir ; }
	inline const std::string& getReferenceDir() const { return __referencedir ; }

	inline void setLineBetweenObject(size_t nbline) { __line_between_object  = nbline ; }
	inline const size_t& getLineBetweenObject() const { return __line_between_object ; }

protected:

	void print_constructor_begin(std::ostream& os, const std::string& name, const std::string& type, bool newline = true);
	void print_constructor_end(std::ostream& os, SceneObjectPtr obj, const std::string& name, bool newline = true);
	void print_object_end(std::ostream& os);

	template <typename T>
	std::ostream& print_field(std::ostream& os, const std::string& name, const std::string& field, const T& value, bool newline = true);

	template <typename T>
	std::ostream& print_arg_field(std::ostream& os, const std::string& field, const T& value, bool newline = true);

	template <typename T>
	std::ostream& print_unamed_arg_field(std::ostream& os, const T& value, bool newline = true);

	template <typename T>
	std::ostream& print_field(std::ostream& os, const std::string& name, const std::string& field, const T& value, bool in_constructor, bool newline);

	inline std::string pgltype(const std::string& pgltypename) const;
	std::string getfilename(const std::string& filename) const;


	std::string scene_name;

	std::string __indentation;
	std::string __indentation_increment;
	std::string __pglnamespace;
	std::string __referencedir;
	size_t __line_between_object;
};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
// __actn_pyprinter_h__
#endif

