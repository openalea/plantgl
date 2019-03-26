/* -*-c++-*-
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


#include "materialeditor.h"
#include "../base/util_qwidget.h"

#include <QtGlobal>
#include <QtCore/qvariant.h>

#include <QtGui/qfont.h>
#include <QtGui/qevent.h>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qmenu.h>
    #include <QtWidgets/qlabel.h>
    #include <QtWidgets/qlineedit.h>
    #include <QtWidgets/qslider.h>
    #include <QtWidgets/qtoolbutton.h>
    #include <QtWidgets/qlayout.h>
    #include <QtWidgets/qtooltip.h>
    #include <QtWidgets/qwhatsthis.h>
    #include <QtWidgets/qcolordialog.h>
#else
    #include <QtGui/qlabel.h>
    #include <QtGui/qlineedit.h>
    #include <QtGui/qslider.h>
    #include <QtGui/qtoolbutton.h>
    #include <QtGui/qlayout.h>
    #include <QtGui/qtooltip.h>
    #include <QtGui/qwhatsthis.h>
    #include <QtGui/qcolordialog.h>
#endif

#include <plantgl/scenegraph/geometry/sphere.h>

#include <plantgl/scenegraph/geometry/sphere.h>

PGL_USING_NAMESPACE
using namespace std;

/*----------------------------------------------------------------------------*/
ViewColorGL::ViewColorGL( QWidget * parent, const char * name, const QGLWidget * shareWidget, Qt::WindowFlags f ):
 QGLWidget(parent,shareWidget,f),
 __sphereobject(NULL),
 __r(__d){
    if(name) setObjectName(name);

  __range=1000;
  __Near=-3000;
  __Far=2000;

  __radius=375;

  __lightpos[0]=1000.0; // x
  __lightpos[1]=1000.0; // y
  __lightpos[2]=2000.0; // z
  __lightpos[3]=0.0; // directional

  __lightmodel[0]=1.0; // white ambient light
  __lightmodel[1]=1.0;
  __lightmodel[2]=1.0;
  __lightmodel[3]=1.0;
}

ViewColorGL::~ViewColorGL(){
}

void ViewColorGL::setAppearance(const AppearancePtr& mat){
    __M = mat;
    updateGL();
  }

// -------------------- Rendering --------------------
void ViewColorGL::initializeGL(){
  glClearColor(0.0,0.0,0.0,1.0);

  glShadeModel(GL_SMOOTH);
  glLightfv(GL_LIGHT0,GL_POSITION,__lightpos);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,__lightmodel);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);

  __sphereobject = GeometryPtr(new Sphere(__radius,80,80));

  __spheredrawlist=glGenLists(1);
  glNewList(__spheredrawlist,GL_COMPILE);
  __sphereobject->apply(__r);
  glEndList();
}

void ViewColorGL::resizeGL(int,int){
  glViewport(0,0,(GLsizei)width(),(GLsizei)height());
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  GLfloat scale;

  if(width()<height()){
    scale=(GLfloat)height()/(GLfloat)width();
    __xmin=-0.5*__range;
    __xmax=0.5*__range;
    __ymin=-0.5*__range*scale;
    __ymax=0.5*__range*scale;
  }
  else if(width()==height()){
    __xmin=-0.5*__range;
    __xmax=0.5*__range;
    __ymin=-0.5*__range;
    __ymax=0.5*__range;
  }
  else{
    scale=(GLfloat)width()/(GLfloat)height();
    __xmin=-0.5*__range*scale;
    __xmax=0.5*__range*scale;
    __ymin=-0.5*__range;
    __ymax=0.5*__range;
  }

  glOrtho(__xmin,__xmax,__ymin,__ymax,__Near,__Far);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void ViewColorGL::paintGL(){

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);

  // traditional background for showing transparency
  glDisable(GL_LIGHTING);
  GLfloat lightcheck[]={0.9f,0.9f,0.9f};
  GLfloat darkcheck[]={0.5f,0.5f,0.5f};

  glBegin(GL_QUADS);{
    glColor3fv(lightcheck);
    glVertex3f(__xmin,__ymax,-500);
    glVertex3f(0,__ymax,-500);
    glVertex3f(0,0,-500);
    glVertex3f(__xmin,0,-500);

    glVertex3f(0,0,-500);
    glVertex3f(__xmax,0,-500);
    glVertex3f(__xmax,__ymin,-500);
    glVertex3f(0,__ymin,-500);

    glColor3fv(darkcheck);
    glVertex3f(0,__ymax,-500);
    glVertex3f(__xmax,__ymax,-500);
    glVertex3f(__xmax,0,-500);
    glVertex3f(0,0,-500);

    glVertex3f(__xmin,0,-500);
    glVertex3f(0,0,-500);
    glVertex3f(0,__ymin,-500);
    glVertex3f(__xmin,__ymin,-500);

  }
  glEnd();

  // draw our sphere
  glPushMatrix();
  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);

  glRotatef(90.0,0.0,1.0,0.0);
  glRotatef(45.0,1.0,0.0,0.0);

  paintAppearance();

  glCallList(__spheredrawlist);

  glPopMatrix();

  glFlush();
}

void ViewColorGL::paintAppearance(){
  if(__M){
    __M->apply(__r);
    __r.endProcess();
  }
}

/*----------------------------------------------------------------------------*/

ColorButton::ColorButton ( QWidget * parent, const char * name ):
  QPushButton(parent),
  __color(0,0,0){
  if(name)setObjectName(name);
  setColor(__color);
}

ColorButton::~ColorButton(){
}

void
ColorButton::setColor(const QColor& c){
    PGL(fillButton)(this,c,QSize(60,40));
    __color = c;
}

const QColor& ColorButton::getColor() const{
  return __color;
}

/*----------------------------------------------------------------------------*/

ColorEditButton::ColorEditButton ( QWidget * parent, const char * name ):
  ColorButton(parent,name){
  QObject::connect(this,SIGNAL(clicked()),this,SLOT(selectColor()));
}

ColorEditButton::~ColorEditButton(){
}

void ColorEditButton::selectColor(){
  QColor col = QColorDialog::getColor(__color,this);
  if(col.isValid()){
    setColor(col);
    emit colorChanged(__color);
  }
}
/*----------------------------------------------------------------------------*/

ColorEditSlider::ColorEditSlider ( QWidget * parent, const char * name  ):
  QSlider(parent){
  if(name)setObjectName(name);
  QObject::connect(this,SIGNAL(valueChanged(int)),this,SLOT(valueChangedEvent(int)));
  setRange( 0, 255 );
}

ColorEditSlider::~ColorEditSlider(){
}

QColor ColorEditSlider::getColor() const{
  return QColor( (int)(__ratio*__color.red()),(int)(__ratio*__color.green()),(int)(__ratio*__color.blue()));
}

void ColorEditSlider::setColor(const QColor& c){
  __color = c;
  int v = max(max(__color.red(),__color.green()),__color.blue());
  if(v == 0){
    __color = QColor(255,255,255);
    __ratio = 0;
  }
  else __ratio = 1;
  setValue(v);
}

void ColorEditSlider::valueChangedEvent(int i){
  int m = max(max(__color.red(),__color.green()),__color.blue());
  real_t r = (real_t)i/(real_t)m;
  if(r != __ratio){
    __ratio = r;
    emit colorChanged();
  }
}

/*----------------------------------------------------------------------------*/

/*
 *  Constructs a MaterialEditor which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */

MaterialEditor::MaterialEditor( QWidget* parent,  const char* name, Qt::WindowFlags fl )
    : GeomModuleEditor( parent,  fl )
{
    if ( !name ) setObjectName( "MaterialEditor" );
    resize( 550, 300 );
    setMinimumSize(550,300);
    QFont f( font() );
    f.setFamily( "adobe-courier" );
    f.setPointSize( 10 );
    setFont( f );
    setWindowTitle( tr( "Material" ) );

    LabelAmbient = new QLabel( this );
    LabelAmbient->setGeometry( QRect( 220, 20, 61, 20 ) );
    QFont LabelAmbient_font(  LabelAmbient->font() );
    LabelAmbient->setFont( LabelAmbient_font );
    LabelAmbient->setText( tr( "Ambient" ) );

    LabelDiffuse = new QLabel( this );
    LabelDiffuse->setGeometry( QRect( 220, 70, 61, 20 ) );
    QFont LabelDiffuse_font(  LabelDiffuse->font() );
    LabelDiffuse->setFont( LabelDiffuse_font );
    LabelDiffuse->setText( tr( "Diffuse" ) );

    LabelSpecular = new QLabel( this );
    LabelSpecular->setGeometry( QRect( 220, 120, 61, 20 ) );
    QFont LabelSpecular_font(  LabelSpecular->font() );
    LabelSpecular->setFont( LabelSpecular_font );
    LabelSpecular->setText( tr( "Specular" ) );

    LabelEmission = new QLabel( this);
    LabelEmission->setGeometry( QRect( 220, 170, 61, 20 ) );
    QFont LabelEmission_font(  LabelEmission->font() );
    LabelEmission->setFont( LabelEmission_font );
    LabelEmission->setText( tr( "Emission" ) );

    LabelShininess = new QLabel( this );
    LabelShininess->setGeometry( QRect( 220, 220, 70, 20 ) );
    QFont LabelShininess_font(  LabelShininess->font() );
    LabelShininess->setFont( LabelShininess_font );
    LabelShininess->setText( tr( "Shininess" ) );

    LabelTransparency = new QLabel( this );
    LabelTransparency->setGeometry( QRect( 220, 260, 90, 20 ) );
    QFont LabelTransparency_font(  LabelTransparency->font() );
    LabelTransparency->setFont( LabelTransparency_font );
    LabelTransparency->setText( tr( "Transparency" ) );

    LabelName = new QLabel( this );
    LabelName->setGeometry( QRect( 30, 230, 70, 20 ) );
    LabelName->setText( tr( "Name" ) );

    EditName = new QLineEdit( this );
    EditName->setGeometry( QRect( 30, 260, 127, 22 ) );

    ButtonAmbient = new ColorEditButton( this );
    ButtonAmbient->setGeometry( QRect( 480, 10, 60, 40 ) );

    ButtonDiffuse = new ColorButton( this );
    ButtonDiffuse->setGeometry( QRect( 480, 60, 60, 40 ) );
    ButtonDiffuse->setFlat(true);

    ButtonSpecular = new ColorEditButton( this);
    ButtonSpecular->setGeometry( QRect( 480, 110, 60, 40 ) );

    ButtonEmission = new ColorEditButton( this );
    ButtonEmission->setGeometry( QRect( 480, 160, 60, 40 ) );

    SliderAmbient = new ColorEditSlider( this );
    SliderAmbient->setGeometry( QRect( 310, 20, 150, 20 ) );
    SliderAmbient->setOrientation( Qt::Horizontal );
    SliderAmbient->setTickPosition( QSlider::TicksLeft );

    SliderDiffuse = new QSlider( this );
    SliderDiffuse->setGeometry( QRect( 310, 70, 150, 20 ) );
    SliderDiffuse->setRange( 0, 255 );
    SliderDiffuse->setOrientation( Qt::Horizontal );
    SliderDiffuse->setTickPosition( QSlider::TicksLeft );

    SliderSpecular = new ColorEditSlider( this );
    SliderSpecular->setGeometry( QRect( 310, 120, 150, 20 ) );
    SliderSpecular->setOrientation( Qt::Horizontal );
    SliderSpecular->setTickPosition( QSlider::TicksLeft );

    SliderEmission = new ColorEditSlider( this );
    SliderEmission->setGeometry( QRect( 310, 170, 150, 20 ) );
    SliderEmission->setOrientation( Qt::Horizontal );
    SliderEmission->setTickPosition( QSlider::TicksLeft );

    SliderShininess = new QSlider( this);
    SliderShininess->setGeometry( QRect( 310, 220, 150, 20 ) );
    SliderShininess->setRange( 0, 100 );
    SliderShininess->setOrientation( Qt::Horizontal );
    SliderShininess->setTickPosition( QSlider::TicksLeft );

    SliderTransparency = new QSlider( this );
    SliderTransparency->setGeometry( QRect( 310, 260, 150, 20 ) );
    SliderTransparency->setRange( 0, 100 );
    SliderTransparency->setOrientation( Qt::Horizontal );
    SliderTransparency->setTickPosition( QSlider::TicksLeft );

    FrameGL = new ViewColorGL( this );
    FrameGL->setGeometry( QRect( 5, 20, 200, 200 ) );

    QObject::connect (EditName,SIGNAL(textChanged(const QString&)),
                      this,SLOT(setGeomName(const QString&)) );
    QObject::connect (SliderDiffuse,SIGNAL(valueChanged(int)),
                      this,SLOT(setDiffuse(int)) );
    QObject::connect (SliderShininess,SIGNAL(valueChanged(int)),
                      this,SLOT(setShininess(int)) );
    QObject::connect (SliderTransparency,SIGNAL(valueChanged(int)),
                      this,SLOT(setTransparency(int)) );

    QObject::connect (ButtonAmbient,SIGNAL(colorChanged(const QColor&)),
                      SliderAmbient,SLOT(setColor(const QColor&)) );
    QObject::connect (ButtonSpecular,SIGNAL(colorChanged(const QColor&)),
                      SliderSpecular,SLOT(setColor(const QColor&)) );
    QObject::connect (ButtonEmission,SIGNAL(colorChanged(const QColor&)),
                      SliderEmission,SLOT(setColor(const QColor&)) );

    QObject::connect (ButtonAmbient,SIGNAL(colorChanged(const QColor&)),
                      this,SLOT(setAmbient(const QColor&)) );
    QObject::connect (ButtonSpecular,SIGNAL(colorChanged(const QColor&)),
                      this,SLOT(setSpecular(const QColor&)) );
    QObject::connect (ButtonEmission,SIGNAL(colorChanged(const QColor&)),
                      this,SLOT(setEmission(const QColor&)) );

    QObject::connect (SliderAmbient,SIGNAL(colorChanged()),
                      this,SLOT(ambientChangedEvent()) );
    QObject::connect (SliderSpecular,SIGNAL(colorChanged()),
                      this,SLOT(specularChangedEvent()) );
    QObject::connect (SliderEmission,SIGNAL(colorChanged()),
                      this,SLOT(emissionChangedEvent()) );

}

/*
 *  Destroys the object and frees any allocated resources
 */
MaterialEditor::~MaterialEditor()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Main event handler. Reimplemented to handle application
 *  font changes
 */
bool MaterialEditor::event( QEvent* ev )
{
    bool ret = QWidget::event( ev );
    if ( ev->type() == QEvent::ApplicationFontChange ) {
        QFont LabelAmbient_font(  LabelAmbient->font() );
        LabelAmbient->setFont( LabelAmbient_font );
        QFont LabelDiffuse_font(  LabelDiffuse->font() );
        LabelDiffuse->setFont( LabelDiffuse_font );
        QFont LabelSpecular_font(  LabelSpecular->font() );
        LabelSpecular->setFont( LabelSpecular_font );
        QFont LabelEmission_font(  LabelEmission->font() );
        LabelEmission->setFont( LabelEmission_font );
        QFont LabelShininess_font(  LabelShininess->font() );
        LabelShininess->setFont( LabelShininess_font );
        QFont LabelTransparency_font(  LabelTransparency->font() );
        LabelTransparency->setFont( LabelTransparency_font );
    }
    return ret;
}

void MaterialEditor::addMenu(QMenuBar * menubar){
}

void MaterialEditor::removeMenu(QMenuBar * menubar){
}

QString MaterialEditor::geomClassName() const{
  return QString("Material");
}

const QSize MaterialEditor::getSize() const {
  return QSize(550,300);
}

bool MaterialEditor::setGeomObject(SceneObjectPtr obj){
  MaterialPtr material = dynamic_pointer_cast<Material>(obj);
  if(material){
    setMaterial(material);
    return true;
  }
  else return false;
}

void MaterialEditor::newObj(){
  setMaterial(MaterialPtr(new Material(string("newMaterial"))));
}

SceneObjectPtr MaterialEditor::getGeomObject(){
  return SceneObjectPtr(__material);
}

void MaterialEditor::setMaterial(const MaterialPtr& m){
  __material = m;
  if(m){
  EditName->setText(QString(m->getName().c_str()));
  Color3 c = m->getAmbient();
  __diffuse = (int)(m->getDiffuse()*(*c.getMax()));
  SliderDiffuse->setValue(__diffuse);
  SliderShininess->setValue((int)(m->getShininess()*100));
  SliderTransparency->setValue((int)(m->getTransparency()*100));
  ButtonAmbient->setColor(QColor(c.getRed(),c.getGreen(),c.getBlue()));
  SliderAmbient->setColor(QColor(c.getRed(),c.getGreen(),c.getBlue()));
  real_t r = m->getDiffuse();
  ButtonDiffuse->setColor(QColor((int)(r*c.getRed()),(int)(r*c.getGreen()),(int)(r*c.getBlue())));
  c = m->getSpecular();
  ButtonSpecular->setColor(QColor(c.getRed(),c.getGreen(),c.getBlue()));
  SliderSpecular->setColor(QColor(c.getRed(),c.getGreen(),c.getBlue()));
  c = m->getEmission();
  ButtonEmission->setColor(QColor(c.getRed(),c.getGreen(),c.getBlue()));
  SliderEmission->setColor(QColor(c.getRed(),c.getGreen(),c.getBlue()));
  FrameGL->setAppearance(AppearancePtr(m));
  }
}

void MaterialEditor::setDiffuse(  int v ){
  const Color3& c = __material->getAmbient();
  real_t m = (*(c.getMax()));
  if(m != 0){
    real_t diff = ((real_t)(v )/ m );
    if(__material->getDiffuse() != diff ){
      __material->getDiffuse() = diff;
      __diffuse = v;
      update();
    }
    ButtonDiffuse->setColor(QColor((int)(diff*c.getRed()),(int)(diff*c.getGreen()),(int)(diff*c.getBlue())));
  }
}

void MaterialEditor::setAmbient(  const QColor& c ){
  __material->getAmbient() = Color3(c.red(),c.green(),c.blue());
  real_t m  = *(__material->getAmbient().getMax());
  if( m != 0){
    real_t diff = (real_t)(__diffuse) / m;
    __material->getDiffuse() = diff;
    SliderDiffuse->setValue(__diffuse);
    update();
  }
}

void MaterialEditor::setSpecular(  const QColor& c ){
  __material->getSpecular() = Color3(c.red(),c.green(),c.blue());
  update();
}

void MaterialEditor::setEmission( const QColor& c ){
  __material->getEmission() = Color3(c.red(),c.green(),c.blue());
  update();
}

void MaterialEditor::setShininess( int v ){
  __material->getShininess() = ((real_t)v)/100.0;
  update();
}

void MaterialEditor::setTransparency( int v ){
  __material->getTransparency() = ((real_t)v)/100.0;
  update();
}

void MaterialEditor::setGeomName(const QString& name ){
  if(__material){
    if(name.isEmpty()) __material->setName("");
    else __material->setName(name.toStdString());
  }
}

void MaterialEditor::update(){
  FrameGL->updateGL();
}

void MaterialEditor::ambientChangedEvent(){
  QColor c = SliderAmbient->getColor();
  ButtonAmbient->setColor(c);
  setAmbient(c);
}

void MaterialEditor::specularChangedEvent(){
  QColor c = SliderSpecular->getColor();
  ButtonSpecular->setColor(c);
  setSpecular(c);
}

void MaterialEditor::emissionChangedEvent(){
  QColor c = SliderEmission->getColor();
  ButtonEmission->setColor(c);
  setEmission(c);
}

void MaterialEditor::clear(){
        setMaterial(MaterialPtr());
}
