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

#include "locationbar.h"
#include "icons.h"

#include <QtGlobal>
#include <QtGui/qpixmap.h>
#include <QtGui/qcursor.h>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qcombobox.h>
    #include <QtWidgets/qtoolbutton.h>
    #include <QtWidgets/qlabel.h>
    #include <QtWidgets/QHBoxLayout>
#else
    #include <QtGui/qcombobox.h>
    #include <QtGui/qtoolbutton.h>
    #include <QtGui/qlabel.h>
    #include <QtGui/QHBoxLayout>
#endif

/*----------------------------------------------------------------------------*/

/*
 *  Constructs a ViewLocationBar which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
ViewLocationBar::ViewLocationBar(const QString &label,
                                 QWidget *mw,const char *name)
  : ViewToolBar( label, mw ,name )
{


    QToolButton * bt = NULL;
    QPixmap erase(ViewerIcon::getPixmap(ViewerIcon::locerase));
    bt =  new QToolButton(this);
    bt->setIcon(QIcon(erase));
    bt->setFixedSize(QSize(25,25));
    bt->setWhatsThis("<b>"+tr("Erase Location")+"</b><br><br>"
    "Erase the current filename to specify a new one.");
    addWidget(bt);

    // QLabel * LocationLabel = new QLabel(this );
    // LocationLabel->setText( tr( " Location " ) );
    // LocationLabel->setFixedSize(QSize(50,25));
    // addWidget(LocationLabel);

    __Location = new QComboBox( this );
    __Location->setProperty("minimumHeight",25);
    __Location->setProperty("minimumWidth",50);
    __Location->setEditable( true );
    //__Location->setAutoCompletion ( true ); //setCompleter( QCompleter *c)
    __Location->setCursor( QCursor( Qt::IBeamCursor ) );
    __Location->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    addWidget(__Location);

    __Location->setWhatsThis("<b>"+tr("The Filename")+"</b><br><br>"
    "The name of the file of the current scene.");
    if(bt)QObject::connect(bt,SIGNAL(clicked()),__Location,SLOT(clearEditText()));
 }

/*
 *  Destroys the object and frees any allocated resources
 */
ViewLocationBar::~ViewLocationBar()
{
    // no need to delete child widgets, Qt does it all for us
}

/*----------------------------------------------------------------------------*/

/*
ViewComboBox::ViewComboBox( QWidget * parent, const char * name ):
  QComboBox(parent,name)
{
}

ViewComboBox::~ViewComboBox()
{
}

void ViewComboBox::focusInEvent ( QFocusEvent *e )
{
//  setEditable( true );
}

void ViewComboBox::focusOutEvent ( QFocusEvent *e )
{
//  setEditable( FALSE );
}

void ViewComboBox::mousePressEvent( QMouseEvent*e)
{
//  setEditable( true );
  QComboBox::mousePressEvent(e);
}


void ViewComboBox::clearfocus()
{
  clearFocus();
//  setEditable( FALSE );
}

void ViewComboBox::clearedit()
{
  clearEdit();
//  setEditable( true );
}

*/
