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


/* ----------------------------------------------------------------------- */

#include "view_daemon.h"
#include <qtextstream.h>
#include <qurloperator.h>
#include <qnetwork.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qapplication.h>
#include <qtextview.h>
#include <qsocket.h>
#include <qfileinfo.h>
#include <qregexp.h>
#include <qtimer.h>

/* ----------------------------------------------------------------------- */

ViewerDaemon::ViewerDaemon( QObject* parent ) :
QServerSocket(7777,1,parent)
{
  if ( !ok() ) {
	qWarning("Failed to bind to port 7777");
  }
}

void 
ViewerDaemon::newConnection( int socket )
{
  // When a new client connects, the server constructs a QSocket and all
  // communication with the client is done over this QSocket. QSocket
  // works asynchronouslyl, this means that all the communication is done
  // in the two slots readClient() and discardClient().
  QSocket* s = new QSocket( this );
  connect( s, SIGNAL(readyRead()), this, SLOT(readClient()) );
  connect( s, SIGNAL(delayedCloseFinished()), this, SLOT(discardClient()) );
  s->setSocket( socket );
  emit newConnect();
}

void ViewerDaemon::readClient()
{
  QSocket* socket = (QSocket*)sender();
  if ( socket->canReadLine() ) {
	QString line = socket->readLine();
	emit receiveRequest(line);
	QStringList tokens = QStringList::split( QRegExp("[ \r\n][ \r\n]*"), line );
	if ( tokens[0] == "GET" ) {
		QTextStream os( socket );
		os.setEncoding( QTextStream::UnicodeUTF8 );
		os << "Accepted\r\n";
		socket->close();
		emit wroteToClient();


		tokens.remove(tokens.first());
		QString file = tokens.join(" ");

		if(file[0]=='/')file = file.right(file.length()-1);
		QUrl::decode(file);
		QStringList tokens = QStringList::split( QRegExp(" "), file );
		QString code = tokens[0];
		if(code == "SHOW")emit requestShow();
		else {
			tokens.remove(tokens.first());
			file = tokens.join(" ");
			if(code == "READ")
				emit requestReadFile(file);
			else if(code == "ADD")
				emit requestAddFile(file);
			else {
				emit receiveRequest("Unvalid request command : '"+code+"'");
				emit requestShow();
			}
		}
	}
	else {
		QTextStream os( socket );
		os.setEncoding( QTextStream::UnicodeUTF8 );
		os << "Rejected\r\n";
		socket->close();
		emit wroteToClient();
	}
  }
}

void ViewerDaemon::discardClient()
{
  QSocket* socket = (QSocket*)sender();
  delete socket;
  emit endConnect();
}

/* ----------------------------------------------------------------------- */
ViewClient::ViewClient()
    : QNetworkProtocol(), connectionReady( FALSE )
{
    commandSocket = new QSocket( this );

    connect( commandSocket, SIGNAL( hostFound() ),
	     this, SLOT( hostFound() ) );
    connect( commandSocket, SIGNAL( connected() ),
	     this, SLOT( connected() ) );
    connect( commandSocket, SIGNAL( connectionClosed() ),
	     this, SLOT( closed() ) );
    connect( commandSocket, SIGNAL( readyRead() ),
	     this, SLOT( readyRead() ) );
    connect( commandSocket, SIGNAL( error( int ) ),
	    this, SLOT( slotError( int ) ) );
/*    connect( commandSocket, SIGNAL( bytesWritten( int ) ),
	    this, SLOT( slotBytesWritten( int ) ) );*/
}

ViewClient::~ViewClient()
{
    close();
    delete commandSocket;
}

void ViewClient::operationPut( QNetworkOperation * n)
{
    QString cmd = "POST ";
	cmd += QString(n->rawArg(0));
//    cmd += url()->encodedPathAndQuery();
	// cmd.replace(cmd.find("/",1," "));
    cmd += "\r\n";
    commandSocket->writeBlock( cmd.latin1(), cmd.length() );
}

void ViewClient::operationGet( QNetworkOperation * )
{
    QString cmd = "GET ";
    cmd += url()->encodedPathAndQuery();
    cmd += "\r\n";
    commandSocket->writeBlock( cmd.latin1(), cmd.length() );
}

bool ViewClient::checkConnection( QNetworkOperation * )
{
    if ( !commandSocket->peerName().isEmpty() && connectionReady )
	return TRUE;

    if ( !commandSocket->peerName().isEmpty() )
	return FALSE;

    if ( commandSocket->state() == QSocket::Connecting )
	return FALSE;

    connectionReady = FALSE;
    commandSocket->connectToHost( url()->host(),
				  url()->port() != -1 ? url()->port() : 7777 );

    return FALSE;
}

void ViewClient::close()
{
    if ( !commandSocket->peerName().isEmpty() ) {
 	commandSocket->writeBlock( "quit\r\n", strlen( "quit\r\n" ) );
 	commandSocket->close();
    }
}

int ViewClient::supportedOperations() const
{
    return OpGet | OpPut;
}

void ViewClient::hostFound()
{
    if ( url() )
	emit connectionStateChanged( ConHostFound, tr( "Host %1 found" ).arg( url()->host() ) );
    else
	emit connectionStateChanged( ConHostFound, tr( "Host found" ) );
}

void ViewClient::connected()
{
    if ( url() )
	emit connectionStateChanged( ConConnected, tr( "Connected to host %1" ).arg( url()->host() ) );
    else
	emit connectionStateChanged( ConConnected, tr( "Connected to host" ) );
    connectionReady = TRUE;
}

void ViewClient::closed()
{
    if ( url() )
	emit connectionStateChanged( ConClosed, tr( "Connection to %1 closed" ).arg( url()->host() ) );
    else
	emit connectionStateChanged( ConClosed, tr( "Connection closed" ) );

    connectionReady = FALSE;
    emit finished( operationInProgress() );

}

void ViewClient::readyRead()
{
    QByteArray s;
    s.resize( commandSocket->bytesAvailable() );
    commandSocket->readBlock( s.data(), commandSocket->bytesAvailable() );
    emit data( s, operationInProgress() );
}

void ViewClient::slotError( int err )
{
  QNetworkOperation* op = operationInProgress();
  if(op){
	op->setState(QNetworkProtocol::StFailed);
	op->setErrorCode(err);
	emit finished(op);
  }
  else finished(NULL);
  close();
}

/* ----------------------------------------------------------------------- */





ViewClientObj::ViewClientObj(bool dialog,
							 QObject * parent, 
							 const char * name):
  QObject(parent,name),
  __label(0),
  __dialog(0),
  __urloperator(0),
  res(0){
  qInitNetworkProtocols();
  QNetworkProtocol::registerNetworkProtocol( "pgl", 
	new QNetworkProtocolFactory<ViewClient> );

  __urloperator = new QUrlOperator;
  connect( __urloperator, SIGNAL( finished( QNetworkOperation * ) ),
     this, SLOT( finished( QNetworkOperation *  ) ) );

  if(dialog){
  __dialog = new QDialog(NULL,"Request",true);
  __dialog->setGeometry(100,100,300,105);
  __label = new QTextView(__dialog,"Label");
  __label->setGeometry(10,5,280,70);
  __label->setText("Lookup Existing Viewer ...");
  __bt = new QPushButton("Cancel",__dialog);
  __bt->setGeometry(115,77,97,28);
  connect( __bt, SIGNAL( clicked() ), __dialog, SLOT( reject() ) );
  connect( this, SIGNAL( rejected() ), __dialog, SLOT( reject() ) );
  connect( this, SIGNAL( accepted() ), __dialog, SLOT( accept() ) );

  connect( __urloperator, SIGNAL( data( const QByteArray &, QNetworkOperation * ) ),
     this, SLOT( downloaded( const QByteArray &, QNetworkOperation * ) ) );
  connect( __urloperator, SIGNAL( connectionStateChanged( int , const QString &  ) ),
     this, SLOT( connectionStatus ( int , const QString &  ) ) );
  connect( __urloperator, SIGNAL( dataTransferProgress ( int, int, QNetworkOperation * ) ),
	  this, SLOT( transferProgress ( int, int, QNetworkOperation * ) ) );
  }

}

ViewClientObj::~ViewClientObj(){
  delete __urloperator;
  delete __dialog;
}

bool 
ViewClientObj::request(int argc, char ** argv)
{
  switch(argc){
  case 1:
	return requestShow();
  case 2:{
	QString option = argv[1];
	if(option[0] != '-')
	  return requestReadFile(option);
	else return false;
		 }
	break;
  case 3:{
	QString option = argv[1];
	if(option == "-a" || option == "--add" )
	  return requestAddFile(argv[2]);
	else return false;
		 }
  default:
	return false;
  }
}

bool 
ViewClientObj::requestReadFile(const QString& file){
   *__urloperator = QUrlOperator("pgl://localhost/READ "+QFileInfo(file).absFilePath());
   if(__urloperator->get()==NULL){
	 return false;
   }
   if(exec() == 1)return true;
   else return false;
}

bool 
ViewClientObj::requestAddFile(const QString& file){
   *__urloperator = QUrlOperator("pgl://localhost/ADD "+QFileInfo(file).absFilePath()); 
  if(__urloperator->get()==NULL)return false;
  if(exec()==1)return true;
  else return false;
}

bool 
ViewClientObj::requestShow(){
   *__urloperator = QUrlOperator("pgl://localhost/SHOW"); 
  if(__urloperator->get()==NULL)return false;
  if(exec()==1)return true;
  else return false;
}

void 
ViewClientObj::finished( QNetworkOperation * op )
{
  if(op){
	QString details = op->protocolDetail();
	if(op->state() == QNetworkProtocol::StFailed){
	  status(tr("Network Error :")+details);
	  reject();
	}
	else if(op->state() == QNetworkProtocol::StStopped){
	  status("Transfert Stopped :"+details);
	  reject();
	}
	else if(op->state() == QNetworkProtocol::StDone || 
	  op->state() == QNetworkProtocol::StWaiting){
	  status("Transfert Done :"+details);
	  accept();
	}
  }
  else reject();
}

void
ViewClientObj::downloaded( const QByteArray & ba,
								  QNetworkOperation * op ){
  QString msg(ba);
  if(msg != "Accepted\r\n"){
	status("Rejection");
	res = -1;
  }
  status("Msg :'"+msg+"'");
}

void 
ViewClientObj::connectionStatus ( int i, const QString & msg )
{
  status(msg);
}

void
ViewClientObj::transferProgress ( int i, int j, QNetworkOperation * ) 
{
  status(tr("Transfert progress")+" :"+QString::number((100*i)/j)+"%.");
}

void 
ViewClientObj::status( const QString& t){
  if(__label)__label->append(t);
}

int 
ViewClientObj::exec(){
  QTimer::singleShot(3000,this,SLOT(timeout()));
  if(__dialog) __dialog->exec();
  else qApp->exec();
  return res;
}

void ViewClientObj::accept(){
  if(!__dialog){
	qApp->quit();
	if(res==0)res = 1;
  }
  else {
	if(res==0){
	  res = 1;
	  __bt->setText("Ok");
	  status("Accepted.");
	  disconnect( __bt, SIGNAL( clicked() ), __dialog, SLOT( reject() ) );
	  connect( __bt, SIGNAL( clicked() ), __dialog, SLOT( reject() ) );
//	emit accepted();
	}
	else {
	__bt->setText("Dismiss");
	  status("Rejected.");
//	emit rejected();
	}
  }
}

void ViewClientObj::reject(){
  if(!__dialog){
	qApp->quit();
	res = -1;
  }
  else {
	res = -1;
	status("Rejected.");
	__bt->setText("Dismiss");
//	emit rejected();
  }
}

void ViewClientObj::timeout(){
  if(res==0){
	status("Timeout!");
	reject();
  }
}

/* ----------------------------------------------------------------------- */
/*
bool request(int argc, char ** argv)
{

  if(argc == 2){
	QString option = argv[1];
	if(option[0] != '-'){
	  QUrlOperator url = "pgl://localhost/"+QFileInfo(option).absFilePath();
	  url.get();
	  return true;
	}
  }
  return false;
}*/
/* ----------------------------------------------------------------------- */
