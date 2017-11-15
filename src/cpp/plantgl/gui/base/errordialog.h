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

/*! \file view_errordialog.h
    \brief Definition of the viewer class ViewErrorDialog.
*/

#ifndef __view_errordialog_h__
#define __view_errordialog_h__

/* ----------------------------------------------------------------------- */

#include "../gui_config.h"
#include <QtGlobal>
#include <QtCore/qstringlist.h>
#include <QtCore/qmutex.h>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0) 
    #include <QtWidgets/qdockwidget.h>
#else
    #include <QtGui/qdockwidget.h>
#endif

/* ----------------------------------------------------------------------- */

namespace Ui { class MessageDisplayer; }

/* ----------------------------------------------------------------------- */

/**   
   \class ViewErrorDialog
   \brief The Error Dialog of the Viewer.
*/

/* ----------------------------------------------------------------------- */

class VIEW_API ViewErrorDialog : public QDockWidget
{
  Q_OBJECT 

public:

  /// Constructor
  ViewErrorDialog (QWidget * parent = 0);

  /// Destructor
  ~ViewErrorDialog ();

public slots:

  /// Clear.
  void clear ();

  /// Set the text \b _text to the MultiLineEdit.
  void setText (char *&_text);

  /// Set the text \b _text to the MultiLineEdit.
  void setText (const QString& _text);

  /// Set the text \b _text to the MultiLineEdit as a Warning Message.
  void setWarning (const QString& _text);

  /// Set the text \b _text to the MultiLineEdit as an Error Message.
  void setError (const QString& _text);

  /// Set the text \b _text to the MultiLineEdit as an Info Message.
  void setInfo (const QString& _text);

  /// Append the text \b _text to the MultiLineEdit.
  void appendText (char *& _text);

  /// Append the text \b _text to the MultiLineEdit.
  void appendText (const QString& _text);

  /// Set the text \b _text to the MultiLineEdit as a Warning Message.
  void appendWarning (const QString& _text);

  /// Set the text \b _text to the MultiLineEdit as an Error Message.
  void appendError (const QString& _text);

  /// Set the text \b _text to the MultiLineEdit as an Info Message.
  void appendInfo (const QString& _text);

  void clipboardInfo();
  void qtMessage(QtMsgType type, const char *msg );

  void setVerbose(bool);

  QString& highligthText(QString&);

  void registerQtMsg(bool reg);

protected:

  /// This event handler is called when a key is pressed for this widget. 
  virtual void keyPressEvent (QKeyEvent * e);


private:


  /// The QMultiLineEdit Object
  Ui::MessageDisplayer * __display;
  bool __verbose;

  void __setText(const QString& t);
  void __append(const QString& t);
  bool __isLocked();
  void __lock();
  void __unlock();

  QStringList __listmessage;
  bool __displaylock;
#ifdef QT_THREAD_SUPPORT
  QMutex __mutexlock;
#endif


};


/* ----------------------------------------------------------------------- */

// __view_errordialog_h__
#endif

