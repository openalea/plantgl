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


/*! \file actn_qgeomlistview.h
    \brief Definition of the action class GeomListViewBuilder.
*/


#ifndef __actn_geomlistviewbuilder_h__
#define __actn_geomlistviewbuilder_h__


#include "../gui_config.h"
#include <plantgl/scenegraph/core/action.h>
#include <plantgl/scenegraph/core/sceneobject.h>
#include <plantgl/scenegraph/container/indexarray.h>

#include <QtGui/qpixmap.h>
#include <QtCore/qstack.h>
#include <QtCore/qpair.h>

/* ----------------------------------------------------------------------- */


class QWidget;
class QTreeWidgetItem;
class QTreeWidget;
class QString;

/* ----------------------------------------------------------------------- */
PGL_BEGIN_NAMESPACE

class Vector2;
class Vector3;
class Vector4;

PGL_END_NAMESPACE

PGL_BEGIN_NAMESPACE

class Transform4;
typedef RCPtr<Transform4> Transform4Ptr;
class Color3;
class Color4;

/* ----------------------------------------------------------------------- */

/**
   \class GeomListViewBuilder
   \brief An action which create a Q3ListView with  objects of the scene.
*/


class VIEW_API GeomListViewBuilder : public Action
{

  public:

  /** Constructs a GeomListViewBuilder with the parent widget \e parent and the name \e name */
  GeomListViewBuilder( QWidget * parent = 0 , char * name = 0 );

  /** Constructs a GeomListViewBuilder with the parent widget \e parent and the name \e name */
  GeomListViewBuilder( QTreeWidget * list );

  /// Destructor
  virtual ~GeomListViewBuilder( );

  /// Clears \e self.
  void clear( );

  /// Add A Node To the Q3ListView.
  void addNode(const QString& name, const QString& type = QString(), const QString& id = QString());

  /// Returns the resulting Q3ListView when applying \e self for the last time.
  inline QTreeWidget * getListView() { return __qListView; }

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

  virtual bool process( Texture2D * texture );

  virtual bool process( ImageTexture * textureimg );

  virtual bool process( Texture2DTransformation * texturetransformation );
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

  virtual bool process( ScreenProjected * scp );

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

  /// Initialize the Q3ListView.
  void setHeader();
  void init();

  protected:

  void addNode(const SceneObject * node, const QString& type, int pixmaptouse = 0);
  void endNode();

  void addAttr(const QString& name, bool value);
  void addAttr(const QString& name, real_t value);
  void addAttrAngle(const QString& name, real_t value);
  void addAttr(const QString& name, uint_t value);
  void addAttr(const QString& name, uint16_t value);
  void addAttr(const QString& name, uchar_t value);
  void addAttr(const QString& name, const Vector2& value);
  void addAttr(const QString& name, const Vector3& value);
  void addAttr(const QString& name, const Vector4& value);
  void addAttr(const QString& name, const Color3& value);
  void addAttr(const QString& name, const Color4& value);
  void addAttr(const QString& name, const Index& value);
  void addAttr(const QString& name, const Index3& value);
  void addAttr(const QString& name, const Index4& value);
  void addAttr(const QString& name, const QString& value, const QString& type );
  void addAttr(const QString& name, const Transform4Ptr& value);
  void addAttrNode(const QString& name,const SceneObjectPtr& object,const QString& type );
  void addAttrPtr(const QString& name, const QString& value,const QString& type );
  void addArrayAngle(const QString& name, const RealArrayPtr& _array);
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

  /// The Q3ListView to Create
  QTreeWidget * __qListView;

  /// The QTreeWidgetItem at the root of the scene tree.
  QTreeWidgetItem * __rootItem;

  /// The current QTreeWidgetItem node in the Schape List for the build of the list.
  QTreeWidgetItem * __currentNodeItem;
  QTreeWidgetItem * __currentSiblingItem;

  /// The current Attribute QTreeWidgetItem in the build of the list.
  QTreeWidgetItem * __currentAttrItem;

  QStack<QPair<QTreeWidgetItem *,QTreeWidgetItem *> > __stackItem;

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


