/* -*-c++-*-
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

/*! \file view_rotcenter.h
    \brief Definition of the viewer class ViewRotCenterGL.
*/

#ifndef __view_rotcenter_h__
#define __view_rotcenter_h__

/* ----------------------------------------------------------------------- */

#include "Tools/util_gl.h"
#include <qpopupmenu.h>

#include "view_object.h"
#include "Tools/util_vector.h"

/* ----------------------------------------------------------------------- */

class RotCenterEdit;

/* ----------------------------------------------------------------------- */

class ViewDialog;

/* ----------------------------------------------------------------------- */

/**   
   \class ViewRotCenterGL
   \brief A ViewRotCenterGL for GL Display

*/

/* ----------------------------------------------------------------------- */

class GEOM_API ViewRotCenterGL  : public ViewRelativeObjectGL
{
  Q_OBJECT
  Q_PROPERTY(int X READ x WRITE setX );
  Q_PROPERTY(int Y READ y WRITE setY );
  Q_PROPERTY(int Z READ z WRITE setZ );
  Q_PROPERTY(bool Visible READ isVisible WRITE changeVisibility );
  Q_PROPERTY(bool Active READ isActive WRITE changeActivation );
public:

  /// Constructor.
  ViewRotCenterGL(ViewCameraGL *camera,
		  QGLWidget * parent=0, 
		  const char * name=0);

  /// Destructor.
  virtual ~ViewRotCenterGL();

  /// Return whether self is visible.
  bool isVisible() const ;

  /// Return whether self is active.
  bool isActive() const;
  
  /// x coordinates
  int x() const;

  /// y coordinates
  int y() const;

  /// z coordinates
  int z() const;

  /// get the sliders that control this.
  ViewDialog * getSliders() const;

  /// Create a Tools menu that reflect the functionality of this.
  virtual QPopupMenu * createToolsMenu(QWidget * parent);

  /// Fill toolBar to reflect the functionality of this.
  void fillToolBar(QToolBar * toolBar);

public slots:

  /// reinitialize value.
  void init();

  /// show 
  void show();

  /// hide
  void hide();
  
  /// Change Visibility
  void changeVisibility();

  /// Change Visibility
  void changeVisibility(bool);

  /// activate.
  void activate();

  /// center.
  void center();

  /// desactivate.
  void desactivate();

  /// Change Activation
  void changeActivation();

  /// Change Activation
  void changeActivation(bool);

  /// Set X Coordinates of this to \e x
  void setX(int x);

  /// Set Y Coordinates of this to \e y
  void setY(int y);

  /// Set Z Coordinates of this to \e z
  void setZ(int z);

  /// Set X Coordinates of this to \e x
  void setX(const QString& x);

  /// Set Y Coordinates of this to \e y
  void setY(const QString& y);

  /// Set Z Coordinates of this to \e z
  void setZ(const QString& z);

  /// Initialize the rotating center.
  virtual void initializeGL();

  /// GL command for rotating center.
  virtual void paintGL(); 

  /// Change relative value.
  virtual void changeStepEvent(const int newStep, const int oldStep);

  signals:

  /// emit when visibility changed.
  void visibilityChanged(bool);
  
  /// emit when activation changed
  void activationChanged(bool);

  /// emit when X Coordinates changed.
  void XvalueChanged(int);
  
  /// emit when Y Coordinates changed.
  void YvalueChanged(int);
  
  /// emit when Z Coordinates changed.
  void ZvalueChanged(int);
  
  /// emit when X Coordinates changed.
  void XvalueChanged(const QString&);
  
  /// emit when Y Coordinates changed.
  void YvalueChanged(const QString&);
  
  /// emit when Z Coordinates changed.
  void ZvalueChanged(const QString&);
  
protected :

  void setSliderStep(const int step);

  /// Sliders to control self coordinates.
  ViewDialog * __sliders;
  RotCenterEdit * __editor;

  /// Activation of this.
  bool __active;

  /// Visibility if this.
  bool __visible;

  /// Coordinates.
  TOOLS(Vector3) __position;

  /// display list.
  GLuint __displayList;

};

/* ----------------------------------------------------------------------- */

/**   
   \class ViewRotCenterMenu
   \brief A Menu to control a RotCenterGL

*/
class GEOM_API ViewRotCenterMenu : public QPopupMenu {
   Q_OBJECT
 
public :
  
   /// Constructor.
   ViewRotCenterMenu(ViewRotCenterGL * center, 
		      QWidget * parent=0, 
		      const char * name=0);
  
  /// Destructor.
  ~ViewRotCenterMenu();
  
public slots :

  /// Check the menu entry Visible.
  void setVisible(bool);

  /// Check the menu entry Active.
  void setActive(bool);

  /// Check the menu entry Control.
  void setControl(bool);

  
protected :    
  
  int idVisible;
  int idActive;
  int idControl;
};


/* ----------------------------------------------------------------------- */
#endif

