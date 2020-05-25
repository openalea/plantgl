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


#include "appli.h"
#include "appbuilder.h"
#include "event.h"
#include "object.h"
#include "glframe.h"

#include "../viewer/pglviewer.h"


ViewerBuilder * ViewerAppliInternal::VIEWERBUILDER(0);
ThreadedData<Viewer> ViewerAppliInternal::VIEWER(0);
ThreadStateSaverFactory * ViewerAppliInternal::THREADSTATESAVERFACTORY(0);
bool ViewerAppliInternal::THREADSTATESAVERENABLED(true);

Viewer * ViewerAppliInternal::build() {
    if(VIEWERBUILDER) { VIEWER = VIEWERBUILDER->build(); }
    else VIEWER.set(new PGLViewer());
    return VIEWER.get();
}

Viewer * ViewerAppliInternal::getViewer() {
    if(VIEWER) return VIEWER.get();
    else return build();
}

void ViewerAppliInternal::deleteViewer() {
    if (VIEWER.get()->thread() != QThread::currentThread() && VIEWER.get()->thread()->isRunning())

#ifdef GEOM_DLDEBUG
        printf("Cannot delete Viewer from other thread ...\n");
#endif

    VIEWER.deleteData();
    VIEWER = NULL;
}


void ViewerAppliInternal::setBuilder(ViewerBuilder * builder) { VIEWERBUILDER = builder; }
void ViewerAppliInternal::registerThreadStateSaverFatory(ThreadStateSaverFactory * tssf) { THREADSTATESAVERFACTORY = tssf; }
void ViewerAppliInternal::cleanThreadStateSaverFatory() { delete THREADSTATESAVERFACTORY; THREADSTATESAVERFACTORY = NULL; }
void ViewerAppliInternal::activateStateSaver(bool enabling) { THREADSTATESAVERENABLED =  enabling; }

void
ViewerAppliInternal::sendAnEvent(QEvent *e)
{
    ThreadStateSaver * tss = NULL;
    if (THREADSTATESAVERENABLED && THREADSTATESAVERFACTORY) {
        tss = THREADSTATESAVERFACTORY->produceStateSaver();
        if(tss) tss->pushState();
    }
    startSession(); getViewer()->send(e);
    if (tss) {
        tss->popState();
        delete tss;
    }
}

void
ViewerAppliInternal::postAnEvent(QEvent *e)
{ startSession(); getViewer()->post(e); }


ViewerAppliInternal::ViewerAppliInternal(){ /*ViewObjectGL::BATCHMODE = true;*/ }
ViewerAppliInternal::~ViewerAppliInternal(){  }


std::vector<uint_t>
ViewerAppliInternal::getSelection() {
    std::vector<uint_t> res;
    if(running()){
      ViewSelectRecoverEvent * event = new ViewSelectRecoverEvent(&res) ;
      sendAnEvent(event);
    }
    return res;
}

bool ViewerAppliInternal::getRedrawPolicy()
{
    if(running()){
        bool res;
        sendAnEvent(new ViewGetRedrawEvent(&res));
        return res;
    }
    else return getViewer()->getFrameGL()->isRedrawEnabled();
}

void ViewerAppliInternal::setRedrawPolicy(bool policy)
{
    if(running()){
        sendAnEvent(new ViewSetRedrawEvent(policy));
    }
    else return getViewer()->getFrameGL()->activateRedraw(policy);
}


int
ViewerAppliInternal::question(const std::string& caption,
               const std::string& text,
               const std::string& but0txt,
               const std::string& but1txt,
               const std::string& but2txt) {
    int res = -1;
    ViewQuestionEvent * event = new ViewQuestionEvent(&res,
      QString(caption.c_str()),
      QString(text.c_str()),
      (but0txt.empty()?QString():QString( but0txt.c_str() )),
      (but1txt.empty()?QString():QString( but1txt.c_str() )),
      (but2txt.empty()?QString():QString( but2txt.c_str() )));
    sendAnEvent(event);
    return res;
}

std::string
ViewerAppliInternal::itemSelection(const std::string& caption,
                       const std::string& text,
                       const std::vector<std::string> & values,
                       bool& ok,
                       bool editable){
    QString res ;
    QStringList l;
    for(std::vector<std::string>::const_iterator _it = values.begin();
        _it != values.end(); _it++)
          if(!_it->empty())l.append(QString(_it->c_str()));
    ViewItemSelectionEvent * event = new ViewItemSelectionEvent(&res,
      caption.c_str(), text.c_str(), l, editable,&ok);
    sendAnEvent(event);
    return res.toStdString();
}

double
ViewerAppliInternal::doubleSelection(const std::string& caption,
                                   const std::string& text,
                                   double value,
                                   double minvalue,
                                   double maxvalue,
                                   bool& ok)
{
    double res ;
    ViewDoubleSelectionEvent * event = new ViewDoubleSelectionEvent (&res,
      caption.c_str(), text.c_str(), value, minvalue, maxvalue,&ok);
    sendAnEvent(event);
    return res;
}


std::string
ViewerAppliInternal::getFile(const std::string& caption,
                      const std::string& startPath,
                      const std::string& filter,
                      bool existing,bool dir){

    QString res;
    ViewFileSelEvent * event = new ViewFileSelEvent(&res,
      (caption.empty()?"Choose File":caption.c_str()),
      (startPath.empty()?QString():QString( startPath.c_str() )),
      (filter.empty()?QString():QString( filter.c_str() )),
      existing,
      dir);
    sendAnEvent(event);
    return (res.isEmpty()?std::string():res.toStdString());
  }

ViewRayBuffer *
ViewerAppliInternal::castRays(const Vector3& pos,
                         const Vector3& dir,
                         const Vector3& dx,
                         const Vector3& dy,
                         int sx, int sy){
    ViewRayBuffer * res = NULL ;
    ViewRayBuffEvent * event = new ViewRayBuffEvent(&res,pos,dir,dx,dy,sx,sy);
    sendAnEvent(event);
    return res;
}

ViewZBuffer *
ViewerAppliInternal::grabZBuffer(){
    ViewZBuffer * res = NULL ;
    ViewZBuffEvent * event = new ViewZBuffEvent(&res);
    sendAnEvent(event);
    return res;
}

std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)>
ViewerAppliInternal::grabZBufferPoints(){
    std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)> res ;
    ViewZBuffPointsEvent * event = new ViewZBuffPointsEvent(&res);
    sendAnEvent(event);
    return res;
}

std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)>  
ViewerAppliInternal::grabZBufferPointsWithJitter(float jitter, int raywidth){
    std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)> res ;
    ViewZBuffPoints2Event * event = new ViewZBuffPoints2Event(&res, jitter, raywidth);
    sendAnEvent(event);
    return res;
}

