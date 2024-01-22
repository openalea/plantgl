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

/*! \file view_fog.h
    \brief Definition of the viewer class ViewFogGL.
*/

#ifndef __view_fog_h__
#define __view_fog_h__

#include <QtCore/qstring.h>
#include <QtGui/qcolor.h>
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
  ViewFogGL(ViewCameraGL *camera,QOpenGLBaseWidget * parent=0, const char * name=0, PGLOpenGLFunctionsPtr ogl = NULL);

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

