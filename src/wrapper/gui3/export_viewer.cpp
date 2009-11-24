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

#include <boost/python.hpp>

#include <plantgl/gui3/viewer/pglapplication.h>
#include <plantgl/gui3/base/zbuffer.h>

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

boost::python::list selection(){
  std::vector<uint_t> sel = ViewerApplication3::getSelection();
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
  ViewerApplication3::setSelection(sel);
}

void setMSelection(boost::python::list values){
  std::vector<uint_t> sel;
  try { 
    object iter_obj = boost::python::object( handle<>( PyObject_GetIter( values.ptr() ) ) );
    while( true ) {
          object obj = iter_obj.attr( "next" )();
		  int val = boost::python::extract<int>( obj );
          sel.push_back( val );
       }
    }
	catch( error_already_set )
    {
      PyErr_Clear();
    }

  ViewerApplication3::setSelection(sel);
}

int question0(const std::string& caption,
			  const std::string& text){
  return ViewerApplication3::question(caption,text);
}

int question1(const std::string& caption,
			  const std::string& text,
			  const std::string& but0txt){
  return ViewerApplication3::question(caption,text,but0txt);
}

int question2(const std::string& caption,
			  const std::string& text,
			  const std::string& but0txt,
			  const std::string& but1txt){
  return ViewerApplication3::question(caption,text,but0txt,but1txt);
}

int question3(const std::string& caption,
			  const std::string& text,
			  const std::string& but0txt,
			  const std::string& but1txt,
			  const std::string& but2txt){
  return ViewerApplication3::question(caption,text,but0txt,but1txt,but2txt);
}

object itemSelection(const std::string& caption,
					 const std::string& text,
					 const boost::python::list& values,
					 bool editable){
  std::vector<std::string> vals;
  try { 
    object iter_obj = boost::python::object( handle<>( PyObject_GetIter( values.ptr() ) ) );
    while( true ) {
          object obj = iter_obj.attr( "next" )();
		  std::string val = boost::python::extract<std::string>( obj );
          vals.push_back( val );
       }
    }
  catch( error_already_set )
    {
      PyErr_Clear();
    }
  bool ok = false;
  std::string res = ViewerApplication3::itemSelection(caption,text,vals,ok,editable);
  tuple t = make_tuple(ok,res);
  return t;
}

object itemSelectionNE(const std::string& caption,
					 const std::string& text,
					 const boost::python::list& values){
  return itemSelection(caption,text,values,false);
}

void fullScreen0(){
  ViewerApplication3::fullScreen();
}

void fullScreen1(int b){
  ViewerApplication3::fullScreen(b);
}

void glFrameOnly0(){
  ViewerApplication3::glFrameOnly();
}

void glFrameOnly1(int b){
  ViewerApplication3::glFrameOnly(b);
}

void animation1(int b){
  ViewerApplication3::animation(b);
}

void animation0(){
  ViewerApplication3::animation(true);
}

void setBGColorO(boost::python::object o){
  int r = extract<int>(o.attr("red"))();
  int g = extract<int>(o.attr("green"))();
  int b = extract<int>(o.attr("blue"))();
  ViewerApplication3::setBgColor(r,g,b);
}

std::string getOpenFile0(){
  return ViewerApplication3::getOpenFile("Open File","","");
}

std::string getOpenFile1(const std::string& caption){
  return ViewerApplication3::getOpenFile(caption,"","");
}

std::string getOpenFile2(const std::string& caption, const std::string& filter){
  return ViewerApplication3::getOpenFile(caption,filter,"");
}

std::string getSaveFile0(){
  return ViewerApplication3::getSaveFile("Save File","","");
}

std::string getSaveFile1(const std::string& caption){
  return ViewerApplication3::getSaveFile(caption,"","");
}

std::string getSaveFile2(const std::string& caption, const std::string& filter){
  return ViewerApplication3::getSaveFile(caption,filter,"");
}

std::string getDirectory0(){
  return ViewerApplication3::getDirectory("Choose Directory","");
}

std::string getDirectory1(const std::string& caption){
  return ViewerApplication3::getDirectory(caption,"");
}

boost::python::object raybuf_to_python(ViewRayBuffer3 * buf) {
	boost::python::list res;
	for(size_t i = 0; i < buf->getColsSize();i++){
		boost::python::list row;
		for(size_t j = 0; j < buf->getRowsSize();j++){
			boost::python::list zlist;
			for(size_t k = 0; k < buf->getAt(i,j).size();k++){
				const RayHit3& inter = buf->getAt(i,j)[k];
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
	ViewRayBuffer3 * buf = ViewerApplication3::castRays(pos,dir,dx,dy,sx,sy);
	boost::python::object res = raybuf_to_python(buf);
	delete buf;
	return res;
}

boost::python::object zbuf_to_python(ViewZBuffer3 * buf, bool allvalues) {
	boost::python::list res;
	for(size_t i = 0; i < buf->getColsSize();i++){
		boost::python::list row;
		for(size_t j = 0; j < buf->getRowsSize();j++){
			const ZBufferUnit3& unit = buf->getAt(i,j);
			if (allvalues || (0.0 < unit.depth && unit.depth < 1.0))
				row.append(make_tuple(unit.pos,unit.color,unit.depth));
			else row.append(object());
		}
		res.append(row);
	}
	return res;
}
boost::python::object grabZBuffer(bool allvalues){
	ViewZBuffer3 * buf = ViewerApplication3::grabZBuffer();
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
	double size = ViewerApplication3::getProjectionSize(&nbpix,&pixwidth);
	return make_tuple(size,nbpix,pixwidth);
}

boost::python::object getProjectionSizes(const ScenePtr& sc){
  std::vector<std::pair<uint_t,double> > res = PGLViewerApplication3::getProjectionSizes(sc);
  if(res.empty()) return object();
  else {
	boost::python::list bres;
	for(vector<std::pair<uint_t,double> >::const_iterator _it = res.begin();
	_it != res.end(); _it++){
	  bres.append(make_tuple((int)(_it->first),_it->second));
	}
	return bres;
  }
}

boost::python::object raypointhitbuf_to_python(ViewRayPointHitBuffer3 * buf) {
	boost::python::list res;
	for(size_t i = 0; i < buf->getColsSize();i++){
		boost::python::list row;
		for(size_t j = 0; j < buf->getRowsSize();j++){
			boost::python::list zlist;
			for(size_t k = 0; k < buf->getAt(i,j).size();k++){
				const RayPointHit3& inter = buf->getAt(i,j)[k];
				zlist.append(make_tuple(inter.id,inter.zmax,inter.zmin));
			}
			row.append(zlist);
		}
		res.append(row);
	}
	return res;
}

boost::python::object castRays2(const ScenePtr& sc, bool back_test){
	ViewRayPointHitBuffer3 * buf = PGLViewerApplication3::castRays2(sc,back_test);
	boost::python::object res = raypointhitbuf_to_python(buf);
	delete buf;
	return res;
}

boost::python::object castRays2_1(const ScenePtr& sc){
	ViewRayPointHitBuffer3 * buf = PGLViewerApplication3::castRays2(sc,true);
	boost::python::object res = raypointhitbuf_to_python(buf);
	delete buf;
	return res;
}

void saveImage1(const std::string& fname)
{
	// ext = get_extension(fname);
	// if (ext.empty()) ext = "PNG";
	// else ext = toUpper(ext);
	ViewerApplication3::saveImage(fname,"PNG");
}
void saveImage2(const std::string& fname, const std::string& type)
{
	ViewerApplication3::saveImage(fname,type);
}
bool viewer_wait(){
	return ViewerApplication3::wait();
}

class PyViewCamera3 {
public :
	PyViewCamera3();
};

class PyViewGrids3 {
public :
	PyViewGrids3();
};

class PyViewDialog3 {
public :
	PyViewDialog3();
};

class PyViewFrameGL3 {
public :
	PyViewFrameGL3();
};

class PyViewWidgetGeometry3 {
public :
	PyViewWidgetGeometry3();
};

void export_viewer()
{
  scope viewer = class_< PGLViewerApplication3 >("Viewer", no_init )
	.add_static_property("selection",&selection,&setMSelection)
	.add_static_property("threaded",&ViewerApplication3::isThreadUsed,&ViewerApplication3::useThread)

	.def("setBashMode", &ViewerApplication3::setBashMode,"setBashMode(bool enable) : No blocking message from the Viewer.",args("enable"))
    .staticmethod("setBashMode")
	.def("wait", &ViewerApplication3::wait, "wait(long timeout = MAXLONG) : wait for the viewer ending.", args("timeout"))
    .def("wait", &viewer_wait)
    .staticmethod("wait")
    .def("isRunning", &ViewerApplication3::running,"Tell if viewer is curently running.")
    .staticmethod("isRunning")
    .def("stop", &ViewerApplication3::stop,"Stop Viewer")
    .staticmethod("stop")
    .def("start", &ViewerApplication3::start,"Start Viewer")
    .staticmethod("start")
    .def("exit", &ViewerApplication3::exit,"Exit from Viewer Application. Cannot be restarted after.")
    .staticmethod("exit")
	.def("display",(void (*)(const ScenePtr&))&PGLViewerApplication3::display,"display(Scene scene) : display a scene.",args("scene"))
    .def("display",(void (*)(const GeometryPtr&))&PGLViewerApplication3::display,"display(Geometry geom) : display a particular geometry.",args("geom"))
    .staticmethod("display")
    .def("add",(void (*)(const ScenePtr&))&PGLViewerApplication3::add,"add(Scene scene) : add a scene to the current displayed scene.",args("scene"))
    .def("add",(void (*)(const GeometryPtr&))&PGLViewerApplication3::add,"add(Geometry geom) : add a particular geometry to the current displayed scene.",args("geom"))
    .staticmethod("add")
    .def("getCurrentScene",&PGLViewerApplication3::getCurrentScene,"getCurrentScene() : get the current displayed scene")
    .staticmethod("getCurrentScene")
	.def("update",&ViewerApplication3::update,"update() : update the current visualization.")
    .staticmethod("update")
	.def("animation",&animation1,"animation(bool enable = True) : Set viewer in animation mode [No display list, No camera adjutement]",args("enable"))
    .def("animation",&animation0)
    .staticmethod("animation") 
    ;

  export_camera();
  export_grids();
  export_dialog();
  export_framegl();
  export_widgetgeometry();

}

void export_camera(){
	class_<PyViewCamera3 >("camera", no_init )
		.def("setPerspective",&ViewerApplication3::setPerspectiveCamera,"setPerspective() : Set Camera in Perspective mode.")
    .staticmethod("setPerspective")
    .def("setOrthographic",&ViewerApplication3::setOrthographicCamera,"setOrthographic() : Set Camera in Orthographic mode.")
    .staticmethod("setOrthographic")
    .def("setPosition",(void(*)(const TOOLS(Vector3)&))&ViewerApplication3::setCamera, "setPosition(Vector3 pos)",args("pos"))
    .staticmethod("setPosition")
    .def("set",(void(*)(const TOOLS(Vector3)&,real_t,real_t))&ViewerApplication3::setCamera,"set(Vector3 pos, float elevation, float azimut)", args("pos","elevation","azimut"))
    .staticmethod("set")
    .def("lookAt",(void(*)(const TOOLS(Vector3)&))&ViewerApplication3::lookAt,"lookAt(Vector3 target)",args("target"))
    .def("lookAt",(void(*)(const TOOLS(Vector3)&,const TOOLS(Vector3)&))&ViewerApplication3::lookAt,"lookAt(Vector3 pos, Vector3 target)",args("pos","target"))
    .staticmethod("lookAt")
	;
}

void export_grids(){
	class_<PyViewGrids3 >("grids", no_init )
    .def("setXYPlane",&ViewerApplication3::setXYGrid,"setXYPlane(bool enable)",args("enable") )
    .staticmethod("setXYPlane")
    .def("setXZPlane",&ViewerApplication3::setXZGrid,"setXYPlane(bool enable)",args("enable"))
    .staticmethod("setXZPlane")
    .def("setYZPlane",&ViewerApplication3::setYZGrid,"setXYPlane(bool enable)",args("enable"))
    .staticmethod("setYZPlane")
    .def("setAxis",&ViewerApplication3::setAxis,"setAxis(bool enable)",args("enable"))
    .staticmethod("setAxis")
    .def("setUnit",&ViewerApplication3::setGridUnit,"setUnit(int value)",args("value"))
    .staticmethod("setUnit")
    .def("setSize",&ViewerApplication3::setGridSize,"setSize(int value)",args("value"))
    .staticmethod("setSize")
    .def("set",&ViewerApplication3::setGrid,"set(bool xy, bool xz, bool yz, bool axis)",args("xy","xz","yz","axis"))
    .staticmethod("set")
	;
}
void export_dialog(){
	class_<PyViewDialog3> ("dialog", no_init )
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
    .def("getOpenFile",&ViewerApplication3::getOpenFile,"getOpenFile(str caption = \"\", str filter = \"\", str startPath = \"\")",args("caption","filter","startPath"))
    .staticmethod("getOpenFile")
    .def("getSaveFile",&getSaveFile0)
    .def("getSaveFile",&getSaveFile1,args("caption"))
    .def("getSaveFile",&getSaveFile2,args("caption","filter"))
    .def("getSaveFile",&ViewerApplication3::getSaveFile,"getSaveFile(str caption = \"\", str filter = \"\", str startPath = \"\")",args("caption","filter","startPath"))
    .staticmethod("getSaveFile")
    .def("getDirectory",&getDirectory0)
    .def("getDirectory",&getDirectory1,args("caption"))
    .def("getDirectory",&ViewerApplication3::getDirectory,"getDirectory(str caption = \"\", str startPath = \"\")",args("caption","startPath"))
    .staticmethod("getDirectory")
	;
}
void export_framegl(){
	class_<PyViewFrameGL3> ("frameGL", no_init )
    .def("saveImage",&saveImage1)
    .def("saveImage",&saveImage2)
    .def("saveImage",&ViewerApplication3::saveImage,"saveImage(str filename, str type='PNG', bool withAlpha = False)", args("filename","type","withAlpha"))
    .staticmethod("saveImage")
    .def("setBgColor",&ViewerApplication3::setBgColor,"setBgColor(int red, int green, int blue)",args("red","green","blue"))
    .def("setBgColor",&setBGColorO,"setBgColor(Color color)",args("color"))
    .staticmethod("setBgColor")
    .def("setSize",&ViewerApplication3::glFrameSize,"setSize(int width, int height)",args("width","height"))
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
	class_<PyViewWidgetGeometry3> ("widgetGeometry", no_init )
    .def("setFullScreen",&fullScreen0)
    .def("setFullScreen",&fullScreen1,"setFullScreen(bool enable)",args("enable"))
    .staticmethod("setFullScreen")
    .def("setSize",&ViewerApplication3::resize,"setSize(int width, int height)",args("width","height"))
    .staticmethod("setSize")
    .def("setPosition",&ViewerApplication3::move,"setPosition(int x, int y)",args("x","y"))
    .staticmethod("setPosition")
    .def("set",&ViewerApplication3::setGeometry,"set(int x, int y, int width, int height)",args("x","y","width","height"))
    .staticmethod("set")
	;
}

void initViewer()
{
	PGLViewerApplication3::init();
}

void endViewer()
{
	ViewerApplication3::exit();
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


