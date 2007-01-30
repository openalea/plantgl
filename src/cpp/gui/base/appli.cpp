/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): F. Boudon
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

#include "appli.h"
#include "appbuilder.h"
#include "event.h"
#include "object.h"

ViewerBuilder * ViewerAppli::VIEWERBUILDER(0);
Viewer * ViewerAppli::build() { return (VIEWERBUILDER?VIEWERBUILDER->build():NULL); }
void ViewerAppli::setBuilder(ViewerBuilder * builder) { VIEWERBUILDER = builder; }

ViewerAppli::ViewerAppli(){ ViewObjectGL::BASHMODE = true; }
ViewerAppli::~ViewerAppli(){  }


const std::vector<uint32_t>
ViewerAppli::getSelection() {
	std::vector<uint32_t> res;
    if(isRunning()){
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
	  caption.c_str(),
	  text.c_str(),
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
	return std::string(res.latin1());
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
	return (res.isEmpty()?std::string():std::string(res.latin1()));
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

