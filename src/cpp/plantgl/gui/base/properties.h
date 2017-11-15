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
			
/*! \file view_properties.h
    \brief Definition of the viewer class ViewProperties.
*/

#ifndef __view_properties_h__
#define __view_properties_h__

/* ----------------------------------------------------------------------- */

#include "glframe.h"
#include <QtCore/qvariant.h>
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0) 
    #include <QtWidgets/qdialog.h>
#else
    #include <QtGui/qdialog.h>
#endif

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

class ViewGLFrame;
class ViewFileManager;
class ViewControlPanel;

/* ----------------------------------------------------------------------- */

/// Dialog to display properties of the file, the scene, the camera, ...
class VIEW_API ViewProperties : public QDialog
{ 
    Q_OBJECT

public:
    ViewProperties( ViewGLFrame *g, 
		ViewFileManager* parent = 0,
		ViewControlPanel * controlpanel=0,
		bool config = false,
		const char* name = 0,
		bool modal = false, 
		Qt::WindowFlags fl = 0 );

  ~ViewProperties();

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
