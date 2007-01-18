/*
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): F. Boudon
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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
#include <qpopupmenu.h>   
#include "view_object.h"
class ClippingPlaneWidget;
class ViewDialog;

/* ----------------------------------------------------------------------- */

/**   
   \class ViewClippingPlaneGL
   \brief Some Clipping Planes for GL Display

*/

/* ----------------------------------------------------------------------- */

class GEOM_API ViewClippingPlaneGL  : public ViewObjectGL
{
  Q_OBJECT

public:

  /// Constructor.
  ViewClippingPlaneGL(QGLWidget * parent=0, const char * name=0);

  /// Destructor.
  virtual ~ViewClippingPlaneGL();
  
  virtual QPopupMenu * createToolsMenu(QWidget * parent);

  bool isPlaneEnable(int);
  double valA(int);
  double valB(int);
  double valC(int);
  double valD(int);

  ViewDialog * getControl() const;

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

  void setPlane1A(const QString&);
  void setPlane2A(const QString&);
  void setPlane3A(const QString&);
  void setPlane4A(const QString&);
  void setPlane5A(const QString&);
  void setPlane6A(const QString&);

  void setPlane1B(const QString&);
  void setPlane2B(const QString&);
  void setPlane3B(const QString&);
  void setPlane4B(const QString&);
  void setPlane5B(const QString&);
  void setPlane6B(const QString&);

  void setPlane1C(const QString&);
  void setPlane2C(const QString&);
  void setPlane3C(const QString&);
  void setPlane4C(const QString&);
  void setPlane5C(const QString&);
  void setPlane6C(const QString&);

  void setPlane1D(const QString&);
  void setPlane2D(const QString&);
  void setPlane3D(const QString&);
  void setPlane4D(const QString&);
  void setPlane5D(const QString&);
  void setPlane6D(const QString&);

  void validValue();

  /// Initialize
  virtual void initializeGL();

  /// GL command.
  virtual void paintGL(); 

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
  ClippingPlaneWidget * __cpw;
};

/* ----------------------------------------------------------------------- */

/// Menu to control a ViewClippingPlaneGL
class GEOM_API ViewCPlaneMenu : public QPopupMenu {
   Q_OBJECT
 
public :
  
  ViewCPlaneMenu(ViewClippingPlaneGL * cp, QWidget * parent=0, const char * name=0);
  
  ~ViewCPlaneMenu();
  
public slots :

  void setPlane1Enable(bool);
  void setPlane2Enable(bool);
  void setPlane3Enable(bool);
  void setPlane4Enable(bool);
  void setPlane5Enable(bool);
  void setPlane6Enable(bool);
  void controlVisibility(bool);
  
protected :    
  
  int idP1;
  int idP2;
  int idP3;
  int idP4;
  int idP5;
  int idP6;
  int ctrl;
};


/* ----------------------------------------------------------------------- */
#endif

