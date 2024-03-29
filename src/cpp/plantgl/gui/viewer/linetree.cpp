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


#include "linetree.h"

#include <plantgl/algo/codec/linetreeprinter.h>
#include <plantgl/algo/codec/ligfile.h>
#include <plantgl/tool/util_enviro.h>

#include <QtGlobal>
#include <QtCore/qtextstream.h>
#include <QRegularExpression>
#include <QtGui/qevent.h>
#include <QtCore/qvariant.h>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qfiledialog.h>
    #include <QtWidgets/qlabel.h>
    #include <QtWidgets/qlineedit.h>
    #include <QtWidgets/qpushbutton.h>
    #include <QtWidgets/qtoolbutton.h>
    #include <QtWidgets/qlayout.h>
    #include <QtWidgets/qtooltip.h>
    #include <QtWidgets/qwhatsthis.h>
    #include <QtWidgets/qcheckbox.h>
    #include <QtWidgets/qmessagebox.h>
#else
    #include <QtGui/qfiledialog.h>
    #include <QtGui/qlabel.h>
    #include <QtGui/qlineedit.h>
    #include <QtGui/qpushbutton.h>
    #include <QtGui/qtoolbutton.h>
    #include <QtGui/qlayout.h>
    #include <QtGui/qtooltip.h>
    #include <QtGui/qwhatsthis.h>
    #include <QtGui/qcheckbox.h>
    #include <QtGui/qmessagebox.h>
#endif
#include <stdlib.h>

static QString LIG_FILENAME;
static QString DTA_FILENAME;
static QString SMB_PATH;


/*
 *  Constructs a ViewReadLinetree which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */


 class DblClickEater : public QObject
    {
    public:
     DblClickEater(QLineEdit * target):
        QObject(target),_target(target){}

    protected:
        bool eventFilter( QObject *o, QEvent *e ){
            if ( e->type() == QEvent::MouseButtonDblClick ) {
                if(_target-> echoMode() != QLineEdit::Password)_target->setEchoMode(QLineEdit::Password);
                else _target->setEchoMode(QLineEdit::Normal);
                return true; // eat event
            } else return false;
        }

        QLineEdit * _target;
    };


ViewReadLinetree::ViewReadLinetree( bool open, QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl )
    : QDialog( parent, fl ),
        __open(open)
{
    setModal(modal);
    if ( !name ) setObjectName( "ViewReadLinetree" );
    resize( 500, 250 );
    if(open)setWindowTitle( tr( "Read Linetree" ) );
    else setWindowTitle( tr( "Save Linetree" ) );

    buttonLig = new QToolButton( this );
    buttonLig->setGeometry( QRect( 450, 10, 40, 21 ) );
    buttonLig->setText( tr( "..." ) );
    QObject::connect( buttonLig,SIGNAL(clicked()),this,SLOT(SelectLigFile()));

    QLabel * labelDta = new QLabel( this );
    labelDta->setGeometry( QRect( 10, 60, 61, 20 ) );
    labelDta->setText( tr( "Dta File" ) );

    buttonDta = new QToolButton( this );
    buttonDta->setGeometry( QRect( 450, 60, 40, 21 ) );
    buttonDta->setText( tr( "..." ) );
    QObject::connect( buttonDta,SIGNAL(clicked()),this,SLOT(SelectDtaFile()));

    QLabel * labelSmb = new QLabel( this);
    labelSmb->setGeometry( QRect( 10, 110, 61, 20 ) );
    labelSmb->setText( tr( "SMB Path" ) );

    buttonSmb = new QToolButton( this );
    buttonSmb->setGeometry( QRect( 450, 110, 40, 21 ) );
    buttonSmb->setText( tr( "..." ) );
    QObject::connect( buttonSmb,SIGNAL(clicked()),this,SLOT(SelectSMBPath()));

    QLabel * labelLig = new QLabel( this );
    labelLig->setGeometry( QRect( 10, 10, 61, 20 ) );
    labelLig->setText( tr( "Lig File" ) );

    editLig = new QLineEdit( this );
    editLig->setGeometry( QRect( 100, 10, 350, 22 ) );

    editDta = new QLineEdit( this );
    editDta->setGeometry( QRect( 100, 60, 350, 22 ) );
        if(!DTA_FILENAME.isEmpty())editDta->setText( DTA_FILENAME );

    editSmb = new QLineEdit( this );
    editSmb->setGeometry( QRect( 100, 110, 350, 22 ) );
    if(SMB_PATH.isEmpty())    {
      SMB_PATH = (TOOLS(getPlantGLDir())+"share/plantgl/SMBFiles").c_str() ;
    }
    if(!SMB_PATH.isEmpty())editSmb->setText( SMB_PATH  );

    labelKey = new QLabel( this );
    labelKey->setGeometry( QRect( 10, 160, 89, 20 ) );
    if(open)labelKey->setText( tr( "Key :" ) );
        else labelKey->setText("Key : [AMAPmod]");

    editKey = new QLineEdit( this );
    editKey->setGeometry( QRect( 100, 160, 350, 22 ) );
    editKey->setReadOnly(open);
    editKey->setFont(QFont("courier", 8));

    editKey->setEchoMode(QLineEdit::Password);
    if(!open)editKey->setText(PGL(LinetreePrinter)::PROTECTION.c_str());
    QObject::connect( editKey,SIGNAL(textChanged(const QString&)),this,SLOT(checkKey(const QString&)));
    DblClickEater * dblCkEater = new DblClickEater(editKey);
    labelKey->installEventFilter( dblCkEater );

        if(!open){
                QToolButton * buttonKey = new QToolButton( this );
                buttonKey->setGeometry( QRect( 450, 160, 40, 21 ) );
                buttonKey->setText( tr( "Reset" ) );
                QObject::connect( buttonKey,SIGNAL(clicked()),this,SLOT(resetKey()));
        }


    buttonCancel = new QPushButton( this );
    buttonCancel->setGeometry( QRect( 390, 200, 104, 28 ) );
    buttonCancel->setText( tr( "&Cancel" ) );
    QObject::connect( buttonCancel,SIGNAL(clicked()),this,SLOT(reject()));

    buttonOk = new QPushButton( this );
    buttonOk->setGeometry( QRect( 280, 200, 104, 28 ) );
    buttonOk->setText( tr( "&Ok" ) );
    QObject::connect( buttonOk,SIGNAL(clicked()),this,SLOT(ok()));

    endianBox = new QCheckBox( this );
    endianBox->setGeometry( QRect( 10, 205, 90, 20 ) );
    endianBox->setText( tr( "Big Endian" ) );
    endianBox->setChecked(true);

    if(open){
    QPushButton * buttonTestEndianess = new QPushButton( this );
    buttonTestEndianess->setGeometry( QRect( 100, 200, 54, 28 ) );
    buttonTestEndianess->setText( tr( "Test" ) );
    QObject::connect( buttonTestEndianess,SIGNAL(clicked()),this,SLOT(testEndianess()));
    }

    if(!LIG_FILENAME.isEmpty()){
       editLig->setText( LIG_FILENAME );
       checkKey();
    }

}

/*
 *  Destroys the object and frees any allocated resources
 */
ViewReadLinetree::~ViewReadLinetree()
{
    // no need to delete child widgets, Qt does it all for us
}

void ViewReadLinetree::ok()
{
  if(!getSMBPath().isEmpty() && getSMBPath()!=SMB_PATH)SMB_PATH = getSMBPath();
//  if(SMB_PATH != TOOLS(getSymbolDir()).c_str()){
//       TOOLS(setSymbolDir(SMB_PATH.toAscii().constData()));
//  }
  LIG_FILENAME = getLigFile();
  DTA_FILENAME = getDtaFile();
  accept();
}

QString ViewReadLinetree::getLigFile() const{
    return editLig->text();
}

QString ViewReadLinetree::getDtaFile() const{
    return editDta->text();
}

QString ViewReadLinetree::getSMBPath() const{
    return editSmb->text();
}

bool ViewReadLinetree::bigEndian() const {
    return endianBox->isChecked();
}


QString ViewReadLinetree::getKey() const{
    return editKey->text();
}

void ViewReadLinetree::resetKey(){
        labelKey->setText("Key : [AMAPmod]");
        editKey->setText(PGL(LinetreePrinter)::PROTECTION.c_str());
}

void ViewReadLinetree::checkKey(const QString&){
        checkKey();
}

void ViewReadLinetree::checkKey(){
  if(__open){
    if(!getLigFile().isEmpty() && QFile::exists(getLigFile())){
      QFile f (getLigFile());
      if(f.open(QIODevice::ReadOnly) ) {
        QDataStream stream(&f);
        if(bigEndian())stream.setByteOrder(QDataStream::BigEndian);
        else stream.setByteOrder(QDataStream::LittleEndian);
        char t[81];
        stream.readRawData(t,80);
        t[80] = '\0';
        QString key = t;
        editKey->setText( key );
        if(key == PGL(LinetreePrinter)::PROTECTION.c_str()){
          labelKey->setText("Key : [AMAPmod]");
        }
        else {
          labelKey->setText("Key : ");
        }
        f.close();
      }
    }
    else {
      editKey->setText("");
      labelKey->setText("Key : ");
    }
  }
  else {
    if(editKey->text() == PGL(LinetreePrinter)::PROTECTION.c_str()){
      labelKey->setText("Key : [AMAPmod]");
    }
    else {
      labelKey->setText("Key : ");
    }
  }
}

void ViewReadLinetree::checkCfg(const QString& path){
  if(path.isEmpty())return;
  QString cfg = path;
  if(cfg[-1] != '/' && cfg[-1] != '\\' )cfg += '/';
  cfg += ".cfg";
  if(QFileInfo(cfg).exists()){
    QFile c(cfg);
    if(c.open(QIODevice::ReadOnly)){
      QTextStream s(&c);
      QString l,p;
      while(!s.atEnd()&&p.isEmpty()){
        l = s.readLine();
        if(l.contains("SYMBOLES =") == 1){
          p = l.mid(l.indexOf('=')+1);
          p = p.simplified();
          int i = 0;
          while(p[i] == ' ')i++;
          if(i!=0) p = p.mid(i);
        }
      }
      if(!p.isEmpty()){
        if(QDir::isRelativePath(p)){
          if(path[-1] != '/' && path[-1] != '\\' )p = '/' + p;
          p = path + p;
          p = QDir::cleanPath(p);
        }
        if(QDir(p).exists()){
          setSMBPath(p);
        }
      }
    }
  }
}

void ViewReadLinetree::setLigFile( const QString& f ){
  editLig->setText(f);
  if(__open){
    QString dta = f;
    dta = dta.replace(QRegularExpression("\\.lig"),".dta");
    if(QFileInfo(dta).exists()){
        setDtaFile(dta);
    }
    else {
      dta = f;
      dta = dta.replace(QRegularExpression("[0-9]*\\.lig"),".dta");
      if(QFileInfo(dta).exists())
        setDtaFile(dta);
    }
  }
  else {
    QString dta = f;
    dta = dta.replace(QRegularExpression("[0-9]*\\.lig"),".dta");
    setDtaFile(dta);
  }
  testEndianess();
  checkCfg(QFileInfo(f).dir().path());
}

void ViewReadLinetree::setDtaFile( const QString& f ){
    editDta->setText(f);
}

void ViewReadLinetree::setSMBPath( const QString& f ){
    SMB_PATH = f;
    editSmb->setText(f);
}

void ViewReadLinetree::setEndianess( bool bigEndian ){
   endianBox->setChecked(bigEndian);
   checkKey();
}

void ViewReadLinetree::SelectLigFile(){

  QString initial;
  if(!editLig->text().isEmpty())initial = editLig->text();
  else if(!editDta->text().isEmpty())initial = editDta->text();
  else if(!editSmb->text().isEmpty())initial = editSmb->text();

  QString _filename;
  if(__open)_filename =  QFileDialog::getOpenFileName ( this, "Open Lig File", initial,"Lig Files (*.lig)"  );
  else _filename =  QFileDialog::getSaveFileName ( this, "Save Lig File", initial,"Lig Files (*.lig)" );
  if(!_filename.isEmpty())
    setLigFile(_filename);
}

void ViewReadLinetree::SelectDtaFile(){
  QString initial;
  if(!editDta->text().isEmpty())initial = editDta->text();
  else if(!editLig->text().isEmpty())initial = editLig->text();
  else if(!editSmb->text().isEmpty())initial = editSmb->text();

  QString _filename;
  if(__open)_filename =  QFileDialog::getOpenFileName ( this, "Open Dta File",initial, "Dta Files (*.dta)"  );
  else _filename =  QFileDialog::getSaveFileName ( this, "Save Dta File", initial,"Dta Files (*.dta)" );
  if(!_filename.isEmpty())
    setDtaFile(_filename);
}

void ViewReadLinetree::SelectSMBPath(){
  QString dir;
  if(!editSmb->text().isEmpty())dir = editSmb->text();
  else if(!editLig->text().isEmpty())dir = (editLig->text().left(editDta->text().lastIndexOf('/')));
  else if(!editDta->text().isEmpty())dir = (editDta->text().left(editDta->text().lastIndexOf('/')));

  QString path;
  path = QFileDialog::getExistingDirectory(this,QString("Select SMB Path"),dir);
  if(!path.isEmpty())
    setSMBPath(path);
}

void ViewReadLinetree::testEndianess(){
    bool bigendian = true;
    if(!editLig->text().isEmpty()){
        if(!PGL(Ligfile::isBigEndian(qPrintable(editLig->text()))))
            bigendian = false;
    }
    setEndianess(bigendian);
}

