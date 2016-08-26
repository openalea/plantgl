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

#include "errordialog.h"
#include "configuration.h"
#include "interface/messagedisplayer.h"
#include <QtGui/qevent.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qradiobutton.h>
#include <QtGui/qapplication.h>
#include <QtGui/qclipboard.h>
#include <QtGui/qmime.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qregexp.h>
#include <QtGui/qwhatsthis.h>
#include <QtGui/qtextbrowser.h>

#include <iostream>

/* ----------------------------------------------------------------------- */

static ViewErrorDialog * QT_ERROR_MESSAGE_DISPLAY = NULL;

void handleQtMessage(QtMsgType type, const char *msg){
	if(QT_ERROR_MESSAGE_DISPLAY)
		QT_ERROR_MESSAGE_DISPLAY->qtMessage(type,msg);
	else std::cerr << msg << std::endl;
}

void registerForQtMessage(ViewErrorDialog * display){
  QT_ERROR_MESSAGE_DISPLAY = display;
  qInstallMsgHandler(handleQtMessage);
}

void unregisterForQtMessage(){
	qInstallMsgHandler(0);
	QT_ERROR_MESSAGE_DISPLAY = NULL;
}

ViewErrorDialog::ViewErrorDialog( QWidget * parent) 
    : QDockWidget("Error Log", parent),
	 __verbose(
#ifdef PGL_DEBUG
	true
#else
	 false
#endif
	 ),
	 __displaylock(false)
{
  setObjectName("ErrorLog");
  QWidget * mwidget = new QWidget(this);
  setWidget(mwidget);
  __display = new Ui::MessageDisplayer();
  __display->setupUi(mwidget);
  __display->__text->setFont(QFont("courrier", 9 ));
  QObject::connect(__display->VerboseButton,SIGNAL(toggled(bool)),
					this,SLOT(setVerbose(bool)));
  registerForQtMessage(this);
  QObject::connect(__display->ClearButton,SIGNAL(clicked()),this,SLOT(clear()));

  ViewerSettings settings;
  settings.beginGroup("ErrorDialog");
#ifndef PGL_DEBUG
  __verbose = settings.value("Verbose",__verbose).toBool();
#endif
  __display->VerboseButton->setChecked(__verbose);
  __display->mAutoClearButton->setChecked(settings.value("AutoClear",__display->mAutoClearButton->isChecked()).toBool());
  __display->__popupButton->setChecked(settings.value("ErrorPopup",__display->__popupButton->isChecked()).toBool());
  settings.endGroup();

  qDebug("Registered for Qt message");
}


ViewErrorDialog::~ViewErrorDialog()
{
	unregisterForQtMessage();
}

void ViewErrorDialog::registerQtMsg(bool reg)
{
	if(reg)registerForQtMessage(this);
	else unregisterForQtMessage();
}

void ViewErrorDialog::setVerbose(bool b){
	if(__verbose != b){
		__verbose = b;
		if(__verbose){
			QObject::connect(QApplication::clipboard(),SIGNAL(dataChanged()),
			this,SLOT(clipboardInfo()));
			// registerForQtMessage(this);
		}
		else {
			QObject::disconnect(QApplication::clipboard(),SIGNAL(dataChanged()),
			this,SLOT(clipboardInfo()));
			// unregisterForQtMessage();
		}
	}
}

void 
ViewErrorDialog::__setText(const QString& t){
  if(__isLocked()){
#ifdef _DEBUG
//   if(__verbose)__listmessage.append(t);
#else
   ;
#endif
  }
  else {
	__lock();
	__display->__text->setText(t);
	QString t;
	if(!__listmessage.isEmpty()){
	  t = __listmessage.join("<BR>");
	  __listmessage.clear();
	}
	__unlock();
	if(!t.isEmpty())__append(t);
  }
}

void 
ViewErrorDialog::__append(const QString& t){
  if(__isLocked()){
#ifdef _DEBUG
//   if(__verbose)__listmessage.append(t);
#else
   ;
#endif
  }
  else {
	__lock();
	__display->__text->append(t);
	QString t;
	if(!__listmessage.isEmpty()){
	  t = __listmessage.join("<BR>");
	  __listmessage.clear();
	}
	__unlock();
	if(!t.isEmpty())__append(t);
  }
}

void 
ViewErrorDialog::__lock(){
#ifdef QT_THREAD_SUPPORT
  __mutexlock.lock();
#endif
  __displaylock = true;
#ifdef QT_THREAD_SUPPORT
  __mutexlock.unlock();
#endif
}

void 
ViewErrorDialog::__unlock()
{
#ifdef QT_THREAD_SUPPORT
  __mutexlock.lock();
#endif
  __displaylock = false;
#ifdef QT_THREAD_SUPPORT
  __mutexlock.unlock();
#endif
}

bool
ViewErrorDialog::__isLocked()
{
#ifdef QT_THREAD_SUPPORT
  bool displaylock;
  __mutexlock.lock();
  displaylock = __displaylock;
  __mutexlock.unlock();
  return displaylock;
#else
  return __displaylock;
#endif
}

void ViewErrorDialog::clear(){
    __display->__text->setText(QString::null);
}

void ViewErrorDialog::setText(char * &_text ){
  QString t(_text);
  if(t.length() < 5000){
	__setText(highligthText(t));
  }
  else __setText(t);
  if(__display->__popupButton->isChecked())show();   
}

void ViewErrorDialog::setText(const QString &_text ){
  if(_text.length() < 5000){
	QString t = _text;
	__setText(highligthText(t));
  }
  else __setText(_text);
  if(__display->__popupButton->isChecked())show();   
}

void ViewErrorDialog::setWarning(const QString &_text ){
  if(_text.length() < 5000){
	QString t = _text;
	__setText(highligthText(t));
  }
  else __setText(_text);
}

void ViewErrorDialog::setInfo(const QString &_text ){
  if(_text.length() < 5000){
	QString t = _text;
	__setText(highligthText(t));
  }
  else __setText(_text);
}

void ViewErrorDialog::setError(const QString &_text ){
  if(__display->__popupButton->isChecked())show();
  if(_text.length() < 5000){
	QString t = _text;
	__setText(highligthText(t));
  }
  else __setText(_text);
}

void ViewErrorDialog::appendText(char * &_text ){
	if(__display->__popupButton->isChecked())show();
    QString t(_text);
 	if(t.length() < 5000){
	  __append(highligthText(t));
	}
	else __append(t);
}

void ViewErrorDialog::appendText(const QString &_text ){
  if(__display->__popupButton->isChecked())show();
  if(_text.length() < 5000){
	QString t = _text;
	__append(highligthText(t));
  }
  else __append(_text);
}

void ViewErrorDialog::appendError(const QString &_text ){
  if(__display->__popupButton->isChecked())show();
  if(_text.length() < 5000){
	QString t = _text;
	__append(highligthText(t));
  }
  else __append(_text);
}

void ViewErrorDialog::appendWarning(const QString &_text ){
  if(_text.length() < 5000){
	QString t = _text;
	__append(highligthText(t));
  }
  else __append(_text);
}

void ViewErrorDialog::appendInfo(const QString &_text ){
  if(_text.length() < 5000){
	QString t = _text;
	__append(highligthText(t));
  }
  else __append(_text);
}



void ViewErrorDialog::keyPressEvent ( QKeyEvent * e){
	if( e->key() == Qt::Key_F3 || 
      e->key() == Qt::Key_Escape || 
      e->key() == Qt::Key_Home) hide();
}

void ViewErrorDialog::clipboardInfo()
{
	QStringList formats;
	QString text;
	QClipboard * clipboard = QApplication::clipboard();
	if(clipboard ){
		const QMimeData* data = clipboard->mimeData();
		formats = data->formats();
		if(data->hasText())text = data->text();
	}
	else { 
		__append("<B>*** Qt Info:</B> Clipboard not available.\n");
		return;
	}
	
	__append("<B>*** Qt Info:</B> Clipboard data changed.\n");
	
	if(!formats.isEmpty()){			
		__append("<B>*** Qt Info:</B> Clipboard data format : \""+formats.join("\" , \"")+"\".\n");
	}
	if(!text.isEmpty())
		__append("<B>*** Qt Info:</B> Clipboard Value : "
		+text+"\n");
}

void ViewErrorDialog::qtMessage(QtMsgType type, const char *msg )
{
	switch ( type ) {
	case QtDebugMsg:
#ifdef _DEBUG
		std::cerr << "*** Qt Debug: " << msg << std::endl;
#else
		if(__verbose)	
#endif
		__append("<B>*** Qt Debug:</B> "+QString(msg)+"\n");
		break;
	case QtWarningMsg:
#ifdef _DEBUG
		std::cerr << "*** Qt Warning: " << msg << std::endl;
#else
		if(__verbose)	
#endif
		if(__display->__popupButton->isChecked())show();
		__append("<B>*** Qt Warning:</B> "+QString(msg)+"\n");
		break;
    case QtCriticalMsg:
        std::cerr << "*** Qt Critical: " << msg << std::endl;
        show();
        __append("<B>*** Qt Critical:</B> "+QString(msg)+"\n");
        break;
	case QtFatalMsg:
		std::cerr << "*** Qt Fatal: " << msg << std::endl;
		show();
		__append("<B>*** Qt Fatal:</B> "+QString(msg)+"\n");
		abort();
        break;
    default:
        break;
	}
}

QString& ViewErrorDialog::highligthText(QString&t){
	t.replace(QRegExp("<"),"&lt;");
	t.replace(QRegExp(">"),"&gt;");
	t.replace(QRegExp("\\*\\*\\*[ \\t]*[Ww][Aa][Rr][Nn][Ii][Nn][Gg][ \\t]*:"),
			  "<B>*** Warning :</B>");
	t.replace(QRegExp("\\*\\*\\*[ \\t]*[Cc][Oo][Mm][Mm][Ee][Nn][Tt][ \\t]*:"),"<B>*** Comment :</B>");
	t.replace(QRegExp("\\*\\*\\*[ \\t]*[Ee][Rr][Rr][Oo][Rr][ \\t]*:"),"<B>*** Error :</B>");
	t.replace(QRegExp("\\*\\*\\*[ \\t]*[Pp][Aa][Rr][Ss][Ee][Rr][ \\t]*:"),"<B>*** Parser :</B>");
	t.replace(QRegExp("\\*\\*\\*[ \\t]*[Ss][Cc][Aa][Nn][Nn][Ee][Rr][ \\t]*:"),"<B>*** Scanner :</B>");
	t.replace(QRegExp("\\n"),"<BR>");
	return t;
}
