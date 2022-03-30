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


#include "simpleappli.h"
#include "viewer.h"
#include "event.h"
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qapplication.h>
#else
    #include <QtGui/qapplication.h>
#endif
#include <QtCore/qthread.h>

ViewerSimpleAppli::ViewerSimpleAppli():ViewerAppliInternal(), __ownappli(false) {
    // printf("Create Simple appli\n");
    launch();
}

ViewerSimpleAppli::~ViewerSimpleAppli(){
    if(running()) exit();
    if(qApp && __ownappli)delete qApp;
    // deleteViewer();
}

void
ViewerSimpleAppli::startSession()
{ if(!running()) getViewer()->show(); }

bool
ViewerSimpleAppli::stopSession()
{ if(running()) { getViewer()->hide(); return true; } else return false; }

bool
ViewerSimpleAppli::exit()
{
    if(qApp && __ownappli) { qApp->quit(); return true; }
    else return false;
}


bool
ViewerSimpleAppli::running()
{ return getViewer() != NULL && getViewer()->isVisible(); }

bool
ViewerSimpleAppli::Wait ( unsigned long time  )
{ return false; }

static qapp_initiator_func QAPP_INITIATOR = NULL;
void register_qapp_initiator(qapp_initiator_func initiator)
{
    QAPP_INITIATOR = initiator;
}

void
ViewerSimpleAppli::launch(){
    if(QApplication::instance() == NULL){
        if (QAPP_INITIATOR) QAPP_INITIATOR();
    }
    if(QApplication::instance() != NULL){
        __ownappli = false;
        Viewer * v = build();
        if (QApplication::instance()->thread() != QThread::currentThread()){
            v->moveToThread(QApplication::instance()->thread());
            if (v->thread() == QThread::currentThread()){

#ifdef GEOM_DLDEBUG
                printf("Viewer did not move to graphic thread. This will certainly lead to crash.\n");
#endif
                v->show();
            }
            else v->post(new ViewShowEvent());
        }
        else v->show();
    }
    else {
        int argc = 0;
        new QApplication(argc,NULL);
        __ownappli = true;
        Viewer * v = build();
        v->show();
        QApplication::instance()->exec();
    }
}

