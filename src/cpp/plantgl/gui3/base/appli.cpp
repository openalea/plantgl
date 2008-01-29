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

#include "../viewer/pglviewer.h"

ViewerBuilder3 * ViewerAppli3::VIEWERBUILDER(0);

Viewer3 * ViewerAppli3::build() {
	if(VIEWERBUILDER) return VIEWERBUILDER->build();
	else return new PGLViewer3();
}

void ViewerAppli3::setBuilder(ViewerBuilder3 * builder) { VIEWERBUILDER = builder; }

ViewerAppli3::ViewerAppli3(){ ViewObjectGL3::BASHMODE = true; }
ViewerAppli3::~ViewerAppli3(){  }


const std::vector<uint_t>
ViewerAppli3::getSelection() {
	std::vector<uint_t> res;
    if(isRunning()){
      ViewSelectRecoverEvent3 * event = new ViewSelectRecoverEvent3(&res) ;
      sendAnEvent(event);
    }
    return res;
}

int 
ViewerAppli3::question(const std::string& caption,
			   const std::string& text,
			   const std::string& but0txt,
			   const std::string& but1txt,
			   const std::string& but2txt) {
	int res = -1;
    ViewQuestionEvent3 * event = new ViewQuestionEvent3(
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
ViewerAppli3::itemSelection(const std::string& caption,
					   const std::string& text,
					   const std::vector<std::string> & values,
					   bool& ok,
					   bool editable){
	QString res ;
	QStringList l;
	for(std::vector<std::string>::const_iterator _it = values.begin();
		_it != values.end(); _it++)
		  if(!_it->empty())l.append(QString(_it->c_str()));
    ViewItemSelectionEvent3 * event = new ViewItemSelectionEvent3(
	  caption.c_str(), text.c_str(), l, editable,&res,&ok);
	sendAnEvent(event);
	return std::string(res.latin1());
}

std::string 
ViewerAppli3::getFile(const std::string& caption,
					  const std::string& startPath,
				      const std::string& filter,
					  bool existing,bool dir){

	QString res;
    ViewFileSelEvent3 * event = new ViewFileSelEvent3(
	  (caption.empty()?"Choose File":caption.c_str()),
	  (startPath.empty()?QString::null:QString( startPath.c_str() )),
	  (filter.empty()?QString::null:QString( filter.c_str() )),
	  existing,
	  dir, &res);
	sendAnEvent(event);
	return (res.isEmpty()?std::string():std::string(res.latin1()));
  }

ViewRayBuffer3 * 
ViewerAppli3::castRays(const TOOLS(Vector3)& pos, 
	                     const TOOLS(Vector3)& dir,
						 const TOOLS(Vector3)& dx, 
						 const TOOLS(Vector3)& dy,
						 int sx, int sy){
	ViewRayBuffer3 * res = NULL ;
	ViewRayBuffEvent3 * event = new ViewRayBuffEvent3(pos,dir,dx,dy,sx,sy,&res);
	sendAnEvent(event);
	return res;
}

ViewZBuffer3 * 
ViewerAppli3::grabZBuffer(){
	ViewZBuffer3 * res = NULL ;
	ViewZBuffEvent3 * event = new ViewZBuffEvent3(&res);
	sendAnEvent(event);
	return res;
}

