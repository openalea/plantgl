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

#ifndef __view_threadedappli_h__
#define __view_threadedappli_h__

#include "../gui_config.h"

#ifndef QT_THREAD_SUPPORT
#ifdef __GNUC__
#warning "Qt compiled without openGL support"
#else
#pragma message "Qt compiled without openGL support"
#endif
#else

#include "appli.h"
#include <qthread.h>
#include <qmutex.h>
#include <qwaitcondition.h>

class QApplication;
class Viewer;

class ViewerThreadedAppli : public ViewerAppli, public QThread {
	public:
	ViewerThreadedAppli();
	virtual ~ViewerThreadedAppli();

	virtual void startSession();
	virtual bool stopSession() ;
	virtual bool exit() ;

    virtual bool running() ;
    virtual bool Wait ( unsigned long time = ULONG_MAX ) ;

	virtual const std::vector<uint_t> getSelection();

protected:

	void launch();
	virtual void init();
	virtual void cleanup();
	virtual void exec();
	virtual void run();

private :

	void startSync() ;
	void sync() ;
	void join() ;

	QMutex         syncmutex;
	QWaitCondition synccond;

	/// Wait Condition for initialisation.
	QWaitCondition contcond;

	/// Wait Condition for end viewer session.
	QMutex         sessionmutex;
	QWaitCondition session;

	QMutex         __continue;
	QMutex         __running;

	std::vector<uint_t> __selection;

};

#endif

#endif
