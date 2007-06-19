/*  -*-c++-*-----------------------------------------------------------------
 *  AMAP                            CIRAD/INRA - UMR Modelisation des Plantes
 *
 *  $Source: /usr/cvsmaster/AMAPmod/src/GEOM/actn_qgeomlistview.h,v $
 *  $Id$
 *
 *  -------------------------------------------------------------------------
 *
 *  Note:
 *
 *  -------------------------------------------------------------------------
 *
 *
 *
 *  -------------------------------------------------------------------------
 */

/*! \file actn_qgeomlistview.h
    \brief Definition of the action class GeomListViewBuilder3.
*/


#ifndef __actn_geomlistviewbuilder_h__
#define __actn_geomlistviewbuilder_h__


#include "../gui3_config.h"
#include <plantgl/scenegraph/core/action.h>
#include <plantgl/scenegraph/core/sceneobject.h>
#include <plantgl/scenegraph/container/indexarray.h>

#include <qpixmap.h>
#include <qptrstack.h>
#include <qpair.h>

/* ----------------------------------------------------------------------- */


class QWidget;
class QListView;
class QListViewItem;
class QString;

/* ----------------------------------------------------------------------- */
TOOLS_BEGIN_NAMESPACE

class Vector2;
class Vector3;
class Vector4;

TOOLS_END_NAMESPACE

PGL_BEGIN_NAMESPACE

class Transform4;
typedef RCPtr<Transform4> Transform4Ptr;
class Color3;
class Color4;

/* ----------------------------------------------------------------------- */

/**
   \class GeomListViewBuilder3
   \brief An action which create a QListView with  objects of the scene.
*/


class VIEW3_API GeomListViewBuilder3 : public Action
{

  public:

  /** Constructs a GeomListViewBuilder3 with the parent widget \e parent and the name \e name */
  GeomListViewBuilder3( QWidget * parent = 0 , char * name = 0 );

  /** Constructs a GeomListViewBuilder3 with the parent widget \e parent and the name \e name */
  GeomListViewBuilder3( QListView * list );

  /// Destructor
  virtual ~GeomListViewBuilder3( );

  /// Clears \e self.
  void clear( );

  /// Add A Node To the QListView.
  void addNode(const QString& name, const QString& type = QString::null, const QString& id = QString::null);

  /// Returns the resulting QListView when applying \e self for the last time.
  QListView * getQListView();

  void setFullMode(bool b){
	__fullmode = b;
  }
  
  /// @name Shape
  //@{
 virtual bool process(Shape * geomShape);

  //@}

  /// @name Material
  //@{

  virtual bool process( Material * material );

  virtual bool process( MonoSpectral * monoSpectral );

  virtual bool process( MultiSpectral * multiSpectral );

  virtual bool process( ImageTexture * texture );

  //@}

  /// @name Geom3D
  //@{


  virtual bool process( AmapSymbol * amapSymbol );

  virtual bool process( AsymmetricHull * asymmetricHull );

  virtual bool process( AxisRotated * axisRotated );

  virtual bool process( BezierCurve * bezierCurve );

  virtual bool process( BezierPatch * bezierPatch );

  virtual bool process( Box * box );

  virtual bool process( Cone * cone );

  virtual bool process( Cylinder * cylinder );

  virtual bool process( ElevationGrid * elevationGrid );

  virtual bool process( EulerRotated * eulerRotated );

  virtual bool process( ExtrudedHull * extrudedHull );

  virtual bool process( FaceSet * faceSet );

  virtual bool process( Frustum * frustum );

  virtual bool process( Extrusion * extrusion );

  virtual bool process( Group * group );

  virtual bool process( IFS * ifs );

  virtual bool process( NurbsCurve * nurbsCurve );

  virtual bool process( NurbsPatch * nurbsPatch );

  virtual bool process( Oriented * oriented );

  virtual bool process( Paraboloid * paraboloid );

  virtual bool process( PointSet * pointSet );

  virtual bool process( Polyline * polyline );

  virtual bool process( QuadSet * quadSet );

  virtual bool process( Revolution * revolution );

  virtual bool process( Swung * swung );

  virtual bool process( Scaled * scaled );

  virtual bool process( Sphere * sphere );

  virtual bool process( Tapered * tapered );

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

  virtual bool process( Text * text );

  virtual bool process( Font * font );

  //@}

  private:

  /// Initialize the QListView.
  void init();

  protected:

  void addNode(const SceneObjectPtr& node, const QString& type, int pixmaptouse = 0);
  void endNode();

  void addAttr(const QString& name, bool value);
  void addAttr(const QString& name, real_t value);
  void addAttrAngle(const QString& name, real_t value);
  void addAttr(const QString& name, uint32_t value);
  void addAttr(const QString& name, uint16_t value);
  void addAttr(const QString& name, uchar_t value);
  void addAttr(const QString& name, const TOOLS(Vector2)& value);
  void addAttr(const QString& name, const TOOLS(Vector3)& value);
  void addAttr(const QString& name, const TOOLS(Vector4)& value);
  void addAttr(const QString& name, const Color3& value);
  void addAttr(const QString& name, const Color4& value);
  void addAttr(const QString& name, const Index& value);
  void addAttr(const QString& name, const Index3& value);
  void addAttr(const QString& name, const Index4& value);
  void addAttr(const QString& name, const QString& value, const QString& type );
  void addAttr(const QString& name, const Transform4Ptr& value);
  void addAttrNode(const QString& name,const SceneObjectPtr& object,const QString& type );
  void addAttrPtr(const QString& name, const QString& value,const QString& type );
  void addArrayAngle(const QString& name, const TOOLS(RealArrayPtr)& _array);
  template <class T>
  void addArray(const QString& name, const T& _array, const QString& type);
  template <class T>
  void addArrayNode(const QString& name, const T& _array, const QString& type);
  template <class T>
  void addMatrix(const QString& name, const T& _matrix, const QString& type);

  void pushItems();
  void popItems();

  protected:

  /// Full mode flag
  bool __fullmode;

  /// The QListView to Create
  QListView * __qListView;

  /// The QListViewItem at the root of the scene tree.
  QListViewItem * __rootItem;

  /// The current QListViewItem node in the Schape List for the build of the list.
  QListViewItem * __currentNodeItem;
  QListViewItem * __currentSiblingItem;

  /// The current Attribute QListViewItem in the build of the list.
  QListViewItem * __currentAttrItem;

  QStack<QPair<QListViewItem *,QListViewItem *> > __stackItem;

  QPixmap __pixgeom;
  QPixmap __pixappe;
  QPixmap __pixshape;
  QPixmap __pixtransf;
  QPixmap __pixatt;
  QPixmap __pixattptr;

};


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
// __actn_geomlistviewbuilder_h__
#endif


