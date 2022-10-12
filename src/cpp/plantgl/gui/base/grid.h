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




/*! \file view_grid.h
    \brief Definition of the viewer class ViewGridGL.
*/


#ifndef __view_grid_h__
#define __view_grid_h__

/* ----------------------------------------------------------------------- */

#include <QtCore/qstring.h>
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qmenu.h>
#else
    #include <QtGui/qmenu.h>
#endif
#include "object.h"
#include <plantgl/algo/opengl/util_gl.h>

class ViewEvent;

/* ----------------------------------------------------------------------- */

/**
   \class ViewGridGL
   \brief A ViewGridGL for GL Display

*/

/* ----------------------------------------------------------------------- */


class VIEW_API ViewGridGL  : public ViewRelativeObjectGL
{
  Q_OBJECT
  Q_PROPERTY(bool Axis READ getAxis WRITE showAxis );
  Q_PROPERTY(bool XYGrid READ getXYGrid WRITE showXYGrid );
  Q_PROPERTY(bool XZGrid READ getXZGrid WRITE showXZGrid );
  Q_PROPERTY(bool YZGrid READ getYZGrid WRITE showYZGrid );
  Q_PROPERTY(double Unit READ getGridUnit WRITE setGridUnit );
  Q_PROPERTY(int Size READ getGridSize WRITE setGridSize );

public:

  /// Constructor.
  ViewGridGL(ViewCameraGL *camera,QOpenGLWidget * parent=0, const char * name=0);

  /// Destructor.
  virtual ~ViewGridGL();

  /// Get Axis Display.
  const bool getAxis() const{
    return __Axis;
  }

  /// Get XY Plane Grid Display.
  const bool getXYGrid() const{
    return __XYGrid;
  }

  /// Get XZ Plane Grid Display.
  const bool getXZGrid() const{
    return __XZGrid;
  }

  /// Get YZ Plane Grid Display.
  const bool getYZGrid() const{
    return __YZGrid;
  }

  int getState() const;
  void setState(int);

  /// Get Grid Unit.
  const double getGridUnit() const{
    return __gridUnit;
  }

  /// Get Grid Size.
  const int getGridSize() const{
    return __gridSize;
  }

  virtual void geomCoordSys();

  virtual void glCoordSys();

  /// Create a Popup menu that reflect the functionality of this.
  virtual QMenu * createToolsMenu(QWidget * parent);

  /// Fill a toolBar that reflect the functionality of this.
  virtual void fillToolBar(QToolBar * toolBar);

  virtual void gridEvent(ViewEvent *);

  /// end event.
  virtual void endEvent();

public slots:

  /// change the Axis Display Mode
  void changeAxisDisplayMode();
  /// show Axis
  void showAxis(bool);

  /// change the XY Plane Grid Display Mode
  void changeXYGridDisplayMode();
  /// show the XY Plane Grid
  void showXYGrid(bool);

  /// change the XZ Plane Grid Display Mode
  void changeXZGridDisplayMode();
  /// show the XZ Plane Grid
  void showXZGrid(bool);

  /// change the YZ Plane Grid Display Mode
  void changeYZGridDisplayMode();
  /// show the YZ Plane Grid
  void showYZGrid(bool);

  /// Set Grid Unit value to \e unit.
  void setGridUnit(double unit);

  /// Set Grid Unit value to \e unit.
  void setGridUnit(const QString& unit);

  /// Set Grid Size value to \e size.
  void setGridSize(int unit);

  /// Set Grid Size value to \e size.
  void setGridSize(const QString& size);

  /// Initialize the grid
  virtual void initializeGL();
  /// GL command for Grid.
  virtual void paintGL();

signals:

  /// Emit when the Axis Display changed
  void AxisDisplayChanged(bool);

  /// Emit when the XY Grid Display changed
  void XYGridDisplayChanged(bool);

  /// Emit when the XZ Grid Display changed
  void XZGridDisplayChanged(bool);

  /// Emit when the YZ Grid Display changed
  void YZGridDisplayChanged(bool);

  /// Emit when the Grid Unit changed
  void GridUnitChanged(double);

  /// Emit when the Grid Size changed
  void GridSizeChanged(int);

protected :

  virtual void changeStepEvent(double newStep, double oldStep);

  ///  unit of the Grid (multiplicative factor for zoom and translation)
  double __gridUnit;

  ///  size of the Grid (multiplicative factor for zoom and translation)
  int __gridSize;

  /// The id of the Display List of the Grid.
  GLuint __gridList;

  /// The id of the Display List of the .
  GLuint __axisList;

  /// Axis display
  bool __Axis;

  /// XYGrid display
  bool __XYGrid;

  /// XZGrid display
  bool __XZGrid;

  /// YZGrid display
  bool __YZGrid;

};

/* ----------------------------------------------------------------------- */
#endif

