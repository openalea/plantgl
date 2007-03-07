/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       GeomPy: Python wrapper for the Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR AMAP 
 *
 *       File author(s): C. Pradal (christophe.pradal@cirad.fr)
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

#include "export_viewer.h"
#include "../util/extract_list.h"

#include <boost/python.hpp>

#include <gui/viewer/pglapplication.h>
#include <gui/base/zbuffer.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

boost::python::list selection(){
  std::vector<uint32_t> sel = ViewerApplication::getSelection();
  if(sel.empty()) return boost::python::list();
  else {
	boost::python::list res;
	for(vector<uint32_t>::const_iterator _it = sel.begin();
	_it != sel.end(); _it++){
	  res.append(object((int)*_it));
	}
	return res;
  }
}

void setSelection(int i){
  std::vector<uint32_t> sel;
  sel.push_back(i);
  ViewerApplication::setSelection(sel);
}

void setMSelection(boost::python::list values){
  std::vector<uint32_t> sel = extract_vec<uint32_t>(values)();
  ViewerApplication::setSelection(sel);
}

int question0(const std::string& caption,
			  const std::string& text){
  return ViewerApplication::question(caption,text);
}

int question1(const std::string& caption,
			  const std::string& text,
			  const std::string& but0txt){
  return ViewerApplication::question(caption,text,but0txt);
}

int question2(const std::string& caption,
			  const std::string& text,
			  const std::string& but0txt,
			  const std::string& but1txt){
  return ViewerApplication::question(caption,text,but0txt,but1txt);
}

int question3(const std::string& caption,
			  const std::string& text,
			  const std::string& but0txt,
			  const std::string& but1txt,
			  const std::string& but2txt){
  return ViewerApplication::question(caption,text,but0txt,but1txt,but2txt);
}

object itemSelection(const std::string& caption,
					 const std::string& text,
					 const boost::python::list& values,
					 bool editable){
  std::vector<std::string> vals = extract_vec<std::string>(values)();
  bool ok = false;
  std::string res = ViewerApplication::itemSelection(caption,text,vals,ok,editable);
  tuple t = make_tuple(ok,res);
  return t;
}

object itemSelectionNE(const std::string& caption,
					 const std::string& text,
					 const boost::python::list& values){
  return itemSelection(caption,text,values,false);
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

void animation1(int b){
  ViewerApplication::animation(b);
}

void animation0(){
  ViewerApplication::animation(true);
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
	for(size_t i = 0; i < buf->getColsSize();i++){
		boost::python::list row;
		for(size_t j = 0; j < buf->getRowsSize();j++){
			boost::python::list zlist;
			for(size_t k = 0; k < buf->getAt(i,j).size();k++){
				const RayHit& inter = buf->getAt(i,j)[k];
				zlist.append(make_tuple(inter.id,inter.zmax,inter.zmin));
			}
			row.append(zlist);
		}
		res.append(row);
	}
	return res;
}

boost::python::object castRays(const TOOLS(Vector3)& pos, 
								  const TOOLS(Vector3)& dir,
							      const TOOLS(Vector3)& dx, 
							      const TOOLS(Vector3)& dy,
								  int sx, int sy){
	ViewRayBuffer * buf = ViewerApplication::castRays(pos,dir,dx,dy,sx,sy);
	boost::python::object res = raybuf_to_python(buf);
	delete buf;
	return res;
}

boost::python::object zbuf_to_python(ViewZBuffer * buf, bool allvalues) {
	boost::python::list res;
	for(size_t i = 0; i < buf->getColsSize();i++){
		boost::python::list row;
		for(size_t j = 0; j < buf->getRowsSize();j++){
			const ZBufferUnit& unit = buf->getAt(i,j);
			if (allvalues || (0.0 < unit.depth && unit.depth < 1.0))
				row.append(make_tuple(unit.pos,unit.color,unit.depth));
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

boost::python::object getProjectionSize(){
	int nbpix;
	double pixwidth;
	double size = ViewerApplication::getProjectionSize(&nbpix,&pixwidth);
	return make_tuple(size,nbpix,pixwidth);
}

boost::python::object getProjectionSizes(const ScenePtr& sc){
  std::vector<std::pair<uint32_t,double> > res = PGLViewerApplication::getProjectionSizes(sc);
  if(res.empty()) return object();
  else {
	boost::python::list bres;
	for(vector<std::pair<uint32_t,double> >::const_iterator _it = res.begin();
	_it != res.end(); _it++){
	  bres.append(make_tuple((int)(_it->first),_it->second));
	}
	return bres;
  }
}

boost::python::object raypointhitbuf_to_python(ViewRayPointHitBuffer * buf) {
	boost::python::list res;
	for(size_t i = 0; i < buf->getColsSize();i++){
		boost::python::list row;
		for(size_t j = 0; j < buf->getRowsSize();j++){
			boost::python::list zlist;
			for(size_t k = 0; k < buf->getAt(i,j).size();k++){
				const RayPointHit& inter = buf->getAt(i,j)[k];
				zlist.append(make_tuple(inter.id,inter.zmax,inter.zmin));
			}
			row.append(zlist);
		}
		res.append(row);
	}
	return res;
}

boost::python::object castRays2(const ScenePtr& sc, bool back_test){
	ViewRayPointHitBuffer * buf = PGLViewerApplication::castRays2(sc,back_test);
	boost::python::object res = raypointhitbuf_to_python(buf);
	delete buf;
	return res;
}

boost::python::object castRays2_1(const ScenePtr& sc){
	ViewRayPointHitBuffer * buf = PGLViewerApplication::castRays2(sc,true);
	boost::python::object res = raypointhitbuf_to_python(buf);
	delete buf;
	return res;
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

void export_viewer()
{
  scope viewer = class_< PGLViewerApplication >("Viewer", no_init )
	.add_static_property("selection",&selection,&setMSelection)
	.add_static_property("threaded",&ViewerApplication::isThreadUsed,&ViewerApplication::useThread)

	.def("setBashMode", &ViewerApplication::setBashMode,"setBashMode(bool enable) : No blocking message from the Viewer.",args("enable"))
    .staticmethod("setBashMode")
	.def("wait", &ViewerApplication::wait, "wait(long timeout = MAXLONG) : wait for the viewer ending.", args("timeout"))
    .def("wait", &viewer_wait)
    .staticmethod("wait")
    .def("isRunning", &ViewerApplication::running,"Tell if viewer is curently running.")
    .staticmethod("isRunning")
    .def("stop", &ViewerApplication::stop,"Stop Viewer")
    .staticmethod("stop")
    .def("start", &ViewerApplication::start,"Start Viewer")
    .staticmethod("start")
    .def("exit", &ViewerApplication::exit,"Exit from Viewer Application. Cannot be restarted after.")
    .staticmethod("exit")
	.def("display",(void (*)(const ScenePtr&))&PGLViewerApplication::display,"display(Scene scene) : display a scene.",args("scene"))
    .def("display",(void (*)(const GeometryPtr&))&PGLViewerApplication::display,"display(Geometry geom) : display a particular geometry.",args("geom"))
    .staticmethod("display")
    .def("add",(void (*)(const ScenePtr&))&PGLViewerApplication::add,"add(Scene scene) : add a scene to the current displayed scene.",args("scene"))
    .def("add",(void (*)(const GeometryPtr&))&PGLViewerApplication::add,"add(Geometry geom) : add a particular geometry to the current displayed scene.",args("geom"))
    .staticmethod("add")
    .def("getCurrentScene",&PGLViewerApplication::getCurrentScene,"getCurrentScene() : get the current displayed scene")
    .staticmethod("getCurrentScene")
	.def("update",&ViewerApplication::update,"update() : update the current visualization.")
    .staticmethod("update")
	.def("animation",&animation1,"animation(bool enable = True) : Set viewer in animation mode [No display list, No camera adjutement]",args("enable"))
    .def("animation",&animation0)
    .staticmethod("animation") 
    ;

  export_camera();
  export_grids();
  export_clippingplanes();
  export_dialog();
  export_framegl();
  export_widgetgeometry();

}

Vector3 getCameraPosition(){
	Vector3 pos, h, up;
	ViewerApplication::getCamera(pos,h,up);
	return pos;
}

object getCameraPositionInfo(){
	Vector3 pos, h, up;
	ViewerApplication::getCamera(pos,h,up);
	return make_tuple(pos,h,up);
}

void export_camera(){
	class_<PyViewCamera >("camera", no_init )
		.def("setPerspective",&ViewerApplication::setPerspectiveCamera,"setPerspective() : Set Camera in Perspective mode.")
    .staticmethod("setPerspective")
    .def("setOrthographic",&ViewerApplication::setOrthographicCamera,"setOrthographic() : Set Camera in Orthographic mode.")
    .staticmethod("setOrthographic")
    .def("getPosition",&getCameraPositionInfo)
    .staticmethod("getPosition")
	.add_static_property("position",&getCameraPosition,(void(*)(const TOOLS(Vector3)&))&ViewerApplication::setCamera)
    .def("set",(void(*)(const TOOLS(Vector3)&,real_t,real_t))&ViewerApplication::setCamera,"set(Vector3 pos, float elevation, float azimut)", args("pos","elevation","azimut"))
    .staticmethod("set")
    .def("lookAt",(void(*)(const TOOLS(Vector3)&))&ViewerApplication::lookAt,"lookAt(Vector3 target)",args("target"))
    .def("lookAt",(void(*)(const TOOLS(Vector3)&,const TOOLS(Vector3)&))&ViewerApplication::lookAt,"lookAt(Vector3 pos, Vector3 target)",args("pos","target"))
    .staticmethod("lookAt")
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
    .def("question",&question0,"question(str caption, str text)",args("caption","text"))
    .def("question",&question1,"question(str caption, str text, str button1)",args("caption","text","button1"))
    .def("question",&question2,"question(str caption, str text, str button1, str button2)",args("caption","text","button1","button2"))
    .def("question",&question3,"question(str caption, str text, str button1, str button2, str button3)",args("caption","text","button1","button2","button3"))
    .staticmethod("question")
    .def("itemSelection",&itemSelection,"itemSelection(str caption, str text, list values, bool editable = False)",args("caption","text","values","editable"))
    .def("itemSelection",&itemSelectionNE,args("caption","text","values"))
    .staticmethod("itemSelection")
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
	.def("getProjectionSize",&getProjectionSize,"return projected_size,pixel_nb,pixel_size : compute the projected size of the displayed scene onto the current camera." )
    .staticmethod("getProjectionSize")
	.def("getProjectionSizes",&getProjectionSizes,"getProjectionSizes(Scene objects) : individual projected sizes of a set of elements.",args("objects"))
    .staticmethod("getProjectionSizes")
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

void initViewer()
{
	PGLViewerApplication::init();
}

void endViewer()
{
	ViewerApplication::exit();
}

void cleanViewer() 
{
  static bool RegisterCleanViewer = false;
  if (! RegisterCleanViewer)
  {
	  Py_AtExit( &endViewer );
	  RegisterCleanViewer = true;
  }
}

void module_gui()
{
  initViewer();

  export_viewer();

  cleanViewer();
}


