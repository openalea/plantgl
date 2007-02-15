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

/*! \file view_locationbar.h
    \brief Definition of the viewer class ViewLocationBar.
*/

#ifndef __view_locationbar_h__
#define __view_locationbar_h__

/* ----------------------------------------------------------------------- */

#include "util_qwidget.h"
#include <qcombobox.h>
class QLabel;
class QToolButton;
class QMainWindow;

/* ----------------------------------------------------------------------- */

/*
class VIEW_API ViewComboBox : public QComboBox
{
  Q_OBJECT

    public:

  ViewComboBox( QWidget * parent=0, const char * name=0 ) ;

  ~ViewComboBox();

public slots:

      virtual void clearfocus();

      virtual void clearedit();

 protected:

    virtual void focusInEvent ( QFocusEvent *e );
    
    virtual void focusOutEvent ( QFocusEvent *e );

    /// This event handler is called when a mouse press event is in progress for this widget.
    virtual void mousePressEvent( QMouseEvent*);
};*/

/// Viewer Location Bar
class ViewLocationBar : public ViewToolBar
{ 
    Q_OBJECT

public:
    ViewLocationBar( const QString &label, 
		     QMainWindow *mw, 
#if QT_VERSION >= 300
		     ToolBarDock = DockTop,
#else
		     QMainWindow::ToolBarDock tbd = QMainWindow::Top,
#endif
		     bool newLine=FALSE, 
		     const char *name=0);
    ~ViewLocationBar();


    QComboBox* __Location;


};

/* ----------------------------------------------------------------------- */
#endif
 // VIEWLOCATIONBAR_H
