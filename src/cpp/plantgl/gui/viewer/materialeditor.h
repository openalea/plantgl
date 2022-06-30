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


 /*! \file view_materialeditor.h
    \brief Geometry Edition module : MaterialEditor.
*/


#ifndef MATERIALEDITOR_H
#define MATERIALEDITOR_H

#include <plantgl/scenegraph/appearance/material.h>
#include <plantgl/algo/base/discretizer.h>
#include <plantgl/algo/opengl/glrenderer.h>
#include "moduleeditor.h"

#include <QtCore/qvariant.h>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qwidget.h>
    #include <QtWidgets/qpushbutton.h>
    #include <QtWidgets/qslider.h>
#else
    #include <QtGui/qwidget.h>
    #include <QtGui/qpushbutton.h>
    #include <QtGui/qslider.h>
#endif
#include <QtOpenGL/qgl.h>
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

  ViewColorGL( QWidget * parent=0, const char * name=0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f=Qt::WindowFlags() );
  virtual ~ViewColorGL();
  void setAppearance(const PGL(AppearancePtr)& mat);


protected:

  virtual void initializeGL();
  virtual void resizeGL(int,int);
  virtual void paintGL();
  virtual void paintAppearance();

  GLuint __spheredrawlist;
  GLfloat __smoothness;
  GLfloat __range;

  GLfloat __Near;
  GLfloat __Far;

  GLfloat __radius;
  GLfloat __lightpos[4];
  GLfloat __lightmodel[4];
  GLfloat __xmin,__xmax,__ymin,__ymax;

  PGL(GeometryPtr) __sphereobject;
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
    MaterialEditor( QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WindowFlags() );
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
