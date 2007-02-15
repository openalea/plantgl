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

/*! \file view_errordialog.h
    \brief Definition of the viewer class ViewErrorDialog.
*/

#ifndef __view_errordialog_h__
#define __view_errordialog_h__

/* ----------------------------------------------------------------------- */

#include <qstringlist.h>
#include "util_qwidget.h"
#include <qthread.h>

/* ----------------------------------------------------------------------- */

class MessageDisplayer;

/* ----------------------------------------------------------------------- */

/**   
   \class ViewErrorDialog
   \brief The Error Dialog of the Viewer.
*/

/* ----------------------------------------------------------------------- */

class VIEW_API ViewErrorDialog : public ViewDialog
{
  Q_OBJECT 

public:

  /// Constructor
  ViewErrorDialog (QWidget * parent = 0, 
	       const char *name = 0, 
	       bool modal = FALSE);

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

  /// This event handler is called when the widget is resized.
  virtual void resizeEvent (QResizeEvent * event);


private:


  /// The QMultiLineEdit Object
  MessageDisplayer * __display;
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

