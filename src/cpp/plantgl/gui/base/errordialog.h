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

