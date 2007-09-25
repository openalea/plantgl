/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
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

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/make_constructor.hpp>

#include <plantgl/scenegraph/scene/factory.h>
#include <string>
#include <sstream>
#include <qthread.h>

#include "../util/export_refcountptr.h"
#include "../util/export_property.h"
#include "../util/exception.h"
#include "../util/export_list.h"
#include "../util/extract_list.h"
#include "../util/list_converter.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(SceneFactory)


std::string get_scodec_name(SceneCodec * codec) {
        return codec->getName(); 
}

static QThread * PythonThread = NULL;

struct PythonInterpreterAcquirer {
public:

    PythonInterpreterAcquirer() 
    { 
        multiple_thread =  (QThread::currentThread() != PythonThread );
        if(multiple_thread) gstate = PyGILState_Ensure(); 
    }
    ~PythonInterpreterAcquirer()
    { 
        if(multiple_thread) PyGILState_Release(gstate); 
    }

    bool multiple_thread;
protected:
    PyGILState_STATE gstate;
};

class PySceneCodec : public SceneCodec, public boost::python::wrapper<SceneCodec>
{
public:
    PySceneCodec(const std::string& name = "", Mode mode = None) : 
      SceneCodec(name,mode), wrapper<SceneCodec>() 
      { if (PythonThread == NULL) PythonThread = QThread::currentThread(); }

    virtual SceneFormatList formats() const
    { 
        PythonInterpreterAcquirer py;
        return call<SceneFormatList>(this->get_override("formats").ptr()); 
    }

    bool default_test(const std::string& fname, Mode openingMode)
    {  return SceneCodec::test(fname,openingMode); }
	virtual bool test(const std::string& fname, Mode openingMode)
    {
        PythonInterpreterAcquirer py;
        if (override func = this->get_override("test"))
            return call<bool>(func.ptr(),object(fname),object(openingMode)); 
        return default_test(fname,openingMode);
    }

	ScenePtr default_read(const std::string& fname) 
    { return SceneCodec::read(fname); }
	virtual ScenePtr read(const std::string& fname)
    {
        {
            PythonInterpreterAcquirer py;
            if (override func = this->get_override("read"))
                return call<ScenePtr>(func.ptr(),object(fname)); 
        }
        return default_read(fname);
    }

	void default_write(const std::string& fname,const ScenePtr&	scene) 
    { SceneCodec::write(fname,scene); }
	virtual void write(const std::string& fname,const ScenePtr&	scene)
    {
        {
            PythonInterpreterAcquirer py;
            if (override func = this->get_override("write"))
                return call<void>(func.ptr(),object(fname),object(scene)); 
        }
        return default_write(fname,scene);
    }
protected:
    SceneFormatList __formats;
};

typedef RCPtr<PySceneCodec> PySceneCodecPtr;
DEF_POINTEE(PySceneCodec)

boost::python::object scformat_get_suffixes(SceneFormat * sf){
    return make_list<std::vector<std::string> >(sf->suffixes)(); 
}

void scformat_set_suffixes(SceneFormat * sf, boost::python::object suf){
    sf->suffixes = extract_vec<std::string>(suf)();
}

std::string scformat_repr(SceneFormat * sf){
    stringstream ss;
    ss << "<SceneFormat '" << sf->name << "' for files [";
    if (!sf->suffixes.empty()){
        ss << "'" << sf->suffixes[0] << "'";
        for(std::vector<std::string>::const_iterator it = sf->suffixes.begin() +1;
            it != sf->suffixes.end(); ++it)
                ss << ",'" << *it << "'";
    }
    ss << "] : " << sf->comment << ">";
    return ss.str();
}

SceneFormat * make_sformat(const std::string& name, boost::python::list suffixes, const std::string& comment = "")
{
    SceneFormat * sf = new SceneFormat();
    sf->name = name;
    sf->comment = comment;
    sf->suffixes = extract_vec<std::string>(object(suffixes))();
    return sf;
}

SceneFormatList * make_sformatlist(boost::python::list formats)
{ return new SceneFormatList(extract_vec<SceneFormat>(formats)()); }

void export_SceneCodec()
{
  class_<SceneFormat>("SceneFormat", no_init)
      .def( "__init__", make_constructor( make_sformat ) ) 
      .add_property("name",make_getter(&SceneFormat::name),make_setter(&SceneFormat::name))
      .add_property("comment",make_getter(&SceneFormat::comment),make_setter(&SceneFormat::comment))
      .add_property("suffixes",&scformat_get_suffixes,&scformat_get_suffixes)
      .def("__repr__",&scformat_repr)
      ;  

  class_<std::vector<SceneFormat> >("SceneFormatList")
    .def( "__init__", make_constructor( make_sformatlist ) ) 
    .def(vector_indexing_suite<std::vector<SceneFormat> >())
    ;
  pgllist_from_list <std::vector<SceneFormat> >();


  scope codec = class_< PySceneCodec,PySceneCodecPtr,boost::noncopyable >("SceneCodec",
        init<optional<const std::string&,SceneCodec::Mode> >("SceneCodec([name,mode])",args("name","mode")))
    .def("formats",pure_virtual(&SceneCodec::formats))
    .def("test", &SceneCodec::test,&PySceneCodec::default_test)
    .def("read", &SceneCodec::read,&PySceneCodec::default_read)
    .def("write", &SceneCodec::write,&PySceneCodec::default_write)
    .add_property("name",&get_scodec_name,&SceneCodec::setName)
    .add_property("mode",&SceneCodec::getMode,&SceneCodec::setMode)
    ;

  implicitly_convertible<PySceneCodecPtr, SceneCodecPtr>();

  enum_<SceneCodec::Mode>("Mode")
      .value("No",SceneCodec::None)
      .value("Read",SceneCodec::Read)
      .value("Write",SceneCodec::Write)
      .value("ReadWrite",SceneCodec::ReadWrite)
      ;
}


SceneFormatList sf_formats( SceneFactory * f) {
    return f->formats();
}

void export_SceneFactory()
{
  class_<SceneFactory,SceneFactoryPtr, boost::noncopyable>("SceneFactory",no_init)
      .def("get", &SceneFactory::get,return_value_policy<reference_existing_object>())
      .staticmethod("get") 
      .def("formats", &SceneFactory::formats)
      .def("formats", &sf_formats)
      .def("registerCodec", &SceneFactory::registerCodec)
      .def("unregisterCodec", &SceneFactory::unregisterCodec)
      .def("isReadable", &SceneFactory::isReadable)
      .def("isWritable", &SceneFactory::isWritable)
      .def("read", (ScenePtr(SceneFactory::*)(const std::string&))&SceneFactory::read)
      .def("read", (ScenePtr(SceneFactory::*)(const std::string&,const std::string&))&SceneFactory::read)
      .def("write", (void(SceneFactory::*)(const std::string&,const ScenePtr&))&SceneFactory::write)
      .def("write", (void(SceneFactory::*)(const std::string&,const ScenePtr&,const std::string&))&SceneFactory::write)
  ;

}


