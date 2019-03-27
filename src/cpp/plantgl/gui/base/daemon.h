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
#include <QtNetwork/qtcpserver.h>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qdialog.h>
#else
    #include <QtGui/qdialog.h>
#endif

#include "../gui_config.h"
/* ----------------------------------------------------------------------- */

// #define VIEW_NETWORK_SUPPORT


class VIEW_API ViewerDaemon : public QTcpServer
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
    void processNextConnection();

protected:
    QTcpSocket * currentSocket;

};

/* ----------------------------------------------------------------------- */

#ifdef VIEW_NETWORK_SUPPORT

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtNetwork/qhttp.h>
#else
    #include <QtNetwork/QNetworkAccessManager>
#endif

class VIEW_API ViewClient : public
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    QHttp
#else
    QNetworkAccessManager
#endif

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


class VIEW_API ViewClientObj : public QObject
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

#endif
