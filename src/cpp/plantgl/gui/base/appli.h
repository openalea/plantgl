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
    virtual std::pair<PGL(Point3ArrayPtr),PGL(Color4ArrayPtr)>  grabZBufferPointsWithJitter(float jitter, int raywidth, bool mixcolor = true);


};

#endif

