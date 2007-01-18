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


/*! \file view_daemon.h
    \brief Definition of the viewer daemon.
*/


#ifndef __view_daemon_h__
#define __view_daemon_h__

class QString;
class QLabel;
class QTextView;
class QPushButton;
class QSocket;
#include <qserversocket.h>
#include <qnetworkprotocol.h>
#include <qdialog.h>
#include "geom_namespace.h"
/* ----------------------------------------------------------------------- */


class ViewerDaemon : public QServerSocket
{
    Q_OBJECT
public:
    ViewerDaemon( QObject* parent=0 );

    void newConnection( int socket );

signals:
    void newConnect();
    void endConnect();
    void wroteToClient();

    void receiveRequest(const QString&);
    void requestReadFile(const QString&);
    void requestAddFile(const QString&);
    void requestShow();

private slots:
    void readClient();
    void discardClient();

};

/* ----------------------------------------------------------------------- */

class ViewClient : public QNetworkProtocol
{
    Q_OBJECT

public:
    ViewClient();
    virtual ~ViewClient();
    virtual int supportedOperations() const;

protected:
    virtual void operationGet( QNetworkOperation *op );
    virtual void operationPut( QNetworkOperation *op );

    QSocket *commandSocket;
    bool connectionReady, passiveMode;

private:
    bool checkConnection( QNetworkOperation *op );
    void close();

protected slots:
    void hostFound();
    void connected();
    void closed();
    void readyRead();
    void slotError( int err );
};

/* ----------------------------------------------------------------------- */


class GEOM_API ViewClientObj : public QObject
{
    Q_OBJECT
public:

    ViewClientObj(bool dialog = false,
					QObject * parent=0, 
				    const char * name=0);

    virtual ~ViewClientObj();
	bool requestReadFile(const QString&);
	bool requestAddFile(const QString&);
	bool requestShow();
	bool request(int argc, char ** argv);

	int exec();

protected slots:
  void finished( QNetworkOperation * );

  void downloaded( const QByteArray &, QNetworkOperation * );
  void connectionStatus ( int , const QString &  );
  void transferProgress ( int, int, QNetworkOperation * );

  void status( const QString& );

  void accept();
  void reject();
  void timeout();

signals :
  void accepted();
  void rejected();

protected:
  QTextView * __label;
  QDialog * __dialog;
  QPushButton * __bt;

  QUrlOperator * __urloperator;

  int res;
};

/* ----------------------------------------------------------------------- */

#endif
