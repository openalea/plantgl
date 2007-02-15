/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
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

#include "properties.h"
#include "scenegl.h"
#include "camera.h"
#include "glframe.h"
#include "filemanager.h"
#include "controlpanel.h"
#include "translator.h"
#include <tool/util_enviro.h>

#include <qframe.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qmessagebox.h>
#include <qcombobox.h>

#define XBEG_1 20
#define WIDTH_1 80
#define XBEG_2 XBEG_1+WIDTH_1+10
#define WIDTH_2 260

PGL_USING_NAMESPACE

/*
 *  Constructs a Properties which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ViewProperties::ViewProperties(  ViewGLFrame *g,
             ViewFileManager* parent,
             ViewControlPanel * controlpanel,
             bool config,
             const char* name,
             bool modal,
             WFlags fl )
  : QDialog( parent, name, modal, fl )
{

    if ( !name )
    setName( "Geom Properties" );

    resize( 424, 515 );
    setFixedSize(QSize(424, 515 ));
    setCaption( tr( "Properties" ) );


    QPushButton * PushButton = new QPushButton( this, "Cancel" );
    PushButton->setGeometry( QRect( 310, 470, 91, 31 ) );
    PushButton->setText( QMessageBox::tr( "&Cancel" ) );
    QObject::connect( PushButton,SIGNAL(clicked()),this,SLOT(reject()));


    PushButton = new QPushButton( this, "Ok" );
    PushButton->setGeometry( QRect( 190, 470, 91, 31 ) );
    PushButton->setText( QMessageBox::tr( "&Ok" ) );
    QObject::connect( PushButton,SIGNAL(clicked()),this,SLOT(apply()));

    QTabWidget *TabWidget = new QTabWidget( this );
    TabWidget->setGeometry( QRect( 10, 10, 400, 450 ) );

    ViewRendererGL * scene = g->getSceneRenderer();
    const QString filename = scene->getFilename();

    if(!filename.isEmpty() && QFile::exists(filename) ){
    QWidget * tab = new QWidget( TabWidget, "tab" );

    QFileInfo _f(filename);

    QFrame * Line = new QFrame( tab );
    Line->setGeometry( QRect( XBEG_1, 255, 351, 20 ) );
#if QT_VERSION < 300
    Line->setFrameStyle( QFrame::HLine | QFrame::Sunken );
#else
    Line->setFrameShape( QFrame::HLine );
    Line->setFrameShadow( QFrame::Sunken );
#endif
    Line = new QFrame( tab );
    Line->setGeometry( QRect( XBEG_1, 80, 351, 20 ) );
#if QT_VERSION < 300
    Line->setFrameStyle( QFrame::HLine | QFrame::Sunken );
#else
    Line->setFrameShape( QFrame::HLine );
    Line->setFrameShadow( QFrame::Sunken );
#endif

    QLabel * TextLabel = new QLabel( tab );
    TextLabel->setGeometry( QRect( XBEG_1, 30, WIDTH_1, 31 ) );
    TextLabel->setText( tr( "Name")+" :"  );

    TextLabel= new QLabel( tab );
    TextLabel->setGeometry( QRect( XBEG_1, 110, WIDTH_1, 31 ) );
    TextLabel->setText( tr( "Location")+" :" );

    TextLabel= new QLabel( tab );
    TextLabel->setGeometry( QRect( XBEG_1, 160, WIDTH_1, 31 ) );
    TextLabel->setText( tr( "Size")+" :" );

    TextLabel= new QLabel( tab );
    TextLabel->setGeometry( QRect( XBEG_1, 210, WIDTH_1, 31 ) );
    TextLabel->setText( tr( "Owner")+" :" );

    TextLabel= new QLabel( tab );
    TextLabel->setGeometry( QRect( XBEG_1, 295, WIDTH_1, 40 ) );
    TextLabel->setText( tr( "Last Modified")+" :" );

    TextLabel= new QLabel( tab );
    TextLabel->setGeometry( QRect( XBEG_1, 350, WIDTH_1, 31 ) );
    TextLabel->setText( tr( "Last Accessed")+" :" );

    TextLabel= new QLabel( tab );
    TextLabel->setGeometry( QRect( XBEG_2, 30, WIDTH_2, 31 ) );
    TextLabel->setText( tr( _f.fileName() ) );

    // Path
    QLineEdit * TextLabel2 = new QLineEdit(  tab );
    TextLabel2->setGeometry( QRect( XBEG_2, 106,  WIDTH_2, 40 ) );
    TextLabel2->setReadOnly(true);
    TextLabel2->setText( tr( _f.dirPath(true)  ) );

    TextLabel = new QLabel( tab );
    TextLabel->setGeometry( QRect( XBEG_2, 155,  WIDTH_2, 40 ) );
    if(_f.size()>1024)
        TextLabel->setText( tr( QString::number((int)(_f.size()/1024))+" Kbytes "+ QString::number((int)(_f.size()%1024)))+" bytes" );
    else
        TextLabel->setText( tr( QString::number(_f.size())+" bytes" ) );

    TextLabel = new QLabel( tab );
    TextLabel->setGeometry( QRect( XBEG_2, 205,  WIDTH_2, 40 ) );
    TextLabel->setText( _f.owner()+" - "+_f.group()  );

    TextLabel = new QLabel( tab );
    TextLabel->setGeometry( QRect( XBEG_2, 295,  WIDTH_2, 40 ) );
    TextLabel->setText( (_f.lastModified()).toString() );

    TextLabel = new QLabel( tab );
    TextLabel->setGeometry( QRect( XBEG_2, 345,  WIDTH_2, 40 ) );
    TextLabel->setText( (_f.lastRead()).toString()  );

    TabWidget->insertTab( tab, tr( "&File" ) );
    }
    g->addProperties(TabWidget);

    QWidget * tab = new QWidget( TabWidget, "tab" );
    TabWidget->insertTab( tab, tr( "C&onfig" ) );
    if(config)TabWidget->showPage (tab);

    QGroupBox * SaveGroup = new QGroupBox( tab, "SaveGroup" );
    SaveGroup->setProperty( "geometry", QRect( 30, 20, 320, 190 ) );
    SaveGroup->setProperty( "title", tr( "Saved Options")+" - v"+QString::number(ViewFileManager::CONFIG_VERSION) );

    QCheckBox * Box = new QCheckBox( SaveGroup, "WinBox" );
    Box->setProperty( "geometry", QRect( 10, 20, 270, 20 ) );
    Box->setProperty( "text", tr( "Window Position and Size" ) );
    Box->setChecked(parent->getSaveWinPos());
    QObject::connect(Box,SIGNAL(toggled(bool)),parent,SLOT(saveWinPos(bool)));

    Box = new QCheckBox( SaveGroup, "WinStyleBox" );
    Box->setProperty( "geometry", QRect( 10, 40, 270, 20 ) );
    Box->setProperty( "text", tr( "Window Style" ) );
    Box->setChecked(parent->getSaveStyle());
    QObject::connect(Box,SIGNAL(toggled(bool)),parent,SLOT(saveStyle(bool)));

    Box = new QCheckBox( SaveGroup, "ToolBarBox" );
    Box->setProperty( "geometry", QRect( 10, 60, 270, 20 ) );
    Box->setProperty( "text", tr( "ToolBars States (Experimental)" ) );
    Box->setChecked(parent->getToolbarStateSave());
    QObject::connect(Box,SIGNAL(toggled(bool)),parent,SLOT(saveToolbarState(bool)));

    Box = new QCheckBox( SaveGroup, "HistoryBox" );
    Box->setProperty( "geometry", QRect( 10, 80, 270, 20 ) );
    Box->setProperty( "text", tr( "File History" ) );
    Box->setChecked(parent->getSaveHistory());
    QObject::connect(Box,SIGNAL(toggled(bool)),parent,SLOT(saveHistory(bool)));

    Box = new QCheckBox( SaveGroup, "BgBox" );
    Box->setProperty( "geometry", QRect( 10, 100, 270, 20 ) );
    Box->setProperty( "text", tr( "BackGround Color" ) );
    Box->setChecked(parent->getSaveBgColor());
    QObject::connect(Box,SIGNAL(toggled(bool)),parent,SLOT(saveBgColor(bool)));

    Box = new QCheckBox( SaveGroup, "GridBox" );
    Box->setProperty( "geometry", QRect( 10, 120, 270, 20 ) );
    Box->setProperty( "text", tr( "Grids Visibility" ) );
    Box->setChecked(parent->getSaveGridVisibility());
    QObject::connect(Box,SIGNAL(toggled(bool)),parent,SLOT(saveGridVisibility(bool)));

    Box = new QCheckBox( SaveGroup, "CamBox" );
    Box->setProperty( "geometry", QRect( 10, 140, 270, 20 ) );
    Box->setProperty( "text", tr( "Camera/Grid Automatic Fitting" ) );
    Box->setChecked(parent->getSaveCameraReDim());
    QObject::connect(Box,SIGNAL(toggled(bool)),parent,SLOT(saveCameraReDim(bool)));

    Box = new QCheckBox( SaveGroup, "SpinBox" );
    Box->setProperty( "geometry", QRect( 10, 160, 270, 20 ) );
    Box->setProperty( "text", tr( "SpinBox instead of Dials in Control Panel" ) );
    Box->setChecked(parent->getSaveUseSpinBox());
    QObject::connect(Box,SIGNAL(toggled(bool)),parent,SLOT(saveUseSpinBox(bool)));

    QGroupBox * AppGroup = new QGroupBox( tab, "AppGroup" );
    AppGroup->setProperty( "geometry", QRect( 30, 210, 320, 70 ) );
    AppGroup->setProperty( "title", tr( "Appearance Options") );

    Box = new QCheckBox( AppGroup, "ControlPanelBox" );
    Box->setProperty( "geometry", QRect( 10, 20, 270, 20 ) );
    Box->setProperty( "text", tr( "use SpinBox instead of Dials in Control Panel" ) );
    if(controlpanel)Box->setChecked(controlpanel->spinBoxUsed());
    QObject::connect(Box,SIGNAL(toggled(bool)),controlpanel,SLOT(useSpinBox(bool)));

    Box = new QCheckBox( AppGroup, "InitBox" );
    Box->setProperty( "geometry", QRect( 10, 40, 270, 20 ) );
    Box->setProperty( "text", tr( "show Initialization Dialog at Startup" ) );
    Box->setChecked(parent->getShowInit());
    QObject::connect(Box,SIGNAL(toggled(bool)),parent,SLOT(showInit(bool)));

    QString langname = TOOLS(getLanguage()).c_str();
    QGroupBox * LangGroup = new QGroupBox( tab, "LangGroup" );
    LangGroup->setProperty( "geometry", QRect( 30, 280, 320, 70 ) );
    LangGroup->setProperty( "title", tr( "Language") + ": " + langname);

    lang = new QComboBox( LangGroup, "LangBox" );
    lang->setProperty( "geometry", QRect( 10, 20, 270, 20 ) );
    int id  = -1;
    lang->insertItem("English",0);
    lang->insertItem("French",1);
    if(langname == "English")lang->setCurrentItem(0);
    if(langname == "French")lang->setCurrentItem(1);
    // QObject::connect(lang,SIGNAL(activated(const QString&)),this,SLOT(setLanguage(const QString&)));

    QLabel * lbl = new QLabel( tr("Note: Language change takes effect only at next startup."), LangGroup, "InitBox" );
    lbl->setProperty( "geometry", QRect( 10, 40, 300, 20 ) );

}

/*
 *  Destroys the object and frees any allocated resources
 */
ViewProperties::~ViewProperties()
{
    // no need to delete child widgets, Qt does it all for us
}

void
ViewProperties::apply(){
  QString langname = lang->currentText();
  if(TOOLS(getLanguage()).c_str() != langname){
    TOOLS(setLanguage(langname.latin1()));
    if(langname == "English")removeTranslator();
    else if(langname == "French")setFrenchTranslator();
  }
  accept();
}
