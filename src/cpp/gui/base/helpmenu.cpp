/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): C. Nouguier & F. Boudon
 *                       N. Dones & B. Adam
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
 *
 *  ----------------------------------------------------------------------------
 * 
 *                      GNU General Public license
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

#include <qstring.h>
#include <qmessagebox.h>
#include <qlistview.h> 
#include <qapplication.h> 
#include <qlabel.h>
#include <qtimer.h>
#include <qtextview.h>
#include <qfile.h>
#include <qtextstream.h>
#include <tool/util_enviro.h>

/*  ------------------------------------------------------------------------ */

#if QT_VERSION < 300

#include <qplatinumstyle.h>
#include <qmotifplusstyle.h> 
#include <qsgistyle.h> 
#include <qcdestyle.h>

#define STYLE_MACRO(macro) \
	macro(Windows) \
	macro(Platinum) \
	macro(Motif) \
	macro(MotifPlus) \
	macro(CDE) \
	macro(SGI)

#else

#include <qstyle.h>
#include <qstylefactory.h>

#endif

/*  ------------------------------------------------------------------------ */

#include "icons.h"

#include "helpmenu.h"
#include "info.h"
#include "qobjectbrowser.h"

#include "view_version.h"
#include <scenegraph/pgl_version.h>
#include <tool/tools_version.h>

#include <algo/codec/scne_scanner.h>
#include <algo/codec/binaryprinter.h>
#include <gui/viewer/geomscenegl.h>
#include <tool/util_enviro.h>

PGL_USING_NAMESPACE
using namespace std;



#if QT_VERSION < 300
#define CONSTRUCT_MENU(style) \
	__ids.push_back( __style->insertItem(#style,this,SLOT(setStyle(int))));
#endif

static QString default_style_name;

ViewHelpMenu::ViewHelpMenu(QWidget * parent,
                   QGLWidget * glwidget,
                   const char * name) :
  QPopupMenu(parent,name),
  __glwidget(glwidget)
{
	__about = new ViewAboutDialog(this,"General Information",false);
#ifndef _DEBUG
//	if(__about->useNewStyle())__about->display(1000);
#endif
	QObject::connect(__about,SIGNAL(licenseView()),this,SLOT(showLicense()));
	insertItem( tr("What's &This?"), parent->parent() , SLOT(whatsThis()), Key_F1);
    insertItem(QPixmap(ViewSysInfo::tools_logo),tr("&Help"),this,SLOT(showHelp()),SHIFT+Key_F1);
    insertSeparator();
    insertItem(ViewerIcon::getPixmap(ViewerIcon::icon_flower),tr("&About Viewer"),this,SLOT(showAbout()), CTRL+Key_F1);
    insertItem(tr("&License"),this,SLOT(showLicense()));
    insertItem(QPixmap(ViewSysInfo::qt_logo),tr("About &Qt"),this,SLOT(aboutQt()));
    insertSeparator();
    insertItem(QPixmap(ViewSysInfo::info_logo),tr("Technical Characteristics"),this,SLOT(generalInfo()));
    insertItem(QPixmap(ViewSysInfo::qt_logo),tr("Qt Hierarchy"),this,SLOT(qtbrowse()));
    insertSeparator();
    __style = new QPopupMenu(this,"Style");
#if QT_VERSION < 300
	STYLE_MACRO(CONSTRUCT_MENU);
#else
	__ids.push_back( __style->insertItem("Default",this,SLOT(setStyle(int))));
	QStringList styles = QStyleFactory::keys();
	for(uint k = 0 ; k < styles.size(); k++)
	  __ids.push_back( __style->insertItem(*(styles.at(k)),this,SLOT(setStyle(int))));
#endif
	for(uint i = 0 ; i < __ids.size(); i++)
		__style->setItemParameter( __ids[i],  i  );
    __style->setCheckable(true);
    insertItem(tr("Style"),__style);
	default_style_name = QApplication::style().className();
    checkItem(0);
}

ViewHelpMenu::~ViewHelpMenu(){
#ifdef  GEOM_DEBUG
  cout << "Help Menu deleted" << endl;
#endif
}

void 
ViewHelpMenu::setGLWidget(QGLWidget * glwidget)
{
  __glwidget = glwidget;
}

int ViewHelpMenu::getStyle() const {
    QStyle& style = QApplication::style();
    QString classname = style.className();
	if (classname == default_style_name) return 0;
	for(uint i = 0 ; i < __ids.size() ; i++){
#if QT_VERSION < 300
		if('Q'+__style->text(__ids[i])+"Style" == classname)
			return i;
#else		
		if(__style->text(__ids[i]) == classname)
			return i;
#endif
	}
	return -1;
}

#if QT_VERSION < 300
#define APPLY_STYLE(stylename) \
	if(__style->text(__ids[i])==#stylename){ \
		QApplication::setStyle( new Q##stylename##Style ); \
	    qDebug((QString("Application.setStyle(") + #stylename +')').latin1() ); } \
	 else
#endif

void  
ViewHelpMenu::setStyle(int i)
{
	if(i < 0 && i >=__ids.size())return;
#if QT_VERSION < 300
    STYLE_MACRO(APPLY_STYLE)
		return;
#else
	QString stylename = ( i == 0 ? default_style_name : *(QStyleFactory::keys().at(i-1)));
    QApplication::setStyle( QStyleFactory::create(stylename)); 
    qDebug((QString("Application.setStyle(") + stylename +')').latin1() ); 
#endif
	if(i>= 0 && i<__ids.size())checkItem(i);
}

void  
ViewHelpMenu::checkItem(int i)
{
  for(uint j = 0; j < __ids.size() ; j++)
    __style->setItemChecked(__ids[j],false);
  if(i>=0 && i < __ids.size())__style->setItemChecked(__ids[i],true);
}

void ViewHelpMenu::showHelp()
{
  QString message ="<h3>How to use Viewer</h3><br>"
    "<p><b>Left button       :</b> Rotation <br>"
    "<b>Right button      :</b> Translation <br>"
    "<b>Middle button     :</b> Zoom <br>"
    "<b>Ctrl + click      :</b> Light <br>"
    "<b>Shift + click     :</b> Selection <br>"
    "<b>x-move</b> -> Azimuth control <br>"
    "<b>y-move</b> -> Elevation control <br></p>";

  QMessageBox::information(this, tr("How to use Viewer"),tr(message));
}


void ViewHelpMenu::showAbout()
{  
  if(!__about)__about = new ViewAboutDialog(this,"About",false);
  __about->display(-1);
}

void ViewHelpMenu::showInit()
{  
  if(!__about)__about = new ViewAboutDialog(this,"About",false);
  __about->display(2000);
}

void ViewHelpMenu::showLicense()
{  
  QDialog b(this,"License",true,
	WStyle_Customize | WStyle_DialogBorder | WStyle_Tool | WStyle_SysMenu);
  QPixmap logo = ViewerIcon::getPixmap( "gnu.png");
  QLabel * llogo = new QLabel(&b,"licenselogo");
  llogo->setGeometry(QRect(QPoint(0,0),logo.size()));
  llogo->setPixmap(logo);
  b.setMinimumSize(QSize(logo.width()*4,logo.height()));
  b.setCaption("License");
  QTextView * lictext = new QTextView(&b,"License Text");
  QFont f("Courrier", 8);
  lictext->setFont( f );
  lictext->setGeometry(QRect(logo.width(),0,logo.width()*3,logo.height()));
  lictext->setHScrollBarMode(QScrollView::AlwaysOff);
  lictext->setLineWidth(0);
  QPalette pal = lictext->palette();
  QColorGroup c = pal.active();
  c.setColor(QColorGroup::Background,QColor(255,255,255));
  pal.setActive(c);
  lictext->setPalette(pal);
  QFile copyright((TOOLS(getPlantGLDir())+"/share/plantGL/LICENSE").c_str());
  if(copyright.exists() && copyright.open(IO_ReadOnly)){
	QTextStream s(&copyright);
	lictext->setText(s.read());
	copyright.close();
  }
  QSize s = qApp->desktop()->size();
  s = s - b.size();
  s /= 2;
  b.move(s.width(),s.height()); 
  b.exec();

}

void 
ViewHelpMenu::setInitText(const QString&t){
  if(!__about)__about = new ViewAboutDialog(this,"About",false);
  __about->setText(t);
}
void 
ViewHelpMenu::setInitText(const QString&t,int timeout)
{
  if(!__about)__about = new ViewAboutDialog(this,"About",false);
  __about->setText(t,timeout);
}


void
ViewHelpMenu::aboutQt()
{
 QMessageBox::aboutQt( this, tr("About Qt") );
}

void
ViewHelpMenu::qtbrowse()
{
        ViewQObjectBrowser a(this,"qtbrowse",TRUE);
        a.exec();
}

void
ViewHelpMenu::generalInfo()
{
  std::string text2 = getViewerVersionString();
  QString text;
  ViewSysInfo a(this,__glwidget,tr("PlantGL Viewer")+" "+QString(text2.c_str()),TRUE);
  QListViewItem * itemF = a.addItem(tr("Geom Library"));
  QListViewItem *item = new QListViewItem( itemF );
  item->setText( 0, tr( "Version" ) );
  text2 = getPGLVersionString();
  item->setText( 1, QString(text2.c_str()) );
  item = new QListViewItem( itemF, item );
  item->setText( 0, tr( "Binary Format Version" ) );
  item->setText( 1, QString::number(BinaryPrinter::BINARY_FORMAT_VERSION) );
  item = new QListViewItem( itemF, item );
  item->setText( 0, tr( "Real Type Precision" ) );
#ifdef GEOM_USE_DOUBLE
  text = "Double";
#else
  text = "Simple";
#endif
  item->setText( 1, tr( text ) );
  item = new QListViewItem( itemF, item );
  item->setText( 0, tr( "Using Threads" ) );
  if(ViewGeomSceneGL::useThread()) text = "True";
  else text = "False";
  item->setText( 1, tr( text ) );

  item = new QListViewItem( itemF, item );
  item->setText( 0, tr( "Geom Namespace" ) );
#ifndef GEOM_NAMESPACE_NAME
  text = "False";
#else
  text = "True";
#endif
  item->setText( 1, tr( text ) );
  item = new QListViewItem( itemF, item );
  item->setText( 0, tr( "Geom Debug" ) );
#ifdef GEOM_DEBUG
  text = "True";
#else
  text = "False";
#endif
  item->setText( 1, tr( text ) );
#ifdef _WIN32
  item = new QListViewItem( itemF, item );
  item->setText( 0, tr( "PGL DLL" ) );
#ifdef GEOM_DLL
  text = "True";
#else
  text = "False";
#endif
  item->setText( 1, tr( text ) );
#endif
  item = new QListViewItem( itemF, item );
  item->setText( 0, tr( "Using Glut" ) );
#ifdef WITH_GLUT
  text = "True";
#else
  text = "False";
#endif
  item->setText( 1, tr( text ) );
  itemF = a.addItem(tr("Tools Library"));
  item = new QListViewItem( itemF );
  item->setText( 0, tr( "Version" ) );
  item->setText( 1, QString(getToolsVersionString().c_str()) );
  item = new QListViewItem( itemF, item );
  item->setText( 0, tr( "Tools Namespace" ) );
#ifndef TOOLS_NAMESPACE_NAME
  text = "False";
#else
  text = "True";
#endif
  item->setText( 1, tr( text ) );
  item = new QListViewItem( itemF, item );
  item->setText( 0, tr( "Using RogueWave" ) );
#ifdef RWOUT
  text = "False";
#else
  text = "True";
#endif
  item->setText( 1, tr( text ) );
  itemF = a.addItem(tr("PlantGL"));
  item = new QListViewItem( itemF );
  item->setText( 0, tr( "Install Path" ) );
  string p = TOOLS(getPlantGLDir());
  if(!p.empty())item->setText( 1, QString(p.c_str()) );
  itemF = a.addItem(tr("Flex"));
  item = new QListViewItem( itemF );
  item->setText( 0, tr( "Version" ) );
  item->setText( 1, QString(lexerVersion().c_str())  );
  a.exec();
}





ViewAboutDialog::ViewAboutDialog ( QWidget * parent, 
								  const char * name, 
								  int timeout,
								  bool modal):
QDialog ( parent, name, modal, 
		 WStyle_Customize | WStyle_NoBorderEx | WStyle_StaysOnTop  ),
		__text(NULL),
	    __style(true),
		__license(false){
  __logo = ViewerIcon::getPixmap( "geomviewer.png");
  if(__logo.isNull()) {
    __style = false;
    __logo = ViewerIcon::getPixmap( ViewerIcon::icon_plantlogo);
  }
  else {
    __logo2 = ViewerIcon::getPixmap( "geomviewer2.png");
  }
  setIconPixmap(__logo);
  __licenseRect = QRect(140,180,62,110);
  
  QString message =  "License : GNU General Public License (GPL).\n"
    "Developper : F. Boudon, C. Nouguier,\n"
    "C. Pradal, C. Godin -\n"
    "Virtual Plants Team - UMR AMAP\n"
    "N. Dones, B. Adam - UMR PIAF\n"
    "3D Models : F. Boudon, H. Sinoquet\n"
    "Y. Caraglio, F. Danjon";
  if(__style)setInfo(message,QRect(240,190,180,height()-210),7); 
  
  QString message2 =  "Version "+QString(getPGLVersionString().c_str())+
    "\nDate : " __DATE__;
  if(__style)setInfo(message2, QRect(20,220,95,40),7);
  else setInfo(message2, QRect(5,height()-45,80,40),6);

  if(__style){
    __text = new QLabel(this);
    QFont f("Courrier", 7);
    __text->setFont( f );
    __text->setGeometry(QRect(240,height()-25,260,15));
    QPalette pal = __text->palette();
    QColorGroup c = pal.active();
    c.setColor(QColorGroup::Background,QColor(255,255,255));
    pal.setActive(c);
    __text->setPalette(pal);
    __text->installEventFilter(this);
    QObject::connect(&__timer,SIGNAL(timeout()),__text,SLOT(clear()));
    setText("Initialization ...",800);
  }
};

ViewAboutDialog::~ViewAboutDialog(){}


void ViewAboutDialog::display(int timeout){
  if(timeout != -1)
	QTimer::singleShot(timeout,this,SLOT(hide()));
  show();
}


void ViewAboutDialog::setIconPixmap( const QPixmap & icon){
  setBackgroundPixmap ( icon ) ;
  setMinimumSize(icon.size());
  setMaximumSize(icon.size());
  QSize s = qApp->desktop()->size();
  s = s - icon.size();
  s /= 2;
  move(s.width(),s.height()); 
}

void ViewAboutDialog::setInfo(const QString& text, QRect r,int s){
  QLabel * _text = new QLabel(this);
  QFont f("Courrier", s);
  _text->setFont( f );
  _text->setGeometry(r);
  QPalette pal = _text->palette();
  QColorGroup c = pal.active();
  c.setColor(QColorGroup::Background,QColor(255,255,255));
  pal.setActive(c);
  _text->setPalette(pal);
  _text->setText(text);
  _text->installEventFilter(this);
}

void ViewAboutDialog::mousePressEvent ( QMouseEvent * e) {
  QPoint p = e->pos();
  if(__style && __licenseRect.contains(p))
	emit licenseView();
  else	if (e->button() ==  LeftButton) accept();
}

bool ViewAboutDialog::eventFilter( QObject *o, QEvent *e )
{
  if ( e->type() == QEvent::MouseButtonPress ) {
	accept();
  }
  if ( e->type() == QEvent::MouseMove ) { 
	QMouseEvent * m = (QMouseEvent *)e;
	changeLogo(m->pos());
	return QWidget::eventFilter( o, e );
  }
  return QWidget::eventFilter( o, e );    // standard event processing
}

void 
ViewAboutDialog::setText(const QString& t){
  if(__text){
   if(__timer.isActive())__timer.stop();
	__text->setText(t);
  }
}

void 
ViewAboutDialog::setText(const QString& t,int timeout){
  if(__text){
   if(__timer.isActive())__timer.stop();
	__text->setText(t);
	__timer.start(timeout);
  }
}

void 
ViewAboutDialog::clear(){
  if(__text){
   if(__timer.isActive())__timer.stop();
	__text->clear();
  }
}

void 
ViewAboutDialog::showEvent ( QShowEvent * )
{
  setMouseTracking(true);
}

void
ViewAboutDialog::hideEvent ( QHideEvent * )
{
  setMouseTracking(false);
}

void
ViewAboutDialog::mouseMoveEvent ( QMouseEvent * e )
{
	if (e->state() == NoButton) changeLogo(e->pos());
	else move(e->globalPos());
}

void
ViewAboutDialog::changeLogo(const QPoint& p)
{
  if(__style && !__logo2.isNull()){
//	qDebug("Point = ("+QString::number(p.x())+','+QString::number(p.x())+')');
	if(__license && !__licenseRect.contains(p)){
	  __license = false;
	  setBackgroundPixmap ( __logo ) ;
	}
	else if(!__license && __licenseRect.contains(p)){
	  __license = true;
	  setBackgroundPixmap ( __logo2 ) ;
	}
  }
}
