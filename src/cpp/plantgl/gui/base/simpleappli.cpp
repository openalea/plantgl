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
#include "event.h"
#include <qapplication.h>
#include <qthread.h>

ViewerSimpleAppli::ViewerSimpleAppli():ViewerAppli(), __ownappli(false) { 
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

const std::vector<uint32_t> 
ViewerSimpleAppli::getSelection(){
	if(getViewer())return getViewer()->getSelection();
	else return std::vector<uint32_t>();
}

void
ViewerSimpleAppli::launch(){
	if(qApp != NULL){
		__ownappli = false;
		Viewer * v = build();
		if (qApp->thread() != QThread::currentThread()){
			v->moveToThread(qApp->thread());
			if (v->thread() == QThread::currentThread()){
				printf("Viewer did not move to graphic thread. This will certainly lead to crash.\n");
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
		qApp->exec();
	}
}

