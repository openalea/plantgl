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



#include <plantgl/scenegraph/core/sceneobject.h>
#include <plantgl/scenegraph/core/action.h>

#include <plantgl/python/export_refcountptr.h>

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;

DEF_POINTEE(SceneObject)



std::string get_sco_name(SceneObject * obj){ return obj->getName(); }
void set_sco_name(SceneObject * obj, std::string v){ obj->setName(v); }

bool has_refcountlistener( const RefCountObject * a )
{  return a->getRefCountListener() != NULL; }



void export_SceneObject()
{
  class_< RefCountObject,RefCountObjectPtr, boost::noncopyable >("PglObject", no_init)
    .def("getPglReferenceCount",&RefCountObject::use_count)
    .def("getPglId",&RefCountObject::uid) \
    .def("__hasPythonRefCountLink",&has_refcountlistener) \
    .def("__removePglReference",&RefCountObject::removeReference) \
    .def("__addPglReference",&RefCountObject::addReference) \
    ;

  class_< SceneObject,SceneObjectPtr, bases< RefCountObject > , boost::noncopyable >(
      "SceneObject",
      "Abstract base class for all objects of the scenegraph.\n"
      "It is named, has unique id and support reference counting.\n"
      "It can support Action application.",
      no_init)
    // .def("__del__",&pydel<SceneObject>)
    .def("getName", &SceneObject::getName, return_value_policy< copy_const_reference >())
    .def("isNamed", &SceneObject::isNamed)
    .def("setName", &SceneObject::setName)
    .add_property("name",get_sco_name,&SceneObject::setName)
    .def("isValid", &SceneObject::isValid)
    .def("apply", &SceneObject::apply)
    .def("getId", &SceneObject::getId)
    .enable_pickling()
    ;

  implicitly_convertible<SceneObjectPtr, RefCountObjectPtr >();
}

