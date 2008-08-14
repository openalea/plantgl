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

/*! \file view_fog.h
    \brief Definition of the viewer class ViewFogGL.
*/

#ifndef __view_fog_h__
#define __view_fog_h__

#include <qstring.h>
#include <qcolor.h>
#include "object.h"
#include <plantgl/tool/util_types.h>

/* ----------------------------------------------------------------------- */

namespace Ui { class FogWidget; }

/* ----------------------------------------------------------------------- */

class ViewDialog;

/* ----------------------------------------------------------------------- */

/**   
   \class ViewFogGL
   \brief A ViewFogGL for GL Display

*/

/* ----------------------------------------------------------------------- */

class VIEW_API ViewFogGL  : public ViewRelativeObjectGL
{
  Q_OBJECT
  Q_PROPERTY(bool Enable READ enable WRITE setEnable );
  Q_PROPERTY(int Mode READ getMode WRITE setMode );
  Q_PROPERTY(int HintMode READ getHintMode WRITE setHints );
  Q_PROPERTY(double Density READ getDensity WRITE setDensity );
  Q_PROPERTY(double Start READ getStart WRITE setStart );
  Q_PROPERTY(double End READ getEnd WRITE setEnd );

public:

  /// Constructor.
  ViewFogGL(ViewCameraGL *camera,QGLWidget * parent=0, const char * name=0);

  /// Destructor.
  virtual ~ViewFogGL();
  
  virtual QMenu * createToolsMenu(QWidget * parent);

  bool enable() const;

  int getMode() const;

  int getHintMode() const;

  const QColor& getColor() const;

  double getDensity() const;

  double getStart() const;

  double getEnd() const;

public slots:

  void setEnable(bool);
 
  void setEnable();

  void setMode(int);

  void setHints(int);

  void changeColor();

  void setColor(const QColor&);

  void setDensity(int);
  void setDensity(const double&);

  void setStart(const QString&);
  void setStart(const double&);

  void setEnd(const QString&);
  void setEnd(const double&);

  void validValue();

  /// Initialize the grid
  virtual void initializeGL();
  /// GL command for Grid.
  virtual void paintGL(); 
  
  virtual void changeStepEvent(double newStep,double oldStep);

 signals:

  void enabled(bool);

  void colorChanged(const QColor&);

 protected:

  bool __enable;

  int __mode;

  int __hintmode;

  QColor __color;

  double __density;

  double __start;

  double __end;

  ViewDialog * __control;
  Ui::FogWidget * __fogw;
};

/* ----------------------------------------------------------------------- */
#endif

