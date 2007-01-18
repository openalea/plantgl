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


#include "view_materialeditor.h"

#include <qlabel.h>
#include <qlineedit.h>
#include <qslider.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qfont.h>
#include <qcolordialog.h>

GEOM_USING_NAMESPACE
using namespace std;

/*----------------------------------------------------------------------------*/
ViewColorGL::ViewColorGL( QWidget * parent, const char * name, const QGLWidget * shareWidget, WFlags f ):
 QGLWidget(parent,name,shareWidget,f),
 __sphereobject(NULL),
 __r(__d){
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
  if(__sphereobject)gluDeleteQuadric(__sphereobject);
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

  if(__sphereobject)gluDeleteQuadric(__sphereobject);
  __sphereobject = gluNewQuadric();
  gluQuadricDrawStyle(__sphereobject,GLU_FILL);
  gluQuadricNormals(__sphereobject,GLU_SMOOTH);

  __spheredrawlist=glGenLists(1);
  glNewList(__spheredrawlist,GL_COMPILE);{
    gluSphere(__sphereobject,__radius,80,80);
  }
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
  QPushButton(parent,name),
  __color(0,0,0){
  setColor(__color);
}

ColorButton::~ColorButton(){
}

void
ColorButton::setColor(const QColor& c){
  __color = c;
  QPixmap col(60,40);
  col.fill(c);
  setPixmap(col);
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
  QSlider(parent,name){
  QObject::connect(this,SIGNAL(valueChanged(int)),this,SLOT(valueChangedEvent(int)));
  setMaxValue( 255 );
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

MaterialEditor::MaterialEditor( QWidget* parent,  const char* name, WFlags fl )
    : GeomModuleEditor( parent, name, fl )
{
    if ( !name )
        setName( "MaterialEditor" );
    resize( 550, 300 );
    setMinimumSize(550,300);
    QFont f( font() );
    f.setFamily( "adobe-courier" );
    f.setPointSize( 10 );
    setFont( f );
    setCaption( tr( "Material" ) );

    LabelAmbient = new QLabel( this, "LabelAmbient" );
    LabelAmbient->setGeometry( QRect( 220, 20, 61, 20 ) );
    QFont LabelAmbient_font(  LabelAmbient->font() );
    LabelAmbient->setFont( LabelAmbient_font );
    LabelAmbient->setText( tr( "Ambient" ) );

    LabelDiffuse = new QLabel( this, "LabelDiffuse" );
    LabelDiffuse->setGeometry( QRect( 220, 70, 61, 20 ) );
    QFont LabelDiffuse_font(  LabelDiffuse->font() );
    LabelDiffuse->setFont( LabelDiffuse_font );
    LabelDiffuse->setText( tr( "Diffuse" ) );

    LabelSpecular = new QLabel( this, "LabelSpecular" );
    LabelSpecular->setGeometry( QRect( 220, 120, 61, 20 ) );
    QFont LabelSpecular_font(  LabelSpecular->font() );
    LabelSpecular->setFont( LabelSpecular_font );
    LabelSpecular->setText( tr( "Specular" ) );

    LabelEmission = new QLabel( this, "LabelEmission" );
    LabelEmission->setGeometry( QRect( 220, 170, 61, 20 ) );
    QFont LabelEmission_font(  LabelEmission->font() );
    LabelEmission->setFont( LabelEmission_font );
    LabelEmission->setText( tr( "Emission" ) );

    LabelShininess = new QLabel( this, "LabelShininess" );
    LabelShininess->setGeometry( QRect( 220, 220, 70, 20 ) );
    QFont LabelShininess_font(  LabelShininess->font() );
    LabelShininess->setFont( LabelShininess_font );
    LabelShininess->setText( tr( "Shininess" ) );

    LabelTransparency = new QLabel( this, "LabelTransparency" );
    LabelTransparency->setGeometry( QRect( 220, 260, 90, 20 ) );
    QFont LabelTransparency_font(  LabelTransparency->font() );
    LabelTransparency->setFont( LabelTransparency_font );
    LabelTransparency->setText( tr( "Transparency" ) );

    LabelName = new QLabel( this, "LabelName" );
    LabelName->setGeometry( QRect( 30, 230, 70, 20 ) );
    LabelName->setText( tr( "Name" ) );

    EditName = new QLineEdit( this, "EditName" );
    EditName->setGeometry( QRect( 30, 260, 127, 22 ) );

    ButtonAmbient = new ColorEditButton( this, "ButtonAmbient" );
    ButtonAmbient->setGeometry( QRect( 480, 10, 60, 40 ) );

    ButtonDiffuse = new ColorButton( this, "ButtonDiffuse" );
    ButtonDiffuse->setGeometry( QRect( 480, 60, 60, 40 ) );
    ButtonDiffuse->setFlat(true);

    ButtonSpecular = new ColorEditButton( this, "ButtonSpecular" );
    ButtonSpecular->setGeometry( QRect( 480, 110, 60, 40 ) );

    ButtonEmission = new ColorEditButton( this, "ButtonEmission" );
    ButtonEmission->setGeometry( QRect( 480, 160, 60, 40 ) );

    SliderAmbient = new ColorEditSlider( this, "SliderAmbient" );
    SliderAmbient->setGeometry( QRect( 310, 20, 150, 20 ) );
    SliderAmbient->setOrientation( QSlider::Horizontal );
    SliderAmbient->setTickmarks( QSlider::Left );

    SliderDiffuse = new QSlider( this, "SliderDiffuse" );
    SliderDiffuse->setGeometry( QRect( 310, 70, 150, 20 ) );
    SliderDiffuse->setMaxValue( 255 );
    SliderDiffuse->setOrientation( QSlider::Horizontal );
    SliderDiffuse->setTickmarks( QSlider::Left );

    SliderSpecular = new ColorEditSlider( this, "SliderSpecular" );
    SliderSpecular->setGeometry( QRect( 310, 120, 150, 20 ) );
    SliderSpecular->setOrientation( QSlider::Horizontal );
    SliderSpecular->setTickmarks( QSlider::Left );

    SliderEmission = new ColorEditSlider( this, "SliderEmission" );
    SliderEmission->setGeometry( QRect( 310, 170, 150, 20 ) );
    SliderEmission->setOrientation( QSlider::Horizontal );
    SliderEmission->setTickmarks( QSlider::Left );

    SliderShininess = new QSlider( this, "SliderShininess" );
    SliderShininess->setGeometry( QRect( 310, 220, 150, 20 ) );
    SliderShininess->setMaxValue( 100 );
    SliderShininess->setOrientation( QSlider::Horizontal );
    SliderShininess->setTickmarks( QSlider::Left );

    SliderTransparency = new QSlider( this, "SliderTransparency" );
    SliderTransparency->setGeometry( QRect( 310, 260, 150, 20 ) );
    SliderTransparency->setMaxValue( 100 );
    SliderTransparency->setOrientation( QSlider::Horizontal );
    SliderTransparency->setTickmarks( QSlider::Left );

    FrameGL = new ViewColorGL( this, "FrameGL" );
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
  MaterialPtr material;
  if(material.cast(obj)){
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
    else __material->setName(name.data());
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
        setMaterial(MaterialPtr(0));
}
