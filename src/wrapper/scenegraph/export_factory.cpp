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




#include <plantgl/scenegraph/scene/factory.h>
#include <string>
#include <sstream>

#include <plantgl/python/pyobj_reference.h>
#include <plantgl/python/pyinterpreter.h>

BOOST_INITIALIZE_WRAPPER_FIX_DECLARE(PySceneCodec)

#include <plantgl/python/export_refcountptr.h>
#include <plantgl/python/export_property.h>
#include <plantgl/python/exception.h>
#include <plantgl/python/export_list.h>
#include <plantgl/python/extract_list.h>
#include <plantgl/python/list_converter.h>

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/make_constructor.hpp>

// PGL_USING_NAMESPACE
// PGL_USING_NAMESPACE

// using namespace std;
#define bp boost::python
PGL_USING(SceneFormat)
PGL_USING(SceneFormatList)
PGL_USING(SceneCodec)
PGL_USING(SceneCodecPtr)
PGL_USING(SceneFactory)
PGL_USING(SceneFactoryPtr)
PGL_USING(ScenePtr)

DEF_POINTEE(SceneFactory)


std::string get_scodec_name(SceneCodec * codec) {
        return codec->getName();
}


class PySceneCodec : public SceneCodec, public bp::wrapper<SceneCodec>
{
public:
    PySceneCodec(const std::string& name = "", Mode mode = Mode::Nothing) :
     SceneCodec(name,mode), bp::wrapper<SceneCodec>()
      {  }

    virtual SceneFormatList formats() const
    {
        PythonInterpreterAcquirer py;
        try{
            return bp::call<SceneFormatList>(this->get_override("formats").ptr());
        }
        catch(bp::error_already_set) { PyErr_Print(); }
        return SceneFormatList();
    }

    bool default_test(const std::string& fname, Mode openingMode)
    {  return SceneCodec::test(fname,openingMode); }
    virtual bool test(const std::string& fname, Mode openingMode)
    {
        PythonInterpreterAcquirer py;
        if (bp::override func = this->get_override("test")){
            try{
                return bp::call<bool>(func.ptr(),bp::object(fname),bp::object(openingMode));
            }
            catch(bp::error_already_set) { PyErr_Print(); }
            return false;
        }
        return default_test(fname,openingMode);
    }

    ScenePtr default_read(const std::string& fname)
    { return SceneCodec::read(fname); }
    virtual ScenePtr read(const std::string& fname)
    {
        {
            PythonInterpreterAcquirer py;
            if (bp::override func = this->get_override("read")){
                try{
                    return bp::call<ScenePtr>(func.ptr(),bp::object(fname));
                }
                catch(bp::error_already_set) { PyErr_Print(); }
                return ScenePtr();
            }
        }
        return default_read(fname);
    }

    bool default_write(const std::string& fname,const ScenePtr& scene)
    { return SceneCodec::write(fname,scene); }
    virtual bool write(const std::string& fname,const ScenePtr& scene)
    {
        {
            PythonInterpreterAcquirer py;
            if (bp::override func = this->get_override("write")){
                try{
                     boost::python::object res = bp::call<boost::python::object>(func.ptr(),bp::object(fname),bp::object(scene));
                     if (res == boost::python::object()) return true;
                     return boost::python::extract<bool>(res)();
                }
                catch(bp::error_already_set) { PyErr_Print(); return false; }
            }
        }
        return default_write(fname,scene);
    }
protected:
    SceneFormatList __formats;
};


typedef RCPtr<PySceneCodec> PySceneCodecPtr;
DEF_POINTEE(PySceneCodec)
BOOST_INITIALIZE_WRAPPER_FIX(PySceneCodec)

bp::object scformat_get_suffixes(SceneFormat * sf){
    return make_list(sf->suffixes)();
}

void scformat_set_suffixes(SceneFormat * sf, bp::object suf){
    sf->suffixes = extract_vec<std::string>(suf)();
}

std::string scformat_repr(SceneFormat * sf){
    std::stringstream ss;
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

SceneFormat * make_sformat(const std::string& name, bp::list suffixes, const std::string& comment = "")
{
    SceneFormat * sf = new SceneFormat();
    sf->name = name;
    sf->comment = comment;
    sf->suffixes = extract_vec<std::string>(bp::object(suffixes))();
    return sf;
}

SceneFormatList * make_sformatlist(bp::list formats)
{ return new SceneFormatList(extract_vec<SceneFormat>(formats)()); }

void pydel_scenecodec (PySceneCodec * c) {
#ifdef MAINTAIN_PYTHON_OBJECT_ID
    boost::intrusive_ptr_clear_pyobject(c);
#endif
    SceneFactory::get().unregisterCodec(SceneCodecPtr(c));
}



void export_SceneCodec()
{
  bp::class_<SceneFormat>("SceneFormat", "A scene description format.", bp::no_init)
      .def( "__init__", bp::make_constructor( make_sformat ) )
      .def_readwrite("name",&SceneFormat::name)
      .def_readwrite("comment",&SceneFormat::comment)
      .add_property("suffixes",&scformat_get_suffixes,&scformat_get_suffixes)
      .def("__repr__",&scformat_repr)
      ;

  bp::class_<std::vector<SceneFormat> >("SceneFormatList", "A list of scene description format.")
    .def( "__init__", bp::make_constructor( make_sformatlist ) )
    .def(bp::vector_indexing_suite<std::vector<SceneFormat> >())
    ;
  pgllist_from_list <std::vector<SceneFormat> >();


  bp::scope codec = bp::class_< PySceneCodec,PySceneCodecPtr,boost::noncopyable >("SceneCodec",
        "Coder/Decoder of a scene description.",
        bp::init<bp::optional<const std::string&,SceneCodec::Mode> >("SceneCodec([name,mode])",bp::args("name","mode")))
    .def("__del__",&pydel_scenecodec)
    .def("formats",bp::pure_virtual(&SceneCodec::formats))
    .def("test", &SceneCodec::test,&PySceneCodec::default_test)
    .def("read", &SceneCodec::read,&PySceneCodec::default_read)
    .def("write", &SceneCodec::write,&PySceneCodec::default_write)
    .add_property("name",&get_scodec_name,&SceneCodec::setName)
    .add_property("mode",&SceneCodec::getMode,&SceneCodec::setMode)
    ;

  bp::implicitly_convertible<PySceneCodecPtr, SceneCodecPtr>();

  bp::enum_<SceneCodec::Mode>("Mode"
#if BOOST_VERSION >= 103500
      ,"Enum representing coding and decoding capabilities of a codec."
#endif
      )
      .value("No",SceneCodec::Nothing)
      .value("Read",SceneCodec::Read)
      .value("Write",SceneCodec::Write)
      .value("ReadWrite",SceneCodec::ReadWrite)
      .export_values()
      ;
}


SceneFormatList sf_formats( SceneFactory * f) {
    return f->formats();
}

void export_SceneFactory()
{

  bp::class_<SceneFactory,SceneFactoryPtr, boost::noncopyable>("SceneFactory","A factory of Scene that register and use SceneCodec to read scene from files.",bp::no_init)
      .def("get", &SceneFactory::get,bp::return_value_policy<bp::reference_existing_object>())
      .staticmethod("get")
      .def("formats", &SceneFactory::formats)
      .def("formats", &sf_formats)
      .def("registerCodec", &SceneFactory::registerCodec)
      .def("unregisterCodec", &SceneFactory::unregisterCodec)
      .def("isReadable", &SceneFactory::isReadable)
      .def("isWritable", &SceneFactory::isWritable)
      .def("read", (ScenePtr(SceneFactory::*)(const std::string&))&SceneFactory::read)
      .def("read", (ScenePtr(SceneFactory::*)(const std::string&,const std::string&))&SceneFactory::read)
      .def("write", (bool(SceneFactory::*)(const std::string&,const ScenePtr&))&SceneFactory::write)
      .def("write", (bool(SceneFactory::*)(const std::string&,const ScenePtr&,const std::string&))&SceneFactory::write)
  ;

}


