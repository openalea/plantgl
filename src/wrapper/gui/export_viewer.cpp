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



#include "export_viewer.h"
#include <plantgl/python/extract_list.h>
#include <plantgl/python/extract_widget.h>
#include <plantgl/python/pyinterpreter.h>

#include <boost/python.hpp>

#include <plantgl/gui/viewer/pglapplication.h>
#include <plantgl/gui/viewer/editgeomscenegl.h>
#include <plantgl/gui/base/zbuffer.h>
#include <plantgl/gui/base/appbuilder.h>
#include <plantgl/gui/base/simpleappli.h>

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;
#define bp boost::python


boost::python::list selection(){
  std::vector<uint_t> sel = ViewerApplication::getSelection();
  if(sel.empty()) return boost::python::list();
  else {
    boost::python::list res;
    for(vector<uint_t>::const_iterator _it = sel.begin();
    _it != sel.end(); _it++){
      res.append(object((int)*_it));
    }
    return res;
  }
}

void setSelection(int i){
  std::vector<uint_t> sel;
  sel.push_back(i);
  ViewerApplication::setSelection(sel);
}

void setMSelection(boost::python::list values){
  std::vector<uint_t> sel = extract_vec<uint_t>(values)();
  ViewerApplication::setSelection(sel);
}

int question00(std::string text){
  return ViewerApplication::question("Question",text);
}

int question0(std::string caption,
              std::string text){
  return ViewerApplication::question(caption,text);
}

int question1(std::string caption,
              std::string text,
              std::string but0txt){
  return ViewerApplication::question(caption,text,but0txt);
}

int question2(std::string caption,
              std::string text,
              std::string but0txt,
              std::string but1txt){
  return ViewerApplication::question(caption,text,but0txt,but1txt);
}

int question3(std::string caption,
              std::string text,
              std::string but0txt,
              std::string but1txt,
              std::string but2txt){
  return ViewerApplication::question(caption,text,but0txt,but1txt,but2txt);
}


#ifdef False // PGL_DEBUG

template <class T >
class MyExtractor : public boost::python::extract<const char *> {
public:
    MyExtractor(object obj):extract<const char *>(obj){};
};

object itemSelection(boost::python::str _caption,
                     boost::python::str _text,
                     boost::python::list values,
                     bool editable){
  std::string caption = boost::python::extract<const char *>(_caption);
  std::string text = boost::python::extract<const char *>(_text);
  std::vector<std::string> vals = extract_vec<std::string,MyExtractor >(values)();
#else
object itemSelection(std::string caption,
                     std::string text,
                     boost::python::list values,
                     bool editable){
  std::vector<std::string> vals = extract_vec<std::string >(values)();
#endif
  bool ok = false;
  std::string res;
  {
    res = ViewerApplication::itemSelection(caption,text,vals,ok,editable);
  }
  boost::python::tuple t = boost::python::make_tuple(ok,res);
  return t;
}

object itemSelection2(std::string text,
                     boost::python::list values){
  std::vector<std::string> vals = extract_vec<std::string >(values)();
  bool ok = false;
  std::string res;
  res = ViewerApplication::itemSelection("Select Item",text,vals,ok,false);
  boost::python::tuple t = boost::python::make_tuple(ok,res);
  return t;
}

object itemSelection1(boost::python::list values){
  std::vector<std::string> vals = extract_vec<std::string >(values)();
  bool ok = false;
  std::string res;
  res = ViewerApplication::itemSelection("Select Item","Please select an item from the list",vals,ok,false);
  boost::python::tuple t = boost::python::make_tuple(ok,res);
  return t;
}

object itemSelectionNE(std::string caption,
                       std::string text,
                       boost::python::list values){
  return itemSelection(caption,text,values,false);
}


object doubleSelection(std::string caption,
                       std::string text,
                       double value,
                       double minvalue,
                       double maxvalue){
  double res = 0;
  bool ok;
  {
    res = ViewerApplication::doubleSelection(caption,text,value,minvalue,maxvalue,ok);
  }
  if (ok) return object(res);
  else return object();
}


object doubleSelection3(std::string caption,
                       std::string text,
                       double value)
{
  return doubleSelection(caption,text,value,DBL_MIN,DBL_MAX);
}

object doubleSelection2(std::string caption,
                       std::string text)
{
   return doubleSelection(caption,text,0,DBL_MIN,DBL_MAX);
}

object doubleSelection1(std::string text)
{
   return doubleSelection("Select a value",text,0,DBL_MIN,DBL_MAX);
}

object doubleSelection0()
{
   return doubleSelection("Select a value","Please, select a value",0,DBL_MIN,DBL_MAX);
}


void fullScreen0(){
  ViewerApplication::fullScreen();
}

void fullScreen1(int b){
  ViewerApplication::fullScreen(b);
}

void glFrameOnly0(){
  ViewerApplication::glFrameOnly();
}

void glFrameOnly1(int b){
  ViewerApplication::glFrameOnly(b);
}

void setAnimation1(eAnimationFlag b){
  ViewerApplication::setAnimation(b);
}

void animation1(bool b){
    ViewerApplication::setAnimation(b?eAnimatedScene:eStatic);
}


void setBGColorO(boost::python::object o){
  int r = extract<int>(o.attr("red"))();
  int g = extract<int>(o.attr("green"))();
  int b = extract<int>(o.attr("blue"))();
  ViewerApplication::setBgColor(r,g,b);
}

std::string getOpenFile0(){
  return ViewerApplication::getOpenFile("Open File","","");
}

std::string getOpenFile1(const std::string& caption){
  return ViewerApplication::getOpenFile(caption,"","");
}

std::string getOpenFile2(const std::string& caption, const std::string& filter){
  return ViewerApplication::getOpenFile(caption,filter,"");
}

std::string getSaveFile0(){
  return ViewerApplication::getSaveFile("Save File","","");
}

std::string getSaveFile1(const std::string& caption){
  return ViewerApplication::getSaveFile(caption,"","");
}

std::string getSaveFile2(const std::string& caption, const std::string& filter){
  return ViewerApplication::getSaveFile(caption,filter,"");
}

std::string getDirectory0(){
  return ViewerApplication::getDirectory("Choose Directory","");
}

std::string getDirectory1(const std::string& caption){
  return ViewerApplication::getDirectory(caption,"");
}

boost::python::object raybuf_to_python(ViewRayBuffer * buf) {
    boost::python::list res;
    for(size_t i = 0; i < buf->getColumnSize();i++){
        boost::python::list row;
        for(size_t j = 0; j < buf->getRowSize();j++){
            boost::python::list zlist;
            for(size_t k = 0; k < buf->getAt(i,j).size();k++){
                const RayHit& inter = buf->getAt(i,j)[k];
                zlist.append(boost::python::make_tuple(inter.id,inter.zmax,inter.zmin));
            }
            row.append(zlist);
        }
        res.append(row);
    }
    return res;
}

boost::python::object castRays(const Vector3& pos,
                                  const Vector3& dir,
                                  const Vector3& dx,
                                  const Vector3& dy,
                                  int sx, int sy){
    ViewRayBuffer * buf = ViewerApplication::castRays(pos,dir,dx,dy,sx,sy);
    boost::python::object res = raybuf_to_python(buf);
    delete buf;
    return res;
}

boost::python::object zbuf_to_python(ViewZBuffer * buf, bool allvalues) {
    boost::python::list res;
    for(size_t i = 0; i < buf->getColumnSize();i++){
        boost::python::list row;
        for(size_t j = 0; j < buf->getRowSize();j++){
            const ZBufferUnit& unit = buf->getAt(i,j);
            if (allvalues || (0.0 < unit.depth && unit.depth < 1.0))
                row.append(boost::python::make_tuple(unit.pos,unit.color,unit.depth));
            else row.append(object());
        }
        res.append(row);
    }
    return res;
}

boost::python::object grabZBuffer(bool allvalues){
    ViewZBuffer * buf = ViewerApplication::grabZBuffer();
    boost::python::object res = zbuf_to_python(buf,allvalues);
    delete buf;
    return res;
}

boost::python::object grabZBuffer0(){
    return grabZBuffer(false);
}

boost::python::object grabZBufferPoints(){
    std::pair<Point3ArrayPtr,Color4ArrayPtr> bufpoints = ViewerApplication::grabZBufferPoints();
    return boost::python::make_tuple(bufpoints.first,bufpoints.second);
}

boost::python::object grabZBufferPoints2(float jitter, int raywidth, bool mixcolor = true){
    std::pair<Point3ArrayPtr,Color4ArrayPtr> bufpoints = ViewerApplication::grabZBufferPointsWithJitter(jitter, raywidth, mixcolor);
    return boost::python::make_tuple(bufpoints.first,bufpoints.second);
}

boost::python::object getProjectionSize(){
    int nbpix;
    double pixwidth;
    double size;
    size = ViewerApplication::getProjectionSize(&nbpix,&pixwidth);
    return boost::python::make_tuple(size,nbpix,pixwidth);
}

boost::python::object getProjectionSizes(const ScenePtr& sc){
  std::vector<std::pair<uint_t,double> > res;
  res = PGLViewerApplication::getProjectionSizes(sc);
  if(res.empty()) return object();
  else {
    boost::python::list bres;
    for(vector<std::pair<uint_t,double> >::const_iterator _it = res.begin();
    _it != res.end(); _it++){
      bres.append(boost::python::make_tuple(_it->first,_it->second));
    }
    return bres;
  }
}

boost::python::object getProjectionPerShape(){
  std::vector<std::pair<uint_t,uint_t> > res;
  double pixelsize;
  res = PGLViewerApplication::getProjectionPerShape(pixelsize);
  if(res.empty()) return object();
  else {
    boost::python::list bres;
    for(vector<std::pair<uint_t,uint_t> >::const_iterator _it = res.begin();
    _it != res.end(); _it++){
      bres.append(boost::python::make_tuple(_it->first,_it->second));
    }
    return boost::python::make_tuple(bres,pixelsize);
  }
}

boost::python::object raypointhitbuf_to_python(ViewRayPointHitBuffer * buf) {
    boost::python::list res;
    for(size_t i = 0; i < buf->getColumnSize();i++){
        boost::python::list row;
        for(size_t j = 0; j < buf->getRowSize();j++){
            boost::python::list zlist;
            for(size_t k = 0; k < buf->getAt(i,j).size();k++){
                const RayPointHit& inter = buf->getAt(i,j)[k];
                zlist.append(boost::python::make_tuple(inter.id,inter.zmax,inter.zmin));
            }
            row.append(zlist);
        }
        res.append(row);
    }
    return res;
}

boost::python::object castRays2(const ScenePtr& sc, bool back_test){
    ViewRayPointHitBuffer * buf;
    buf = PGLViewerApplication::castRays2(sc,back_test);
    boost::python::object res = raypointhitbuf_to_python(buf);
    delete buf;
    return res;
}

boost::python::object castRays2_1(const ScenePtr& sc){
    ViewRayPointHitBuffer * buf ;
    buf = PGLViewerApplication::castRays2(sc,true);
    boost::python::object res = raypointhitbuf_to_python(buf);
    delete buf;
    return res;
}


void displaySh(ShapePtr sh){
    ScenePtr s = ScenePtr(new Scene());
    s->add(sh);
    PGLViewerApplication::display(s);
}

void addSh(ShapePtr sh){
    ScenePtr s = ScenePtr(new Scene());
    s->add(sh);
    PGLViewerApplication::add(s);
}

void saveImage1(const std::string& fname)
{
    // ext = get_extension(fname);
    // if (ext.empty()) ext = "PNG";
    // else ext = toUpper(ext);
    ViewerApplication::saveImage(fname,"PNG");
}
void saveImage2(const std::string& fname, const std::string& type)
{
    ViewerApplication::saveImage(fname,type);
}

bool viewer_wait(){
    return ViewerApplication::wait();
}


MaterialPtr pyGetMaterialFromDialog(boost::python::object pyparent = boost::python::object(),
                                       const char * caption = "",
                                       MaterialPtr initial = MaterialPtr() )
{
    QWidget * parent  = NULL;
    if (pyparent != boost::python::object())
        parent = extract_widget<QWidget>(pyparent)();
    return getMaterialFromDialog(parent,caption,initial);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(pyGetMaterialFromDialog_overloads, pyGetMaterialFromDialog, 0, 3)

int pyEditMaterialInDialog(Material * initial,
                            boost::python::object pyparent = boost::python::object(),
                            const char * caption = "")
{
    QWidget * parent  = NULL;
    if (pyparent != boost::python::object())
        parent = extract_widget<QWidget>(pyparent)();
    return editMaterialInDialog(initial,parent,caption);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(pyEditMaterialInDialog_overloads, pyEditMaterialInDialog, 1, 3)

static boost::python::object * __PYABORTFUNC = NULL;

bool pyDialogAbortFunc() {
    if(!__PYABORTFUNC)return false;
    else {
        PythonInterpreterAcquirer py;
        try{
            return extract<bool>((*__PYABORTFUNC)());
        }
        catch(bp::error_already_set) { PyErr_Print(); PyErr_Clear(); }
        return false;
    }
}



void pySetDialogAbortFunc(boost::python::object func) {
    if(__PYABORTFUNC) delete __PYABORTFUNC;
    if (func == boost::python::object()) {
        __PYABORTFUNC = NULL;
        ViewerApplication::setAborter(NULL);
    }
    else {
        __PYABORTFUNC = new boost::python::object(func);
        ViewerApplication::setAborter(&pyDialogAbortFunc);
    }
}

static boost::python::object * __PYQAPPINITIATOR = NULL;

void pyQAppInitiator() {
    if(!__PYQAPPINITIATOR)return ;
    else {
        try{
            (*__PYQAPPINITIATOR)();
        }
        catch(bp::error_already_set) { PyErr_Print(); PyErr_Clear(); }
    }
}
void py_register_qapp_initiator(boost::python::object func) {
    if(__PYQAPPINITIATOR) delete __PYQAPPINITIATOR;
    if (func == boost::python::object()) {
        __PYQAPPINITIATOR = NULL;
        register_qapp_initiator(NULL);
    }
    else {
        __PYQAPPINITIATOR = new boost::python::object(func);
        register_qapp_initiator(&pyQAppInitiator);
    }
}

boost::python::object PyWaitSelection(const std::string& message) {
    uint_t res = ViewerApplication::waitSelection(message);
    if (res == UINT32_MAX) return boost::python::object();
    else return boost::python::object(res);

}

#define LIGHTFUNCWRAP(COLNAME) \
void setLight##COLNAME(boost::python::object o){ \
  int r = extract<int>(o.attr("red"))(); \
  int g = extract<int>(o.attr("green"))(); \
  int b = extract<int>(o.attr("blue"))(); \
  ViewerApplication::setLight##COLNAME(r,g,b); \
} \
\
boost::python::object getLight##COLNAME(){ \
  int r,g,b; \
  ViewerApplication::getLight##COLNAME(r,g,b); \
  return boost::python::object(Color3(r,g,b)); \
} \

LIGHTFUNCWRAP(Ambient)
LIGHTFUNCWRAP(Diffuse)
LIGHTFUNCWRAP(Specular)



class PyViewCamera {
public :
    PyViewCamera();
};

class PyViewGrids {
public :
    PyViewGrids();
};

class PyViewDialog {
public :
    PyViewDialog();
};

class PyViewFrameGL {
public :
    PyViewFrameGL();
};

class PyViewWidgetGeometry {
public :
    PyViewWidgetGeometry();
};

class PyViewClippingPlanes {
public :
    PyViewClippingPlanes();
};

class PyViewLight {
public :
    PyViewLight();
};

void export_viewer()
{
  enum_<eAnimationFlag>("eAnimationFlag")
      .value("eStatic",eStatic)
      .value("eAnimatedPrimitives",eAnimatedPrimitives)
      .value("eAnimatedScene",eAnimatedScene)
      .export_values()
      ;

  ViewerApplication::registerThreadStateSaver<PyStateSaver>();

  def("getMaterialFromDialog", pyGetMaterialFromDialog, pyGetMaterialFromDialog_overloads());
  def("editMaterialInDialog", pyEditMaterialInDialog, pyEditMaterialInDialog_overloads());
  def("pgl_register_qapp_initiator",&py_register_qapp_initiator);

  scope viewer = class_< PGLViewerApplication >("Viewer", no_init )
    .add_static_property("selection",&selection,&setMSelection)
    .add_static_property("threaded",&ViewerApplication::isThreadUsed,&ViewerApplication::useThread)
    .add_static_property("redrawPolicy",&ViewerApplication::getRedrawPolicy,&ViewerApplication::setRedrawPolicy)

    .def("setBatchMode", &ViewerApplication::setBatchMode,"setBatchMode(bool enable) : No blocking message from the Viewer.",args("enable"))
    .staticmethod("setBatchMode")
    .def("wait", &ViewerApplication::wait, "wait(long timeout = MAXLONG) : wait for the viewer ending.", args("timeout"))
    .def("wait", &viewer_wait)
    .staticmethod("wait")
    .def("waitSelection", &PyWaitSelection,(bp::arg("caption")=std::string("")),"Wait for a selection in the viewer. Return selected id.")
    .staticmethod("waitSelection")
    .def("isRunning", &ViewerApplication::running,"Tell if viewer is curently running.")
    .staticmethod("isRunning")
    .def("stop", &ViewerApplication::stop,"Hide the Viewer")
    .staticmethod("stop")
    .def("hide", &ViewerApplication::stop,"Hide the Viewer")
    .staticmethod("hide")
    .def("start", &ViewerApplication::start,"Show the Viewer")
    .staticmethod("start")
    .def("show", &ViewerApplication::start,"Show the Viewer")
    .staticmethod("show")
    .def("showMessage", &ViewerApplication::showMessage,(bp::arg("message"),bp::arg("timeout")=0),"Show a message in the viewer. args: message and timeout")
    .staticmethod("showMessage")
    .def("exit", &ViewerApplication::exit,"Exit from Viewer Application. Cannot be restarted after.")
    .staticmethod("exit")
    .def("display",(void (*)(const ScenePtr&))&PGLViewerApplication::display,"display(Scene scene) : display a scene.",args("scene"))
    .def("display",(void (*)(const GeometryPtr&))&PGLViewerApplication::display,"display(Geometry geom) : display a particular geometry.",args("geom"))
    .def("display",&displaySh,"display(Shape sh) : display a particular shape.",args("sh"))
    .staticmethod("display")
    .def("add",(void (*)(const ScenePtr&))&PGLViewerApplication::add,"add(Scene scene) : add a scene to the current displayed scene.",args("scene"))
    .def("add",(void (*)(const GeometryPtr&))&PGLViewerApplication::add,"add(Geometry geom) : add a particular geometry to the current displayed scene.",args("geom"))
    .def("add",&addSh,"add(Shape sh) : add a particular shape to the current displayed scene.",args("sh"))
    .staticmethod("add")
    .def("getCurrentScene",&PGLViewerApplication::getCurrentScene,"getCurrentScene() : get the current displayed scene")
    .staticmethod("getCurrentScene")
    .def("update",&ViewerApplication::update,"update() : update the current visualization.")
    .staticmethod("update")
    .def("setAnimation",&setAnimation1,"setAnimation(flag = eStatic[|eAnimatedScene|eAnimatedPrimitives]) : Set viewer in animation mode [Minimal/No display list, No camera adjutement]. eAnimatedScene supposed that the number of element changes but previous primitives stay the same. eAnimatedPrimitives supposed that even individual primitives can changed and thus cache/display list are not reused.",(bp::arg("flag")=eStatic))
    .staticmethod("setAnimation")
    .def("animation",&animation1,"deprecated",(bp::arg("enabled")=true))
    .staticmethod("animation")
    .def("winId",&ViewerApplication::viewerId)
    .staticmethod("winId")
    .def("setDialogAbortFunc",&pySetDialogAbortFunc,args("func"),"Set a function that return true if dialog should abort")
    .staticmethod("setDialogAbortFunc")
    .def("saveSnapshot",&saveImage1)
    .def("saveSnapshot",&saveImage2)
    .def("saveSnapshot",&ViewerApplication::saveImage,"saveImage(str filename, str type='PNG', bool withAlpha = False). Compatibility with PyQGLViewer.", args("filename","type","withAlpha"))
    .staticmethod("saveSnapshot")
    ;

  export_camera();
  export_grids();
  export_clippingplanes();
  export_dialog();
  export_framegl();
  export_widgetgeometry();
  export_light();


}

Vector3 getCameraPosition(){
    Vector3 pos, h, up;
    ViewerApplication::getCamera(pos,h,up);
    return pos;
}

void setCameraPosition(const Vector3& pos){
    ViewerApplication::setCamera(pos);
}

object getCameraPositionInfo(){
    Vector3 pos, h, up;
    ViewerApplication::getCamera(pos,h,up);
    return boost::python::make_tuple(pos,h,up);
}

void lookAt1 (const Vector3& pos) { ViewerApplication::lookAt(pos); }
void lookAt3 (const Vector3& pos,const Vector3& target) { ViewerApplication::lookAt(pos,target); }

void export_camera(){
    class_<PyViewCamera >("camera", no_init )
        .def("setPerspective",&ViewerApplication::setPerspectiveCamera,"setPerspective() : Set Camera in Perspective mode.")
    .staticmethod("setPerspective")
    .def("setOrthographic",&ViewerApplication::setOrthographicCamera,"setOrthographic() : Set Camera in Orthographic mode.")
    .staticmethod("setOrthographic")
    .def("getPosition",&getCameraPositionInfo)
    .staticmethod("getPosition")
    .def("setPosition",(void(*)(const Vector3&))&ViewerApplication::setCamera)
    .staticmethod("setPosition")
    .add_static_property("position",&getCameraPosition,&setCameraPosition)
    .def("set",(void(*)(const Vector3&,real_t,real_t))&ViewerApplication::setCamera,"set(Vector3 pos, float elevation, float azimut)", args("pos","elevation","azimut"))
    .staticmethod("set")
    .def("lookAt",(void(*)(const Vector3&))&ViewerApplication::lookAt,"lookAt(Vector3 target)",args("target"))
    .def("lookAt",(void(*)(const Vector3&,const Vector3&))&ViewerApplication::lookAt,"lookAt(Vector3 pos, Vector3 target)",args("pos","target"))
    .staticmethod("lookAt")
    .def("getViewAngle",&ViewerApplication::getViewAngle)
    .staticmethod("getViewAngle")
    .def("setViewAngle",&ViewerApplication::setViewAngle)
    .staticmethod("setViewAngle")
    ;
}

void export_grids(){
    class_<PyViewGrids >("grids", no_init )
    .def("setXYPlane",&ViewerApplication::setXYGrid,"setXYPlane(bool enable)",args("enable") )
    .staticmethod("setXYPlane")
    .def("setXZPlane",&ViewerApplication::setXZGrid,"setXYPlane(bool enable)",args("enable"))
    .staticmethod("setXZPlane")
    .def("setYZPlane",&ViewerApplication::setYZGrid,"setXYPlane(bool enable)",args("enable"))
    .staticmethod("setYZPlane")
    .def("setAxis",&ViewerApplication::setAxis,"setAxis(bool enable)",args("enable"))
    .staticmethod("setAxis")
    .def("setUnit",&ViewerApplication::setGridUnit,"setUnit(int value)",args("value"))
    .staticmethod("setUnit")
    .def("setSize",&ViewerApplication::setGridSize,"setSize(int value)",args("value"))
    .staticmethod("setSize")
    .def("set",&ViewerApplication::setGrid,"set(bool xy, bool xz, bool yz, bool axis)",args("xy","xz","yz","axis"))
    .staticmethod("set")
    ;
}

void export_clippingplanes(){
    class_<PyViewClippingPlanes >("clippingPlanes", no_init )
    .def("activate",&ViewerApplication::activateCP,"activate(int planeid, bool enable)",args("planeid","enable") )
    .staticmethod("activate")
    .def("set",&ViewerApplication::setCP,"set(int planeid, double a, double b, double c, double d)",args("planeid","a","b","c","d") )
    .staticmethod("set")
    ;
}

void export_dialog(){
    class_<PyViewDialog> ("dialog", no_init )
    .def("question",&question00,"question(str text)",args("text"))
    .def("question",&question0,"question(str caption, str text)",args("caption","text"))
    .def("question",&question1,"question(str caption, str text, str button1)",args("caption","text","button1"))
    .def("question",&question2,"question(str caption, str text, str button1, str button2)",args("caption","text","button1","button2"))
    .def("question",&question3,"question(str caption, str text, str button1, str button2, str button3)",args("caption","text","button1","button2","button3"))
    .staticmethod("question")
    .def("itemSelection",&itemSelection,"itemSelection(str caption, str text, list values, bool editable = False)",args("caption","text","values","editable"))
    .def("itemSelection",&itemSelectionNE,args("caption","text","values"))
    .def("itemSelection",&itemSelection2,args("text","values"))
    .def("itemSelection",&itemSelection1,args("values"))
    .staticmethod("itemSelection")
    .def("doubleSelection",&doubleSelection,"doubleSelection([str caption, str text, double value, double minvalue, double maxvalue])",args("caption","text","value","minvalue","maxvalue"))
    .def("doubleSelection",&doubleSelection3,args("caption","text","value"))
    .def("doubleSelection",&doubleSelection2,args("caption","text"))
    .def("doubleSelection",&doubleSelection1,args("text"))
    .def("doubleSelection",&doubleSelection0)
    .staticmethod("doubleSelection")
    .def("getOpenFile",&getOpenFile0)
    .def("getOpenFile",&getOpenFile1,args("caption"))
    .def("getOpenFile",&getOpenFile2,args("caption","filter"))
    .def("getOpenFile",&ViewerApplication::getOpenFile,"getOpenFile(str caption = \"\", str filter = \"\", str startPath = \"\")",args("caption","filter","startPath"))
    .staticmethod("getOpenFile")
    .def("getSaveFile",&getSaveFile0)
    .def("getSaveFile",&getSaveFile1,args("caption"))
    .def("getSaveFile",&getSaveFile2,args("caption","filter"))
    .def("getSaveFile",&ViewerApplication::getSaveFile,"getSaveFile(str caption = \"\", str filter = \"\", str startPath = \"\")",args("caption","filter","startPath"))
    .staticmethod("getSaveFile")
    .def("getDirectory",&getDirectory0)
    .def("getDirectory",&getDirectory1,args("caption"))
    .def("getDirectory",&ViewerApplication::getDirectory,"getDirectory(str caption = \"\", str startPath = \"\")",args("caption","startPath"))
    .staticmethod("getDirectory")
    ;
}
void export_framegl(){
    class_<PyViewFrameGL> ("frameGL", no_init )
    .def("saveImage",&saveImage1)
    .def("saveImage",&saveImage2)
    .def("saveImage",&ViewerApplication::saveImage,"saveImage(str filename, str type='PNG', bool withAlpha = False)", args("filename","type","withAlpha"))
    .staticmethod("saveImage")
    .def("setBgColor",&ViewerApplication::setBgColor,"setBgColor(int red, int green, int blue)",args("red","green","blue"))
    .def("setBgColor",&setBGColorO,"setBgColor(Color color)",args("color"))
    .staticmethod("setBgColor")
    .def("setSize",&ViewerApplication::glFrameSize,"setSize(int width, int height)",args("width","height"))
    .staticmethod("setSize")
    .def("maximize",&glFrameOnly1,"maximize(bool enable = True) : maximize FrameGL size into the Viewer.",args("enable"))
    .def("maximize",&glFrameOnly0)
    .staticmethod("maximize")
    .def("castRays",&castRays,"castRays(Vector3 pos, Vector3 dir, Vector3 dx, Vector3 dy, float sx, float sy)",args("pos","dir","dx","dy","sx","sy") /*,return_value_policy<manage_new_object>()*/)
    .staticmethod("castRays")
    .def("castRays2",&castRays2,"castRays2(Scene scene [, bool back_test = True])",args("scene","back_test"))
    .def("castRays2",&castRays2_1)
    .staticmethod("castRays2")
    .def("grabZBuffer",&grabZBuffer0 )
    .def("grabZBuffer",&grabZBuffer  ,"grabZBuffer(allvalues = False)", args("allvalues") )
    .staticmethod("grabZBuffer")
    .def("grabZBufferPoints",&grabZBufferPoints)
    .def("grabZBufferPoints",&grabZBufferPoints2, (bp::arg("jitter")=0,bp::arg("raywidth")=0,bp::arg("color")=true), "grabZBufferPoints(jitter, raywidth, mixcolor)\nReturn the ZBuffer as 3D points.\n\nParameters\n---------\n\tjitter : float\n\t\tAdd some uniform noise in the view direction on the position of points\n\traywidth : int\n\t\tSuppose a width for each ray and compute the position of each pixel as the mean of the neighboring pixels. Neighborhood is defined as all pixels at a distance of central pixel lower or equal to raywidth.\n\tmixcolor : bool\n\t\tSpecify whether the colors of the pixels should be a blend of the neighborhood colors in case of raywidth > 0\n\nReturns\n-------\n\tPoint3Array\n\t\tThe position of the points.\n\tColor4Array\n\t\tThe colors of the points." )
    .staticmethod("grabZBufferPoints")
    .def("getProjectionSize",&getProjectionSize,"return projected_size,pixel_nb,pixel_size : compute the projected size of the displayed scene onto the current camera." )
    .staticmethod("getProjectionSize")
    .def("getProjectionSizes",&getProjectionSizes,"getProjectionSizes(Scene objects) : individual projected sizes of a set of elements.",args("objects"))
    .staticmethod("getProjectionSizes")
    .def("getProjectionPerShape",&getProjectionPerShape,"getProjectionPerShape() : projected sizes of a set of elements per elements.")
    .staticmethod("getProjectionPerShape")
    ;
}

void export_widgetgeometry(){
    class_<PyViewWidgetGeometry> ("widgetGeometry", no_init )
    .def("setFullScreen",&fullScreen0)
    .def("setFullScreen",&fullScreen1,"setFullScreen(bool enable)",args("enable"))
    .staticmethod("setFullScreen")
    .def("setSize",&ViewerApplication::resize,"setSize(int width, int height)",args("width","height"))
    .staticmethod("setSize")
    .def("setPosition",&ViewerApplication::move,"setPosition(int x, int y)",args("x","y"))
    .staticmethod("setPosition")
    .def("set",&ViewerApplication::setGeometry,"set(int x, int y, int width, int height)",args("x","y","width","height"))
    .staticmethod("set")
    ;
}

void export_light(){
    class_<PyViewLight >("light", no_init )
    .add_static_property("enabled",&ViewerApplication::isLightEnabled,&ViewerApplication::setLightEnabled )
    .add_static_property("position",&ViewerApplication::getLightPosition,&ViewerApplication::setLightPosition )
    .add_static_property("ambient",&getLightAmbient,&setLightAmbient)
    .add_static_property("diffuse",&getLightDiffuse,&setLightDiffuse)
    .add_static_property("specular",&getLightSpecular,&setLightSpecular)
    ;
}
void initViewer()
{
    PGLViewerApplication::init();
}


void ShutDownViewer()
{
    ViewerApplication::cleanThreadStateSaverFatory();
    ViewerApplication::exit();
}

void cleanViewer()
{
  static bool RegisterCleanViewer = false;
  if (! RegisterCleanViewer)
  {
      Py_AtExit( &ShutDownViewer );
      RegisterCleanViewer = true;
  }
}


void module_gui()
{
  initViewer();

  export_viewer();

  cleanViewer();
  export_icon();
}


