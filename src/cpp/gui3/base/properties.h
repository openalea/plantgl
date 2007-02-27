/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
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
			
/*! \file view_properties.h
    \brief Definition of the viewer class ViewProperties3.
*/

#ifndef __view_properties_h__
#define __view_properties_h__

/* ----------------------------------------------------------------------- */

#include "glframe.h"
#include <qvariant.h>
#include <qdialog.h>

/* ----------------------------------------------------------------------- */

class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QFrame;
class QLabel;
class QLineEdit;
class QPushButton;
class QTabWidget;
class QWidget;
class QComboBox;

/* ----------------------------------------------------------------------- */

class ViewGLFrame3;
class ViewFileManager3;
class ViewControlPanel3;

/* ----------------------------------------------------------------------- */

/// Dialog to display properties of the file, the scene, the camera, ...
class VIEW3_API ViewProperties3 : public QDialog
{ 
    Q_OBJECT

public:
    ViewProperties3( ViewGLFrame3 *g, 
		ViewFileManager3* parent = 0,
		ViewControlPanel3 * controlpanel=0,
		bool config = false,
		const char* name = 0,
		bool modal = FALSE, 
		WFlags fl = 0 );

  ~ViewProperties3();

protected slots:

//  void setLanguage(const QString&);
  virtual void apply();
private :
  
  void initialize();
  QComboBox * lang;
};


/* ----------------------------------------------------------------------- */
#endif
 // PROPERTIES_H
