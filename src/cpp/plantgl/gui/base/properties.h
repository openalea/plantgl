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
        Qt::WindowFlags fl = Qt::Widget );

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
