/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
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
	  (but0txt.empty()?QString::null:QString( but0txt.c_str() )),
	  (but1txt.empty()?QString::null:QString( but1txt.c_str() )),
	  (but2txt.empty()?QString::null:QString( but2txt.c_str() )));
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
	  (startPath.empty()?QString::null:QString( startPath.c_str() )),
	  (filter.empty()?QString::null:QString( filter.c_str() )),
	  existing,
	  dir);
	sendAnEvent(event);
	return (res.isEmpty()?std::string():res.toStdString());
  }

ViewRayBuffer * 
ViewerAppliInternal::castRays(const TOOLS(Vector3)& pos, 
	                     const TOOLS(Vector3)& dir,
						 const TOOLS(Vector3)& dx, 
						 const TOOLS(Vector3)& dy,
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

