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

	virtual bool process( ImageTexture * texture ); //

	//@}
	
	/// @name Geom3D
    //@{
	//virtual bool process( AmapSymbol * amapSymbol );
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

protected:

	std::string scene_name;

};

/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
// __actn_pyprinter_h__
#endif

