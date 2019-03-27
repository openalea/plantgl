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


/*! \file view_clippingplane.h
    \brief Definition of the viewer class ViewClippingPlaneGL.
*/

#ifndef __view_clippingplane_h__
#define __view_clippingplane_h__

/* ----------------------------------------------------------------------- */

#include <QtCore/qstring.h>
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qmenu.h>
#else
    #include <QtGui/qmenu.h>
#endif
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

