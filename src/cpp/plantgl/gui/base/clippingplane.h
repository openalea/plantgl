/*
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
 *
 *  ----------------------------------------------------------------------------
 * 
 *                      GNU General Public Licence
 *           
 *       This program is free software; you can redistribute it and/or
 *       modify it under the terms of the GNU General Public License as
 *       published by the Free Software Foundation; either version 2 of
 *       the License, or (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public
 *       License along with this program; see the file COPYING. If not,
 *       write to the Free Software Foundation, Inc., 59
 *       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  ----------------------------------------------------------------------------
 */				


/*! \file view_clippingplane.h
    \brief Definition of the viewer class ViewClippingPlaneGL.
*/

#ifndef __view_clippingplane_h__
#define __view_clippingplane_h__

/* ----------------------------------------------------------------------- */

#include <qstring.h>
#include <qmenu.h>   
#include "object.h"
class ViewDialog;
class ViewEvent;

/* ----------------------------------------------------------------------- */

/**   
   \class ViewClippingPlaneGL
   \brief Some Clipping Planes for GL Display

*/

/* ----------------------------------------------------------------------- */

class VIEW_API ViewClippingPlaneGL  : public ViewObjectGL
{
  Q_OBJECT

public:

  /// Constructor.
  ViewClippingPlaneGL(QGLWidget * parent=0, const char * name=0);

  /// Destructor.
  virtual ~ViewClippingPlaneGL();
  
  virtual QMenu * createToolsMenu(QWidget * parent);

  bool isPlaneEnable(int);
  double valA(int);
  double valB(int);
  double valC(int);
  double valD(int);

  ViewDialog * getControl() const;

  /// Initialize
  virtual void initializeGL();

  /// GL command.
  virtual void paintGL(); 

  virtual void clippingPlaneEvent(ViewEvent * e);

public slots:

  void setPlane1Enable(bool);
  void setPlane2Enable(bool);
  void setPlane3Enable(bool);
  void setPlane4Enable(bool);
  void setPlane5Enable(bool);
  void setPlane6Enable(bool);

  void setPlane1Enable();
  void setPlane2Enable();
  void setPlane3Enable();
  void setPlane4Enable();
  void setPlane5Enable();
  void setPlane6Enable();

  void setPlane1A(double);
  void setPlane2A(double);
  void setPlane3A(double);
  void setPlane4A(double);
  void setPlane5A(double);
  void setPlane6A(double);

  void setPlane1B(double);
  void setPlane2B(double);
  void setPlane3B(double);
  void setPlane4B(double);
  void setPlane5B(double);
  void setPlane6B(double);

  void setPlane1C(double);
  void setPlane2C(double);
  void setPlane3C(double);
  void setPlane4C(double);
  void setPlane5C(double);
  void setPlane6C(double);

  void setPlane1D(double);
  void setPlane2D(double);
  void setPlane3D(double);
  void setPlane4D(double);
  void setPlane5D(double);
  void setPlane6D(double);


signals:

  void plane1Enabled(bool);
  void plane2Enabled(bool);
  void plane3Enabled(bool);
  void plane4Enabled(bool);
  void plane5Enabled(bool);
  void plane6Enabled(bool);

protected :

  bool __enable[6];

  double __A[6];

  double __B[6];

  double __C[6];

  double __D[6];

  ViewDialog * __control;
};

/* ----------------------------------------------------------------------- */
#endif

