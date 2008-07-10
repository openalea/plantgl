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

#include "../viewer/pglviewer.h"

ViewerBuilder * ViewerAppli::VIEWERBUILDER(0);
ThreadedData<Viewer> ViewerAppli::VIEWER(0);
ThreadStateSaverFactory * ViewerAppli::THREADSTATESAVERFACTORY(0);
bool ViewerAppli::THREADSTATESAVERENABLED(true);

Viewer * ViewerAppli::build() {
	if(VIEWERBUILDER) { VIEWER = VIEWERBUILDER->build(); }
	else VIEWER.set(new PGLViewer());
	return VIEWER.get(); 
}

Viewer * ViewerAppli::getViewer() {
	if(VIEWER) return VIEWER.get();
	else return build();
}

void ViewerAppli::deleteViewer() {
	if (VIEWER.get()->thread() != QThread::currentThread() && VIEWER.get()->thread()->isRunning())
		printf("Cannot delete Viewer from other thread ...\n");
	VIEWER.deleteData();
	VIEWER = NULL; 
}


void ViewerAppli::setBuilder(ViewerBuilder * builder) { VIEWERBUILDER = builder; }
void ViewerAppli::registerThreadStateSaverFatory(ThreadStateSaverFactory * tssf) { THREADSTATESAVERFACTORY = tssf; }
void ViewerAppli::cleanThreadStateSaverFatory() { delete THREADSTATESAVERFACTORY; THREADSTATESAVERFACTORY = NULL; }
void ViewerAppli::activateStateSaver(bool enabling) { THREADSTATESAVERENABLED =  enabling; }

void 
ViewerAppli::sendAnEvent(QEvent *e)
{ 
    ThreadStateSaver * tss = NULL;
    if (THREADSTATESAVERENABLED && THREADSTATESAVERFACTORY) {
        tss = THREADSTATESAVERFACTORY->produceStateSaver();
        if(tss) tss->pushState();
    }
	else { std::cerr << "do not save state" << std::endl; }
    startSession(); getViewer()->send(e); 
    if (tss) {
        tss->popState();
        delete tss;
    }
}

void 
ViewerAppli::postAnEvent(QEvent *e)
{ startSession(); getViewer()->post(e); }


ViewerAppli::ViewerAppli(){ ViewObjectGL::BASHMODE = true; }
ViewerAppli::~ViewerAppli(){  }


std::vector<uint_t>
ViewerAppli::getSelection() {
	std::vector<uint_t> res;
    if(running()){
      ViewSelectRecoverEvent * event = new ViewSelectRecoverEvent(&res) ;
      sendAnEvent(event);
    }
    return res;
}

int 
ViewerAppli::question(const std::string& caption,
			   const std::string& text,
			   const std::string& but0txt,
			   const std::string& but1txt,
			   const std::string& but2txt) {
	int res = -1;
    ViewQuestionEvent * event = new ViewQuestionEvent(
	  QString(caption.c_str()),
	  QString(text.c_str()),
	  (but0txt.empty()?QString::null:QString( but0txt.c_str() )),
	  (but1txt.empty()?QString::null:QString( but1txt.c_str() )),
	  (but2txt.empty()?QString::null:QString( but2txt.c_str() )),
	  &res);
	sendAnEvent(event);
	return res;
}

std::string 
ViewerAppli::itemSelection(const std::string& caption,
					   const std::string& text,
					   const std::vector<std::string> & values,
					   bool& ok,
					   bool editable){
	QString res ;
	QStringList l;
	for(std::vector<std::string>::const_iterator _it = values.begin();
		_it != values.end(); _it++)
		  if(!_it->empty())l.append(QString(_it->c_str()));
    ViewItemSelectionEvent * event = new ViewItemSelectionEvent(
	  caption.c_str(), text.c_str(), l, editable,&res,&ok);
	sendAnEvent(event);
	return std::string(res.toAscii().constData());
}

double 
ViewerAppli::doubleSelection(const std::string& caption,
								   const std::string& text,
								   double value,
                                   double minvalue,
                                   double maxvalue,
								   bool& ok)
{
    double res ;
    ViewDoubleSelectionEvent * event = new ViewDoubleSelectionEvent (
	  caption.c_str(), text.c_str(), value, minvalue, maxvalue,&res,&ok);
	sendAnEvent(event);
	return res;
}


std::string 
ViewerAppli::getFile(const std::string& caption,
					  const std::string& startPath,
				      const std::string& filter,
					  bool existing,bool dir){

	QString res;
    ViewFileSelEvent * event = new ViewFileSelEvent(
	  (caption.empty()?"Choose File":caption.c_str()),
	  (startPath.empty()?QString::null:QString( startPath.c_str() )),
	  (filter.empty()?QString::null:QString( filter.c_str() )),
	  existing,
	  dir, &res);
	sendAnEvent(event);
	return (res.isEmpty()?std::string():std::string(res.toAscii().constData()));
  }

ViewRayBuffer * 
ViewerAppli::castRays(const TOOLS(Vector3)& pos, 
	                     const TOOLS(Vector3)& dir,
						 const TOOLS(Vector3)& dx, 
						 const TOOLS(Vector3)& dy,
						 int sx, int sy){
	ViewRayBuffer * res = NULL ;
	ViewRayBuffEvent * event = new ViewRayBuffEvent(pos,dir,dx,dy,sx,sy,&res);
	sendAnEvent(event);
	return res;
}

ViewZBuffer * 
ViewerAppli::grabZBuffer(){
	ViewZBuffer * res = NULL ;
	ViewZBuffEvent * event = new ViewZBuffEvent(&res);
	sendAnEvent(event);
	return res;
}

