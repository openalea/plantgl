/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s) : F. Boudon (frederic.boudon@cirad.fr)
 *
 *       Form interface generated from reading ui file 'materialeditor.ui'
 *
 *       Created: Mon Sep 9 15:27:59 2002
 *        by:  The User Interface Compiler (uic)
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

 /*! \file view_materialeditor.h
    \brief Geometry Edition module : MaterialEditor.
*/


#ifndef MATERIALEDITOR_H
#define MATERIALEDITOR_H

#include <scenegraph/appearance/material.h>
#include <algo/base/discretizer.h>
#include <algo/opengl/glrenderer.h>
#include "moduleeditor.h"

#include <qvariant.h>
#include <qwidget.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qgl.h>
#include <GL/glu.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;
class QLineEdit;

/*----------------------------------------------------------------------------*/

class VIEW_API ViewColorGL : public QGLWidget
{ 
    Q_OBJECT

public:

  ViewColorGL( QWidget * parent=0, const char * name=0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f=0 );
  virtual ~ViewColorGL();
  void setAppearance(const PGL(AppearancePtr)& mat);
  
  
protected:

  virtual void initializeGL();
  virtual void resizeGL(int,int);
  virtual void paintGL();
  virtual void paintAppearance();

  GLUquadricObj* __sphereobject;
  GLuint __spheredrawlist;
  GLfloat __smoothness; 
  GLfloat __range;

  GLfloat __Near;
  GLfloat __Far;
  
  GLfloat __radius; 
  GLfloat __lightpos[4];
  GLfloat __lightmodel[4];
  GLfloat __xmin,__xmax,__ymin,__ymax;
  PGL(AppearancePtr) __M;
  PGL(Discretizer) __d;
  PGL(GLRenderer) __r;
};

/*----------------------------------------------------------------------------*/

class VIEW_API ColorButton : public QPushButton
{ 
    Q_OBJECT

public:
    ColorButton ( QWidget * parent, const char * name = 0 );
    ~ColorButton();
    const QColor& getColor() const;

public slots :
    void setColor(const QColor&);
  

 protected:
  QColor __color;
};

class VIEW_API ColorEditButton : public ColorButton
{ 
    Q_OBJECT

public:
    ColorEditButton ( QWidget * parent, const char * name = 0 );
    ~ColorEditButton();

public slots :
  void selectColor();

signals:
  void colorChanged(const QColor&);
  
};

/*----------------------------------------------------------------------------*/

class VIEW_API ColorEditSlider : public QSlider
{ 
    Q_OBJECT

public:
    ColorEditSlider ( QWidget * parent, const char * name = 0 );
  ~ColorEditSlider();
  
  QColor getColor() const;
  
public slots :
  void setColor(const QColor&);
  
protected slots:
  void valueChangedEvent(int);

signals:
  void colorChanged();
  
protected:
  QColor __color;
  real_t __ratio; 
};

/*----------------------------------------------------------------------------*/

class VIEW_API MaterialEditor : public GeomModuleEditor
{ 
    Q_OBJECT

public:
    MaterialEditor( QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = 0 );
    ~MaterialEditor();

  /// Add a menu to the main menu bar when show.
  virtual void addMenu(QMenuBar * menubar) ;

  /// remove menu to the main menu bar when hide.
  virtual void removeMenu(QMenuBar * menubar) ;

  /// Get the name of the class of objets that the module manage. 
  virtual QString geomClassName() const ;

  /// Get Optimal size of display.
  virtual const QSize getSize() const ;

  /* 
     Must return true if \e this can manage the edition, else false.
     Must recopy the object.
  */
  virtual bool setGeomObject(PGL(SceneObjectPtr) obj);

  /// Edit a new object.
  virtual void newObj();

  /// Ask object before saving.
  virtual PGL(SceneObjectPtr) getGeomObject();

    /// Get the Material
    const PGL(MaterialPtr) & getMaterial() const{
      return __material;
    }
    
    /// Set the Material
    void setMaterial(const PGL(MaterialPtr)& );
      
    QLabel* LabelAmbient;
    QLabel* LabelDiffuse;
    QLabel* LabelSpecular;
    QLabel* LabelEmission;
    QLabel* LabelShininess;
    QLabel* LabelTransparency;
    QLabel* LabelName;
    QLineEdit* EditName;
    ColorEditButton* ButtonAmbient;
    ColorButton* ButtonDiffuse;
    ColorEditButton* ButtonSpecular;
    ColorEditButton* ButtonEmission;

    ColorEditSlider* SliderAmbient;
    ColorEditSlider* SliderSpecular;
    ColorEditSlider* SliderEmission;
    QSlider* SliderDiffuse;
    QSlider* SliderShininess;
    QSlider* SliderTransparency;

    ViewColorGL* FrameGL;

public slots:
	virtual void clear();

 protected slots:
 
  void setAmbient(const QColor&);
  void setSpecular(const QColor&);
  void setEmission(const QColor&);

  void ambientChangedEvent();
  void specularChangedEvent();
  void emissionChangedEvent();

  void setDiffuse(int);
  void setShininess(int);
  void setTransparency(int);
  void setGeomName(const QString&);
  void update();

protected:
  bool event( QEvent* );
  
  PGL(MaterialPtr) __material;
  int __diffuse;
};

/*----------------------------------------------------------------------------*/

#endif // MATERIALEDITOR_H
