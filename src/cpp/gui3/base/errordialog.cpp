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
#ifdef DEBUG
#error Debug Mode
#endif

#include "errordialog.h"
#include "interface/messagedisplayer.h"
#include <qevent.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <qmime.h>
#include <qiconview.h>
#include <qdragobject.h> 
#include <qstringlist.h>
#include <qregexp.h>
#include <qwhatsthis.h>
#include <qtextbrowser.h>

#include <iostream>

/* ----------------------------------------------------------------------- */

static ViewErrorDialog3 * QT_ERROR_MESSAGE_DISPLAY = NULL;

void handleQtMessage(QtMsgType type, const char *msg){
	if(QT_ERROR_MESSAGE_DISPLAY)
		QT_ERROR_MESSAGE_DISPLAY->qtMessage(type,msg);
	else std::cerr << msg << std::endl;
}

void registerForQtMessage(ViewErrorDialog3 * display){
  QT_ERROR_MESSAGE_DISPLAY = display;
  qInstallMsgHandler(handleQtMessage);
}

void unregisterForQtMessage(){
	qInstallMsgHandler(0);
	QT_ERROR_MESSAGE_DISPLAY = NULL;
}

ViewErrorDialog3::ViewErrorDialog3( QWidget * parent, const char * name, bool modal) 
    : ViewDialog3(parent,name, modal),
	 __verbose(
// #ifdef DEBUG
// 	 true
// #else
	 false
//#endif
	 ),
	 __displaylock(false)
{
  setCaption(tr("Viewer Error Dialog"));
  QWhatsThis::add(this,tr("<b>Viewer Error Dialog</b><br><br>"
	"This dialog displays all the error messages and debugging informations."
	));

#if QT_VERSION >= 220
  setSizeGripEnabled(TRUE);
#endif
  __display = new MessageDisplayer(this,"Error Dialog");
  __display->__text->setFont(QFont("courrier", 9 ));
  __display->VerboseButton->setChecked(__verbose);
  QObject::connect(__display->VerboseButton,SIGNAL(toggled(bool)),
					this,SLOT(setVerbose(bool)));
  resize( __display->size());
  if(__verbose)
  QObject::connect(QApplication::clipboard(),SIGNAL(dataChanged()),
				   this,SLOT(clipboardInfo()));
  registerForQtMessage(this);
  QObject::connect(__display->OkButton,SIGNAL(clicked()),this,SLOT(hide()));
  QObject::connect(__display->CancelButton,SIGNAL(clicked()),this,SLOT(hide()));
  QObject::connect(__display->ClearButton,SIGNAL(clicked()),this,SLOT(clear()));
}


ViewErrorDialog3::~ViewErrorDialog3()
{
	unregisterForQtMessage();
}

void ViewErrorDialog3::registerQtMsg(bool reg)
{
	if(reg)registerForQtMessage(this);
	else unregisterForQtMessage();
}

void ViewErrorDialog3::setVerbose(bool b){
	if(__verbose != b){
		__verbose = b;
		if(__verbose)
			QObject::connect(QApplication::clipboard(),SIGNAL(dataChanged()),
			this,SLOT(clipboardInfo()));
		else 
			QObject::disconnect(QApplication::clipboard(),SIGNAL(dataChanged()),
			this,SLOT(clipboardInfo()));
	}
}

void 
ViewErrorDialog3::__setText(const QString& t){
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
ViewErrorDialog3::__append(const QString& t){
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
ViewErrorDialog3::__lock(){
#ifdef QT_THREAD_SUPPORT
  __mutexlock.lock();
#endif
  __displaylock = true;
#ifdef QT_THREAD_SUPPORT
  __mutexlock.unlock();
#endif
}

void 
ViewErrorDialog3::__unlock()
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
ViewErrorDialog3::__isLocked()
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

void ViewErrorDialog3::clear(){
    __display->__text->setText(QString::null);
}

void ViewErrorDialog3::setText(char * &_text ){
  QString t(_text);
  if(t.length() < 5000){
	__setText(highligthText(t));
  }
  else __setText(t);
  if(__display->__popupButton->isChecked())show();   
}

void ViewErrorDialog3::setText(const QString &_text ){
  if(_text.length() < 5000){
	QString t = _text;
	__setText(highligthText(t));
  }
  else __setText(_text);
  if(__display->__popupButton->isChecked())show();   
}

void ViewErrorDialog3::setWarning(const QString &_text ){
  if(_text.length() < 5000){
	QString t = _text;
	__setText(highligthText(t));
  }
  else __setText(_text);
}

void ViewErrorDialog3::setInfo(const QString &_text ){
  if(_text.length() < 5000){
	QString t = _text;
	__setText(highligthText(t));
  }
  else __setText(_text);
}

void ViewErrorDialog3::setError(const QString &_text ){
  if(__display->__popupButton->isChecked())show();
  if(_text.length() < 5000){
	QString t = _text;
	__setText(highligthText(t));
  }
  else __setText(_text);
}

void ViewErrorDialog3::appendText(char * &_text ){
	if(__display->__popupButton->isChecked())show();
    QString t(_text);
 	if(t.length() < 5000){
	  __append(highligthText(t));
	}
	else __append(t);
}

void ViewErrorDialog3::appendText(const QString &_text ){
  if(__display->__popupButton->isChecked())show();
  if(_text.length() < 5000){
	QString t = _text;
	__append(highligthText(t));
  }
  else __append(_text);
}

void ViewErrorDialog3::appendError(const QString &_text ){
  if(__display->__popupButton->isChecked())show();
  if(_text.length() < 5000){
	QString t = _text;
	__append(highligthText(t));
  }
  else __append(_text);
}

void ViewErrorDialog3::appendWarning(const QString &_text ){
  if(_text.length() < 5000){
	QString t = _text;
	__append(highligthText(t));
  }
  else __append(_text);
}

void ViewErrorDialog3::appendInfo(const QString &_text ){
  if(_text.length() < 5000){
	QString t = _text;
	__append(highligthText(t));
  }
  else __append(_text);
}


void ViewErrorDialog3::resizeEvent( QResizeEvent * event){
  if(isVisible()){
    int x = event->size().width() - event->oldSize().width();
    int y = event->size().height() - event->oldSize().height();
    __display->resize( event->size().width(),event->size().height()  );
    __display->__text->resize(   __display->__text->size() + QSize(x,y) );
    __display->OkButton->move(__display->OkButton->pos()+QPoint(x,y));
    __display->CancelButton->move(__display->CancelButton->pos()+QPoint(x,y));
    __display->ClearButton->move(__display->ClearButton->pos()+QPoint(x,y));
  }
}


void ViewErrorDialog3::keyPressEvent ( QKeyEvent * e){
  if( e->key() == Key_F3 || 
      e->key() == Key_Escape || 
      e->key() == Key_Home) hide();
}

void ViewErrorDialog3::clipboardInfo()
{
	QStringList formats;
	QString text;
	QClipboard * clipboard = QApplication::clipboard();
	if(clipboard ){
		QMimeSource* data = clipboard->data();
		if(data!=NULL /*&& qApp->activeWindow() !=NULL */ ){
			int i = 0;
			while(data->format(i)!=NULL){
				formats.append(data->format(i));i++; }
			if(QUriDrag::canDecode(data)){
				QStringList res;
				if(QUriDrag::decodeToUnicodeUris(data,res))
				  if(!res.isEmpty()){
					text = QString::number(res.count())+" :: \"" ;
					text += res.join("\" , \"") +'"';
				  }
				  else text = "Empty File : \""+clipboard->text()+"\"";
				else text = "(Cannot Decode)";
			}
			else if(QTextDrag::canDecode(data))
				text = "=\""+clipboard->text()+'"';
			else if(QImageDrag::canDecode(data))text = "(Image)";
			else if(QIconDrag::canDecode(data))text = "(Icon)";
			else if(QColorDrag ::canDecode(data))text = "(Color)";
			else text = "(Cannot Decode)";
		}
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

void ViewErrorDialog3::qtMessage(QtMsgType type, const char *msg )
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
	case QtFatalMsg:
		std::cerr << "*** Qt Fatal: " << msg << std::endl;
		show();
		__append("<B>*** Qt Fatal:</B> "+QString(msg)+"\n");
		abort();
	}
}

QString& ViewErrorDialog3::highligthText(QString&t){
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
