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

#include "simpleappli.h"
#include "viewer.h"
#include <qapplication.h>

ViewerSimpleAppli::ViewerSimpleAppli():ViewerAppli(), __viewer(0),__appli(0), __ownappli(false) { launch(); }

ViewerSimpleAppli::~ViewerSimpleAppli(){
	if(isRunning()) exit();
	if(__appli && __ownappli)delete __appli;
	if(__viewer) delete __viewer;
}

void 
ViewerSimpleAppli::startSession()
{ if(!isRunning()) __viewer->show(); }

bool 
ViewerSimpleAppli::stopSession()
{ if(isRunning()) { __viewer->hide(); return true; } else return false; }

bool 
ViewerSimpleAppli::exit()
{ 
	if(__appli && __ownappli) { __appli->quit(); return true; }
	else return false;
}

void 
ViewerSimpleAppli::sendAnEvent(QCustomEvent *e)
{ QApplication::sendEvent(__viewer,e); delete e; }

void 
ViewerSimpleAppli::postAnEvent(QCustomEvent *e)
{ QApplication::postEvent(__viewer,e); }

bool 
ViewerSimpleAppli::isRunning() 
{ return __viewer != NULL && __viewer->isShown(); }

bool 
ViewerSimpleAppli::Wait ( unsigned long time  )
{ return false; }

const std::vector<uint32_t> 
ViewerSimpleAppli::getSelection(){
	if(__viewer)return __viewer->getSelection();
	else return std::vector<uint32_t>();
}

QApplication * 
ViewerSimpleAppli::getApplication()
{ return __appli; }

void
ViewerSimpleAppli::launch(){
	if(qApp != NULL){
		__appli = qApp;
		__ownappli = false;
		__viewer = build();
		if(__appli->mainWidget() == NULL)
			__appli->setMainWidget(__viewer);
		__viewer->show();
	}
	else {
		int argc = 0;
		__appli = new QApplication(argc,NULL);
		__ownappli = true;
		__viewer = build();
        __appli->setMainWidget(__viewer);
		__viewer->show();
		__appli->exec();
	}
}

