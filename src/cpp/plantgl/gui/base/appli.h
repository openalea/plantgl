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

#ifndef __view_appli_h__
#define __view_appli_h__

#include "../gui_config.h"
#include <plantgl/math/util_vector.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/colorarray.h>
#include "util_threadeddata.h"
#include "appbuilder.h"
#include <string>
#include <vector>

class ViewRayBuffer;
class ViewZBuffer;
class QEvent;
class Viewer;

class ViewerAppliInternal {

protected:
    static ViewerBuilder * VIEWERBUILDER;
    static ThreadedData<Viewer> VIEWER;
    static ThreadStateSaverFactory * THREADSTATESAVERFACTORY;
    static bool THREADSTATESAVERENABLED;


    static Viewer * build();
    void activateStateSaver(bool enabling);

public:
    static void setBuilder(ViewerBuilder * builder);

    template<class T>
    static void registerThreadStateSaver()
    { registerThreadStateSaverFatory(new ThreadStateSaverTFactory<T>()); }

    static void registerThreadStateSaverFatory(ThreadStateSaverFactory * tssf) ;
    static void cleanThreadStateSaverFatory() ;

    ViewerAppliInternal();
    virtual ~ViewerAppliInternal();

    static Viewer * getViewer();
    static void deleteViewer();

    void sendAnEvent(QEvent *e) ;
    void postAnEvent(QEvent *e) ;

    virtual void startSession()= 0;
    virtual bool stopSession() = 0;
    virtual bool exit() = 0;

    virtual bool running() = 0;
    virtual bool Wait ( unsigned long time = ULONG_MAX ) = 0;

    virtual std::vector<uint_t> getSelection();

    virtual bool getRedrawPolicy();
    virtual void setRedrawPolicy(bool);

    virtual int question(const std::string& caption,
                         const std::string& text,
                         const std::string& but0txt = "",
                         const std::string& but1txt = "",
                         const std::string& but2txt = "");


    virtual std::string itemSelection(const std::string& caption,
                                      const std::string& text,
                                      const std::vector<std::string> & values,
                                      bool& ok,
                                      bool editable);

    virtual double doubleSelection(const std::string& caption,
                                   const std::string& text,
                                   double value,
                                   double minvalue,
                                   double maxvalue,
                                   bool& ok);

    virtual std::string getFile(const std::string& caption,
                                const std::string& startPath,
                                const std::string& filter,
                                bool existing,
                                bool dir);

    virtual ViewRayBuffer * castRays(const PGL(Vector3)& pos,
                                      const PGL(Vector3)& dir,
                                      const PGL(Vector3)& dx,
                                      const PGL(Vector3)& dy,
                                      int sx, int sy);
    virtual ViewZBuffer * grabZBuffer();

    virtual std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)>  grabZBufferPoints();


};

#endif

